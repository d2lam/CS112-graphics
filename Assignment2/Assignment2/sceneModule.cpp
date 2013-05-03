/*
 * OpenGL demonstration program for UCI ICS Computer Graphics courses
 * sceneModule.cpp,v 2.1 2013/01/10 11:38 pm Shan Jiang
 *
 */
#define PI 3.14159265
#include<Math.h>
#include "sceneModule.h"
#include "my_gl.h"

GLfloat angley = 20;    /* in degrees */
GLfloat anglex = 30;   /* in degrees */
GLfloat distanceX = 0.0;
GLfloat distanceY = 0.0;
GLfloat distanceZ = 5.0;
//added
GLdouble current_view[3]={5,0,0};
GLdouble prevx=0.0,prevy=0.0;
int startX;
int startY;
float speedx=0.005,speedy=0.005;


static float cubeColors[6][4] =
{
  {0.8, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.0, 1.0},
  {0.0, 0.8, 0.0, 1.0},
  {0.0, 0.0, 0.8, 1.0},
  {0.0, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.8, 1.0},
};

static float cubeVertexes[6][4][4] =
{
  {
    {-1.0, -1.0, -1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0}},

  {
    {-1.0, -1.0, -1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0}},

  {
    {-1.0, -1.0, -1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0}}
};

void drawScene()
{
  for (int i = 0; i < 6; i++) 
  {
	glPushMatrix();
    glBegin(GL_POLYGON);
		glColor3fv(&cubeColors[i][0]);
		glVertex4fv(&cubeVertexes[i][0][0]);
		glVertex4fv(&cubeVertexes[i][1][0]);
		glVertex4fv(&cubeVertexes[i][2][0]);
		glVertex4fv(&cubeVertexes[i][3][0]);
    glEnd();
	glPopMatrix();
  }
}

void Scross_product(GLdouble *ax, GLdouble *ay, GLdouble *az,
    GLdouble bx, GLdouble by, GLdouble bz,
    GLdouble cx, GLdouble cy, GLdouble cz)
{
	//vx = by * cz - bz * cy
	//vy = bz * cx - bx * cz
	//vz = bx * cy - by * cx

   *ax =	(by * cz - bz * cy);
   *ay =	(bz * cx - bx * cz);
   *az =	(bx * cy - by * cx);
}
void Snormalize(GLdouble *x, GLdouble *y, GLdouble *z)
{
	// x = ax/|a|
	// y = ay/|a|
	// z = az/|a|
	// |a| = length

	GLdouble length = (GLdouble)sqrt(((*x) * (*x)) + ((*y) * (*y)) + ((*z) * (*z)));

	*x = *x/length;
	*y = *y/length;
	*z = *z/length;
}

void sceneTransformation()
{
	
	glLoadIdentity( );
	glTranslatef(-distanceX, distanceY, -distanceZ);
	glRotatef( anglex, 1.0, 0.0, 0.0 );
	glRotatef( angley, 0.0, 1.0, 0.0 );

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,1388/1024,1.0,100.0);
	gluLookAt(1.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	*/
	//gluLookAt(0 , 3, 0,0.0,0.0,0.0,0.0 ,1.0 ,0.0);

}

void display(){
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  /* Set up transformation */
  sceneTransformation();

  /*
	GLdouble V[3],U[3]= {0,1,0},R[3];

	V[0] = -current_view[0];
	V[1] = -current_view[1];
	V[2] = -current_view[2];
	Scross_product(&R[0],&R[1],&R[2],U[0],U[1],U[2],V[0],V[1],V[2]);
	Snormalize(&R[0],&R[1],&R[2]);
	GLdouble mx,my;

	mx = startX-prevx;
	my = startY-prevy;

	GLdouble newViewPoint[3] = {current_view[0] + (mx*distanceX *R[0]) + (my*distanceY*U[0]),
		current_view[1] + (mx*distanceX *R[1]) + (my*distanceY*U[1]),
		current_view[2] + (mx*distanceX *R[2]) + (my*distanceY*U[2])};
	GLdouble length_nvp = sqrt((newViewPoint[0]*newViewPoint[0])+
		(newViewPoint[1]*newViewPoint[1])+(newViewPoint[2]*newViewPoint[2]));
	
	current_view[0] = newViewPoint[0]*distanceX/length_nvp;
	current_view[1] = newViewPoint[1]*distanceY/length_nvp;
	current_view[2] = newViewPoint[2]*distanceZ/length_nvp;

	Scross_product(&U[0],&U[1],&U[2],R[0],R[1],R[2],V[0],V[1],V[2]);
	Snormalize(&U[0],&U[1],&U[2]);

	gluLookAt(-V[0],-V[1],-V[2],0,0,0,U[0],U[1],U[2]);

	prevx = startX;
	prevy = startY;
	*/

  /* Draw the scene into the back buffer */

  //draw rotator (larger cube)
  
  glPushMatrix();
  glLoadIdentity();
  glScalef(0.5f,0.5f,0.5f);
  gluLookAt(-1.0, -6.0, -2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glTranslatef(0.0f, 0.0f, 0.0f);
  glRotated(anglex,1,0,0);
  drawScene();
  glPopMatrix();
  

  //draw spinner (smaller cube)
  
  glPushMatrix();
  glRotated(angley,0,1,0);
  glTranslatef(-0.0f, 4.5f, -0.0f);
  glScalef(0.25f, 0.25f, 0.25f);
  drawScene();
  glPopMatrix();
  
  /*
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,20.0,1.0,100.0);
  glMatrixMode(GL_MODELVIEW);
  */
  /* Swap the front buffer with the back buffer - assumes double buffering */
  glutSwapBuffers();
}

void idle_func()
{
	if(anglex < 360)
	{
		anglex += speedx;
	}
	if(anglex > 360)
	{
		anglex = -anglex + speedx;
	}
	if(angley < 360)
	{
		angley += speedy;
	}
	if(angley > 360)
	{
		angley = -angley + speedy;
	}
    glutPostRedisplay();
}