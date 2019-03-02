#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

in vec3 Normal[];

out vec3 fColor;
out vec3 fNormal;
out vec3 fPos;

void build_house(vec4 position)
{    
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left   
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right
    EmitVertex();
	fColor = vec3(1.0f, 1.0f, 1.0f);
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
	fNormal = Normal[0];
	fPos = vec3(gl_in[0].gl_Position);
}