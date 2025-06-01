#version 460

uniform vec3 sColor;

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
  for (int i = 0; i < 100; ++i) {
    ray.position = origin + ray.direction * t;
    float dist = scene(ray.position);
    if (dist < 0.001) {
      return false;
    }
    t += dist;
    if (t > 100.0) break;
  }

  return true;
}

void main() {
  spheres[0] = Sphere(vec3(0.0, 1.0, 0.0), 0.5);
  spheres[1] = Sphere(vec3(0.0, 0.0, 0.0), 1.0);

  Sun sun;
  sun.direction = vec3(0.25, 0.25, 0.0);

  Ray ray;

  vec3 origin = vec3(0.0, 0.0, -3.0);
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
        color = vec4(0.0, 0.0, 0.0, 0.0);
        return;
      }
    }
    t += dist;
    if (t > 100.0) break;
  }

  float a = 0.5 * (ray.direction.y + 1.0);
  vec3 skyColor = mix(vec3(1.0, 1.0, 1.0), vec3(0.5, 0.7, 1.0), a);
  color = vec4(skyColor, 1.0);
  return;
}
