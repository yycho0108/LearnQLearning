#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <vector>
#define ROW (3)
#define COL (4)
#define MAX_ITER (200)

extern double GAMMA;
typedef std::vector<std::vector<double>> Grid_Type;
typedef std::vector<std::vector<bool>> Topology_Type;
int toID(int,int);
std::pair<int,int> fromID(int);
#endif
