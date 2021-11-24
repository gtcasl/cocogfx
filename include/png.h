#include <cstdint>
#include <vector>

namespace cocogfx {

int LoadPNG(const char *filename, 
            std::vector<uint8_t> &pixels,
            uint32_t *width,
            uint32_t *height,
            uint32_t *bpp);

int SavePNG(const char *filename, 
            const std::vector<uint8_t> &pixels, 
            uint32_t width,
            uint32_t height, 
            uint32_t bpp);

}