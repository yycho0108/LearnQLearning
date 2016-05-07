#ifndef __AGENT_H__
#define __AGENT_H__

#include <random>
#include <ctime>
#include <deque>
#include "tiny_cnn/tiny_cnn.h"
#include "Board.h"
#include "Table.h"
#include "Utility.h"

using namespace tiny_cnn;
using namespace tiny_cnn::activation;

using ConvNet = network<mse, gradient_descent_levenberg_marquardt>;

auto pick = std::uniform_real_distribution<double>();
std::default_random_engine rng(time(0));

constexpr long int ppow(int x, int n){ //positive power
	return n<1?1:x*ppow(x,n-1);
}
constexpr long int log2(int x){
	return (x<2)?0:1+log2(x>>1);
}

template<int n, int m, typename T=double>// n = state size
struct Memory{
	using A_Board = Board<n,m>;
	A_Board s;
	DIR a;
	double r;
	A_Board s_n;

	Memory(A_Board& s, DIR a, double r, A_Board& s_n)
	:s(s),a(a),r(r),s_n(s_n){

	}
};

void setup(ConvNet& net, int n, int m,int H){
	/* ** CONV LAYER TEST ** */
    net << convolutional_layer<tan_h>(n, m, 3, 1, 6) // 32x32 in, 5x5 kernel, 1-6 fmaps conv
       << convolutional_layer<tan_h>(14, 14, 3, 6, 16) // with connection-table
       << convolutional_layer<tan_h>(5, 5, 3, 16, H)
       << fully_connected_layer<tan_h>(H, 10);

	net.optimizer().alpha = 0.001;

}
template<int n, int m, typename T = double> //double/float = cannot use table, must use neural network
class Agent{
	using A_Memory = Memory<n,m,T>;
	using A_Board = Board<n,m>;
	static const int H = 4*log2(ppow(10,n*m));
private:
	ConvNet net;
	double gamma; // gamma = reduction to future /ewards
	double min_eps;
	std::deque<A_Memory> memories;
	int mSize; //memory size
	int rSize; //recall size = # samples to recall
	bool _verbose;
	//input = 4x4 = 16 states + 4 actions
	//output = Q-value
public:
	Agent(int mSize=1, double gamma=0.8, double min_eps=0.05) //size of memory
		:gamma(gamma),min_eps(min_eps),mSize(mSize),rSize(1>mSize/3?1:mSize/3)
		//rho, eps, decay
		//learning rate = 0.3, weight decay = 0.001
	{
		setup(net,n,m,H);
		_verbose = false;
		std::cout << H << std::endl;
		/*std::vector<double> v(n*m+4);
		for(int i=0;i<4;++i){
			v[n*m+i] = 1;
			auto val = net.FF(v);
			namedPrint(val);
			v[n*m+i] = 0;
		}*/

	}
	//Agent Saving/Loading (to/from file) ... To Be Added
	DIR getRand(A_Board& board){
		const bool* available = board.getAvailable();

		std::vector<DIR> av; //list of available actions
		for(int i=0;i<4;++i){
			if(available[i]){
				auto dir = (DIR)i;
				av.push_back(dir);
			}
		}
		auto p = pick(rng);

		return av[int(av.size()*p)];// if av.size()==3 then ranges from 0~2.9999, end-exclusive
	}	
	std::vector<double> process(A_Board& board){
		return process(board.board());
	}
	std::vector<double> process(const char* board){
		auto v = std::vector<double>((char*)board, (char*)board+n*m);
		for(auto& e : v){
			e /= 10.0;
		}
		return v;
		//auto b = cv::Mat(n,m,cv::DataType<char>::type,(char*)board);
		//b.convertTo(b,cv::DataType<float>::type);
		//b /= 10.0; //normalize
		//return b;
	}
	DIR getBest(A_Board& board){
		//get best purely based on network feedforward q-value
		auto v = process(board);
		//std::vector<double>& v= board.vec();
		//currently editing here
		double maxVal=-99999;
		DIR maxDir=X;

		const bool* available = board.getAvailable();
		//namedPrint(v);
		auto a = net.predict(v);
		//cv::Mat a = net.FF(v);
		for(int i=0;i<4;++i){ 
			if(available[i]){ //among available actions
				//namedPrint(val);
				auto val = a[i];
				if(val > maxVal){ //this is why R was favorite
					maxVal = val;
					maxDir = (DIR)i;
				}
			}
		}
		//namedPrint(maxDir);

		return maxDir;
	}

