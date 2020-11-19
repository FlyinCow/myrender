#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "vector.h"
#include "bmpimage.h"
#include "model.h"

class Renderer
{
private:
    BmpImage screen;
    float *zbuffer;
    void triangle_texture(Vec3i verts[3], Vec2i uv[3], const Model &model);
    Vec3i screen_coord(const Vec3f &world_coord);

public:
    Renderer(int width, int height);
    ~Renderer();
    void draw_line(Vec2i v0, Vec2i v1, const BmpColor &color);
    void draw_triangle(Vec2i verts[3], const BmpColor &color);
    void draw_model(const Model &model);
    void dump(const char *filename);
};

#endif // _RENDERER_H_