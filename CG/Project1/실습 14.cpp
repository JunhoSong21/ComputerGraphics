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
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid Timer(int value);

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

void drawXYline();
void drawCube();
void drawTriPyramid();

bool IsDepthTest = true;
bool IsStyleSolid = true;

bool IsCube = true;

int XRotating = 0;
int YRotating = 0;

float Xmove = 0.f;
float Ymove = 0.f;
float XRotate = 30.f;
float YRotate = -30.f;

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);

    glutCreateWindow("Example 14");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    glEnable(GL_DEPTH_TEST);

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeyboard);
    glutTimerFunc(16, Timer, 1);

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glDisable(GL_DEPTH_TEST);
    drawXYline();
    
    if (IsDepthTest)
        glDisable(GL_DEPTH_TEST);
    else if (!IsDepthTest)
        glEnable(GL_DEPTH_TEST);
    if (IsStyleSolid)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if(!IsStyleSolid)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (IsCube)
        drawCube();
    else if (!IsCube)
        drawTriPyramid();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'c':
        IsCube = true;
        break;
    case 'p':
        IsCube = false;
        break;
    case 'h':
        IsDepthTest = !IsDepthTest;
        break;
    case 'w':
        IsStyleSolid = false;
        break;
    case 'W':
        IsStyleSolid = true;
        break;
    case 'x':
        if (XRotating == 1)
            XRotating = 0;
        else
            XRotating = 1;
        break;
    case 'X':
        if (XRotating == -1)
            XRotating = 0;
        else
            XRotating = -1;
        break;
    case 'y':
        if (YRotating == 1)
            YRotating = 0;
        else
            YRotating = 1;
        break;
    case 'Y':
        if (YRotating == -1)
            YRotating = 0;
        else
            YRotating = -1;
        break;
    case 's':
        XRotating = 0;
        YRotating = 0;
        XRotate = 30.f;
        YRotate = -30.f;
        Xmove = 0.f;
        Ymove = 0.f;
        break;
    }

    glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        Xmove -= 0.05f;
        break;
    case GLUT_KEY_RIGHT:
        Xmove += 0.05f;
        break;
    case GLUT_KEY_UP:
        Ymove += 0.05f;
        break;
    case GLUT_KEY_DOWN:
        Ymove -= 0.05f;
        break;
    }

    glutPostRedisplay();
}

GLvoid Timer(int value) {
    if (XRotating == 1) {
        XRotate += 0.5f;
        if (XRotate >= 180.f)
            XRotate = -180.f;
    }
    else if (XRotating == -1) {
        XRotate -= 0.5f;
        if (XRotate <= -180.f)
            XRotate = 180.f;
    }
    if (YRotating == 1) {
        YRotate += 0.5f;
        if (YRotate >= 180.f)
            YRotate = -180.f;
    }
    else if (YRotating == -1) {
        YRotate -= 0.5f;
        if (YRotate <= -180.f)
            YRotate = 180.f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 1);
}

void drawXYline() {
    glm::mat4 None = glm::mat4(1.f);
    None = glm::rotate(None, glm::radians(0.f), glm::vec3(1.0, 1.0, 1.0));

    std::vector<GLfloat> vertices = {
        0.f, 1.f, 1.f,
        0.f, -1.f, 1.f,

        1.f, 0.f, 1.f,
        -1.f, 0.f, 1.f
    };

    std::vector<GLfloat> colors = {
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,

        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(None));

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 0, 4);

    glBindVertexArray(0);
}

void drawCube() {
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);
    
    Translate = glm::translate(Translate, glm::vec3(Xmove, Ymove, 0.f));
    RotateX = glm::rotate(RotateX, glm::radians(XRotate), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(YRotate), glm::vec3(0.0, 1.0, 0.0));

    Conversion = Translate * RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        -0.3f, -0.3f, 0.3f,
        -0.3f, -0.3f, -0.3f,
        0.3f, -0.3f, -0.3f,
        0.3f, -0.3f, 0.3f,

        -0.3f, 0.3f, 0.3f,
        -0.3f, 0.3f, -0.3f,
        0.3f, 0.3f, -0.3f,
        0.3f, 0.3f, 0.3f
    };

    std::vector<GLfloat> colors = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, 1.f, 0.f,

        1.f, 0.f, 1.f,
        0.f, 1.f, 1.f,
        1.f, 0.5f, 0.5f,
        0.f, 0.5f, 0.5f
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
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteBuffers(1, &ibo);
}

void drawTriPyramid() {
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    Translate = glm::translate(Translate, glm::vec3(Xmove, Ymove, 0.f));
    RotateX = glm::rotate(RotateX, glm::radians(XRotate), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(YRotate), glm::vec3(0.0, 1.0, 0.0));

    Conversion = Translate * RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        -0.3f, -0.3f, 0.3f,
        -0.3f, -0.3f, -0.3f,
        0.3f, -0.3f, -0.3f,
        0.3f, -0.3f, 0.3f,
        0.f, 0.3f, 0.f
    };

    std::vector<GLfloat> colors = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, 1.f, 0.f,
        1.f, 0.f, 1.f
    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0,
        0, 4, 1,
        1, 4, 2,
        2, 4, 3,
        3, 4, 0
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteBuffers(1, &ibo);
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