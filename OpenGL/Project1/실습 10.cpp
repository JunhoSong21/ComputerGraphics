#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <cmath>
#include <gl/glew.h>
#include <gl/freeglut.h>

std::random_device rd;
std::default_random_engine eng(rd());

GLfloat spiralAngle = 0.0f;   // 현재 스파이럴 각도
int spiralCount = 0;           // 현재 그린 스파이럴 수
bool drawPoints = false;       // 점으로 그릴지 여부
std::vector<std::pair<float, float>> spiralPoints; // 스파이럴 점 위치 저장

void InitSpiralPoints(int count);
void DrawSpiral(float centerX, float centerY);
void Display();
void Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void ChangeBackgroundColor();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Spiral Animation");

    glewExperimental = GL_TRUE;
    glewInit();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard);
    glutIdleFunc(Display); // IDLE 시 화면을 계속 업데이트

    glutMainLoop();
    return 0;
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& point : spiralPoints) {
        DrawSpiral(point.first, point.second);
    }

    glutSwapBuffers();
}

void Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float normalizedX = (float)(x) / 400.0f - 1.0f; // 좌표 정규화 (-1.0f ~ 1.0f)
        float normalizedY = 1.0f - (float)(y) / 300.0f; // 좌표 정규화 (1.0f ~ -1.0f)

        // 스파이럴 포인트 초기화
        spiralCount = 0;
        InitSpiralPoints(1); // 클릭한 위치에 스파이럴을 그리기 위해 포인트 추가
        ChangeBackgroundColor(); // 배경색 변경
        glutPostRedisplay(); // 화면 업데이트
    }
}

void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p':
        drawPoints = true; // 점으로 그리기
        break;
    case 'l':
        drawPoints = false; // 선으로 그리기
        break;
    case '1': case '2': case '3': case '4': case '5':
        spiralCount = key - '0'; // 입력된 숫자만큼의 스파이럴 수 설정
        InitSpiralPoints(spiralCount);
        ChangeBackgroundColor(); // 배경색 변경
        glutPostRedisplay(); // 화면 업데이트
        break;
    default:
        break;
    }
}

void ChangeBackgroundColor() {
    glClearColor((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0f);
}

void InitSpiralPoints(int count) {
    spiralPoints.clear(); // 기존 포인트 초기화
    for (int i = 0; i < count; ++i) {
        float centerX = (float)(rand() % 200 - 100) / 100.0f; // 랜덤 X좌표
        float centerY = (float)(rand() % 200 - 100) / 100.0f; // 랜덤 Y좌표
        spiralPoints.push_back({ centerX, centerY }); // 포인트 추가
    }
}

void DrawSpiral(float centerX, float centerY) {
    glBegin(drawPoints ? GL_POINTS : GL_LINE_STRIP); // 점 또는 선으로 그리기
    float radius = 0.0f;

    // 스파이럴 그리기 (2~3 바퀴)
    for (int i = 0; i < 720; ++i) { // 2 바퀴
        spiralAngle = (i * 0.1f); // 각도 증가
        radius += 0.0025f; // 반지름 증가 (부드러운 변화를 위해 작게 설정)
        float x = centerX + radius * cos(spiralAngle);
        float y = centerY + radius * sin(spiralAngle);
        glVertex2f(x, y);
    }

    // 안에서 밖으로 그리기
    radius = 0.0f; // 반지름 초기화
    for (int i = 720; i >= 0; --i) { // 2 바퀴
        spiralAngle = (i * 0.1f); // 각도 증가
        radius -= 0.0025f; // 반지름 감소 (부드러운 변화를 위해 작게 설정)
        float x = centerX + radius * cos(spiralAngle);
        float y = centerY + radius * sin(spiralAngle);
        glVertex2f(x, y);
    }

    glEnd(); // 그리기 끝
}
