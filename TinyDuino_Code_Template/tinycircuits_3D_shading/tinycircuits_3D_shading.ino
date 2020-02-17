#include <TinyScreen.h>

#include <stdio.h>
#include <stdlib.h>
#include "Shading.h"

TinyScreen display = TinyScreen(TinyScreenDefault);

#define BACKGROUND_COL <<BACKGROUND_COL>>
#define N_TRIANGLES <<N_TRIANGLES>>
const char SPRITE[N_TRIANGLES*9] PROGMEM = <<SPRITE>>;

const unsigned char Rx = 96;
const unsigned char Ry = 64;
#define SCALE <<SCALE>>

float a2 = 1.57;
const VEC LG = GLOBAL_ILLUMINATION;
const VEC CAM = CAMVEC;

const float pixelscale = Rx*SCALE;

typedef struct {
  char c;
  unsigned char ind1;
  unsigned char ind2;
} TriangleZ;

TriangleZ ZList[N_TRIANGLES];

void setup() {
  display.begin();
  display.setBrightness(15);
  DisplayFrame();
}

void loop() {
  if (display.getButtons(TSButtonUpperRight)){a2 += 0.3; DisplayFrame(); delay(100);}
  if (display.getButtons(TSButtonUpperLeft )){a2 -= 0.3; DisplayFrame(); delay(100);}
}

void GetZList(){
  int i;
  float c2 = cos(a2);
  float s2 = sin(a2);
  for (i = 0; i < N_TRIANGLES; i++){
    
    VEC p1 = {(float)(signed char)pgm_read_byte_near(SPRITE + 9*i    ), (float)(signed char)pgm_read_byte_near(SPRITE + 9*i + 1), 0};
    VEC p2 = {(float)(signed char)pgm_read_byte_near(SPRITE + 9*i + 3), (float)(signed char)pgm_read_byte_near(SPRITE + 9*i + 4), 0};
    VEC p3 = {(float)(signed char)pgm_read_byte_near(SPRITE + 9*i + 6), (float)(signed char)pgm_read_byte_near(SPRITE + 9*i + 7), 0};
    
    ZList[i].c = -s2*p1.i - c2*p1.j - s2*p2.i - c2*p2.j - s2*p3.i - c2*p3.j;
    
    ZList[i].ind1 = i%256;
    ZList[i].ind2 = floor(i/256);
  }
}

void swap_Z(TriangleZ *a, TriangleZ *b){
   TriangleZ t;
   t  = *b;
   *b = *a;
   *a = t;
}

int sort(TriangleZ* arr, int size){
  int i, j, temp;
  void *p1,  *p2;
  for (i = 0; i < size; i++){
    for (j = i + 1; j < size; j++){
      if (arr[i].c > arr[j].c){
        p1 = &arr[i];
        p2 = &arr[j];
        swap_Z(p1, p2);
      }
    }
  }
  return 0;
}

void GetsortZList(){
  GetZList();
  sort(ZList, N_TRIANGLES);
}

void DisplayFrame(){
  GetsortZList();
  int i, index;
  unsigned char col;
  
  float c2 = cos(a2);
  float s2 = sin(a2);
  VEC L_temp = {c2*LG.i - s2*LG.j, LG.k, -s2*LG.i - c2*LG.j};
  VEC cam_temp = {CAM.i, CAM.k, -CAM.j}; // Pretty buggy. Might have to completely redo the rotation matrix
    
  display.drawRect(0,0,96,64,TSRectangleFilled, BACKGROUND_COL);
  float X1, Y1, X2, Y2, X3, Y3;
  for (i = 0; i < N_TRIANGLES; i++){
    
    index = ZList[i].ind1 + ZList[i].ind2*256;

    VEC p1 = {(float)(signed char)pgm_read_byte_near(SPRITE + 9*index    ), (float)(signed char)pgm_read_byte_near(SPRITE + 9*index + 1), (float)(signed char)pgm_read_byte_near(SPRITE + 9*index + 2)};
    VEC p2 = {(float)(signed char)pgm_read_byte_near(SPRITE + 9*index + 3), (float)(signed char)pgm_read_byte_near(SPRITE + 9*index + 4), (float)(signed char)pgm_read_byte_near(SPRITE + 9*index + 5)};
    VEC p3 = {(float)(signed char)pgm_read_byte_near(SPRITE + 9*index + 6), (float)(signed char)pgm_read_byte_near(SPRITE + 9*index + 7), (float)(signed char)pgm_read_byte_near(SPRITE + 9*index + 8)};
    
    X1 = c2*p1.i - s2*p1.j;
    Y1 = p1.k;
    X2 = c2*p2.i - s2*(signed char)p2.j;
    Y2 = p2.k;
    X3 = c2*p3.i - s2*(signed char)p3.j;
    Y3 = p3.k;
    
    X1 = (pixelscale * X1) + Rx/2;
    Y1 = (pixelscale * Y1) + Ry/2;
    X2 = (pixelscale * X2) + Rx/2;
    Y2 = (pixelscale * Y2) + Ry/2;
    X3 = (pixelscale * X3) + Rx/2;
    Y3 = (pixelscale * Y3) + Ry/2;
    
    col = VEC_to_8bit(GetPhongFromTriangle(p1, p2, p3, L_temp, cam_temp));

    FillTriangle((unsigned char)X1, (unsigned char)Y1, (unsigned char)X2, (unsigned char)Y2, (unsigned char)X3, (unsigned char)Y3, col);
  }
}
