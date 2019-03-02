#include "grid3D.h"

grid3D::grid3D()
{
	this->_x = 1;
	this->_y = 1;
	this->_z = 1;
}

grid3D::grid3D(int x, int y, int z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

grid3D::~grid3D(){}

void grid3D:: calculateOffset(std::vector<surfel> surfels)
{
	float right, left, top, bottom, back, front;
	//calculate this ^
	surfel temp;
	//get first point and fill this^ with its coordinates
	temp = surfels.front();
	right = temp.getX();
	left = temp.getX();
	top = temp.getY();
	bottom = temp.getY();
	back = temp.getZ();
	front = temp.getZ();

	for (surfel s:surfels)
	{
		if (s.getX() > right)
			right = s.getX();
		if (s.getX() < left)
			left = s.getX();
		if (s.getY() > top)
			top = s.getY();
		if (s.getY() < bottom)
			bottom = s.getY();
		if (s.getZ() > back)
			back = s.getZ();
		if (s.getZ() < front)
			front = s.getZ();
	}
	//calculate offset
	this->offsetX = abs(right - left) / _x;
	this->offsetY = abs(top - bottom) / _y;
	this->offsetZ = abs(back - front) / _z;
	this->leftX = left;
	this->bottomY = bottom;
	this->frontZ = front;
}

std::vector<surfel> grid3D::createNewSurfels(std::vector<surfel> surfels)
{
	std::vector<surfel> output;
	std::vector<surfel> temp;
	//initialize L, R, B, T, F, Ba
	float L = leftX;
	float R = L + offsetX;
	float B = bottomY;
	float T = B + offsetY;
	float F = frontZ;
	float Ba = F + offsetZ;

	for (int i = 0; i < _z; i++)
	{
		for (int j = 0; j < _y; j++)
		{
			for (int k = 0; k < _x; k++)
			{
				for (surfel s : surfels)
				{
					//check if surfel within cube
					if (s.getX() >= L && s.getX() <= R && s.getY() >= B && s.getY() <= T && s.getZ() >= F && s.getZ() <= Ba)
					{
						//add to temporary vector
						temp.push_back(s);
					}			
				}
				//create new surfel from temporary vector (NEW FUNCTION) and add it to our output vector
				if (!temp.empty())
				{
					output.push_back(createAveragedSurfel(temp));
				}
				temp.clear();

				//increment L and R
				L = R;
				R = L + offsetX;
			}
			//reset L and R
			L = leftX;
			R = L + offsetX;
			//increment B and T
			B = T;
			T = B + offsetY;
		}
		//reset B and T
		B = bottomY;
		T = B + offsetY;
		//increment F and Ba
		F = Ba;
		Ba = F + offsetZ;
	}

	return output;
}

surfel grid3D::createAveragedSurfel(std::vector<surfel> input)
{
	float x = input.front().getX();
	float y = input.front().getY();
	float z = input.front().getZ();
	float r = input.front().getR();
	float g = input.front().getG();
	float b = input.front().getB();
	input.pop_back();
	for (surfel s : input)
	{
		//average position
		x = (x + s.getX()) / 2;
		y = (y + s.getY()) / 2;
		z = (z + s.getZ()) / 2;
		//average color
		r = (r + s.getR()) / 2;
		g = (g + s.getG()) / 2;
		b = (b + s.getB()) / 2;
	}
	
	//return average surfel
	return surfel(x, y, z, r, g, b);
}
