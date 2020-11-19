#include "bmpimage.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <cassert>

BmpImage::BmpImage() : w(0), h(0), data(nullptr), bpp(0) {}
BmpImage::BmpImage(int width, int height, int bpp = 3) : w(width), h(height), bpp(bpp)
{
    data = new char[w * h * bpp];
    std::memset(data, 0, w * h * bpp);
}

BmpImage::BmpImage(const BmpImage &image) : BmpImage(image.w, image.h, image.bpp)
{
    if (data != nullptr)
        delete[] data;
    data = new char[w * h * bpp];
    std::memcpy(data, image.data, w * h * bpp);
}

BmpImage &BmpImage::operator=(const BmpImage &image)
{
    w = image.w;
    h = image.h;
    bpp = image.bpp;
    if (data != nullptr)
        delete[] data;
    data = new char[w * h * bpp];
    std::memcpy(data, image.data, w * h * bpp);
    return *this;
}

BmpImage::~BmpImage()
{
    if (data)
        delete[] data;
}

inline int BmpImage::index(int x, int y) const
{
    int i = y * w + x;
    assert(i < w * h);
    return i;
}

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

    // Read headers.
    BmpFileHeader file_header;
    in.read(reinterpret_cast<char *>(&file_header), sizeof(BmpFileHeader));
    BmpInfoHeader info_header;
    in.read(reinterpret_cast<char *>(&info_header), sizeof(BmpInfoHeader));

    // Check Type, offset, color depth, color palette.
    if (std::strncmp(file_header.type, "BM", 2) != 0)
    {
        std::cerr << "Type unsupported: " << file_header.type[0] << file_header.type[1] << std::endl;
        in.close();
        return -1;
    }
    if (file_header.offset != (14 + info_header.header_size))
    {
        std::cerr << "Color palette unsupported: Offset(" << file_header.offset << ") != header size(" << sizeof(file_header) << "+" << info_header.header_size << ")" << std::endl;
        in.close();
        return -1;
    }
    w = info_header.width;
    h = info_header.height;
    if (info_header.bitcount == 24)
        bpp = RGB;
    else if (info_header.bitcount == 32)
        bpp = RGBA;
    else
    {
        std::cerr << "Color depth unsupported: bbp is " << info_header.bitcount << std::endl;
        in.close();
        return -1;
    }
    if (info_header.compression != 0)
    {
        std::cerr << "Compresion unsupported." << std::endl;
    }

    // Copy colors.
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
    file_header.size = w * h * bpp + 54;
    out.write(reinterpret_cast<const char *>(&file_header), sizeof(BmpFileHeader));

    // info header
    BmpInfoHeader info_header;
    info_header.header_size = 40; // use bmp v3
    std::memset(&info_header, 0, sizeof(BmpInfoHeader));
    info_header.width = w;
    info_header.height = h;
    info_header.bitcount = bpp * 8;
    info_header.header_size = sizeof(BmpInfoHeader);
    info_header.planes = 1;
    out.write(reinterpret_cast<const char *>(&info_header), sizeof(BmpInfoHeader));

    // colors
    out.write(data, w * h * bpp);

    out.close();
    return w * h;
}

void BmpImage::set(int x, int y, BmpColor color)
{
    memcpy(data + index(x, y) * bpp, color.raw, bpp);
}

BmpColor BmpImage::at(int x, int y) const
{
    BmpColor color;
    memcpy(color.raw, data + index(x, y) * bpp, bpp);
    if (bpp == RGB)
        color.a = 0xff;
    return color;
}

void BmpImage::clear()
{
    if (data != nullptr)
    {
        delete[] data;
        data = nullptr;
    }
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
    data = new char[w * h * bpp];
    return w * h * bpp;
}

int BmpImage::width() const { return w; }
int BmpImage::height() const { return h; }
int BmpImage::size() const { return w * h; }