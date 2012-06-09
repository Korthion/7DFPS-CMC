#include <stdio.h>
#include <windows.h>	   
#include <GL/gl.h>		 
#include "glut.h"
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;


class Model
{
  public: 
	Model();			
    float* calculate_normals(float* coord1,float* coord2,float* coord3 );
    int Load(char* filename, float x, float y, float z);	// Loads the model with specified coordinates
	void Draw();					// Draws the model
	void Release();				
 
	GLuint texture;

	float* normals;						
    float* triangle_faces;				
	float* vertex_buffer;				
	long total_connected_points;			
	long total_connected_triangles;		
 
};