/*////////////////////////////////////////////////////////////////////////////////////
display module for the renderer
////////////////////////////////////////////////////////////////////////////////////*/

#include<stdio.h>
#include "inc\essentials.h"

//---------------------models------------------------------
extern PLYObject *model;
//---------------------------------------------------------

//------external variables for the shader------------------------------
extern GLuint vertexLoc, colorLoc,normalLoc;
extern GLuint projMatrixLoc, viewMatrixLoc, cLoc, LightPosLoc, LightDifLoc, LightAmbLoc, LightSpecLoc, eyeLoc;
extern GLuint ProgramID,VertexID,FragmentID;
//---------------------------------------------------------------------


//-----------global variables for setting up the scene-----------------
static const double TwoPi = 2.0 * 3.1415926;
double theta = 0;
double phi   = 0;//TwoPi/4;
double dist  = 8.00;
double znear = 5.0;
double zfar  = 20.0;
double zoom=0.0;
double alpha=0.0;
Vec3 eye;
Vec3 origin( 0, 0, 0 );
Vec3 up( 1, 1,0 );//default up vector specified by user
double L_theta = 0.2, L_phi = 1.6, L_dist = 2.0, shininess = 30;
float projMatrix[16];// storage for Matrices
float viewMatrix[16];// storage for Matrices
float LightPosition[4];
float DiffuseLight[4], AmbientLight[4], SpecularLight[4];
extern int Width,Height;
extern unsigned int FrameCount;
extern int MODE;
//-----------------------------------------------------------------------


//--------Vertex and Index buffers for rendering-------------------------
GLuint VertexBuffer, IndexBuffer,NormalBuffer;
// Vertex Array Objects Identifiers
GLuint VAO;
//-----------------------------------------------------------------------

 
//---------routines for setting up view-----------------------------------
void setIdentityMatrix( float *mat, int size) {
    for (int i = 0; i < size * size; ++i)mat[i] = 0.0f;
    for (int i = 0; i < size; ++i) mat[i + i * size] = 1.0f;
}
void setIdentityMatrix4x4( float *mat) {
	int size=4;
    for (int i = 0; i < size * size; ++i) mat[i] = 0.0f;
    for (int i = 0; i < size; ++i) mat[i + i * size] = 1.0f;
}
void multMatrix(float *a, float *b) {
    float res[16];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));
}
 
