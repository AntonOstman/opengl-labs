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
Model *roof;
Model *balcony;
Model *walls;
Model *wing1;
Model *wing2;
Model *wing3;
Model *wing4;

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

struct GraphicsEntity{
    Model* model;
    mat4 translation;
    mat4 rotation;
    mat4 scaling;
    GraphicsEntity *next;
    GraphicsEntity *child;
} typedef GraphicsEntity;


GraphicsEntity* createGraphicsEntity(Model* model) {

    GraphicsEntity* entity = (GraphicsEntity*) malloc(sizeof(GraphicsEntity));
    entity->translation = mat4(1);
    entity->rotation = mat4(1);
    entity->scaling = mat4(1);

    entity->child = nullptr;
    entity->next = nullptr;
    entity->model = model;
    return entity;
}

void renderEntity(GraphicsEntity* entity) {

    GraphicsEntity* cur_entity = entity;
    mat4 scaling = mat4(1);
    mat4 new_scaling = mat4(1);

    while (!(cur_entity == nullptr)){

        new_scaling = scaling * cur_entity->translation * cur_entity->rotation* cur_entity->scaling;
        if (cur_entity->model != nullptr){
            glBindVertexArray(cur_entity->model->vao);    // Select VAO for bunny 1
            glUniformMatrix4fv(glGetUniformLocation(program, "scaleMatrix"), 1, GL_TRUE, new_scaling.m);
            glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L); // draw bunny
            printError("bind");
        }
        printError("render entity error");

        if (!(cur_entity->next == nullptr)){
            cur_entity = cur_entity->next;
        }
        else{
            scaling = new_scaling;
            cur_entity = cur_entity->child;
        }
    }
}

GraphicsEntity* windmill;
GraphicsEntity* Groof;
GraphicsEntity* Gbalcony;
GraphicsEntity* wingRotation;
GraphicsEntity* Gwing1;
GraphicsEntity* Gwing2;
GraphicsEntity* Gwing3;
GraphicsEntity* Gwing4;


void copymatrix(mat4* first, mat4* second){
    for (int i = 0; i < 16; i++){
        first->m[i] = second->m[i];
    }
}


vec3 p;
vec3 l;
vec3 v;
mat4 lookAtm;

mat4 xRotation(float theta){

    mat4 rotation = mat4(1, 0, 0, 0,
                         0, cos(theta), sin(theta), 0,
                         0, -sin(theta), cos(theta), 0,
                         0, 0, 0, 1);
    return rotation;
}

