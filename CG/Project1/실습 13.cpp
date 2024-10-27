#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

std::random_device rd;
std::default_random_engine eng(rd());

int RandomInt(int f1, int f2) {
    std::uniform_int_distribution<GLint> distr(f1, f2);
    return distr(eng);
}

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

void drawXYline();
void makeCube1();
void makeCube2();
void makeCube3();
void makeCube4();
void makeCube5();
void makeCube6();
void makeTetra1();
void makeTetra2();
void makeTetra3();
void makeTetra4();

void Allfalse();

bool IsCube1 = false;
bool IsCube2 = false;
bool IsCube3 = false;
bool IsCube4 = false;
bool IsCube5 = false;
bool IsCube6 = false;
bool IsTetra1 = false;
bool IsTetra2 = false;
bool IsTetra3 = false;
bool IsTetra4 = false;

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);

    glutCreateWindow("Example 13");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    make_shaderProgram();
    InitBuffer();
    
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    drawXYline();
    
    if (IsCube2)
        makeCube2(); // µÞ¸é
    if (IsCube4)
        makeCube4(); // ¾Æ·§¸é
    if (IsCube5)
        makeCube5(); // ¿ÞÂÊ¸é
    if (IsCube6)
        makeCube6(); // ¿À¸¥ÂÊ¸é
    if (IsCube3)
        makeCube3(); // À­¸é
    if (IsCube1)
        makeCube1(); // ¾Õ¸é
    if (IsTetra1)
        makeTetra1(); // ¾Æ·§¸é
    if (IsTetra2)
        makeTetra2(); // µÞ¸é
    if (IsTetra3)
        makeTetra3(); // ¿À¸¥ÂÊ¸é
    if (IsTetra4)
        makeTetra4(); // ¾Õ¸é

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        Allfalse();
        IsCube1 = true;
        break;
    case '2':
        Allfalse();
        IsCube2 = true;
        break;
    case '3':
        Allfalse();
        IsCube3 = true;
        break;
    case '4':
        Allfalse();
        IsCube4 = true;
        break;
    case '5':
        Allfalse();
        IsCube5 = true;
        break;
    case '6':
        Allfalse();
        IsCube6 = true;
        break;
    case '7':
        Allfalse();
        IsTetra1 = true;
        break;
    case '8':
        Allfalse();
        IsTetra2 = true;
        break;
    case '9':
        Allfalse();
        IsTetra3 = true;
        break;
    case '0':
        Allfalse();
        IsTetra4 = true;
        break;
    case 'c': {
        int a = 0, b = 0;

        while (a == b) {
            a = RandomInt(1, 6);
            b = RandomInt(1, 6);
        }

        Allfalse();
        if (a == 1)
            IsCube1 = true;
        else if (a == 2)
            IsCube2 = true;
        else if (a == 3)
            IsCube3 = true;
        else if (a == 4)
            IsCube4 = true;
        else if (a == 5)
            IsCube5 = true;
        else if (a == 6)
            IsCube6 = true;

        if (b == 1)
            IsCube1 = true;
        else if (b == 2)
            IsCube2 = true;
        else if (b == 3)
            IsCube3 = true;
        else if (b == 4)
            IsCube4 = true;
        else if (b == 5)
            IsCube5 = true;
        else if (b == 6)
            IsCube6 = true;

        break;
    }
    case 't': {
        int a = 0, b = 0;

        while (a == b) {
            a = RandomInt(1, 4);
            b = RandomInt(1, 4);
        }

        Allfalse();
        if (a == 1)
            IsTetra1 = true;
        else if (a == 2)
            IsTetra2 = true;
        else if (a == 3)
            IsTetra3 = true;
        else if (a == 4)
            IsTetra4 = true;

        if (b == 1)
            IsTetra1 = true;
        else if (b == 2)
            IsTetra2 = true;
        else if (b == 3)
            IsTetra3 = true;
        else if (b == 4)
            IsTetra4 = true;

        break;
    }
    }

    glutPostRedisplay();
}

void drawXYline() {
    glm::mat4 None = glm::mat4(1.f);
    None = glm::rotate(None, glm::radians(0.f), glm::vec3(1.0, 1.0, 1.0));

    std::vector<GLfloat> vertices = {
        0.f, 1.f, 1.f,
        0.f, -1.f, 1.f,

        1.f, 0.f, 1.f,
        -1.f, 0.f, 1.f
    };

    std::vector<GLfloat> colors = {
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,

        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f
    };
    
    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(None));

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 0, 4);
    
    glBindVertexArray(0);
}

