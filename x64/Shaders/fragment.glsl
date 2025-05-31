#version 460

uniform vec3 sColor;

in vec2 UV;
out vec4 color;

struct Sphere {
    vec3 center;
    float radius;
};

const int sphereCount = 2;
Sphere spheres[sphereCount];

float sdfSphere(vec3 p, Sphere s) {
    return length(p - s.center) - s.radius;
}

float scene(vec3 p) {
    float minDist = 1e10;
    for (int i = 0; i < sphereCount; ++i) {
        float d = sdfSphere(p, spheres[i]);
        minDist = min(minDist, d);
    }
    return minDist;
}

vec3 getRayDir(vec2 uv) {
    vec2 p = uv * 2.0 - 1.0;
    p.x *= 4.0 / 3.0;
    return normalize(vec3(p, 1.0));
}

void main() {
    spheres[0] = Sphere(vec3(0.0, 1.0, 0.0), 0.5);
    spheres[1] = Sphere(vec3(0.0, 0.0, 0.0), 1.0);

    vec3 rayOrigin = vec3(0.0, 0.0, -3.0);
    vec3 rayDirection = getRayDir(UV);

    float t = 0.0;
    for (int i = 0; i < 100; ++i) {
        vec3 position = rayOrigin + rayDirection * t;
        float dist = scene(position);
        if (dist < 0.001) {
            color = vec4(sColor, 1.0);
            return;
        }
        t += dist;
        if (t > 100.0) break;
    }

    color = vec4(0.0, 0.0, 0.0, 1.0);
}
