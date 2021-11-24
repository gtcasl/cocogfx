#include "blitter.h"
#include <assert.h>
#include <cstring>
#include <iostream>

using namespace cocogfx;

namespace {
constexpr uint32_t count_leading_zeros(uint32_t value) {
  return value ? __builtin_clz(value) : 32;
}

constexpr uint32_t log2ceil(uint32_t value) {
  return 32 - count_leading_zeros(value - 1);
}

struct SurfaceDesc {
  ePixelFormat Format;
  uint8_t *pBits;
  uint32_t Width;
  uint32_t Height;
  uint32_t Pitch;
};

class BlitTable {
public:
  typedef int (*PfnCopy)(const SurfaceDesc &dstDesc, 
                         uint32_t dstOffsetX,
                         uint32_t dstOffsetY, 
                         uint32_t copyWidth,
                         uint32_t copyHeight, 
                         const SurfaceDesc &srcDesc,
                         uint32_t srcOffsetX, 
                         uint32_t srcOffsetY);

  BlitTable() {
    for (uint32_t s = 0; s < FORMAT_COLOR_SIZE_; ++s) {
      for (uint32_t d = 0; d < FORMAT_COLOR_SIZE_; ++d) {
        copyFuncs_[s][d] = CopyInvalid;
      }
    }

    for (uint32_t s = 0; s < FORMAT_COLOR_SIZE_; ++s) {
      switch (s) {
      case FORMAT_A8:
      case FORMAT_L8:
        copyFuncs_[s][s] = CopyFast<uint8_t>;
        break;

      case FORMAT_A8L8:
        copyFuncs_[FORMAT_A8L8][FORMAT_A8] = Copy<FORMAT_A8L8, FORMAT_A8>;
        copyFuncs_[FORMAT_A8L8][FORMAT_A8L8] = CopyFast<uint16_t>;
        break;

      case FORMAT_R5G6B5:
        copyFuncs_[FORMAT_R5G6B5][FORMAT_L8] = Copy<FORMAT_R5G6B5, FORMAT_L8>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_R5G6B5] = CopyFast<uint16_t>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_R8G8B8] = Copy<FORMAT_R5G6B5, FORMAT_R8G8B8>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_B8G8R8] = Copy<FORMAT_R5G6B5, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_A8B8G8R8] = Copy<FORMAT_R5G6B5, FORMAT_A8B8G8R8>;
        copyFuncs_[FORMAT_R5G6B5][FORMAT_A8R8G8B8] = Copy<FORMAT_R5G6B5, FORMAT_A8R8G8B8>;
        break;

