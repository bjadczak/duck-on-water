#version 410 core

out vec4 fragColor;

in vec3 localPos;
in vec3 worldPos;
in vec2 texCoords;
in vec3 norm;

uniform vec3 viewPos;

struct Material {
    float shininess;
    vec4 albedo;

    bool hasTexture;
    sampler2D texture;
};
uniform Material material;

struct PointLight {
    vec3 position;
    vec3 color;
    float strength;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};
uniform PointLight pointLight;

uniform samplerCube cubemapTexture;


vec3 intersect_ray(vec3 p, vec3 v) {
    vec3 t_m = (vec3(-1, -1, -1) - p) / v;
    vec3 t_p = (vec3(1, 1, 1) - p) / v;
    vec3 t = max(t_m, t_p);
    float a = min(t.x, min(t.y, t.z));
    return p + a * v;
}

float fresnel(vec3 v, vec3 n) {
    float co = max(dot(v, n), 0);
    float F0 = 0.14;
    return F0 + (1 - F0) * pow(1 - co, 5);
}

vec3 intersectRay(vec3 p, vec3 d)
{
    vec3 tplus  = (1.0 - p) / d;
    vec3 tminus = (-1.0 - p) / d;

    vec3 t = max(tplus, tminus);

    float tmp = min(t.x, min(t.y, t.z));

    return p + tmp * d;
}

void main() {
    vec3 view_vector = normalize(viewPos.xyz - worldPos);
    vec3 normal = normalize(texture(material.texture, texCoords).rgb);
    float n1n2 = 3.0/4.0;

    bool below = dot(view_vector, normal) < 0;
    if (below) {
        normal.y = -normal.y;
        n1n2 = 1.0 / n1n2;
    }

    vec3 reflected = reflect(-view_vector, normal);
    vec3 refracted = refract(-view_vector, normal, n1n2);
    float fresnel_value = fresnel(view_vector, normal);

    vec3 reflectCol = texture(cubemapTexture, intersectRay(localPos, reflected)).rgb;
    vec3 refractCol = texture(cubemapTexture, intersectRay(localPos, refracted)).rgb;

    if (!all(isnan(refracted))) {
        fragColor = vec4(mix(refractCol, reflectCol, fresnel_value), 1.0f);
    } else {
        fragColor = vec4(reflectCol, 1.0f);
    }
}