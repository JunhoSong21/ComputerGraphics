#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <random>

struct Rect {
    float x_min, y_min, x_max, y_max;
    float color[3];
    float orig_x_min, orig_y_min;
    float dx, dy;
    float zigzagX, zigzagY;
};

std::vector<Rect> rectangles;
const int MAX_RECTS = 5;
const double PI = 3.141592;
bool moveDiagonal = false;
bool moveZigZag = false;
bool changeSize = false;
bool changeColor = false;
bool animationsActive = false;
float zigzagLimit = 0.01f;
float sizeChange = 0.001f;

std::random_device rd;
std::default_random_engine eng(rd());

float RandomRGB() {
    std::uniform_real_distribution<float> distr(0.0f, 1.0f);
    return distr(eng);
}

GLvoid drawRect(const Rect& rect);
GLvoid display();
GLvoid mouse(int button, int state, int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid reshape(int w, int h);
GLvoid update(int value);
GLvoid colorTimer(int value);

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Example 4");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);
    glutTimerFunc(1000, colorTimer, 1);

    glutMainLoop();
}

GLvoid drawRect(const Rect& rect) {
    glColor3f(rect.color[0], rect.color[1], rect.color[2]);
    glRectf(rect.x_min, rect.y_min, rect.x_max, rect.y_max);
}

GLvoid display() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (const auto& rect : rectangles) {
        drawRect(rect);
    }
    glutSwapBuffers();
}

GLvoid reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (rectangles.size() < MAX_RECTS) {
            Rect newRect;
            newRect.orig_x_min = (2.0f * x / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
            newRect.orig_y_min = 1.0f - (2.0f * y / glutGet(GLUT_WINDOW_HEIGHT));
            newRect.x_min = newRect.orig_x_min - 0.1f;
            newRect.y_min = newRect.orig_y_min - 0.1f;
            newRect.x_max = newRect.orig_x_min + 0.1f;
            newRect.y_max = newRect.orig_y_min + 0.1f;

            for (auto i = 0; i < 3; ++i)
                newRect.color[i] = RandomRGB();

            newRect.dx = 0.01f;
            newRect.dy = 0.01f;
            newRect.zigzagX = 0.01f;
            newRect.zigzagY = 0.01f;
            rectangles.push_back(newRect);
        }
    }
}

GLvoid keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 's':
        animationsActive = false;
        moveDiagonal = false;
        moveZigZag = false;
        changeSize = false;
        changeColor = false;
        break;
    case 'm':
        animationsActive = false;
        moveDiagonal = false;
        moveZigZag = false;
        changeSize = false;
        changeColor = false;
        for (auto& rect : rectangles) {
            rect.x_min = rect.orig_x_min - 0.05f;
            rect.y_min = rect.orig_y_min - 0.05f;
            rect.x_max = rect.orig_x_min + 0.05f;
            rect.y_max = rect.orig_y_min + 0.05f;
        }
        break;
    case 'r':
        animationsActive = false;
        moveDiagonal = false;
        moveZigZag = false;
        changeSize = false;
        changeColor = false;
        rectangles.clear();
        break;
    case 'q':
        glutLeaveMainLoop();
        break;
    case '1':
        if (rectangles.size() > 0) {
            animationsActive = true;
            moveDiagonal = !moveDiagonal;
            moveZigZag = false;
        }
        break;
    case '2':
        if (rectangles.size() > 0) {
            for (auto& rect : rectangles) {
                rect.zigzagY = (rect.y_max + rect.y_min) / 2;
            }
            animationsActive = true;
            moveZigZag = !moveZigZag;
            moveDiagonal = false;
        }
        break;
    case '3':
        if (rectangles.size() > 0) {
            animationsActive = true;
            changeSize = !changeSize;
        }
        break;
    case '4':
        if (rectangles.size() > 0) {
            animationsActive = true;
            changeColor = !changeColor;
        }
        break;
    }

    glutPostRedisplay();
}

GLvoid update(int value) {
    if (animationsActive) {
        for (auto& rect : rectangles) {
            if (moveDiagonal) {
                rect.x_min += rect.dx;
                rect.x_max += rect.dx;
                rect.y_min += rect.dy;
                rect.y_max += rect.dy;

                if (rect.x_min < -1.0f || rect.x_max > 1.0f) rect.dx *= -1;
                if (rect.y_min < -1.0f || rect.y_max > 1.0f) rect.dy *= -1;
            }

            if (moveZigZag) {
                rect.x_min += rect.zigzagX;
                rect.x_max += rect.zigzagX;
                rect.y_min += zigzagLimit;
                rect.y_max += zigzagLimit;

                if (rect.x_min < -1.0f || rect.x_max > 1.0f) rect.zigzagX *= -1;
                if (rect.y_min > rect.zigzagY + 0.15f || rect.y_max < rect.zigzagY - 0.15f) zigzagLimit *= -1;
            }

            if (changeSize) {
                rect.x_min += sizeChange;
                rect.x_max -= sizeChange;
                rect.y_min -= sizeChange;
                rect.y_max += sizeChange;

                if (rect.x_max - rect.x_min > 0.3f || rect.x_max - 0.1f < rect.x_min)
                    sizeChange *= -1;
            }
        }
    }
    glutTimerFunc(16, update, 0);

    glutPostRedisplay();
}

GLvoid colorTimer(int value) {
    for (auto& rect : rectangles) {
        if (changeColor) {
            for (auto i = 0; i < 3; i++) {
                rect.color[i] = RandomRGB();
            }
        }
    }

    glutTimerFunc(1000, colorTimer, 1);
}