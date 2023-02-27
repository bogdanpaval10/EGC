#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 v_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_color;
out vec2 frag_tex_coord;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position = v_position;
    frag_color = v_color + vec3(cos(Time), cos(Time), 1);
    //frag_color = v_normal;
    //frag_color = v_color;
    frag_normal = v_normal;
    frag_tex_coord = tex_coord;

    // TODO(student): Compute gl_Position
    //gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    gl_Position = Projection * View * Model * vec4(v_position[0], v_position[1] + sin(Time), v_position[2], 1.0);
    `
}
