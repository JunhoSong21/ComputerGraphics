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

struct StandardPoint {
    float x;
    float z;
};
StandardPoint point;

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint shaderProgramID;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid Timer(int value);

void drawXYZline();
void drawCube();
void drawSphere();
void drawSpiral();
void allFalse();

void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

float angle = 0.f;
float halfLine = 0.f;
GLUquadricObj* qobj;
int RevolveY = 0;

float Xmove = 0.f;
float Ymove = 0.f;
float Zmove = 0.f;
float NoneMscaleV = 1.f;
float MscaleV = 1.f;

float Xani = 0.f;
float Yani = 0.f;
float Zani = 0.f;

bool KeyboardControl = true;
bool Animation1 = false;
bool Animation2 = false;
bool Animation3 = false;
bool Animation4 = false;
bool Animation5 = false;

float AniRadiusIncrement = 0.0015f; // 각도에 따라 반지름이 증가하는 정도
float AniCurrentRadius = 0.0f;   // 초기 반지름
int AniNumPoints = 700;         // 스파이럴의 포인트 수
float AniAngleIncrement = 0.1f;  // 각도 증가량 (라디안)
int AniI = 0;

GLvoid main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);

    glutCreateWindow("Example 16");

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to Initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialize\n";

    point.x = 0.7f * cos(angle);
    point.z = 0.7f * sin(angle);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeyboard);
    glutTimerFunc(16, Timer, 1);

    glutMainLoop();
}

GLvoid drawScene() {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glDisable(GL_DEPTH_TEST);
    drawXYZline();

    drawCube();   
    drawSphere();
    
    drawSpiral();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        allFalse();
        Animation1 = true;
        break;
    case '2':
        allFalse();
        Animation2 = true;
        break;
    case '3':
        allFalse();
        Animation3 = true;
        break;
    case '4':
        allFalse();
        Animation4 = true;
        break;
    case '5':
        allFalse();
        Animation5 = true;
        break;
    case 'q':
        allFalse();
        KeyboardControl = true;
        Xmove += 0.1f;
        break;
    case 'Q':
        allFalse();
        KeyboardControl = true;
        Xmove -= 0.1f;
        break;
    case 'w':
        allFalse();
        KeyboardControl = true;
        Ymove += 0.1f;
        break;
    case 'W':
        allFalse();
        KeyboardControl = true;
        Ymove -= 0.1f;
        break;
    case 'e':
        allFalse();
        KeyboardControl = true;
        Zmove += 0.1f;
        break;
    case 'E':
        allFalse();
        KeyboardControl = true;
        Zmove -= 0.1f;
        break;
    case 'p':
        allFalse();
        KeyboardControl = true;
        NoneMscaleV += 0.01f;
        break;
    case 'P':
        allFalse();
        KeyboardControl = true;
        NoneMscaleV -= 0.01f;
        break;
    case 'o':
        allFalse();
        KeyboardControl = true;
        MscaleV += 0.01f;
        break;
    case 'O':
        allFalse();
        KeyboardControl = true;
        MscaleV -= 0.01f;
        break;
    case 's':
        angle = 0.f;
        RevolveY = 0;
        break;
    }

    glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y) {
    allFalse();
    KeyboardControl = true;

    switch (key) {
    case GLUT_KEY_LEFT:
        Xmove -= 0.1f;
        break;
    case GLUT_KEY_RIGHT:
        Xmove += 0.1f;
        break;
    case GLUT_KEY_UP:
        Zmove -= 0.1f;
        break;
    case GLUT_KEY_DOWN:
        Zmove += 0.1f;
        break;
    }

    glutPostRedisplay();
}

