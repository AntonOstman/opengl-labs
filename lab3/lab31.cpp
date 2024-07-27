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

#define far 1000.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5

vec3 lightSourcesColorsArr[] = { vec3(1.0f, 0.0f, 0.0f), // Red light
                                 vec3(0.0f, 1.0f, 0.0f), // Green light
                                 vec3(0.0f, 0.0f, 1.0f), // Blue light
                                 vec3(1.0f, 1.0f, 1.0f) }; // White light

GLint isDirectional[] = {0,0,1,1};

vec3 lightSourcesDirectionsPositions[] = { vec3(10.0f, 5.0f, 0.0f), // Red light, positional
                                       vec3(0.0f, 5.0f, 10.0f), // Green light, positional
                                       vec3(-1.0f, 0.0f, 0.0f), // Blue light along X
                                       vec3(0.0f, 0.0f, -1.0f) }; // White light along Z

GLfloat specularExponent[] = {100.0, 200.0, 60.0};
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
    const char* in_pos;
    const char* in_normal;
    const char* in_tex;
    
} typedef GraphicsEntity;

#define kGroundSize 100.f

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

Model *teapot;

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

GraphicsEntity *Gteapot;

GraphicsEntity* createGraphicsEntity(Model* model) {

    // Fet memory leak inc.
    GraphicsEntity* entity = (GraphicsEntity*) malloc(sizeof(GraphicsEntity));

    entity->in_pos = "in_Position";
    entity->in_normal = "in_Normal";
    entity->in_tex = "inTexCoord";

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
    scaling.m[15] = 1;

    while (!(cur_entity == nullptr)){

        new_scaling = scaling * cur_entity->translation * cur_entity->rotation * cur_entity->scaling;
        if (cur_entity->model != nullptr){
            glUniformMatrix4fv(glGetUniformLocation(shader, "scaleMatrix"), 1, GL_TRUE, new_scaling.m);
            /*glBindVertexArray(cur_entity->model->vao);    // Select VAO*/
            DrawModel(cur_entity->model, shader, cur_entity->in_pos, cur_entity->in_normal, cur_entity->in_tex);
            glDrawElements(GL_TRIANGLES, cur_entity->model->numIndices, GL_UNSIGNED_INT, 0L); // draw element on VAO
            printError("error drawing");
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

mat4 r = mat4(1);
vec3 p;
vec3 l;
vec3 v;
mat4 lookAtm;
mat4 translated_lookAtm;
mat4 camT;

mat4 xRotation(float theta){

    mat4 rotation = mat4(1, 0, 0, 0,
                         0, cos(theta), sin(theta), 0,
                         0, -sin(theta), cos(theta), 0,
                         0, 0, 0, 1);
    return rotation;
}

void mouseCallback(int x, int y);

void createTeapot(GLuint shaderprog){
	teapot = LoadModel("models/various/teapot.obj");
    Gteapot = createGraphicsEntity(teapot);
    Gteapot->translation = T(20,0,20);
    Gteapot->in_tex = nullptr;
}
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

    windmill->in_tex = NULL;
    Groof->in_tex = NULL;
    Gbalcony->in_tex = NULL;
    Gwing1->in_tex = NULL;
    Gwing2->in_tex = NULL;
    Gwing3->in_tex = NULL;
    Gwing4->in_tex = NULL;

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
    Gground = createGraphicsEntity(ground);
    Gground->in_tex = nullptr;

	printError("ground err");
}

void createSkyBox(GLuint shaderprog){

    skybox = LoadModel("skybox/skyboxfull.obj");
    Gskybox = createGraphicsEntity(skybox);
    Gskybox->translation = T(0,0,10);
    Gskybox->in_normal = NULL;

	printError("skybox err");
}

void activateTexture(const char* texname, unsigned int unit, GLuint tex, GLuint shader){

    GLuint glTextureUnit = GL_TEXTURE0 + unit;
    if (glTextureUnit > 0x84DF){
        printf("to many texture units\n");
        exit(1);
    }

    glActiveTexture(glTextureUnit);
	printError("activate tex err");
    glBindTexture(GL_TEXTURE_2D, tex);
	printError("bind 2 tex err");
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/

    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
}

void clearTexture(unsigned int unit){
    GLuint glTextureUnit = GL_TEXTURE0 + unit;

    glActiveTexture(glTextureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
	printError("clear tex err");
}

// -- Rotation matrix
int prevx = -1;
int prevy = -1;
float anglex = 0;
float angley = 0;
// -- Rotation matrix

void init(void)
{
    printf("start program\n");
	// Reference to shader program
	dumpInfo();
	skyboxshader = loadShaders("skyboxshader.vert", "skyboxshader.frag");
	program = loadShaders("simple-vert.glsl", "simple-frag.glsl");
	printError("init shader");

    LoadTGATextureSimple("skybox/SkyBoxFull.tga", &myTex);
    /*LoadTGATextureSimple("maskros512.tga", &myTex2);*/

	// GL inits
	glClearColor(0.2,0.2,0.4,0);
	printError("GL inits");

    p = vec3(0, 0, 10.f);
    l = vec3(0.f, 0.f, 0.f);
    v = vec3(0, 1.f, 0.f);
    /*translated_lookAtm = lookAtv(p, l, v);*/
    camT = T(-50,-15,5);
    anglex = 1.7;
    angley = -0.16;
    r = Ry(anglex) * Rx(angley);
    lookAtm = inverse(r) * camT * lookAtv(p, l, v);

    createGround(program);
    createWindmill(program);
    createTeapot(program);
    createSkyBox(skyboxshader);
    
    glutMotionFunc(mouseCallback);
	printError("init arrays");
}


void keyPressed(unsigned char key, int x, int y);

void renderEntities(){


    glUseProgram(skyboxshader); // Added 2022-03
    glUniformMatrix4fv(glGetUniformLocation(skyboxshader, "lookAt"), 1, GL_TRUE, (inverse(r) * lookAtv(p, l, v)).m);
    glUniformMatrix4fv(glGetUniformLocation(skyboxshader, "projection"), 1, GL_TRUE, projectionMatrix);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("skybox matrix error");
    activateTexture("labskybox512.tga", 0, myTex, skyboxshader);
    renderEntity(Gskybox, skyboxshader);
    clearTexture(0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	printError("skybox error");

    glUseProgram(program); // Added 2022-03
    vec3 camPos = vec3(camT.m[3], camT.m[7], camT.m[11]);
    printVec3(camPos);
    glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &camPos.x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    glUniformMatrix4fv(glGetUniformLocation(program, "lookAt"), 1, GL_TRUE, (lookAtm).m);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix);
	printError("upload matrix error");
    wingRotation->rotation = xRotation(t/1000);

    vec3 ambientColor = vec3(0,0,0);
    glUniform3fv(glGetUniformLocation(program, "ambientColor"), 1, &ambientColor.x);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[2]);
    renderEntity(Gground, program);

    // ambientColor = vec3(0.6,0.6,0.6);
    ambientColor = vec3(1,1,1);
    glUniform3fv(glGetUniformLocation(program, "ambientColor"), 1, &ambientColor.x);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[2]);
    renderEntity(windmill, program);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[2]);
    renderEntity(Gteapot, program);

	printError("maskros error");
	printError("Render::error");

}

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    /*printMat4(camT);*/
    /*printMat4(r);*/
    renderEntities();

    glutKeyboardFunc(keyPressed);

	printError("display");
	glutSwapBuffers();
}

