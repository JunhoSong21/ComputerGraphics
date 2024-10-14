#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

using namespace std;

GLuint vao;
GLuint vbo[2];

int maxTri = 4;

char* filetobuf(const char* file) {
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(file, "rb");
    if (!fptr)
        return nullptr;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;
    return buf;
}

struct Triangle {
    GLfloat x, y;
    GLfloat size;
    GLfloat r, g, b;
    GLfloat vx, vy;
    bool exists;
    bool isMovingUp;
    bool isMovingDown;
    bool isMovingLeft;
    bool isMovingRight;
    GLfloat angle;
    GLfloat squareSize; // �簢�� ũ��
    int direction;
};

vector<Triangle> triangles;

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Timer(int value);

GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

int window_Width = 800;
int window_Height = 600;

bool isMoving = false;
bool isZigzag = false;

bool isSquareSpiral = false;
GLfloat spiralStep = 0.05f;
GLfloat currentStep = 0.0f;
int direction = 0;

bool isCircularSpiral = false;
GLfloat theta = 0.0f;
GLfloat radius = 0.05f;
GLfloat radiusIncrement = 0.01f;

float spiralAngle = 0.0f; // �����̷� ����
float spiralRadius = 0.1f;
float rotationAngle = 0.0f;

void InitBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(2, vbo);
}

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Example 9");

    glewExperimental = GL_TRUE;
    glewInit();

    srand(static_cast<unsigned int>(time(0)));

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutTimerFunc(16, Timer, 0);

    glutMainLoop();
}

void make_vertexShaders() {
    GLchar* vertexSource;

    vertexSource = filetobuf("vertex.glsl");

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader\n" << errorLog << std::endl;
        return;
    }
}

void make_fragmentShaders() {
    GLchar* fragmentSource;

    fragmentSource = filetobuf("fragment.glsl");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: frag_shader\n" << errorLog << std::endl;
        return;
    }
}

GLuint make_shaderProgram() {
    make_vertexShaders();
    make_fragmentShaders();

    shaderProgramID = glCreateProgram();

    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgramID);
    return shaderProgramID;
}

void createRandomTriangle(GLfloat x, GLfloat y, bool isRightClick) {
    if (!isRightClick) {
        if (triangles.size() < maxTri) {
            Triangle newTriangle;
            newTriangle.x = x;
            newTriangle.y = y;
            newTriangle.size = (rand() % 40 + 10) / 100.0f;
            newTriangle.r = rand() / (float)RAND_MAX;
            newTriangle.g = rand() / (float)RAND_MAX;
            newTriangle.b = rand() / (float)RAND_MAX;
            newTriangle.vx = ((rand() % 2 == 0) ? 0.05f : -0.05f);
            newTriangle.vy = ((rand() % 2 == 0) ? 0.05f : -0.05f);
            newTriangle.exists = true;

            newTriangle.isMovingUp = false;
            newTriangle.isMovingDown = false;
            newTriangle.isMovingLeft = false;
            newTriangle.isMovingRight = false;
            newTriangle.angle = 0.f;

            newTriangle.squareSize = 2.0f; // �ʱ� �簢�� ũ��
            newTriangle.direction = 0; // �ʱ� ���� ���� (������)

            triangles.push_back(newTriangle);
        }
    }
}

