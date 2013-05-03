#version 150
 
uniform vec3 color;

uniform mat4 viewMatrix, projMatrix;
uniform vec4 LightPos, LightDif, LightAmb, LightSpec;
uniform vec3 eye;
 
in vec4 position;
in vec3 normal;
float NdotL,NdotHV;

vec3 vertex_normal;
vec4 LP;


mat3 M = mat3(viewMatrix[0].xyz,viewMatrix[1].xyz,viewMatrix[2].xyz);
mat3 N = transpose(inverse(M));
out vec4 Color;

void main()
{
	//after applying light equation here:
	Color.w=1; //delete after applying light equation
	Color=vec4(0,0,0,1.0); // delete after applying light equation
	Color=vec4(color,1); // delete after applying light equation
	
	gl_Position = projMatrix * viewMatrix * position ;


}