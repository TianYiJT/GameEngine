
#uniform
Depth On

Cull off

float FocusDistance

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


float LinearizeDepth(float d)
{
    float zNear = 1.0f;    
    float zFar  = 500.0f; 
    float depth = d*2-1;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}


void main()
{
	float tempDepth=LinearizeDepth(texture2D(CMGE_DEPTHTEX,myUV).r);
	tempDepth = (500.0f-1.0f)*tempDepth+1.0f;
	if(tempDepth>FocusDistance)
	{
		vec4 mainColor = vec4(0,0,0,0);
		vec2 IUV = vec2(myUV.x*CMGE_SCREEN.x,myUV.y*CMGE_SCREEN.y);
		for(int i=-2;i<=2;i++)
		{
			for(int j=-2;j<=2;j++)
			{
				vec2 tempUV = vec2(IUV.x+i,IUV.y+j);
				tempUV = vec2(tempUV.x/CMGE_SCREEN.x,tempUV.y/CMGE_SCREEN.y);
				mainColor+=texture2D(CMGE_MAINTEX,tempUV);
			}
		}
		mainColor/=25.0f;
		col = mainColor;
	}
	else
	{
		col = texture2D(CMGE_MAINTEX,myUV);
		//col = vec4(1,0,0,1);
	}
	//col = vec4(texture2D(CMGE_DEPTHTEX,myUV).r,texture2D(CMGE_DEPTHTEX,myUV).r,texture2D(CMGE_DEPTHTEX,myUV).r,1);
	//col = vec4(1,0,0,1);
}

#endfragment