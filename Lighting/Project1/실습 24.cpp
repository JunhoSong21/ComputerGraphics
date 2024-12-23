#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);

bool IsCube = true;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos = glm::vec3(0.0, 0.0, 3.0);

void drawCube();
void drawPyramid();

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);

    glutCreateWindow("Example 24");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(16, Timer, 1);

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glEnable(GL_DEPTH_TEST);

    if (IsCube)
        drawCube();
    else if (!IsCube)
        drawPyramid();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'z':
        lightPos.z += 0.01f;
        break;
    case 'q':
        glutLeaveMainLoop();
        break;
    }


    glutPostRedisplay();
}

GLvoid Timer(int value) {
    

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 1);
}

void drawCube() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY;

    std::vector<GLfloat> vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,



    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0,
        1, 0, 4, 4, 5, 1,
        2, 1, 5, 5, 6, 2,
        0, 3, 7, 7, 4, 0,
        3, 2, 6, 6, 7, 3,
        4, 7, 6, 6, 5, 4
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
    unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");

    glm::mat4 mTransform = glm::mat4(1.0f);
    mTransform = Conversion;
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &mTransform[0][0]);

    glm::mat4 vTransform = glm::mat4(1.0f);
    vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &vTransform[0][0]);

    glm::mat4 pTransform = glm::mat4(1.0f);
    pTransform = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &pTransform[0][0]);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

    glUseProgram(shaderProgramID);
    unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos");
    glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
    unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor");
    glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
    unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor");
    glUniform3f(objColorLocation, 0.0, 1.0, 0.0);
    unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos");
    glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

    glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteBuffers(1, &ibo);
}

void drawPyramid() {

}

void InitBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(2, vbo);
    glBindVertexArray(0);
}

void make_shaderProgram() {
    make_vertexShaders();
    make_fragmentShaders();
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgramID);
}

void make_vertexShaders() {
    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }
}

void make_fragmentShaders() {
    fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }
}

char* filetobuf(const char* file) {
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(file, "rb");
    if (!fptr) return nullptr;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}