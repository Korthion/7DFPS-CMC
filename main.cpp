//TEST2
#include <stdio.h>
#include "hud.h"
#include "obj.h"
#include "bullet.h"
#include "SoundEngine.h"
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include "texture.h"
#include "ObjectInst.h"
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
using namespace std;
using namespace irrklang;

float lastx, lasty, xpos = 0, ypos = 2, zpos = 0, xrot = 0, yrot = 0, target_x = 170.9;
float g_rotation = 0, g_rotation_speed = 90.0f, accuracy = 0, recoil = 0;
const float PI = 3.141592654;
float positionz[50], positionx[50];
bool firstMouseButton = false;
int bulletcount = 0;
bool clearToShoot = true, zoomedIn = false, iswalking = false, tracers = false, bullet_time = false, fullscreen = true, glock_auto = false, show_hud = true;
int winW = 0, winH = 0, Sensitivity = 6;
int startTime, prevTime;
float angle=0, bullet_time_const = 1;
int glock_firerate = 200;
texture targetTexture, groundTexture, wallTexture, skyTexture, MG, pistol, scope;
SoundEngine Sound;
HUD hud;
Model obj[1];
bullet bullets[200];
GLfloat intensity[] = {0.90, 0.006, 0};
GLfloat lumi[] = {1, 1, 1, 1};

ObjectInst item;

vertex lookAt;

struct gun
{
	string name;
	int magazine_cap;
	int magazine_count;
} weapon_MG, weapon_pistol, weapon_sniper, *weapon_current;


float toRad(float degrees)
{
	return degrees * PI / 180;
}

bool collision3D(float x, float y, float z, float x1, float y1, float z1, float x2, float y2, float z2)
{
	return (x>=x1)&&(x<=x2)&&(z>=z1)&&(z<=z2)&&(y>=y1)&&(y<=y2);
}

/*
Timer Functions start
*/
void shotfade(int a)
{ 
    Sound.StopShotFade();
	Sound.playShotFade();
}

void sniper_reload(int a)
{ 
   Sound.playsniper_reload();
}

void firerate(int a)
{
    clearToShoot=true;
	if (a==1)
		{
		(*weapon_current).magazine_count=(*weapon_current).magazine_cap;
		}
}

void accuracyfalloff(int a)
{
    if (accuracy>=0.002)
		{
		  accuracy=accuracy-0.002;		  
		}
	if(recoil>0)
		{
		recoil=recoil-0.9;
		if (recoil <0.9)
			{recoil=0;}		
		}	

	if (accuracy<=0.002)
		{
		  accuracy=0;
		}
	glutTimerFunc(50, accuracyfalloff,0);
}

void walk(int a)
	{
	Sound.resume_Walk();
	}

void stopwalk(int a)
	{
	Sound.pause_Walk();
	}
/*
Timer Functions end
*/

