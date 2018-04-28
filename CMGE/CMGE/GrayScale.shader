

#uniform
Depth On

Cull off

float GrayIntensity

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

void main()
{
	vec4 mainColor = texture2D(CMGE_MAINTEX,myUV);
	float depth = texture2D(CMGE_DEPTHTEX,myUV).r;
	// mainColor = vec4(1,0,0,1);
	float gray = (mainColor.r+mainColor.g+mainColor.b)/3;
	col = vec4(gray*GrayIntensity,gray*GrayIntensity,gray*GrayIntensity,1.0f);
//	col = vec4(depth,depth,depth,1);
	//col = mainColor;
	
}

#endfragment