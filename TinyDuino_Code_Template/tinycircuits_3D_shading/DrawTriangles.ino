void swap(int *a, int *b){
   int t;
   t  = *b;
   *b = *a;
   *a = t;
}

/*
 * FillTriangle is modified from the source code of
 * Adafruit-GFX-Library/Adafruit_GFX.cpp for use in
 * TinyCircuits
 */

void FillTriangle( int x0, int y0,
                   int x1, int y1,
                   int x2, int y2, int color) {
                    
  int a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(&y0, &y1); swap(&x0, &x1);
  }
  if (y1 > y2) {
    swap(&y2, &y1); swap(&x2, &x1);
  }
  if (y0 > y1) {
    swap(&y0, &y1); swap(&x0, &x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    display.drawLine(a, y0, b, y0, color);
    return;
  }

  int
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if(a > b) swap(&a, &b);
    display.drawLine(a, y, b, y, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if(a > b) swap(&a, &b);
    display.drawLine(a, y, b, y, color);
  }
}
