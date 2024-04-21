// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.

#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "LoadTGA.h"
#include "LittleOBJLoader.h"
#include <math.h>
#include "VectorUtils4.h"
// Globals
// Data would normally be read from files
// void LoadTGATextureSimple(char *filename, GLuint *tex);
GLfloat t;
GLuint program;
GLuint myTex;
Model *m;
Model *m2;

/*GLfloat vertices[] =
{
  -0.5f,0.0f,0.5f, //  1
  -0.5f,0.0f,-0.5f, // 2
  0.5f,0.0f,-0.5f, //  3
  0.5f, 0.0f, 0.5f, // 4
  0.0f,0.5f,0.0f //    5
  };
*/
#define near 1.0

#define far 30.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5

GLfloat projectionMatrix[] = {2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                              0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                              0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                              0.0f, 0.0f, -1.0f, 0.0f };

GLfloat triangles[] =
{
  1,2,3,
  1,3,4,
  1,2,5,
  2,3,5,
  3,4,5,
  1,4,5
};

GLfloat vertices[] =
{
  -0.5f,0.0f,0.5f, // 1
  -0.5f,0.0f,-0.5f, // 2
  0.5f,0.0f,-0.5f, // 3

  -0.5f,0.0f,0.5f, // 1
  0.5f,0.0f,-0.5f, // 3
  0.5f, 0.0f, 0.5f, // 4

  -0.5f,0.0f,0.5f, // 1
  -0.5f,0.0f,-0.5f, // 2
  0.0f,0.5f,0.0f, // 5

 -0.5f,0.0f,-0.5f, // 2
  0.5f,0.0f,-0.5f, // 3
  0.0f,0.5f,0.0f, // 5

  0.5f,0.0f,-0.5f, // 3
  0.5f, 0.0f, 0.5f, // 4
  0.0f,0.5f,0.0f, // 5

  -0.5f,0.0f,0.5f, // 1
  0.5f, 0.0f, 0.5f, // 4
  0.0f,0.5f,0.0f // 5

};


GLfloat colors[] =
{
  1.0f, 0.0f, 0.0f, 1.0f, // 1
  0.0f, 1.0f, 0.0f, 1.0f, // 2
  0.0f, 0.0f, 1.0f, 1.0f, // 3

  1.0f, 0.0f, 0.0f, 1.0f, // 1
  0.0f, 0.0f, 1.0f, 1.0f, // 3
  0.5f, 0.5f, 0.0f, 1.0f, // 4

  1.0f, 0.0f, 0.0f, 1.0f, // 1
  0.0f, 1.0f, 0.0f, 1.0f, // 2
  0.0f, 0.5f, 0.5f, 1.0f, // 5

  0.0f, 1.0f, 0.0f, 1.0f, // 2
  0.0f, 0.0f, 1.0f, 1.0f, // 3
  0.0f, 0.5f, 0.5f, 1.0f, // 5

  0.0f, 0.0f, 1.0f, 1.0f, // 3
  0.5f, 0.5f, 0.0f, 1.0f, // 4
  0.0f, 0.5f, 0.5f, 1.0f, // 5

  1.0f, 0.0f, 0.0f, 1.0f, // 1
  0.5f, 0.5f, 0.0f, 1.0f, // 4
  0.0f, 0.5f, 0.5f, 1.0f // 5
};


