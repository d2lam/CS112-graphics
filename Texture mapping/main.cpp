/*
 * OpenGL demonstration program for UCI ICS Computer Graphics courses
 * main.cpp,v 2.1 2013/01/10 11:38 pm Shan Jiang
 *
 */

#include<iostream>
#include<cassert>
#include "sceneModule.h"
#include "inputModule.h"
#include "imageloader.h"

#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

int window;

GLfloat fieldOfView;
GLfloat aspect;
GLfloat nearPlane;
GLfloat farPlane;

extern float cubeColors[][4];
extern float cubeVertexes[][4][4];
extern void sceneTransformation();
//store texture data
GLuint texName[1];
//store image 
Image *pic;
void loadImage();

void drawScene(){
	//glBindTexture(GL_TEXTURE_2D, texName[0]);
	for (int i = 0; i < 6; i++) 
	{
		//GL_QUADS or GL_POLYGON
		glBegin(GL_POLYGON);
			//glColor3fv(&cubeColors[i][0]); //replace color information with texture coordinates
			//glTexCoord2f(0.0, 0.0);
			//glTexCoord4f(pic[i].pixels[i],pic[i].pixels[0],pic[i].pixels[0],1.0);
			glTexCoord2f(0.0, 0.0);	glVertex4fv(&cubeVertexes[i][0][0]);
			glTexCoord2f(0.0, 1.0);	glVertex4fv(&cubeVertexes[i][1][0]);
			glTexCoord2f(1.0, 1.0);	glVertex4fv(&cubeVertexes[i][2][0]);
			glTexCoord2f(1.0, 0.0);	glVertex4fv(&cubeVertexes[i][3][0]);
		glEnd();
	}
}

//##########################################
// Init display settings

void initDisplay( ){

  /* Perspective projection parameters */
	
  fieldOfView = 45.0;
  aspect = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
  nearPlane   = 0.1;
  farPlane    = 50.0;

  /* setup context */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective( fieldOfView, aspect, nearPlane, farPlane );

  glEnable( GL_DEPTH_TEST );
  glDisable( GL_CULL_FACE );

  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  glClearDepth( 1 );

  glMatrixMode( GL_MODELVIEW );

  glEnable(GL_TEXTURE_2D); 
  loadImage();
  //glBindTexture(GL_TEXTURE_2D, texName[0]);
}

void loadImage()
{
	//load the image into the image object
	char texture[] = "F:\\sjiang4_CS112_A4Package\\CS112_A4Package\\Texture mapping\\space.bmp";
	pic = loadBMP(texture);

	glGenTextures(1, texName);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, pic->width, pic->height, 0, GL_RGB,GL_UNSIGNED_BYTE, pic->pixels);

	assert( GL_NO_ERROR == glGetError() );
}

void display(){
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
  /* Set up transformation */
  sceneTransformation();
  /* Draw the scene into the back buffer */
  drawScene();
  /* Swap the front buffer with the back buffer - assumes double buffering */
  glutSwapBuffers();
}

//##########################################
// Main function

int main( int argc, char **argv ){

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |
                       GLUT_DEPTH | GLUT_MULTISAMPLE );

  glutInitWindowSize( IMAGE_WIDTH,IMAGE_HEIGHT );

  glutInitWindowPosition(100,120);
  window = glutCreateWindow("Assignment 4");

  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  /* Register the appropriate callback functions with GLUT */
  glutDisplayFunc( display );
  glutKeyboardFunc( readKeyboard );
  glutMouseFunc( mouseButtHandler );
  glutMotionFunc( mouseMoveHandler );
  glutPassiveMotionFunc( mouseMoveHandler );

  //glutIdleFunc(*some function*);

  initDisplay( );
  glutMainLoop( );

  return( 0 );
}
