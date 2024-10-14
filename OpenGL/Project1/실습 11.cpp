#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <cmath>
#include <gl/glew.h>
#include <gl/freeglut.h>

enum Shape { LINE, TRIANGLE, SQUARE, PENTAGON }; // ���� Ÿ�� ����

// ��и鿡 �׸� ������ ���� ����
Shape shapes[4] = { LINE, TRIANGLE, SQUARE, PENTAGON };
Shape targetShapes[4] = { LINE, TRIANGLE, SQUARE, PENTAGON }; // ��ǥ ����

void drawShape(Shape shape, float x, float y) {
    // �� ���� �׸���
}

void animateShapes() {
    for (int i = 0; i < 4; ++i) {
        // ��ǥ �������� ����
        if (shapes[i] != targetShapes[i]) {
            shapes[i] = targetShapes[i];
        }
    }
    glutPostRedisplay(); // ȭ�� ������Ʈ
}

GLvoid timer(int value) {
    if (isAnimating) {
        // �� ������ ��ǥ ���� ����
        for (int i = 0; i < 4; ++i) {
            targetShapes[i] = static_cast<Shape>((shapes[i] + 1) % 4); // ���� �������� ��ȯ
        }
        animateShapes(); // �ִϸ��̼� ����
    }
    glutTimerFunc(1000, timer, 0); // 1�ʸ��� ȣ��
}

GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'l':
        targetShapes[0] = TRIANGLE; // �� -> �ﰢ��
        break;
    case 't':
        targetShapes[1] = SQUARE; // �ﰢ�� -> �簢��
        break;
    case 'r':
        targetShapes[2] = PENTAGON; // �簢�� -> ������
        break;
    case 'p':
        targetShapes[3] = LINE; // ������ -> ��
        break;
    case 'a':
        // ��� ���� �ʱ�ȭ
        shapes[0] = LINE;
        shapes[1] = TRIANGLE;
        shapes[2] = SQUARE;
        shapes[3] = PENTAGON;
        break;
    case 's':
        isAnimating = !isAnimating; // �ִϸ��̼� ����/����
        break;
    }
}}

GLvoid reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // ��ǥ�� ����
    glMatrixMode(GL_MODELVIEW);
}

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shape Animation");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, timer, 0); // Ÿ�̸� �Լ� ȣ��

    glClearColor(1.0f, 1.0f, 1.0f, 1.f); // ���� ���
    glColor3f(0.0f, 0.0f, 0.0f); // ���� ���� ������

    glutMainLoop();
}
