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

float RandomRGB() {
    std::uniform_real_distribution<float> distr(0.1f, 1.0f);
    return distr(eng);
}
float RandomWidthHeight() {
    std::uniform_real_distribution<float> distr(0.1f, 0.2f);
    return distr(eng);
}

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLenum Drawmode = GL_FILL;
int MaxTris = 3;

// 사분면 별로 저장할 삼각형의 좌표와 색상
std::vector<std::vector<GLfloat>> triangles[4]; // 4개의 사분면을 위한 벡터 배열
std::vector<std::vector<GLfloat>> colors[4];

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);

void DrawQuadrantLines();
void DrawTriangles();
void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

GLvoid main(int argc, char** argv) {
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

    DrawQuadrantLines();
    DrawTriangles(); // 삼각형 그리기

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a': // 면으로 그리기
        Drawmode = GL_FILL;
        break;
    case 'b': // 선으로 그리기
        Drawmode = GL_LINE;
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
    float mx = (float)x / 400.0f - 1.0f;
    float my = 1.0f - (float)y / 300.0f;

    int quadrant = 0;
    if (x >= 400 && y < 300) { // 1사분면
        quadrant = 0;
    }
    else if (x < 400 && y < 300) { // 2사분면
        quadrant = 1;
    }
    else if (x < 400 && y >= 300) { // 3사분면
        quadrant = 2;
    }
    else if (x >= 400 && y >= 300) { // 4사분면
        quadrant = 3;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // 왼쪽 클릭
        if (!triangles[quadrant].empty()) { // 기존 삼각형 삭제
            triangles[quadrant].clear();
            colors[quadrant].clear();
        }
        float a = RandomWidthHeight();
        std::vector<GLfloat> newTriangle = {
            mx, my + a, 1.f,
            mx - a, my - a, 1.f,
            mx + a, my - a, 1.f
        };
        triangles[quadrant].push_back(newTriangle);

        // 동일한 색상 할당 (랜덤으로 하나의 색상을 생성한 후, 세 꼭짓점에 동일하게 적용)
        GLfloat r = RandomRGB(), g = RandomRGB(), b = RandomRGB();
        std::vector<GLfloat> newColor = {
            r, g, b,
            r, g, b,
            r, g, b
        };
        colors[quadrant].push_back(newColor);
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { // 오른쪽 클릭
        if (triangles[quadrant].size() < MaxTris) { // 최대 3개의 삼각형
            float a = RandomWidthHeight();
            std::vector<GLfloat> newTriangle = {
                mx, my + a, 1.f,
                mx - a, my - a, 1.f,
                mx + a, my - a, 1.f
            };
            triangles[quadrant].push_back(newTriangle);

            // 동일한 색상 할당 (랜덤으로 하나의 색상을 생성한 후, 세 꼭짓점에 동일하게 적용)
            GLfloat r = RandomRGB(), g = RandomRGB(), b = RandomRGB();
            std::vector<GLfloat> newColor = {
                r, g, b,
                r, g, b,
                r, g, b
            };
            colors[quadrant].push_back(newColor);
        }
    }

    glutPostRedisplay();
}

void DrawQuadrantLines() {
    glBindVertexArray(vao);

    std::vector<GLfloat> lines = {
        0.0f, 1.0f, 1.0f,
        0.0f, -1.0f, 1.0f,

        -1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f
    };

    std::vector<GLfloat> lineColors = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(GLfloat), lines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, lineColors.size() * sizeof(GLfloat), lineColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);
}

void DrawTriangles() {
    glPolygonMode(GL_FRONT_AND_BACK, Drawmode);

    glBindVertexArray(vao);

    for (int q = 0; q < 4; ++q) { // 각 사분면을 순회
        for (size_t i = 0; i < triangles[q].size(); ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triangles[q][i].data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors[q][i].data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }

    glBindVertexArray(0);
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