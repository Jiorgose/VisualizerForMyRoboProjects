#version 460

#define pi 4.0 * atan(1.0)

uniform float time;
uniform vec2 resolution;

uniform vec2 mousePosition;

uniform vec3 objectPosition;
uniform vec4 objectRotation;

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

mat3 rotationMatrix(vec3 angles) {
  float sx = sin(angles.x), cx = cos(angles.x);
  float sy = sin(angles.y), cy = cos(angles.y);
  float sz = sin(angles.z), cz = cos(angles.z);

  mat3 rotX = mat3(
    1.0, 0.0, 0.0,
    0.0, cx, -sx,
    0.0, sx, cx
  );
    
  mat3 rotY = mat3(
    cy, 0.0, sy,
    0.0, 1.0, 0.0,
    -sy, 0.0, cy
  );
    
  mat3 rotZ = mat3(
    cz, -sz, 0.0,
    sz, cz, 0.0,
    0.0, 0.0, 1.0
  );

  return rotZ * rotY * rotX;
}

mat3 quatToMat(vec4 q) {
    float x = q.x, y = q.y, z = q.z, w = q.w;
    return mat3(
        1.0 - 2.0*y*y - 2.0*z*z, 2.0*x*y - 2.0*z*w,     2.0*x*z + 2.0*y*w,
        2.0*x*y + 2.0*z*w,     1.0 - 2.0*x*x - 2.0*z*z, 2.0*y*z - 2.0*x*w,
        2.0*x*z - 2.0*y*w,     2.0*y*z + 2.0*x*w,     1.0 - 2.0*x*x - 2.0*y*y
    );
}

float sphereSDF(vec3 pos, Sphere sphere)
{
  return(length(pos - sphere.center) - sphere.radius);
}

float cylinderSDF( vec3 p, float h, float r )
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - vec2(r,h);
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
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

float SDFobject(vec3 pos) {
  mat3 rot = quatToMat(objectRotation);
  vec3 localPos = transpose(rot) * (pos - objectPosition);
  return boxSDF(localPos, vec3(0.75));
}

float SDFplane(vec3 pos) { return boxSDF(pos + vec3(0.0, 101.0, 0.0), vec3(3.99, 100.0, 3.99)); }

float scene(vec3 pos) {
  return min(SDFobject(pos), SDFplane(pos));
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
  return smoothstep(-1.0, 0.0, shadow * 0.75);
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

vec3 col = vec3(0.0);
int samples = 4;
vec2 offsets[4] = vec2[](
  vec2(-0.25, -0.25),
  vec2( 0.25, -0.25),
  vec2(-0.25,  0.25),
  vec2( 0.25,  0.25)
);


void main() {
  for (int s = 0; s < samples; ++s) {
    vec2 jitter = offsets[s] / resolution;
    vec2 uv = UV + jitter;
    //Initialization
    Ray ray;
    ray.origin = vec3(0.0, 0.0, -6.0);
    ray.direction = getRayDir(uv);

    vec2 mousePos = (mousePosition * 2.0 - resolution) / resolution.y;

    ray.origin.yz *= rot2D(-mousePos.y * 2.0);
    ray.direction.yz *= rot2D(-mousePos.y * 2.0);

    ray.origin.xz *= rot2D(-mousePos.x * 2.0);
    ray.direction.xz *= rot2D(-mousePos.x * 2.0);

    vec3 sampleCol = vec3(0.0);
    float totalDistance = 0.0;
    float dist = 0.0;

    //Raymarching
    for (int i = 0; i < 100; i++) {
      ray.position = ray.origin + ray.direction * totalDistance;

      dist = scene(ray.position);

      totalDistance += max(dist, 0.001);

      vec3 lightDir = vec3(-20.0, 40.0, -20.0) - ray.position;
      float lightDist = length(lightDir);
      lightDir /= lightDist;

      if (dist < 0.001) {
        if (SDFobject(ray.position) < 0.001) { sampleCol = vec3(1.0, 0.0, 0.0); }
        else if (SDFplane(ray.position) < 0.001) {
          float scale = 1.0;
          vec2 coords = ray.position.xz * scale;
          float checker = mod(floor(coords.x) + floor(coords.y), 2.0);
          vec3 baseColor1 = vec3(0.8);
          vec3 baseColor2 = vec3(0.2);
          sampleCol = mix(baseColor1, baseColor2, checker);
        }

        vec3 normal = calcNormal(ray.position);
        vec3 shadowStart = ray.position + normal * 0.001;

        float spec = pow(max(dot(normal, normalize(lightDir + normalize(-ray.direction))), 0.0), 64.0);

        sampleCol += vec3(1.0) * spec * 0.2;
        //sampleCol *= shadow(shadowStart, lightDir, 0.01, lightDist);
        sampleCol *= max(dot(normal, lightDir), 0.05);
        sampleCol *= AO(ray.position, calcNormal(ray.position), 0.1);
      }
      if (dist < .001 || dist > 100.0) break;
    }
    sampleCol = pow(sampleCol, vec3(1.0 / 2.2));

    float fog = 1.0 - exp(-totalDistance * 0.05);
    sampleCol = mix(sampleCol, vec3(15.0 / 255.0), fog);

    if (dist > 0.001) {
      sampleCol = vec3(15.0 / 255.0);
    }

    col += sampleCol;
  }
  col /= float(samples);
  color = vec4(col, 1.0);
}