#include "draw.h"
#include <algorithm>

// Bresenham's line drawing
// todo: remove floating number
void line(Vec2i v0, Vec2i v1, BmpImage &img, const BmpColor &color)
{
    // vertical line
    if (v0.x == v1.x)
    {
        for (int y = v0.y; y <= v1.y; y++)
            img.set(v0.x, y, color);
        return;
    }
    // if from right to left
    bool steep = (std::abs(v1.y - v0.y) > std::abs(v1.x - v0.x));
    if (steep)
    {
        std::swap(v0.x, v0.y);
        std::swap(v1.x, v1.y);
    }
    if (v0.x > v1.x)
    {
        std::swap(v0.x, v1.x);
        std::swap(v0.y, v1.y);
    }
    // horizontal line
    if (v0.y == v1.y)
    {
        for (int x = v0.x; x <= v1.x; x++)
            img.set(x, v0.y, color);
        return;
    }
    float k = (v1.y - v0.y) / (float)(v1.x - v0.x);
    int dy = k > 0 ? 1 : -1;
    int x = v0.x, y = v0.y;
    float yf = v0.y;
    for (; x <= v1.x; x++)
    {
        if (steep)
            img.set(y, x, color);
        else
            img.set(x, y, color);
        yf += k;
        if (yf - y >= 0.5 || yf - y <= -.5)
            y += dy;
    }
}