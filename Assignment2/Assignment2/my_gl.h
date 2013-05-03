
// the following definitions overwrite original OpenGL functions with my_gl
//working so far
#define glMatrixMode my_glMatrixMode
#define glLoadIdentity my_glLoadIdentity
#define glPushMatrix my_glPushMatrix
#define glPopMatrix my_glPopMatrix
#define glLoadMatrixf my_glLoadMatrixf
#define glLoadMatrixd my_glLoadMatrixd

//fixed after transposing matrices
#define glTranslated my_glTranslated
#define glTranslatef my_glTranslatef

//working after fixed matrix multiplication function
#define glRotated my_glRotated
#define glRotatef my_glRotatef

//works just fine
#define glScaled my_glScaled
#define glScalef my_glScalef
//find out if the stack has to be a certain pointer and how to
#define gluLookAt my_gluLookAt
#define glFrustum my_glFrustum
//need to check whats happenging here
#define gluPerspective my_gluPerspective

// the following declarations link application to my_gl
void my_glMatrixMode(GLenum mode);
void my_glLoadIdentity(void);
void my_glPushMatrix(void);
void my_glPopMatrix(void);
void my_glLoadMatrixf(const GLfloat *m);
void my_glLoadMatrixd(const GLdouble *m);
void my_glTranslated(GLdouble x, GLdouble y, GLdouble z);
void my_glTranslatef(GLfloat x, GLfloat y, GLfloat z);
void my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void my_glScaled(GLdouble x, GLdouble y, GLdouble z);
void my_glScalef(GLfloat x, GLfloat y, GLfloat z);
void my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
    GLdouble centerX, GLdouble centerY, GLdouble centerZ,
    GLdouble upX, GLdouble upY, GLdouble upZ);
void my_glFrustum(GLdouble left, GLdouble right, 
    GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void my_gluPerspective(GLdouble fovy, GLdouble aspect,
    GLdouble zNear, GLdouble zFar);
