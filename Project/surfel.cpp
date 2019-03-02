#include "surfel.h"

surfel::surfel()
{
}

surfel::~surfel()
{
}

surfel::surfel(float x,float y, float z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_r = 0;
	this->_g = 0;
	this->_b = 0;
	this->_n.u = 0;
	this->_n.v = 0;
	this->_n.w = 0;
}
surfel::surfel(float x, float y, float z, float r, float g, float b)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_r = r;
	this->_g = g;
	this->_b = b;
	this->_n.u = 0;
	this->_n.v = 0;
	this->_n.w = 0;
}
surfel::surfel(float x, float y, float z, float r, float g, float b, float u, float v, float w)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_r = r;
	this->_g = g;
	this->_b = b;
	this->_n.u = u;
	this->_n.v = v;
	this->_n.w = w;
}
surfel::surfel(float x, float y, float z, float r, float g, float b, normal normals[6])
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_r = r;
	this->_g = g;
	this->_b = b;
	this->_n.u = 0;
	this->_n.v = 0;
	this->_n.w = 0;
	this->_normals[0] = normals[0];
	this->_normals[1] = normals[1];
	this->_normals[2] = normals[2];
	this->_normals[3] = normals[3];
	this->_normals[4] = normals[4];
	this->_normals[5] = normals[5];
}

float surfel::getX() { return this->_x; }
float surfel::getY() { return this->_y; }
float surfel::getZ() { return this->_z; }
float surfel::getR() { return this->_r; }
float surfel::getG() { return this->_g; }
float surfel::getB() { return this->_b; }
surfel::normal surfel::getN() { return this->_n; }
surfel::normal* surfel::getNormArr() { return this->_normals; }
bool surfel::getNC() { return this->normalCone; }

int surfel::getCode() { return this->_code; }
void surfel::setCode(int c) { this->_code = c; }
void surfel::setN(normal n) { this->_n = n; }
void surfel::setNC(bool b) { this->normalCone = b; }