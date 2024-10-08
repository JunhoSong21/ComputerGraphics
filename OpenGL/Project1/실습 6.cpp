#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <random>

std::random_device rd;
std::default_random_engine eng(rd());

struct Rect {
    float   x, y;
    float   width, height;
    float   RGB[3];
    bool    IsActive;
    int     DirectionOpt; // 0: 좌우상하, 1: 대각선, 2: 같은 방향 3: 좌우상하대각선
    int     Direction; // 0 위쪽 1 오른쪽 2 아래쪽 3 왼쪽 4 우상단 5 우하단 6 좌하단 7 좌상단
};

std::vector<Rect> Recs;

float RandomRGB() {
    std::uniform_real_distribution<float> distr(0.f, 0.9f);
    return distr(eng);
}

float RandomFloat(float min, float max) {
    std::uniform_real_distribution<float> distr(min, max);
    return distr(eng);
}

float WHcombine(float width) {
    float equal = width / 6 * 8;

    return equal;
}

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Timer(int value);
void MakeRandomRecs();
void SubdivideRect(Rect& rect);

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Example 6");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    MakeRandomRecs();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(50, Timer, 0);

    glutMainLoop();
}

GLvoid drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (size_t i = 0; i < Recs.size(); ++i) {
        Rect& rect = Recs[i];
        glColor3f(rect.RGB[0], rect.RGB[1], rect.RGB[2]);
        glRectf(rect.x - rect.width, rect.y - rect.height,
            rect.x + rect.width, rect.y + rect.height);
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    if (key == 'q') {
        glutLeaveMainLoop();
    }

    glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mx = (float)x / 400.0f - 1.0f;
        float my = 1.0f - (float)y / 300.0f;

        for (size_t i = 0; i < Recs.size(); ++i) {
            Rect& rect = Recs[i];

            float left = rect.x - rect.width;
            float right = rect.x + rect.width;
            float top = rect.y + rect.height;
            float bottom = rect.y - rect.height;

            bool IsInside = (mx >= left && mx <= right && my >= bottom && my <= top);

            if (IsInside && !rect.IsActive) {
                SubdivideRect(rect);
                Recs.erase(Recs.begin() + i);
            }
            else
                ++i;
        }
    }

    glutPostRedisplay();
}

GLvoid Timer(int value) {
    for (size_t i = 0; i < Recs.size(); ++i) {
        Rect& rect = Recs[i];

        if (rect.IsActive) {
            if (rect.Direction == 2)
                rect.y += 0.01f;
            else if (rect.Direction == 3)
                rect.x += 0.01f;
            else if (rect.Direction == 0)
                rect.x -= 0.01f;
            else if (rect.Direction == 1)
                rect.y -= 0.01f;
            else if (rect.Direction == 4) {
                rect.x += 0.02f;
                rect.y += 0.02f;
            }
            else if (rect.Direction == 5) {
                rect.x += 0.02f;
                rect.y -= 0.02f;
            }
            else if (rect.Direction == 6) {
                rect.x -= 0.02f;
                rect.y -= 0.02f;
            }
            else if (rect.Direction == 7) {
                rect.x += 0.02f;
                rect.y -= 0.02f;
            }

            rect.width *= 0.99f;
            rect.height *= 0.99f;

            if (rect.width < 0.02f) {
                Recs.erase(Recs.begin() + i);
                continue;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, Timer, 0);
}

void MakeRandomRecs() {
    int numRects = (int)RandomFloat(5.0f, 10.0f);
    Rect newRect;

    for (int i = 0; i < numRects; ++i) {
        newRect.x = RandomFloat(-0.8f, 0.8f);
        newRect.y = RandomFloat(-0.8f, 0.8f);
        newRect.width = RandomFloat(0.1f, 0.3f);
        newRect.height = WHcombine(newRect.width);
        newRect.IsActive = false;
        newRect.DirectionOpt = rand() % 4;
        for (int j = 0; j < 3; ++j)
            newRect.RGB[j] = RandomRGB();
        Recs.push_back(newRect);
    }
}

void SubdivideRect(Rect& rect) {
    // 0: 좌우상하, 1: 대각선, 2: 같은 방향 3: 좌우상하대각선
    // 0 위쪽 1 오른쪽 2 왼쪽 3 아래쪽 4 좌상단 5 우상단 6 좌하단 7 우하단
    float halfW = rect.width / 2.0f;
    float halfH = rect.height / 2.0f;

    Rect newRect;

    if (rect.DirectionOpt == 0) { // 좌우상하
        for (int i = 0; i < 4; ++i) { 
            newRect.width = halfW / 2.0;
            newRect.height = halfH / 2.0;
            newRect.IsActive = true;
            newRect.Direction = i;

            newRect.x = rect.x + ((i % 2) * halfW - halfW / 2);
            newRect.y = rect.y + ((i / 2) * halfH - halfH / 2);

            for (int k = 0; k < 3; ++k)
                newRect.RGB[k] = RandomRGB();

            Recs.push_back(newRect);
        }
    }
    else if (rect.DirectionOpt == 1) { // 대각선
        for (int i = 0; i < 4; ++i) {
            newRect.width = halfW / 2.0;
            newRect.height = halfH / 2.0;
            newRect.IsActive = true;
            newRect.Direction = i + 4;

            newRect.x = rect.x + ((i % 2) * halfW / 2 - halfW / 4);
            newRect.y = rect.y + ((i / 2) * halfH / 2 - halfH / 4);

            for (int k = 0; k < 3; ++k)
                newRect.RGB[k] = RandomRGB();

            Recs.push_back(newRect);
        }
    }
    else if (rect.DirectionOpt == 2) { // 같은 방향
        for (int i = 0; i < 4; ++i) {
            newRect.width = halfW / 2.0;
            newRect.height = halfH / 2.0;
            newRect.IsActive = true;
            newRect.Direction = 1;

            newRect.x = rect.x + ((i % 2) * halfW - halfW / 2);
            newRect.y = rect.y + ((i / 2) * halfH - halfH / 2);

            for (int k = 0; k < 3; ++k)
                newRect.RGB[k] = RandomRGB();

            Recs.push_back(newRect);
        }
    }
    else if (rect.DirectionOpt == 3) { // 좌우상하대각선
        for (int i = 0; i < 8; ++i) {
            newRect.width = halfW / 2.0;
            newRect.height = halfH / 2.0;
            newRect.IsActive = true;
            newRect.Direction = i;

            newRect.x = rect.x + ((i % 4) % 2 * halfW / 2 - halfW / 4);
            newRect.y = rect.y + ((i / 4) * halfH / 2 - halfH / 4);

            for (int k = 0; k < 3; ++k)
                newRect.RGB[k] = RandomRGB();

            Recs.push_back(newRect);
        }
    }
}