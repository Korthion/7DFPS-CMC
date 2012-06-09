#include "__PreCompile.h"
#include "obj.h" 
 
#define points_per_vertex 3
#define total_floats_in_triangle 9

Model::Model()
{
total_connected_triangles = 0; 
total_connected_points = 0;
}
 
float* Model::calculate_normals( float *coord1, float *coord2, float *coord3 )
{
   // calculate vectors
   float va[3], vb[3], vr[3], val;

   va[0] = coord1[0] - coord2[0];
   va[1] = coord1[1] - coord2[1];
   va[2] = coord1[2] - coord2[2];
   vb[0] = coord1[0] - coord3[0];
   vb[1] = coord1[1] - coord3[1];
   vb[2] = coord1[2] - coord3[2];
 
   // cross product 
   vr[0] = va[1] * vb[2] - vb[1] * va[2];
   vr[1] = vb[0] * va[2] - va[0] * vb[2];
   vr[2] = va[0] * vb[1] - vb[0] * va[1];
 
   // normalization 
   val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );
 
   float norm[3];
   norm[0] = vr[0]/val;
   norm[1] = vr[1]/val;
   norm[2] = vr[2]/val; 
 
   return norm; //returns normal
}
  
int Model::Load(char* filename, float x, float y, float z)
{
string line;
ifstream objFile (filename);	
if (objFile.is_open())													
{
	objFile.seekg (0, ios::end);										// go to end of file
	long fileSize = objFile.tellg();									
	objFile.seekg (0, ios::beg);										
	vertex_buffer = (float*) malloc (fileSize);							// memory for verteces
	triangle_faces = (float*) malloc(fileSize*sizeof(float));			// memory for triangles
	normals  = (float*) malloc(fileSize*sizeof(float));					// memory for normals

	int triangle_index = 0;												
	int normal_index = 0;												
 
	while (!objFile.eof())			
	{		
		getline (objFile,line);											
 
		if (line.c_str()[0] == 'v')										// The first character is 'v' = vertex
		{
			line[0] = ' ';												

            sscanf(line.c_str(),"%f %f %f",	&vertex_buffer[total_connected_points], &vertex_buffer[total_connected_points+1],	&vertex_buffer[total_connected_points+2]);
			vertex_buffer[total_connected_points] +=x;
			vertex_buffer[total_connected_points+1] +=y;
			vertex_buffer[total_connected_points+2] +=z;
			
			// Read floats from the line: v x/y/z

			total_connected_points += points_per_vertex;		// Add 3 to the total connected points
		}

		if (line.c_str()[0] == 'f')										// The first character is 'f' = point
		{
	    	line[0] = ' ';												

			int vertexNumber[4] = { 0, 0, 0 };
            sscanf(line.c_str(),"%i %i %i",								// Read integers from the line:  f 1 2 3
			&vertexNumber[0], &vertexNumber[1], &vertexNumber[2]);										

			vertexNumber[0] -= 1;										
			vertexNumber[1] -= 1;										
			vertexNumber[2] -= 1;				
  
			/*
			  Makes triangles using thr stored points 		  
			*/

			int tCounter = 0;
			for (int i = 0; i < points_per_vertex; i++)					
			{
				triangle_faces[triangle_index + tCounter    ] = vertex_buffer[3*vertexNumber[i]   ];
				triangle_faces[triangle_index + tCounter +1 ] = vertex_buffer[3*vertexNumber[i]+1 ];
				triangle_faces[triangle_index + tCounter +2 ] = vertex_buffer[3*vertexNumber[i]+2 ];
				tCounter += points_per_vertex;
			}

			/*
			  Calculates the normals for lightling 
			*/ 
				float coord1[3] = {triangle_faces[triangle_index],   triangle_faces[triangle_index+1], triangle_faces[triangle_index+2]};
				float coord2[3] = {triangle_faces[triangle_index+3], triangle_faces[triangle_index+4], triangle_faces[triangle_index+5]};
				float coord3[3] = {triangle_faces[triangle_index+6], triangle_faces[triangle_index+7], triangle_faces[triangle_index+8]};
				float *norm = calculate_normals( coord1, coord2, coord3 ); //normalization
 
				tCounter = 0;
				for (int i = 0; i < points_per_vertex; i++)
				{
				normals[normal_index + tCounter   ] = norm[0];
				normals[normal_index + tCounter +1] = norm[1];
				normals[normal_index + tCounter +2] = norm[2];
				tCounter += points_per_vertex;
				}
 
				triangle_index += total_floats_in_triangle;
				normal_index += total_floats_in_triangle;
				total_connected_triangles += total_floats_in_triangle;			
			}	
		}

		objFile.close();														
	}

	else 
	{
	//do nothing. Or output an error. Don't really care 
	}
	return 0;
}
 
//Called before exiting to free up memory
void Model::Release()
{
	delete(triangle_faces);
	delete(normals);
	delete(vertex_buffer);
}
 
void Model::Draw()
{
    glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal 
 	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex 

	glVertexPointer(3,GL_FLOAT,	0,triangle_faces);				// Vertex Pointer 
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer 
	glDrawArrays(GL_TRIANGLES, 0, total_connected_triangles);   // Draw triangles

	glDisableClientState(GL_VERTEX_ARRAY);						
	glDisableClientState(GL_NORMAL_ARRAY);							
}