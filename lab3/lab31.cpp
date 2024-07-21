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

struct GraphicsEntity{
    Model* model;
    mat4 translation;
    mat4 rotation;
    mat4 scaling;
    GraphicsEntity *next;
    GraphicsEntity *child;
} typedef GraphicsEntity;

#define kGroundSize 1000.f

vec3 vertices[] =
{
 vec3(-kGroundSize,0.0f,-kGroundSize),
 vec3(-kGroundSize,0.0f,kGroundSize),
 vec3(kGroundSize,-0.0f,-kGroundSize),
 vec3(kGroundSize,-0.0f,kGroundSize)
};

vec3 vertex_normals[] =
{
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f),
  vec3(0.0f,1.0f,0.0f)
};

vec2 tex_coords[] =
{
  vec2(0.0f,0.0f),
  vec2(0.0f,20.0f),
  vec2(20.0f,0.0f),
  vec2(20.0f,20.0f)
};

GLuint indices[] =
{
  0, 1, 2, 1, 3, 2
};

GLfloat t;
GLuint program;
GLuint skyboxshader;
GLuint myTex;
GLuint myTex2;

Model *roof;
Model *balcony;
Model *walls;
Model *wing1;
Model *wing2;
Model *wing3;
Model *wing4;

Model *ground;
Model *skybox;

GraphicsEntity* windmill;
GraphicsEntity* Groof;
GraphicsEntity* Gbalcony;
GraphicsEntity* wingRotation;
GraphicsEntity* Gwing1;
GraphicsEntity* Gwing2;
GraphicsEntity* Gwing3;
GraphicsEntity* Gwing4;

GraphicsEntity* Gskybox;
GraphicsEntity* Gground;

GraphicsEntity* createGraphicsEntity(Model* model) {

    // Fet memory leak inc.
    GraphicsEntity* entity = (GraphicsEntity*) malloc(sizeof(GraphicsEntity));
    entity->translation = mat4(1);
    entity->rotation = mat4(1);
    entity->scaling = mat4(1);

    entity->child = nullptr;
    entity->next = nullptr;
    entity->model = model;
    return entity;
}

void OnTimer(int value)
{
  glutPostRedisplay();

  glutTimerFunc(20, &OnTimer, value);
}

