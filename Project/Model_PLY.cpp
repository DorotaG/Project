#include "Model_PLY.h"

//http://openglsamples.sourceforge.net/files/glut_ply.cpp

Model_PLY::Model_PLY()
{

}


float* Model_PLY::calculateNormal(float *coord1, float *coord2, float *coord3)
{
	/* calculate Vector1 and Vector2 */
	float va[3], vb[3], vr[3], val;
	va[0] = coord1[0] - coord2[0];
	va[1] = coord1[1] - coord2[1];
	va[2] = coord1[2] - coord2[2];

	vb[0] = coord1[0] - coord3[0];
	vb[1] = coord1[1] - coord3[1];
	vb[2] = coord1[2] - coord3[2];

	/* cross product */
	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	/* normalization factor */
	val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

	float norm[3];
	norm[0] = vr[0] / val;
	norm[1] = vr[1] / val;
	norm[2] = vr[2] / val;


	return norm;
}



int Model_PLY::Load(char* filename)
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedQuads = 0;
	this->TotalConnectedPoints = 0;

	char* pch = strstr(filename, ".ply");

	if (pch != NULL)
	{
		FILE* file = fopen(filename, "r");

		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);

		try
		{
			Vertex_Buffer = (float*)malloc(ftell(file));
		}
		catch (char*)
		{
			return -1;
		}
		if (Vertex_Buffer == NULL) return -1;
		fseek(file, 0, SEEK_SET);

		Faces_Triangles = (float*)malloc(fileSize * sizeof(float));
		Normals = (float*)malloc(fileSize * sizeof(float));

		if (file)
		{
			int i = 0;
			int temp = 0;
			int quads_index = 0;
			int triangle_index = 0;
			int normal_index = 0;
			char buffer[1000];


			fgets(buffer, 300, file);			// ply


			// READ HEADER
			// -------------

			// Find number of vertecies
			// -------------------------
			while (strncmp("element vertex", buffer, strlen("element vertex")) != 0)
			{
				fgets(buffer, 300, file);			// format
			}
			strcpy(buffer, buffer + strlen("element vertex"));
			sscanf(buffer, "%i", &this->TotalConnectedPoints);

			fgets(buffer, 300, file);
			//find all vertex properties: position, farbe... normale(?)
			int prop_count = 0; // if =3 only position, if =6 also color... normals(?) 
			while (strncmp("property", buffer, strlen("property")) == 0)
			{
				prop_count++;
				fgets(buffer, 300, file);
			}

			// go to end_header
			// -----------------
			while (strncmp("end_header", buffer, strlen("end_header")) != 0)
			{
				fgets(buffer, 300, file);			// format
			}

			// read verteces
			// -------------
			surfel temporarySurf = surfel();
			float x = 0;
			float y = 0;
			float z = 0;
			int r, g, b = 0;
			float u, v, w = 0;
			for (int iterator = 0; iterator < this->TotalConnectedPoints; iterator++)
			{
				fgets(buffer, 300, file);
				switch (prop_count)
				{
				case 0:
					break;
				case 3://position
					sscanf(buffer, "%f %f %f", &x, &y, &z);
					surfels.push_back(surfel(x, y, z));
					continue;
				case 6://color
					sscanf(buffer, "%f %f %f %i %i %i", &x, &y, &z, &r, &g, &b);//COLORS
					surfels.push_back(surfel(x, y, z, (float)r/255.0, (float)g/255.0, (float)b/255.0));
					continue;
				case 9://normals
					sscanf(buffer, "%f %f %f %f %f %f %i %i %i", &x, &y, &z, &u, &v, &w, &r, &g, &b);//NORMALS
					surfels.push_back(surfel(x, y, z, (float)r / 255.0, (float)g / 255.0, (float)b / 255.0, u, v, w));
					continue;
				default:
					break;
				}
			}


			fclose(file);
		}

		else { printf("File can't be opened\n"); }
	}
	else {
		printf("File does not have a .PLY extension. ");
	}
	return 0;
}