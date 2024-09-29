#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

bool TimerActive = false;

float RandomRGB() {
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<float> distr(0.0f, 1.0f);  // �յ� ����

	return distr(eng);
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);

	
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to Initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialize\n";

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid drawScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void TimerFunction(int value) {
	if (TimerActive) {
		glClearColor(RandomRGB(), RandomRGB(), RandomRGB(), 1.0f);
		glutPostRedisplay();
		glutTimerFunc(2000, TimerFunction, 1);
	}
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 'm':
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		break;
	case 'y':
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 'a':
		glClearColor(RandomRGB(), RandomRGB(), RandomRGB(), 1.0f);
		break;
	case 'w':
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 'k':
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 't':
		glutTimerFunc(2000, TimerFunction, 1);
		TimerActive = true;
		break;
	case 's':
		TimerActive = false;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	
	glutPostRedisplay();
}