#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec2 texCoords;
out vec3 normal;
out vec3 vertexPosition;
out float oWaveHeight;

uniform mat4 V;
uniform mat4 P;
uniform mat4 M;
uniform mat4 N;
uniform float uTime;
uniform float uDeltaTime;
uniform float fbmScale;
uniform int OCTAVES;

uniform vec4 uWave0;
uniform vec4 uWave1;
uniform vec4 uWave2;
uniform vec4 uWave3;

#define NUM_OCTAVES 10

// map from [-1,1] to [0,1]
float normalizeFloat(float v){
	return v*0.5+0.5;
}

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 p){
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}
vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec2 fade(vec2 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}
float cnoise(vec2 P){
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod(Pi, 289.0); // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;
  vec4 i = permute(permute(ix) + iy);
  vec4 gx = 2.0 * fract(i * 0.0243902439) - 1.0; // 1/41 = 0.024...
  vec4 gy = abs(gx) - 0.5;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;
  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);
  vec4 norm = 1.79284291400159 - 0.85373472095314 * 
    vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11));
  g00 *= norm.x;
  g01 *= norm.y;
  g10 *= norm.z;
  g11 *= norm.w;
  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));
  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}

float fbm(vec2 x) {
	float v = 0.0;
	float a = 0.5;
	vec2 shift = vec2(100);
	// Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
	for (int i = 0; i < OCTAVES; ++i) {
		v += a * cnoise(x);
		x = rot * x * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}

struct gerstnerWaveOptions {
	vec2 direction;
	float steepness;
	float wavelength;
};
gerstnerWaveOptions options0 = gerstnerWaveOptions(vec2(0.0, -1.0), 0.15, 0.1);
gerstnerWaveOptions options1 = gerstnerWaveOptions(vec2(-1.0, 1.0), 0.15, 0.4);
gerstnerWaveOptions options2 = gerstnerWaveOptions(vec2(0.3, 0.7), 0.2, 0.5);
gerstnerWaveOptions options3 = gerstnerWaveOptions(vec2(-0.5, -0.4), 0.3, 0.2);

vec3 gerstnerWave(vec3 p, gerstnerWaveOptions opt, float time) {
	float steepness = opt.steepness;
	float wavelength = opt.wavelength;

	float k = 2.0 * 3.14 / wavelength;
	float c = sqrt(9.8 / k);
	vec2 d = normalize(opt.direction);
	float f = k * dot(d, p.xz) - c * time;
	float a = steepness / k;

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
	temp.steepness = opt.z/4.0;
	temp.wavelength = abs(opt.w);
	return temp;
}

vec3 displace(vec3 point) {
	vec3 p = point;
	vec3 n = vec3(0);

	n.y = fbm(p.xz + (uTime*0.1)) * fbmScale;
	n += gerstnerWave(p, getOptions(uWave0), uTime).xyz;
	n += gerstnerWave(p, getOptions(uWave1), uTime).xyz;
	n += gerstnerWave(p, getOptions(uWave2), uTime).xyz;
	n += gerstnerWave(p, getOptions(uWave3), uTime).xyz;

	oWaveHeight = normalizeFloat(n.y);

	return point + n;

}

void main() {
	texCoords = aUV;
	

	float eps = 0.00001;
	vec3 p1 = aPosition + vec3(1, 0, 0) * eps; 
	p1 = displace(p1);
	vec3 p2 = aPosition + vec3(0, 0, 1) * eps; 
	p2 = displace(p2);
	vec3 p3 = aPosition + normalize(((p1+p2)*0.5 - aPosition)) * eps; 
	p3 = displace(p3);
	normal = normalize(cross(normalize(p2-p1), normalize(p2-p3)));

	vec3 point = displace(aPosition).xyz;
	point = displace(point);

	gl_Position = P*V*M*vec4(point, 1.0);
	vertexPosition = gl_Position.xyz;
}