void texturize()
{		
    glColor3f(255.0, 255.0, 255.0);
    glEnable( GL_TEXTURE_2D );
	glEnable(GL_NORMALIZE);
	
	glBindTexture( GL_TEXTURE_2D, targetTexture.getId()); 
    glBegin (GL_QUADS);


    glTexCoord2d(0.0,0.0); glVertex3d(target_x,-5,12); 
    glTexCoord2d(1.0,0.0); glVertex3d(target_x,-5,25); 
    glTexCoord2d(1.0,1.0); glVertex3d(target_x,8,25);
    glTexCoord2d(0.0,1.0); glVertex3d(target_x,8,12);
    glEnd();
	

	glBindTexture( GL_TEXTURE_2D, groundTexture.getId());    
    glBegin (GL_QUADS);

	glNormal3f(0.0, 1.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3d(200,-5,200); 
    glTexCoord2d(40,0.0); glVertex3d(-200,-5,200);
    glTexCoord2d(40,40); glVertex3d(-200,-5,-200); 
    glTexCoord2d(0.0,40); glVertex3d(200,-5,-200);
    glEnd();

	glBindTexture( GL_TEXTURE_2D, wallTexture.getId());    
    glBegin (GL_QUADS);
	
	glTexCoord2d(0.0,0.0); glVertex3d(200,-5,200); 
	glTexCoord2d(80,0.0); glVertex3d(-200,-5,200);
	glTexCoord2d(80,3); glVertex3d(-200,8,200);
    glTexCoord2d(0.0,3); glVertex3d(200,8,200); 
	
    glTexCoord2d(0.0,0.0); glVertex3d(-200,-5,200);
    glTexCoord2d(80,0.0); glVertex3d(-200,-5,-200); 
    glTexCoord2d(80,3); glVertex3d(-200,8,-200);
    glTexCoord2d(0.0,3); glVertex3d(-200,8,200); 

    glTexCoord2d(0.0,0.0); glVertex3d(-200,-5,-200); 
    glTexCoord2d(80,0.0); glVertex3d(200,-5,-200);
    glTexCoord2d(80,3); glVertex3d(200,8,-200);
    glTexCoord2d(0.0,3); glVertex3d(-200,8,-200); 

	glTexCoord2d(0.0,0.0); glVertex3d(200,-5,-200); 
    glTexCoord2d(80,0.0); glVertex3d(200,-5,200);
    glTexCoord2d(80,3); glVertex3d(200,8,200);
    glTexCoord2d(0.0,3); glVertex3d(200,8,-200); 
    glEnd();
	glDisable(GL_TEXTURE_2D);	
}

void drawSkybox()
{ // draws a box which relative to the player's position. The faces will then be applied a sky texture. 
	glColor3f(1.0,1.0,1.0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);	
	glEnable( GL_TEXTURE_2D );

	glBindTexture( GL_TEXTURE_2D, skyTexture.getId());  
	glBegin (GL_QUADS);

	glTexCoord2d(0.0,0.0); glVertex3d(300+xpos,-5,zpos+300); 
    glTexCoord2d(3,0.0); glVertex3d(-300+xpos,-5,300+zpos);
    glTexCoord2d(3,1); glVertex3d(-300+xpos,50,300+zpos);
    glTexCoord2d(0.0,1); glVertex3d(300+xpos,50,300+zpos);

	glTexCoord2d(0.0,0.0); glVertex3d(-300+xpos,-5,300+zpos);
    glTexCoord2d(3,0.0); glVertex3d(-300+xpos,-5,-300+zpos); 
    glTexCoord2d(3,1); glVertex3d(-300+xpos,50,-300+zpos);
    glTexCoord2d(0.0,1); glVertex3d(-300+xpos,50,300+zpos); 

	glTexCoord2d(0.0,0.0); glVertex3d(-300+xpos,-5,-300+zpos); 
    glTexCoord2d(3,0.0); glVertex3d(300+xpos,-5,-300+zpos);
    glTexCoord2d(3,1); glVertex3d(300+xpos,50,-300+zpos);
    glTexCoord2d(0.0,1); glVertex3d(-300+xpos,50,-300+zpos); 

	glTexCoord2d(0.0,0.0); glVertex3d(300+xpos,-5,-300+zpos); 
    glTexCoord2d(3,0.0); glVertex3d(300+xpos,-5,300+zpos);
    glTexCoord2d(3,1); glVertex3d(300+xpos,50,300+zpos);
    glTexCoord2d(0.0,1); glVertex3d(300+xpos,50,-300+zpos); 

	glTexCoord2d(0.0,0.0); glVertex3d(300+xpos,50,300+zpos); 
    glTexCoord2d(3.0,0.0); glVertex3d(-300+xpos,50,300+zpos);
	glTexCoord2d(3.0,5.0); glVertex3d(-300+xpos,50,-300+zpos);
    glTexCoord2d(0.0,5.0); glVertex3d(300+xpos,50,-300+zpos);     
    glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void init (void) 
{  
    glEnable(GL_DEPTH_TEST); // depth testing
    glDepthMask(true);

	glEnable(GL_LIGHTING); // lighting
    glEnable(GL_LIGHT0); // first light
	glEnable(GL_LIGHT1); // second light	
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH); // smooth shader

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1);

}

