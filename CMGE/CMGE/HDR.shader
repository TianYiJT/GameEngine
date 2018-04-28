#uniform

Depth On

Cull off

float exposure

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
	const float gamma = 2.2f;  

    vec3 hdrColor = texture(CMGE_MAINTEX, myUV).rgb;  

    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);  

    mapped = pow(mapped, vec3(1.0 / gamma));  
  
    col = vec4(mapped, 1.0);  
//	col = hdrColor;
}

#endfragment