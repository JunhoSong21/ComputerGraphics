#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <random>

struct Rect {
    float x_min, y_min, x_max, y_max;
    float color[3];
};

std::vector<Rect> rectangles;
const int MAX_RECTANGLES = 10;

bool isDragging = false;
int selectedRectangle = -1;
float dragOffsetX = 0, dragOffsetY = 0;

float RandomRGB() {
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(0.0f, 1.0f);
    return distr(eng);
}

void DrawRect(const Rect& rect);
bool CheckOverlap(const Rect& r1, const Rect& r2);
void MergeRectangles(Rect& r1, const Rect& r2);
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid MouseMotion(int x, int y);

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Example 3");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(MouseMotion);
    glutMainLoop();
}

void CreateRandomRectangle() {
    if (rectangles.size() >= MAX_RECTANGLES) return;

    Rect newRect;
    newRect.x_min = RandomRGB() * 1.8f - 1.0f;
    newRect.y_min = RandomRGB() * 1.8f - 1.0f;
     
    float size = 0.2f;
    newRect.x_max = newRect.x_min + size;
    newRect.y_max = newRect.y_min + (size / 6 * 8);

    for (auto i = 0; i < 3; ++i)
        newRect.color[i] = RandomRGB();

    rectangles.push_back(newRect);

    glutPostRedisplay();
}

void DrawRect(const Rect& rect) {
    glColor3f(rect.color[0], rect.color[1], rect.color[2]);
    glRectf(rect.x_min, rect.y_min, rect.x_max, rect.y_max);
}

bool CheckOverlap(const Rect& r1, const Rect& r2) {
    return !(r1.x_max < r2.x_min || r1.x_min > r2.x_max ||
        r1.y_max < r2.y_min || r1.y_min > r2.y_max);
}

void MergeRectangles(Rect& r1, const Rect& r2) {
    r1.x_min = std::min(r1.x_min, r2.x_min);
    r1.y_min = std::min(r1.y_min, r2.y_min);
    r1.x_max = std::max(r1.x_max, r2.x_max);
    r1.y_max = std::max(r1.y_max, r2.y_max);

    for (auto i = 0; i < 3; ++i)
        r1.color[i] = RandomRGB();
}

GLvoid drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& rect : rectangles) {
        DrawRect(rect);
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    if (key == 'a') {
        CreateRandomRectangle();
    }
    glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            float xPos = (float)x / 400.0f - 1.0f;
            float yPos = 1.0f - (float)y / 300.0f;

            for (int i = 0; i < rectangles.size(); i++) {
                Rect& rect = rectangles[i];

                if (xPos >= rect.x_min && xPos <= rect.x_max &&
                    yPos >= rect.y_min && yPos <= rect.y_max) {
                    selectedRectangle = i;
                    dragOffsetX = xPos - rect.x_min;
                    dragOffsetY = yPos - rect.y_min;
                    isDragging = true;
                    break;
                }
            }
        }
        else if (state == GLUT_UP) {
            isDragging = false;

            if (selectedRectangle != -1) {
                Rect& rect = rectangles[selectedRectangle];
                for (int i = rectangles.size() - 1; i >= 0; i--) {
                    if (i != selectedRectangle && CheckOverlap(rect, rectangles[i])) {
                        MergeRectangles(rect, rectangles[i]);
                        rectangles.erase(rectangles.begin() + i);
                        break;
                    }
                }
            }
            selectedRectangle = -1;

            glutPostRedisplay();
        }
    }
}

GLvoid MouseMotion(int x, int y) {
    if (isDragging && selectedRectangle != -1) {
        float xPos = (float)x / 400.0f - 1.0f;
        float yPos = 1.0f - (float)y / 300.0f;

        Rect& rect = rectangles[selectedRectangle];
        float width = rect.x_max - rect.x_min;
        float height = rect.y_max - rect.y_min;

        rect.x_min = xPos - dragOffsetX;
        rect.y_min = yPos - dragOffsetY;
        rect.x_max = rect.x_min + width;
        rect.y_max = rect.y_min + height;

        glutPostRedisplay();
    }
}