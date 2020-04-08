#version 330 

layout (location = 0) in vec3 vertexCords;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;


void main(){
	gl_Position = Projection * View * Model * vec4(vertexCords,1.0f) ; //получаем координаты точки относительно камеры и спроецированной на экран
}