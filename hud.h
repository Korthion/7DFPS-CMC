#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
using namespace std;


class HUD
	{

	public:

	void outputText(float x, float y, string text, int font);
	void crossair(bool zoomedIn);
	void health();	
	void image(GLuint texture, float x, float y, float x2, float y2, bool zoomedIn);

	GLuint LoadTexture( const char * filename, int width, int height, GLfloat param );
	char * image_array;
	GLuint texture;
    };