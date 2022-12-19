#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec2 texCoords;
out vec3 normal;
out vec3 vertexPosition;

uniform mat4 V;
uniform mat4 P;
uniform mat4 M;
uniform mat4 N;
uniform float uTime;
uniform float uDeltaTime;

uniform vec4 uWave0;
uniform vec4 uWave1;
uniform vec4 uWave2;
uniform vec4 uWave3;

struct gerstnerWaveOptions {
	vec2 direction;
	float steepness;
	float wavelength;
};
gerstnerWaveOptions options0 = gerstnerWaveOptions(vec2(0.0, -1.0), 0.15, 0.1);
gerstnerWaveOptions options1 = gerstnerWaveOptions(vec2(-1.0, 1.0), 0.15, 0.4);
gerstnerWaveOptions options2 = gerstnerWaveOptions(vec2(0.3, 0.7), 0.2, 0.5);
gerstnerWaveOptions options3 = gerstnerWaveOptions(vec2(-0.5, -0.4), 0.3, 0.2);

vec3 gerstnerWave(vec3 p, gerstnerWaveOptions opt, float time, out vec3 binormal, out vec3 tangent) {
	float steepness = opt.steepness;
	float wavelength = opt.wavelength;

	float k = 2.0 * 3.14 / wavelength;
	float c = sqrt(9.8 / k);
	vec2 d = normalize(opt.direction);
	float f = k * dot(d, p.xz) - c * time;
	float a = steepness / k;

	// Calc tangent
	tangent += vec3(-d.x * d.x * (steepness * sin(f)),
		d.x * (steepness * cos(f)),
		-d.x * d.y * (steepness * sin(f))
	);
	// Calc binormal
	binormal += vec3(-d.x * d.y * (steepness * sin(f)),
		d.y * (steepness * cos(f)),
		-d.y * d.y * (steepness * sin(f))
	);
	// Calc position
	return vec3(
		d.x * (a * cos(f)),
		a * sin(f),
		d.y * (a * cos(f))
	);
}

gerstnerWaveOptions getOptions(vec4 opt) {
	gerstnerWaveOptions temp;
	temp.direction = opt.xy;
	temp.steepness = opt.z;
	temp.wavelength = abs(opt.w);
	return temp;
}

vec3 displace(vec3 point, inout vec3 tangent, inout vec3 binormal) {
	vec3 p = point;
	vec3 n = vec3(0);

	n += gerstnerWave(p, getOptions(uWave0), uTime, tangent, binormal).xyz;
	n += gerstnerWave(p, getOptions(uWave1), uTime, tangent, binormal).xyz;
	n += gerstnerWave(p, getOptions(uWave2), uTime, tangent, binormal).xyz;
	n += gerstnerWave(p, getOptions(uWave3), uTime, tangent, binormal).xyz;
	return point + n;
}

void main() {
	texCoords = aUV;
	
	vec3 tangent = vec3(1,0,0);
	vec3 binormal = vec3(0,1,0);
	vec3 point = displace(aPosition, tangent, binormal).xyz;
	point = displace(point, tangent, binormal);
	normal = normalize((N*vec4(cross(binormal, tangent),1.0)).xyz);

	gl_Position = P*V*M*vec4(point, 1.0);
	vertexPosition = gl_Position.xyz;
}