#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

std::random_device rd;
std::default_random_engine eng(rd());

struct Triangle {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    GLenum mode = GL_TRIANGLES;
    bool IsDraw;
};

struct Line {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    GLenum mode = GL_LINE;
};

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLfloat FirstTrishape[3][3] = {

};

std::vector<Triangle> triangles[4][3];
std::vector<Triangle> OnlyTri[4];
GLenum Drawmode = GL_FILL;
int MaxTris = 3;
bool FirstQrtLeft = true;
bool SecondQrtLeft = true;
bool ThirdQrtLeft = true;
bool FourthQrtLeft = true;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);

void makeLine();
void DrawTriangles();

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

float RandomRGB() {
    std::uniform_real_distribution<float> distr(0.0f, 1.0f);
    return distr(eng);
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Example 8");

    glewExperimental = GL_TRUE;
    glewInit();

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);
    
    makeLine();
    DrawTriangles();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        break;
    case 'b':
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
    float mx = (float)x / 400.0f - 1.0f;
    float my = 1.0f - (float)y / 300.0f;

    if (button == GL_LEFT && state == GLUT_DOWN) {
        if (x >= 400 && y < 300) { // 1사분면
            FirstQrtLeft = true;

        }
        else if (x < 400 && y < 300) { // 2사분면
            SecondQrtLeft = true;
        }
        else if (x < 400 && y >= 300) { // 3사분면
            ThirdQrtLeft = true;
        }
        else if (x >= 400 && y >= 300) { // 4사분면
            FourthQrtLeft = true;
        }
    }
    else if (button == GL_RIGHT && state == GLUT_DOWN) {
        if (x >= 400 && y < 300) { // 1사분면
            FirstQrtLeft = false;
        }
        else if (x < 400 && y < 300) { // 2사분면
            SecondQrtLeft = false;
        }
        else if (x < 400 && y >= 300) { // 3사분면
            ThirdQrtLeft = false;
        }
        else if (x >= 400 && y >= 300) { // 4사분면
            FourthQrtLeft = false;
        }
    }

    glutPostRedisplay();
}

void makeLine() {
    glBindVertexArray(vao);

    std::vector<GLfloat> vertices = {
        -1.f, 0.f, 1.f,
         1.f, 0.f, 1.f,
         0.f, -1.f, 1.f,
         0.f, 1.f, 1.f
    };
    std::vector<GLfloat> colors = {
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glLineWidth(1.f);
    glDrawArrays(GL_LINES, 0, 2);
    glDrawArrays(GL_LINES, 2, 4);
    glBindVertexArray(0);
}

void DrawTriangles() {
    glBindVertexArray(vao);

    if (FirstQrtLeft == true) {
        
    }
}

void InitBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo[0]);
    glGenBuffers(1, &vbo[1]);
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