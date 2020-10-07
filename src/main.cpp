#include "bmpimage.h"

const BmpColor white = BmpColor(255, 255, 255, 255);
const BmpColor red = BmpColor(255, 0, 0, 255);

int main(int argc, char **argv)
{
	BmpImage image(100, 100, BmpImage::RGB);
	for (int i = 0; i < 100; i++)
	{
		image.set(i, 99 - i, white);
		image.set(i, i, red);
	}
	image.write("test.bmp");
	return 0;
}