GLvoid Timer(int value) {
    if (Animation1) {
        angle = AniI * AniAngleIncrement;
        AniCurrentRadius += AniRadiusIncrement;

        Xani = AniCurrentRadius * cos(angle);
        Zani = AniCurrentRadius * sin(angle);
        AniI += 1;
        if (AniI >= 700) {
            AniI = 0;
            AniCurrentRadius = 0.0f;
            angle = 0.f;
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
    RotateY = glm::rotate(RotateY, glm::radians(-30.f), glm::vec3(0.0, 1.0, 0.0));

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
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
    };

    std::vector<GLuint> index = {
        0, 1,
        2, 3,
        4, 5
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

void drawCube() {
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 AfterScale = glm::mat4(1.f);
    glm::mat4 BeforeScale = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    Translate = glm::translate(Translate, glm::vec3(point.x, 0.f, point.z));
    Translate = glm::translate(Translate, glm::vec3(Xmove, Ymove, Zmove));
    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(-30.f), glm::vec3(0.0, 1.0, 0.0));
    AfterScale = glm::scale(AfterScale, glm::vec3(MscaleV, MscaleV, MscaleV));
    BeforeScale = glm::scale(BeforeScale, glm::vec3(NoneMscaleV, NoneMscaleV, NoneMscaleV));

    Conversion = AfterScale * RotateX * RotateY * Translate * BeforeScale;

    std::vector<GLfloat> vertices = {
        -0.2f, -0.2f, 0.2f,
        -0.2f, -0.2f, -0.2f,
        0.2f, -0.2f, -0.2f,
        0.2f, -0.2f, 0.2f,

        -0.2f, 0.2f, 0.2f,
        -0.2f, 0.2f, -0.2f,
        0.2f, 0.2f, -0.2f,
        0.2f, 0.2f, 0.2f
    };

    std::vector<GLfloat> colors = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, 1.f, 0.f,

        1.f, 0.f, 1.f,
        0.f, 1.f, 1.f,
        1.f, 0.5f, 0.5f,
        0.f, 0.5f, 0.5f
    };

    std::vector<GLuint> index = {
        0, 1, 2, 2, 3, 0,
        1, 0, 4, 4, 5, 1,
        2, 1, 5, 5, 6, 2,
        0, 3, 7, 7, 4, 0,
        3, 2, 6, 6, 7, 3,
        4, 7, 6, 6, 5, 4
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

void drawSphere() {
    glm::mat4 Translate = glm::mat4(1.f);
    glm::mat4 KeyTranslate = glm::mat4(1.f);
    glm::mat4 AniTranslate = glm::mat4(1.f);
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 AfterScale = glm::mat4(1.f);
    glm::mat4 BeforeScale = glm::mat4(1.f);
    glm::mat4 AniBeforeScale = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    Translate = glm::translate(Translate, glm::vec3(-point.x, 0.f, -point.z));
    KeyTranslate = glm::translate(KeyTranslate, glm::vec3(Xmove, Ymove, Zmove));
    AniTranslate = glm::translate(AniTranslate, glm::vec3(Xani, Yani, Zani));
    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(-30.f), glm::vec3(0.0, 1.0, 0.0));
    AfterScale = glm::scale(AfterScale, glm::vec3(MscaleV, MscaleV, MscaleV));
    BeforeScale = glm::scale(BeforeScale, glm::vec3(NoneMscaleV, NoneMscaleV, NoneMscaleV));
    AniBeforeScale = glm::scale(AniBeforeScale, glm::vec3(0.5f, 0.5f, 0.5f));

    if (KeyboardControl)
        Conversion = AfterScale * RotateX * RotateY * KeyTranslate * Translate * BeforeScale;
    else
        Conversion = AfterScale * RotateX * RotateY * AniTranslate * AniBeforeScale;

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_LINE);
    gluSphere(qobj, 0.3, 20, 20);
}

void drawSpiral() {
    glm::mat4 RotateX = glm::mat4(1.f);
    glm::mat4 RotateY = glm::mat4(1.f);
    glm::mat4 Conversion = glm::mat4(1.f);

    RotateX = glm::rotate(RotateX, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));
    RotateY = glm::rotate(RotateY, glm::radians(-30.f), glm::vec3(0.0, 1.0, 0.0));

    Conversion = RotateX * RotateY;

    unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Conversion));

    float radiusIncrement = 0.0015f; // 각도에 따라 반지름이 증가하는 정도
    float currentRadius = 0.0f;   // 초기 반지름
    int numPoints = 700;         // 스파이럴의 포인트 수
    float angleIncrement = 0.1f;  // 각도 증가량 (라디안)
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float angle = 0.f;

    glBegin(GL_LINE_STRIP); // 선으로 연결된 점을 그리기 위해 사용
    for (int i = 0; i < numPoints; ++i) {
        angle = i * angleIncrement; // 각도 계산
        currentRadius += radiusIncrement; // 반지름 증가

        x = currentRadius * cos(angle);
        z = currentRadius * sin(angle);

        glVertex3f(x, y, z); // 스파이럴의 점 추가
    }
    glEnd();
}

void allFalse() {
    KeyboardControl = false;
    Animation1 = false;
    Animation2 = false;
    Animation3 = false;
    Animation4 = false;
    Animation5 = false;
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