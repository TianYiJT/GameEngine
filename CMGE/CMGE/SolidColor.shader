#uniform 

Depth On


Cull off

Vector4 mColor

#enduniform


#vertex


out vec4 myColor;

void main()
{
	gl_Position = CMGE_MVP*vec4(pos.x,pos.y,pos.z,1.0f);
	myColor = color;
}


#endvertex
 

#fragment

in vec4 myColor;


void main()
{
	
	col = myColor;

	//col = mColor;
}

#endfragment