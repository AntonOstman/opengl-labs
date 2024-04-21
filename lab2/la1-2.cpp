// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
	init ();

	//glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
#endif
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "LittleOBJLoader.h"
#include <math.h>

// Globals
// Data would normally be read from files
GLfloat t;
GLuint program;
Model *m;

/*GLfloat vertices[] =
{
  -0.5f,0.0f,0.5f, // 1
  -0.5f,0.0f,-0.5f, // 2
  0.5f,0.0f,-0.5f, // 3
  0.5f, 0.0f, 0.5f, // 4
  0.0f,0.5f,0.0f // 5
  };
*/


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
unsigned int vertexArrayObjID;

unsigned int bunnyVertexArrayObjID;

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

	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;
	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyTexCoordBufferObjID;

	//unsigned int triangleBufferObjID;
	// Reference to shader program

	dumpInfo();

	// BUNNY
	m = LoadModel("bunnyplus.obj");

	// GL inits
	glClearColor(0.2,0.2,0.4,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-2.vert", "lab1-2.frag");
	printError("init shader");

	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glGenBuffers(1, &bunnyVertexBufferObjID);
	glGenBuffers(1, &bunnyIndexBufferObjID);
	glGenBuffers(1, &bunnyNormalBufferObjID);

	glBindVertexArray(bunnyVertexArrayObjID);


	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	printError("vertex error");

	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    printError("normal bind error");
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	printError("normal data error");
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	printError("normal attribute error");
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

	printError("normal error");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	glGenBuffers(1, &bunnyTexCoordBufferObjID);


	if (m->texCoordArray != NULL)

	  {

	    glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);

	    glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);

	    glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);

	    glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));

	  }


	printError("index error");

/*
	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*18*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// End of upload of geometry
*/
	// Upload color
	/*glGenBuffers(1, &colorBufferObjID);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 4*18*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));
*/

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

	//glBindVertexArray(vertexArrayObjID);	// Select VAO
	//glDrawArrays(GL_TRIANGLES, 0, 18);	// draw object

	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L); // draw bunny

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