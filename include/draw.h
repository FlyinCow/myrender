#ifndef _DRAW_H_
#define _DRAW_H_

#include "vector.h"
#include "bmpimage.h"

void line(Vec2i v0, Vec2i v1, BmpImage &img, const BmpColor &color);

#endif // _DRAW_H_