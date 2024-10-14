#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <cmath>
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

GLfloat triangles[4][9];  // 4개의 삼각형을 저장
GLfloat colors[4][9];      // 각 삼각형의 색상
int currentMovement = -1;  // 현재 이동 모드 초기값 (이동 모드 설정 전)
float speeds[4][2];        // 속도 (x, y)
float directions[4][2];    // 방향 (x, y)
bool trianglesInitialized = false; // 삼각형이 초기화되었는지 여부

GLenum Drawmode = GL_FILL;

// 함수 선언
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
void DrawTriangles();
void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void MoveTriangles();
void ResetTriangles();

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Moving Triangles");

    glewExperimental = GL_TRUE;
    glewInit();

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse); // 마우스 클릭 이벤트 추가
    glutIdleFunc(MoveTriangles); // IDLE 시 삼각형 이동

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);
    if (trianglesInitialized) { // 삼각형이 초기화된 경우에만 그립니다.
        DrawTriangles();
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        currentMovement = 0;  // 튕기기
        break;
    case '2':
        currentMovement = 1;  // 좌우로 지그재그
        break;
    case '3':
        currentMovement = 2;  // 사각 스파이럴
        break;
    case '4':
        currentMovement = 3;  // 원 스파이럴
        break;
    default:
        break;
    }
}

GLvoid Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        ResetTriangles(); // 좌클릭 시 삼각형 초기화
        trianglesInitialized = true; // 삼각형 초기화 플래그 설정
        glutPostRedisplay(); // 화면 업데이트
    }
}

void DrawTriangles() {
    glBindVertexArray(vao);
    for (int i = 0; i < 4; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triangles[i], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors[i], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glBindVertexArray(0);
}

void ResetTriangles() {
    for (int i = 0; i < 4; ++i) {
        float mx = RandomWidthHeight();
        float a = 0.1f; // 삼각형의 높이
        triangles[i][0] = mx;                  // 꼭짓점 1
        triangles[i][1] = a;                    // 꼭짓점 1
        triangles[i][2] = 0.0f;                  // 깊이
        triangles[i][3] = mx - 0.05f;           // 꼭짓점 2
        triangles[i][4] = -a;                   // 꼭짓점 2
        triangles[i][5] = 0.0f;                  // 깊이
        triangles[i][6] = mx + 0.05f;           // 꼭짓점 3
        triangles[i][7] = -a;                   // 꼭짓점 3
        triangles[i][8] = 0.0f;                  // 깊이

        for (int j = 0; j < 3; j++) {
            colors[i][3 * j] = RandomRGB();
            colors[i][3 * j + 1] = RandomRGB();
            colors[i][3 * j + 2] = RandomRGB();
        }

        // 속도 및 방향 초기화
        speeds[i][0] = 0.01f * (i + 1); // 각 삼각형마다 다른 속도
        speeds[i][1] = 0.01f * (i + 1);
        directions[i][0] = 1.0f; // 초기 방향
        directions[i][1] = 1.0f;
    }
}

void MoveTriangles() {
    if (currentMovement == -1) return; // 이동 모드가 설정되지 않은 경우

    for (int i = 0; i < 4; ++i) {
        switch (currentMovement) {
        case 0: // 튕기기
            triangles[i][0] += speeds[i][0] * directions[i][0];
            triangles[i][1] += speeds[i][1] * directions[i][1];

            // 벽에 부딪히면 방향 바꾸기
            if (triangles[i][0] >= 1.0f || triangles[i][0] <= -1.0f) {
                directions[i][0] *= -1; // x 방향 반전
            }
            if (triangles[i][1] >= 1.0f || triangles[i][1] <= -1.0f) {
                directions[i][1] *= -1; // y 방향 반전
            }
            break;

        case 1: // 좌우로 지그재그
            triangles[i][0] += speeds[i][0] * directions[i][0];
            if (triangles[i][0] >= 1.0f || triangles[i][0] <= -1.0f) {
                directions[i][0] *= -1; // x 방향 반전
            }
            triangles[i][1] += (i % 2 == 0 ? 0.01f : -0.01f); // 짝수는 위로, 홀수는 아래로
            if (triangles[i][1] >= 1.0f) {
                triangles[i][1] = 1.0f; // 경계 유지
            }
            else if (triangles[i][1] <= -1.0f) {
                triangles[i][1] = -1.0f; // 경계 유지
            }
            break;

        case 2: // 사각 스파이럴
            triangles[i][0] += speeds[i][0] * cos(i + glutGet(GLUT_ELAPSED_TIME) * 0.001);
            triangles[i][1] += speeds[i][1] * sin(i + glutGet(GLUT_ELAPSED_TIME) * 0.001);
            break;

        case 3: // 원 스파이럴
            triangles[i][0] = 0.5f * cos(glutGet(GLUT_ELAPSED_TIME) * 0.001 + i) * (1.0f - 0.005f * (glutGet(GLUT_ELAPSED_TIME) * 0.001));
            triangles[i][1] = 0.5f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.001 + i) * (1.0f - 0.005f * (glutGet(GLUT_ELAPSED_TIME) * 0.001));
            break;
        }
    }
    glutPostRedisplay(); // 화면 업데이트
}

void InitBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(2, vbo);
}

void make_shaderProgram() {
    make_vertexShaders();
    make_fragmentShaders();
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);
}

void make_vertexShaders() {
    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
}

void make_fragmentShaders() {
    fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
}

char* filetobuf(const char* file) {
    FILE* fp;
    char* buf;
    long length;
    fopen_s(&fp, file, "rb");
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buf = (char*)malloc(length + 1);
    fread(buf, 1, length, fp);
    buf[length] = '\0';
    fclose(fp);
    return buf;
}
