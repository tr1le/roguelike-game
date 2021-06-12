#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>

constexpr int tileSize = 16;

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

Pixel operator* (const Pixel& lhs, double rhs);

Pixel operator- (const Pixel& lhs, const Pixel& rhs);

constexpr Pixel backgroundColor{0, 0, 0, 0};

struct Image
{
    explicit Image(const std::string &a_path);
    Image(int a_width, int a_height, int a_channels);
    Image(const Image& img, int x, int y, int a_width, int a_height, int a_channels);

    int Save(const std::string &a_path);

    int Width()    const { return width; }
    int Height()   const { return height; }
    int Channels() const { return channels; }
    size_t Size()  const { return size; }
    Pixel* Data()        { return  data; }

    Pixel GetPixel(int x, int y) const { return data[width * y + x]; }
    void  PutPixel(int x, int y, const Pixel &pix) { data[width* y + x] = pix; }
    Pixel AddPixel(const Pixel& orig, const Pixel& add, int channels);

    void PutImage(int x, int y, const Image& img);
    void PutMirror(int x, int y, const Image& img);
    Image GetImage(int x, int y, int width, int height) const;

    ~Image();

private:
    int width = -1;
    int height = -1;
    int channels = 3;
    size_t size = 0;
    Pixel *data = nullptr;
    bool self_allocated = false;
};

#endif //MAIN_IMAGE_H
