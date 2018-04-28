#uniform
Depth On

Cull off

int BlurIntensity

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
	float tempIntensity = 0.0f;
	vec2 iUV = vec2(myUV.x*CMGE_SCREEN.x,myUV.y*CMGE_SCREEN.y);
	for(int i=-BlurIntensity;i<=BlurIntensity;i++)
	{
		for(int j=-BlurIntensity;j<=BlurIntensity;j++)
		{
			vec2 tempIUV = vec2(iUV.x+i,iUV.y+j);
			tempIUV = vec2(tempIUV.x/CMGE_SCREEN.x,tempIUV.y/CMGE_SCREEN.y);
			vec4 temp = texture2D(CMGE_MAINTEX,tempIUV);
			if((temp.r+temp.g+temp.b)/3.0f>=tempIntensity)
			{
				mainColor = temp;
				tempIntensity = (temp.r+temp.g+temp.b)/3.0f;
			}
		}
	}
	if(tempIntensity>0.35f)
	{
		col = mainColor;
	}
	else
	{
		col = vec4(0.0,0.0,0.0,1.0f);
	}
}

#endfragment