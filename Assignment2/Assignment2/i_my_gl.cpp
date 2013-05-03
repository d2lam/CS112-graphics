
#include <windows.h>
#include<iostream>
#include <GL/gl.h>

// define maximum stack capacity
#define STACK_CAP 16
// define max matrix size
#define MAX_MATRIX_SIZE 16
// define pi for converting angles
#define PI 3.14159265359

//debugging boolean
#define debug false

// structure for the matrix stack, top specifies current top position on the stack, initially zero (which means one matrix in the stack)
struct matrix_stack
{
    GLdouble m[STACK_CAP][16];
    int top;
};

// define a macro for retrieving current matrix from top of current stack
#define current_matrix (current_stack->m[current_stack->top])
//handy for row operations
#define current(row,col) current[(col<<2)+row]
#define b(row,col) b[(col<<2)+row]

// identity matrix constant
const GLdouble identity[16] =
{1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1};

// the model view matrix stack
struct matrix_stack model_view = {{{0}}, 0};
// the projection matrix stack
struct matrix_stack projection = {{{0}}, 0};
// the current stack pointer that specifies the matrix mode
struct matrix_stack *current_stack = &model_view;

/*multiply 2 matrix ab and put the result in a*/
void multi_matrix(GLdouble *a,GLdouble *b)
{
	for (int i = 0; i < MAX_MATRIX_SIZE/4; ++i)
	{
		for (int j = 0; j < MAX_MATRIX_SIZE/4; ++j)
		{
			for (int k = 0; k < MAX_MATRIX_SIZE/4; ++k)
			{
				a[i*4 + j] += a[i*4 + k] * (b[k*4 + j]);
			}
		}
	}
}

/*Transpose a matrix , made this to make my matrices
easier to understand and also keep the function working*/
void matrix_transpose(GLdouble *b)
{
	GLdouble transpose[MAX_MATRIX_SIZE];
	// first transpose the matrix
	for(int i = 0; i < MAX_MATRIX_SIZE/4; i++)
	{
		for (int j = 0; j < MAX_MATRIX_SIZE/4; j++)
		{
			transpose[j * 4 + i] = b[i * 4 + j];
		}
	}
	// now replace the values in b with
	// the transpose
	for(int i = 0; i < MAX_MATRIX_SIZE; i++)
	{
		b[i] = transpose[i];
	}
}

// multiply current matrix with matrix b, put the result in current matrix
// current = current * b
void matrix_multiply(const GLdouble *b)
{
    //N.B all arrays are 1d in c/c++
	//N.B row by column
	
	GLdouble *current = current_matrix;
	GLint i;
   for (i = 0; i < 4; i++) 
   {
	  const GLfloat currenti0=current(i,0),  currenti1=current(i,1),  currenti2=current(i,2),  currenti3=current(i,3);
      current(i,0) = currenti0 * b(0,0) + currenti1 * b(1,0) + currenti2 * b(2,0) + currenti3 * b(3,0);
      current(i,1) = currenti0 * b(0,1) + currenti1 * b(1,1) + currenti2 * b(2,1) + currenti3 * b(3,1);
      current(i,2) = currenti0 * b(0,2) + currenti1 * b(1,2) + currenti2 * b(2,2) + currenti3 * b(3,2);
      current(i,3) = currenti0 * b(0,3) + currenti1 * b(1,3) + currenti2 * b(2,3) + currenti3 * b(3,3);
   }
}

