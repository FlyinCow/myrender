#ifndef _MODEL_H_
#define _MODEL_H_

#include "bmpimage.h"
#include "vector.h"
#include <vector>

class Model
{
    std::vector<Vec3f> v;
    std::vector<Vec3i> f;
    std::vector<Vec2f> vt;
    std::vector<Vec3i> tc;
    BmpImage diffusemap;

public:
    Model() = default;
    Model(const char *filename);
    int open(const char *filename);
    size_t vcount() const;
    size_t fcount() const;
    Vec3i face(size_t index) const;
    Vec3f vert(size_t index) const;
    Vec2i uv(size_t iface, size_t ivert) const;
    BmpColor diffuse(int u, int v) const;
    void load_texture(const char *filename);
};

#endif // _MODEL_H_