#version 400 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 p_osition[];
in vec4 fragPos[];
in vec2 tex_coord[];

out vec3 normals;
out vec3 Position;
out vec4 ModelPosition;
out vec2 Tex_coord;

vec3 createNormal(vec3 v1, vec3 v2, vec3 v3)
{
    vec3 edge1 = v2 - v1;
    vec3 edge2 = v3 - v1;
    vec3 normal = normalize(cross(edge1, edge2));
			
    return normal;
}

void main()
{
    normals = createNormal( p_osition[0],  p_osition[2],  p_osition[1]);

    gl_Position = gl_in[0].gl_Position;
    Position = p_osition[0];
    ModelPosition = fragPos[0];
    Tex_coord = tex_coord[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    Position = p_osition[1];
    ModelPosition = fragPos[1];
    Tex_coord = tex_coord[1];
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    Position = p_osition[2];
    ModelPosition = fragPos[2];
    Tex_coord = tex_coord[2];
    EmitVertex();
    EndPrimitive();
}
