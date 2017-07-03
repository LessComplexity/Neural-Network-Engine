#version 330 core

layout(location = 0) in vec4 vertexPosition_modelspace;

void main(){
	gl_Position.xyzw = vertexPosition_modelspace.xyzw;
}