#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);

float Rec1[3] = { 1.0f, 0.f, 1.0f };
float Rec2[3] = { 1.0f, 1.0f, 0.f };
float Rec3[3] = { 0.f, 1.0f, 0.f };
float Rec4[3] = { 0.f, 0.1f, 1.0f };
float RecIn1[3] = { 0.5f, 0.5f, 0.5f };
float RecIn1xy[4] = { -0.75f, 0.75f, -0.25f, 0.25f };
float RecIn2[3] = { 1.0f, 0.25f, 0.75f };
float RecIn2xy[4] = { 0.25f, 0.75f, 0.75f, 0.25f };
float RecIn3[3] = { 0.f, 0.75f, 0.25f };
float RecIn3xy[4] = { -0.75f, -0.25f, -0.25f, -0.75f };
float RecIn4[3] = { 0.5f, 0.f, 1.0f };
float RecIn4xy[4] = { 0.25f, -0.25f, 0.75f, -0.75f };

float RandomRGB() {
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<float> distr(0.0f, 1.0f);

	return distr(eng);
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);


	glutCreateWindow("Example2");

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to Initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialize\n";

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid drawScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(Rec1[0], Rec1[1], Rec1[2]);
	glRectf(-1.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(Rec2[0], Rec2[1], Rec2[2]);
	glRectf(0.f, 0.f, 1.0f, 1.0f);
	glColor3f(Rec3[0], Rec3[1], Rec3[2]);
	glRectf(-1.0f, 0.f, 0.f, -1.0f);
	glColor3f(Rec4[0], Rec4[1], Rec4[2]);
	glRectf(0.f, -1.0f, 1.0f, 0.0f);

	glColor3f(RecIn1[0], RecIn1[1], RecIn1[2]);
	glRectf(RecIn1xy[0], RecIn1xy[1], RecIn1xy[2], RecIn1xy[3]);
	glColor3f(RecIn2[0], RecIn2[1], RecIn2[2]);
	glRectf(RecIn2xy[0], RecIn2xy[1], RecIn2xy[2], RecIn2xy[3]);
	glColor3f(RecIn3[0], RecIn3[1], RecIn3[2]);
	glRectf(RecIn3xy[0], RecIn3xy[1], RecIn3xy[2], RecIn3xy[3]);
	glColor3f(RecIn4[0], RecIn4[1], RecIn4[2]);
	glRectf(RecIn4xy[0], RecIn4xy[1], RecIn4xy[2], RecIn4xy[3]);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
	float fx = (float)(x - 400) / 400;
	float fy = -(float)(y - 300) / 300;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (x < 400 && y < 300) {
			if (fx > RecIn1xy[0] && fx < RecIn1xy[2] && fy < RecIn1xy[1] && fy > RecIn1xy[3])
				for (int i = 0; i < 3; ++i)
					RecIn1[i] = RandomRGB();
			else
				for (int i = 0; i < 3; ++i)
					Rec1[i] = RandomRGB();
		}
		else if (x >= 400 && y < 300) {
			if (fx > RecIn2xy[0] && fx < RecIn2xy[2] && fy < RecIn2xy[1] && fy > RecIn2xy[3])
				for (int i = 0; i < 3; ++i)
					RecIn2[i] = RandomRGB();
			else
				for (int i = 0; i < 3; ++i)
					Rec2[i] = RandomRGB();
		}
		else if (x < 400 && y >= 300) {
			if (fx > RecIn3xy[0] && fx < RecIn3xy[2] && fy < RecIn3xy[1] && fy > RecIn3xy[3])
				for (int i = 0; i < 3; ++i)
					RecIn3[i] = RandomRGB();
			else
				for (int i = 0; i < 3; ++i)
					Rec3[i] = RandomRGB();
		}
		else if (x >= 400 && y >= 300) {
			if (fx > RecIn4xy[0] && fx < RecIn4xy[2] && fy < RecIn4xy[1] && fy > RecIn4xy[3])
				for (int i = 0; i < 3; ++i)
					RecIn4[i] = RandomRGB();
			else
				for (int i = 0; i < 3; ++i)
					Rec4[i] = RandomRGB();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (x < 400 && y < 300) {
			if (fx > RecIn1xy[0] && fx < RecIn1xy[2] && fy < RecIn1xy[1] && fy > RecIn1xy[3]) {
				if (RecIn1xy[0] + 0.1f < RecIn1xy[2]) {
					RecIn1xy[0] += 0.01f;
					RecIn1xy[2] -= 0.01f;
					RecIn1xy[1] -= 0.01f;
					RecIn1xy[3] += 0.01f;
				}
			}
			else {
				RecIn1xy[0] -= 0.01f;
				RecIn1xy[2] += 0.01f;
				RecIn1xy[1] += 0.01f;
				RecIn1xy[3] -= 0.01f;
			}
		}
		else if (x >= 400 && y < 300) {
			if (fx > RecIn2xy[0] && fx < RecIn2xy[2] && fy < RecIn2xy[1] && fy > RecIn2xy[3]) {
				if (RecIn2xy[0] + 0.1f < RecIn2xy[2]) {
					RecIn2xy[0] += 0.01f;
					RecIn2xy[2] -= 0.01f;
					RecIn2xy[1] -= 0.01f;
					RecIn2xy[3] += 0.01f;
				}
			}
			else {
				RecIn2xy[0] -= 0.01f;
				RecIn2xy[2] += 0.01f;
				RecIn2xy[1] += 0.01f;
				RecIn2xy[3] -= 0.01f;
			}
		}
		else if (x < 400 && y >= 300) {
			if (fx > RecIn3xy[0] && fx < RecIn3xy[2] && fy < RecIn3xy[1] && fy > RecIn3xy[3]) {
				if (RecIn3xy[0] + 0.1f < RecIn3xy[2]) {
					RecIn3xy[0] += 0.01f;
					RecIn3xy[2] -= 0.01f;
					RecIn3xy[1] -= 0.01f;
					RecIn3xy[3] += 0.01f;
				}
			}
			else {
				RecIn3xy[0] -= 0.01f;
				RecIn3xy[2] += 0.01f;
				RecIn3xy[1] += 0.01f;
				RecIn3xy[3] -= 0.01f;
			}
		}
		else if (x >= 400 && y >= 300) {
			if (fx > RecIn4xy[0] && fx < RecIn4xy[2] && fy < RecIn4xy[1] && fy > RecIn4xy[3]) {
				if (RecIn4xy[0] + 0.1f < RecIn4xy[2]) {
					RecIn4xy[0] += 0.01f;
					RecIn4xy[2] -= 0.01f;
					RecIn4xy[1] -= 0.01f;
					RecIn4xy[3] += 0.01f;
				}
			}
			else {
				RecIn4xy[0] -= 0.01f;
				RecIn4xy[2] += 0.01f;
				RecIn4xy[1] += 0.01f;
				RecIn4xy[3] -= 0.01f;
			}
		}
	}

	glutPostRedisplay();
}