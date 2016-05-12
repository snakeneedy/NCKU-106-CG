// modified from http://antongerdelan.net/opengl/hellotriangle.html
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>

static void error_callback (int error, const char * description);
static void key_callback (GLFWwindow * window, int key, int scancode, int action, int mods);
static void render ();
static std::string readfile (const char *filename);
static unsigned int setup_shader(const char *vertex_shader, const char *fragment_shader);

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
	program = setup_shader(readfile("shaders/vs.glsl").c_str(), readfile("shaders/fs.glsl").c_str());
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

static std::string readfile (const char *filename)
{
	std::ifstream ifs(filename);
	if(!ifs)
		exit(EXIT_FAILURE);
	return std::string( (std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
}

static unsigned int setup_shader(const char *vertex_shader, const char *fragment_shader)
{
	int status, maxLength;
	char *infoLog=nullptr;

	GLuint vs=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const GLchar**)&vertex_shader, nullptr);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE)
	{
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);
		infoLog = new char[maxLength];
		glGetShaderInfoLog(vs, maxLength, &maxLength, infoLog);
		fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);
		delete [] infoLog;
		return 0;
	}

	GLuint fs=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const GLchar**)&fragment_shader, nullptr);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE)
	{
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);
		infoLog = new char[maxLength];
		glGetShaderInfoLog(fs, maxLength, &maxLength, infoLog);
		fprintf(stderr, "Fragment Shader Error: %s\n", infoLog);
		delete [] infoLog;
		return 0;
	}

	unsigned int program=glCreateProgram();
	// Attach our shaders to our program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status==GL_FALSE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		infoLog = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, NULL, infoLog);
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);
		fprintf(stderr, "Link Error: %s\n", infoLog);
		delete [] infoLog;
		return 0;
	}
	return program;
}

