/*////////////////////////////////////////////////////////////////////////////////////
set up the shaders
////////////////////////////////////////////////////////////////////////////////////*/

#include<stdio.h>
#include "inc\essentials.h"
#include "inc\textfile.h"

// Shader Names
char *vertexFileName = "C:\\Users\\evan\\Documents\\Visual Studio 2012\\Projects\\openglShading\\Debug\\color.vert";
char *fragmentFileName = "C:\\Users\\evan\\Documents\\Visual Studio 2012\\Projects\\openglShading\\Debug\\color.frag";
 
// Vertex Attribute Locations
GLuint vertexLoc, colorLoc, normalLoc;
 
// Uniform variable Locations
GLuint projMatrixLoc, viewMatrixLoc, cLoc, LightPosLoc, LightDifLoc, LightAmbLoc, LightSpecLoc, eyeLoc;

#define printOpenGLError() printOglError(__FILE__, __LINE__)
 
int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;
 
    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}
 
void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
GLuint setupShaders() {
 
    char *vs = NULL,*fs = NULL,*fs2 = NULL;
 
    GLuint p,v,f;
 
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
 
    vs = textFileRead(vertexFileName);
    fs = textFileRead(fragmentFileName);
 
	printf("%s\n",vs);
	printf("----------------\n");
	printf("%s\n",fs);
	int a=0;
	scanf("%d",&a);

    const char * vv = vs;
    const char * ff = fs;
 
    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);
 
    free(vs);free(fs);
 
    glCompileShader(v);
    glCompileShader(f);
 
    printShaderInfoLog(v);
    printShaderInfoLog(f);
 
    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
 
    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    printProgramInfoLog(p);
 
    vertexLoc = glGetAttribLocation(p,"position");
	normalLoc = glGetAttribLocation(p,"normal");
 
    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");

	LightPosLoc =  glGetUniformLocation(p, "LightPos");
	LightDifLoc =  glGetUniformLocation(p, "LightDif");
	LightAmbLoc =  glGetUniformLocation(p, "LightAmb");
	LightSpecLoc = glGetUniformLocation(p, "LightSpec");

	eyeLoc = glGetUniformLocation(p,"eye");

    cLoc = glGetUniformLocation(p, "color");
 
    return(p);
}
 