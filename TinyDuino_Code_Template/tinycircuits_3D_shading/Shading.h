#include <stdio.h>
#include <math.h>
#include "vectors.h"

//#####################
//### Shading Magic ###
//#####################

// Vector points TO the camera
const VEC CAMVEC = {0, -1, 0};
// Vector points TO the light (?, ?, ?). It's messed up cuz i messed up the rotation matrix

VEC GLOBAL_ILLUMINATION = {-1, 1, 1};

const VEC ka = <<PHONG_AMBIANCE_COL>>;
const float ia = <<PHONG_AMBIANCE_INTENSITY>>;

const VEC kd = <<PHONG_DIFFUSE_COL>>;
const float id = <<PHONG_DIFFUSE_INTENSITY>>;

const VEC ks = <<PHONG_SPECTRAL_COL>>;
const float is = <<PHONG_SPECTRAL_INTENSITY>>;
const float alp = <<SHINYNESS>>;

VEC GetNormal(VEC side1, VEC side2, VEC cam){
  VEC normal = normalize(cross(side1, side2));
  if (dot(normal, cam) < 0){normal = scale(normal, -1);}
  return normal;
}

VEC GetLightReflection(VEC light, VEC N){
  return reflect(normalize(light), N);
}

VEC GetPhong(VEC L, VEC N, VEC cam, VEC R){
  VEC col = scale(ka, ia); //AMBIENCE

  if (dot(N, L) > 0){
    col = sum(col, scale(scale(kd, dot(normalize(L), N)), id)); //DIFFUSE
    col = sum(col, scale(scale(ks, pow(dot(R, normalize(cam)), alp)), is)); //spectral
  }
  return clamp(col); 
}

VEC GetPhongFromTriangle(VEC p1, VEC p2, VEC p3, VEC L, VEC cam){
  VEC side1 = sum(p1, scale(p2, -1));
  VEC side2 = sum(p2, scale(p3, -1));
  VEC N = GetNormal(side1, side2, cam);
  VEC R = GetLightReflection(L, N);
  return GetPhong(L, N, cam, R);
}

unsigned char VEC_to_8bit(VEC col){
  // 8Bit: BBBGGGRR
  return (unsigned char)floor(col.i*4-0.1) + (unsigned char)floor(col.j*8-0.1)*4 + (unsigned char)floor(col.k*8-0.1)*4*8;
}
