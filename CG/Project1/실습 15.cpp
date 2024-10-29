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

struct StandardPoint {
    float x;
    float z;
};
StandardPoint point;

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);

void drawXYZline();
void drawCube();
void drawCylinder();

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

float angle = 0.f;
GLUquadricObj* qobj;
int RevolveY = 0;

int IsSelfXRotate = 0;
int IsSelfYRotate = 0;
bool IsLeftRotate = false;
bool IsRightRotate = false;

float RightRotateX = 0.f;
float RightRotateY = 0.f;
float LeftRotateX = 0.f;
float LeftRotateY = 0.f;

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

    point.x = 0.5f * cos(angle);
    point.z = 0.5f * sin(angle);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(16, Timer, 1);

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glDisable(GL_DEPTH_TEST);
    drawXYZline();

    drawCube();
    drawCylinder();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        IsRightRotate = false;
        IsLeftRotate = true;
        break;
    case '2':
        IsRightRotate = true;
        IsLeftRotate = false;
        break;
    case '3':
        IsRightRotate = true;
        IsLeftRotate = true;
        break;
    case 'x':
        if (IsSelfXRotate != 1) {
            IsSelfXRotate = 1;
            IsRightRotate = true;
            IsLeftRotate = true;
        }
        else
            IsSelfXRotate = 0;
        break;
    case 'X':
        if (IsSelfXRotate != -1) {
            IsSelfXRotate = -1;
            IsRightRotate = true;
            IsLeftRotate = true;
        }
        else
            IsSelfXRotate = 0;
        break;
    case 'y':
        if (IsSelfYRotate != 1) {
            IsSelfYRotate = 1;
            IsRightRotate = true;
            IsLeftRotate = true;
        }
        else
            IsSelfYRotate = 0;
        break;
    case 'Y':
        if (IsSelfYRotate != -1) {
            IsSelfYRotate = -1;
            IsRightRotate = true;
            IsLeftRotate = true;
        }
        else
            IsSelfYRotate = 0;
        break;
    case 'r':
        if (RevolveY != 1)
            RevolveY = 1;
        else
            RevolveY = 0;
        break;
    case 'R':
        if(RevolveY != -1)
            RevolveY = -1;
        else
            RevolveY = 0;
        break;
    case 'c':
        break;
    case 's':
        break;
    }

    glutPostRedisplay();
}

GLvoid Timer(int value) {
    if (IsSelfXRotate == 1) {
        if (IsRightRotate)
            RightRotateX += 0.2f;
        if (IsLeftRotate)
            LeftRotateX += 0.2f;
    }

    if (IsSelfXRotate == -1) {
        if (IsRightRotate)
            RightRotateX -= 0.2f;
        if (IsLeftRotate)
            LeftRotateY -= 0.2f;
    }

    if (IsSelfYRotate == 1) {
        if (IsRightRotate)
            RightRotateY += 0.2f;
        if (IsLeftRotate)
            LeftRotateY += 0.2f;
    }

    if (IsSelfYRotate == -1) {
        if (IsRightRotate)
            RightRotateY -= 0.2f;
        if (IsLeftRotate)
            LeftRotateY -= 0.2f;
    }

    if (RevolveY == 1) {
        angle += 0.1f;
        point.x = 0.5f * cos(angle);
        point.z = 0.5f * sin(angle);
    }
    else if (RevolveY == -1) {
        angle -= 0.1f;
        point.x = 0.5f * cos(angle);
        point.z = 0.5f * sin(angle);
    }

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 1);
}

void drawXYZline() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(-30.f), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY;

    std::vector<GLfloat> vertices = {
        -1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,

        0.f, -1.f, 0.f,
        0.f, 1.f, 0.f,

        0.f, 0.f, -1.f,
        0.f, 0.f, 1.f
    };

    std::vector<GLfloat> colors = {
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
    };

    std::vector<GLuint> index = {
        0, 1,
        2, 3,
        4, 5
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

    glDrawElements(GL_LINES, index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteBuffers(1, &ibo);
}

void drawCube() {
    glm::mat4 SelfRotationX = glm::mat4(1.f);
    glm::mat4 SelfRotationY = glm::mat4(1.f);
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Revolve = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    SelfRotationX = glm::rotate(SelfRotationX, glm::radians(RightRotateX), glm::vec3(1.0, 0.0, 0.0));
    SelfRotationY = glm::rotate(SelfRotationY, glm::radians(RightRotateY), glm::vec3(0.0, 1.0, 0.0));
    Translate = glm::translate(Translate, glm::vec3(point.x, 0.f, point.z));
    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(-30.f), glm::vec3(0.0, 1.0, 0.0));
    Revolve = glm::rotate(Revolve, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY * Translate *  SelfRotationX * SelfRotationY * Revolve;

    std::vector<GLfloat> vertices = {
        -0.2f, -0.2f, 0.2f,
        -0.2f, -0.2f, -0.2f,
        0.2f, -0.2f, -0.2f,
        0.2f, -0.2f, 0.2f,

        -0.2f, 0.2f, 0.2f,
        -0.2f, 0.2f, -0.2f,
        0.2f, 0.2f, -0.2f,
        0.2f, 0.2f, 0.2f
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

void drawCylinder() {
    glm::mat4 SelfRotationX = glm::mat4(1.f);
    glm::mat4 SelfRotationY = glm::mat4(1.f);
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    SelfRotationX = glm::rotate(SelfRotationX, glm::radians(LeftRotateX), glm::vec3(1.0, 0.0, 0.0));
    SelfRotationY = glm::rotate(SelfRotationY, glm::radians(LeftRotateY), glm::vec3(0.0, 1.0, 0.0));
    Translate = glm::translate(Translate, glm::vec3(-point.x, 0.1f, -point.z));
    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(-30.f), glm::vec3(0.0, 1.0, 0.0));
    
    Conversion = Translate * RotateX * RotateY * SelfRotationX * SelfRotationY;

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluCylinder(qobj, 0.3, 0.0, 0.6, 20, 8);
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