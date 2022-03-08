#include "png.hpp"
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
                     const uint8_t* pixels, 
                     uint32_t width,
                     uint32_t height, 
                     uint32_t bpp,
                     int32_t pitch) {
    LuImage image;
    image.width    = width;
    image.height   = height;
    image.depth    = 8;
    image.channels = bpp;

    std::vector<uint8_t> tmp_buf;    
    if (pitch != (int32_t)(width * bpp)) {
      tmp_buf.resize(width * height * bpp);
      auto src_row = pixels;
      auto dst_ptr = tmp_buf.data();
      for (uint32_t y = 0; y < height; ++y) {
        auto src_ptr = src_row;
        for (uint32_t x = 0; x < width; ++x) {
          memcpy(dst_ptr, src_ptr, bpp);
          src_ptr += bpp;
          dst_ptr += bpp;
        }
        src_row += pitch;
      }
      image.data = (uint8_t*)tmp_buf.data();
    } else {    
      image.data = (uint8_t*)pixels;
    }

    return luPngWriteFile(filename, &image);            
}