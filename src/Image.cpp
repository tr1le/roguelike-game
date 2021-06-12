#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

Pixel operator- (const Pixel& lhs, const Pixel& rhs) {
    return {uint8_t(lhs.r - rhs.r), uint8_t(lhs.g - rhs.g), uint8_t(lhs.b - rhs.b), lhs.a};
}

Pixel operator* (const Pixel& lhs, double rhs) {
    return {uint8_t(lhs.r * rhs), uint8_t(lhs.g * rhs), uint8_t(lhs.b * rhs), lhs.a};
}


Image::Image(const std::string &a_path) {
    if ((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, sizeof(Pixel))) != nullptr) {
        size = width * height * channels;
    }
}

Image::Image(int a_width, int a_height, int a_channels) {
    data = new Pixel[a_width * a_height ]{};

    if (data != nullptr) {
        width = a_width;
        height = a_height;
        size = a_width * a_height * a_channels;
        channels = a_channels;
        self_allocated = true;
    }
}

Image::Image(const Image& img, int x, int y, int a_width, int a_height, int a_channels) {
    data = new Pixel[a_width * a_height]{};

    if (data != nullptr) {
        width = a_width;
        height = a_height;
        size = a_width * a_height * a_channels;
        channels = a_channels;
        self_allocated = true;
    }

    PutImage(0, 0, img.GetImage(x, y, a_width, a_height));
}


int Image::Save(const std::string &a_path) {
    auto extPos = a_path.find_last_of('.');
    if (a_path.substr(extPos, std::string::npos) == ".png" ||
            a_path.substr(extPos, std::string::npos) == ".PNG") {
        stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
    } else if (a_path.substr(extPos, std::string::npos) == ".jpg" ||
            a_path.substr(extPos, std::string::npos) == ".JPG" ||
            a_path.substr(extPos, std::string::npos) == ".jpeg" ||
            a_path.substr(extPos, std::string::npos) == ".JPEG") {
        stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
    } else {
        std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
        return 1;
    }
    return 0;
}

Pixel Image::AddPixel(const Pixel& orig, const Pixel& add, int channels) {
    Pixel res = orig;
    double alpha = add.a / 256.0;
    res.r = res.r * (1 - alpha) + add.r * alpha;
    res.g = res.g * (1 - alpha) + add.g * alpha;
    res.b = res.b * (1 - alpha) + add.b * alpha;
    if (channels > 3) {
        res.a = std::max(orig.a, add.a);
    } else {
        res.a = 255;
    }
    return res;
}

void Image::PutImage(int x, int y, const Image& img) {
    for (int xx = 0; xx < img.Width(); ++xx) {
        for (int yy = 0; yy < img.Height(); ++yy) {
            if (x + xx >= 0 && y + yy >= 0)
            PutPixel(x + xx, y + yy, AddPixel(GetPixel(x + xx, y + yy), img.GetPixel(xx, yy), Channels()));
        }
    }
}

void Image::PutMirror(int x, int y, const Image& img) {
    for (int xx = 0; xx < img.Width(); ++xx) {
        for (int yy = 0; yy < img.Height(); ++yy) {
            PutPixel(x + xx, y + yy, AddPixel(GetPixel(x + xx, y + yy), img.GetPixel(img.Width() - xx - 1, yy), Channels()));
        }
    }
}

Image Image::GetImage(int x, int y, int width, int height) const {
    Image res(width, height, 4);
    for (int xx = 0; xx < width; ++xx) {
        for (int yy = 0; yy < height; ++yy) {
            res.PutPixel(xx, yy, GetPixel(x + xx, y + yy));
        }
    }
    return res;
}

Image::~Image()
{
    if (self_allocated) {
        delete [] data;
    } else {
        stbi_image_free(data);
    }
}