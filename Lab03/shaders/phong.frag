#version 410

struct Light {
    vec4 lPos;  // position of the light
    vec3 la;    // ambient, diffuse, specular
    vec3 ld;
    vec3 ls;
};

struct Material {
    vec3 ka;  // ambient, diffuse, specular
    vec3 kd;
    vec3 ks;
    float sh;
};

vec4 Lighting(vec3 n, vec4 pos, Light light, Material mat) {
    vec3 l = normalize(vec3(light.lPos - pos));
    vec3 v =
        normalize(-pos.xyz);  // camera is at [0,0,0] so the direction to the viewing vector is this
    vec3 r = reflect(-l, n);
    float sDot = max(dot(l, n), 0.0);
    vec3 ambient = light.la * mat.ka;
    vec3 diffuse = light.ld * mat.kd * sDot;
    vec3 specular = vec3(0.0);
    if (sDot > 0) {
        specular = light.ls * mat.ks * pow(max(dot(r, v), 0.0), mat.sh);
    }
    return vec4(ambient + diffuse + specular, 1);
}

layout(location = 0) in vec4 iPosCamera;
layout(location = 1) in vec3 iNormCamera;
layout(location = 0) out vec4 oColor;
uniform Material mat;
uniform Light light;

void main() {
    oColor = Lighting(normalize(iNormCamera), iPosCamera, light, mat);
}
