#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <random>

std::random_device rd;
std::default_random_engine eng(rd());

struct Rect {
    float x, y;
    float RGB[3];
};

struct Erase {
    float x, y;
    float RGB[3];
    bool IsDrag;
};

std::vector<Rect> Recs;
Erase Eraser;

float RandomRGB() {
    std::uniform_real_distribution<float> distr(0.0f, 1.0f);
    return distr(eng);
}

void DrawRect(const Rect& rect);
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
void MakeRandomRec();

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Example 5");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    MakeRandomRec();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
}

void DrawRect(const Rect& rect) {
    glColor3f(rect.RGB[0], rect.RGB[1], rect.RGB[2]);
    glRectf(rect.x - 0.05f, rect.y + 0.065f, rect.x + 0.05f, rect.y - 0.065f);
}

GLvoid drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& rect : Recs) {
        DrawRect(rect);
    }

    if (Eraser.IsDrag) {
        glColor3f(Eraser.RGB[0], Eraser.RGB[1], Eraser.RGB[2]);
        glRectf(Eraser.x - 0.1f, Eraser.y + 0.13f, Eraser.x + 0.1f, Eraser.y - 0.13f);
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
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            Eraser.IsDrag = true;
            Eraser.x = (float)x / 400.0f - 1.0f;
            Eraser.y = 1.0f - (float)y / 300.0f;
        }
        else if (state == GLUT_UP) {
            Eraser.IsDrag = false;
        }
    }

    glutPostRedisplay();
}

GLvoid Motion(int x, int y) {
    if (Eraser.IsDrag == true) {
        Eraser.x = (float)x / 400.0f - 1.0f;
        Eraser.y = 1.0f - (float)y / 300.0f;
    }

    glutPostRedisplay();
}

void MakeRandomRec() {
    int Max_Rec = (int)(RandomRGB() + 1) * 20;
    Rect newRect;

    for (auto i = 0; i < Max_Rec; ++i) {
        newRect.x = RandomRGB() * 1.7f - 0.85f;
        newRect.y = RandomRGB() * 1.7f - 0.85f;

        for (auto i = 0; i < 3; ++i) {
            newRect.RGB[i] = RandomRGB();
        }

        Recs.push_back(newRect);
    }

    for (auto i = 0; i < 3; ++i) {
        Eraser.RGB[i] = 0.f;
    }

    glutPostRedisplay();
}