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

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);

void drawXYZline();
void drawCubeBack();
void drawCubeUnder();
void drawCubeRight();
void drawCubeLeft();
void drawCubeFront();
void drawCubeUp();

void drawPyramidBottom();
void drawPyramidBack();
void drawPyramidRight();
void drawPyramidLeft();
void drawPyramidFront();

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool DepthTest = true;
bool IsAllMove = false;

bool IsCube = true;
bool IsCubeBackMove = false;
bool IsCubeSideMove = false;
bool IsCubeFrontMove = false;
bool IsCubeUpMove = false;

bool IsPyramidAllMove = false;

float Allangle = 0.0f;

float PyramidAllangle = 0.0f;

float Backangle = 0.0f;
float BackScale = 1.0f;
float SideUp = 0.0f;
float Frontangle = 0.0f;
float Upangle = 0.0f;

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);

    glutCreateWindow("Example 17");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(16, Timer, 1);

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glEnable(GL_DEPTH_TEST);
    drawXYZline();

    if(DepthTest)
        glEnable(GL_DEPTH_TEST);
    else if(!DepthTest)
        glDisable(GL_DEPTH_TEST);

    if (IsCube) {
        drawCubeBack();
        drawCubeUnder();
        drawCubeRight();
        drawCubeLeft();
        drawCubeFront();
        drawCubeUp();
    }
    else if (!IsCube) {
        drawPyramidBottom();
        drawPyramidBack();
        drawPyramidRight();
        drawPyramidLeft();
        drawPyramidFront();
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'y':
        IsAllMove = !IsAllMove;
        break;
    case 'h':
        DepthTest = !DepthTest;
        break;
    case 'p':
        break;
    case 'b':
        IsCube = true;
        IsCubeBackMove = !IsCubeBackMove;
        break;
    case 's':
        IsCube = true;
        IsCubeSideMove = !IsCubeSideMove;
        break;
    case 'f':
        IsCube = true;
        IsCubeFrontMove = !IsCubeFrontMove;
        break;
    case 't':
        IsCube = true;
        IsCubeUpMove = !IsCubeUpMove;
        break;
    case 'o':
        IsCube = false;
        IsPyramidAllMove = !IsPyramidAllMove;
        break;
    case 'r':
        IsCube = false;
        break;
    }

    glutPostRedisplay();
}

GLvoid Timer(int value) {
    if (IsAllMove)
        Allangle += 0.5f;
    if (IsCube) {
        if (IsCubeBackMove) {
            Backangle += 0.03f;
            BackScale = cos(Backangle) / 2 + 0.5f;
        }

        if (IsCubeSideMove) {
            if (SideUp < 0.5f)
                SideUp += 0.01f;
        }
        else if (!IsCubeSideMove) {
            if (SideUp > 0.0f)
                SideUp -= 0.01f;
        }

        if (IsCubeFrontMove) {
            if (Frontangle < 90.0f)
                Frontangle += 0.4f;
        }
        else if (!IsCubeFrontMove) {
            if (Frontangle > 0.f)
                Frontangle -= 0.4f;
        }

        if (IsCubeUpMove) {
            Upangle += 0.5f;
        }
    }
    else if (!IsCube) {
        if (IsPyramidAllMove) {
            if (PyramidAllangle < 242.0f)
                PyramidAllangle += 0.5f;
        }
        else if (!IsPyramidAllMove) {
            if (PyramidAllangle > 0.0f)
                PyramidAllangle -= 0.5f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 1);
}

void drawXYZline() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY;

    std::vector<GLfloat> vertices = {
        -1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,

        0.f, -1.f, 0.f,
        0.f, 1.f, 0.f,

        0.f, 0.f, -1.f,
        0.f, 0.f, 1.f
    };

    std::vector<GLfloat> colors = {
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
    };

    std::vector<GLuint> index = {
        0, 1,
        2, 3,
        4, 5
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
    unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");

    glm::mat4 mTransform = glm::mat4(1.0f);
    mTransform = Conversion;
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &mTransform[0][0]);

    glm::mat4 vTransform = glm::mat4(1.0f);
    vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &vTransform[0][0]);

    glm::mat4 pTransform = glm::mat4(1.0f);
    pTransform = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &pTransform[0][0]);

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

