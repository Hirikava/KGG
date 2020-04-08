#include <glad/gl.h>
#include <glfw3.h>
#include <iostream>

#include <cmath>
#include "camera/camera.h"
#include "tools/shaders.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


float func(float x, float y)
{
	return std::cos(x * y);
}

bool mouseFirst = true;

Camera* camera = new Camera(glm::vec3(-3, 0, 0));

bool keys[1024];

GLfloat lastX = 400;
GLfloat lastY = 300;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouseFirst)
	{
		mouseFirst = false;
		lastX = xpos;
		lastY = ypos;
		return;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	// ќбратный пор¤док вычитани¤ потому что оконные Y-координаты возрастают с верху вниз 
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera->Turn(yoffset, xoffset);
}

void do_movement() 
{
	// Camera controls
	GLfloat cameraSpeed = 0.04f;
	if (keys[GLFW_KEY_W])
		camera->MoveForward(cameraSpeed);
	if (keys[GLFW_KEY_S])
		camera->MoveBack(cameraSpeed);
}


int main()
{
	// Блок создания и настройки окна оторбражения
	if (!glfwInit())
	{
		std::cout << "Unable to initialize graphics module" << std::endl;
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	const auto window = glfwCreateWindow(900, 900, "Da'As LAB4", nullptr, nullptr);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //делаем курсор невидимым и привязанным к окну
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!window)
	{
		std::cout << "Unable to create a window" << std::endl;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	// Блок подготовки графического конвейра

	//генерация данных
	float X = -1, Y = -1;
	unsigned int XStepNumber = 200;
	unsigned int YStepNumber = 200;
	float step = 0.02;
	
	GLfloat* data = new GLfloat[XStepNumber * YStepNumber * 3];

	for(int i = 0; i < YStepNumber; i++)
	{
		for(int j = 0; j < XStepNumber; j++)
		{
			float x = X + step * j;
			float y = Y + step * i;
			data[i * 3 * YStepNumber + j * 3] =x;
			data[i * 3* YStepNumber + j * 3 + 1] = y;
			data[i *3 * YStepNumber + j *3 + 2] = func(x, y);
		}
	}
	
	
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * XStepNumber * YStepNumber * 3, data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);



	GLfloat lines[] = {
		-10,0,0,
		10,0,0,
		0,-10,0,
		0,10,0,
		0,0,-10,
		0,0,10
	};
	GLuint MeshVAO, MeshVBO;
	glGenVertexArrays(1, &MeshVAO);
	glBindVertexArray(MeshVAO);

	glGenBuffers(1, &MeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, MeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lines),lines, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	

	GLint status;
	
	GLuint program, vertex, fragment;
	program = glCreateProgram();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertex_shader, 0);
	glShaderSource(fragment, 1, &fragment_shader, 0);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &status);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &status);
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	std::cout << glGetError() << std::endl;

	glUseProgram(program);
	
	GLuint viewLoc = glGetUniformLocation(program, "View");
	GLuint projectionLoc = glGetUniformLocation(program, "Projection");
	GLuint colorLoc = glGetUniformLocation(program, "Color");
	
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)(400) / (GLfloat)(400), 0.1f,1000.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	

	glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 greenColor = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 blueColor = glm::vec3(0.0f, .0f, 1.0f);
	glm::vec3 color = glm::vec3(0.8f, 0.4f, 0.4f);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер

		auto view = camera->GetViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(MeshVAO);
		glUniform3fv(colorLoc, 1, glm::value_ptr(redColor)); //цвет x
		glDrawArrays(GL_LINES, 0, 2);
		glUniform3fv(colorLoc, 1, glm::value_ptr(blueColor)); //цвет y
		glDrawArrays(GL_LINES, 2, 2);
		glUniform3fv(colorLoc, 1, glm::value_ptr(greenColor)); //цвет z
		glDrawArrays(GL_LINES, 4, 2);

		
		glBindVertexArray(VAO);
		glUniform3fv(colorLoc, 1, glm::value_ptr(color)); // цвет графика
		glDrawArrays(GL_POINTS,0, XStepNumber * YStepNumber);
		glfwSwapBuffers(window); //отображаем текущий буфер
		glfwPollEvents(); //функция вызывающая обработку сообщений окна (нужна для управления камерой)
		do_movement();
	}

	return 0;
}