	double getMax(A_Board& board){
		//pick this function as this serves an entirely new purpose...ish.
		double maxVal = 0.0;
		const bool* available = board.getAvailable();
		auto vals = net.predict(board.board());
		//cv::Mat v = net.FF(process(board.board()));
		for(int i=0;i<4;++i){
			if(available[i]){
				//auto nexti = process(board.speculate((DIR)i));
				//board.print();
				//namedPrint((DIR)i);
				//namedPrint(nexti);

				//auto val = getMax(nexti);
				auto val = vals[i];
				maxVal = maxVal>val?maxVal:val;
			}
		}
		return maxVal;
	}
	
	DIR getNext(A_Board& board, double epsilon){
		epsilon = std::max(min_eps,epsilon); //0.05 = min_epsilon
		//namedPrint(epsilon);
		//occasional random exploration
		//0.9 corresponds to "gamma" .. ish.
		//e-greedy
		return (pick(rng) < epsilon)? getRand(board) : getBest(board);
	}
	double learn(A_Memory& memory, double alpha){
		const DIR a = memory.a;
		const double r = memory.r;
		auto s = process(memory.s);
		//auto& s = memory.s.vec(); 

		auto maxqn = getMax(memory.s_n);
		//namedPrint(SA);
		auto yp = net.predict(s);
		auto y = yp;

		if(_verbose){
			hline();
			memory.s.print();
			namedPrint(y);
		}
		//auto oldy = y;
		//namedPrint(oldy);
		y[(int)a] = (1-alpha)*y[(int)a] + alpha*(r+gamma*maxqn);

		//std::cout << "<[[" <<std::endl;
		
		if(_verbose){
			namedPrint(r);
			namedPrint(maxqn);
			namedPrint(y)
		}	
		net.train(s,y,1,1);

		if(_verbose){
			cout << "--> " << endl;
			y = net.predict(s);
			namedPrint(y);
		}

		return 0.0; //net.error();
	}
	double learn_bundle(double alpha){
		//static std::random_device rd;
		//static std::mt19937 eng(rd());
		//static std::uniform_int_distribution<int> distr(0,mSize);
		double sum = 0;
		for(int i=0;i<rSize;++i){
			//potentially replace with distinct random numbers
			//learn(memories[distr(eng)], alpha);
			sum += learn(memories[rand()%mSize],alpha);
		}
		return sum/rSize;
	}
	double learn_bundle(double alpha, int size){
		double sum = 0;
		for(int i=0;i<size;++i){
			sum += learn(memories[rand()%memories.size()],alpha);
		}
		return sum/size; //average
	}
	void memorize(A_Board& S, DIR a, double r, A_Board& next){
		//SARSA
		//State-Action, Reward, Max(next), alpha		
		memories.emplace_back(S, a, r,next);
		if(memories.size() > mSize){
			memories.pop_front();
		}
		//this->learn(memories.back(),alpha);

		//if(learn){
		//	if(memories.size() > mSize){
		//		memories.pop_front();
		//		learn_bundle(alpha);
		//	}else{
		//		auto size = std::min(rSize,(int)memories.size());
		//		learn_bundle(alpha,size);
		//	}
		//}
	}
	cv::Mat guess(A_Board& board){
		return net.FF(process(board));
	}
	void print(){
		//net.print();
	}
	bool& verbose(){
		return _verbose;
	}
};
#endif
