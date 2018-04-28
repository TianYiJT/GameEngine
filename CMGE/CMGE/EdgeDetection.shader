#uniform
Depth On

Cull off



#enduniform


#vertex

out vec2 myUV;
void main()
{
	gl_Position = vec4(pos.x,pos.y,pos.z,1.0f);
	myUV = uv;
}

#endvertex

#fragment
in vec2 myUV;

float countGray(vec4 color)
{
	return (color.r+color.g+color.b)/3.0f;
}

float countSobel(vec2 uv)
{
	vec2 deltaVx = vec2(1.0f/CMGE_SCREEN.x,0.0f);
	vec2 deltaVy = vec2(0.0f,1.0f/CMGE_SCREEN.y);
	float gx = -(texture2D(CMGE_DEPTHTEX,uv-deltaVx-deltaVy).r)+(texture2D(CMGE_DEPTHTEX,uv+deltaVx-deltaVy).r)+
		 -2.0f*(texture2D(CMGE_DEPTHTEX,uv-deltaVx).r)+2.0f*(texture2D(CMGE_DEPTHTEX,uv+deltaVx).r)+
		 -(texture2D(CMGE_DEPTHTEX,uv-deltaVx+deltaVy).r)+(texture2D(CMGE_DEPTHTEX,uv+deltaVx+deltaVy).r);

	float gy = -(texture2D(CMGE_DEPTHTEX,uv-deltaVx-deltaVy).r)+(texture2D(CMGE_DEPTHTEX,uv-deltaVx+deltaVy).r)+
		 -2.0f*(texture2D(CMGE_DEPTHTEX,uv-deltaVy).r)+2.0f*(texture2D(CMGE_DEPTHTEX,uv+deltaVy).r)+
		 -(texture2D(CMGE_DEPTHTEX,uv+deltaVx-deltaVy).r)+(texture2D(CMGE_DEPTHTEX,uv+deltaVx+deltaVy).r);

	return pow(gx*gx+gy*gy,0.5f);
}

void main()
{
	float _sobel = countSobel(myUV);
	col = vec4(_sobel,_sobel,_sobel,1.0f);
//	col = vec4(1.0f,1.0f,1.0f,1.0f);
}

#endfragment