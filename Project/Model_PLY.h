/* Demonstrates how to load PLY files
 * Needs some refactoring.
 *
 * Model needs to be triangulated
 * Use blender
 * http://openglsamples.sourceforge.net/files/glut_ply.cpp
 * Just the class for loading PLY files.
 *
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <string>
#include <vector>
#include "surfel.h"

using namespace std;
class Model_PLY
{
public:
	int Load(char *filename);
	float* calculateNormal(float *coord1, float *coord2, float *coord3);
	Model_PLY();

	float* Faces_Triangles;
	float* Faces_Quads;
	float* Vertex_Buffer;
	int* Color_Buffer;
	float* Normals;
	vector<surfel> surfels;

	int TotalConnectedTriangles;
	int TotalConnectedQuads;
	int TotalConnectedPoints;
	int TotalFaces;


};