      case FORMAT_A1R5G5B5:
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_A8] = Copy<FORMAT_A1R5G5B5, FORMAT_A8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_L8] = Copy<FORMAT_A1R5G5B5, FORMAT_L8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_A8L8] = Copy<FORMAT_A1R5G5B5, FORMAT_A8L8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_R8G8B8] =  Copy<FORMAT_A1R5G5B5, FORMAT_R8G8B8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_A8R8G8B8] = Copy<FORMAT_A1R5G5B5, FORMAT_A8R8G8B8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_R5G5B5A1] = Copy<FORMAT_A1R5G5B5, FORMAT_R5G5B5A1>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_R4G4B4A4] = Copy<FORMAT_A1R5G5B5, FORMAT_R4G4B4A4>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_B8G8R8] = Copy<FORMAT_A1R5G5B5, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_A1R5G5B5][FORMAT_A8B8G8R8] = Copy<FORMAT_A1R5G5B5, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_A4R4G4B4:
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_A8] = Copy<FORMAT_A4R4G4B4, FORMAT_A8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_L8] = Copy<FORMAT_A4R4G4B4, FORMAT_L8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_A8L8] = Copy<FORMAT_A4R4G4B4, FORMAT_A8L8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_R8G8B8] = Copy<FORMAT_A4R4G4B4, FORMAT_R8G8B8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_A8R8G8B8] = Copy<FORMAT_A4R4G4B4, FORMAT_A8R8G8B8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_R5G5B5A1] = Copy<FORMAT_A4R4G4B4, FORMAT_R5G5B5A1>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_R4G4B4A4] = Copy<FORMAT_A4R4G4B4, FORMAT_R4G4B4A4>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_B8G8R8] = Copy<FORMAT_A4R4G4B4, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_A4R4G4B4][FORMAT_A8B8G8R8] = Copy<FORMAT_A4R4G4B4, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_R8G8B8:
        copyFuncs_[FORMAT_R8G8B8][FORMAT_L8] = Copy<FORMAT_R8G8B8, FORMAT_L8>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_R5G6B5] = Copy<FORMAT_R8G8B8, FORMAT_R5G6B5>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_R8G8B8] = CopyFast<uint24_t>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_B8G8R8] = Copy<FORMAT_R8G8B8, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_A8B8G8R8] = Copy<FORMAT_R8G8B8, FORMAT_A8B8G8R8>;
        copyFuncs_[FORMAT_R8G8B8][FORMAT_A8R8G8B8] = Copy<FORMAT_R8G8B8, FORMAT_A8R8G8B8>;
        break;

      case FORMAT_A8R8G8B8:
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_A8] = Copy<FORMAT_A8R8G8B8, FORMAT_A8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_L8] = Copy<FORMAT_A8R8G8B8, FORMAT_L8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_A8L8] = Copy<FORMAT_A8R8G8B8, FORMAT_A8L8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_R5G6B5] = Copy<FORMAT_A8R8G8B8, FORMAT_R5G6B5>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_R8G8B8] = Copy<FORMAT_A8R8G8B8, FORMAT_R8G8B8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_A8R8G8B8] = CopyFast<uint32_t>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_R5G5B5A1] = Copy<FORMAT_A8R8G8B8, FORMAT_R5G5B5A1>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_R4G4B4A4] = Copy<FORMAT_A8R8G8B8, FORMAT_R4G4B4A4>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_B8G8R8] = Copy<FORMAT_A8R8G8B8, FORMAT_B8G8R8>;
        copyFuncs_[FORMAT_A8R8G8B8][FORMAT_A8B8G8R8] = Copy<FORMAT_A8R8G8B8, FORMAT_A8B8G8R8>;
        break;

      case FORMAT_R5G5B5A1:
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_A8] = Copy<FORMAT_R5G5B5A1, FORMAT_A8>;
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_L8] = Copy<FORMAT_R5G5B5A1, FORMAT_L8>;
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_A8L8] = Copy<FORMAT_R5G5B5A1, FORMAT_A8L8>;
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_RGB] = Copy<FORMAT_R5G5B5A1, FORMAT_RGB>;
        copyFuncs_[FORMAT_R5G5B5A1][FORMAT_ARGB] = Copy<FORMAT_R5G5B5A1, FORMAT_ARGB>;
        break;

      case FORMAT_R4G4B4A4:
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_A8] = Copy<FORMAT_R4G4B4A4, FORMAT_A8>;
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_L8] = Copy<FORMAT_R4G4B4A4, FORMAT_L8>;
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_A8L8] = Copy<FORMAT_R4G4B4A4, FORMAT_A8L8>;
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_RGB] = Copy<FORMAT_R4G4B4A4, FORMAT_RGB>;
        copyFuncs_[FORMAT_R4G4B4A4][FORMAT_ARGB] = Copy<FORMAT_R4G4B4A4, FORMAT_ARGB>;
        break;

      case FORMAT_B8G8R8:
        copyFuncs_[FORMAT_B8G8R8][FORMAT_L8] = Copy<FORMAT_B8G8R8, FORMAT_L8>;
        copyFuncs_[FORMAT_B8G8R8][FORMAT_RGB] = Copy<FORMAT_B8G8R8, FORMAT_RGB>;
        break;

      case FORMAT_A8B8G8R8:
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_A8] = Copy<FORMAT_A8B8G8R8, FORMAT_A8>;
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_L8] = Copy<FORMAT_A8B8G8R8, FORMAT_L8>;
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_A8L8] = Copy<FORMAT_A8B8G8R8, FORMAT_A8L8>;
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_RGB] = Copy<FORMAT_A8B8G8R8, FORMAT_RGB>;
        copyFuncs_[FORMAT_A8B8G8R8][FORMAT_ARGB] = Copy<FORMAT_A8B8G8R8, FORMAT_ARGB>;
        break;
      }
    }
  }

  PfnCopy get(uint32_t srcFormat, uint32_t dstFormat) const {
    assert(srcFormat < FORMAT_COLOR_SIZE_);
    assert(dstFormat < FORMAT_COLOR_SIZE_);
    return copyFuncs_[srcFormat][dstFormat];
  }

