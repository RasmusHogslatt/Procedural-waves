#version 330

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 TopDown;

uniform float near;
uniform float far;
uniform float uTime;

in vec2 texCoords;
in vec3 normal;
in vec3 vertexPosition;

void main(){
	vec3 Normal = normal;
	//vec3 lightPosition = vec3(0,10,0); // Light position
	//vec3 L = lightPosition - vertexPosition; // Light direction
	//vec3 E = normalize(-vertexPosition); // Eye position
	//vec3 R = normalize(-reflect(L, N));

	//vec4 lightColor = vec4(0.0824, 0.7686, 0.5961, 0.616);
	//vec4 diffuse = max(dot(N, L), 0.0) * lightColor;
	//float shininess = 100.5;
	//vec4 specular = lightColor * pow(max(dot(R,E), 0.0), shininess);
	//FragColor = vec4(diffuse.xyz + specular.xyz, 1.0);
	//FragColor = vec4(N, 1.0);
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	//FragColor = vec4(0,0,1,1);
	
	TopDown = vec4(Normal, gl_FragCoord.z);
	//TopDown = vec4(0, 1, 0, 1);
}