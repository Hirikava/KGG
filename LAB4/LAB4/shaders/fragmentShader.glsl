#version 330

out vec4 FragColor;
uniform buffer depthBuffer[GL_RENDER_BUFFER_SIZE_X,GL_RENDER_BUFFER_SIZE_Y]; //передаём в шейдер буфер глубины 

void main()
{
	if(gl_Position.z < depthBuffer[x,y]) //Сравниваем занчение в буфере и значение в пикселе 
	{
		depthBuffer[x,y] = gl_Position.z; //Если значение пикселя меньше, то рисуем его и обновляем буфер
		FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
	}
}