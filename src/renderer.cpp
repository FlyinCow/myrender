#include "renderer.h"
#include "vector.h"
#include <algorithm>
#include <cmath>

// Bresenham's line drawing
void Renderer::draw_line(Vec2i v0, Vec2i v1, const BmpColor &color)
{
    // vertical line
    if (v0.x == v1.x)
    {
        if (v0.y > v1.y)
            std::swap(v0, v1);
        for (int y = v0.y; y <= v1.y; y++)
            screen.set(v0.x, y, color);
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
            screen.set(x, v0.y, color);
        return;
    }
    float k = (v1.y - v0.y) / (float)(v1.x - v0.x);
    int dy = k > 0 ? 1 : -1;
    int x = v0.x, y = v0.y;
    float yf = v0.y;
    for (; x <= v1.x; x++)
    {
        if (steep)
            screen.set(y, x, color);
        else
            screen.set(x, y, color);
        yf += k;
        if (yf - y >= 0.5 || yf - y <= -.5)
            y += dy;
    }
}

void Renderer::draw_triangle(Vec2i verts[3], const BmpColor &color)
{
    if (verts[0].y > verts[1].y)
        std::swap(verts[0], verts[1]);
    if (verts[0].y > verts[2].y)
        std::swap(verts[0], verts[2]);
    if (verts[1].y > verts[2].y)
        std::swap(verts[1], verts[2]);

    float t1 = 0, t2 = 0;

    for (int y = verts[0].y; y < verts[1].y; y++)
    {
        t1 = (float)(y - verts[0].y) / (verts[1].y - verts[0].y);
        t2 = (float)(y - verts[0].y) / (verts[2].y - verts[0].y);
        draw_line({(verts[0] + t1 * (verts[1] - verts[0])).x, y}, {(verts[0] + t2 * (verts[2] - verts[0])).x, y}, color);
    }

    for (int y = verts[1].y; y < verts[2].y; y++)
    {
        t1 = (float)(y - verts[1].y) / (verts[2].y - verts[1].y);
        t2 = (float)(y - verts[0].y) / (verts[2].y - verts[0].y);
        draw_line({(verts[1] + t1 * (verts[2] - verts[1])).x, y}, {(verts[0] + t2 * (verts[2] - verts[0])).x, y}, color);
    }
}

void Renderer::triangle_texture(Vec3i verts[3], Vec2i uv[3], const Model &model)
{
    Vec2i left_bottom = {screen.width() - 1, screen.height() - 1};
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

    BmpColor color;

    for (int i = left_bottom.x; i < right_top.x; i++)
    {
        for (int j = left_bottom.y; j < right_top.y; j++)
        {
            u.z = verts[0].x - i;
            v.z = verts[0].y - j;
            Vec3i w = u ^ v;
            // if w.x, w.y && w.z are not all >=0 or all <=0
            if (!((w.x >= 0 && w.y >= 0 && w.z >= 0) || (w.x <= 0 && w.y <= 0 && w.z <= 0)))
                continue;
            if (!(std::abs(w.x + w.y) <= std::abs(w.z)))
                continue;
            float z = 0;
            for (int k = 0; k < 3; k++)
                z += verts[k].z * w.raw[k];
            z /= (float)(w.x + w.y + w.z);
            if (zbuffer[i * screen.width() + j] < z)
            {
                int uvx = 0, uvy = 0;
                for (int k = 0; k < 3; k++)
                {
                    uvx += uv[k].x * w.raw[k];
                    uvy += uv[k].y * w.raw[k];
                }
                uvx /= (w.x + w.y + w.z);
                uvy /= (w.x + w.y + w.z);
                zbuffer[i * screen.width() + j] = z;
                screen.set(i, j, model.diffuse(uvx, uvy));
            }
        }
    }
}

Vec3i Renderer::screen_coord(const Vec3f &world_coord)
{
    return {(world_coord.x + 1.) * (screen.width() - 1) / 2, (world_coord.y + 1.) * (screen.height() - 1) / 2, (world_coord.z + 1.) * (screen.width() - 1) / 2};
}

void Renderer::draw_model(const Model &model)
{
    Vec3f light = {0, 0, -1};
    Vec2i uv[3];

    for (int i = 0; i < model.fcount(); i++)
    {
        Vec3i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            screen_coords[j] = screen_coord(model.vert(model.face(i).raw[j]));
            world_coords[j] = model.vert(model.face(i).raw[j]);
        }
        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        float intensity = n * (1 / std::sqrt(n * n)) * light;
        if (intensity >= 0)
        {
            for (int k = 0; k < 3; k++)
                uv[k] = model.uv(i, k);
            triangle_texture(screen_coords, uv, model);
        }
    }
}

void Renderer::dump(const char *filename)
{
    screen.write(filename);
}

Renderer::Renderer(int width, int height)
{
    zbuffer = new float[width * height];
    screen = BmpImage(width, height, BmpImage::RGB);
}

Renderer::~Renderer()
{
    delete[] zbuffer;
}