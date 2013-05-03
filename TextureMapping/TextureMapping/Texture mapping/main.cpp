/*
 * OpenGL demonstration program for UCI ICS Computer Graphics courses
 * main.cpp,v 2.1 2013/01/10 11:38 pm Shan Jiang
 *
 */

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

void drawScene(){

  for (int i = 0; i < 6; i++) {
    glBegin(GL_POLYGON);
		glColor3fv(&cubeColors[i][0]); //replace color information with texture coordinates
		glVertex4fv(&cubeVertexes[i][0][0]);
		glVertex4fv(&cubeVertexes[i][1][0]);
		glVertex4fv(&cubeVertexes[i][2][0]);
		glVertex4fv(&cubeVertexes[i][3][0]);
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
