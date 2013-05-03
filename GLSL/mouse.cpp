/*////////////////////////////////////////////////////////////////////////////////////
mouse and keyboard functionalities
////////////////////////////////////////////////////////////////////////////////////*/

#include "inc\essentials.h"

//external variables used
//-------------------------------------------------------------
extern int Width,Height;
extern double zoom;
extern double theta;
extern double phi;
extern double dist;
extern Vec3 origin,eye,up;
extern double L_theta ;
extern double L_phi ;
extern double L_dist ;
extern double shininess;
//-------------------------------------------------------------
Vec2 curr_pos, last_pos;


Vec2 model_coords( int x, int y ){
	return Vec2(-1.0* ( 1.0 - (2.0 * y) / Height ) ,1.0 * ( (2.0 * x) / Width - 1.0 ));
}


// This is the function that we will tell OpenGL to call
// whenever the cursor is moved with a button pressed.
void mouse_motion( int x, int y )
{
	Vec2 P( model_coords( x, y ) );
	Vec2 delta( P - curr_pos );
	last_pos = curr_pos;
	curr_pos = P;
	theta += 0.5 * delta.y;
	phi   -= 0.5 * delta.x;
	glutPostRedisplay();
}



// This is the function that we will tell OpenGL to call
// whenever a mouse button is pressed or released.
void mouse_button( int button, int state, int x, int y ){
	Vec2 P( model_coords( x, y ) );
	curr_pos=P;


	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		last_pos = P;
		curr_pos = P;
		glutPostRedisplay();
	}
	else if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
	{
		glutPostRedisplay();
	}

}


void ProcessKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
	else if (key=='q'){
		zoom-=0.02;
		glutPostRedisplay();
	}
	else if (key=='w'){;
		zoom+=0.02;
		glutPostRedisplay();
	}

}


void special_key_press( int key, int x, int y ) 
{
	switch( key )
		{
		case GLUT_KEY_LEFT:
			L_theta += 0.05;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			L_theta -= 0.05;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
			L_phi += 0.05;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			L_phi -= 0.05;
			glutPostRedisplay();
			break;
	}

}