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

int main (int argc, char * argv[])
{
	GLFWwindow * window;
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
	while( !glfwWindowShouldClose(window) )
	{
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
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
}
