#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>
#include <random>

std::random_device rd;
std::default_random_engine eng(rd());

struct Basket {
    float x;
    float y;
    float RGB[3];
};

struct Elements {
    float x, y;
    float RGB[3];
    int Edge;
    bool IsDivide;
    bool IsThrowRight;
    bool IsDivideLeft;
    bool IsEat;
};

struct SliceLine {
    float startX, startY;
    float endX, endY;
    float RGB[3];
};

float RandomRGB() {
    std::uniform_real_distribution<float> distr(0.0f, 1.0f);
    return distr(eng);
}

int RandomEdge() {
    std::uniform_int_distribution<int> distr(3, 4);
    return distr(eng);
}

std::vector<Elements> elements;
std::vector<Elements> Divide;

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid MouseMove(int x, int y);
GLvoid Timer(int value);

void drawBasket();
void makeElements();
void drawElements();
void drawDragLine();
void collisionCheck();
void basketCheck();

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

Basket basket;
SliceLine dragLine;

bool IsDrag = false;
bool IsFillMode = true;
bool IsBasketRightMove = true;
bool IsThrowRight = true;

float throwSpeed = 0.01f;

unsigned int TimeCount = 0;
unsigned int smallTri = 0;
unsigned int bigTri = 0;

char text[50];

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);

    glutCreateWindow("Poligon Slice");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    make_shaderProgram();
    InitBuffer();

    basket.x = 0.5f;
    basket.y = -0.7f;
    basket.RGB[0] = 0.0f;
    basket.RGB[1] = 0.0f;
    basket.RGB[2] = 1.0f;

    for (int i = 0; i < 3; ++i)
        dragLine.RGB[i] = 0.0f;
    
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(MouseMove);
    glutTimerFunc(16, Timer, 1);

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawBasket();

    if (IsFillMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if(!IsFillMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    drawElements();

    if (IsDrag)
        drawDragLine();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'm':
        IsFillMode = !IsFillMode;
        break;
    case '[':
        if(throwSpeed <= 0.03f)
            throwSpeed += 0.005f;
        break;
    case ']':
        if (throwSpeed > 0.005f)
            throwSpeed -= 0.005f;
        break;
    case 'q':
        glutLeaveMainLoop();
        break;
    }

    glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
    float mx = (float)x / 400.0f - 1.0f;
    float my = 1.0f - (float)y / 400.0f;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        IsDrag = true;
        dragLine.startX = mx;
        dragLine.startY = my;
        dragLine.endX = mx;
        dragLine.endY = my;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        IsDrag = false;
        collisionCheck();
    }
    

    glutPostRedisplay();
}

GLvoid MouseMove(int x, int y) {
    float mx = (float)x / 400.0f - 1.0f;
    float my = 1.0f - (float)y / 400.0f;

    if (IsDrag == true) {
        dragLine.endX = mx;
        dragLine.endY = my;
    }

    glutPostRedisplay();
}

GLvoid Timer(int value) {
    if (basket.x >= 0.6f || basket.x <= -0.6f)
        IsBasketRightMove = !IsBasketRightMove;

    if (IsBasketRightMove)
        basket.x += 0.01f;
    else if (!IsBasketRightMove)
        basket.x -= 0.01f;

    if (TimeCount % 100 == 0) { // 특정 시간마다 도형 생성후 방향 변경
        makeElements();
        IsThrowRight = !IsThrowRight;
    }

    for (size_t i = 0; i < elements.size(); ++i) {
        Elements& element = elements[i];

        if (element.IsThrowRight)
            element.x += throwSpeed * 3;
        else if (!element.IsThrowRight)
            element.x -= throwSpeed * 3;

        if (element.x >= -0.3f && element.IsThrowRight)
            element.y -= throwSpeed * 1.5;
        else if (element.x <= 0.3f && !element.IsThrowRight)
            element.y -= throwSpeed * 1.5;
        else
            element.y -= throwSpeed;
    }
    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
            [](const Elements& element) { return element.y < -0.8f; }),
        elements.end()
    );

    for (size_t i = 0; i < Divide.size(); ++i) {
        Elements& element = Divide[i];

        if (!element.IsEat)
            element.y -= throwSpeed;
        else if (element.IsEat)
            element.x = basket.x;
    }
    Divide.erase(
        std::remove_if(Divide.begin(), Divide.end(),
            [](const Elements& element) { return element.y < -0.9f; }),
        Divide.end()
    );

    basketCheck();

    TimeCount += 1;
    glutPostRedisplay();
    glutTimerFunc(16, Timer, 1);
}