// calculating cross product of b and c, put the result in a
// a = b x c
void cross_product(GLdouble *ax, GLdouble *ay, GLdouble *az,
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

// normaliz vector (x, y, z)
void normalize(GLdouble *x, GLdouble *y, GLdouble *z)
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

// switch matrix mode by changing the current stack pointer
void I_my_glMatrixMode(GLenum mode)
{
	//Not so sure about this one , pointer could be wrong
	if(debug){std::cout<<"Current matrix mode = ";}

    // switch the current stack pointer
	if(mode == GL_MODELVIEW)
	{
		current_stack = &model_view;
		if(debug){std::cout<<"model_view"<<std::endl;}
		return;
	}
	else if(mode == GL_PROJECTION)
	{
		current_stack = &projection;
		if(debug){std::cout<<"projection"<<std::endl;}
		return;
	}
}

// overwrite current matrix with identity matrix
void I_my_glLoadIdentity(void)
{
    // ...
	GLdouble *current = current_matrix;

	for(int i=0;i<MAX_MATRIX_SIZE;i++)
	{
		*(current +i) = identity[i];
	}
}

// push current matrix onto current stack, report error if the stack is already full
void I_my_glPushMatrix(void)
{
	//get current matrix		
	GLdouble *current = current_matrix;

	// get current stack
	if(current_stack == &model_view)
	{
		if(model_view.top < STACK_CAP)
		{
			//copy the current matrix into the new top of the stack
			for(int i=0;i<MAX_MATRIX_SIZE;i++)
			{
				model_view.m[model_view.top+1][i] = *(current +i);
			}
			//increment top
			model_view.top++;
		}
		else
		{
			std::cout<<"Cannot Push to Model View Stack,Limit reached"<<std::endl;
		}
	}
	else
	{
		if(projection.top < STACK_CAP)
		{
			//copy the current matrix into the new top of the stack
			for(int i=0;i<MAX_MATRIX_SIZE;i++)
			{
				projection.m[projection.top+1][i] = *(current +i);
			}
			//increment top
			projection.top++;
		}
		else
		{
			std::cout<<"Cannot Push to Projection Stack,Limit reached"<<std::endl;
		}
	}
}

// pop current matrix from current stack, report error if the stack has only one matrix left
void I_my_glPopMatrix(void)
{
    // need to fix this part as well
	if(current_stack == &model_view)
	{
		if(model_view.top > -1)
		{
			//decrement the counter of the current stack
			model_view.top--;
		}
		else
		{
			std::cout<<"Cannot POP from Model View Stack,Nothing left!!"<<std::endl;
		}
	}
	else
	{
		if(projection.top > -1)
		{
			//decrement the counter of the current stack
			projection.top--;
		}
		else
		{
			std::cout<<"Cannot POP from Projection Stack,Nothing left!!"<<std::endl;
		}
	}
}

// overwrite currentmatrix with m
void I_my_glLoadMatrixf(const GLfloat *m)
{
	if(debug){std::cout<<"I_my_glLoadMatrixf"<<std::endl;}
	GLdouble *current = current_matrix;
	for(int i=0;i<MAX_MATRIX_SIZE;i++)
	{
		*(current +i) = (GLdouble) *(m+i);
	}
}

void I_my_glLoadMatrixd(const GLdouble *m)
{
    // ...
	if(debug){std::cout<<"I_my_glLoadMatrixd"<<std::endl;}

	GLdouble *current = current_matrix;
	for(int i=0;i<MAX_MATRIX_SIZE;i++)
	{
		*(current +i) = *(m+i);
	}
}

void I_my_glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
    // ...
	if(debug){std::cout<<"I_my_glTranslated"<<std::endl;}
	GLdouble translation_matrix[16] = {
		1,0,0,x,
		0,1,0,y,
		0,0,1,z,
		0,0,0,1};

	matrix_transpose(translation_matrix);
	matrix_multiply(translation_matrix);
}