// Defines a transformation matrix mat with a translation
void setTranslationMatrix(float *mat, float x, float y, float z) {
    setIdentityMatrix(mat,4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}
 
// ----------------------------------------------------
// Projection Matrix
//
 
void buildProjectionMatrix(float fov, float ratio, float nr, float fr) {
 
    double f = 1.0f / tan (fov * (TwoPi / 720.0));
 
    setIdentityMatrix(projMatrix,4);
 
    projMatrix[0] = float(f) / ratio;
    projMatrix[1 * 4 + 1] = float(f);
    projMatrix[2 * 4 + 2] = (fr + nr) / (nr - fr);
    projMatrix[3 * 4 + 2] = (2.0f * fr * nr) / (nr - fr);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;
}
 
// ----------------------------------------------------
// View Matrix
//
//
 
void setCamera() {
	Vec3 dir(Unit(origin-eye));
	Vec3 right(Unit(dir^up));
	up=Unit(right^dir);
 
    float aux[16];
 
    viewMatrix[0]  = float(right.x);
    viewMatrix[4]  = float(right.y);
    viewMatrix[8]  = float(right.z);
    viewMatrix[12] = 0.0f;
 
    viewMatrix[1]  = float(up.x);
    viewMatrix[5]  = float(up.y);
    viewMatrix[9]  = float(up.z);
    viewMatrix[13] = 0.0f;
 
    viewMatrix[2]  = float(-dir.x);
    viewMatrix[6]  = float(-dir.y);
    viewMatrix[10] = float(-dir.z);
    viewMatrix[14] =  0.0f;
 
    viewMatrix[3]  = 0.0f;
    viewMatrix[7]  = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;

	setTranslationMatrix(aux, float(-eye.x), float(-eye.y), float(-eye.z));
    multMatrix(viewMatrix, aux);
		//set the projection matrix
	double d(1-zoom);
	setIdentityMatrix4x4(projMatrix);
	projMatrix[0]=float(1/d);
	projMatrix[5]=float(1/d);
	projMatrix[10]=float(-2/(zfar-znear));
	projMatrix[14]=float(-(zfar+znear)/(zfar-znear));
}
 
// ----------------------------------------------------

 
void changeSize(int w, int h) {
	Width=w;Height=h;
    float ratio;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;
 
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
 
    ratio = (1.0f * w) / h;
    buildProjectionMatrix(53.13f, ratio, 1.0f, 30.0f);
}


void setupBuffers() {
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(1, &VertexBuffer);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*model->nv*sizeof(float), model->vrt.getVertices(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
 
	glGenBuffers(1, &NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*model->nv*sizeof(float), model->vrt.getNormals(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*model->nf*sizeof(int), model->fac.getIndices(), GL_DYNAMIC_DRAW);
	glBindVertexArray(0);

	GLenum ErrorCheckValue = glGetError();
	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
		);

		exit(-1);
	}
}
 

void setLighting(void) 
{
    DiffuseLight[0] = 0.6; DiffuseLight[1] = 0.6;DiffuseLight[2] = 0.6;DiffuseLight[3] = 1.0;
    AmbientLight[0]= 0.2;AmbientLight[1]= 0.2;AmbientLight[2]= 0.2;AmbientLight[3]= 1.0;
    SpecularLight[0]= 1.0;SpecularLight[1]= 1.0;SpecularLight[2]= 1.0;SpecularLight[3]= 1.0;
    
	Vec3 lgt( cos( L_theta ) * cos( L_phi ), sin( L_theta ) * cos( L_phi ), sin( L_phi ) );
	//printf("%f %f %f\n",cos( L_theta ) * cos( L_phi ), sin( L_theta ) * cos( L_phi ), sin( L_phi ) );
	Vec3 LP=L_dist * lgt;
	LightPosition[0]=LP.x; //set the LightPosition to the specified values
	LightPosition[1]=LP.y;
	LightPosition[2]=LP.z;
	//Vec3 LP=L_dist * eye;
	//LightPosition[0]=-LP.x; //set the LightPosition to the specified values
	//LightPosition[1]=-LP.y;
	///LightPosition[2]=-LP.z;
	LightPosition[3]=0;

}

void display(void) {
	++FrameCount;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	eye=Vec3( dist * cos( theta ) * (cos( phi )), dist * sin( theta ) * (cos( phi )), dist * sin( phi ) );
	
	setCamera();
	setLighting();
    //setCamera();

	float EYE[]={eye.x,eye.y,eye.z};
 
    glUseProgram(ProgramID);
    glUniformMatrix4fv(projMatrixLoc,  1, false, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, false, viewMatrix);
	glUniform4fv(LightPosLoc, 1, LightPosition);
	glUniform4fv(LightDifLoc, 1, DiffuseLight);
	glUniform4fv(LightAmbLoc, 1, AmbientLight);
	glUniform4fv(LightSpecLoc, 1, SpecularLight);
	glUniform3fv(eyeLoc, 1, EYE);
 
    glBindVertexArray(VAO);
	if(MODE==0){
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glDrawElements(GL_TRIANGLES,3*model->nf,GL_UNSIGNED_INT,(GLvoid*)0);
	}
	else if(MODE==1){
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glDrawElements(GL_TRIANGLES,3*model->nf,GL_UNSIGNED_INT,(GLvoid*)0);

	}
	else if(MODE==2){
		glUniform3f(cLoc,0,0,0);
		glDrawElements(GL_POINTS,3*model->nf,GL_UNSIGNED_INT,(GLvoid*)0);
	}


    glutSwapBuffers();
}
  