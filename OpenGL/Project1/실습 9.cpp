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
    GLfloat squareSize; // 사각형 크기
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

float spiralAngle = 0.0f; // 스파이럴 각도
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

            newTriangle.squareSize = 2.0f; // 초기 사각형 크기
            newTriangle.direction = 0; // 초기 방향 설정 (오른쪽)

            triangles.push_back(newTriangle);
        }
    }
}

void drawTriangle(Triangle& triangle) {
    GLfloat halfSize = triangle.size / 2.0f;

    // 회전 변환 행렬 생성
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(triangle.angle), glm::vec3(0.0f, 0.0f, 1.0f));

    // 정점 위치
    std::vector<glm::vec4> vertices = {
        rotationMatrix * glm::vec4(0.0f, halfSize, 0.0f, 1.0f),        // 꼭짓점 위
        rotationMatrix * glm::vec4(-halfSize, -halfSize, 0.0f, 1.0f),  // 꼭짓점 왼쪽 아래
        rotationMatrix * glm::vec4(halfSize, -halfSize, 0.0f, 1.0f)    // 꼭짓점 오른쪽 아래
    };

    // 삼각형을 그리기 위해 OpenGL 버퍼에 데이터 전달
    GLfloat verticesData[9];
    for (int i = 0; i < 3; i++) {
        verticesData[i * 3] = vertices[i].x + triangle.x;   // x 좌표
        verticesData[i * 3 + 1] = vertices[i].y + triangle.y; // y 좌표
        verticesData[i * 3 + 2] = 0.0f;                       // z 좌표
    }

    GLfloat colorData[9] = {
    triangle.r, triangle.g, triangle.b, // 꼭짓점 위 색상
    triangle.r, triangle.g, triangle.b, // 꼭짓점 왼쪽 아래 색상
    triangle.r, triangle.g, triangle.b  // 꼭짓점 오른쪽 아래 색상
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // 색상 데이터 버퍼 바인딩
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // 색상 속성 설정
    glEnableVertexAttribArray(1);

    // 삼각형 그리기
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
                    triangle.angle = -90.f; // 왼쪽 벽에 부딪혔을 때
                }
                else {
                    triangle.angle = 90.0f; // 오른쪽 벽에 부딪혔을 때
                }
            }
            if (triangle.y - halfSize <= -1.0f || triangle.y + halfSize >= 1.0f) {
                triangle.vy = -triangle.vy;
                if (triangle.y - halfSize <= -1.0f) {
                    triangle.angle = 0.0f; // 아래 벽에 부딪혔을 때
                }
                else {
                    triangle.angle = 180.0f; // 위쪽 벽에 부딪혔을 때
                }
            }
        }

        if (isZigzag) {
            // 삼각형의 위치 업데이트
            triangle.x += triangle.vx;

            // 벽에 부딪힐 때 처리
            if (triangle.x - halfSize <= -1.0f && triangle.vx < 0) {
                triangle.vx *= -1; // 오른쪽으로 방향 전환
                triangle.angle = -90.f; // 방향 조정
                triangle.y -= 0.07f; // 아래로 이동
            }
            else if (triangle.x + halfSize >= 1.0f && triangle.vx > 0) {
                triangle.vx *= -1; // 왼쪽으로 방향 전환
                triangle.angle = 90.0f; // 방향 조정
                triangle.y -= 0.07f; // 아래로 이동
            }
        }

        if (isSquareSpiral) {
            // 방향에 따라 이동
            switch (triangle.direction) {
            case 0: // 오른쪽
                triangle.x += 0.02f; // 이동
                if (triangle.x + halfSize >= triangle.squareSize / 2 || triangle.x + halfSize >= 1.0f) {
                    triangle.direction = 1; // 아래로 방향 전환
                    triangle.angle = 90.0f;
                    triangle.squareSize -= 0.1f; // 사각형 크기 감소
                    if (triangle.squareSize < 0.01f) {
                        triangle.squareSize = 0.01f; // 사각형 크기 최소값
                    }
                }
                break;
            case 1: // 아래
                triangle.y -= 0.02f; // 이동
                if (triangle.y - halfSize <= -triangle.squareSize / 2 || triangle.y - halfSize <= -1.0f) {
                    triangle.direction = 2; // 왼쪽으로 방향 전환
                    triangle.angle = 0.0f;
                    triangle.squareSize -= 0.1f; // 사각형 크기 감소
                    if (triangle.squareSize < 0.01f) {
                        triangle.squareSize = 0.01f; // 사각형 크기 최소값
                    }
                }
                break;
            case 2: // 왼쪽
                triangle.x -= 0.02f; // 이동
                if (triangle.x - halfSize <= -triangle.squareSize / 2 || triangle.x - halfSize <= -1.0f) {
                    triangle.direction = 3; // 위로 방향 전환
                    triangle.angle = -90.f;
                    triangle.squareSize -= 0.1f; // 사각형 크기 감소
                    if (triangle.squareSize < 0.01f) {
                        triangle.squareSize = 0.01f; // 사각형 크기 최소값
                    }
                }
                break;
            case 3: // 위쪽
                triangle.y += 0.02f; // 이동
                if (triangle.y + halfSize >= triangle.squareSize / 2 || triangle.y + halfSize >= 1.0f) {
                    triangle.direction = 0; // 오른쪽으로 방향 전환
                    triangle.angle = 180.0f;
                    triangle.squareSize -= 0.1f; // 사각형 크기 감소
                    if (triangle.squareSize < 0.01f) {
                        triangle.squareSize = 0.01f; // 사각형 크기 최소값
                    }
                }
                break;
            }
        }

        if (isCircularSpiral) {
            // 삼각형의 각도에 따라 위치 업데이트
            triangle.x = spiralRadius * cos(spiralAngle);
            triangle.y = spiralRadius * sin(spiralAngle);

            // 각도 증가 (스파이럴 효과)
            spiralAngle += 0.1f; // 각도 증가
            spiralRadius += 0.01f; // 반지름 증가하여 점점 넓어짐

            // 회전 각도 업데이트
            if (triangle.angle < 180.f)
                triangle.angle += 8.0f; // 회전 속도 설정 (예: 매 프레임 2도 회전)
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
            triangle.x = (rand() % 200 - 100) / 100.0f; // 랜덤한 X 위치 설정
            triangle.y = 1.0f; // Y 위치를 최상단으로 설정
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