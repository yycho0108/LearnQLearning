
#include <random>
#include <ctime>
#include <functional>
int dumb(){
	srand(time(0));
	return 0;
}
double f(){
	static int i = dumb();
	static auto f = std::bind(std::uniform_real_distribution<double>(0.0,1.0), std::default_random_engine(time(0)));
	return f();
}
