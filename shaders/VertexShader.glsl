#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_text;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int HP;

// Output
out vec3 world_position;
out vec3 world_normal;


void main()
{

    vec4 position=vec4(v_position, 1.0);
    if(HP!=3){
        float dif=position.x-position.z;
        if(dif>0){
            position.x-=dif*(3-HP)/16;
        }
        else if(dif<0){
            position.z+=dif*(3-HP)/16;
        }
    }
    //Tank becomes warped based on its HP

    world_position = (Model * position).xyz;
    world_normal = normalize( mat3(Model) * v_normal );

    gl_Position = Projection * View * Model * position;
}
