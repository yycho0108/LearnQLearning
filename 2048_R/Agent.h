#ifndef __AGENT_H__
#define __AGENT_H__

#include <random>
#include <ctime>
#include <deque>
#include "ConvNet/ConvNet.h"
#include "Board.h"
#include "Utility.h"

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

	net.push_back(new ConvolutionLayer(1,3));
	net.push_back(new ActivationLayer("relu"));
	//net.push_back(new PoolLayer(Size(2,2),Size(2,2)));

	net.push_back(new ConvolutionLayer(3,6));
	net.push_back(new ActivationLayer("relu"));
	//net.push_back(new PoolLayer(Size(2,2),Size(2,2)));
		
	net.push_back(new FlattenLayer(6));
	net.push_back(new DenseLayer(1,H));
	net.push_back(new ActivationLayer("relu"));
	net.push_back(new DenseLayer(1,4));
	net.push_back(new ActivationLayer("sigmoid"));
	//net.push_back(new SoftMaxLayer());

	net.setup(Size(n,m));
}

template<int n, int m, typename T = double> //double/float = cannot use table, must use neural network
class Agent{
	using A_Memory = Memory<n,m,T>;
	using A_Board = Board<n,m>;
	static const int H = log2(ppow(10,n*m))/2;
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

	cv::Mat process(A_Board& board){
		//build input
		return process(board.board());
	}
	cv::Mat process(const char* board){
		auto b = cv::Mat(n,m,cv::DataType<char>::type,(char*)board);
		b.convertTo(b,cv::DataType<float>::type);
		double maxVal = *std::max_element(b.begin<float>(),b.end<float>());
		if (maxVal > 0)
			b /= maxVal;
		//cv::minMaxLoc( b, nullptr, &maxVal,nullptr,nullptr);
		//namedPrint(maxVal);
		//b /= 10.0; //normalize
		return b;
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

		cv::Mat q = net.FF(v);
		for(int a=0;a<4;++a){ 
			if(available[a]){ //among available actions
				//namedPrint(val);
				auto val = q.at<float>(a);
				if(val > maxVal){ //this is why R was favorite
					maxVal = val;
					maxDir = (DIR)a;
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
		cv::Mat q = net.FF(process(board.board()));
		for(int a=0;a<4;++a){
			if(available[a]){
				//auto nexti = process(board.speculate((DIR)a));
				//board.print();
				//namedPrint((DIR)a);
				//namedPrint(nexti);

				//auto val = getMax(nexti);
				auto val = q.at<float>(a);
				maxVal = maxVal>val?maxVal:val;
			}
		}
		return maxVal;
	}
	
	double getMax(cv::Mat& state){// this is for probabilistic calculation
		double maxVal = -99999;
		int empty=0;
		//namedPrint(state);
		for(int i=0;i<n;++i){
			for(int j=0;j<m;++j){
				auto& s = state.at<float>(i,j);
				if(s == 0){ //= if empty
					++empty;
					s = 1/10.0;///fill with 2, div. 1024.0 normalization;

					cv::Mat res = net.FF(state);
					for(int a=0;a<4;++a){
						//TODO: check if action is possible
						auto val = 0.9 * res.at<float>(a); //probability 0.9 of tile 2
						maxVal = maxVal > val ? maxVal : val;
					}

					s = 2/10.0;///fill with 4, div. 1024.0;
					res = net.FF(state);
					for(int a=0;a<4;++a){
						auto val = 0.1 * res.at<float>(a); //probability 0.1 of tile 4
						maxVal = maxVal > val ? maxVal : val;
					}

					s = 0; //back to 0
				}
			}
		}

		if(empty == 0)
			return 0.0; //terminal state maxqn
		else
			return maxVal/empty;
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
		//namedPrint(s);
		//auto& s = memory.s.vec(); 

		auto maxqn = getMax(memory.s_n);
		//namedPrint(SA);
		cv::Mat yp = net.FF(s); //old value
		cv::Mat y;
	   //	= yp.clone();//new value
	   	yp.copyTo(y);

		if(_verbose){
			hline();
			memory.s.print();
			namedPrint(y);
		}
		//auto oldy = y;
		//namedPrint(oldy);
		double old_q = y.at<float>((int)a);
		double new_q = r + gamma*maxqn;

		y.at<float>((int)a) = (1-alpha)*old_q + (alpha)*new_q;

		//std::cout << "<[[" <<std::endl;
		
		if(_verbose){
			namedPrint(r);
			namedPrint(maxqn);
			namedPrint(y)
		}	

		net.BP(yp, y);

		if(_verbose){
			cout << "--> " << endl;
			y = net.FF(s);
			namedPrint(y);
		}

		return 0.5 * (new_q - old_q) * (new_q - old_q);
	}
	double learn_bundle(double alpha){
		static std::random_device rd;
		static std::mt19937 eng(rd());
		static std::uniform_int_distribution<int> distr(0,mSize);
		double sum = 0;
		for(int i=0;i<rSize;++i){
			//potentially replace with distinct random numbers
			sum += learn(memories[distr(eng)], alpha);
			//sum += learn(memories[rand()%mSize],alpha);
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
