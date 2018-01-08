#include "Triangle_app.h"

const GLchar* vs =
"#version 430\n"
"uniform mat4 rot;\n"
"uniform vec3 move;\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec3 norm;\n"
"layout(location=0) out vec4 out_norm;\n"
"void main()\n"
"{\n"
"	//Apply projection after translation and rotation\n"
"	vec4 position = rot * vec4(pos, 1) + vec4(move, 1);\n"
"   position[3] = -position[2];\n"
"	\n"
"	gl_Position = position;\n"
"	out_norm = rot * vec4(norm, 1);\n"
"}\n";

const char* fs =
"#version 430\n"
"layout(location=0) in vec4 norm;\n"
"out vec4 out_color;\n"
"void main()\n"
"{\n"
"	out_color = abs(norm);\n"
"}\n";

Triangle_app::Triangle_app()
{
	move[0] = 0;
	move[1] = 0;
	position[0] = 0;
	position[1] = 0;
}

Triangle_app::~Triangle_app()
{
	Window::Destroy_window(window);
}

void Triangle_app::Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Triangle_app* app = (Triangle_app*)glfwGetWindowUserPointer(window);
	switch (key)
	{
		case GLFW_KEY_RIGHT:
			if (action == GLFW_PRESS)
				app->move[0] += 0.01f;
			else if (action == GLFW_RELEASE)
				app->move[0] -= 0.01f;
			break;
		case GLFW_KEY_LEFT:
			if (action == GLFW_PRESS)
				app->move[0] -= 0.01f;
			else if (action == GLFW_RELEASE)
				app->move[0] += 0.01f;
			break;
		case GLFW_KEY_UP:
			if (action == GLFW_PRESS)
				app->move[1] += 0.01f;
			else if (action == GLFW_RELEASE)
				app->move[1] -= 0.01f;
			break;
		case GLFW_KEY_DOWN:
			if (action == GLFW_PRESS)
				app->move[1] -= 0.01f;
			else if (action == GLFW_RELEASE)
				app->move[1] += 0.01f;
			break;
	}
}

void Triangle_app::Shader_error_log(const char* msg, GLuint shader)
{
	GLint log_length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 1)
	{
		GLchar* buf = new GLchar[log_length];
		glGetShaderInfoLog(shader, log_length, NULL, buf);
		printf("%s %s %d\n", msg, buf, log_length);
		delete[] buf;
	}
}

void Triangle_app::Program_error_log(const char* msg, GLuint program)
{
	GLint log_length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 1)
	{
		GLchar* buf = new GLchar[log_length];
		glGetProgramInfoLog(program, log_length, NULL, buf);
		printf("%s %s \n", msg, buf);
		delete[] buf;
	}
}

void Triangle_app::Set_matrix_rot(float radians, float (&matrix)[16])
{
	float mCos = cosf(radians);
	float mSin = sinf(radians);

	matrix[0] = mCos;
	matrix[2] = mSin;
	matrix[8] = -mSin;
	matrix[10] = mCos;
}

bool Triangle_app::Setup()
{
	//Initialize GLFW and create a window
	window = Window::Create_window(640, 640, "Triangle_app");
	if (window == nullptr)
	{
		printf("Window: Failed to create window\nEnter any button to continue ");
		scanf("%*s");
		return false;
	}
	
	//Set this as window user pointer so that the Key_callback can access the class Triangle_app's member variables.
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, Key_callback);
	
	//Setup vertex shader
	vs_int = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_int, 1, &vs, NULL);
	glCompileShader(vs_int);
	Shader_error_log("[VERTEX SHADER COMPILE ERROR]:", vs_int);

	//Setup fragment shader
	fs_int = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs_int, 1, &fs, NULL);
	glCompileShader(fs_int);
	Shader_error_log("[FRAGMENT SHADER COMPILE ERROR]:", fs_int);

	//Create a program object
	program = glCreateProgram();
	glAttachShader(program, vs_int);
	glAttachShader(program, fs_int);
	glLinkProgram(program);
	Program_error_log("[PROGRAM LINK ERROR]:", program);
	
	//Set clear color to gray
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//Setup vbo
	//positions x3 + normals x3
	float vbo[] =
	{
		-0.25f,	-0.25f,	0,	
		0,		0,		1,	
		0,		0.25f,	0,	
		0,		0,		1,		
		0.25f,	-0.25f,	0,	
		0,		0,		1,		
	};
	glGenBuffers(1, &triangle);
	glBindBuffer(GL_ARRAY_BUFFER, triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbo), vbo, GL_STATIC_DRAW);
	
	//Setup ebo
	int ebo[] = { 0, 1, 2 };
	glGenBuffers(1, &triangle_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo), ebo, GL_STATIC_DRAW);
	
	return true;
}

void Triangle_app::Application_loop()
{
	glUseProgram(program);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));
	
	float rot = 0;
	float rot_matrix[] = 
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 0
	};
	
	//Program loop
	while (!glfwWindowShouldClose(window))
	{
        glClear(GL_COLOR_BUFFER_BIT);
		
		//Set position of triangle
		position[0] += move[0];
		position[1] += move[1];
		float move_vec[] = { position[0], position[1], -1 };
		glUniform3fv(glGetUniformLocation(program, "move"), 1, move_vec);
		
		//Rotate then draw triangle
		rot += 0.01f;
		Set_matrix_rot(rot, rot_matrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "rot"), 1, GL_FALSE, rot_matrix);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}