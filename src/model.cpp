#include "model.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

Model::Model(const char *filename)
{
    open(filename);
}

// todo: unify
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

    while (!in.eof())
    {
        std::getline(in, line);
        auto ss = std::istringstream(line);
        Vec3f v_;
        Vec3i f_;
        if (!line.compare(0, 2, "v "))
        {
            // v x y z
            ss >> ctrash >> v_.x >> v_.y >> v_.z;
            v.push_back(v_);
        }
        else if (!line.compare(0, 2, "f "))
        {
            // f x/i/i y/i/i z/i/i
            ss >> ctrash >> f_.x >> ctrash >> itrash >> ctrash >> itrash >> f_.y >> ctrash >> itrash >> ctrash >> itrash >> f_.z >> ctrash >> itrash >> ctrash >> itrash;
            f_ -= {1, 1, 1};
            f.push_back(f_);
        }
    }

    in.close();
    return 0;
}

size_t Model::vcount() const { return v.size(); }
size_t Model::fcount() const { return f.size(); }
Vec3i Model::face(size_t index) const { return f[index]; }
Vec3f Model::vert(size_t index) const { return v[index]; }