#version 330

out vec4 FragColor;
uniform buffer depthBuffer[GL_RENDER_BUFFER_SIZE_X,GL_RENDER_BUFFER_SIZE_Y]; //������� � ������ ����� ������� 

void main()
{
	if(gl_Position.z < depthBuffer[x,y]) //���������� �������� � ������ � �������� � ������� 
	{
		depthBuffer[x,y] = gl_Position.z; //���� �������� ������� ������, �� ������ ��� � ��������� �����
		FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
	}
}