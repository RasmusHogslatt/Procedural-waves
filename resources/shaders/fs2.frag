#version 330

layout (location = 0) out vec4 FragColor;

uniform float near;
uniform float far;
uniform float uTime;
uniform float waveShininess;
uniform vec3 waveColor1;
uniform vec3 waveColor2;
uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;
uniform vec3 lightPos4;
uniform int activeLights;
uniform float lightStrength;

in vec2 texCoords;
in vec3 normal;
in vec3 vertexPosition;
in float oWaveHeight;

vec3 phong(vec3 lightPos, vec3 eye, vec3 n, vec3 point, float shininess, vec3 color) {
	vec3 L = normalize(lightPos - point);
	vec3 R = normalize(-reflect(L, normal));
	
	vec3 lightColor = vec3(1.0, 1.0, 1.0) * lightStrength;
	vec3 diffuse = max(dot(normal, L), 0.0) * color * lightColor;

	vec3 specular = pow(max(dot(R,eye), 0.0), shininess) * lightColor;
	return diffuse + specular;
}

void main(){
	vec3 lights[4] = vec3[4](lightPos1, lightPos2, lightPos3, lightPos4);
	vec3 E = normalize(-vertexPosition);

	
	vec3 resColor = mix(waveColor1, waveColor2, abs(oWaveHeight));
	vec3 color = vec3(0);

	for(int currentLight = 0; currentLight < activeLights; ++currentLight){
		color += phong(lights[currentLight], E, normal, vertexPosition, waveShininess, resColor);
	}

	FragColor = vec4(color, 1.0);
}