void makeCube1() { // ¾Õ¸é
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        -0.3f, -0.3f, 0.3f,
        0.3f, -0.3f, 0.3f,
        0.3f, 0.3f, 0.3f,
        -0.3f, 0.3f, 0.3f
    };

    std::vector<GLfloat> colors = {
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f
    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeCube2() { // µÞ¸é
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        0.3f, -0.3f, -0.3f,
        -0.3f, -0.3f, -0.3f,
        -0.3f, 0.3f, -0.3f,
        0.3f, 0.3f, -0.3f
    };

    std::vector<GLfloat> colors = {
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f
    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeCube3() { // À­¸é
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        -0.3f, 0.3f, 0.3f,
        0.3f, 0.3f, 0.3f,
        0.3f, 0.3f, -0.3f,
        -0.3f, 0.3f, -0.3f
    };

    std::vector<GLfloat> colors = {
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f
    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeCube4() { // ¾Æ·§¸é
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        -0.3f, -0.3f, -0.3f,
        0.3f, -0.3f, -0.3f,
        0.3f, -0.3f, 0.3f,
        -0.3f, -0.3f, 0.3f
    };

    std::vector<GLfloat> colors = {
        0.f, 0.5f, 0.5f,
        0.f, 0.5f, 0.5f,
        0.f, 0.5f, 0.5f,
        0.f, 0.5f, 0.5f
    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeCube5() { // ¿ÞÂÊ¸é
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        -0.3f, -0.3f, -0.3f,
        -0.3f, -0.3f, 0.3f,
        -0.3f, 0.3f, 0.3f,
        -0.3f, 0.3f, -0.3f
    };

    std::vector<GLfloat> colors = {
        0.5f, 0.5f, 0.f,
        0.5f, 0.5f, 0.f,
        0.5f, 0.5f, 0.f,
        0.5f, 0.5f, 0.f
    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeCube6() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        0.3f, -0.3f, 0.3f,
        0.3f, -0.3f, -0.3f,
        0.3f, 0.3f, -0.3f,
        0.3f, 0.3f, 0.3f
    };

    std::vector<GLfloat> colors = {
        0.5f, 0.f, 0.5f,
        0.5f, 0.f, 0.5f,
        0.5f, 0.f, 0.5f,
        0.5f, 0.f, 0.5f
    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeTetra1() { // ¾Æ·§¸é
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        -0.3f, -0.1f, 0.f,
        0.1f, -0.1f, -0.2f,
        0.1f, -0.1f, 0.2f,
    };

    std::vector<GLfloat> colors = {
        0.5f, 1.f, 0.f,
        0.5f, 1.f, 0.f,
        0.5f, 1.f, 0.f,
    };

    std::vector<GLuint> index = {
        0, 1, 2
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeTetra2() { // µÞ¸é
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        0.1f, -0.1f, -0.2f,
        -0.3f, -0.1f, 0.f,
        0.f, 0.3f, 0.f
    };

    std::vector<GLfloat> colors = {
        0.5f, 0.f, 0.5f,
        0.5f, 0.f, 0.5f,
        0.5f, 0.f, 0.5f,
    };

    std::vector<GLuint> index = {
        0, 1, 2
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeTetra3() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        0.1f, -0.1f, 0.2f,
        0.1f, -0.1f, -0.2f,
        0.f, 0.3f, 0.f
    };

    std::vector<GLfloat> colors = {
        0.f, 0.5f, 1.f,
        0.f, 0.5f, 1.f,
        0.f, 0.5f, 1.f,
    };

    std::vector<GLuint> index = {
        0, 1, 2
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void makeTetra4() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Rotate = glm::mat4(1.f);
    RotateX = glm::rotate(RotateX, glm::radians(10.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(10.f), glm::vec3(0.0, 1.0, 0.0));

    Rotate = RotateY * RotateX;

    std::vector<GLfloat> vertices = {
        -0.3f, -0.1f, 0.f,
        0.1f, -0.1f, 0.2f,
        0.f, 0.3f, 0.f
    };

    std::vector<GLfloat> colors = {
        1.f, 0.2f, 0.f,
        1.f, 0.2f, 0.f,
        1.f, 0.2f, 0.f,
    };

    std::vector<GLuint> index = {
        0, 1, 2
    };

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));

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

void Allfalse() {
    IsCube1 = false;
    IsCube2 = false;
    IsCube3 = false;
    IsCube4 = false;
    IsCube5 = false;
    IsCube6 = false;
    IsTetra1 = false;
    IsTetra2 = false;
    IsTetra3 = false;
    IsTetra4 = false;
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
        std::cerr << "ERROR: vertex shader ÄÄÆÄÀÏ ½ÇÆÐ\n" << errorLog << std::endl;
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
        std::cerr << "ERROR: fragment shader ÄÄÆÄÀÏ ½ÇÆÐ\n" << errorLog << std::endl;
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