void init(void)
{
	// Reference to shader program
	dumpInfo();
	// BUNNY
	program = loadShaders("lab31.vert", "lab31.frag");
	printError("init shader");
    LoadTGATextureSimple("labskybox512.tga", &myTex);
    glBindTexture(GL_TEXTURE_2D, myTex);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
    glActiveTexture(GL_TEXTURE0);

	roof = LoadModel("windmill/windmill-roof.obj");
	walls = LoadModel("windmill/windmill-walls2.obj");
	balcony = LoadModel("windmill/windmill-balcony.obj");
	wing1 = LoadModel("windmill/blade.obj");
	wing2 = LoadModel("windmill/blade.obj");
	wing3 = LoadModel("windmill/blade.obj");
	wing4 = LoadModel("windmill/blade.obj");
	m = LoadModel("bunnyplus.obj");
	m2 = LoadModel("bunnyplus.obj");
	printError("bunny err");

    windmill = createGraphicsEntity(walls);
    Groof = createGraphicsEntity(roof);
    Gbalcony = createGraphicsEntity(balcony);
    wingRotation = createGraphicsEntity(nullptr);
    Gwing1 = createGraphicsEntity(wing1);
    Gwing2 = createGraphicsEntity(wing2);
    Gwing3 = createGraphicsEntity(wing3);
    Gwing4 = createGraphicsEntity(wing4);
    
    // windmill->translation = T(15.100021, 0.000000, 10.000000);
    float PI = 3.14; 
    float theta = PI/2;
    float wingx =  4.599998;
    float wingy = 9.199999;
    float wingz =  0.000000;
    wingRotation->translation = T(wingx, wingy, wingz);
    Gwing1->rotation = xRotation(PI/2);
    Gwing2->rotation = xRotation(2 * PI/2);
    Gwing3->rotation = xRotation(3 * PI/2);
    Gwing4->rotation = xRotation(4 * PI/2);

	// GL inits
	glClearColor(0.2,0.2,0.4,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader

    // createModelBuffers(m, myTex);

    p = vec3(0, 0, 10.f);
    l = vec3(0.f, 0.f, 0.f);
    v = vec3(0, 1.f, 0.f);
    // printf("p.x %f", p.z);
    lookAtm = lookAtv(p, l, v);
    // copymatrix(&lookAtm, &new_look);
    // printf("look %f", lookAtm.m[0]);
    
    DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(m2, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(roof, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(wing1, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(wing2, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(wing3, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(wing4, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(walls, program, "in_Position", "in_Normal", "inTexCoord");
    DrawModel(balcony, program, "in_Position", "in_Normal", "inTexCoord");


    windmill->child = Gbalcony;
    Gbalcony->next = Groof;
    Groof->next = wingRotation;
    wingRotation->child = Gwing1;
    Gwing1->next = Gwing2;
    Gwing2->next = Gwing3;
    Gwing3->next = Gwing4;


	// Z buffer init
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);


	printError("init arrays");
}

void keyPressed(unsigned char key, int x, int y);

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// upload matrix

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	printError("projection");

    mat4 ones = mat4(1);
     
    // mat4 new_look = lookAtv(p,l,v);
    // mat4 lookAtm;
    // copymatrix(&lookAtm, &new_look);

    glUniformMatrix4fv(glGetUniformLocation(program, "lookAt"), 1, GL_TRUE, lookAtm.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotTrans"), 1, GL_TRUE, ones.m);
    wingRotation->rotation = xRotation(t/1000);

    glutKeyboardFunc(keyPressed);
    renderEntity(windmill);


	printError("upload matrix error");


	printError("display");

	glutSwapBuffers();
}

void translate(float x, float y, float z, mat4* matrix){
    matrix->m[3] = matrix->m[3] + x;
    matrix->m[7] = matrix->m[7] + y; 
    matrix->m[11] = matrix->m[11] + z; 
    printf("x: %f, y: %f, z: %f\n", matrix->m[3], matrix->m[7], matrix->m[11]);
}

void move(unsigned char key, mat4* matrix){
    if (key == 'x'){
        translate(0.1, 0.0, 0.0, matrix);
    }
    if (key == 'y'){
       translate(0.0, 0.1 ,0.0, matrix);
    }
    if (key == 'z'){
        translate(0.0, 0, 0.1, matrix);
    }
    if (key == 'X'){
        translate(-0.1, 0, 0, matrix);
    }
    if (key == 'Y'){
        translate(0.0, -0.1, 0.0, matrix);
    }
    if (key == 'Z'){
        translate(0.0, 0.0, -0.1, matrix);
    }

}


unsigned char prev_key;

void keyPressed(unsigned char key, int xx, int yy) {

    if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5'){
        prev_key = key;
    }
    switch (prev_key){
        case '1':
            move(key, &(windmill->translation));
        break;
        case '2':
            move(key, &(Groof->translation));
        break;
        case '3':
            move(key, &(Gbalcony->translation));
        break;
        case '4':
            move(key, &(Gwing1->translation));
            move(key, &(Gwing2->translation));
            move(key, &(Gwing3->translation));
            move(key, &(Gwing4->translation));
            break;
        case '5':
            mat4 pos = mat4(1);
            pos.m[3] = p.x;
            pos.m[7] = p.y;
            pos.m[11] = p.z;
            move(key, &pos);
            p = vec3(pos.m[3], pos.m[7], pos.m[11]);
            // mat4 new_look = lookAtv(p, l, v);
            // copymatrix(&lookAtm, &new_look);
            lookAtm = lookAtv(p, l, v);
            break;
        break;
    }
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
