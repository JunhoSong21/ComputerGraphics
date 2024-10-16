#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct Shape {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    int vectexCount; // 꼭짓점 개수

    bool IsMove;
    int Direction;
};

std::random_device rd;
std::default_random_engine eng(rd());

float RandomFloat(float f1, float f2) {
    std::uniform_real_distribution<GLfloat> distr(f1, f2);
    return distr(eng);
}

GLuint vao, vertexVBO, colorVBO;
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

void makeShapes();
void drawShapes();
void makeDot();
void makeLine();
void makeTriangle();
void makeRectangle();
void makePentagon();

std::vector<std::vector<Shape>> shapes;

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Example 12");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    InitBuffer();
    make_shaderProgram();

    makeShapes();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    glutMainLoop();
}

GLvoid drawScene() {
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
    glutPostRedisplay();
}

GLvoid Motion(int x, int y) {
    glutPostRedisplay();
}

void makeShapes() {
    for (int i = 0; i < 3; ++i) {
        makeDot();
        makeLine();
        makeTriangle();
        makeRectangle();
        makePentagon();
    }
}

void drawShapes() {

}

void makeDot() {
    Shape newDot;

    GLfloat baseX = RandomFloat(-0.9f, 0.9f);
    GLfloat baseY = RandomFloat(-0.9f, 0.9f);

    newDot.vertices = {
        baseX + 0.01f, baseY + 0.01f, 1.f,
        baseX - 0.01f, baseY + 0.01f, 1.f,
        baseX - 0.01f, baseY - 0.01f, 1.f,
        baseX + 0.01f, baseY - 0.01f, 1.f
    };

    newDot.colors = {
        RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f),
        RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f),
        RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f),
        RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f)
    };

    newDot.vectexCount = 1;
    newDot.Direction = 1; // 의미 없는 초기화
    newDot.IsMove = false;

    shapes[0].push_back(newDot);
}

void makeLine() {
    Shape newLine;

}

void makeTriangle() {

}

void makeRectangle() {

}

void makePentagon() {

}

void InitBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &colorVBO);
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