void drawBasket() {
    glm::mat4 Conversion = glm::mat4(1.f);
    Conversion = glm::translate(Conversion, glm::vec3(0.0f, 0.0f, 0.0f));

    std::vector<GLfloat> vertices = {
        basket.x - 0.2f, basket.y - 0.02f , 0.f,
        basket.x + 0.2f, basket.y - 0.02f, 0.f,
        basket.x + 0.2f, basket.y + 0.02f, 0.f,
        basket.x - 0.2f, basket.y + 0.02f, 0.f
    };
    std::vector<GLfloat> colors = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };
    std::vector<GLuint> index = {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteBuffers(1, &ibo);
}

void makeElements() {
    Elements newElement;

    if (IsThrowRight)
        newElement.x = -1.1f;
    else if (!IsThrowRight)
        newElement.x = 1.1f;
    newElement.y = 0.8f;
    for (int i = 0; i < 3; ++i)
        newElement.RGB[i] = RandomRGB();
    newElement.Edge = RandomEdge();
    newElement.IsDivide = false;
    newElement.IsThrowRight = IsThrowRight;

    elements.push_back(newElement);
}

void drawElements() {
    for (size_t i = 0; i < elements.size(); ++i) {
        Elements& element = elements[i];

        glm::mat4 Conversion = glm::mat4(1.f);
        Conversion = glm::translate(Conversion, glm::vec3(0.0f, 0.0f, 0.0f));

        if (!element.IsDivide && element.Edge == 3) {
            std::vector<GLfloat> vertices = {
                element.x - 0.15f, element.y - 0.15f, 0.0f,
                element.x + 0.15f, element.y - 0.15f, 0.0f,
                element.x, element.y + 0.15f, 0.0f
            };
            std::vector<GLfloat> colors = {
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2]
            };
            std::vector<GLuint> index = {
                0, 1, 2
            };

            unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            GLuint ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glDeleteBuffers(1, &ibo);
        }
        else if (element.Edge == 4) {
            std::vector<GLfloat> vertices = {
                element.x - 0.15f, element.y - 0.15f, 0.0f,
                element.x + 0.15f, element.y - 0.15f, 0.0f,
                element.x + 0.15f, element.y + 0.15f, 0.0f,
                element.x - 0.15f, element.y + 0.15f, 0.0f
            };
            std::vector<GLfloat> colors = {
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2]
            };
            std::vector<GLuint> index = {
                0, 1, 2,
                2, 3, 0
            };

            unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            GLuint ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glDeleteBuffers(1, &ibo);
        }
    }
    for (size_t i = 0; i < Divide.size(); ++i) {
        Elements& element = Divide[i];

        glm::mat4 Conversion = glm::mat4(1.f);
        Conversion = glm::translate(Conversion, glm::vec3(0.0f, 0.0f, 0.0f));

        if (element.IsDivide && element.Edge == 2 && element.IsDivideLeft) {
            std::vector<GLfloat> vertices = {
                element.x - 0.08f, element.y - 0.15f, 0.0f,
                element.x + 0.08f, element.y - 0.15f, 0.0f,
                element.x + 0.08f, element.y + 0.15f, 0.0f
            };
            std::vector<GLfloat> colors = {
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2]
            };
            std::vector<GLuint> index = {
                0, 1, 2
            };

            unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            GLuint ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glDeleteBuffers(1, &ibo);
        }
        else if (element.IsDivide && element.Edge == 2 && !element.IsDivideLeft) {
            std::vector<GLfloat> vertices = {
                element.x - 0.08f, element.y - 0.15f, 0.0f,
                element.x + 0.08f, element.y - 0.15f, 0.0f,
                element.x - 0.08f, element.y + 0.15f, 0.0f
            };
            std::vector<GLfloat> colors = {
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2]
            };
            std::vector<GLuint> index = {
                0, 1, 2
            };

            unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            GLuint ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glDeleteBuffers(1, &ibo);
        }
        else if (element.IsDivide && element.Edge == 3 && element.IsDivideLeft) {
            std::vector<GLfloat> vertices = {
                element.x, element.y, 0.0f,
                element.x + 0.3f, element.y, 0.0f,
                element.x, element.y + 0.3f, 0.0f
            };
            std::vector<GLfloat> colors = {
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2]
            };
            std::vector<GLuint> index = {
                0, 1, 2
            };

            unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            GLuint ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glDeleteBuffers(1, &ibo);
        }
        else if (element.IsDivide && element.Edge == 3 && !element.IsDivideLeft) {
            std::vector<GLfloat> vertices = {
                element.x, element.y, 0.0f,
                element.x - 0.3f, element.y, 0.0f,
                element.x, element.y - 0.3f, 0.0f
            };
            std::vector<GLfloat> colors = {
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2],
                element.RGB[0], element.RGB[1], element.RGB[2]
            };
            std::vector<GLuint> index = {
                0, 1, 2
            };

            unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            GLuint ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glDeleteBuffers(1, &ibo);
            }
    }
}

