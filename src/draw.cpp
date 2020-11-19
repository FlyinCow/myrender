#include "draw.h"
#include "vector.h"
#include <algorithm>

// Bresenham's line drawing
// todo: remove floating number
void line(Vec2i v0, Vec2i v1, BmpImage &img, const BmpColor &color)
{
    // vertical line
    if (v0.x == v1.x)
    {
        if (v0.y > v1.y)
            std::swap(v0, v1);
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

void triangle(Vec2i v0, Vec2i v1, Vec2i v2, BmpImage &image, const BmpColor &color)
{
    if (v0.y > v1.y)
        std::swap(v0, v1);
    if (v0.y > v2.y)
        std::swap(v0, v2);
    if (v1.y > v2.y)
        std::swap(v1, v2);

    line(v0, v1, image, color);
    line(v1, v2, image, color);
    line(v0, v2, image, color);

    float t1 = 0, t2 = 0;

    for (int y = v0.y; y < v1.y; y++)
    {
        t1 = (float)(y - v0.y) / (v1.y - v0.y);
        t2 = (float)(y - v0.y) / (v2.y - v0.y);
        line({(v0 + t1 * (v1 - v0)).x, y}, {(v0 + t2 * (v2 - v0)).x, y}, image, color);
    }

    for (int y = v1.y; y < v2.y; y++)
    {
        t1 = (float)(y - v1.y) / (v2.y - v1.y);
        t2 = (float)(y - v0.y) / (v2.y - v0.y);
        line({(v1 + t1 * (v2 - v1)).x, y}, {(v0 + t2 * (v2 - v0)).x, y}, image, color);
    }
}

void triangle(Vec2i verts[3], BmpImage &img, const BmpColor &color)
{
    Vec2i left_bottom = {img.width() - 1, img.height() - 1};
    Vec2i right_top = {0, 0};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            left_bottom.raw[j] = std::min(left_bottom.raw[j], verts[i].raw[j]);
            right_top.raw[j] = std::max(right_top.raw[j], verts[i].raw[j]);
        }
    }

    Vec3i u = {verts[1].x - verts[0].x, verts[2].x - verts[0].x, 0};
    Vec3i v = {verts[1].y - verts[0].y, verts[2].y - verts[0].y, 0};

    for (int i = left_bottom.x; i < right_top.x; i++)
    {
        for (int j = left_bottom.y; j < right_top.y; j++)
        {
            u.z = verts[0].x - i;
            v.z = verts[0].y - j;
            Vec3i w = u.cross(v);
            // if w.x, w.y && w.z are not all >=0 or all <=0
            if (!((w.x >= 0 && w.y >= 0 && w.z >= 0) || (w.x <= 0 && w.y <= 0 && w.z <= 0)))
                continue;
            if (std::abs(w.x + w.y) <= std::abs(w.z))
                img.set(i, j, color);
        }
    }
}