void light (void) 
{
	glPushMatrix();
    
    GLfloat ambientColor[] = {0.3f, 0.3f, 0.3f, 1.0f}; 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    
    float xlight = 0 + 50 * cos(angle); // coordinates for the moving light source
    float ylight = 0 + 50 * sin(angle);
	
	if (bullet_time==true)
		{
		angle=angle+0.004;
		}
	
	else 
		{
		angle=angle+0.0226;
		}


    GLfloat DiffuseLight[] = {1, 1, 1}; 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
    GLfloat LightPosition[] = {xlight, 2,ylight, 1}; //set the LightPosition to the specified values
    glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); 
	glLightfv (GL_LIGHT0, GL_CONSTANT_ATTENUATION,&intensity[0]);
	glLightfv (GL_LIGHT0, GL_LINEAR_ATTENUATION,&intensity[1]);
	glLightfv (GL_LIGHT0, GL_QUADRATIC_ATTENUATION,&intensity[2]);
	glTranslated(xlight, 2, ylight);
	glColor3f(0,0,0);
		
	glutSolidSphere(0.5,20,20);
	
    glPopMatrix();	
}

void camera(void)
{
gluLookAt(xpos, ypos, zpos, xpos+lookAt.x, ypos+lookAt.y, zpos+lookAt.z, 0, 1, 0);

if (lastx <20) //Fixes mouse getting stuck on the edges of the screen
	{
	glutWarpPointer(lastx+360,lasty);
	lastx+=360;	
	}
if (lastx >970)
	{
	glutWarpPointer(lastx-360,lasty);
	lastx-=360;	
	}
if (lasty>700)
	{	
	glutWarpPointer(lastx,lasty-360);
	lasty-=360;	
	}
if (lasty<20)
	{	
	glutWarpPointer(lastx,lasty+360);
	lasty+=360;	
	}
   
}

static void logic(int value)
{
    glutTimerFunc(1000/40,logic, 0);
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int timeSincePrevFrame = currTime - prevTime;
	int elapsedTime = currTime - startTime;

	g_rotation=(g_rotation_speed/1000) * elapsedTime;

	if ((firstMouseButton==true)&&(clearToShoot==true)&&(*weapon_current).magazine_count>0)
{     

	clearToShoot=false;
	if (bulletcount==200)
		{bulletcount=0;}	

	bullets[bulletcount].stopped=false;	
	
	if (bullet_time == true)
		{
		bullets[bulletcount].speed = 1.5;
		}
	
	else if (bullet_time == false)
		{
		bullets[bulletcount].speed = 6;
		}
			
		
	if((*weapon_current).name=="M249")
		{
		bullets[bulletcount].setCoords(xpos, ypos, zpos, yrot/180*PI, xrot/180*PI, accuracy, (*weapon_current).name, tracers, iswalking, zoomedIn);
		Sound.StopShotFade();	
		Sound.playShotMG();

		glutTimerFunc(200, shotfade, 0);
		glutTimerFunc(85*bullet_time_const, firerate, 0);	

		if (accuracy<0.05)
			{accuracy=accuracy+0.0058;}

		recoil=recoil+0.3;
		}
	
	else if((*weapon_current).name=="Glock G18")
		{
		bullets[bulletcount].setCoords(xpos, ypos, zpos, yrot/180*PI, xrot/180*PI, accuracy, (*weapon_current).name, false, iswalking, zoomedIn);
		Sound.StopShotFade();	
		Sound.playShotPistol();

		glutTimerFunc(glock_firerate, shotfade, 0);
		glutTimerFunc(glock_firerate*bullet_time_const, firerate, 0);		
		if (glock_auto == false)
			{
			if (accuracy<0.040)
				{accuracy=accuracy+0.015;}

			recoil=recoil+0.5;
			}
		else if (glock_auto == true)
			{
			if (accuracy<0.018)
				{accuracy=accuracy+0.01;}

			if (zoomedIn == true)
				{recoil=recoil+0.20;}
			else 
				{recoil=recoil+0.38;}
			}
		}

	else if((*weapon_current).name=="SV-98")
		{
bullets[bulletcount].setCoords(xpos, ypos, zpos, yrot/180*PI, xrot/180*PI, accuracy/5, (*weapon_current).name, false, iswalking, zoomedIn);
		Sound.StopShotFade();	
		Sound.playShotSniper();

		glutTimerFunc(200, shotfade, 0);
		glutTimerFunc(500, sniper_reload, 0);
		if(bullet_time == true)
			{
			glutTimerFunc(3500, firerate, 0);	
			}
		else
			{
			glutTimerFunc(2000, firerate, 0);	
			}

		if (accuracy<0.050)
			{accuracy=accuracy+0.017;}

		recoil=recoil+1.0;
		}

	glutWarpPointer(lastx,lasty+recoil);

	(*weapon_current).magazine_count--;
	bulletcount++;	
}
	
	for(int i=0;i<=200;i++)
    {
        if((bullets[i].stopped==false)&&(bullets[i].exists==true))
			{
			float xpostemp=bullets[i].xpos;
			float zpostemp=bullets[i].zpos;
			float ypostemp=bullets[i].ypos;
			xpostemp += float(sin(bullets[i].yrotrad))*bullets[i].speed;
			zpostemp -= float(cos(bullets[i].yrotrad))*bullets[i].speed;		
			ypostemp -= float(sin(bullets[i].xrotrad))*bullets[i].speed;

			if(ypostemp<=-5)
				{bullets[i].GroundPrediction();}

			if(collision3D(xpostemp,ypostemp,zpostemp,target_x,-5,12,target_x+14,8,25)==true)
				{bullets[i].prediction(target_x, -5, 12, target_x+14, 8, 25, target_x);}

			if(((xpostemp>200)||(xpostemp<-200)||(zpostemp>200)||(zpostemp<-200))&&(ypostemp>-5)&&(ypostemp<8))
				{
				bullets[i].WallsPrediction();			
				}

	  }
    }

	glutPostRedisplay();
	prevTime = currTime;
}

