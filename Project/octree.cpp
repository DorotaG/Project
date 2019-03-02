#include "octree.h"

octree::octree()
{
	this->_np = 0;
	this->_center = surfel();
	this->_surfels;
}

octree::~octree()
{
	delete[] _surfels;
}

bool octree::build(std::vector<surfel> surfels, unsigned int np, unsigned int threshold, surfel center, int maxDepth, int currentDepth)
{
	_center = center;
	//Leaf if np <= threshold or depth >= maxDepth
	if (np <= threshold || currentDepth >= maxDepth)
	{
		//store the points in the node, making it a leaf node
		_np = np;
		_surfels = new surfel[np];
		memcpy(_surfels, surfels.data(), sizeof(surfel)*np );
		return true;
	}

	//counter: how many points assigned to a child 
	unsigned int counterChildSurfels[8];//are there 0?
	for (int i = 0; i < 8; i++)
	{
		counterChildSurfels[i] = 0;
	}

	//if not leaf compute center, create child nodes and classify points to them
	for (int i = 0; i < np; i++)//for every point
	{		
		surfels[i].setCode(0);
		if (surfels[i].getX() > _center.getX()) surfels[i].setCode(surfels[i].getCode() | 1);
		if (surfels[i].getY() > _center.getY()) surfels[i].setCode(surfels[i].getCode() | 2);
		if (surfels[i].getZ() > _center.getZ()) surfels[i].setCode(surfels[i].getCode() | 4);

		counterChildSurfels[surfels[i].getCode()]++;
	}
	//build each of 8 children
	for (int i = 0; i < 8; i++)
	{
		//If there aren't any points there just continue
		if (counterChildSurfels[i] == 0) continue;

		_child[i] = new octree;

		std::vector<surfel> newSurfels;
		int newCount = 0;

		for (surfel s: surfels)
		{
			if (s.getCode() == i)
			{
				//copy
				newSurfels.push_back(s);//memory leak, maybe?
				newCount++;
			}
		}

		//calculate center
		surfel newCenter;
		
			float sumX = 0, sumY = 0, sumZ = 0;
			for (surfel s : newSurfels)
			{
				sumX += s.getX();
				sumY += s.getY();
				sumZ += s.getZ();
			}
			sumX /= newCount;
			sumY /= newCount;
			sumZ /= newCount;
			newCenter = surfel(sumX, sumY, sumZ);//calculate other averages COLOR, NORMALS!

		// Recurse
		_child[i]->build(newSurfels, newCount, threshold, newCenter, maxDepth, currentDepth+1);

		// Clean up
		newSurfels.clear();
	}
	return true;
}

int octree::getNp()
{
	return _np;
}

octree* octree::getChild(int i)
{
	return this->_child[i];
}

surfel* octree::getSurfels()
{
	return this->_surfels;
}
surfel octree::getCenter()
{
	return this->_center;
}
bool octree::getDiscovered()
{
	return this->discovered;
}
void octree::labelDiscovered(bool label)
{
	this->discovered = label;
}