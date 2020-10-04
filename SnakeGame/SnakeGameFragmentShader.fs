#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform int texFlag;

void main()
{
	if(texFlag == 1)
	FragColor = texture(texture1, TexCoord);
	else if(texFlag == 2)
	FragColor = texture(texture2, TexCoord);
	else if(texFlag == 3)
	FragColor = texture(texture3, TexCoord);
	else if(texFlag == 4)
	FragColor = texture(texture4, TexCoord);
}