private:
  template <ePixelFormat SrcFormat, ePixelFormat DstFormat>
  static int Copy(const SurfaceDesc &dstDesc, 
                  uint32_t dstOffsetX,
                  uint32_t dstOffsetY, 
                  uint32_t copyWidth,
                  uint32_t copyHeight, 
                  const SurfaceDesc &srcDesc,
                  uint32_t srcOffsetX, 
                  uint32_t srcOffsetY) {
    auto srcBPP = TFormatInfo<SrcFormat>::CBSIZE;
    auto dstBPP = TFormatInfo<DstFormat>::CBSIZE;
    auto srcNextLine = srcDesc.Pitch;
    auto dstNextLine = dstDesc.Pitch;

    auto pbSrc = srcDesc.pBits + srcOffsetX * srcBPP + srcOffsetY * srcDesc.Pitch;
    auto pbDst = dstDesc.pBits + dstOffsetX * dstBPP + dstOffsetY * dstDesc.Pitch;

    while (copyHeight--) {
      auto pSrc = reinterpret_cast<const typename TFormatInfo<SrcFormat>::TYPE *>(pbSrc);
      for (auto *pDst = reinterpret_cast<typename TFormatInfo<DstFormat>::TYPE *>(pbDst),
          *const pEnd = pDst + copyWidth; pDst != pEnd; ++pDst, ++pSrc) {
        auto tmp = ConvertFrom<SrcFormat, true>(pSrc);
        ConvertTo<DstFormat>(pDst, tmp);
      }
      pbSrc += srcNextLine;
      pbDst += dstNextLine;
    }    
    return 0;
  }

  template <typename Type>
  static int CopyFast(const SurfaceDesc &dstDesc, 
                      uint32_t dstOffsetX,
                      uint32_t dstOffsetY, 
                      uint32_t copyWidth,
                      uint32_t copyHeight, 
                      const SurfaceDesc &srcDesc,
                      uint32_t srcOffsetX, 
                      uint32_t srcOffsetY) {
    auto nBPP = sizeof(Type);
    auto srcNextLine = srcDesc.Pitch;
    auto dstNextLine = dstDesc.Pitch;

    auto pbSrc = srcDesc.pBits + srcOffsetX * nBPP + srcOffsetY * srcNextLine;
    auto pbDst = dstDesc.pBits + dstOffsetX * nBPP + dstOffsetY * dstNextLine;

    if (srcNextLine == dstNextLine) {
      memcpy(pbDst, pbSrc, srcNextLine * copyHeight);
    } else {
      while (copyHeight--) {
        auto pSrc = reinterpret_cast<const Type *>(pbSrc);
        for (auto *pDst = reinterpret_cast<Type *>(pbDst), *const pEnd = pDst + copyWidth;
            pDst != pEnd; ++pDst, ++pSrc) {
          *pDst = *pSrc;
        }
        pbSrc += srcNextLine;
        pbDst += dstNextLine;
      }
    }

    return 0;
  }

  static int CopyInvalid(const SurfaceDesc & /*dstDesc*/,
                         uint32_t /*dstOffsetX*/, 
                         uint32_t /*dstOffsetY*/,
                         uint32_t /*copyWidth*/, 
                         uint32_t /*copyHeight*/,
                         const SurfaceDesc & /*srcDesc*/,
                         uint32_t /*srcOffsetX*/, 
                         uint32_t /*srcOffsetY*/)
  {
    std::cout << "Error: invalid format" << std::endl;
    return -1;
  }

  PfnCopy copyFuncs_[FORMAT_COLOR_SIZE_][FORMAT_COLOR_SIZE_];
};

static const BlitTable s_blitTable;
}