void drawTriangle(Triangle& triangle) {
    GLfloat halfSize = triangle.size / 2.0f;

    // ȸ�� ��ȯ ��� ����
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(triangle.angle), glm::vec3(0.0f, 0.0f, 1.0f));

    // ���� ��ġ
    std::vector<glm::vec4> vertices = {
        rotationMatrix * glm::vec4(0.0f, halfSize, 0.0f, 1.0f),        // ������ ��
        rotationMatrix * glm::vec4(-halfSize, -halfSize, 0.0f, 1.0f),  // ������ ���� �Ʒ�
        rotationMatrix * glm::vec4(halfSize, -halfSize, 0.0f, 1.0f)    // ������ ������ �Ʒ�
    };

    // �ﰢ���� �׸��� ���� OpenGL ���ۿ� ������ ����
    GLfloat verticesData[9];
    for (int i = 0; i < 3; i++) {
        verticesData[i * 3] = vertices[i].x + triangle.x;   // x ��ǥ
        verticesData[i * 3 + 1] = vertices[i].y + triangle.y; // y ��ǥ
        verticesData[i * 3 + 2] = 0.0f;                       // z ��ǥ
    }

    GLfloat colorData[9] = {
    triangle.r, triangle.g, triangle.b, // ������ �� ����
    triangle.r, triangle.g, triangle.b, // ������ ���� �Ʒ� ����
    triangle.r, triangle.g, triangle.b  // ������ ������ �Ʒ� ����
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // ���� ������ ���� ���ε�
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // ���� �Ӽ� ����
    glEnableVertexAttribArray(1);

    // �ﰢ�� �׸���
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLvoid Timer(int value) {
    for (auto& triangle : triangles) {
        GLfloat halfSize = triangle.size / 2.0f;

        if (isMoving) {
            triangle.x += triangle.vx;
            triangle.y += triangle.vy;

            if (triangle.x - halfSize <= -1.0f || triangle.x + halfSize >= 1.0f) {
                triangle.vx = -triangle.vx;
                if (triangle.x - halfSize <= -1.0f) {
                    triangle.angle = -90.f; // ���� ���� �ε����� ��
                }
                else {
                    triangle.angle = 90.0f; // ������ ���� �ε����� ��
                }
            }
            if (triangle.y - halfSize <= -1.0f || triangle.y + halfSize >= 1.0f) {
                triangle.vy = -triangle.vy;
                if (triangle.y - halfSize <= -1.0f) {
                    triangle.angle = 0.0f; // �Ʒ� ���� �ε����� ��
                }
                else {
                    triangle.angle = 180.0f; // ���� ���� �ε����� ��
                }
            }
        }

        if (isZigzag) {
            // �ﰢ���� ��ġ ������Ʈ
            triangle.x += triangle.vx;

            // ���� �ε��� �� ó��
            if (triangle.x - halfSize <= -1.0f && triangle.vx < 0) {
                triangle.vx *= -1; // ���������� ���� ��ȯ
                triangle.angle = -90.f; // ���� ����
                triangle.y -= 0.07f; // �Ʒ��� �̵�
            }
            else if (triangle.x + halfSize >= 1.0f && triangle.vx > 0) {
                triangle.vx *= -1; // �������� ���� ��ȯ
                triangle.angle = 90.0f; // ���� ����
                triangle.y -= 0.07f; // �Ʒ��� �̵�
            }
        }

        if (isSquareSpiral) {
            // ���⿡ ���� �̵�
            switch (triangle.direction) {
            case 0: // ������
                triangle.x += 0.02f; // �̵�
                if (triangle.x + halfSize >= triangle.squareSize / 2 || triangle.x + halfSize >= 1.0f) {
                    triangle.direction = 1; // �Ʒ��� ���� ��ȯ
                    triangle.angle = 90.0f;
                    triangle.squareSize -= 0.1f; // �簢�� ũ�� ����
                    if (triangle.squareSize < 0.01f) {
                        triangle.squareSize = 0.01f; // �簢�� ũ�� �ּҰ�
                    }
                }
                break;
            case 1: // �Ʒ�
                triangle.y -= 0.02f; // �̵�
                if (triangle.y - halfSize <= -triangle.squareSize / 2 || triangle.y - halfSize <= -1.0f) {
                    triangle.direction = 2; // �������� ���� ��ȯ
                    triangle.angle = 0.0f;
                    triangle.squareSize -= 0.1f; // �簢�� ũ�� ����
                    if (triangle.squareSize < 0.01f) {
                        triangle.squareSize = 0.01f; // �簢�� ũ�� �ּҰ�
                    }
                }
                break;
            case 2: // ����
                triangle.x -= 0.02f; // �̵�
                if (triangle.x - halfSize <= -triangle.squareSize / 2 || triangle.x - halfSize <= -1.0f) {
                    triangle.direction = 3; // ���� ���� ��ȯ
                    triangle.angle = -90.f;
                    triangle.squareSize -= 0.1f; // �簢�� ũ�� ����
                    if (triangle.squareSize < 0.01f) {
                        triangle.squareSize = 0.01f; // �簢�� ũ�� �ּҰ�
                    }
                }
                break;
            case 3: // ����
                triangle.y += 0.02f; // �̵�
                if (triangle.y + halfSize >= triangle.squareSize / 2 || triangle.y + halfSize >= 1.0f) {
                    triangle.direction = 0; // ���������� ���� ��ȯ
                    triangle.angle = 180.0f;
                    triangle.squareSize -= 0.1f; // �簢�� ũ�� ����
                    if (triangle.squareSize < 0.01f) {
                        triangle.squareSize = 0.01f; // �簢�� ũ�� �ּҰ�
                    }
                }
                break;
            }
        }

        if (isCircularSpiral) {
            // �ﰢ���� ������ ���� ��ġ ������Ʈ
            triangle.x = spiralRadius * cos(spiralAngle);
            triangle.y = spiralRadius * sin(spiralAngle);

            // ���� ���� (�����̷� ȿ��)
            spiralAngle += 0.1f; // ���� ����
            spiralRadius += 0.01f; // ������ �����Ͽ� ���� �о���

            // ȸ�� ���� ������Ʈ
            if (triangle.angle < 180.f)
                triangle.angle += 8.0f; // ȸ�� �ӵ� ���� (��: �� ������ 2�� ȸ��)
            else
                triangle.angle = triangle.angle - 180.f + 0.f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(60, Timer, 0);
}

GLvoid drawScene() {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (Triangle& triangle : triangles) {
        if (triangle.exists) {
            drawTriangle(triangle);
        }
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        GLfloat x_opengl = (2.0f * x / window_Width) - 1.0f;
        GLfloat y_opengl = 1.0f - (2.0f * y / window_Height);

        if (button == GLUT_LEFT_BUTTON) {
            createRandomTriangle(x_opengl, y_opengl, false);
        }
        glutPostRedisplay();
    }
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        isMoving = true;
        isZigzag = false;
        isCircularSpiral = false;
        isSquareSpiral = false;
        break;
    case '2':
        isMoving = false;
        isSquareSpiral = false;
        isCircularSpiral = false;
        isZigzag = true;
        break;
    case '3':
        for (auto& triangle : triangles) {
            triangle.x = (rand() % 200 - 100) / 100.0f; // ������ X ��ġ ����
            triangle.y = 1.0f; // Y ��ġ�� �ֻ������ ����
        }
        isMoving = false;
        isZigzag = false;
        isSquareSpiral = true;
        isCircularSpiral = false;
        currentStep = 0.0f;
        direction = 0;
        break;
    case '4':
        isMoving = false;
        isZigzag = false;
        isSquareSpiral = false;
        isCircularSpiral = true;
        theta = 0.0f;
        radius = 0.05f;
        break;
    case 'q':
        glutLeaveMainLoop();
        break;
    }

    glutPostRedisplay();
}