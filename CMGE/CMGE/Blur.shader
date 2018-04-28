

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

float Gauss(int x,int y)
{
	float E = 2.71828f;
	float P = -pow(x,2)/2.0f-pow(y,2)/2.0f;
	return pow(E,P);
}


void main()
{
	vec4 mainColor = vec4(0.0f,0.0f,0.0f,0.0f);
	vec2 iUV = vec2(myUV.x*CMGE_SCREEN.x,myUV.y*CMGE_SCREEN.y);
	float allpow = 0.0f;
	for(int i=-BlurIntensity;i<=BlurIntensity;i++)
	{
		for(int j=-BlurIntensity;j<=BlurIntensity;j++)
		{
			vec2 tempIUV = vec2(iUV.x+i,iUV.y+j);
			tempIUV = vec2(tempIUV.x/CMGE_SCREEN.x,tempIUV.y/CMGE_SCREEN.y);
			mainColor = mainColor + texture2D(CMGE_MAINTEX,tempIUV)*Gauss(i,j);
			allpow = allpow + Gauss(i,j);
		}
	}
	mainColor = mainColor/allpow;
	col = mainColor;
}

#endfragment
