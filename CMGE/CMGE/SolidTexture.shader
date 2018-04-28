#uniform 

Depth On


Cull off

Texture2D MainTexture

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
	
	col = texture2D(MainTexture,myUV);

}

#endfragment