void drawCubeBack() {
    glm::mat4 Scale = glm::mat4(1.f);
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    Scale = glm::scale(Scale, glm::vec3(BackScale, BackScale, 1.0f));
    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));
    Translate = glm::translate(Translate, glm::vec3(0.f, 0.f, -0.3f));
    
    Conversion = RotateX * RotateY * AllRotate * Translate * Scale;

    std::vector<GLfloat> vertices = {
        0.3f, -0.3f, 0.f,
        -0.3f, -0.3f, 0.f,
        -0.3f, 0.3f, 0.f,
        0.3f, 0.3f, 0.f
    };

    std::vector<GLfloat> colors = {
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 1.0f,
        1.0f, 1.0f, 1.0f
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

void drawCubeUnder() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));
    Translate = glm::translate(Translate, glm::vec3(0.f, -0.3f, 0.f));

    Conversion =  RotateX * RotateY * AllRotate * Translate;

    std::vector<GLfloat> vertices = {
        -0.3f, 0.f, -0.3f,
        0.3f, 0.f, -0.3f,
        0.3f, 0.f, 0.3f,
        -0.3f, 0.f, 0.3f
    };

    std::vector<GLfloat> colors = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f
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

void drawCubeRight() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 MoveTranslate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    Translate = glm::translate(Translate, glm::vec3(0.3f, 0.f, 0.f));
    MoveTranslate = glm::translate(MoveTranslate, glm::vec3(0.f, SideUp, 0.f));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY * AllRotate * MoveTranslate * Translate;

    std::vector<GLfloat> vertices = {
        0.f, -0.3f, 0.3f,
        0.f, -0.3f, -0.3f,
        0.f, 0.3f, -0.3f,
        0.f, 0.3f, 0.3f
    };

    std::vector<GLfloat> colors = {
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f
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

void drawCubeLeft() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 MoveTranslate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    Translate = glm::translate(Translate, glm::vec3(-0.3f, 0.f, 0.f));
    MoveTranslate = glm::translate(MoveTranslate, glm::vec3(0.f, SideUp, 0.f));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY * AllRotate * MoveTranslate * Translate;

    std::vector<GLfloat> vertices = {
        0.f, 0.3f, 0.3f,
        0.f, 0.3f, -0.3f,
        0.f, -0.3f, -0.3f,
        0.f, -0.3f, 0.3f
    };

    std::vector<GLfloat> colors = {
        0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f
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

void drawCubeFront() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 UpTranslate = glm::mat4(1.f);
    glm::mat4 DownTranslate = glm::mat4(1.f);
    glm::mat4 RotateUnder = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    Translate = glm::translate(Translate, glm::vec3(0.f, 0.f, 0.3f));
    RotateUnder = glm::rotate(RotateUnder, glm::radians(Frontangle), glm::vec3(1.0, 0.0, 0.0));
    UpTranslate = glm::translate(UpTranslate, glm::vec3(0.f, 0.3f, 0.f));
    DownTranslate = glm::translate(DownTranslate, glm::vec3(0.f, -0.3f, 0.f));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY * AllRotate * Translate * DownTranslate * RotateUnder * UpTranslate;

    std::vector<GLfloat> vertices = {
        -0.3f, -0.3f, 0.f,
        0.3f, -0.3f, 0.f,
        0.3f, 0.3f, 0.f,
        -0.3f, 0.3f, 0.f
    };

    std::vector<GLfloat> colors = {
        1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f,
        0.0f, 1.0f, 0.0f
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

void drawCubeUp() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 UpRotate = glm::mat4(1.f);
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    Translate = glm::translate(Translate, glm::vec3(0.f, 0.3f, 0.f));
    UpRotate = glm::rotate(UpRotate, glm::radians(Upangle), glm::vec3(1.0, 0.0, 0.0));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY * AllRotate * Translate * UpRotate;

    std::vector<GLfloat> vertices = {
        0.3f, 0.f, -0.3f,
        -0.3f, 0.f, -0.3f,
        -0.3f, 0.f, 0.3f,
        0.3f, 0.f, 0.3f
    };

    std::vector<GLfloat> colors = {
        1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f
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

void drawPyramidBottom() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY * AllRotate;

    std::vector<GLfloat> vertices = {
        -0.3f, 0.f, 0.3f,
        -0.3f, 0.f, -0.3f,
        0.3f, 0.f, -0.3f,
        0.3f, 0.f, 0.3f
    };

    std::vector<GLfloat> colors = {
        1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 1.0f
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

void drawPyramidBack() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 PyramidAllRotate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 MoveTranslate = glm::mat4(1.f);
    glm::mat4 BackTranslate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    PyramidAllRotate = glm::rotate(PyramidAllRotate, glm::radians(PyramidAllangle), glm::vec3(-1.0, 0.0, 0.0));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));
    MoveTranslate = glm::translate(MoveTranslate, glm::vec3(0.0f, 0.0f, 0.3f));
    BackTranslate = glm::translate(BackTranslate, glm::vec3(0.0f, 0.0f, -0.3f));

    Conversion = RotateX * RotateY * AllRotate * BackTranslate * PyramidAllRotate * MoveTranslate;

    std::vector<GLfloat> vertices = {
        0.3f, 0.f, -0.3f,
        -0.3f, 0.f, -0.3f,
        0.0f, 0.5f, 0.0f
    };

    std::vector<GLfloat> colors = {
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
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

void drawPyramidRight() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 PyramidAllRotate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 MoveTranslate = glm::mat4(1.f);
    glm::mat4 BackTranslate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    PyramidAllRotate = glm::rotate(PyramidAllRotate, glm::radians(PyramidAllangle), glm::vec3(0.0, 0.0, -1.0));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));
    MoveTranslate = glm::translate(MoveTranslate, glm::vec3(-0.3f, 0.0f, 0.0f));
    BackTranslate = glm::translate(BackTranslate, glm::vec3(0.3f, 0.0f, 0.0f));

    Conversion = RotateX * RotateY * AllRotate * BackTranslate * PyramidAllRotate * MoveTranslate;

    std::vector<GLfloat> vertices = {
        0.3f, 0.f, 0.3f,
        0.3f, 0.f, -0.3f,
        0.0f, 0.5f, 0.0f
    };

    std::vector<GLfloat> colors = {
        0.0f, 0.5f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
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

void drawPyramidLeft() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 PyramidAllRotate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 MoveTranslate = glm::mat4(1.f);
    glm::mat4 BackTranslate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    PyramidAllRotate = glm::rotate(PyramidAllRotate, glm::radians(PyramidAllangle), glm::vec3(0.0, 0.0, 1.0));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));
    MoveTranslate = glm::translate(MoveTranslate, glm::vec3(0.3f, 0.0f, 0.0f));
    BackTranslate = glm::translate(BackTranslate, glm::vec3(-0.3f, 0.0f, 0.0f));

    Conversion = RotateX * RotateY * AllRotate * BackTranslate * PyramidAllRotate * MoveTranslate;

    std::vector<GLfloat> vertices = {
        -0.3f, 0.f, -0.3f,
        -0.3f, 0.f, 0.3f,
        0.0f, 0.5f, 0.0f
    };

    std::vector<GLfloat> colors = {
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
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

void drawPyramidFront() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 PyramidAllRotate = glm::mat4(1.f);
    glm::mat4 AllRotate = glm::mat4(1.f);
    glm::mat4 MoveTranslate = glm::mat4(1.f);
    glm::mat4 BackTranslate = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(30.f), glm::vec3(0.0, 1.0, 0.0));
    PyramidAllRotate = glm::rotate(PyramidAllRotate, glm::radians(PyramidAllangle), glm::vec3(1.0, 0.0, 0.0));
    AllRotate = glm::rotate(AllRotate, glm::radians(Allangle), glm::vec3(0.0, 1.0, 0.0));
    MoveTranslate = glm::translate(MoveTranslate, glm::vec3(0.0f, 0.0f, -0.3f));
    BackTranslate = glm::translate(BackTranslate, glm::vec3(0.0f, 0.0f, 0.3f));

    Conversion = RotateX * RotateY * AllRotate * BackTranslate * PyramidAllRotate * MoveTranslate;

    std::vector<GLfloat> vertices = {
        -0.3f, 0.f, 0.3f,
        0.3f, 0.f, 0.3f,
        0.0f, 0.5f, 0.0f
    };

    std::vector<GLfloat> colors = {
        1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 1.0f,
        1.0f, 0.0f, 0.0f,
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