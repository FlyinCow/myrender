#ifndef _BMPIMAGE_H_
#define _BMPIMAGE_H_

#include <cstring>

#pragma pack(push, 1)
struct BmpFileHeader
{
    /**
     * BMP file type.
     * "BM" -- Windows
     * "BA" -- OS/2 Bitmap Array
     * "CI" -- OS/2 Color Icon
     * "CP" -- OS/2 Color Pointer
     * "IC" -- OS/2 Icon
     * "PT" -- OS/2 Pointer
     */
    char type[2];
    int size;                   /*Image size counted by Bytes.*/
    unsigned char researved[4]; /*Zeros.*/
    int offset = 54;            /*Offset of color data. (14 + 40) when there's no color palette.*/
};

struct BmpInfoHeader
{
    int header_size;     /*BMP info header size counted by Byte.*/
    int width;           /*Image width.*/
    int height;          /*Image height. If negtive, the origin point is at the left-bottom corner.*/
    short planes;        /*#color planes. Always 1.*/
    short bitcount;      /*Bitcount, 32 is 8bit per channel include alpha.*/
    int compression;     /*Compression type. 0 for no compression.*/
    int img_size;        /*Image size counted by Bytes. Could set to 0 with compresstion set to 0.*/
    int x_ppm;           /*Horizontal pixels per meter*/
    int y_ppm;           /*Vertical pixels per meter*/
    int color_used;      /*#colors used in color palette. 0 for using all colors.*/
    int color_important; /*#colors important for the image. 0 for all are important.*/
};
#pragma pack(pop)

struct BmpColor
{
    union
    {
        struct
        {
            unsigned char b;
            unsigned char g;
            unsigned char r;
            unsigned char a;
        };
        unsigned char raw[4]; // Bytes
        unsigned int value;   // Hex
    };

    BmpColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : b(b), g(g), r(r), a(a) {}
    BmpColor(unsigned int v) : value(v){};
    BmpColor(const BmpColor &color) : value(color.value){};
    BmpColor &operator=(const BmpColor &color)
    {
        if (this != &color)
            value = color.value;
        return *this;
    };
};

class BmpImage
{
private:
    int width;
    int height;
    char *data; // colors
    int bpp;    // byte per pixel
    int size;   // size of colors

    int index(int x, int y);

public:
    enum
    {
        RGB = 3,
        RGBA = 4
    };
    BmpImage();
    BmpImage(int width, int height, int bpp);
    BmpImage(const BmpImage &image);
    ~BmpImage();
    int set(int x, int y, BmpColor color);
    // Construct BmpColor inplace instead.
    // int set(int x, int y, int value);
    // int set(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    int write(const char *filename);
    int read(const char *filename);
    void clear();
    int create(int width, int height, int bpp);
};

#endif // _BMPIMAGE_H_