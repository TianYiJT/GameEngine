#uniform
Depth On

Cull off


Vector2 LightPos
Vector4 LightColor
float maxDistance

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
#define E 2.71828

float computeLightDensity(vec2 my,vec2 light)
{
	float d = distance(vec2(my.x*CMGE_SCREEN.x,my.y*CMGE_SCREEN.y),vec2(light.x*CMGE_SCREEN.x,light.y*CMGE_SCREEN.y));
	float density = pow(E,-d*d/(maxDistance*maxDistance*2));
	return density; 
}

void main()
{
	float depth = texture2D(CMGE_DEPTHTEX,myUV).r;
	float Density = computeLightDensity(myUV,LightPos);
	if(depth>0.98f)
	{
		col = LightColor*Density;
	}
	else
	{
		col = vec4(0.0f,0.0f,0.0f,1.0f);
	}
//	col = vec4(1.0f,0.0f,0.0f,1.0f);
}

#endfragment