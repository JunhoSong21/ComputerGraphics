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

GLfloat triangles[4][9];  // 4���� �ﰢ���� ����
GLfloat colors[4][9];      // �� �ﰢ���� ����
int currentMovement = -1;  // ���� �̵� ��� �ʱⰪ (�̵� ��� ���� ��)
float speeds[4][2];        // �ӵ� (x, y)
float directions[4][2];    // ���� (x, y)
bool trianglesInitialized = false; // �ﰢ���� �ʱ�ȭ�Ǿ����� ����

GLenum Drawmode = GL_FILL;

// �Լ� ����
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
    glutMouseFunc(Mouse); // ���콺 Ŭ�� �̺�Ʈ �߰�
    glutIdleFunc(MoveTriangles); // IDLE �� �ﰢ�� �̵�

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);
    if (trianglesInitialized) { // �ﰢ���� �ʱ�ȭ�� ��쿡�� �׸��ϴ�.
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
        currentMovement = 0;  // ƨ���
        break;
    case '2':
        currentMovement = 1;  // �¿�� �������
        break;
    case '3':
        currentMovement = 2;  // �簢 �����̷�
        break;
    case '4':
        currentMovement = 3;  // �� �����̷�
        break;
    default:
        break;
    }
}

GLvoid Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        ResetTriangles(); // ��Ŭ�� �� �ﰢ�� �ʱ�ȭ
        trianglesInitialized = true; // �ﰢ�� �ʱ�ȭ �÷��� ����
        glutPostRedisplay(); // ȭ�� ������Ʈ
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
        float a = 0.1f; // �ﰢ���� ����
        triangles[i][0] = mx;                  // ������ 1
        triangles[i][1] = a;                    // ������ 1
        triangles[i][2] = 0.0f;                  // ����
        triangles[i][3] = mx - 0.05f;           // ������ 2
        triangles[i][4] = -a;                   // ������ 2
        triangles[i][5] = 0.0f;                  // ����
        triangles[i][6] = mx + 0.05f;           // ������ 3
        triangles[i][7] = -a;                   // ������ 3
        triangles[i][8] = 0.0f;                  // ����

        for (int j = 0; j < 3; j++) {
            colors[i][3 * j] = RandomRGB();
            colors[i][3 * j + 1] = RandomRGB();
            colors[i][3 * j + 2] = RandomRGB();
        }

        // �ӵ� �� ���� �ʱ�ȭ
        speeds[i][0] = 0.01f * (i + 1); // �� �ﰢ������ �ٸ� �ӵ�
        speeds[i][1] = 0.01f * (i + 1);
        directions[i][0] = 1.0f; // �ʱ� ����
        directions[i][1] = 1.0f;
    }
}

void MoveTriangles() {
    if (currentMovement == -1) return; // �̵� ��尡 �������� ���� ���

    for (int i = 0; i < 4; ++i) {
        switch (currentMovement) {
        case 0: // ƨ���
            triangles[i][0] += speeds[i][0] * directions[i][0];
            triangles[i][1] += speeds[i][1] * directions[i][1];

            // ���� �ε����� ���� �ٲٱ�
            if (triangles[i][0] >= 1.0f || triangles[i][0] <= -1.0f) {
                directions[i][0] *= -1; // x ���� ����
            }
            if (triangles[i][1] >= 1.0f || triangles[i][1] <= -1.0f) {
                directions[i][1] *= -1; // y ���� ����
            }
            break;

        case 1: // �¿�� �������
            triangles[i][0] += speeds[i][0] * directions[i][0];
            if (triangles[i][0] >= 1.0f || triangles[i][0] <= -1.0f) {
                directions[i][0] *= -1; // x ���� ����
            }
            triangles[i][1] += (i % 2 == 0 ? 0.01f : -0.01f); // ¦���� ����, Ȧ���� �Ʒ���
            if (triangles[i][1] >= 1.0f) {
                triangles[i][1] = 1.0f; // ��� ����
            }
            else if (triangles[i][1] <= -1.0f) {
                triangles[i][1] = -1.0f; // ��� ����
            }
            break;

        case 2: // �簢 �����̷�
            triangles[i][0] += speeds[i][0] * cos(i + glutGet(GLUT_ELAPSED_TIME) * 0.001);
            triangles[i][1] += speeds[i][1] * sin(i + glutGet(GLUT_ELAPSED_TIME) * 0.001);
            break;

        case 3: // �� �����̷�
            triangles[i][0] = 0.5f * cos(glutGet(GLUT_ELAPSED_TIME) * 0.001 + i) * (1.0f - 0.005f * (glutGet(GLUT_ELAPSED_TIME) * 0.001));
            triangles[i][1] = 0.5f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.001 + i) * (1.0f - 0.005f * (glutGet(GLUT_ELAPSED_TIME) * 0.001));
            break;
        }
    }
    glutPostRedisplay(); // ȭ�� ������Ʈ
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
