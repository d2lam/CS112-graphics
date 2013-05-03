/*////////////////////////////////////////////////////////////////////////////////////
3D model View software
(c) Shan Jiang, Uddipan Mukherjee {sjiang4,umukherj@uci.edu}
////////////////////////////////////////////////////////////////////////////////////*/

#include "inc\essentials.h"


//----------------------external functions--------------------------------
extern GLuint setupShaders();
extern void display();
extern void changeSize(int,int);
extern void mouse_motion( int x, int y );
extern void mouse_button( int button, int state, int x, int y );
extern void ProcessKeys(unsigned char key,int x,int y);
extern void special_key_press( int key, int x, int y );
extern void ReadFile(string);
extern void setupBuffers();
//------------------------------------------------------------------------

//-----------Program and Shader Identifiers-------------------------------
GLuint ProgramID,VertexID,FragmentID;
//------------------------------------------------------------------------

//------------OpenGL Window parameters------------------------------------
int Width=800,Height=700; 
unsigned int FrameCount=0;//reqd for calculating fps
int MODE=0;
int main_window;
//------------------------------------------------------------------------


int main(int argc, char **argv) {
	string s="bunny.ply";
	ReadFile(s);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(Width,Height);
    main_window=glutCreateWindow("Assignment 4");
	glClearColor(  1,1,1,1.0 ); // clear window.
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_LIGHTING );
	glEnable( GL_NORMALIZE );
	glEnable( GL_DEPTH_TEST );
	glCullFace( GL_FRONT);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glewInit();
    glutDisplayFunc(display);
    //glutIdleFunc(display);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(ProcessKeys);
	glutSpecialFunc(special_key_press);
	glutMouseFunc(mouse_button);
	glutMotionFunc(mouse_motion);	

	ProgramID = setupShaders();
	setupBuffers();

    glutMainLoop();
	return 0;
}