#include "png.hpp"
#include "png.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iostream>

using namespace cocogfx;

static void pngfile_error(png_structp /*png*/, png_const_charp msg) {
	std::cerr << msg << std::endl;
}

int cocogfx::LoadPNG(const char *filename, 
                     std::vector<uint8_t> &pixels, 
                     uint32_t *width,
                     uint32_t *height,
                     uint32_t *bpp) {
  // open file
  auto file = fopen(filename, "rb");
  if (nullptr == file) {
    std::cerr << "couldn't open file: " << filename << "!" << std::endl;
    return -1;
  }

  // check signature
  png_byte pbSig[8];
  if (fread(pbSig, 1, 8, file) != 8
   || !png_check_sig(pbSig, 8)) {
    fclose(file);
    std::cerr << "invalid png file signature!" << std::endl;
    return -1;
	}

  // create png structure
  auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
      (png_error_ptr)pngfile_error, (png_error_ptr)NULL);
	if (!png) {
    fclose(file);
		return -1;
	}

	auto png_info = png_create_info_struct(png);
	if (!png_info) {
		png_destroy_read_struct(&png, NULL, NULL);
    fclose(file);
		return -1;
	}

	png_init_io(png, file);
	png_set_sig_bytes(png, 8);

	// read PNG info
	png_read_info(png, png_info);

	// get width, height, bit-depth and color-type
	int depth, colorType, channels;
	png_uint_32 pwidth, pheight;

	png_get_IHDR(png, png_info, &pwidth, &pheight, &depth, &colorType, NULL, NULL, NULL);
  channels = png_get_channels(png, png_info);

  // adjust depth to 8
  if (depth < 8)
		png_set_expand(png);
	else if (depth == 16)
		png_set_strip_16(png);

	// change palette color into RGB
	if (colorType == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png);

  // if there is a transparency palette, create alpha channel
	if (png_get_valid(png, png_info, PNG_INFO_tRNS))
		png_set_expand(png);

  // use BGR order
	png_set_bgr(png);

  // update info
  png_read_update_info(png, png_info);
  png_get_IHDR(png, png_info, &pwidth, &pheight, &depth, &colorType, NULL, NULL, NULL);
  channels = png_get_channels(png, png_info);

	// allocate a vector of row pointers
  uint32_t pitch = pwidth * channels;
	std::vector<uint8_t*> img_rows(pheight);
	for (uint32_t y = 0; y < pheight; ++y) {
    img_rows.at(y) = new uint8_t[pitch];
  }

	// read the whole image
	png_read_image(png, img_rows.data());
  png_read_end(png, png_info);

	png_destroy_read_struct(&png, &png_info, NULL);

	fclose(file);

  pixels.resize(pwidth * pheight * channels);
  for (uint32_t y = 0; y < pheight; ++y) {
    memcpy(pixels.data() + y * pitch, img_rows.at(y), pitch);
    delete [] img_rows.at(y);
  }

  *width  = pwidth;
	*height = pheight;
  *bpp    = channels;
                       
  return 0;
}

int cocogfx::SavePNG(const char *filename, 
                     const uint8_t* pixels, 
                     uint32_t width,
                     uint32_t height, 
                     uint32_t bpp,
                     int32_t pitch) {
  FILE *file = fopen(filename, "wb");
  if (nullptr == file) {
    std::cerr << "couldn't create file: " << filename << "!" << std::endl;
    return -1;
  }

  auto png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,
      (png_error_ptr)pngfile_error, (png_error_ptr)NULL);
	if (!png) {
    fclose(file);
		return -1;
	}

  auto png_info = png_create_info_struct(png);
  if (!png_info) {
    fclose(file);
		return -1;
  }

	png_init_io(png, file);

	int depth = 8;
	int colortype = (bpp == 1) ? PNG_COLOR_TYPE_GRAY :
		              (bpp == 3) ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGB_ALPHA;

	png_set_IHDR(png, png_info, width, height, depth, colortype,
		PNG_INTERLACE_NONE, 
		PNG_COMPRESSION_TYPE_DEFAULT, 
		PNG_FILTER_TYPE_DEFAULT);

	// write the file header information
	png_write_info(png, png_info);

	// swap the BGR pixels in the DiData structure to RGB
	png_set_bgr(png);

  // allocate a vector of row pointers
	std::vector<const uint8_t*> img_rows(height);
  auto src_rows = pixels;
	for (uint32_t y = 0; y < height; ++y) {
    img_rows.at(y) = src_rows;
    src_rows += pitch;
  }

	// write the whole image
	png_write_image(png, (uint8_t**)img_rows.data());
	png_write_end(png, png_info);

	png_destroy_write_struct(&png, (png_infopp) NULL);

  fclose(file);

  return 0;
}