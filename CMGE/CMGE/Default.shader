
#uniform 

Depth On


Cull off

Texture2D  mTex
Texture2D  nTex
Vector4 MainColor

  
#enduniform


#vertex

out vec2 myUV;
out vec3 myNor;
out vec3 myTangent;
out vec3 viewVec;
void main()
{
	gl_Position = CMGE_MVP*vec4(pos.x,pos.y,pos.z,1.0f);
	//gl_Position = vec4(pos,1.0f);
	myUV = uv;
	myNor = nor;
	myTangent = tangent;
	vec3 worldpos = (CMGE_O2W*vec4(pos.x,pos.y,pos.z,1.0f)).rgb;
	viewVec = CMGE_VIEW-worldpos;
}


#endvertex
 

#fragment

in vec2 myUV;
in vec3 myNor;
in vec3 myTangent;
in vec3 viewVec;

vec3 TangentTransform(vec3 nor_,vec3 tangent_,vec3 opos)
{
	vec3 binormal_ = cross(nor_,tangent_);
	mat3 rotation = mat3(tangent_,binormal_,nor_);
	return rotation*opos;
}

void main()
{
	

	vec4 nTexcolor = texture2D(nTex,myUV);
	vec3 oLightDir = (CMGE_W2O*vec4(-0.5f,-0.4f,0.7f,1.0f)).rgb;
	vec3 oView = (CMGE_W2O*vec4(viewVec,1.0f)).rgb;
	vec3 tLightDir = TangentTransform(myNor,myTangent,oLightDir);
	vec3 tView = TangentTransform(myNor,myTangent,oView);
	vec3 tNor = texture2D(nTex,myUV).rgb*2-1;

	float NdotL = dot(normalize(myNor),normalize(oLightDir));
	vec3 blinn = normalize((normalize(oView)+normalize(oLightDir))/2);

	vec3 tblinn = TangentTransform(myNor,myTangent,blinn);

	float spcular = dot(normalize(blinn),normalize(myNor));

	NdotL = clamp(NdotL,0,1);
	spcular = clamp(spcular,0,1);

	spcular = pow(spcular,12);
	
	vec4 col_mTex = texture2D(mTex,myUV);
	col = col_mTex*(vec4(1,1,1,1)*(NdotL*1.6)+0.35+spcular);

	float NdotL_ = dot(normalize(myNor),normalize(oLightDir));
	float spcular_ = dot(normalize(blinn),normalize(myNor));
   spcular_ = pow(spcular_,4);
	col = col_mTex*(vec4(1,1,1,1)*(NdotL_*2.0f)+spcular_*1.8f);

	col.a = 0.3f;

}

#endfragment