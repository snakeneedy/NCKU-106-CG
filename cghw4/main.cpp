#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "tiny_obj_loader.h"

#define GLM_FORCE_RADIANS


struct object_struct{
	unsigned int program;
	unsigned int vao;
	unsigned int vbo[4];
	unsigned int texture;
	glm::mat4 model;
	object_struct(): model(glm::mat4(1.0f)){}
} ;

std::vector<object_struct> objects;//vertex array object,vertex buffer object and texture(color) for objs
unsigned int program, program2;
std::vector<int> indicesCount;//Number of indice of objs

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static unsigned int setup_shader(const char *vertex_shader, const char *fragment_shader);
void setup_both_shader(const char *vertex_shader, const char *fragment_shader);
static std::string readfile(const char *filename);
static unsigned char *load_bmp(const char *bmp, unsigned int *width, unsigned int *height, unsigned short int *bits);
static int add_obj(unsigned int program, const char *filename,const char *texbmp);
static void releaseObjects();
static void setUniformMat4(unsigned int program, const std::string &name, const glm::mat4 &mat);
static void setBothUniformMat4(const std::string &name, const glm::mat4 &mat);
static void setBothUniformVec3(const std::string &name, const glm::vec3 &vec);
static void render();

int main(int argc, char *argv[])
{
	GLFWwindow* window;
	// set which function to be called when error occur
	glfwSetErrorCallback(error_callback);
	// initialize GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);
	// OpenGL 3.3, Mac OS X is reported to have some problem. However I don't have Mac to test
	// set the specified window hint
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// For Mac OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// create a window with specific size and title
	int windowWidth = 800, windowHeight = 600;
	window = glfwCreateWindow(windowWidth, windowHeight, "Computer Graphic HW. 4", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// make the context of the specific window on the calling thread
	glfwMakeContextCurrent(window);

	// This line MUST put below glfwMakeContextCurrent
	glewExperimental = GL_TRUE;
	glewInit();

	// Enable vsync
	glfwSwapInterval(1);

	// Setup input callback
	glfwSetKeyCallback(window, key_callback);

	// load shader program
	setup_both_shader(readfile("shaders/vs.glsl").c_str(), readfile("shaders/fs.glsl").c_str());
	// program = setup_shader(readfile("shaders/vs.glsl").c_str(), readfile("shaders/fs.glsl").c_str());
	// program2 = setup_shader(readfile("shaders/vs.glsl").c_str(), readfile("shaders/fs.glsl").c_str());

	// int sun = add_obj(program, "materials/sun.obj","materials/sun.bmp");
	// int earth = add_obj(program2, "materials/earth.obj","materials/earth.bmp");

	// GL_DEPTH_TEST: do depth comparisons and update the depth buffer
	// glEnable(GL_DEPTH_TEST);
	// cull out back-face
	// glCullFace(GL_BACK);
	// Enable blend mode for billboard
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// default viewpoint in HW.2
	glm::vec3 cameraPos = glm::vec3(20.0f);
	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)windowWidth/windowHeight), 1.0f, 100.f);

	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 10.0f);

	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	// glm::mat4 tl=glm::translate(glm::mat4(),glm::vec3(15.0f,0.0f,0.0));
	// glm::mat4 rot;
	// glm::mat4 rev;

	// reference velocity
	const float constFVelocity = 1000.f;
	// set velocity (with real-world ratio)
	float earthRevolutionRadius = 11.5f,
			earthRevolutionVelocity = constFVelocity / 365.25f,
			earthRotationVelocity = constFVelocity,
			sunRotationVelocity = constFVelocity / 24.47f;
	float last, start;
	last = start = glfwGetTime();

	std::cout << "0: HW2 shading (default)\n";
	std::cout << "1: Flat shading\n";
	std::cout << "2: Gouraud shading\n";
	std::cout << "3: Phong shading\n";
	std::cout << "4: Blinn-phong shading\n";
	
	while (!glfwWindowShouldClose(window))
	{//program will keep draw here until you close the window
		float delta = glfwGetTime() - start;
		// send model's translation and rotation to sl.
		setUniformMat4(::program, "model", glm::rotate(glm::mat4(1.f), (float) glm::radians(sunRotationVelocity * delta), glm::vec3(0,1,0)));
		setUniformMat4(::program2, "model",  glm::translate(glm::mat4(),glm::vec3(earthRevolutionRadius * cos(-glm::radians(earthRevolutionVelocity * delta)),0.0f,earthRevolutionRadius * sin(-glm::radians(earthRevolutionVelocity * delta)))) *
			glm::rotate(glm::mat4(1.f), (float) glm::radians(earthRotationVelocity * delta), glm::vec3(0,1,0)));
		setBothUniformMat4("view", view);
		setBothUniformMat4("projection", projection);
		setBothUniformVec3("lightColor", lightColor);
		setBothUniformVec3("lightPos", lightPos);
		setBothUniformVec3("cameraPos", cameraPos);
		// draw!!!
		render();
		// swap the front and back buffers of the specified window
		glfwSwapBuffers(window);
		// process events, which is in the event queue, and then returns immediately
		glfwPollEvents();
	}

	releaseObjects();
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}



