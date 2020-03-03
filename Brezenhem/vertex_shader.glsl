#version 440

layout(location = 0) in vec3 vertexCord;

void main()
{
	gl_Position = vec4(vertexCord,1.0f); 
}
