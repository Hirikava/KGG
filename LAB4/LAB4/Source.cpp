#include <glad/gl.h>
#include <glfw3.h>
#include <iostream>


int main()
{
	if(!glfwInit()) 
	{
		std::cout << "Unable to initialize graphics module" << std::endl;
		exit(1);
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	const auto window = glfwCreateWindow(400, 400, "Da'As LAB4", nullptr, nullptr);

	if(!window)
	{
		std::cout << "Unable to create a window" << std::endl;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT); // чистим буфер
		
		glfwSwapBuffers(window); //отображаем текущий буфер
		glfwPollEvents(); //функция вызывающая обработку сообщений окна (нужна для управления камерой)
	}

	return 0;
}
