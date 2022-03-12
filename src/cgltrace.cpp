#include "cgltrace.hpp"
#include <fstream>
#include <string.h>

using namespace cocogfx;

static std::ostream& operator<<(std::ostream& os, const ePixelFormat& format) {
  os << (int)format;
  return os;
}

static std::istream& operator>>(std::istream& is, ePixelFormat& format) {
  int tmp;
  is >> tmp;
  format = (ePixelFormat)tmp;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

static std::ostream& operator<<(std::ostream& os, const CGLTrace::ecompare& compare) {
  os << (int)compare;
  return os;
}

static std::istream& operator>>(std::istream& is, CGLTrace::ecompare& compare) {
  int tmp;
  is >> tmp;
  compare = (CGLTrace::ecompare)tmp;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

static std::ostream& operator<<(std::ostream& os, const CGLTrace::eStencilOp& op) {
  os << (int)op;
  return os;
}

static std::istream& operator>>(std::istream& is, CGLTrace::eStencilOp& op) {
  int tmp;
  is >> tmp;
  op = (CGLTrace::eStencilOp)tmp;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

static std::ostream& operator<<(std::ostream& os, const CGLTrace::eEnvMode& mode) {
  os << (int)mode;
  return os;
}

static std::istream& operator>>(std::istream& is, CGLTrace::eEnvMode& mode) {
  int tmp;
  is >> tmp;
  mode = (CGLTrace::eEnvMode)tmp;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

static std::ostream& operator<<(std::ostream& os, const CGLTrace::eTexFilter& filter) {
  os << (int)filter;
  return os;
}

static std::istream& operator>>(std::istream& is, CGLTrace::eTexFilter& filter) {
  int tmp;
  is >> tmp;
  filter = (CGLTrace::eTexFilter)tmp;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

static std::ostream& operator<<(std::ostream& os, const CGLTrace::eTexAddress& addr) {
  os << (int)addr;
  return os;
}

static std::istream& operator>>(std::istream& is, CGLTrace::eTexAddress& addr) {
  int tmp;
  is >> tmp;
  addr = (CGLTrace::eTexAddress)tmp;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

static std::ostream& operator<<(std::ostream& os, const CGLTrace::eBlendOp& op) {
  os << (int)op;
  return os;
}

static std::istream& operator>>(std::istream& is, CGLTrace::eBlendOp& op) {
  int tmp;
  is >> tmp;
  op = (CGLTrace::eBlendOp)tmp;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace::pos_t& pos) {
  os << pos.x << std::endl << pos.y << std::endl << pos.z << std::endl << pos.w;
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::pos_t& pos) {
  is >> pos.x >> pos.y >> pos.z >> pos.w;
  return is;
}
    
///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace::color_t& color) {
  os << color.r << std::endl << color.g << std::endl << color.b << std::endl << color.a;
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::color_t& color) {
  is >> color.r >> color.g >> color.b >> color.a;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace::texcoord_t& texcoord) {
  os << texcoord.u << std::endl << texcoord.v;
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::texcoord_t& texcoord) {
  is >> texcoord.u >> texcoord.v;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace::viewport_t& viewport) {
  os << viewport.left << std::endl << viewport.right << std::endl 
     << viewport.top << std::endl << viewport.bottom << std::endl 
     << viewport.near << std::endl << viewport.far;
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::viewport_t& viewport) {
  is >> viewport.left >> viewport.right
     >> viewport.top  >> viewport.bottom
     >> viewport.near >> viewport.far;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace::vertex_t& vertex) {
  os << vertex.pos << std::endl << vertex.color << std::endl << vertex.texcoord;
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::vertex_t& vertex) {
  is >> vertex.pos >> vertex.color >> vertex.texcoord;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace::primitive_t& primitive) {
  os << primitive.i0 << std::endl << primitive.i1 << std::endl << primitive.i2;
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::primitive_t& primitive) {
  is >> primitive.i0 >> primitive.i1 >> primitive.i2;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

bool CGLTrace::texture_t::operator==(const CGLTrace::texture_t& rhs) const {
  return (format == rhs.format) 
      && (width == rhs.width)
      && (height == rhs.height)
      && (pixels.size() == rhs.pixels.size())
      && (0 == memcmp(pixels.data(), rhs.pixels.data(), pixels.size()));
}

std::ostream& operator<<(std::ostream& os, const CGLTrace::texture_t& texture) {
  os << texture.format << std::endl << texture.width << std::endl << texture.height;
  {
    os << std::endl << (uint32_t)texture.pixels.size();
    for (auto pixel : texture.pixels) {
      os << std::endl << pixel;
    }
  }
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::texture_t& texture) {
  is >> texture.format >> texture.width >> texture.height;
  {
    uint32_t pixels_size;
    is >> pixels_size;
    texture.pixels.resize(pixels_size);
    for (auto& pixel : texture.pixels) {
      is >> pixel;
    }
  }
  return is;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace::states_t& states) {
  os << states.color_enabled << std::endl;
  os << states.color_format << std::endl;
  os << states.color_writemask << std::endl;

  os << states.depth_test << std::endl;
  os << states.depth_writemask << std::endl;
  os << states.depth_format << std::endl; 
  os << states.depth_func << std::endl;
  
  os << states.stencil_test << std::endl;
  os << states.stencil_func << std::endl;
  os << states.stencil_zpass << std::endl;
  os << states.stencil_zfail << std::endl;
  os << states.stencil_fail << std::endl;
  os << states.stencil_ref << std::endl;
  os << states.stencil_mask << std::endl;
  os << states.stencil_writemask << std::endl;
  
  os << states.texture_enabled << std::endl;
  os << states.texture_envcolor << std::endl;
  os << states.texture_envmode << std::endl;
  os << states.texture_minfilter << std::endl;
  os << states.texture_magfilter << std::endl;
  os << states.texture_addressU << std::endl;
  os << states.texture_addressV << std::endl;

  os << states.blend_enabled << std::endl;
  os << states.blend_src << std::endl;
  os << states.blend_dst;
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::states_t& states) {
  is >> states.color_enabled;
  is >> states.color_format;
  is >> states.color_writemask;

  is >> states.depth_test;
  is >> states.depth_writemask;
  is >> states.depth_format; 
  is >> states.depth_func;
  
  is >> states.stencil_test;
  is >> states.stencil_func;
  is >> states.stencil_zpass;
  is >> states.stencil_zfail;
  is >> states.stencil_fail;
  is >> states.stencil_ref;
  is >> states.stencil_mask;
  is >> states.stencil_writemask;
  
  is >> states.texture_enabled;
  is >> states.texture_envcolor;
  is >> states.texture_envmode;
  is >> states.texture_minfilter;
  is >> states.texture_magfilter;
  is >> states.texture_addressU;
  is >> states.texture_addressV;

  is >> states.blend_enabled;
  is >> states.blend_src;
  is >> states.blend_dst;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace::drawcall_t& drawcall) {
  os << drawcall.states << std::endl << drawcall.texture_id;
  {
    os << std::endl << (uint32_t)drawcall.vertices.size();
    for (auto it : drawcall.vertices) {
      os << std::endl << it.first << std::endl << it.second;
    }
  }
  {
    os << std::endl << drawcall.primitives.size();
    for (auto& primitive : drawcall.primitives) {
      os << std::endl << primitive;
    }
  }
  os << std::endl << drawcall.viewport;
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace::drawcall_t& drawcall) {
  is >> drawcall.states >> drawcall.texture_id;
  {
    uint32_t vertices_size;
    is >> vertices_size;
    for (uint32_t i = 0; i < vertices_size; ++i) {
      uint32_t index;
      is >> index;
      is >> drawcall.vertices[index];
    }
  }
  {
    uint32_t primitives_size;
    is >> primitives_size;
    drawcall.primitives.resize(primitives_size);
    for (auto& primitive : drawcall.primitives) {
      is >> primitive;
    }
  }
  is >> drawcall.viewport;
  return is;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const CGLTrace& trace) {
  {
    os << (uint32_t)trace.drawcalls.size();
    for (auto& drawcall : trace.drawcalls) {
      os << std::endl << drawcall;
    }
  }
  {
    os << std::endl << (uint32_t)trace.textures.size();
    for (auto it : trace.textures) {
      os << std::endl << it.first << std::endl << it.second;
    }
  }
  return os;
}

std::istream& operator>>(std::istream& is, CGLTrace& trace) {
  {
    uint32_t draws_size;
    is >> draws_size;
    trace.drawcalls.resize(draws_size);
    for (auto& drawcall : trace.drawcalls) {
      is >> drawcall;
    }
  }
  {
    uint32_t textures_size;
    is >> textures_size;
    for (uint32_t i = 0; i < textures_size; ++i) {
      uint32_t index;
      is >> index;
      is >> trace.textures[index];
    }
  }
  return is;
}

int CGLTrace::load(const char* filename) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "couldn't open file: " << filename << "!" << std::endl;
    return -1;
  }
  ifs >> *this;
  return 0;
}

int CGLTrace::save(const char* filename) {
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "couldn't create file: " << filename << "!" << std::endl;
    return -1;
  }
  ofs << *this;
  return 0;
}