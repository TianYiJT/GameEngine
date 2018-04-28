
#uniform 

Depth On


Cull off

Texture_Cube Skybox



  
#enduniform


#vertex

out vec3 TexCoord0;

void main()
{
	vec4 WVP_Pos = CMGE_MVP*vec4(pos.x,pos.y,pos.z,1.0f);
    gl_Position = WVP_Pos.xyww;
    TexCoord0 = pos;
}


#endvertex
 

#fragment

in vec3 TexCoord0;

void main()
{
	col = texture(Skybox, TexCoord0);
	//col = vec4(1,0,0,1);
}

#endfragment