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
  ar & boost::serialization::make_nvp("x", pos.x);;
  ar & boost::serialization::make_nvp("y", pos.y);;
  ar & boost::serialization::make_nvp("z", pos.z);;
  ar & boost::serialization::make_nvp("w", pos.w);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::color_t & color, const unsigned int) {
  ar & boost::serialization::make_nvp("r", color.r);;
  ar & boost::serialization::make_nvp("g", color.g);;
  ar & boost::serialization::make_nvp("b", color.b);;
  ar & boost::serialization::make_nvp("a", color.a);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::texcoord_t & texcoord, const unsigned int) {
  ar & boost::serialization::make_nvp("u", texcoord.u);;
  ar & boost::serialization::make_nvp("v", texcoord.v);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::vertex_t & vertex, const unsigned int) {
  ar & boost::serialization::make_nvp("pos", vertex.pos);;
  ar & boost::serialization::make_nvp("color", vertex.color);;
  ar & boost::serialization::make_nvp("texcoord", vertex.texcoord);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::primitive_t & primitive, const unsigned int) {
  ar & boost::serialization::make_nvp("i0", primitive.i0);;
  ar & boost::serialization::make_nvp("i1", primitive.i1);;
  ar & boost::serialization::make_nvp("i2", primitive.i2);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::texture_t & texture, const unsigned int) {
  ar & boost::serialization::make_nvp("format", texture.format);;
  ar & boost::serialization::make_nvp("width", texture.width);;
  ar & boost::serialization::make_nvp("height", texture.height);;
  ar & boost::serialization::make_nvp("pixels", texture.pixels);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::viewport_t & viewport, const unsigned int) {
  ar & boost::serialization::make_nvp("left", viewport.left);;
  ar & boost::serialization::make_nvp("right", viewport.right);;
  ar & boost::serialization::make_nvp("top", viewport.top);;
  ar & boost::serialization::make_nvp("bottom", viewport.bottom);;
  ar & boost::serialization::make_nvp("near", viewport.near);;
  ar & boost::serialization::make_nvp("far", viewport.far);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::states_t & states, const unsigned int) {
  ar & boost::serialization::make_nvp("color_enabled", states.color_enabled);;
  ar & boost::serialization::make_nvp("color_format", states.color_format);;
  ar & boost::serialization::make_nvp("color_writemask", states.color_writemask);;

  ar & boost::serialization::make_nvp("depth_test", states.depth_test);;
  ar & boost::serialization::make_nvp("depth_writemask", states.depth_writemask);;
  ar & boost::serialization::make_nvp("depth_format", states.depth_format);; 
  ar & boost::serialization::make_nvp("depth_func", states.depth_func);;

  ar & boost::serialization::make_nvp("stencil_test", states.stencil_test);;
  ar & boost::serialization::make_nvp("stencil_func", states.stencil_func);;
  ar & boost::serialization::make_nvp("stencil_zpass", states.stencil_zpass);;
  ar & boost::serialization::make_nvp("stencil_zfail", states.stencil_zfail);;
  ar & boost::serialization::make_nvp("stencil_fail", states.stencil_fail);;
  ar & boost::serialization::make_nvp("stencil_ref", states.stencil_ref);;
  ar & boost::serialization::make_nvp("stencil_mask", states.stencil_mask);;
  ar & boost::serialization::make_nvp("stencil_writemask", states.stencil_writemask);;

  ar & boost::serialization::make_nvp("texture_enabled", states.texture_enabled);;
  ar & boost::serialization::make_nvp("texture_envcolor", states.texture_envcolor);;
  ar & boost::serialization::make_nvp("texture_envmode", states.texture_envmode);;
  ar & boost::serialization::make_nvp("texture_minfilter", states.texture_minfilter);;
  ar & boost::serialization::make_nvp("texture_magfilter", states.texture_magfilter);;
  ar & boost::serialization::make_nvp("texture_addressU", states.texture_addressU);;
  ar & boost::serialization::make_nvp("texture_addressV", states.texture_addressV);;

  ar & boost::serialization::make_nvp("blend_enabled", states.blend_enabled);;
  ar & boost::serialization::make_nvp("blend_src", states.blend_src);;
  ar & boost::serialization::make_nvp("blend_dst", states.blend_dst);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace::drawcall_t & drawcall, const unsigned int) {
  ar & boost::serialization::make_nvp("states", drawcall.states);;
  ar & boost::serialization::make_nvp("texture_id", drawcall.texture_id);;
  ar & boost::serialization::make_nvp("vertices", drawcall.vertices);;
  ar & boost::serialization::make_nvp("primitives", drawcall.primitives);;
  ar & boost::serialization::make_nvp("viewport", drawcall.viewport);;
}

template<class Archive>
void serialize(Archive & ar, CGLTrace & trace, const unsigned int) {
  ar & boost::serialization::make_nvp("version", trace.version);;
  ar & boost::serialization::make_nvp("drawcalls", trace.drawcalls);;
  ar & boost::serialization::make_nvp("textures", trace.textures);;
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