static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		setup_both_shader(readfile("shaders/vs.glsl").c_str(), readfile("shaders/fs.glsl").c_str());
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		setup_both_shader(readfile("shaders/flat.vs.glsl").c_str(), readfile("shaders/flat.fs.glsl").c_str());
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		setup_both_shader(readfile("shaders/gouraud.vs.glsl").c_str(), readfile("shaders/gouraud.fs.glsl").c_str());
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		setup_both_shader(readfile("shaders/phong.vs.glsl").c_str(), readfile("shaders/phong.fs.glsl").c_str());
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		setup_both_shader(readfile("shaders/blinn-phong.vs.glsl").c_str(), readfile("shaders/blinn-phong.fs.glsl").c_str());
}

static unsigned int setup_shader(const char *vertex_shader, const char *fragment_shader)
{
	GLuint vs=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const GLchar**)&vertex_shader, nullptr);

	glCompileShader(vs);

	int status, maxLength;
	char *infoLog=nullptr;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE)
	{
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];

		glGetShaderInfoLog(vs, maxLength, &maxLength, infoLog);

		fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
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

		/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];

		glGetShaderInfoLog(fs, maxLength, &maxLength, infoLog);

		fprintf(stderr, "Fragment Shader Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
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


		/* The maxLength includes the NULL character */
		infoLog = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, NULL, infoLog);

		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		fprintf(stderr, "Link Error: %s\n", infoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete [] infoLog;
		return 0;
	}
	return program;
}
void setup_both_shader(const char *vertex_shader, const char *fragment_shader)
{
	releaseObjects();
	program = setup_shader(vertex_shader, fragment_shader);
	program2 = setup_shader(vertex_shader, fragment_shader);
	int sun = add_obj(program, "materials/sun.obj","materials/sun.bmp");
	int earth = add_obj(program2, "materials/earth.obj","materials/earth.bmp");
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}

