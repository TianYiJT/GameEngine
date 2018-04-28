#version 420 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec2 uvnormal;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec4 color;
uniform vec4 mColor;
uniform mat4 CMGE_MVP;
uniform mat4 CMGE_O2W;
uniform mat4 CMGE_W2O;
uniform mat4 CMGE_S2W;
uniform mat4 CMGE_W2S;
uniform vec3 CMGE_VIEW;
uniform vec2 CMGE_SCREEN;
uniform int CMGE_LOD;
uniform int CMGE_PRE_RENDER_OR_NOT;
uniform vec4 CMGE_MAIN_COLOR;
uniform sampler2D CMGE_DEPTHTEX;
uniform sampler2D CMGE_MAINTEX;
uniform mat4 CMGE_LIGHT0;
uniform mat4 CMGE_LIGHT1;
uniform mat4 CMGE_LIGHT2;
uniform mat4 CMGE_LIGHT3;
uniform mat4 CMGE_LIGHT4;
uniform mat4 CMGE_LIGHT5;
uniform mat4 CMGE_LIGHT6;
out vec4 myColor;
void main()
{
	gl_Position = CMGE_MVP*vec4(pos.x,pos.y,pos.z,1.0f);
	myColor = vec4(pos.x,pos.y,pos.z,1.0f);
}