void draw_houses()	
{
glPushMatrix();
glPushMatrix();	
	glColor3f(0.90,0.80,0.80);	 
	glTranslated(40, 0, 50);	
	obj[0].Draw();		
	glPopMatrix();

	glPushMatrix();	 
	glColor3f(0.85,0.85,0.85);			
	glTranslated(-120, 0, 100);	
	glRotatef(-45,0,1,0);
	obj[0].Draw();		
	glPopMatrix();

	glPushMatrix();	       
	glColor3f(0.85,0.85,0.60);	
	glTranslated(-150, 0, 10);	
	glRotatef(-80,0,1,0);
	obj[0].Draw();		
	glPopMatrix();

	glPushMatrix();	                     
	glColor3f(0.85,0.85,0.85);		 
	glTranslated(-150, 0, -100);	
	glRotatef(-110,0,1,0);	
	obj[0].Draw();		
	glPopMatrix();

	glPushMatrix();	                    
	glColor3f(0.85,0.85,0.60);			 
	glTranslated(-60, 0, -150);	
	glRotatef(180,0,1,0);	
	obj[0].Draw();		
	glPopMatrix();

	glColor3f(0.90,0.80,0.80);	 
	glTranslated(80, 0, -50);
	glRotatef(180,0,1,0);
	obj[0].Draw();		
	glPopMatrix();

	glPopMatrix();
}

void render(void)
{   
    glDisable(GL_DEPTH_TEST);
	stringstream ss,ss2,ss3,ammo;
	if (show_hud == true)
	{	
    ss <<"X: "<<xpos<<"   "<<"Y: "<<zpos;
	ss2<<"Total bullet Count: "<<bulletcount<<"  Accuracy: "<<accuracy<<"  Recoil:"<<recoil;
	

	if ((*weapon_current).name=="M249")
		{
		if (tracers==false)
		ss3<<"Weapon: M249 Machine Gun   Rounds: Subsonic anti-personnel";
		else
		ss3<<"Weapon: M249 Machine Gun   Rounds: Tracer";
		}

	else if ((*weapon_current).name=="Glock G18")
		{
		if (glock_auto == false)
		ss3<<"Weapon: Glock G18 Sidearm  Mode: Semi-auto";

		else
		ss3<<"Weapon: Glock G18 Sidearm  Mode: Full-auto";
		}

	else if ((*weapon_current).name=="SV-98")
		{
		ss3<<"Weapon: SV-98 Sniper Rifle";
		}
	
	}

	ammo<<(*weapon_current).magazine_count<<" / "<<(*weapon_current).magazine_cap;

    glClearColor(1.0,1.0,1.0,1.0); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
    camera();
    
	drawSkybox();
	light();
	glEnable(GL_DEPTH_TEST);	
	texturize();

	item.draw();

	draw_houses();

	glPushMatrix();
	glRotatef(57.5*angle,0,1,0);
	obj[1].Draw();
	glPopMatrix();

	for(int i=0;i<=200;i++)
    {
		if(bullets[i].exists==true)
			{bullets[i].draw(target_x);}
    }


	if ((*weapon_current).name=="M249") 
		{
		hud.image(MG.getId(),-1,-0.6,1,1,zoomedIn);
		}
	
	else if ((*weapon_current).name=="Glock G18") 
		{
			hud.image(pistol.getId(),-1,-0.6,1,1,zoomedIn);
		}
	
	else if ((*weapon_current).name=="SV-98")
		{
		if (zoomedIn==true)
			hud.image(scope.getId(),-0.5,0.5,1,1,zoomedIn);
		}
   
	if(zoomedIn==false)
	    {
		hud.outputText(-0.9,0.9, ss.str(),1);
		hud.outputText(-0.9,0.8, ss2.str(),1);
		hud.outputText(-0.9,0.7, ss3.str(),1);
		hud.outputText(-0.9,-0.55, ammo.str(),1);
		}

    else if(zoomedIn==true)
		{
		hud.outputText(-0.225,0.225, ss.str(),1);
		hud.outputText(-0.225,0.2, ss2.str(),1);
		hud.outputText(-0.225,0.175, ss3.str(),1);
		}	

	hud.crossair(zoomedIn);			
	
    glutSwapBuffers(); 	
}

