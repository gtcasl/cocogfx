#include "cgltrace.hpp"
#include <fstream>
#include <string.h>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace cocogfx;

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, CGLTrace::pos_t & pos, const unsigned int) {
  ar & pos.x;
  ar & pos.y;
  ar & pos.z;
  ar & pos.w;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::color_t & color, const unsigned int) {
  ar & color.r;
  ar & color.g;
  ar & color.b;
  ar & color.a;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::texcoord_t & texcoord, const unsigned int) {
  ar & texcoord.u;
  ar & texcoord.v;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::vertex_t & vertex, const unsigned int) {
  ar & vertex.pos;
  ar & vertex.color;
  ar & vertex.texcoord;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::primitive_t & primitive, const unsigned int) {
  ar & primitive.i0;
  ar & primitive.i1;
  ar & primitive.i2;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::texture_t & texture, const unsigned int) {
  ar & texture.format;
  ar & texture.width;
  ar & texture.height;
  ar & texture.pixels;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::viewport_t & viewport, const unsigned int) {
  ar & viewport.left;
  ar & viewport.right;
  ar & viewport.top;
  ar & viewport.bottom;
  ar & viewport.near;
  ar & viewport.far;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::states_t & states, const unsigned int) {
  ar & states.color_enabled;
  ar & states.color_format;
  ar & states.color_writemask;

  ar & states.depth_test;
  ar & states.depth_writemask;
  ar & states.depth_format; 
  ar & states.depth_func;

  ar & states.stencil_test;
  ar & states.stencil_func;
  ar & states.stencil_zpass;
  ar & states.stencil_zfail;
  ar & states.stencil_fail;
  ar & states.stencil_ref;
  ar & states.stencil_mask;
  ar & states.stencil_writemask;

  ar & states.texture_enabled;
  ar & states.texture_envcolor;
  ar & states.texture_envmode;
  ar & states.texture_minfilter;
  ar & states.texture_magfilter;
  ar & states.texture_addressU;
  ar & states.texture_addressV;

  ar & states.blend_enabled;
  ar & states.blend_src;
  ar & states.blend_dst;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::drawcall_t & drawcall, const unsigned int) {
  ar & drawcall.states;
  ar & drawcall.texture_id;
  ar & drawcall.vertices;
  ar & drawcall.primitives;
  ar & drawcall.viewport;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace & trace, const unsigned int) {
  ar & trace.version;
  ar & trace.drawcalls;
  ar & trace.textures;
}

}}

int CGLTrace::load(const char* filename) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "couldn't open file: " << filename << "!" << std::endl;
    return -1;
  }

  boost::archive::text_iarchive ia(ifs);
  ia >> (*this);

  return 0;
}

int CGLTrace::save(const char* filename) {
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "couldn't create file: " << filename << "!" << std::endl;
    return -1;
  }

  boost::archive::text_oarchive oa(ofs);
  oa << (*this);

  return 0;
}