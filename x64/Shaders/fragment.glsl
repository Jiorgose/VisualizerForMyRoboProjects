#version 460

uniform float time;
uniform vec3 sColor;
uniform float aspectRatio;

uniform vec3 cameraPosition;

in vec2 UV;
out vec4 color;

struct Ray {
  vec3 origin;
  vec3 position;
  vec3 direction;
};

struct Sphere {
  float radius;
  vec3 center;
};

float sphereSDF(vec3 pos, Sphere sphere)
{
  return(length(pos - sphere.center) - sphere.radius);
}

float planeSDF(vec3 pos, float h)
{
  return pos.z + h;
}

vec3 getRayDir(vec2 uv) {
  vec2 p = uv * 2.0 - 1.0;
  p.x *= aspectRatio;
  return normalize(vec3(p, 1.0));
}

void main() {
  //Initialization
  Ray ray;
  ray.origin = cameraPosition;
  ray.direction = getRayDir(UV);

  vec3 col = vec3(0.0);

  float totalDistance = 0.0;

  //Raymarching
  for (int i = 0; i < 80; i++) {
    ray.position = ray.origin + ray.direction * totalDistance;

    Sphere sphere;
    sphere.radius = 1.0;
    sphere.center = vec3(sin(time) * 3.0, 0.0, 0.0);
    float dist = sphereSDF(ray.position, sphere);

    totalDistance += dist;

    if (dist < .001 || dist > 100.0) break;
  }

 col = vec3(totalDistance * 0.2);

  color = vec4(col, 1.0);
}

  //col = 15.0 / 255.0;
