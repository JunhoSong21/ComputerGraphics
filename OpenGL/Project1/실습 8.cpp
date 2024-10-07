#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/freeglut.h>

struct Triangle {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    GLenum mode;
};

struct Line {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    GLenum mode;
};

std::vector<Triangle> triangles[4];
GLenum currentMode = GL_FILL;
int maxTrianglesPerQuadrant = 3;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
void createTriangle(int quadrant, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& colors);
void deleteTriangle(int quadrant);
void drawTriangles();
void drawLine(GLenum mode, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& colors);
void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();

char* filetobuf(const char* file) {
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(file, "rb");
    if (!fptr) return nullptr;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}

GLuint vao, vertexVBO, colorVBO;
GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Example 8");

    glewExperimental = GL_TRUE;
    glewInit();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutMainLoop();
    return 0;
}

GLvoid drawScene() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // X��� Y�� �׸���
    glLineWidth(2.0f);
    std::vector<GLfloat> lineVertices = { -1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f };
    std::vector<GLfloat> lineColors = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
    drawLine(GL_LINES, lineVertices, lineColors);

    // �ﰢ�� �׸���
    glUseProgram(shaderProgramID);
    drawTriangles();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    if (key == 'a') {
        currentMode = GL_FILL; // ������ �׸���
    }
    if (key == 'b') {
        currentMode = GL_LINE; // ������ �׸���
    }

    glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        float normalizedX = (float)x / (windowWidth / 2) - 1.0f;
        float normalizedY = 1.0f - (float)y / (windowHeight / 2);

        int quadrant = 0;

        if (normalizedX > 0 && normalizedY > 0) {
            quadrant = 0; // 1��и�
        }
        else if (normalizedX < 0 && normalizedY > 0) {
            quadrant = 1; // 2��и�
        }
        else if (normalizedX < 0 && normalizedY < 0) {
            quadrant = 2; // 3��и�
        }
        else if (normalizedX > 0 && normalizedY < 0) {
            quadrant = 3; // 4��и�
        }

        if (button == GLUT_LEFT_BUTTON) {
            deleteTriangle(quadrant); // ���� �ﰢ�� ����

            GLfloat randomColor[3] = { (GLfloat)(rand() % 100) / 100.0f, (GLfloat)(rand() % 100) / 100.0f, (GLfloat)(rand() % 100) / 100.0f };

            // �̵ �ﰢ���� ��ǥ ����
            std::vector<GLfloat> vertices = {
                normalizedX, normalizedY + 0.1f, 0.0f,
                normalizedX - 0.1f, normalizedY - 0.1f, 0.0f,
                normalizedX + 0.1f, normalizedY - 0.1f, 0.0f
            };

            std::vector<GLfloat> colors = {
                randomColor[0], randomColor[1], randomColor[2],
                randomColor[0], randomColor[1], randomColor[2],
                randomColor[0], randomColor[1], randomColor[2],
            };

            createTriangle(quadrant, vertices, colors);
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            if (triangles[quadrant].size() < maxTrianglesPerQuadrant) {
                GLfloat randomColor[3] = { (GLfloat)(rand() % 100) / 100.0f, (GLfloat)(rand() % 100) / 100.0f, (GLfloat)(rand() % 100) / 100.0f };

                std::vector<GLfloat> vertices = {
                    normalizedX, normalizedY + 0.1f, 0.0f,
                    normalizedX - 0.1f, normalizedY - 0.1f, 0.0f,
                    normalizedX + 0.1f, normalizedY - 0.1f, 0.0f
                };

                std::vector<GLfloat> colors = {
                    randomColor[0], randomColor[1], randomColor[2],
                    randomColor[0], randomColor[1], randomColor[2],
                    randomColor[0], randomColor[1], randomColor[2]
                };

                createTriangle(quadrant, vertices, colors);
            }
        }
    }

    glutPostRedisplay();
}

void createTriangle(int quadrant, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& colors) {
    if (triangles[quadrant].size() < maxTrianglesPerQuadrant) {
        Triangle newTriangle;
        newTriangle.vertices = vertices;
        newTriangle.colors = colors;
        newTriangle.mode = currentMode;

        triangles[quadrant].push_back(newTriangle);
    }
}

void deleteTriangle(int quadrant) {
    if (!triangles[quadrant].empty()) {
        triangles[quadrant].pop_back(); // ������ �ﰢ�� ����
    }
}

void drawTriangles() {
    glBindVertexArray(vao);

    for (int i = 0; i < 4; ++i) {
        for (const Triangle& triangle : triangles[i]) {
            glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
            glBufferData(GL_ARRAY_BUFFER, triangle.vertices.size() * sizeof(GLfloat), triangle.vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
            glBufferData(GL_ARRAY_BUFFER, triangle.colors.size() * sizeof(GLfloat), triangle.colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            glDrawArrays(triangle.mode, 0, triangle.vertices.size() / 3);
        }
    }

    glBindVertexArray(0);
}

void drawLine(GLenum mode, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& colors) {
    Line line;
    line.mode = GL_LINES;
    line.vertices = vertices;
    line.colors = colors;
    // ���� �׸��� ������ �߰��ؾ� ������ �� ���������� ����
}

void InitBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &colorVBO);
    glBindVertexArray(0);
}

void make_shaderProgram() {
    make_vertexShaders(); //--- ���ؽ� ���̴� �����
    make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
    //-- shader Program
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    //--- ���̴� �����ϱ�
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //--- Shader Program ����ϱ�
    glUseProgram(shaderProgramID);
}

void make_vertexShaders() {
    vertexSource = filetobuf("vertex.glsl");
    //--- ���ؽ� ���̴� ��ü �����
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
    //--- ���ؽ� ���̴� �������ϱ�
    glCompileShader(vertexShader);
    //--- �������� ����� ���� ���� ���: ���� üũ
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
        return;
    }
}

void make_fragmentShaders() {
    fragmentSource = filetobuf("fragment.glsl");
    //--- �����׸�Ʈ ���̴� ��ü �����
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
    //--- �����׸�Ʈ ���̴� ������
    glCompileShader(fragmentShader);
    //--- �������� ����� ���� ���� ���: ������ ���� üũ
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
        return;
    }
}
