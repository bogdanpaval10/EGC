#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
//uniform vec3 light_direction2;
//uniform vec3 light_position2;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms


uniform vec3 object_color;
uniform int type_light;
uniform float cutt_angle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{

    vec3 V = normalize(eye_position - world_position);
    vec3 L = normalize(light_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = normalize(reflect(-L, world_normal));
    vec3 V2 = normalize(eye_position - world_position);
    vec3 L2 = normalize(light_position - world_position);
    vec3 H2 = normalize(L2 + V2);
    vec3 R2 = normalize(reflect(-L2, world_normal));

    // TODO(student): Define ambient light component
    float ambient_light = 0.25;

    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);
    float diffuse_light2 = material_kd * max(dot(world_normal, L2), 0);

    // TODO(student): Compute specular light component
    float specular_light = 0;
    float specular_light2 = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
        specular_light2 = material_ks * pow(max(dot(V2, R2), 0), material_shininess);
    }

    float cut_off = radians(cutt_angle);
    float spot_light = dot(-L, light_direction);
    float spot_light_limit = cos(cut_off);
    float light = 0.0f;
    
    float factorAtenuare = 1 / distance(light_position, world_position) * distance(light_position, world_position);
    
    if (type_light == 1) {
        if (spot_light > cos(cut_off))
        {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);
            light = ambient_light + light_att_factor * (diffuse_light + specular_light);
        }
    } else {

        light = ambient_light + (diffuse_light + specular_light) * factorAtenuare;
    }
    float light2 = ambient_light + (diffuse_light2 + specular_light2) * factorAtenuare;

    // TODO(student): Write pixel out color
    //out_color = vec4(object_color * (light + light2), 1.0);
    out_color = vec4(object_color * light, 1.0);

}
