#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

#include "surfel.h"

class grid3D
{
private:
	//number of cells in each direction
	int _x;
	int _y;
	int _z;
	float offsetX;
	float offsetY;
	float offsetZ;
	float leftX;
	float bottomY;
	float frontZ;
public:
	grid3D();
	grid3D(int x, int y, int z);
	~grid3D();
	void calculateOffset(std::vector<surfel> surfels);
	std::vector<surfel> createNewSurfels(std::vector<surfel> surfels);
	surfel createAveragedSurfel(std::vector<surfel> input);
};

