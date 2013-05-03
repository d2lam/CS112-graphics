
#include<iostream>
#include <windows.h>
#include <GL\GL.h>

// include declaration ofI_my_gl functions
#include "i_my_gl.h"

void printMatrix(double *matrix)
{
	// simply print the matrix
	for(int i=0,count=0;i<16;i++)
	{
		std::cout<<matrix[i]<<"\t";
		count++;
		if(count%4==0)
		{
			//print a newline
			std::cout<<" "<<std::endl;
		}
	}
}

boolean compareFloating(double a,double b)
{
	/**
		Comparing floating point numbers
	*/
	double diff= a - b;
	return !fabs(diff)<DBL_EPSILON;
}

boolean compareMat(double *matrix1,double *matrix2)
{
	/**
	Return if matrices are the same
	*/
	for(int i=0;i<16;i++)
	{
		if(compareFloating(matrix1[i],matrix2[i]))
		{
			return true;
		}
	}
	return false;
}

void checks()
{
	//template for the others , never actually call it
	double m1[16],m2[16];

	glPushMatrix();
	glLoadIdentity( );//opengl command to compare to
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glLoadIdentity();//my opengl implementation
	I_my_glGetMatrixd(m2);//load my matrix

	std::cout<<"start"<<std::endl;
	printMatrix(m1);
	std::cout<<"-------"<<std::endl;
	printMatrix(m2);
	std::cout<<"end"<<std::endl;
	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
}

// the followings are wrapper functions
// synchronize OpenGL's current matrix with i_my_gl's current matrix
void sync_matrix()
{
    GLdouble m[16];
    I_my_glGetMatrixd(m);
    glLoadMatrixd(m);
}

// switching matrix mode for both OpenGL and i_my_gl
void my_glMatrixMode(GLenum mode)
{
    I_my_glMatrixMode(mode);
    glMatrixMode(mode);
    sync_matrix();
}

// all following functions first call corresponding i_my_gl functions and then synchronize current matrix with OpenGL

void my_glLoadIdentity(void)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	glLoadIdentity( );//opengl command to compare to
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glLoadIdentity();//my opengl implementation
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glLoadIdentity();
    sync_matrix();
}

void my_glPushMatrix(void)
{
	/*
	double m1[16],m2[16];

	glLoadIdentity();//my opengl implementation
	glPushMatrix();
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glLoadIdentity();//my opengl implementation
	I_my_glPushMatrix();
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glPushMatrix();
    sync_matrix();
}

void my_glPopMatrix(void)
{
    I_my_glPopMatrix();
    sync_matrix();
}

void my_glLoadMatrixf(const GLfloat *m)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	glLoadMatrixf(m);
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glLoadMatrixf(m);
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glLoadMatrixf(m);
    sync_matrix();
}

void my_glLoadMatrixd(const GLdouble *m)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	glLoadMatrixd(m);
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glLoadMatrixd(m);
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glLoadMatrixd(m);
    sync_matrix();    
}

void my_glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	glTranslated(x, y, z);
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glTranslated(x, y, z);
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glTranslated(x, y, z);
    sync_matrix();
}

void my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	glTranslatef(x, y, z);
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glTranslatef(x, y, z);
	I_my_glGetMatrixd(m2);//load my matrix
	
	std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
   I_my_glTranslatef(x, y, z);
   sync_matrix();
}

void my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	glRotated(angle, x, y, z);
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glRotated(angle, x, y, z);
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glRotated(angle, x, y, z);
    sync_matrix();
}

void my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	glRotatef(angle, x, y, z);//opengl command to compare to
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glRotatef(angle, x, y, z);//my opengl implementation
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glRotatef(angle, x, y, z);
    sync_matrix();
}

void my_glScaled(GLdouble x, GLdouble y, GLdouble z)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	I_my_glScaled(x, y, z);//opengl command to compare to
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glScaled(x, y, z);//my opengl implementation
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glScaled(x, y, z);
    sync_matrix();
}

void my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	I_my_glScalef(x, y, z);//opengl command to compare to
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_glScalef(x, y, z);//my opengl implementation
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_glScalef(x, y, z);
    sync_matrix();
}

void my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
    GLdouble centerX, GLdouble centerY, GLdouble centerZ,
    GLdouble upX, GLdouble upY, GLdouble upZ)
{
    I_my_gluLookAt(eyeX, eyeY, eyeZ, 
        centerX, centerY, centerZ,
        upX, upY, upZ);
    sync_matrix();
}

void my_glFrustum(GLdouble left, GLdouble right, 
    GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    I_my_glFrustum(left, right, bottom, top, zNear, zFar);
    sync_matrix();
}

void my_gluPerspective(GLdouble fovy, GLdouble aspect,
    GLdouble zNear, GLdouble zFar)
{
	/*
	double m1[16],m2[16];

	glPushMatrix();
	gluPerspective(fovy, aspect, zNear, zFar);//opengl command to compare to
	glGetDoublev(GL_MODELVIEW_MATRIX,m1);//load opengls matrix
	glPopMatrix();

	I_my_gluPerspective(fovy, aspect, zNear, zFar);//my opengl implementation
	I_my_glGetMatrixd(m2);//load my matrix

	if(compareMat(m1,m2))
	{
		std::cout<<"Not the Same"<<std::endl;
		std::cout<<"GL version"<<std::endl;
		printMatrix(m1);
		std::cout<<"Mine"<<std::endl;
		printMatrix(m2);
	}
	*/
    I_my_gluPerspective(fovy, aspect, zNear, zFar);
    sync_matrix();
}
//*/



