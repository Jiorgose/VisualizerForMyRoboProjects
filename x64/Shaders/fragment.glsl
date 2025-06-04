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

mat2 rot2D(float angle) {
  float sine = sin(angle);
  float cosine = cos(angle);
  return mat2(cosine, -sine, sine, cosine);
}

float smin(float a, float b, float k) {
  float h = clamp(0.5 + 0.5*(a-b)/k, 0.0, 1.0);
  return mix(a, b, h) - k*h*(1.0-h);
}

float sphereSDF(vec3 pos, Sphere sphere)
{
  return(length(pos - sphere.center) - sphere.radius);
}

float boxSDF(vec3 p, vec3 b)
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float planeSDF(vec3 p, vec2 size, float h)
{
  vec3 q = p - vec3(0.0, h, 0.0);

  vec2 d = abs(q.xz) - size;
    
  vec2 outside = max(d, vec2(0.0));
  float horizDist = length(outside);

  float vertDist = q.y;

  return length(vec2(horizDist, vertDist));
}


float scene(vec3 pos) {
  Sphere sphere;
  sphere.radius = 1.0;
  sphere.center = vec3(sin(time) * 3.0, 0.0, 0.0);

  Sphere sphere2;
  sphere2.radius = 0.5;
  sphere2.center = vec3((sin(time) * 3.0) + (sin(time * 4) * 1.5), 0.0, 0.0);

  float SDFsphere = sphereSDF(pos, sphere);
  float SDFsphere2 = sphereSDF(pos, sphere2);

  vec3 q = pos;

  q.xy *= rot2D(time);
  q.xz *= rot2D(time);
  q.zy *= rot2D(time);

  float SDFbox = boxSDF(q, vec3(0.75));

  float objects = min(min(SDFsphere, SDFsphere2), SDFbox);

  return min(objects, planeSDF(pos, vec2(2.0, 2.0), -1.0));
}

vec3 calcNormal(vec3 p) { 
  vec3 n = vec3(0.0);
  for( int i=0; i<4; i++ )
  {
    vec3 e = 0.5773*(2.0*vec3((((i+3)>>1)&1),((i>>1)&1),(i&1))-1.0);
    n += e*scene(p+0.0005*e).x;
    }
    return normalize(n);
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
  float dist = 0.0;

  //Raymarching
  for (int i = 0; i < 80; i++) {
    ray.position = ray.origin + ray.direction * totalDistance;

    dist = scene(ray.position);

    totalDistance += max(dist, 0.001);

    //col = vec3(i) / 80.0;
    col = calcNormal(ray.position);

    if (dist < .001 || dist > 100.0) break;
  }

  //if (dist < 0.001) {
  //  col = vec3(1.0);
  //}
  //else {
  //  col = vec3(0.0);
  //}


  //col = vec3(totalDistance * 0.2);
  //col = pow(col, vec3(1.0 / 2.2)); // Apply gamma correction

  color = vec4(col, 1.0);
}