#include "bmpimage.h"
#include <fstream>
#include <cstring>
#include <iostream>

BmpImage::BmpImage() : w(0), h(0), data(nullptr), bpp(0) {}
BmpImage::BmpImage(int width, int height, int bpp = 3) : w(width), h(height), bpp(bpp)
{
    size = width * height * bpp;
    data = new char[size];
    std::memset(data, 0, size);
}

BmpImage::BmpImage(const BmpImage &image) : BmpImage(image.w, image.h, image.bpp)
{
    std::memcpy(data, image.data, size);
}

BmpImage::~BmpImage()
{
    if (data)
        delete[] data;
}

inline int BmpImage::index(int x, int y)
{
    int i = (y * w + x) * bpp;
    if (i >= size)
        return -1;
    return i;
}

// todo: test this
int BmpImage::read(const char *filename)
{
    if (data != nullptr)
    {
        std::cerr << "Call clear first." << std::endl;
        return -1;
    }
    std::fstream in;
    in.open(filename, std::ios::in | std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        in.close();
        return -1;
    }
    BmpFileHeader file_header;
    in.read(reinterpret_cast<char *>(&file_header), sizeof(BmpFileHeader));
    if (std::strncmp(file_header.type, "BM", 2) != 0)
    {
        std::cerr << "Type unsupported: " << file_header.type[0] << file_header.type[1] << std::endl;
        in.close();
        return -1;
    }
    if (file_header.offset != 54)
    {
        std::cerr << "Bad offset: " << file_header.offset << ". Color palette unsupported." << std::endl;
        in.close();
        return -1;
    }
    BmpInfoHeader info_header;
    in.read(reinterpret_cast<char *>(&info_header), sizeof(BmpInfoHeader));
    w = info_header.width;
    h = info_header.height;
    if (info_header.bitcount == 24)
        bpp = 3;
    else if (info_header.bitcount == 32)
        bpp = 4;
    else
    {
        std::cerr << "Color depth unsupported: bbp is " << info_header.bitcount << std::endl;
        in.close();
        return -1;
    }
    size = w * h * bpp;
    if (info_header.compression != 0)
    {
        std::cerr << "Compresion unsupported." << std::endl;
    }
    int nbytes = file_header.size - file_header.offset /*width * height * bpp*/;
    data = new char[nbytes];
    in.seekg(file_header.offset);
    in.read(data, nbytes);
    if (!in.good())
    {
        std::cerr << "Error reading data." << std::endl;
        in.close();
        return -1;
    }
    return nbytes;
}

int BmpImage::write(const char *filename)
{
    if (data == nullptr)
        return 0;
    std::fstream out;
    out.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        out.close();
        return -1;
    }

    // file header
    BmpFileHeader file_header;
    std::memcpy(file_header.type, "BM", 2);
    file_header.offset = 54;
    std::memset(file_header.researved, 0, 4);
    file_header.size = size + 54;
    out.write(reinterpret_cast<const char *>(&file_header), sizeof(BmpFileHeader));

    // info header
    BmpInfoHeader info_header;
    std::memset(&info_header, 0, sizeof(BmpInfoHeader));
    info_header.width = w;
    info_header.height = h;
    info_header.bitcount = bpp * 8;
    info_header.header_size = sizeof(BmpInfoHeader);
    info_header.planes = 1;
    out.write(reinterpret_cast<const char *>(&info_header), sizeof(BmpInfoHeader));

    // colors
    out.write(data, size);

    out.close();
    return size;
}

int BmpImage::set(int x, int y, BmpColor color)
{
    int i = index(x, y);
    if (-1 == i)
        return -1;
    memcpy(data + i, color.raw, bpp);
    return 0;
}

void BmpImage::clear()
{
    if (data != nullptr)
    {
        delete[] data;
        data = nullptr;
    }
    size = 0;
    w = 0;
    bpp = 0;
    h = 0;
}

int BmpImage::create(int width, int height, int bpp = 3)
{
    if (data != nullptr)
        return -1;
    this->w = width;
    this->h = height;
    this->bpp = bpp;
    this->size = width * height * bpp;
    data = new char[size];
    return size;
}

int BmpImage::width() const { return w; };
int BmpImage::height() const { return h; }
