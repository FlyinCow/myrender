#include "model.h"
#include "bmpimage.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

Model::Model(const char *filename)
{
    open(filename);
}

int Model::open(const char *filename)
{
    std::ifstream in;
    in.open(filename);
    if (!in.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        in.close();
        return -1;
    }
    std::string line;
    char ctrash;
    int itrash;
    float ftrash;

    while (!in.eof())
    {
        std::getline(in, line);
        auto ss = std::istringstream(line);
        Vec3f v_;
        Vec3i f_;
        Vec2f vt_;
        Vec3i uv_;
        if (!line.compare(0, 2, "v "))
        {
            // v x y z
            ss >> ctrash >> v_.x >> v_.y >> v_.z;
            v.push_back(v_);
        }
        else if (!line.compare(0, 2, "f "))
        {
            // f x/uvx/i y/uvy/i z/uvz/i
            ss >> ctrash >> f_.x >> ctrash >> uv_.x >> ctrash >> itrash >> f_.y >> ctrash >> uv_.y >> ctrash >> itrash >> f_.z >> ctrash >> uv_.z >> ctrash >> itrash;
            f_ -= {1, 1, 1};
            uv_ -= {1, 1, 1};
            f.push_back(f_);
            tc.push_back(uv_);
        }
        else if (!line.compare(0, 2, "vt"))
        {
            // vt x y z
            ss >> ctrash >> ctrash >> vt_.x >> vt_.y >> ftrash;
            vt.push_back(vt_);
        }
    }

    in.close();
    return 0;
}

size_t Model::vcount() const { return v.size(); }
size_t Model::fcount() const { return f.size(); }
Vec3i Model::face(size_t index) const { return f[index]; }
Vec3f Model::vert(size_t index) const { return v[index]; }
Vec2i Model::uv(size_t iface, size_t ivert) const
{
    auto vt_ = vt[tc[iface].raw[ivert]];
    return {vt_.x * diffusemap.width(), vt_.y * diffusemap.height()};
}

BmpColor Model::diffuse(int u, int v) const
{
    return diffusemap.at(u, v);
}

void Model::load_texture(const char *filename)
{
    diffusemap.read(filename);
}