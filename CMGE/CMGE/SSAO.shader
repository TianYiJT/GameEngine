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

float SampleSize = 16.0f;
float SampleInterval = 32.0f;
int SampleNumber = 20;

float LinearizeDepth(float d)
{
    float zNear = 1.0f;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 500.0f; // TODO: Replace by the zFar  of your perspective projection
    float depth = d*2-1;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}


float computeAO(vec2 UV)
{
	float d = LinearizeDepth(texture2D(CMGE_DEPTHTEX,UV).r);
	vec2 IUV = vec2(CMGE_SCREEN.x*UV.x,CMGE_SCREEN.y*UV.y);
	float sucSamples = 0;
	float allSamples = 0;
	float deltangle=2*3.1415926f/(SampleNumber);
	for(int i=0;i<SampleNumber;i++)
	{
		float uux = sin(deltangle*float(i));
		float vvy = cos(deltangle*float(i));
		float DeltaSample = SampleSize/SampleInterval;
		for(int j=1;j<=SampleInterval;j++)
		{
			float radis = DeltaSample * float(j);
			float ux = uux*radis;
			float vy = vvy*radis;
			vec2 ITUV = vec2(IUV.x+ux,IUV.y+vy);
			ITUV = vec2(ITUV.x/CMGE_SCREEN.x,ITUV.y/CMGE_SCREEN.y);
			float d1 = LinearizeDepth(texture2D(CMGE_DEPTHTEX,ITUV).r);
			if(d1<d)
			{
				sucSamples+=float(SampleInterval-j)/float(SampleInterval);
			}
			allSamples+=1.0f;
		}
	}
	return float(sucSamples)/float(allSamples);
}


void main()
{
	vec4 mainColor = texture2D(CMGE_MAINTEX,myUV);
	float ao = computeAO(myUV);
	ao=clamp(ao,0,1);
	col = mainColor*(1.0f-ao);

	//col = vec4(1.0f-ao,1.0f-ao,1.0f-ao,1);
}

#endfragment