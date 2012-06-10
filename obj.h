#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;

struct vertex 
{
	float x;
	float y;
	float z;
};

class Model
{
  public: 
	Model();			
    float* calculate_normals(float* coord1,float* coord2,float* coord3 );
    int Load(char* filename, float x, float y, float z);	// Loads the model with specified coordinates
	void Draw();					// Draws the model
	void Release();				

	float* normals;						
    float* triangle_faces;				
	float* vertex_buffer;				
	long total_connected_points;			
	long total_connected_triangles;		
	vertex position;
};