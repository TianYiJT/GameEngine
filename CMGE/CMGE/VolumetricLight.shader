#uniform
Depth On

Cull off

Texture2D RadioLightTex
int NUM_SAMPLES
float Decay
float weight
float Density
float Exposure
Vector2 lightcoord

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

	vec2 uv = myUV;
	vec2 deltaTexcoord = myUV-lightcoord;
	deltaTexcoord*= 1.0f/NUM_SAMPLES*Density;
	vec3  color = texture2D(RadioLightTex,uv).xyz;
	float illumationDecay = 1.0f;
	for(int i=0;i<NUM_SAMPLES;i++)
	{
		uv -= deltaTexcoord;
		vec3 samplecolor = texture2D(RadioLightTex,uv).xyz;
		samplecolor *=illumationDecay*weight;
		color+=samplecolor;
		illumationDecay*=Decay;
	}
	color*=Exposure;
	vec3 maincolor = texture2D(CMGE_MAINTEX,myUV).xyz;
	col = vec4((color+maincolor),1.0f);
	
//	col = vec4(color,1.0f);
	//col = texture2D(RadioLightTex,myUV);
}

#endfragment