#include "__PreCompile.h"
#include "hud.h"


/*
LoadTexture() loads a texture from 24 bit .bmp files.
It does not support alpha channels, instead it's build so that it treat certain colours as transparent (The pink method)
Here I use pure blue (0,0,250) as transparent.
*/
GLuint HUD::LoadTexture( const char * filename, int width, int height, GLfloat param )
{
    GLuint texture;
    unsigned char * data;
	unsigned char * data2;
    FILE * file;
    
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;
    data = (unsigned char *)malloc( width * height * 3 );
	data2 = (unsigned char *)malloc( width * height * 4 );
    fread( data, width * height * 3, 1, file );
	unsigned long size = width * height * 3;	
	fread(data, size, 1, file);
	char aux;
	for(int bit_pixel_a = 0, bit_pixel_b = 0; bit_pixel_a < size; bit_pixel_a += 3, bit_pixel_b += 4)
	{
	data2[bit_pixel_b]=data[bit_pixel_a];
	data2[bit_pixel_b+1]=data[bit_pixel_a+1];
	data2[bit_pixel_b+2]=data[bit_pixel_a+2];
	if (data[bit_pixel_a] == 255 && data[bit_pixel_a+1] == 0 && data[bit_pixel_a+2] == 0)
		data2[bit_pixel_b+3]=0;
	else
		data2[bit_pixel_b+3]=255;
	}

    fclose( file );


    glGenTextures( 1, &texture ); //generate the texture with the loaded data
    glBindTexture( GL_TEXTURE_2D, texture ); // bind 
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ONE_MINUS_SRC_ALPHA ); //set texture environment parameters
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR  );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR  );  
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param );

    //Generate the texture
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data2 );
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
    free( data ); //free the texture

    return texture; 
}


/*
outputText() outputs a string at the x and y coordinates on the screen.
*/
void HUD::outputText(float x, float y, string text, int font)
	{
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);					
	glPushMatrix();							
	glLoadIdentity();						
	glMatrixMode(GL_MODELVIEW);					
	glPushMatrix();							
	glLoadIdentity();
	glColor3f(0,0,0);						

   glRasterPos2f(x, y); 
   
   for (int i = 0; i < text.length(); i++) 
   {
   glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
   }

   	glMatrixMode( GL_PROJECTION );					
	glPopMatrix();							
	glMatrixMode( GL_MODELVIEW );					
	glPopMatrix();	
		glEnable(GL_LIGHTING);

	}

/*
crossair() outputs a simple crossair on the screen. 
It's dynamically implemented so that it's bigger while zoomed-in
*/
void HUD::crossair(bool zoomedIn)
	{	
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);					
	glPushMatrix();							
	glLoadIdentity();						
	glMatrixMode(GL_MODELVIEW);					
	glPushMatrix();							
	glLoadIdentity();	


	glColor3f(1,0,0);
 
    glLineWidth(1.5);
    glBegin(GL_LINES); 
if(zoomedIn==false)
   {
   glVertex2f(0, 0.022); 
   glVertex2f(0, -0.022);   
   glVertex2f(0.0135, 0); 
   glVertex2f(-0.0135, 0);
   }

else if(zoomedIn==true)
   {
   glVertex2f(0, 0.011); 
   glVertex2f(0, -0.011);   
   glVertex2f(0.00675, 0); 
   glVertex2f(-0.00675, 0);
   }
		
	
    glEnd();  


	glMatrixMode( GL_PROJECTION );					
	glPopMatrix();							
	glMatrixMode( GL_MODELVIEW );					
	glPopMatrix();		
	glEnable(GL_LIGHTING);
	}


/*
image() is used to display an image from a texture on the HUD. 
The function also scales images accordingly to the zoomed-in state.
*/
void HUD::image(GLuint texture, float x, float y, float x2, float y2, bool zoomedIn)
{
	glMatrixMode(GL_PROJECTION);					
	glPushMatrix();							
	glLoadIdentity();						
	glMatrixMode(GL_MODELVIEW);					
	glPushMatrix();							
	glLoadIdentity();

	glColor3f(1.0,1.0,1.0);

	glDisable(GL_LIGHTING);
	glEnable( GL_TEXTURE_2D );
	//glDisable(GL_DEPTH_TEST);
    glBindTexture( GL_TEXTURE_2D, texture); //bind our texture to our shape   


	 glBegin (GL_QUADS);
	 if(zoomedIn==false)
	    {
		glTexCoord2d(0.0, 0.0); glVertex3f(x, x, 0.0);  
		glTexCoord2d(1.0, 0.0); glVertex3f(y, x, 0.0); 
		glTexCoord2d(1.0, 1.0); glVertex3f(y, y, 0.0);
		glTexCoord2d(0.0, 1.0);	glVertex3f(x, y, 0.0); 
		}
	 else 
		{
		glTexCoord2d(0.0, 0.0); glVertex3f(x/4, x/4, 0.0);  
		glTexCoord2d(1.0, 0.0); glVertex3f(y/4, x/4, 0.0); 
		glTexCoord2d(1.0, 1.0); glVertex3f(y/4, y/4, 0.0);
		glTexCoord2d(0.0, 1.0);	glVertex3f(x/4, y/4, 0.0); 
		}
	glEnd();	
		
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
   	glMatrixMode( GL_PROJECTION );					
	glPopMatrix();							
	glMatrixMode( GL_MODELVIEW );					
	glPopMatrix();
}
