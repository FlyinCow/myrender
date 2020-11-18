#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include "vector.h"

class Model
{
    std::vector<Vec3f> v;
    std::vector<Vec3i> f;

public:
    Model() = default;
    Model(const char *filename);
    int open(const char *filename);
    size_t vcount() const;
    size_t fcount() const;
    Vec3i face(size_t index) const;
    Vec3f vert(size_t index) const;
};

#endif // _MODEL_H_