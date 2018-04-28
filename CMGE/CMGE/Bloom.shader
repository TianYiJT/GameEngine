


#uniform
Depth On

Cull off

int BlurIntensity

float BloomIntensity

Texture2D BloomTex

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
			vec4 tempColor =  texture2D(BloomTex,tempIUV);
			mainColor = mainColor + tempColor;
			allpow = allpow + 1;
		}
	}
	mainColor = mainColor/allpow;
	col = mainColor*BloomIntensity+texture2D(CMGE_MAINTEX,myUV);
	//col = mainColor;
}

#endfragment