void renderEntity(GraphicsEntity* entity, GLuint shader) {

    GraphicsEntity* cur_entity = entity;
    mat4 scaling = mat4(1);
    mat4 new_scaling = mat4(1);

    while (!(cur_entity == nullptr)){

        new_scaling = scaling * cur_entity->translation * cur_entity->rotation * cur_entity->scaling;
        if (cur_entity->model != nullptr){
            glBindVertexArray(cur_entity->model->vao);    // Select VAO
            glUniformMatrix4fv(glGetUniformLocation(shader, "scaleMatrix"), 1, GL_TRUE, new_scaling.m);
            glDrawElements(GL_TRIANGLES, cur_entity->model->numIndices, GL_UNSIGNED_INT, 0L); // draw element on VAO
            printError("error binding");
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


void copymatrix(mat4* first, mat4* second){
    for (int i = 0; i < 16; i++){
        first->m[i] = second->m[i];
    }
}

mat4 r = mat4(1);
vec3 p;
vec3 l;
vec3 v;
mat4 lookAtm;
mat4 translated_lookAtm;

mat4 xRotation(float theta){

    mat4 rotation = mat4(1, 0, 0, 0,
                         0, cos(theta), sin(theta), 0,
                         0, -sin(theta), cos(theta), 0,
                         0, 0, 0, 1);
    return rotation;
}

void mouseCallback(int x, int y);

void createWindmill(GLuint shaderprog){

	roof = LoadModel("windmill/windmill-roof.obj");
	walls = LoadModel("windmill/windmill-walls2.obj");
	balcony = LoadModel("windmill/windmill-balcony.obj");
	wing1 = LoadModel("windmill/blade.obj");
	wing2 = LoadModel("windmill/blade.obj");
	wing3 = LoadModel("windmill/blade.obj");
	wing4 = LoadModel("windmill/blade.obj");

	printError("bunny err");

    windmill = createGraphicsEntity(walls);
    Groof = createGraphicsEntity(roof);
    Gbalcony = createGraphicsEntity(balcony);
    wingRotation = createGraphicsEntity(nullptr);
    Gwing1 = createGraphicsEntity(wing1);
    Gwing2 = createGraphicsEntity(wing2);
    Gwing3 = createGraphicsEntity(wing3);
    Gwing4 = createGraphicsEntity(wing4);
    DrawModel(roof, shaderprog,    "in_Position", "in_Normal", nullptr);
    DrawModel(wing1, shaderprog,   "in_Position", "in_Normal", nullptr);
    DrawModel(wing2, shaderprog,   "in_Position", "in_Normal", nullptr);
    DrawModel(wing3, shaderprog,   "in_Position", "in_Normal", nullptr);
    DrawModel(wing4, shaderprog,   "in_Position", "in_Normal", nullptr);
    DrawModel(walls, shaderprog,   "in_Position", "in_Normal", nullptr);
    DrawModel(balcony, shaderprog, "in_Position", "in_Normal", nullptr);

	printError("windmill err");

    float PI = 3.14; 
    float wingx =  4.599998;
    float wingy = 9.199999;
    float wingz =  0.000000;
    wingRotation->translation = T(wingx, wingy, wingz);
    Gwing1->rotation = xRotation(PI/2);
    Gwing2->rotation = xRotation(2 * PI/2);
    Gwing3->rotation = xRotation(3 * PI/2);
    Gwing4->rotation = xRotation(4 * PI/2);

    windmill->child = Gbalcony;
    Gbalcony->next = Groof;
    Groof->next = wingRotation;
    wingRotation->child = Gwing1;
    Gwing1->next = Gwing2;
    Gwing2->next = Gwing3;
    Gwing3->next = Gwing4;
}

void createGround(GLuint shaderprog){

    ground = LoadDataToModel(vertices, vertex_normals, NULL, nullptr, indices, 4, 6);
    DrawModel(ground, shaderprog, "in_Position", "in_Normal", nullptr);
    Gground = createGraphicsEntity(ground);

	printError("ground err");
}

void createSkyBox(GLuint shaderprog){

	skybox = LoadModel("new-skyboxes/skybox.obj");
    Gskybox = createGraphicsEntity(skybox);
    /*Gskybox->scaling = mat4(1000);*/
    /*Gskybox->translation = T(0,0,0);*/
    /*Gskybox->rotation = Rz(3.14);*/
    Gskybox->translation = T(0,0,10);
    /*Gskybox->scaling.m[5] = 4;*/
    /*Gskybox->scaling.m[10] = 4;*/
    /*Gskybox->scaling.m[15] = 1;*/

    DrawModel(skybox, shaderprog, "in_Position", nullptr, "inTexCoord");
	printError("skybox err");
}

void activateTexture(const char* texname, unsigned int unit, GLuint tex, GLuint shader){

    GLuint glTextureUnit = GL_TEXTURE0 + unit;
    if (glTextureUnit > 0x84DF){
        printf("to many texture units\n");
        exit(1);
    }

    /*glUniform1i(glGetUniformLocation(program, "texUnit"), unit); // Texture unit 1*/
    glActiveTexture(glTextureUnit);
    glUniform1i(glGetUniformLocation(shader, "texUnit"), unit); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, tex);

	printError("activate tex err");
}

void clearTexture(unsigned int unit){
    GLuint glTextureUnit = GL_TEXTURE0 + unit;

    glActiveTexture(glTextureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void init(void)
{
    printf("start program\n");
	// Reference to shader program
	dumpInfo();
	/*program = loadShaders("lab31.vert", "lab31.frag");*/
	/*skyboxshader = loadShaders("skyboxshader.vert", "skyboxshader.frag");*/
	skyboxshader = loadShaders("simple.vert", "simple.frag");
	program = loadShaders("simple.vert", "simple.frag");
	printError("init shader");

    LoadTGATextureSimple("labskybox512.tga", &myTex);
    LoadTGATextureSimple("maskros512.tga", &myTex2);

	// GL inits
	glClearColor(0.2,0.2,0.4,0);
	/*glDisable(GL_DEPTH_TEST);*/
	printError("GL inits");

    p = vec3(0, 0, 10.f);
    l = vec3(0.f, 0.f, 0.f);
    v = vec3(0, 1.f, 0.f);
    translated_lookAtm = lookAtv(p, l, v);
    lookAtm = lookAtv(p, l, v);

    createGround(program);
    createWindmill(program);
    createSkyBox(skyboxshader);
    
    glutMotionFunc(mouseCallback);
	printError("init arrays");
}


void keyPressed(unsigned char key, int x, int y);

void renderEntities(){

	// upload matrix
    glUniformMatrix4fv(glGetUniformLocation(skyboxshader, "lookAt"), 1, GL_TRUE, (r * lookAtv(p, l, v)).m);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
    Gskybox->rotation = inverse(r);
    activateTexture("labskybox512.tga", 0, myTex, skyboxshader);
    renderEntity(Gskybox, skyboxshader);
    clearTexture(0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	printError("skybox error");

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    glUniformMatrix4fv(glGetUniformLocation(program, "lookAt"), 1, GL_TRUE, lookAtm.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix);
	printError("upload matrix error");
    wingRotation->rotation = xRotation(t/1000);


    activateTexture("maskros512.tga", 0, myTex2, program);
    renderEntity(Gground, program);
    renderEntity(windmill, program);
    clearTexture(0);
	printError("maskros error");
	printError("Render::error");

}

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderEntities();

    glutKeyboardFunc(keyPressed);

	printError("display");
	glutSwapBuffers();
}

void translate(float x, float y, float z, mat4* matrix){
    matrix->m[3] = matrix->m[3] + x;
    matrix->m[7] = matrix->m[7] + y; 
    matrix->m[11] = matrix->m[11] + z; 
    printf("x: %f, y: %f, z: %f\n", matrix->m[3], matrix->m[7], matrix->m[11]);
}


void move(unsigned char key, mat4* matrix, const char* keyset){

    if (key == keyset[0]){
        translate(0.1, 0.0, 0.0, matrix);
    }
    if (key == keyset[1]){
       translate(0.0, 0.1 ,0.0, matrix);
    }
    if (key == keyset[2]){
        translate(0.0, 0, 0.1, matrix);
    }
    if (key == keyset[3]){
        translate(-0.1, 0, 0, matrix);
    }
    if (key == keyset[4]){
        translate(0.0, -0.1, 0.0, matrix);
    }
    if (key == keyset[5]){
        translate(0.0, 0.0, -0.1, matrix);
    }
}


unsigned char prev_key;

void keyPressed(unsigned char key, int xx, int yy) {

    const char* keyset = "aYwdys";
    if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5'){
        prev_key = key;
    }
    switch (prev_key){
        case '1':
            move(key, &(windmill->translation), keyset);
        break;
        case '2':
            move(key, &(Groof->translation), keyset);
        break;
        case '3':
            move(key, &(Gbalcony->translation), keyset);
        break;
        case '4':
            move(key, &(Gwing1->translation), keyset);
            move(key, &(Gwing2->translation), keyset);
            move(key, &(Gwing3->translation), keyset);
            move(key, &(Gwing4->translation), keyset);
            break;
        case '5':
            mat4 pos = mat4(0);
            move(key, &pos, keyset);
            pos = r * pos;
            translated_lookAtm.m[3] += pos.m[3];
            translated_lookAtm.m[7] += pos.m[7];
            translated_lookAtm.m[11] += pos.m[11];
            lookAtm = inverse(r) * translated_lookAtm;
            break;
        break;
    }
}

int prevx = -1;
int prevy = -1;
float anglex = 0;
float angley = 0;

void mouseCallback(int x, int y){

    if (prevx == -1){
        prevx = x;
        prevy = y;
    }
    int dx = prevx - x;
    int dy = prevy - y;

    if (abs(dx) > 5 || abs(dy) > 5)
    {
        dx = 0;
        dy = 0;
    }

    printf("%d %d %f %f \n", dx, dy, anglex, angley);

    anglex += ((float)dx) / 500.f;
    angley += ((float)dy) / 500.f;

    mat3 rx = mat3(1);
    mat3 ry = mat3(1);

    rx = Rx(angley);
    ry = Ry(anglex);
    r = ry * rx;
    printVec3(l);
    prevx = x;
    prevy = y;
    vec3 camt = vec3(lookAtm.m[3], lookAtm.m[7], lookAtm.m[11]);
    lookAtm = inverse(r) * translated_lookAtm;
    printMat4(lookAtm);
}

int main(int argc, char *argv[])
{

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);

	glutCreateWindow("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();

	glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
	return 0;
}