namespace cocogfx {

int CopyBuffers(ePixelFormat dst_format,
                uint8_t *dst_pixels,
                uint32_t dst_width,
                uint32_t dst_height,
                uint32_t dst_pitch,
                int32_t dst_offsetx,
                int32_t dst_offsety,
                ePixelFormat src_format,
                const uint8_t *src_pixels,
                uint32_t src_width,
                uint32_t src_height,
                uint32_t src_pitch,
                int32_t src_offsetx,
                int32_t src_offsety,                                     
                uint32_t width, 
                uint32_t height) {  
  if ((src_offsetx >= (int32_t)src_width) || (src_offsety >= (int32_t)src_height) ||
      (dst_offsetx >= (int32_t)dst_width) || (dst_offsety >= (int32_t)dst_height)) {
    return -1;
  }

  if (width > dst_width) {
    width = dst_width;
  }

  if (width > src_width) {
    width = src_width;
  }

  if (height > dst_height) {
    height = dst_height;
  }

  if (height > src_height) {
    height = src_height;
  }

  SurfaceDesc src_desc{src_format, (uint8_t*)src_pixels, width, height, src_pitch};            
  SurfaceDesc dst_desc{dst_format, dst_pixels, width, height, dst_pitch};

  return s_blitTable.get(src_format, dst_format)(
    dst_desc, dst_offsetx, dst_offsety, width, height, src_desc, src_offsetx, src_offsety);
}

int ConvertImage(std::vector<uint8_t>& dst_pixels,
                 const std::vector<uint8_t>& src_pixels,
                 ePixelFormat src_format,
                 uint32_t src_width,
                 uint32_t src_height,                 
                 ePixelFormat dst_format) {
  uint32_t src_pitch = GetInfo(src_format).BytePerPixel * src_width;
  uint32_t dst_pitch = GetInfo(dst_format).BytePerPixel * src_width;
  dst_pixels.resize(dst_pitch * src_height);

  return CopyBuffers(    
    dst_pixels.data(),
    dst_format,
    src_width,
    src_height,
    dst_pitch,
    0,
    0,    
    src_pixels.data(),
    src_format,
    src_width,
    src_height,
    src_pitch,
    0,
    0,                                     
    src_width, 
    src_height
  );
}

int GenerateMipmaps(std::vector<uint8_t>& dst_pixels,
                    std::vector<uint32_t>& mip_offsets,
                    const std::vector<uint8_t>& src_pixels,
                    ePixelFormat format,
                    uint32_t src_width,
                    uint32_t src_height,
                    uint32_t src_pitch) {
  uint32_t bpp = GetInfo(format).BytePerPixel;
  uint32_t src_logwidth  = log2ceil(src_width);
  uint32_t src_logheight = log2ceil(src_height);
  uint32_t num_mipmaps   = std::max(src_logwidth, src_logheight) + 1;
  auto fmt_convert_from  = GetConvertFrom(format, true);
  auto fmt_convert_to    = GetConvertTo(format);

  mip_offsets.resize(num_mipmaps);

  // Calculate mipmaps buffer size
  uint32_t num_pixels = 0;
  for (uint32_t lod = 0, w = src_width, h = src_height; lod < num_mipmaps; ++lod) {
    assert((w > 0) || (w > 0));
    uint32_t pw = std::max<int>(w, 1);
    uint32_t ph = std::max<int>(h, 1);
    mip_offsets.at(lod) = num_pixels;
    num_pixels += pw * ph;
    w >>= 1;
    h >>= 1;
  }

  // allocate mipmap
  dst_pixels.resize(num_pixels * bpp);

  // generate mipmaps  
  {
    auto pSrc = src_pixels.data();
    auto pDst = dst_pixels.data();

    // copy level 0
    CopyBuffers(
      pDst, format, src_width, src_height, src_width * bpp,
      0, 0,
      pSrc, format, src_width, src_height, src_pitch,
      0, 0, src_width, src_height
    );    
    pSrc = pDst;
    pDst += src_width * src_height * bpp;

    // copy lower levels
    for (uint32_t lod = 1, w = (src_width/2), h = (src_height/2); lod < num_mipmaps;) {
      assert((w > 0) || (w > 0));
      uint32_t pw = std::max<int>(w, 1);
      uint32_t ph = std::max<int>(h, 1);
      for (uint32_t y = 0; y < pw; ++y) {
        uint32_t v0 = 2 * y;
        uint32_t v1 = 2 * y + ((ph > 1) ? 1 : 0);
        
        uint32_t p0 = v0 * 2 * pw;
        uint32_t p1 = v1 * 2 * pw;

        for (uint32_t x = 0; x <pw; ++x) {
          uint32_t u0 = 2 * x;
          uint32_t u1 = 2 * x + ((pw > 1) ? 1 : 0);

          auto c00 = fmt_convert_from(pSrc + (u0 + p0) * bpp);
          auto c01 = fmt_convert_from(pSrc + (u1 + p0) * bpp);
          auto c10 = fmt_convert_from(pSrc + (u0 + p1) * bpp);
          auto c11 = fmt_convert_from(pSrc + (u1 + p1) * bpp);

          const ColorARGB color((c00.a + c01.a + c10.a + c11.a+2) >> 2,
                                (c00.r + c01.r + c10.r + c11.r+2) >> 2,
                                (c00.g + c01.g + c10.g + c11.g+2) >> 2,
                                (c00.b + c01.b + c10.b + c11.b+2) >> 2);
                                
          fmt_convert_to(pDst + (x + y * pw) * bpp, color);
        }
      } 
      ++lod; 
      pSrc = pDst;
      pDst += pw * ph * bpp;
      w /= 2;
      h /= 2;  
    }
    assert(pDst == (dst_pixels.data() + dst_pixels.size()));
  }
 
  for (auto& offset : mip_offsets) {
    offset *= bpp;
  }

  return 0;
}

}