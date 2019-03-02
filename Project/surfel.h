#pragma once
class surfel
{
public:
	struct normal
	{
		float u;
		float v;
		float w;
	};
	//constructors & destructor
	surfel();
	surfel(float x, float y, float z);
	surfel(float x, float y, float z, float r, float g, float b);
	surfel(float x, float y, float z, float r, float g, float b, float u, float v, float w);
	surfel(float x, float y, float z, float r, float g, float b, normal normals[6]);
	~surfel();
	//getters
	float getX();
	float getY();
	float getZ();
	float getR();
	float getG();
	float getB();
	surfel::normal getN();
	normal* getNormArr();
	int getCode();
	//setter
	void setCode(int c); //bitwise: 001 x+, 010 y+, 100 z+
	void setN(normal n);
	bool getNC();
	void setNC(bool b);
	
private:
	float _x;//position
	float _y;
	float _z;
	float _r;//color
	float _g;
	float _b;
	normal _n;//normal
	normal _normals[6];
	int _code;
	bool normalCone = false;
};

