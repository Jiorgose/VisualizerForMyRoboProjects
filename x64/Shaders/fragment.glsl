#version 460

uniform float time;
uniform vec3 sColor;
uniform float aspectRatio;

uniform vec3 cameraPosition;

in vec2 UV;
out vec4 color;

struct Sphere {
  vec3 center;
  float radius;
};

struct Plane {
  float height;
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

mat3 setCamera(vec3 ro, vec3 ta, float roll) {
    vec3 cw = normalize(ta - ro);
    vec3 cp = vec3(sin(roll), cos(roll), 0.0);
    vec3 cu = normalize(cross(cw, cp));
    vec3 cv = normalize(cross(cu, cw));
    return mat3(cu, cv, cw);
}

vec3 getRayDir(vec2 uv, vec3 forward, vec3 up, vec3 right, float fov) {
  vec2 p = uv * 2.0 - 1.0;
  float z = 1.0 / tan(radians(fov) * 0.5);
  return normalize(p.x * right * aspectRatio + p.y * up + z * forward);
}

float sphere(vec3 p, Sphere s) {
  return length(p - s.center) - s.radius;
}

float plane(vec3 p, Plane plane) {
  return p.y + plane.height;
}

float scene(vec3 p) {
  float minDist = 1e10;
  for (int i = 0; i < sphereCount; ++i) {
    float d = sphere(p, spheres[i]);
    minDist = min(minDist, d);
  }
  return min(minDist, plane(p, Plane(1.0)));
}

float calculateLight(in vec3 ro, in vec3 rd, float mint, float maxt, float w) {
    float res = 1.0;
    float t = mint;
    for (int i = 0; i < 256 && t < maxt; i++) {
        float h = scene(ro + t * rd);
        res = min(res, h / (w * t));
        t += clamp(h, 0.005, 0.50);
        if (res < -1.0) break;
    }
    res = max(res, -1.0);
    return 0.25 * (1.0 + res) * (1.0 + res) * (2.0 - res);
}


void main() {
  spheres[0] = Sphere(vec3(0.0, 1.0, 0.0), 0.5);
  spheres[1] = Sphere(vec3(0.0, 0.0, 0.0), 1.0);

  Sun sun;
  
  //float angle = time * 0.2;
  //sun.direction = normalize(vec3(cos(angle), 0.25, sin(angle)));
  sun.direction = vec3(0.25, 0.25, 0.0);

  Ray ray;

  vec3 origin = cameraPosition;
  mat3 camMat = setCamera(cameraPosition, vec3(0.0, 0.0, 0.0), 0.0);
  ray.direction = getRayDir(UV, camMat[2], camMat[1], camMat[0], 90.0);


  float t = 0.0;
  for (int i = 0; i < 256; ++i) {
    ray.position = origin + ray.direction * t;
    float dist = scene(ray.position);
    if (dist < 0.0001) {
      float shadow = calculateLight(ray.position + 0.05 * sun.direction, sun.direction, 0.05, 5.0, 0.25);

      vec3 litColor = sColor;
      vec3 shadowColor = vec3(0.05, 0.05, 0.1);
    
      vec3 finalColor = mix(shadowColor, litColor, shadow);
      color = vec4(finalColor, 1.0);
      return;
    }

    t += dist;
    if (t > 256.0) break;
  }

  double imguiColor = 15.0 / 255.0;
  color = vec4(imguiColor, imguiColor, imguiColor, 1.0);
  return;
}