void drawDragLine() {
    glm::mat4 Conversion = glm::mat4(1.f);
    Conversion = glm::translate(Conversion, glm::vec3(0.0f, 0.0f, 0.0f));

    std::vector<GLfloat> vertices = {
        dragLine.startX, dragLine.startY, 0.0f,
        dragLine.endX, dragLine.endY, 0.0f
    };

    std::vector<GLfloat> colors = {
        dragLine.RGB[0], dragLine.RGB[1], dragLine.RGB[2],
        dragLine.RGB[0], dragLine.RGB[1], dragLine.RGB[2]
    };

    std::vector<GLuint> index = {
        0, 1
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));
    
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

    glDrawElements(GL_LINES, index.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteBuffers(1, &ibo);
}

void collisionCheck() {
    float CheckDotX = (dragLine.startX + dragLine.endX) / 2.0f;
    float CheckDotY = (dragLine.startY + dragLine.endY) / 2.0f;

    Elements newElement;

    for (auto it = elements.begin(); it != elements.end(); ) {
        Elements& element = *it;

        if (element.x + 0.15f >= CheckDotX && element.x - 0.15f <= CheckDotX
            && element.y + 0.15f >= CheckDotY && element.y - 0.15f <= CheckDotY && !element.IsDivide) {

            it = elements.erase(it);

            if (element.Edge == 3) {
                newElement.x = element.x - 0.1f;
                newElement.y = element.y;
                newElement.IsDivide = true;
                newElement.Edge = 2;
                for (int i = 0; i < 3; ++i)
                    newElement.RGB[i] = element.RGB[i];
                newElement.IsDivideLeft = true;
                newElement.IsEat = false;

                Divide.push_back(newElement);

                newElement.x = element.x + 0.1f;
                newElement.y = element.y;
                newElement.IsDivide = true;
                newElement.Edge = 2;
                for (int i = 0; i < 3; ++i)
                    newElement.RGB[i] = element.RGB[i];
                newElement.IsDivideLeft = false;
                newElement.IsEat = false;

                Divide.push_back(newElement);
            }
            else if (element.Edge == 4) {
                newElement.x = element.x - 0.17f;
                newElement.y = element.y - 0.17f;
                newElement.IsDivide = true;
                newElement.Edge = 3;
                for (int i = 0; i < 3; ++i)
                    newElement.RGB[i] = element.RGB[i];
                newElement.IsDivideLeft = true;
                newElement.IsEat = false;

                Divide.push_back(newElement);

                newElement.x = element.x + 0.17f;
                newElement.y = element.y + 0.17f;
                newElement.IsDivide = true;
                newElement.Edge = 3;
                for (int i = 0; i < 3; ++i)
                    newElement.RGB[i] = element.RGB[i];
                newElement.IsDivideLeft = false;
                newElement.IsEat = false;

                Divide.push_back(newElement);
            }
        }
        else {
            ++it;
        }
    }
}

void basketCheck() {
    for (auto it = Divide.begin(); it != Divide.end(); ) {
        Elements& element = *it;

        if (element.x + 0.15f >= basket.x - 0.2f && element.x - 0.15f <= basket.x + 0.2f
            && element.y >= basket.y - 0.02f && element.y - 0.15f <= basket.y + 0.02f && element.Edge == 2) {
            element.IsEat = true;
            ++it;
        }
        else if (element.Edge == 3 && element.IsDivideLeft
            && element.x + 0.3f >= basket.x - 0.2f && element.x <= basket.x + 0.2f
            && element.y >= basket.y - 0.02f && element.y <= basket.y + 0.02f) {
            element.IsEat = true;
            ++it;
        }
        else
            ++it;
    }
}

void InitBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(2, vbo);
    glBindVertexArray(0);
}

void make_shaderProgram() {
    make_vertexShaders();
    make_fragmentShaders();
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgramID);
}

void make_vertexShaders() {
    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }
}

void make_fragmentShaders() {
    fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }
}

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