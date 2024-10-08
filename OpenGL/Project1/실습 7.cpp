#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <gl/glew.h>
#include <gl/freeglut.h>

struct Shape {
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	GLenum mode;
};

std::vector<Shape> shapes;
GLfloat currentColor[3] = { 1.0f, 0.0f, 0.0f };
GLenum currentMode = GL_POINTS;
int maxShapes = 10;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
void InitBuffer();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();

void createShape(GLenum mode, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& colors);
void deleteShapes();
void moveRandomShape(char direction);
void drawShapes();

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
GLchar *vertexSource, *fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example 7");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	drawShapes();

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
	if (key == 'p') {
		currentMode = GL_POINTS;
	}
	if (key == 'l') {
		currentMode = GL_LINES;
	}
	if (key == 't') {
		currentMode = GL_TRIANGLES;
	}
	if (key == 'r') {
		currentMode = GL_QUADS;
	}
	if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
		moveRandomShape(key);
	}
	if (key == 'c') {
		deleteShapes();
	}

	std::cout << "Current Mode: " << currentMode << std::endl;

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
		int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
		float normalizedX = (float)x / (windowWidth / 2) - 1.0f;
		float normalizedY = 1.0f - (float)y / (windowHeight / 2);

		std::cout << "Mouse clicked at: (" << x << ", " << y << ")" << std::endl;
		std::cout << "Normalized coords: (" << normalizedX << ", " << normalizedY << ")" << std::endl;
		std::cout << "Current mode: " << currentMode << std::endl;

		std::vector<GLfloat> vertices;
		std::vector<GLfloat> colors;

		GLfloat randomColor[3] = { (GLfloat)(rand() % 100) / 100.0f, (GLfloat)(rand() % 100) / 100.0f, (GLfloat)(rand() % 100) / 100.0f };

		if (currentMode == GL_POINTS) {
			glPointSize(10.0f);
			vertices = { normalizedX, normalizedY, 0.0f };
			colors = { randomColor[0], randomColor[1], randomColor[2] };
			createShape(currentMode, vertices, colors);
		}
		else if (currentMode == GL_LINES) {
			glLineWidth(5.0f);
			vertices = { normalizedX - 0.05f, normalizedY, 0.0f, normalizedX + 0.05f, normalizedY, 0.0f }; // Line of length 0.1
			colors = { randomColor[0], randomColor[1], randomColor[2], randomColor[0], randomColor[1], randomColor[2] };
			createShape(currentMode, vertices, colors);
		}
		else if (currentMode == GL_TRIANGLES) {
			vertices = { normalizedX - 0.1f, normalizedY - 0.1f, 0.0f,
				normalizedX + 0.1f, normalizedY - 0.1f, 0.0f,
				normalizedX, normalizedY + 0.1f, 0.0f }; // Triangle
			colors = { randomColor[0], randomColor[1], randomColor[2], randomColor[0], randomColor[1], randomColor[2], randomColor[0], randomColor[1], randomColor[2] };
			createShape(currentMode, vertices, colors);
		}
		else if (currentMode == GL_QUADS) {
			currentMode = GL_TRIANGLES;
			vertices = {
				// 첫 번째 삼각형: 왼쪽 아래, 오른쪽 아래, 오른쪽 위
				normalizedX - 0.1f, normalizedY - 0.1f, 0.0f,  // 왼쪽 아래
				normalizedX + 0.1f, normalizedY - 0.1f, 0.0f,  // 오른쪽 아래
				normalizedX + 0.1f, normalizedY + 0.1f, 0.0f,  // 오른쪽 위

				// 두 번째 삼각형: 왼쪽 아래, 오른쪽 위, 왼쪽 위
				normalizedX - 0.1f, normalizedY - 0.1f, 0.0f,  // 왼쪽 아래
				normalizedX + 0.1f, normalizedY + 0.1f, 0.0f,  // 오른쪽 위
				normalizedX - 0.1f, normalizedY + 0.1f, 0.0f   // 왼쪽 위
			};

			// 각 꼭짓점마다 동일한 색상 할당
			colors = {
				randomColor[0], randomColor[1], randomColor[2], // 첫 번째 삼각형의 첫 번째 꼭짓점
				randomColor[0], randomColor[1], randomColor[2], // 첫 번째 삼각형의 두 번째 꼭짓점
				randomColor[0], randomColor[1], randomColor[2], // 첫 번째 삼각형의 세 번째 꼭짓점

				randomColor[0], randomColor[1], randomColor[2], // 두 번째 삼각형의 첫 번째 꼭짓점
				randomColor[0], randomColor[1], randomColor[2], // 두 번째 삼각형의 두 번째 꼭짓점
				randomColor[0], randomColor[1], randomColor[2]  // 두 번째 삼각형의 세 번째 꼭짓점
			};

			createShape(currentMode, vertices, colors);
			currentMode = GL_QUADS;
		}
	}

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
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if(!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void createShape(GLenum mode, const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& colors) {
	if (shapes.size() < maxShapes) {
		Shape newShape;
		newShape.mode = mode;
		newShape.vertices = vertices;
		newShape.colors = colors;
		shapes.push_back(newShape);
	}
}

void deleteShapes() {
	shapes.clear();
}

void moveRandomShape(char direction) {
	if (shapes.empty()) return;

	int index = rand() % shapes.size();
	Shape& shape = shapes[index];

	float moveAmount = 0.05f;
	if (direction == 'w') {
		for (size_t i = 0; i < shape.vertices.size(); i += 3)
			shape.vertices[i + 1] += moveAmount;
	}
	else if (direction == 's') {
		for (size_t i = 0; i < shape.vertices.size(); i += 3)
			shape.vertices[i + 1] -= moveAmount;
	}
	else if (direction == 'a') {
		for (size_t i = 0; i < shape.vertices.size(); i += 3)
			shape.vertices[i] -= moveAmount;
	}
	else if (direction == 'd') {
		for (size_t i = 0; i < shape.vertices.size(); i += 3)
			shape.vertices[i] += moveAmount;
	}
}

void drawShapes() {
	glBindVertexArray(vao);

	for (const Shape& shape : shapes) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, shape.vertices.size() * sizeof(GLfloat), shape.vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, shape.colors.size() * sizeof(GLfloat), shape.colors.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);  // 색상 속성 설정
		glEnableVertexAttribArray(1);


		glDrawArrays(shape.mode, 0, shape.vertices.size() / 3);
	}

	glBindVertexArray(0);
}