void reshape(int w, int h)
{	winH=h;
    winW=w;

	if(zoomedIn==true)
		{
	    glViewport(-(GLsizei)w/2, -(GLsizei)h/2, (GLsizei)w*2, (GLsizei)h*2);
		}

    else 
		{ 
		glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
		}

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 810); //perspective (A.O.S (FOV) , width, height, , depth)
    glMatrixMode(GL_MODELVIEW); 
}

void keyboard(unsigned char key, int x, int y) 
{
if (key == 'q')
    {
	if ((*weapon_current).name=="M249")
		{
		if (tracers==false)
			{tracers = true;}
		else 
			{tracers = false;}
		}

	if  ((*weapon_current).name=="Glock G18")
		{
		if (glock_auto == false)
			{
			glock_auto = true;
			glock_firerate = 50;
			}
		else
			{
			glock_auto = false;
			glock_firerate = 200;
			}
		}
    }

if (key == 'w') 
    {	
        iswalking = true;	
		glutTimerFunc(300, walk,0);    
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * PI);
		xrotrad = (xrot / 180 * PI);
		float xpostemp2=xpos;
		float zpostemp2=zpos;
		xpostemp2 += float(sin(yrotrad));
		zpostemp2 -= float(cos(yrotrad));

		if((xpostemp2<199)&&(xpostemp2>-199)&&(zpostemp2<199)&&(zpostemp2>-199))
			{
				if (bullet_time == true)
				{
			    xpos += float(sin(yrotrad))/4;
		        zpos -= float(cos(yrotrad))/4;
				}

				else
				{
	        	xpos += float(sin(yrotrad));
	        	zpos -= float(cos(yrotrad));
				}
			}
		
		//ypos -= float(sin(xrotrad));     Flying mode
		
    }

if (key == 's')
    {	
		iswalking = true;		
		glutTimerFunc(300, walk,0);  
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * PI);
		xrotrad = (xrot / 180 * PI);
		float xpostemp2=xpos;
		float zpostemp2=zpos;
		xpostemp2 -= float(sin(yrotrad));
		zpostemp2 += float(cos(yrotrad));	

		if((xpostemp2<199)&&(xpostemp2>-199)&&(zpostemp2<199)&&(zpostemp2>-199))
			{
			if (bullet_time == true)
				{
				xpos -= float(sin(yrotrad))/4;
			    zpos += float(cos(yrotrad))/4;
				}

			else
				{
				xpos -= float(sin(yrotrad));
			    zpos += float(cos(yrotrad));
				}
			}
	}

if (key == 'd') 
    {
	    iswalking = true;		
        glutTimerFunc(300, walk,0);  
		float yrotrad;
		yrotrad = (yrot / 180 * PI);

		float xpostemp2=xpos;
		float zpostemp2=zpos;
		xpostemp2 += float(cos(yrotrad)) * 0.5;
		zpostemp2 += float(sin(yrotrad)) * 0.5;	
		if((xpostemp2<199)&&(xpostemp2>-199)&&(zpostemp2<199)&&(zpostemp2>-199))
			{
			if (bullet_time == true)
				{
				xpos += (float(cos(yrotrad)) * 0.5)/3;
				zpos += (float(sin(yrotrad)) * 0.5)/3;
				}
				else
				{
				xpos += float(cos(yrotrad)) * 0.5;
				zpos += float(sin(yrotrad)) * 0.5;
				}

			}
    }
	
