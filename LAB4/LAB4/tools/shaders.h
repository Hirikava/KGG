#pragma once


const char*  fragment_shader = "#version 330\n"
"\n"
"out vec4 FragColor;\n"
"uniform vec3 Color;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(Color,1.0f);\n"
"}\n";


const char*  vertex_shader = "#version 330\n"
"layout(location = 0) in vec3 vertexCords;\n"
"\n"
"uniform mat4 Projection;\n"
"uniform mat4 View;\n"
"void main() {\n"
"	gl_Position = Projection * View * vec4(vertexCords, 1.0f);\n"
"}\n";






