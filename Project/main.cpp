#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>

#include "shader.h"
#include "Model_PLY.h"
#include "octree.h"
#include "Grid3D.h"
#include "lod.h"
#include "surfel.h"

#include <iostream>

//https://learnopengl.com/Introduction

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// lighting
glm::vec3 lightPos(0.0f, 0.3f, 0.0f);

int main()
{	
	//parse .ply files
	//------------------------------
	Model_PLY parser;
	const char* filename = "cowCN.ply";
	int test = parser.Load((char*)filename);//exceptions TODO 

	//LOD TEST USING 3DGRID
	//------------------------------
	//grid3D grid(1, 1, 1);
	//grid.calculateOffset(parser.surfels);
	//std::vector<surfel> testLOD = grid.createNewSurfels(parser.surfels);
	//std::vector<surfel> testLOD = parser.surfels;

	//LOD OCTREE
	//------------------------------
	octree root;
	bool ret = root.build(parser.surfels, parser.surfels.size(), 2, surfel(0,0,0), 4, 0);
	lod testLOD(4, root);
	testLOD.surfels.size();

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.5);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Level of Detail", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	// build and compile shaders
	// -------------------------
	Shader shader("VertexShader.vs", "FragmentShader.fs"/*, "GeometryShader.gs"*/);

	unsigned int VBO, VAO;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		// ---------------
		shader.use();
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightPos", lightPos);

		// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
		// -----------------------------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);

		// camera/view transformation
		// --------------------------
		glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("view", view);

		//calculate proper normal
		// ----------------------
		for (unsigned j = 0; j < testLOD.surfels.size(); j++)
		{
			//if there is set of normals 
			if (testLOD.surfels[j].getNC())
			{
				//calculate vector between surfel and camera
				float vx = camX - testLOD.surfels[j].getX();
				float vy = 0.0 - testLOD.surfels[j].getY();
				float vz = camZ - testLOD.surfels[j].getZ();
				//normalize it
				float length = std::sqrt(vx*vx + vy*vy + vz*vz);
				vx = vx / length;
				vy = vy / length;
				vz = vz / length;

				int minimum = vx* testLOD.surfels[j].getNormArr()[0].u + vy* testLOD.surfels[j].getNormArr()[0].v + vz* testLOD.surfels[j].getNormArr()[0].w;
				int temp;
				surfel::normal normToBe = testLOD.surfels[j].getNormArr()[0];

				for (int i = 1; i < 5; i++)
				{
					temp = vx * testLOD.surfels[j].getNormArr()[i].u + vy * testLOD.surfels[j].getNormArr()[i].v + vz * testLOD.surfels[j].getNormArr()[i].w;
					if (minimum > temp)
					{
						normToBe = testLOD.surfels[j].getNormArr()[i];
					}
				}
				//put that as normal
				testLOD.surfels[j].setN(normToBe);
			}
		}
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, parser.surfels.size() * sizeof(surfel), &parser.surfels.front(), GL_STATIC_DRAW);//parsed data only
		//glBufferData(GL_ARRAY_BUFFER, testLOD.size() * sizeof(surfel), &testLOD.front(), GL_STATIC_DRAW);//LOD 3D GRID
		glBufferData(GL_ARRAY_BUFFER, testLOD.surfels.size() * sizeof(surfel), &testLOD.surfels.front(), GL_STATIC_DRAW);//LOD Octree
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(surfel), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(surfel), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(surfel), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);

		// draw points
		// ------------
		glBindVertexArray(VAO);
		//glDrawArrays(GL_POINTS, 0, parser.surfels.size());//parsed data only
		//glDrawArrays(GL_POINTS, 0, testLOD.size()); //LOD 3D GRID
		glDrawArrays(GL_POINTS, 0, testLOD.surfels.size());//LOD Octree

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}