void translate(float x, float y, float z, mat4* matrix){
    matrix->m[3] = matrix->m[3] + x;
    matrix->m[7] = matrix->m[7] + y; 
    matrix->m[11] = matrix->m[11] + z; 
    /*printf("x: %f, y: %f, z: %f\n", matrix->m[3], matrix->m[7], matrix->m[11]);*/
}


void move(unsigned char key, mat4* matrix, const char* keyset){
    float speed = 0.4;

    if (key == keyset[0]){
        translate(speed, 0.0, 0.0, matrix);
    }
    if (key == keyset[1]){
       translate(0.0, speed ,0.0, matrix);
    }
    if (key == keyset[2]){
        translate(0.0, 0, speed, matrix);
    }
    if (key == keyset[3]){
        translate(-speed, 0, 0, matrix);
    }
    if (key == keyset[4]){
        translate(0.0, -speed, 0.0, matrix);
    }
    if (key == keyset[5]){
        translate(0.0, 0.0, -speed, matrix);
    }
}


unsigned char prev_key;
float skyx = 0;
float skyy = 0;
float skyz = 0;

void keyPressed(unsigned char key, int xx, int yy) {

    const char* keyset = "aYwdys";
    if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6'){
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
            {
            mat4 pos = mat4(0);
            move(key, &pos, keyset);
            pos = r * pos;
            camT.m[3] += pos.m[3];
            camT.m[7] += pos.m[7];
            camT.m[11] += pos.m[11];
            lookAtm = inverse(r) *camT *  lookAt(p, l, v);
            }
            break;
        case '6':
            {
            mat4 posi = mat4(0);
            move(key, &posi, keyset);
            skyx += posi.m[3];
            skyz += posi.m[7];
            skyz += posi.m[11];
            Gskybox->rotation = Rx(skyx) * Ry(skyy) * Rz(skyz);
            }
        break;
    }
}


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

    printf("d: %d %d x: %f y: %f \n", dx, dy, anglex, angley);

    anglex += ((float)dx) / 100.f;
    angley += ((float)dy) / 100.f;

    mat3 rx = mat3(1);
    mat3 ry = mat3(1);

    rx = Rx(angley);
    ry = Ry(anglex);
    r = ry * rx;
    /*printVec3(l);*/
    prevx = x;
    prevy = y;
    lookAtm = inverse(r) * camT * lookAt(p, l, v);
    printMat4(camT);
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
