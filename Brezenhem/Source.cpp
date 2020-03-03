
#include "../opengl/opengl.h"
#include <iostream>
#include <SOIL.h>
#include <fstream>
#include <sstream>

const unsigned int vesrionMajor = 4;
const unsigned int versionMinor = 5;



const float X1 = -1;
const float X2 = 1;

const float A = 1;
const float B = -0.5;
const float C = 0;

const unsigned int resolutionX = 500;
const unsigned int resolutionY = 500;


float porabola(float x)
{
	return A * x * x  + B * x + C;
}



std::string readShader(const char* filename)
{
	std::ifstream fileIn(filename, std::ios_base::in);
	if (!fileIn)
		return "";
	std::stringstream ss;
	ss << fileIn.rdbuf();
	fileIn.close();
	return ss.str();
}



int main()
{
	if (!opengl::Build(4, 6))
	{
		std::cout << "Can't initialize OpenGL v." << vesrionMajor << '.' << versionMinor << std::endl;
		exit(1);
	}

	auto context = opengl::CreateContext(4, 6);
	if (context == nullptr)
	{
		std::cout << "Can't create OpenGL rendering context v." << vesrionMajor << '.' << versionMinor << std::endl;
		exit(1);
	}


	context->MakeCurrent();


	// Prepear program
	GLuint program = glCreateProgram();
	GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
	GLuint gsh = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);

	auto vertexShaderStr = readShader("vertex_shader.glsl");
	const char* vertexShaderSource[1] = { vertexShaderStr.c_str() };
	auto fragmentShaderStr = readShader("fragment_shader.glsl");
	const char* fragmentShaderSource[1] = { fragmentShaderStr.c_str() };
	auto geometryShaderStr = readShader("geometry_shader.glsl");
	const char* geometrySahderSource[1] = { geometryShaderStr.c_str() };
	
	glShaderSource(vsh, 1, vertexShaderSource, nullptr);
	glShaderSource(fsh, 1, fragmentShaderSource, nullptr);
	glShaderSource(gsh, 1, geometrySahderSource, nullptr);

	glCompileShader(vsh);
	glCompileShader(fsh);
	glCompileShader(gsh);

	glAttachShader(program, vsh);
	glAttachShader(program, gsh);
	glAttachShader(program, fsh);
	glLinkProgram(program);


	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		char log[512];
		GLint logSize;
		glGetProgramInfoLog(program, 512, &logSize, log);
		std::cout << log;
	}


	
	float dx = ((X2 - X1) / resolutionX);
	float* data = new float[resolutionX * 3];
	
	for (int i = 0; i < resolutionX * 3; i += 3)
	{
		data[i] = X1 + dx * i/3;
		data[i + 1] = -porabola(data[i]);
		data[i + 2] = 0;
	}

	
	
	GLuint vao,vbo;
	glGenBuffers(1,&vbo);
	glGenVertexArrays(1, &vao);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferStorage(GL_ARRAY_BUFFER, sizeof(float) * resolutionX * 3 , data, 0);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
	std::cout << glGetError();
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	

	//Configure Rendering
	GLuint frameBuffer, depthRenderBuffer;
	GLuint texture;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


	glGenTextures(1, &texture); //Create and bind texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolutionX, resolutionY, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr); //Initialize texture with resolution 1000x1000;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // rasterization settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolutionX, resolutionY);


	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

	GLenum drawBuffers = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Frame buffer not complete" << std::endl;
	}


	//Rendering
	glViewport(0, 0, resolutionX, resolutionY);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //white
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(program);
	GLuint uniformLocationX = glGetUniformLocation(program, "stridePerPixelX");
	glUniform1f(uniformLocationX, 2.0f / resolutionX);
	GLuint uniformLocationY = glGetUniformLocation(program, "stridePerPixelY");
	glUniform1f(uniformLocationY, 2.0f / resolutionY);
	glBindVertexArray(vao);


	glDrawArrays(GL_LINE_STRIP, 0, resolutionX);
	


	//Save texture
	GLubyte* pixels = new GLubyte[resolutionX * resolutionY * 24];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	SOIL_save_image("textrue.bmp", SOIL_SAVE_TYPE_BMP, resolutionX, resolutionY, 3, pixels);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &depthRenderBuffer);
	context->Release();

	delete[] pixels;
	return 0;
}
