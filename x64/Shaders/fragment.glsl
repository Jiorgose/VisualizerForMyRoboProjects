#version 460

uniform float time;
uniform vec3 sColor;

uniform vec3 cameraPosition;

in vec2 UV;
out vec4 color;

struct Sphere {
  vec3 center;
  float radius;
};

struct Ray {
  vec3 position;
  vec3 direction;
};

struct Sun {
  vec3 direction;
};

const int sphereCount = 2;
Sphere spheres[sphereCount];

vec3 getRayDir(vec2 uv) {
  vec2 p = uv * 2.0 - 1.0;
  p.x *= 4.0 / 3.0;
  return normalize(vec3(p, 1.0));
}

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

bool calculateLight(vec3 position, Sun sun) {
  Ray ray;
  vec3 origin = position + sun.direction * 0.5;
  ray.direction = sun.direction;

  float t = 0.0;
  for (int i = 0; i < 250; ++i) {
    ray.position = origin + ray.direction * t;
    float dist = scene(ray.position);
    if (dist < 0.001) {
      return false;
    }
    t += dist;
    if (t > 250.0) break;
  }

  return true;
}

void main() {
  spheres[0] = Sphere(vec3(0.0, 1.0, 0.0), 0.5);
  spheres[1] = Sphere(vec3(0.0, 0.0, 0.0), 1.0);

  Sun sun;
  float angle = time * 0.2;
  sun.direction = normalize(vec3(cos(angle), 0.25, sin(angle)));

  Ray ray;

  vec3 origin = cameraPosition;
  ray.direction = getRayDir(UV);

  float t = 0.0;
  for (int i = 0; i < 100; ++i) {
    ray.position = origin + ray.direction * t;
    float dist = scene(ray.position);
    if (dist < 0.001) {
      bool inSun = calculateLight(ray.position, sun);
      if (inSun) {
        color = vec4(sColor, 1.0);
        return;
      }
      else {
        color = vec4(0.05, 0.05, 0.1, 1.0);
        return;
      }
    }
    t += dist;
    if (t > 100.0) break;
  }

  double imguiColor = 15.0 / 255.0;
  color = vec4(imguiColor, imguiColor, imguiColor, 1.0);
  return;
}
