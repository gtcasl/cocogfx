#include "png.h"
#include "lupng.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace cocogfx;

int cocogfx::LoadPNG(const char *filename, 
                     std::vector<uint8_t> &pixels, 
                     uint32_t *width,
                     uint32_t *height,
                     uint32_t *bpp) {
    auto image = luPngReadFile(filename, NULL);
    if (image == NULL)
      return -1;
    
    if (image->depth != 8 
      || (image->channels != 3 
       && image->channels != 4)) {
      luImageRelease(image, NULL);
      std::cerr << "invalid png file format!" << std::endl;  
      return -1;
    }
    
    pixels.resize(image->channels * image->width * image->height);
    memcpy(pixels.data(), image->data, pixels.size());
    *width  = image->width;
    *height = image->height;
    *bpp    = image->channels;
    luImageRelease(image, NULL);

    return 0;
}

int cocogfx::SavePNG(const char *filename, 
                     const std::vector<uint8_t> &pixels, 
                     uint32_t width,
                     uint32_t height, 
                     uint32_t bpp) {
    LuImage image;
    image.width    = width;
    image.height   = height;
    image.depth    = 8;
    image.channels = bpp;
    image.data     = (uint8_t*)pixels.data();
    return luPngWriteFile(filename, &image);            
}