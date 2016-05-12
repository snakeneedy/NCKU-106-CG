// modified from http://antongerdelan.net/opengl/hellotriangle.html
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <cstdlib>

static void error_callback (int error, const char * description);
static void key_callback (GLFWwindow * window, int key, int scancode, int action, int mods);
static void render ();

GLFWwindow * window;
GLuint vs, fs, program, vao, vbo;

int main (int argc, char * argv[])
{
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	window = glfwCreateWindow(800, 600, "GLFWWindow", NULL, NULL);
	if( !window )
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent( window );
	glewExperimental = GL_TRUE; // must below glfwMakeContextCurrent() to stop glew crashing on OSX
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// ## setup components ##
	float points[] = {
		 0.0f, 0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f
	};
	// vbo = vertex buffer object
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
	// vao = vertex attribute object
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// shader
	//int status, maxLength;
	//char * infoLog = nullptr;
	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"void main () {"
		"  gl_Position = vec4 (vp, 1.0);"
		"}";

	vs = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vs, 1, (const GLchar**)("shaders/vs.glsl"), nullptr);
	glShaderSource(vs, 1, &vertex_shader, nullptr);
	glCompileShader(vs);
	//glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	//if (status == GL_FALSE)
	//{
	//	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);
	//	infoLog = new char[maxLength];
	//	glGetShaderInfoLog(vs, maxLength, &maxLength, infoLog);
	//	fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);
	//	delete [] infoLog;
	//	return EXIT_FAILURE;
	//}
	const char* fragment_shader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main () {"
		"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
		"}";

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fs, 1, (const GLchar**)("shaders/fs.glsl"), nullptr);
	glShaderSource(fs, 1, &fragment_shader, nullptr);
	glCompileShader(fs);
	//glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	//if (status == GL_FALSE)
	//{
	//	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);
	//	infoLog = new char[maxLength];
	//	glGetShaderInfoLog(fs, maxLength, &maxLength, infoLog);
	//	fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);
	//	delete [] infoLog;
	//	return EXIT_FAILURE;
	//}
	// program
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	//
	while( !glfwWindowShouldClose(window) )
	{
		render();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

static void error_callback (int error, const char * description)
{
	fputs(description, stderr);
}

static void key_callback (GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void render ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glfwPollEvents();
	glfwSwapBuffers(window);
}
