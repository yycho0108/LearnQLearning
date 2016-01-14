
#include <random>
#include <ctime>
#include <functional>

double f(){
	static auto f = std::bind(std::uniform_real_distribution<double>(0.0,1.0), std::default_random_engine(time(0)));
	return f();
}
