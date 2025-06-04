#version 460

uniform float time;
uniform vec3 sColor;
uniform vec2 resolution;

uniform vec2 mousePosition;

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

  //q.xy *= rot2D(time);
  //q.xz *= rot2D(time);
  //q.zy *= rot2D(time);

  float SDFbox = boxSDF(pos, vec3(1.0));

  float objects = min(min(SDFsphere, SDFsphere2), SDFbox);

  return min(objects, planeSDF(pos, vec2(5.0, 5.0), -1.0));
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

float shadow(vec3 point, vec3 dir, float start, float end) {
 	float depth = start, dist;
  float shadow = 1.0;
  for(int i = 0; i < 256; ++i) {
    dist = scene(point + depth*dir).x;
    if(depth > end || shadow < -1.0)
      break;
        
    shadow = min(shadow, 40.*dist/depth);
    depth += clamp(dist, 0.005, 10.0);
  }
    
  shadow = max(shadow, -1.0);
  return smoothstep(-1.0, 0.0, shadow * 0.85);
}

float AO(vec3 point, vec3 dir, float start) {
	float depth = start, dist;
    float step = 0.05, falloff = 1.0;
    float ao = 0.0;
    for(int i = 0; i < 3; ++i) {
        dist = scene(point + depth*dir).x;
        ao += falloff*clamp(depth - dist, 0.0, 1.0);
        depth += step;
        falloff *= 0.9;
    }
    
    return clamp(1.0 - 1.5*ao, 0.0, 1.0);
}

vec3 getRayDir(vec2 uv) {
  vec2 p = uv * 2.0 - 1.0;
  p.x *= resolution.x / resolution.y;
  return normalize(vec3(p, 1.0));
}

void main() {
  //Initialization
  Ray ray;
  ray.origin = vec3(0.0, 0.0, -6.0);
  ray.direction = getRayDir(UV);

  vec2 mousePos = (mousePosition * 2.0 - resolution) / resolution.y;
  mousePos.y = clamp(float(mousePos.y), -0.8, 0.8);

  ray.origin.yz *= rot2D(-mousePos.y * 2.0);
  ray.direction.yz *= rot2D(-mousePos.y * 2.0);

  ray.origin.xz *= rot2D(-mousePos.x * 2.0);
  ray.direction.xz *= rot2D(-mousePos.x * 2.0);

  vec3 col = vec3(0.0);
  float totalDistance = 0.0;
  float dist = 0.0;

  //Raymarching
  for (int i = 0; i < 256; i++) {
    ray.position = ray.origin + ray.direction * totalDistance;

    dist = scene(ray.position);

    totalDistance += max(dist, 0.001);

    vec3 lightDir = vec3(-20.0, 40.0, -20.0) - ray.position;
    float lightDist = length(lightDir);
    lightDir /= lightDist;

    if (dist < 0.001) {
      vec3 normal = calcNormal(ray.position);
      vec3 shadowStart = ray.position + normal * 0.001;

      col = sColor * shadow(shadowStart, lightDir, 0.01, lightDist);
      col *= AO(ray.position, calcNormal(ray.position), 0.1);
    }

    if (dist < .001 || dist > 100.0) break;
  }

  col = pow(col, vec3(1.0 / 2.2));

  float fog = 1.0 - exp(-totalDistance * 0.05);
  col = mix(col, vec3(15.0 / 255.0), fog);

  if (dist > 0.001) {
    col = vec3(15.0 / 255.0);
  }

  color = vec4(col, 1.0);
}