#include "10.h"

#define PI 3.141592
#define R 0.001
#define POINT_DRAW 300
#define POINT_SPEED 6

char vertex[] = { "vertex.glsl" };
char fragment[] = { "fragment.glsl" };

typedef struct {
    GLfloat color[3];
    GLfloat cx, cy, r; // 중심 좌표, 반지름
    GLint degree, speed;
    GLint count;
    GLint spiral_count;
    bool isLine; // 선인지 점인지 구분
} SPIRAL;

GLuint shaderProgramID;
GLuint vao, vbo[2];

GLint Count = 0;
GLint number = 300;
GLint spiral_count = 1;

bool time_control = FALSE;

SPIRAL s;
std::vector<SPIRAL> spirals;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

void make_shaderProgram();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);

void random_color(GLfloat* color[3]);
void circle_spiral(GLfloat cx, GLfloat cy, GLfloat r, GLint degree, GLint speed, GLint count, bool isLine);

void draw_spiral(SPIRAL* spiral);
void addSpiral(GLfloat x, GLfloat y, bool isLine);
void changeBackgroundColor();

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Example 10");

    glewExperimental = GL_TRUE;
    glewInit();
    make_shaderProgram();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    glutMainLoop();
}

GLvoid drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT); // 배경 색상 클리어

    glUseProgram(shaderProgramID);

    for (int i = 0; i < s.spiral_count; i++) {
        circle_spiral(s.cx + (R * 100 * i), s.cy + (R * 210 * i), s.r, s.degree, s.speed, s.count, s.isLine);
        for (auto& spiral : spirals) draw_spiral(&spiral);
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) { glViewport(0, 0, w, h); }

void make_shaderProgram()
{
    make_VertexShaders(vertex);
    make_FragmentShaders(fragment);

    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgramID);
}

void Keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case '1': case '2': case '3': case '4': case '5':
        spiral_count = key - '0';  // 숫자 변환
        break;
    case 'p': // 점으로 그리기
        s.isLine = false;
        break;
    case 'l': // 선으로 그리기
        s.isLine = true;
        break;
    }
}

void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!time_control) {
            changeBackgroundColor();

            s.cx = conversion_x(x);
            s.cy = conversion_y(y);

            Count = 0;
            s.r = 0;
            s.speed = POINT_SPEED;
            s.count = 0;
            random_color_single(s.color);

            time_control = TRUE;

            s.spiral_count = spiral_count;
            glutTimerFunc(10, TimerFunction, 1);
        }
    }
}

void TimerFunction(int value) {
    glutPostRedisplay();
    Count++;
    s.count++;
    if (Count == POINT_DRAW) time_control = FALSE;
    if (time_control) glutTimerFunc(10, TimerFunction, 1);
}

void random_color(GLfloat* color[3]) {
    for (int i = 0; i < 3; i++) {
        color[i][0] = 1.0f;
        color[i][1] = 1.0f;
        color[i][2] = 1.0f;
    }
}

void circle_spiral(GLfloat cx, GLfloat cy, GLfloat r, GLint degree, GLint speed, GLint count, bool isLine) {
    GLfloat x, y;
    GLfloat prevX = cx, prevY = cy; // 이전 점의 위치를 저장하기 위한 초기값

    if (isLine) {
        glBegin(GL_LINES); // 선으로 그리기
    }
    else {
        glBegin(GL_POINTS); // 점으로 그리기
    }

    for (int i = 0; i < count; i++) {
        if (i < POINT_DRAW / 2) {
            x = cx + (r * cosf(degree * PI / 180));
            y = cy + (r * sinf(degree * PI / 180));
            r += R;
            degree = (degree + speed) % 360;
        }
        else {
            x = cx + (r * cosf(degree * PI / 180));
            y = cy + (r * sinf(degree * PI / 180));
            r -= R;
            degree = (degree - speed) % 360;
        }

        if (i == POINT_DRAW / 2) {
            cx -= r * 2;
            degree -= 180;
        }

        glPointSize(10); // 점 크기 설정
        glColor3f(1.0f, 1.0f, 1.0f); // 하얀색으로 설정
        glVertex2f(x, y); // 점 그리기

        // 이전 점과 연결된 선 그리기
        if (isLine && i > 0) {
            glVertex2f(prevX, prevY); // 이전 점의 위치로 선을 그리기
            glVertex2f(x, y); // 현재 점의 위치로 선을 그리기
        }

        // 현재 점의 위치 저장
        prevX = x;
        prevY = y;
    }

    glEnd();

    if (count == POINT_DRAW) addSpiral(cx + (R * 300), cy, isLine);
}

void addSpiral(GLfloat x, GLfloat y, bool isLine)
{
    SPIRAL new_s;
    new_s.cx = x;
    new_s.cy = y;
    new_s.r = 0;
    new_s.speed = POINT_SPEED;
    new_s.count = POINT_DRAW;
    new_s.isLine = isLine; // 선 또는 점으로 설정

    spirals.push_back(new_s);
}

void draw_spiral(SPIRAL* spiral) {
    GLfloat cx = spiral->cx;
    GLfloat cy = spiral->cy;
    GLfloat r = spiral->r;
    GLint degree = 0;
    GLint speed = POINT_SPEED;
    GLfloat x, y;
    GLfloat prevX = cx, prevY = cy; // 이전 점의 위치

    if (spiral->isLine) {
        glBegin(GL_LINES); // 선으로 그리기
    }
    else {
        glBegin(GL_POINTS); // 점으로 그리기
    }

    for (int i = 0; i < spiral->count; i++) {
        if (i < POINT_DRAW / 2) {
            x = cx + (r * cosf(degree * PI / 180));
            y = cy + (r * sinf(degree * PI / 180));
            r += R;
            degree = (degree + speed) % 360;
        }
        else {
            x = cx + (r * cosf(degree * PI / 180));
            y = cy + (r * sinf(degree * PI / 180));
            r -= R;
            degree = (degree - speed) % 360;
        }

        if (i == POINT_DRAW / 2) {
            cx -= r * 2;
            degree -= 180;
        }

        glPointSize(10); // 점 크기 설정
        glColor3f(1.0f, 1.0f, 1.0f); // 하얀색으로 설정
        glVertex2f(x, y); // 점 그리기

        // 이전 점과 연결된 선 그리기
        if (spiral->isLine && i > 0) {
            glVertex2f(prevX, prevY); // 이전 점의 위치로 선을 그리기
            glVertex2f(x, y); // 현재 점의 위치로 선을 그리기
        }

        // 현재 점의 위치 저장
        prevX = x;
        prevY = y;
    }

    glEnd();
}

void changeBackgroundColor()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(100, HEIGHT / 2 - 100);

    GLfloat RGB[3];

    random_color_single(RGB);

    glClearColor(RGB[0], RGB[1], RGB[2], 1.0f);
}