static std::string readfile(const char *filename)
{
	std::ifstream ifs(filename);
	if(!ifs)
		exit(EXIT_FAILURE);
	return std::string( (std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
}

// mini bmp loader written by HSU YOU-LUN
static unsigned char *load_bmp(const char *bmp, unsigned int *width, unsigned int *height, unsigned short int *bits)
{
	unsigned char *result=nullptr;
	FILE *fp = fopen(bmp, "rb");
	if(!fp)
		return nullptr;
	char type[2];
	unsigned int size, offset;
	// check for magic signature	
	fread(type, sizeof(type), 1, fp);
	if(type[0]==0x42 || type[1]==0x4d){
		fread(&size, sizeof(size), 1, fp);
		// ignore 2 two-byte reversed fields
		fseek(fp, 4, SEEK_CUR);
		fread(&offset, sizeof(offset), 1, fp);
		// ignore size of bmpinfoheader field
		fseek(fp, 4, SEEK_CUR);
		fread(width, sizeof(*width), 1, fp);
		fread(height, sizeof(*height), 1, fp);
		// ignore planes field
		fseek(fp, 2, SEEK_CUR);
		fread(bits, sizeof(*bits), 1, fp);
		unsigned char *pos = result = new unsigned char[size-offset];
		fseek(fp, offset, SEEK_SET);
		while(size-ftell(fp)>0)
			pos+=fread(pos, 1, size-ftell(fp), fp);
	}
	fclose(fp);
	return result;
}

static int add_obj(unsigned int program, const char *filename,const char *texbmp)
{
	object_struct new_node;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, filename);

	if (!err.empty()||shapes.size()==0)
	{
		std::cerr<<err<<std::endl;
		exit(1);
	}

	glGenVertexArrays(1, &new_node.vao);
	glGenBuffers(4, new_node.vbo);
	glGenTextures(1, &new_node.texture);

	glBindVertexArray(new_node.vao);

	// Upload postion array
	glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.positions.size(),
			shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	if(shapes[0].mesh.texcoords.size()>0)
	{

		// Upload texCoord array
		glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.texcoords.size(),
				shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindTexture( GL_TEXTURE_2D, new_node.texture);
		unsigned int width, height;
		unsigned short int bits;
		unsigned char *bgr=load_bmp(texbmp, &width, &height, &bits);
		GLenum format = (bits == 24? GL_BGR: GL_BGRA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, bgr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete [] bgr;
	}

	if(shapes[0].mesh.normals.size()>0)
	{
		// Upload normal array
		glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.normals.size(),
				shapes[0].mesh.normals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Setup index buffer for glDrawElements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_node.vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*shapes[0].mesh.indices.size(),
			shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

	indicesCount.push_back(shapes[0].mesh.indices.size());

	glBindVertexArray(0);

	new_node.program = program;

	objects.push_back(new_node);
	return objects.size()-1;
}

static void releaseObjects()
{
	for(int i=0;i<objects.size();i++){
		glDeleteVertexArrays(1, &objects[i].vao);
		glDeleteTextures(1, &objects[i].texture);
		glDeleteBuffers(4, objects[i].vbo);
	}
	glDeleteProgram(program);
	objects.clear();
}

static void setUniformMat4(unsigned int program, const std::string &name, const glm::mat4 &mat)
{
	// This line can be ignore. But, if you have multiple shader program
	// You must check if currect binding is the one you want
	glUseProgram(program);
	GLint loc=glGetUniformLocation(program, name.c_str());
	if(loc==-1) return;

	// mat4 of glm is column major, same as opengl
	// we don't need to transpose it. so..GL_FALSE
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

static void setBothUniformMat4(const std::string &name, const glm::mat4 &mat)
{
	setUniformMat4(::program, name, mat);
	setUniformMat4(::program2, name, mat);
}

static void setBothUniformVec3(const std::string &name, const glm::vec3 &vec)
{
	GLint loc;

	glUseProgram(::program);
	loc = glGetUniformLocation(::program, name.c_str());
	glUniform3f(loc, vec.x, vec.y, vec.z);

	glUseProgram(::program2);
	loc = glGetUniformLocation(::program2, name.c_str());
	glUniform3f(loc, vec.x, vec.y, vec.z);
}

static void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int i=0;i<objects.size();i++){
		glUseProgram(objects[i].program);
		glBindVertexArray(objects[i].vao);
		glBindTexture(GL_TEXTURE_2D, objects[i].texture);
		//you should send some data to shader here
		// setUniformMat4(objects[i].program, "model", objects[i].model);
		glDrawElements(GL_TRIANGLES, indicesCount[i], GL_UNSIGNED_INT, nullptr);
	}
	glBindVertexArray(0);
}
