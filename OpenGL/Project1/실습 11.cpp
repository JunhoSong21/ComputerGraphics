#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <cmath>
#include <gl/glew.h>
#include <gl/freeglut.h>

enum Shape { LINE, TRIANGLE, SQUARE, PENTAGON }; // 도형 타입 정의

// 사분면에 그릴 도형을 위한 변수
Shape shapes[4] = { LINE, TRIANGLE, SQUARE, PENTAGON };
Shape targetShapes[4] = { LINE, TRIANGLE, SQUARE, PENTAGON }; // 목표 도형

void drawShape(Shape shape, float x, float y) {
    // 각 도형 그리기
}

void animateShapes() {
    for (int i = 0; i < 4; ++i) {
        // 목표 도형으로 변경
        if (shapes[i] != targetShapes[i]) {
            shapes[i] = targetShapes[i];
        }
    }
    glutPostRedisplay(); // 화면 업데이트
}

GLvoid timer(int value) {
    if (isAnimating) {
        // 각 도형의 목표 도형 설정
        for (int i = 0; i < 4; ++i) {
            targetShapes[i] = static_cast<Shape>((shapes[i] + 1) % 4); // 다음 도형으로 변환
        }
        animateShapes(); // 애니메이션 적용
    }
    glutTimerFunc(1000, timer, 0); // 1초마다 호출
}

GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'l':
        targetShapes[0] = TRIANGLE; // 선 -> 삼각형
        break;
    case 't':
        targetShapes[1] = SQUARE; // 삼각형 -> 사각형
        break;
    case 'r':
        targetShapes[2] = PENTAGON; // 사각형 -> 오각형
        break;
    case 'p':
        targetShapes[3] = LINE; // 오각형 -> 선
        break;
    case 'a':
        // 모든 도형 초기화
        shapes[0] = LINE;
        shapes[1] = TRIANGLE;
        shapes[2] = SQUARE;
        shapes[3] = PENTAGON;
        break;
    case 's':
        isAnimating = !isAnimating; // 애니메이션 시작/정지
        break;
    }
}}

GLvoid reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // 좌표계 설정
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
    glutTimerFunc(1000, timer, 0); // 타이머 함수 호출

    glClearColor(1.0f, 1.0f, 1.0f, 1.f); // 배경색 흰색
    glColor3f(0.0f, 0.0f, 0.0f); // 도형 색상 검정색

    glutMainLoop();
}
