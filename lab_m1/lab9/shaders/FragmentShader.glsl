#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform bool mix_textures;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color1 = texture2D(texture1, texcoord);
    vec4 color2 = texture2D(texture2, texcoord);
    if (mix_textures == true) {
        out_color = mix(color1, color2, 0.5f);
    } else {
        out_color = color1;
    }

    if (out_color.a < 0.5f) {
        discard;
    }

}
