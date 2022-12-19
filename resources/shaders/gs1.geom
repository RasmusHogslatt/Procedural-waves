#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform float boxSize;

out vec3 normal;

vec3 getNormal(vec4 p1, vec4 p2, vec4 p3) {
    vec3 v1 = normalize(p2.xyz-p1.xyz);
    vec3 v2 = normalize(p2.xyz-p3.xyz);
    return normalize(cross(v1,v2));
}

void main() {    
    mat4 mvp = P*V*M;
    vec4 center = gl_in[0].gl_Position;
    vec4 dx = mvp[0]*boxSize;
    vec4 dy = mvp[1]*boxSize;
    vec4 dz = mvp[2]*boxSize;

    vec4 p1 = center - 0.5*dx - 0.5*dy - 0.5*dz;
    vec4 p2 = p1 + dx;
    vec4 p3 = p1 + dz;
    vec4 p4 = p1 + dx + dz;
    vec4 p5 = p1 + dy;
    vec4 p6 = p5 + dx;
    vec4 p7 = p5 + dz;
    vec4 p8 = p5 + dx + dz;
    
    // Bottom
    normal = getNormal(p1,p2,p3);
    gl_Position = p2;
    EmitVertex();
    gl_Position = p1;
    EmitVertex();
    gl_Position = p4;
    EmitVertex();
    gl_Position = p3;
    EmitVertex();
    EndPrimitive();

    // Top
    normal = -normal;//getNormal(p6,p5,p8);
    gl_Position = p5;
    EmitVertex();
    gl_Position = p6;
    EmitVertex();
    gl_Position = p7;
    EmitVertex();
    gl_Position = p8;
    EmitVertex();
    EndPrimitive();

    // Right
    normal = getNormal(p4,p2,p6);
    gl_Position = p2;
    EmitVertex();
    gl_Position = p4;
    EmitVertex();
    gl_Position = p6;
    EmitVertex();
    gl_Position = p8;
    EmitVertex();
    EndPrimitive();

    // Left
    normal = -normal;
    gl_Position = p3;
    EmitVertex();
    gl_Position = p1;
    EmitVertex();
    gl_Position = p7;
    EmitVertex();
    gl_Position = p5;
    EmitVertex();
    EndPrimitive();

    // Front
    normal = getNormal(p1,p2,p5);
    gl_Position = p1;
    EmitVertex();
    gl_Position = p2;
    EmitVertex();
    gl_Position = p5;
    EmitVertex();
    gl_Position = p6;
    EmitVertex();
    EndPrimitive();

    // Back
    normal = -normal;
    gl_Position = p4;
    EmitVertex();
    gl_Position = p3;
    EmitVertex();
    gl_Position = p8;
    EmitVertex();
    gl_Position = p7;
    EmitVertex();
    EndPrimitive();
}  