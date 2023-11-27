#version 330 core

in vec4 channelCol; 
in vec3 colOffset; 
out vec4 outCol; 

void main()
{
	outCol = vec4(channelCol.r + colOffset.r, channelCol.g + colOffset.g, channelCol.b + colOffset.b, 1.0);
}