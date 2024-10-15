#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define M_PI 3.14159265358979323846

std::random_device rd;
std::default_random_engine eng(rd());
std::uniform_real_distribution<float> posDist(-1.0f, 1.0f);
std::uniform_real_distribution<float> sizeDist(0.1f, 0.5f);
std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);

GLuint vao, vertexVBO, colorVBO;
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

struct Shape {
    std::vector<float> vertices;
    std::vector<float> colors;
};

// OpenGL function declarations
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

std::vector<Shape> shapes;

Shape createRandomLine();
Shape createRandomTriangle();
Shape createRandomRectangle();
Shape createRandomPentagon();
Shape createRandomPoint();

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shape Combination Example");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    InitBuffer();
    make_shaderProgram();

    // Create random shapes
    for (int i = 0; i < 3; i++) {
        shapes.push_back(createRandomLine());
        shapes.push_back(createRandomTriangle());
        shapes.push_back(createRandomRectangle());
        shapes.push_back(createRandomPentagon());
        shapes.push_back(createRandomPoint());
    }

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    glutMainLoop();
}

Shape createRandomLine() {
    Shape line;
    float x1 = posDist(eng);
    float y1 = posDist(eng);
    float x2 = posDist(eng);
    float y2 = posDist(eng);

    line.vertices = { x1, y1, x2, y2 };
    line.colors = { colorDist(eng), colorDist(eng), colorDist(eng),
                    colorDist(eng), colorDist(eng), colorDist(eng) };
    return line;
}

Shape createRandomTriangle() {
    Shape triangle;
    float size = sizeDist(eng);

    triangle.vertices = {
        posDist(eng), posDist(eng),
        posDist(eng), posDist(eng),
        posDist(eng), posDist(eng)
    };
    triangle.colors = {
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng)
    };
    return triangle;
}

Shape createRandomRectangle() {
    Shape rectangle;
    float width = sizeDist(eng);
    float height = sizeDist(eng);
    float x = posDist(eng);
    float y = posDist(eng);

    rectangle.vertices = {
        x, y,
        x + width, y,
        x + width, y - height,
        x, y - height
    };
    rectangle.colors = {
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng)
    };
    return rectangle;
}

Shape createRandomPentagon() {
    Shape pentagon;
    float size = sizeDist(eng);
    float x = posDist(eng);
    float y = posDist(eng);

    pentagon.vertices = {
        x, y,
        x + size * cos(2 * M_PI / 5 * 0), y + size * sin(2 * M_PI / 5 * 0),
        x + size * cos(2 * M_PI / 5 * 1), y + size * sin(2 * M_PI / 5 * 1),
        x + size * cos(2 * M_PI / 5 * 2), y + size * sin(2 * M_PI / 5 * 2),
        x + size * cos(2 * M_PI / 5 * 3), y + size * sin(2 * M_PI / 5 * 3)
    };
    pentagon.colors = {
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng),
        colorDist(eng), colorDist(eng), colorDist(eng)
    };
    return pentagon;
}

Shape createRandomPoint() {
    Shape point;
    point.vertices = { posDist(eng), posDist(eng) };
    point.colors = { colorDist(eng), colorDist(eng), colorDist(eng) };
    return point;
}

GLvoid drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& shape : shapes) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vertexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, shape.vertices.size() * sizeof(float), shape.vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, shape.colors.size() * sizeof(float), shape.colors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        if (shape.vertices.size() == 2) {
            glDrawArrays(GL_POINTS, 0, 1);
        }
        else if (shape.vertices.size() == 4) {
            glDrawArrays(GL_QUADS, 0, 4);
        }
        else if (shape.vertices.size() == 6) {
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        else if (shape.vertices.size() == 10) {
            glDrawArrays(GL_POLYGON, 0, 5);
        }
        else {
            glDrawArrays(GL_LINES, 0, 2);
        }

        glBindVertexArray(0);
        glDeleteBuffers(1, &vertexVBO);
        glDeleteBuffers(1, &colorVBO);
        glDeleteVertexArrays(1, &vao);
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
    glutPostRedisplay();
}

GLvoid Motion(int x, int y) {
    glutPostRedisplay();
}

void InitBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &colorVBO);
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