void I_my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	if(debug){std::cout<<"I_my_glTranslatef"<<std::endl;}
    I_my_glTranslated((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

// remember to normalize vector (x, y, z), and to convert angle from degree to radius before calling sin and cos
void I_my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    // ...
	if(debug){std::cout<<"I_my_glRotated"<<std::endl;}

	// normalize the vector
	normalize(&x,&y,&z);

	//change angle to radians
	GLdouble theta = (angle*(GLdouble)PI)/((GLdouble)180.0);

	GLdouble c = (GLdouble)cos(theta),
		s = (GLdouble)sin(theta),
		xx=x*x,
		xy=x*y,
		xz=x*z,
		yy=y*y,
		yz=y*z,
		zz=z*z;

	GLdouble general_rotation[MAX_MATRIX_SIZE];
	general_rotation[0] = (xx*(1-c))+c;
	general_rotation[1] = (xy*(1-c))-z*s;
	general_rotation[2] = (xz*(1-c))+y*s;
	general_rotation[3] = 0;
	general_rotation[4] = (xy*(1-c))+z*s;
	general_rotation[5] = (yy*(1-c))+c;
	general_rotation[6] = (yz*(1-c))-x*s;
	general_rotation[7] = 0;
	general_rotation[8] = (xz*(1-c))-y*s;
	general_rotation[9] = (yz*(1-c))+x*s;
	general_rotation[10] = (zz*(1-c))+c;
	general_rotation[11] = 0;
	general_rotation[12] = 0;
	general_rotation[13] = 0;
	general_rotation[14] = 0;
	general_rotation[15] = 1;

	matrix_transpose(general_rotation);
	matrix_multiply(general_rotation);
}

void I_my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	if(debug){std::cout<<"I_my_glRotatef"<<std::endl;}
    I_my_glRotated((GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void I_my_glScaled(GLdouble x, GLdouble y, GLdouble z)
{
    // ...
	if(debug){std::cout<<"I_my_glScaled"<<std::endl;}
	GLdouble scale_matrix[MAX_MATRIX_SIZE] = {
		x,0,0,0,
		0,y,0,0,
		0,0,z,0,
		0,0,0,1};
	matrix_transpose(scale_matrix);
	matrix_multiply(scale_matrix);
}

void I_my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	if(debug){std::cout<<"I_my_glScalef"<<std::endl;}
    I_my_glScaled((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

// copy current matrix to m
void I_my_glGetMatrixf(GLfloat *m)
{
    // ...
	GLdouble *current = current_matrix;
	for(int i=0;i<MAX_MATRIX_SIZE;i++)
	{
		*(m+i) = (GLfloat)*(current +i);
	}
}

void I_my_glGetMatrixd(GLdouble *m)
{
    // ...
	GLdouble *current = current_matrix;
	for(int i=0;i<MAX_MATRIX_SIZE;i++)
	{
		*(m+i) = *(current +i);
	}
}

// remember to normalize vectors0
void I_my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, 
    GLdouble centerX, GLdouble centerY, GLdouble centerZ, 
    GLdouble upX, GLdouble upY, GLdouble upZ)
{
	GLdouble look_at_matrix[MAX_MATRIX_SIZE];
	GLdouble fx,fy,fz,sx,sy,sz,upx,upy,upz;

	fx = centerX - eyeX;
    fy = centerY - eyeY;
    fz = centerZ - eyeZ;
	upx = upX;
	upy = upY;
	upz = upZ;

	normalize(&fx,&fy,&fz);
	//Side = f X up
	cross_product(&sx,&sy,&sz,fx,fy,fz,upx,upy,upz);
	normalize(&sx,&sy,&sz);

	//up = side x forward
	cross_product(&upx,&upy,&upz,sx,sy,sz,fx,fy,fz);

	look_at_matrix[0] = sx;
    look_at_matrix[1] = sy;
    look_at_matrix[2] = sz;
	look_at_matrix[3] = 0;

    look_at_matrix[4] = upx;
    look_at_matrix[5] = upy;
    look_at_matrix[6] = upz;
	look_at_matrix[7] = 0;

    look_at_matrix[8] = -fx;
    look_at_matrix[9] = -fy;
    look_at_matrix[10] = -fz;
	look_at_matrix[11] = 0;

	look_at_matrix[12] = 0;
    look_at_matrix[13] = 0;
    look_at_matrix[14] = 0;
	look_at_matrix[15] = 1;

	matrix_transpose(look_at_matrix);
	// Apply the new view matrix
	matrix_multiply( look_at_matrix );

	// Translate camera (eye) to origin
	I_my_glTranslatef( -eyeX, -eyeY, -eyeZ );
}

void I_my_glFrustum(GLdouble left, GLdouble right, GLdouble bottom,
    GLdouble top, GLdouble zNear, GLdouble zFar)
{
    // ...
	if((zNear<=0)||
		(zFar<=0)||
		(left==right)||
		(bottom == top)||
		(zNear == zFar))
	{
		std::cout<<"INVALID_VALUE"<<std::endl;
		return;
	}

	GLdouble frustum_matrix[MAX_MATRIX_SIZE] = {
		((2*zNear)/(right-left)),	0,	((right+left)/(right-left)),	0,
		0,	((2*zNear)/(top-bottom)),	((top+bottom)/(top-bottom)),	0,
		0,	0,	(-(zFar+zNear)/(zFar-zNear)),	((-2*zFar*zNear)/(zFar-zNear)),
		0,	0,	-1,	0
	};
	matrix_transpose(frustum_matrix);
	matrix_multiply(frustum_matrix);
}

//based on the inputs, calculate left, right, bottom, top, and call I_my_glFrustum accordingly
// remember to convert fovy from degree to radius before calling tan
void I_my_gluPerspective(GLdouble fovy, GLdouble aspect, 
    GLdouble zNear, GLdouble zFar)
{
    //
	GLdouble left, right, bottom, top;

    top = zNear * tan( fovy * PI / 360.0f );
    bottom = -top;
    left = bottom * aspect;
    right = top * aspect;

    I_my_glFrustum( left, right, bottom, top, zNear, zFar );
}
