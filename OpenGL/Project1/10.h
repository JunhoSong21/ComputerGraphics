#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define WIDTH 800.0
#define HEIGHT 600.0

#define POINT_GL 1
#define LINE_GL 2
#define TRIANGLE_GL 3
#define RECTANGLE_GL 4
#define PENTAGON_GL 5

#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "10.h"

GLuint vertexShader;
GLuint fragmentShader;

extern char* filetobuf(const char* file);

extern void make_VertexShaders(char* fileName);
extern void make_FragmentShaders(char* fileName);
extern GLuint make_ShaderProgram();

extern void InitBuffer(GLuint vao, GLuint* vbo, GLfloat triShape[][3], GLfloat colors[][3]);

extern void random_color(GLfloat color[][3]);
void random_color_single(GLfloat* color);

extern GLfloat conversion_x(GLfloat x);
extern GLfloat conversion_y(GLfloat y);


char* filetobuf(const char* file) {

	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");
	if (!fptr) return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

void make_VertexShaders(char* fileName) {
	GLchar* vertexSource;

	vertexSource = filetobuf(fileName);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_FragmentShaders(char* fileName)
{
	GLchar* fragmentSource;

	fragmentSource = filetobuf(fileName);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_ShaderProgram()
{
	GLuint shaderID;
	shaderID = glCreateProgram();

	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID);
	return shaderID;
}

void InitBuffer(GLuint vao, GLuint* vbo, GLfloat triShape[][3], GLfloat colors[][3]) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void random_color(GLfloat color[][3]) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 1000);

	for (int i = 0; i < 3; i++)
	{
		GLfloat value = (GLfloat)dis(gen) / (GLfloat)dis(gen);

		color[0][i] = value;
		color[1][i] = value;
		color[2][i] = value;
	}
}

void random_color_single(GLfloat* color) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 1000);

	for (int i = 0; i < 3; i++)
	{
		GLfloat value = (GLfloat)dis(gen) / (GLfloat)dis(gen);

		color[i] = value;
	}
}

GLfloat conversion_x(GLfloat x) { return (GLfloat)((x - (WIDTH / 2)) / (WIDTH / 2)); }

GLfloat conversion_y(GLfloat y) { return (GLfloat)((y - (HEIGHT / 2)) / (HEIGHT / -2)); }