if (key == 'a')
    {	
	 iswalking = true;		
     glutTimerFunc(300, walk,0);  
     float yrotrad;
	 yrotrad = (yrot / 180 * PI);
		
	 float xpostemp2=xpos;
	 float zpostemp2=zpos;
	 xpostemp2 -= float(cos(yrotrad)) * 0.5;
	 zpostemp2 -= float(sin(yrotrad)) * 0.5;	
	 if((xpostemp2<199)&&(xpostemp2>-199)&&(zpostemp2<199)&&(zpostemp2>-199))
		{
			if (bullet_time == true)
				{
				xpos -= (float(cos(yrotrad)) * 0.5)/3;
				zpos -= (float(sin(yrotrad)) * 0.5)/3;
				}
				else
				{
				xpos -= float(cos(yrotrad)) * 0.5;
				zpos -= float(sin(yrotrad)) * 0.5;
				}

		}
    }

if (key == '1')
	{
	weapon_current = &weapon_MG;
		
	zoomedIn=false;
	Sensitivity=6;
	glViewport(0, 0, (GLsizei)winW, (GLsizei)winH);
	}

if (key == '2')
	{
		weapon_current = &weapon_pistol;
		
		zoomedIn=false;
		Sensitivity=6;
		glViewport(0, 0, (GLsizei)winW, (GLsizei)winH);
	}
	
if (key == '3')
	{
	weapon_current = &weapon_sniper;
	zoomedIn=false;
	Sensitivity=6;
	glViewport(0, 0, (GLsizei)winW, (GLsizei)winH);
	}

if (key == 'r')
	{
	clearToShoot = false;

	if ((*weapon_current).name=="M249")
		{
		Sound.play_mg_reload();
	    glutTimerFunc(5000, firerate, 1);	
		}

	else if((*weapon_current).name=="Glock G18")
		{
		Sound.play_pistol_reload();
		glutTimerFunc(1000, firerate, 1);
		}

	else if((*weapon_current).name=="SV-98")
		{
		glutTimerFunc(2000, firerate, 1);
		}
	}

if (key == 'z')
    {
	if (target_x < 199)
	target_x += 1 / bullet_time_const;
	}

if (key == 'x')
    {	
	target_x -= 1 / bullet_time_const;
	}

if (key == '4')
	{
	if(show_hud == true)
		show_hud = false;

	else
		show_hud = true;
	}

if (key == 27) 
	{
	Sound.drop_engines();
	obj[0].Release();
	exit(0);
	}
}

void keybup(unsigned char key, int x, int y) 
{
if (key=='w') 
{
iswalking=false;
glutTimerFunc(300, stopwalk,0);
}

if (key=='a')
{
iswalking=false;
glutTimerFunc(300, stopwalk,0);
}

if (key=='d') 
{
iswalking=false;
glutTimerFunc(300, stopwalk,0);
}

if (key=='s') 
{
iswalking=false;
glutTimerFunc(300, stopwalk,0);
}

if (key == 't')
{	
     if (bullet_time == false)
	 {
		  Sound.play_slow();
		  for(int i=0;i<=200;i++)
			  {
			  bullets[i].speed=1.5;
			  }
			  
		  bullet_time = true;
		  bullet_time_const=4;
		  Sound.playSpeed=0.3;
     }

     else
	 {
		  Sound.play_resume();
		  for(int i=0;i<=200;i++)
			  {
			  bullets[i].speed=6;
			  }
		  bullet_time = false;
		  bullet_time_const=1;
		  Sound.playSpeed=1.0;
	 }
}
if (key == VK_TAB)
{

if (fullscreen == true)
	{
	fullscreen = false;
	
	
	glutReshapeWindow(900,900);
	glutPositionWindow(800,0);

	}

else if (fullscreen == false)
	{
	fullscreen = true;

	
	glutReshapeWindow(1680,1050);
	glutPositionWindow(0,0);
	glutFullScreen();
	
	}

}

}
float clamp(float value, float min, float max)
{
	if(value > max) return max;
	else if(value < min) return min;
	else return value;
}

