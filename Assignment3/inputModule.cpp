
#include <sys/types.h>
#include "inputModule.h"
#include "PLY.h"
#include "geometry.h"
#include<math.h>

/* This File contains the KeyBoard and mouse handling routines */
static int motionMode;
static int startX;
static int startY;
static GLfloat angle = 20;    /* in degrees */
static GLfloat angle2 = 30;   /* in degrees */

GLfloat current_pos[] = {0.0, 0.0, 5.0};
//Added by me
GLfloat light_position[] = {-100.0, 100.0, 100.0 , 0.0};

static GLfloat LightAngle1 = 20;    /* in degrees */
static GLfloat LightAngle2 = 30;   /* in degrees */
int pos_modifier = 1;

int flat = 0;
int light = 1;
int wire = 0;

extern PLYObject* ply;

void readKeyboard(unsigned char key, int x, int y)
{
  switch(key){
  case  0x1B:
  case  'q':
  case  'Q':
    exit(0);
    break; 
  case '+':
    if (ply)
      ply->eat();
    break;
  case '-':
    if (ply)
      ply->starve();
    break;
  case 'd':
    if (ply)
      ply->dance();
    break;
  case 'i':
  case 'I':
    if (ply)
      ply->invertNormals();
    break;
  case 'l':
  case 'L':
    light = (light + 1) % 2;
		printf("%s lighting\n", (light ? "OpenGL" : "User"));
    break;
  case 't':
  case 'T':
		// A3: Change some variable here...
	   pos_modifier = (pos_modifier + 1) % 2;
	   printf("%s Position\n", (pos_modifier ? "Light" : "Viewer"));
		break;
  case 'r':
  case 'R': 
   // reset initial view parameters
    angle = 20;
    angle2 = 30;
    current_pos[0] = 0.0;
    current_pos[1] = 0.0;
    current_pos[2] = 5.0;
	//reset the light
	LightAngle1 = 20;
    LightAngle2 = 30;

    light_position[0] = -100.0;
    light_position[1] = 100.0;
    light_position[2] = 100.0;
    break;
  case 'h':
  case 'H':
    printf("\tPress q/Q for Quit\n");
    printf("\tPress h/H to print this help\n");
    printf("\tPress l/L to turn on/off Lighting\n");
    printf("\tPress i/I to invert the normals\n");
    printf("\tPress r/R to revert ViewPoint to initial position\n");
    printf("\tPress + to make the bunny grow fatter\n");
    printf("\tPress - to make the bunny grow thinner\n");
    printf("\tPress d/D to make the bunny dance randomly\n");
  default:
    break;
  }
  glutPostRedisplay();
}


void readSpecialKeys(int key, int x, int y)
{
  switch(key){
  case GLUT_KEY_UP: 
    
    break;
  case GLUT_KEY_DOWN: 
    
    break;
  case GLUT_KEY_RIGHT: 
    
    break;
  case GLUT_KEY_LEFT:  
    
    break;
  }
  glutPostRedisplay();
}


void mouseButtHandler(int button, int state, int x, int y)
{
  motionMode = 0;
  
  switch(button){
  case GLUT_LEFT_BUTTON:  
    if(state == GLUT_DOWN) {
      motionMode = 1;		// Rotate object
      startX = x;
      startY = y;
    }
    break;
  case GLUT_MIDDLE_BUTTON:  
    if(state == GLUT_DOWN) {
      motionMode = 2;		// Translate object
      startX = x;
      startY = y;
    }
    break;
  case GLUT_RIGHT_BUTTON: 
    if(state == GLUT_DOWN) { 
      motionMode = 3;		// Zoom
      startX = x;
      startY = y;
    }
    break;
  }
  glutPostRedisplay();
}


void mouseMoveHandler(int x, int y)
{
  
  // No mouse button is pressed... return 
  switch(motionMode){
  case 0:
    return;
    break;

  case 1: // Calculate the rotations
	  if(pos_modifier)
	  {
		LightAngle1 = LightAngle1 + (x - startX);
		LightAngle2 = LightAngle2 + (y - startY);
	  }
	  else
	  {
		angle = angle + (x - startX);
		angle2 = angle2 + (y - startY);
	  }
    startX = x;
    startY = y;
    break;

  case 2:
	  if(pos_modifier)
	  {
		  light_position[0] = light_position[0] - (x - startX)/100.0;
		  light_position[1] = light_position[1] - (y - startY)/100.0;
	  }
	  else
	  {
		  current_pos[0] = current_pos[0] - (x - startX)/100.0;
		  current_pos[1] = current_pos[1] - (y - startY)/100.0;
	  }
    startX = x;
    startY = y;
    break;
    
  case 3:
	  if(pos_modifier)
	  {
		  light_position[2] = light_position[2] - (y - startY)/10.0;
	  }
	  else
	  {
		  current_pos[2] = current_pos[2] - (y - startY)/10.0;
	  }
    
    startX = x;
    startY = y;
    break;
  }
  
  glutPostRedisplay();
}


void setUserView()
{
  //glPushMatrix();
  glLoadIdentity();
  glTranslatef(-current_pos[0], current_pos[1], -current_pos[2]);
  glRotatef(angle2, 1.0, 0.0, 0.0);
  glRotatef(angle, 0.0, 1.0, 0.0);
}

void transformLight()
{
	glTranslatef(-light_position[0],light_position[1],-light_position[2]);
	glRotated(LightAngle1, 0.0, 1.0, 0.0);
	glRotated(LightAngle2, 1.0, 0.0, 0.0);
}
