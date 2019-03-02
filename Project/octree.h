#pragma once
#include <vector>
#include <iostream>
#include "surfel.h"

class octree
{
public:
	octree();
	~octree();
	bool build(std::vector<surfel> surfels, unsigned int np, unsigned int threshold, surfel center, int maxDepth, int currentDepth);
	int getNp();
	bool getDiscovered();
	void labelDiscovered(bool label);
	octree* getChild(int i);
	surfel* getSurfels();
	surfel getCenter();
	void print();

private:
	unsigned int _np; //number of points ==0 inside node >0 leaf node
	surfel _center;
	octree *_child[8];
	surfel* _surfels;
	bool discovered;
};

