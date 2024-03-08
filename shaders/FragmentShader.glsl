#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform vec3 frag_color;
uniform vec3 eye_position;
uniform int HP;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    float specular_light=0;
    vec3 light_position=vec3(0,25,0);
    //Light source position

    vec3 L=normalize(light_position-world_position);
    vec3 V=normalize(eye_position-world_position+vec3(0,-10,0));
    vec3 H=normalize(L+V);

    float diffuse_light=max(dot(world_normal,L),0)/2.f;
    
    if (diffuse_light > 0)
    {
      specular_light=pow(max(dot(world_normal,H), 0), 32)/2.f;
    }

    float dist=distance(world_position,light_position);

    float diminish=1.f+0.004*dist-0.00028*pow(dist,2);

    float light=0.125+diminish*(diffuse_light+specular_light);

    out_color = vec4(frag_color*light/3.f*HP,1);
}
