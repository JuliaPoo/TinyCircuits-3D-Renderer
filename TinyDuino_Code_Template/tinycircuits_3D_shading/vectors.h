#include <stdio.h>
#include <math.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

//####################
//### VECTOR MAGIC ###
//####################

typedef struct{
  float i,j,k;
}VEC;

float dot(VEC a, VEC b){
  return a.i*b.i+a.j*b.j+a.k*b.k;
}
 
VEC cross(VEC a, VEC b){
  VEC c = {a.j*b.k - a.k*b.j, a.k*b.i - a.i*b.k, a.i*b.j - a.j*b.i};
  return c;
}

VEC scale(VEC a, float x){
  a.i *= x; a.j *= x; a.k *= x;
  return a; 
}

float length(VEC a){
  return sqrt(dot(a,a));
}

VEC normalize(VEC a){
  return scale(a, 1./length(a));
}

VEC sum(VEC a, VEC b){
  return (VEC){a.i+b.i, a.j+b.j, a.k+b.k};
}

VEC reflect(VEC I, VEC N){
  // Both I and N are pointing from the same point
  // Output points from same point too
  return sum(scale(N, 2.*dot(I,N)), scale(I, -1));
}

VEC clamp(VEC a){
  a.i = MIN(MAX(a.i,0),1);
  a.j = MIN(MAX(a.j,0),1);
  a.k = MIN(MAX(a.k,0),1);
  return a;
}

void printVEC(VEC a){
  printf("( %f, %f, %f)", a.i, a.j, a.k);
}
