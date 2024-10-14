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

GLfloat spiralAngle = 0.0f;   // ���� �����̷� ����
int spiralCount = 0;           // ���� �׸� �����̷� ��
bool drawPoints = false;       // ������ �׸��� ����
std::vector<std::pair<float, float>> spiralPoints; // �����̷� �� ��ġ ����

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
    glutIdleFunc(Display); // IDLE �� ȭ���� ��� ������Ʈ

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
        float normalizedX = (float)(x) / 400.0f - 1.0f; // ��ǥ ����ȭ (-1.0f ~ 1.0f)
        float normalizedY = 1.0f - (float)(y) / 300.0f; // ��ǥ ����ȭ (1.0f ~ -1.0f)

        // �����̷� ����Ʈ �ʱ�ȭ
        spiralCount = 0;
        InitSpiralPoints(1); // Ŭ���� ��ġ�� �����̷��� �׸��� ���� ����Ʈ �߰�
        ChangeBackgroundColor(); // ���� ����
        glutPostRedisplay(); // ȭ�� ������Ʈ
    }
}

void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p':
        drawPoints = true; // ������ �׸���
        break;
    case 'l':
        drawPoints = false; // ������ �׸���
        break;
    case '1': case '2': case '3': case '4': case '5':
        spiralCount = key - '0'; // �Էµ� ���ڸ�ŭ�� �����̷� �� ����
        InitSpiralPoints(spiralCount);
        ChangeBackgroundColor(); // ���� ����
        glutPostRedisplay(); // ȭ�� ������Ʈ
        break;
    default:
        break;
    }
}

void ChangeBackgroundColor() {
    glClearColor((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0f);
}

void InitSpiralPoints(int count) {
    spiralPoints.clear(); // ���� ����Ʈ �ʱ�ȭ
    for (int i = 0; i < count; ++i) {
        float centerX = (float)(rand() % 200 - 100) / 100.0f; // ���� X��ǥ
        float centerY = (float)(rand() % 200 - 100) / 100.0f; // ���� Y��ǥ
        spiralPoints.push_back({ centerX, centerY }); // ����Ʈ �߰�
    }
}

void DrawSpiral(float centerX, float centerY) {
    glBegin(drawPoints ? GL_POINTS : GL_LINE_STRIP); // �� �Ǵ� ������ �׸���
    float radius = 0.0f;

    // �����̷� �׸��� (2~3 ����)
    for (int i = 0; i < 720; ++i) { // 2 ����
        spiralAngle = (i * 0.1f); // ���� ����
        radius += 0.0025f; // ������ ���� (�ε巯�� ��ȭ�� ���� �۰� ����)
        float x = centerX + radius * cos(spiralAngle);
        float y = centerY + radius * sin(spiralAngle);
        glVertex2f(x, y);
    }

    // �ȿ��� ������ �׸���
    radius = 0.0f; // ������ �ʱ�ȭ
    for (int i = 720; i >= 0; --i) { // 2 ����
        spiralAngle = (i * 0.1f); // ���� ����
        radius -= 0.0025f; // ������ ���� (�ε巯�� ��ȭ�� ���� �۰� ����)
        float x = centerX + radius * cos(spiralAngle);
        float y = centerY + radius * sin(spiralAngle);
        glVertex2f(x, y);
    }

    glEnd(); // �׸��� ��
}