void mouseMovement(int x, int y) 
{    
    int diffx=(x-lastx); 
    int diffy=(y-lasty);

	lastx=x; 
    lasty=y; 
   
	xrot += (float) diffy/Sensitivity-recoil;
	yrot += (float) diffx/Sensitivity;	

    xrot = clamp(xrot, -90, 90);

	float xrad = toRad(xrot);
    float yrad = toRad(yrot);

	lookAt.y = -6 * sin(xrad);
	lookAt.z = 6 * cos(xrad);
	
	
	lookAt.x = 6 * sin(yrad);
	lookAt.z = -6 * cos(yrad);

}

void processMouse(int button, int state, int x, int y)
{

if (state == GLUT_DOWN)
	{
	  if (button == GLUT_LEFT_BUTTON) 
		{		
		firstMouseButton = true;		
		}

	  if (button == GLUT_RIGHT_BUTTON) 
		{		
		if(zoomedIn==false)
			{
			  zoomedIn=true;
			  Sensitivity=12;
			  if ((*weapon_current).name=="SV-98")
				  {
				  Sensitivity=30;
				  glViewport(-(GLsizei)winW*3.5, -(GLsizei)winH*3.5, (GLsizei)winW*8, (GLsizei)winH*8);
				  }
			  else
				  {
		          Sensitivity=12;
				  glViewport(-(GLsizei)winW*1.5, -(GLsizei)winH*1.5, (GLsizei)winW*4, (GLsizei)winH*4);
				  }
			}

		else if(zoomedIn==true)
			{ 
			  zoomedIn=false;
			  Sensitivity=6;
			  glViewport(0, 0, (GLsizei)winW, (GLsizei)winH);
			}
		}
	}

else if (state == GLUT_UP)
	{
	 firstMouseButton = false;
	}

}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize (900, 900);
    glutInitWindowPosition (20, 20);
    //glutCreateWindow("Introduction to 3D Programming");

	glutGameModeString("1680x1050:32@60");
	glutEnterGameMode(); 
	
    init();	

    glutDisplayFunc(render);
    glutReshapeFunc(reshape);

    glutPassiveMotionFunc(mouseMovement); //mouse movement
	glutMotionFunc(mouseMovement); // to fix mouseclick locks
	glutMouseFunc(processMouse);
    glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keybup);
	glutSetCursor(GLUT_CURSOR_NONE);

	obj[0].Load("house.obj",0,-5,0);	 //loads the house object
	obj[1].Load("MG2.obj",0,-12,0);	     //loads the turret object

	Sound.playAmbient();
	Sound.playWalk();
	Sound.pause_Walk();	

	glutTimerFunc(50, accuracyfalloff,0);
    glutTimerFunc(1000/30, logic, 0);      // logic function set to run 30 times a second 


	//loading of textures
	
	targetTexture = texture("target.bmp", GL_CLAMP);
	groundTexture = texture("ground2.bmp", GL_REPEAT);
	wallTexture = texture("brick.bmp", GL_REPEAT);
	skyTexture = texture("sky.bmp", GL_REPEAT);
	MG = texture("mg.bmp", GL_REPEAT);
	pistol = texture("pistol.bmp", GL_CLAMP);
	scope = texture("scope.bmp", GL_CLAMP);

	//Setting up the weapons
	weapon_MG.magazine_cap = 100;
	weapon_MG.magazine_count = 100;
	weapon_MG.name = "M249";
	weapon_pistol.magazine_cap = 14;
	weapon_pistol.magazine_count = 14;
	weapon_pistol.name = "Glock G18";
	weapon_sniper.magazine_cap = 5;
	weapon_sniper.magazine_count = 5;
	weapon_sniper.name = "SV-98";

	item = ObjectInst(&obj[0]);
	item.setPosition(100, 0, 100);
	item.setTint(0.2, 0.2, 0.8, 1.0);
	item.rot_y = 90;
	
	weapon_current = &weapon_MG; // sets the curren weapon to the MG (default equipped)

    // init time variables
	startTime = glutGet(GLUT_ELAPSED_TIME);
	prevTime = startTime;

    glutMainLoop();	
    return 0;
}