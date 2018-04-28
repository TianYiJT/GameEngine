#version 420 core
layout(location=0) out vec4 col;
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
in vec4 myColor;
void main()
{
	
	col = vec4(myColor.r,1,0,1.0f);
	//col = mColor;
}