// translation matrix in x-axis
GLfloat myMatrix[] = { 1.0f, 0.0f, 0.0f, 1.0f,

                        0.0f, 1.0f, 0.0f, 0.0f,

                        0.0f, 0.0f, 1.0f, 0.0f,

                        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat scaleMatrix[] = {
  0.5f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
};

GLfloat yrotMatrix[] = {
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
};


// vertex array object
// unsigned int vertexArrayObjID;

// unsigned int bunnyVertexArrayObjID;

void OnTimer(int value)
{
  glutPostRedisplay();

  glutTimerFunc(20, &OnTimer, value);
}


void init(void)
{
	// vertex buffer object, used for uploading the geometry
	//unsigned int vertexBufferObjID;
	//unsigned int colorBufferObjID;

	// unsigned int bunnyIndexBufferObjID;
	// unsigned int bunnyNormalBufferObjID;
	// unsigned int bunnyVertexBufferObjID;
	// unsigned int bunnyTexCoordBufferObjID;
	//unsigned int triangleBufferObjID;
	// Reference to shader program

	dumpInfo();

	// BUNNY
	m = LoadModel("bunnyplus.obj");
	m2 = LoadModel("bunnyplus.obj");

	// GL inits
	glClearColor(0.2,0.2,0.4,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-3.vert", "lab2-3.frag");
	printError("init shader");

	glGenVertexArrays(1, &m->vao);
	glGenBuffers(1, &m->vb);
	glGenBuffers(1, &m->nb);
	glGenBuffers(1, &m->tb);
	glBindVertexArray(m->vao);
	// // VBO for vertex data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->vao);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
    printError("drawbunny err");

	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);

    printError("drawbunny err");
	glBindBuffer(GL_ARRAY_BUFFER, m->nb);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    printError("drawbunny err");


    LoadTGATextureSimple("maskros512.tga", &myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
    glActiveTexture(GL_TEXTURE0);

	if (m->texCoordArray != NULL){
        glBindBuffer(GL_ARRAY_BUFFER, m->tb);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
    }

	glGenVertexArrays(1, &m2->vao);
	glGenBuffers(1, &m2->vb);
	glGenBuffers(1, &m2->nb);
	glGenBuffers(1, &m2->tb);
	glBindVertexArray(m2->vao);
	// // VBO for vertex data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m2->vao);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m2->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
    printError("drawbunny err");

	glBindBuffer(GL_ARRAY_BUFFER, m2->vb);
	glBufferData(GL_ARRAY_BUFFER, m2->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);

    printError("drawbunny err");
	glBindBuffer(GL_ARRAY_BUFFER, m2->nb);
	glBufferData(GL_ARRAY_BUFFER, m2->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    printError("drawbunny err");


    LoadTGATextureSimple("maskros512.tga", &myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
    glActiveTexture(GL_TEXTURE0);

	if (m->texCoordArray != NULL){
        glBindBuffer(GL_ARRAY_BUFFER, m2->tb);
        glBufferData(GL_ARRAY_BUFFER, m2->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
    }
    DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(m2, program, "in_Position", "in_Normal", "inTexCoord");

    



	// Z buffer init
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);


	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// upload matrix

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	yrotMatrix[0] = cos(t/1000);
	yrotMatrix[2] = sin(t/1000);
	yrotMatrix[8] = -sin(t/1000);
	yrotMatrix[10] = cos(t/1000);

	glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_TRUE, yrotMatrix);
	printError("projection");

    mat4 rot, trans, total;
    trans = T(0, 0, 0);
    // rot = Ry(a);
    total = trans;

    vec3 p;
    vec3 l;
    vec3 v;

    p = vec3(-1.f, 7 * cos(t/1000), 7 * sin(t/1000));
    l = vec3(0.f, 0.f, 0.f);
    v = vec3(0, 1.f, 0.f);
    mat4 lookAt = lookAtv(p, l, v);

    glUniformMatrix4fv(glGetUniformLocation(program, "lookAt"), 1, GL_TRUE, lookAt.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotTrans"), 1, GL_TRUE, total.m);

	glBindVertexArray(m->vao);    // Select VAO for bunny 1
	glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_TRUE, yrotMatrix);
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L); // draw bunny

	yrotMatrix[0] = -cos(t/1000);
	yrotMatrix[2] = -sin(t/1000);
	yrotMatrix[8] = sin(t/1000);
	yrotMatrix[10] = -cos(t/1000);

    trans = T(3, 0, 0);
    total = trans; 

	glBindVertexArray(m2->vao);    // Select VAO for bunny 2
    glUniformMatrix4fv(glGetUniformLocation(program, "rotTrans"), 1, GL_TRUE, total.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_TRUE, yrotMatrix);
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L); // draw bunny


	printError("upload matrix error");


	printError("display");

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();

	glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
	return 0;
}
