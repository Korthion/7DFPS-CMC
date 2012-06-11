#include "__PreCompile.h"
#include "bullet.h"

struct vertex 
	{
	float x;
	float y;
	float z;
	};

bool collision3D2(float x, float y, float z, float x1, float y1, float z1, float x2, float y2, float z2)
{
 if ((x>=x1)&&(x<=x2)&&(z>=z1)&&(z<=z2)&&(y>=y1)&&(y<=y2))
	 {return true;}
 
 return false;
}

bullet::bullet()
	{
	stopped = false;
	exists = false;
	stuck_target = false;
	speed = 6;	
	default_accuracy = 0.0055;
	}


float RandomFloat(float min, float max)
{  
    float random = ((float) rand()) / (float) RAND_MAX;
    float range = max - min;  
    return (random*range) + min;
}

void bullet::setCoords(float x, float y, float z, float yrotrad, 
	                   float xrotrad, float accuracy, string weap, 
					   bool tracers, bool iswalking, bool zoomedIn)
{    
   if (iswalking == true)   // If the player is walking, accuracy will suffer
	   {
	   accuracy = accuracy * 2;
	   }
   weapon = weap;
   exists = true;
   xpos = x;
   zpos = z;
   ypos = y;

   if(weap=="SV-98")    // The sniper rifle is more accurate
   {
    default_accuracy = 0.0015;
   }

   if(weap=="Glock G18")    
   {
    default_accuracy = 0.004;
   }

   this -> tracers=tracers;
   this -> yrotrad = yrotrad + RandomFloat(-default_accuracy - accuracy,default_accuracy + accuracy); //sets the bullet dispersion
   this -> xrotrad = xrotrad + RandomFloat(-default_accuracy - accuracy,default_accuracy + accuracy);
}


void bullet::prediction(float x1, float y1, float z1, float x2, float y2, float z2, float target_x)
{
for(int i=0;i<=122;i++)
	{
	while(stopped==false)
		{
			xpos += float(sin(yrotrad))*0.05;
	        zpos -= float(cos(yrotrad))*0.05;		
			ypos -= float(sin(xrotrad))*0.05;
			if(collision3D2(xpos,ypos,zpos,x1,y1,z1,x2,y2,z2)==true)
				{
				 stopped = true;
				 stuck_target = true;
				}
		}
	}
}

void bullet::WallsPrediction()
{
for(int i=0;i<=122;i++)
	{
	while(stopped==false)
		{
			xpos += float(sin(yrotrad))*0.05;
	        zpos -= float(cos(yrotrad))*0.05;		
			ypos -= float(sin(xrotrad))*0.05;
			if(((xpos>=200)||(xpos<=-200)||(zpos>=200)||(zpos<=-200))&&(ypos>=0)&&(ypos<=18))
				{
				 stopped=true;
				}			
		}
	}
}

void bullet::GroundPrediction()
{
for(int i=0;i<=122;i++)
	{
	while(stopped==false)
		{
			xpos += float(sin(yrotrad))*0.05;
	        zpos -= float(cos(yrotrad))*0.05;		
			ypos -= float(sin(xrotrad))*0.05;
			if(ypos<=0)
				{
				 stopped=true;
				}			
		}
	}
}

void bullet::draw(float target_x)
{   
    glPushMatrix();	
	//glDisable(GL_LIGHTING);			
	
	glColor3f(0,0,0);

	if(stuck_target == true)  // Sticks the bullet to the movable target
		{
		glTranslated(target_x, ypos, zpos);	
		}
	else
		{
		glTranslated(xpos, ypos, zpos);
		}
	if(weapon=="M249")
		{
		glutSolidSphere(0.07,11,10);
		}
	else if (weapon=="Glock G18")
		{
		glutSolidSphere(0.04,11,10);
		}
	else if (weapon=="SV-98")
		{
		glutSolidSphere(0.09,20,20);
		}

    glPopMatrix();	

	if (stopped==false)
	{	
	 if(tracers==true)  // Tracer rounds are fired
		 {
		  GLfloat LightPosition[] = {xpos, ypos, zpos, 1}; 
		  GLfloat intensity[] = {0.65, 0.1, 0};
		  GLfloat DiffuseLight[] = {1, 1, 1}; 
		  glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight); 

		  glLightfv (GL_LIGHT1, GL_POSITION, LightPosition);	
		  glLightfv (GL_LIGHT1, GL_CONSTANT_ATTENUATION,&intensity[0]);
		  glLightfv (GL_LIGHT1, GL_LINEAR_ATTENUATION,&intensity[1]);
		  glLightfv (GL_LIGHT1, GL_QUADRATIC_ATTENUATION,&intensity[2]);
		 }

	xpos += float(sin(yrotrad))*speed;
	zpos -= float(cos(yrotrad))*speed;		
	ypos -= float(sin(xrotrad))*speed;
	}
	
	
	if ((xpos>500)||(xpos<-500)||(zpos<-500)||(zpos>500))
	{
	exists=false;
	}

	
	if (weapon=="SV-98")  // Sniper rifle bullets suffers from lower downforce.  
		{
		 xrotrad=xrotrad+0.00015;
		}

	else
		{
		 xrotrad=xrotrad+0.0004;
		}
	
}


