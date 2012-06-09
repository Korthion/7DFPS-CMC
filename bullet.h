#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;


class bullet
	{
	public:
	bullet();

	void setCoords(float x, float y, float z, float yrotrad, 
		           float xrotrad, float accuracy, string weap,
				   bool tracers, bool iswalking);
	void draw(float tartget_x);
	void prediction(float x1, float y1, float z1, float x2, float y2, float z2, float target_x);	
	void WallsPrediction();
	void GroundPrediction();
	float xpos,ypos,zpos;
	float speed, default_accuracy;
	bool exists, stopped, tracers, stuck_target;
	float yrotrad, xrotrad;		
	string weapon;	
    };

