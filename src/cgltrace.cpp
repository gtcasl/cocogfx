#include "cgltrace.hpp"
#include <fstream>
#include <string.h>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

using namespace cocogfx;

bool CGLTrace::texture_t::operator==(const CGLTrace::texture_t& rhs) const {
  return (format == rhs.format) 
      && (width == rhs.width)
      && (height == rhs.height)
      && (pixels.size() == rhs.pixels.size())
      && (0 == memcmp(pixels.data(), rhs.pixels.data(), pixels.size()));
}

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, CGLTrace::pos_t & pos, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(pos.x);
  ar & BOOST_SERIALIZATION_NVP(pos.y);
  ar & BOOST_SERIALIZATION_NVP(pos.z);
  ar & BOOST_SERIALIZATION_NVP(pos.w);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::color_t & color, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(color.r);
  ar & BOOST_SERIALIZATION_NVP(color.g);
  ar & BOOST_SERIALIZATION_NVP(color.b);
  ar & BOOST_SERIALIZATION_NVP(color.a);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::texcoord_t & texcoord, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(texcoord.u);
  ar & BOOST_SERIALIZATION_NVP(texcoord.v);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::vertex_t & vertex, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(vertex.pos);
  ar & BOOST_SERIALIZATION_NVP(vertex.color);
  ar & BOOST_SERIALIZATION_NVP(vertex.texcoord);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::primitive_t & primitive, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(primitive.i0);
  ar & BOOST_SERIALIZATION_NVP(primitive.i1);
  ar & BOOST_SERIALIZATION_NVP(primitive.i2);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::texture_t & texture, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(texture.format);
  ar & BOOST_SERIALIZATION_NVP(texture.width);
  ar & BOOST_SERIALIZATION_NVP(texture.height);
  ar & BOOST_SERIALIZATION_NVP(texture.pixels);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::viewport_t & viewport, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(viewport.left);
  ar & BOOST_SERIALIZATION_NVP(viewport.right);
  ar & BOOST_SERIALIZATION_NVP(viewport.top);
  ar & BOOST_SERIALIZATION_NVP(viewport.bottom);
  ar & BOOST_SERIALIZATION_NVP(viewport.near);
  ar & BOOST_SERIALIZATION_NVP(viewport.far);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::states_t & states, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(states.color_enabled);
  ar & BOOST_SERIALIZATION_NVP(states.color_format);
  ar & BOOST_SERIALIZATION_NVP(states.color_writemask);

  ar & BOOST_SERIALIZATION_NVP(states.depth_test);
  ar & BOOST_SERIALIZATION_NVP(states.depth_writemask);
  ar & BOOST_SERIALIZATION_NVP(states.depth_format); 
  ar & BOOST_SERIALIZATION_NVP(states.depth_func);

  ar & BOOST_SERIALIZATION_NVP(states.stencil_test);
  ar & BOOST_SERIALIZATION_NVP(states.stencil_func);
  ar & BOOST_SERIALIZATION_NVP(states.stencil_zpass);
  ar & BOOST_SERIALIZATION_NVP(states.stencil_zfail);
  ar & BOOST_SERIALIZATION_NVP(states.stencil_fail);
  ar & BOOST_SERIALIZATION_NVP(states.stencil_ref);
  ar & BOOST_SERIALIZATION_NVP(states.stencil_mask);
  ar & BOOST_SERIALIZATION_NVP(states.stencil_writemask);

  ar & BOOST_SERIALIZATION_NVP(states.texture_enabled);
  ar & BOOST_SERIALIZATION_NVP(states.texture_envcolor);
  ar & BOOST_SERIALIZATION_NVP(states.texture_envmode);
  ar & BOOST_SERIALIZATION_NVP(states.texture_minfilter);
  ar & BOOST_SERIALIZATION_NVP(states.texture_magfilter);
  ar & BOOST_SERIALIZATION_NVP(states.texture_addressU);
  ar & BOOST_SERIALIZATION_NVP(states.texture_addressV);

  ar & BOOST_SERIALIZATION_NVP(states.blend_enabled);
  ar & BOOST_SERIALIZATION_NVP(states.blend_src);
  ar & BOOST_SERIALIZATION_NVP(states.blend_dst);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::drawcall_t & drawcall, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(drawcall.states);
  ar & BOOST_SERIALIZATION_NVP(drawcall.texture_id);
  ar & BOOST_SERIALIZATION_NVP(drawcall.vertices);
  ar & BOOST_SERIALIZATION_NVP(drawcall.primitives);
  ar & BOOST_SERIALIZATION_NVP(drawcall.viewport);
}

template<class Archive>
void serialize(Archive & ar, CGLTrace & trace, const unsigned int) {
  ar & BOOST_SERIALIZATION_NVP(trace.version);
  ar & BOOST_SERIALIZATION_NVP(trace.drawcalls);
  ar & BOOST_SERIALIZATION_NVP(trace.textures);
}

}}

int CGLTrace::load(const char* filename) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "couldn't open file: " << filename << "!" << std::endl;
    return -1;
  }

  boost::archive::xml_iarchive ia(ifs);
  ia >> boost::serialization::make_nvp("cgltrace", *this);

  return 0;
}

int CGLTrace::save(const char* filename) {
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "couldn't create file: " << filename << "!" << std::endl;
    return -1;
  }

  boost::archive::xml_oarchive oa(ofs);
  oa << boost::serialization::make_nvp("cgltrace", *this);

  return 0;
}