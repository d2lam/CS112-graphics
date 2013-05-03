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
vec4 total,V;

mat3 M = mat3(viewMatrix[0].xyz,viewMatrix[1].xyz,viewMatrix[2].xyz);
mat3 N = transpose(inverse(M));
out vec4 Color;

void main()
{
	//Blinn Phong Lighting
	gl_Position = projMatrix * viewMatrix * position ;
	//gl_Position = gl_Position / gl_Position.w;

	//initialize the total
	total = vec4(0,0,0,1.0);

	//V = MV*position
	V = viewMatrix * position;

	//L = Light_position - V vector
	LP = normalize( LightPos  - V);

	//H	=	(L+V)/(|L+V|)
	vec4 H = (LP+vec4(eye,1))/(length(LP+vec4(eye,1)));
	vertex_normal = normalize(normal);

	//distance is length of H
	//float d = length(H);
	//shininess coefficent I fixed at 5.0 (last assignement value)
	float n = 5.0;
	//attenuation is fixed at 1
	float att = 1.0;
	//dot(N.L)
	NdotL = dot(vec4(vertex_normal,1),LP);
	
	total += LightAmb;
	
	if(NdotL > 0.0)
	{
		//diffuse component
		total += LightDif * NdotL * att;	
		
		vec4 E = normalize(vec4(eye,1));
		vec4 R = reflect(LP, vec4(vertex_normal,1));
		
		//specular component
		NdotHV = dot(R, E);
		float specular;

		if(NdotHV > 0.0)
		{
			specular = pow( max(NdotHV, 0.0), n );
			total += LightSpec * specular * att;
		}
		else{/*0 power of anything is 0 so do nothing here*/}	
	}
	Color = total;
}