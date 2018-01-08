#ifndef TRIANGLE_APP_H
#define TRIANGLE_APP_H

#include "..\Window\Window.h"
#include <stdio.h>
#include <math.h>

class Triangle_app
{
public:
	Triangle_app();
	~Triangle_app();
	bool Setup();
	void Application_loop();
	void Shader_error_log(const char* msg, GLuint shader);
	void Program_error_log(const char* msg, GLuint program);
	void Set_matrix_rot(float radians, float (&matrix)[16]);
	static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	GLFWwindow *window;
	GLuint program;
	GLuint vs_int;
	GLuint fs_int;
	GLuint triangle;
	GLuint triangle_ebo;
	float move[2];
	float position[2];
};

#endif //TRIANGLE_APP_H