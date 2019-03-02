#include <queue>
#include <math.h>
#include <numeric>
#include <vector>
#include "octree.h"
#include "surfel.h"

class lod
{
public:
	std::vector<surfel> surfels;
	lod(int level, octree root);
	~lod();

private:
	std::vector<octree> centers;
};
