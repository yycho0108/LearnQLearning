#include "Utility.h"
#include "State.h"
#include "Q.h"
const double GridWorld[ROW][COL] = {
	{.0,.0,.0,1.0},
	{.0,-1.0,.0,-1.0},
	{.0,.0,.0,.0}
};
const bool Topology[ROW*COL][ROW*COL] = {
	{0,1,0,0,0,0,0,0,0,0,0,0},
	{1,0,1,0,0,1,0,0,0,0,0,0},
	{0,1,0,1,0,0,1,0,0,0,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0},
	{1,0,0,0,0,0,0,0,1,0,0,0}, //cannot visit 5
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,0,0,1,0,0,1,0},
	{0,0,0,1,0,0,1,0,0,0,0,1},
	{0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,1,0,1,0},
	{0,0,0,0,0,0,1,0,0,1,0,1},
	{0,0,0,0,0,0,0,1,0,0,1,0}
};

int main(){
	Grid_Type gridWorld;
	for(int i=0;i<ROW;++i){
		gridWorld.push_back(std::vector<double>());
		auto& row = gridWorld.back();
		for(int j=0;j<COL;++j){
			row.push_back(GridWorld[i][j]);
		}
	}
	State::set(gridWorld,ROW,COL);
	
	Topology_Type topology;
	for(int i=0;i<ROW*COL;++i){
		topology.push_back(std::vector<bool>());
		auto& row = topology.back();
		for(int j=0;j<ROW*COL;++j){
			row.push_back(Topology[i][j]);
		}
	}
	State::set(topology,ROW*COL,ROW*COL);

	Q q;
	q.set(topology,ROW*COL,ROW*COL);
	for(int i=0;i<MAX_ITER;++i){
#ifdef VERBOSE
		q.print();
#endif
		q.advance();
	}
	q.print();
}
