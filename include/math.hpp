//
// Copyright (c) Blaise Tine.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools
// installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR
// INDEMNITIES.
//
#pragma once

#include <algorithm>
#include <math.h>
#include "common.hpp"
#include "fixed.hpp"

namespace cocogfx {

template <typename T>
struct Const {
  inline static float Zero() {
    return static_cast<T>(0.0);
  }

  inline static float Half() {
    return static_cast<T>(0.5);
  }

  inline static float One() {
    return static_cast<T>(1);
  }

  inline static float Two() {
    return static_cast<T>(2);
  }

  inline static float Min() {
    return static_cast<T>(-2147483647);
  }

  inline static float Max() {
    return static_cast<T>(2147483647);
  }

  inline static float Epsilon() {
    return static_cast<T>(0.00000001);
  }

  inline static float PI() {
    return static_cast<T>(3.14159265);
  }

  inline static float F128() {
    return static_cast<T>(128);
  }

  inline static float F90() {
    return static_cast<T>(90);
  }

  inline static float F180() {
    return static_cast<T>(180);
  }

  inline static float E() {
    return static_cast<T>(2.71828183);
  }

  inline static float LogE() {
    return static_cast<T>(1.442695041);
  }
};

template <typename T>
struct TRect {
  T left;
  T top;
  T right;
  T bottom;
};

template <typename T>
struct TVector1 {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  enum { DIM = 1 };
  union {
    struct {
      T x;
    };
    struct {
      T m[DIM];
    };
  };
  DISABLE_WARNING_POP

  TVector1() {}

  TVector1(T x) : x(x) {}

  TVector1(const TVector1 &rhs) {
    this->x = rhs.x;
  }
};

template <typename T>
struct TVector2 {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  enum { DIM = 2 };
  union {
    struct {
      T x, y;
    };
    struct {
      T m[DIM];
    };
  };
  DISABLE_WARNING_POP

  TVector2() {}

  TVector2(T x, T y) : x(x), y(y) {}

  TVector2(const TVector2 &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
  }
};

template <typename T>
struct TVector3 {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  enum { DIM = 3 };
  union {
    struct {
      T x, y, z;
    };
    struct {
      T m[DIM];
    };
  };
  DISABLE_WARNING_POP

  TVector3() {}

  TVector3(T x, T y, T z) : x(x), y(y), z(z) {}

  TVector3(const TVector3 &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
  }
};

template <typename T>
struct TVector4 {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  enum { DIM = 4 };
  union {
    struct {
      T x, y, z, w;
    };
    struct {
      T m[DIM];
    };
  };
  DISABLE_WARNING_POP

  TVector4() {}

  TVector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

  TVector4(const TVector4 &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->w = rhs.w;
  }
};

template <typename T>
struct TMatrix44 {
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_ANONYMOUS_STRUCT
  union {
    struct {
      T _11, _12, _13, _14;
      T _21, _22, _23, _24;
      T _31, _32, _33, _34;
      T _41, _42, _43, _44;
    };
    struct {
      T m[4][4];
    };
    struct {
      T _m[16];
    };
  };
  DISABLE_WARNING_POP

  TMatrix44() {}

  TMatrix44(T m11, T m12, T m13, T m14, 
            T m21, T m22, T m23, T m24, 
            T m31, T m32, T m33, T m34, 
            T m41, T m42, T m43, T m44) {
    this->_11 = m11;
    this->_12 = m12;
    this->_13 = m13;
    this->_14 = m14;
    this->_21 = m21;
    this->_22 = m22;
    this->_23 = m23;
    this->_24 = m24;
    this->_31 = m31;
    this->_32 = m32;
    this->_33 = m33;
    this->_34 = m34;
    this->_41 = m41;
    this->_42 = m42;
    this->_43 = m43;
    this->_44 = m44;
  }

  TMatrix44(const TMatrix44 &rhs) {
    this->_11 = rhs._11;
    this->_12 = rhs._12;
    this->_13 = rhs._13;
    this->_14 = rhs._14;
    this->_21 = rhs._21;
    this->_22 = rhs._22;
    this->_23 = rhs._23;
    this->_24 = rhs._24;
    this->_31 = rhs._31;
    this->_32 = rhs._32;
    this->_33 = rhs._33;
    this->_34 = rhs._34;
    this->_41 = rhs._41;
    this->_42 = rhs._42;
    this->_43 = rhs._43;
    this->_44 = rhs._44;
  }

  void toIdentity() {
    this->_11 = Const<T>::One();
    this->_12 = Const<T>::Zero();
    this->_13 = Const<T>::Zero();
    this->_14 = Const<T>::Zero();
    this->_21 = Const<T>::Zero();
    this->_22 = Const<T>::One();
    this->_23 = Const<T>::Zero();
    this->_24 = Const<T>::Zero();
    this->_31 = Const<T>::Zero();
    this->_32 = Const<T>::Zero();
    this->_33 = Const<T>::One();
    this->_34 = Const<T>::Zero();
    this->_41 = Const<T>::Zero();
    this->_42 = Const<T>::Zero();
    this->_43 = Const<T>::Zero();
    this->_44 = Const<T>::One();
  }
};

typedef TFixed<1>  fixed1;
typedef TFixed<2>  fixed2;
typedef TFixed<3>  fixed3;
typedef TFixed<4>  fixed4;
typedef TFixed<5>  fixed5;
typedef TFixed<6>  fixed6;
typedef TFixed<7>  fixed7;
typedef TFixed<8>  fixed8;
typedef TFixed<9>  fixed9;
typedef TFixed<10> fixed10;
typedef TFixed<11> fixed11;
typedef TFixed<12> fixed12;
typedef TFixed<13> fixed13;
typedef TFixed<14> fixed14;
typedef TFixed<15> fixed15;
typedef TFixed<16> fixed16;
typedef TFixed<17> fixed17;
typedef TFixed<18> fixed18;
typedef TFixed<19> fixed19;
typedef TFixed<20> fixed20;
typedef TFixed<21> fixed21;
typedef TFixed<22> fixed22;
typedef TFixed<23> fixed23;
typedef TFixed<24> fixed24;
typedef TFixed<25> fixed25;
typedef TFixed<26> fixed26;
typedef TFixed<27> fixed27;
typedef TFixed<28> fixed28;
typedef TFixed<29> fixed29;
typedef TFixed<30> fixed30;

int32_t fxSin(int32_t x);

int32_t fxCos(int32_t x);

int32_t fxSqrt(int32_t rhs);

int32_t fxInvSqrt(int32_t x);

int32_t fxPow(int32_t x, int32_t y);

int32_t fxExp2(int32_t x);

int Inverse32(int value);

} // namespace cocogfx

namespace std {

using namespace cocogfx;

template <uint32_t F, typename T>
TFixed<F, T> abs(TFixed<F, T> rhs) {
  return TFixed<F, T>::make(::abs(rhs.data()));
}

inline fixed16 sin(fixed16 rhs) {
  return fixed16::make(fxSin(rhs.data()));
}

inline fixed16 cos(fixed16 rhs) {
  return fixed16::make(fxCos(rhs.data()));
}

inline fixed16 sqrt(fixed16 rhs) {
  return fixed16::make(fxSqrt(rhs.data()));
}

inline fixed16 pow(fixed16 lhs, fixed16 rhs) {
  return fixed16::make(fxPow(lhs.data(), rhs.data()));
}

inline fixed16 exp2(fixed16 rhs) {
  return fixed16::make(fxExp2(rhs.data()));
}

template <uint32_t F, typename T>
inline TFixed<F, T> exp(TFixed<F, T> rhs) {
  return TFixed<F, T>(std::exp(static_cast<float>(rhs)));
}

template <uint32_t F, typename T>
inline TFixed<F, T> ceil(TFixed<F, T> rhs) {
  return TFixed<F, T>::make((rhs.data() + TFixed<F, T>::MASK) & TFixed<F, T>::IMASK);
}

template <uint32_t F, typename T>
inline TFixed<F, T> floor(TFixed<F, T> rhs) {
  return TFixed<F, T>::make(rhs.data() & TFixed<F, T>::IMASK);
}

template <uint32_t F, typename T>
inline TFixed<F, T> round(TFixed<F, T> rhs) {
  return TFixed<F, T>::make((rhs.data() + TFixed<F, T>::HALF) & TFixed<F, T>::IMASK);
}

} // namespace std

///////////////////////////////////////////////////////////////////////////////

namespace cocogfx {

namespace detail {

template <typename Type, uint32_t Shift>
struct ShiftInverter {
public:
  inline static Type call(int value) {
    if constexpr (Shift >= 32) {
      return value ? static_cast<Type>((static_cast<int64_t>(1) << Shift) / value) : 0;
    } else if (Shift == 32) {
      Inverse32(value);
    } else {
      return static_cast<Type>((1 << Shift) / value);
    }
  }
};

template <typename R, uint32_t F, typename T>
class Inverter {
public:
  inline static R Invert(TFixed<F, T> rhs) {
    return R::make(
      detail::ShiftInverter<
        typename R::data_type, TFixed<F, T>::FRAC + R::FRAC>::call(rhs.data()));
  }
};

template <uint32_t F, typename T>
class Inverter<float, F, T> {
public:
  inline static float Invert(TFixed<F, T> rhs) {
    return 1.0f / static_cast<float>(rhs);
  }
};

} // namespace detail

///////////////////////////////////////////////////////////////////////////////

template <typename R>
R FromUNORM8(int rhs);

template <typename T>
int ToUNORM8(T rhs);

template <typename T>
int ToUNORM16(T rhs);

template <typename T>
bool IsAlmostZero(T rhs);

template <typename T>
T Sat(T rhs);

template <typename R, typename T>
R Ceil(T rhs);

template <typename R, typename T>
R Floori(T rhs);

template <typename R, typename T>
R Roundi(T rhs);

template <typename T>
T RSqrt(T rhs);

template <typename T>
T Dot(const TVector3<T> &lhs, const TVector3<T> &rhs);

template <typename T>
T Dot(const TVector4<T> &lhs, const TVector4<T> &rhs);

///////////////////////////////////////////////////////////////////////////////

template <>
inline fixed16 RSqrt(fixed16 rhs) {
  return fixed16::make(fxInvSqrt(rhs.data()));
}

template <>
inline float RSqrt(float rhs) {
  assert(rhs != 0.0f);
  return 1.0f / std::sqrt(rhs);
}

///////////////////////////////////////////////////////////////////////////////

inline float MulAdd(float a0, float b0, float a1, float b1, float a2, float b2, float a3, float b3) {
  return a0 * b0 + a1 * b1 + a2 * b2 + a3 * b3;
}

template <uint32_t F, typename T>
inline TFixed<F, T> MulAdd(TFixed<F, T> a0, TFixed<F, T> b0, TFixed<F, T> a1,
                          TFixed<F, T> b1, TFixed<F, T> a2, TFixed<F, T> b2,
                          TFixed<F, T> a3, TFixed<F, T> b3) {
  return TFixed<F, T>::make((static_cast<int64_t>(a0.data()) * b0.data() +
                            static_cast<int64_t>(a1.data()) * b1.data() +
                            static_cast<int64_t>(a2.data()) * b2.data() +
                            static_cast<int64_t>(a3.data()) * b3.data()) >>
                           TFixed<F, T>::FRAC);
}

inline float MulAdd(float a0, float b0, float a1, float b1, float a2, float b2) {
  return a0 * b0 + a1 * b1 + a2 * b2;
}

template <uint32_t F, typename T>
inline TFixed<F, T> MulAdd(TFixed<F, T> a0, TFixed<F, T> b0, TFixed<F, T> a1,
                           TFixed<F, T> b1, TFixed<F, T> a2, TFixed<F, T> b2) {
  return TFixed<F, T>::make((static_cast<int64_t>(a0.data()) * b0.data() +
                            static_cast<int64_t>(a1.data()) * b1.data() +
                            static_cast<int64_t>(a2.data()) * b2.data()) >>
                           TFixed<F, T>::FRAC);
}

inline float MulAdd(float a0, float b0, float a1, float b1) {
  return a0 * b0 + a1 * b1;
}

template <uint32_t F, typename T>
inline TFixed<F, T> MulAdd(TFixed<F, T> a0, TFixed<F, T> b0, TFixed<F, T> a1,
                          TFixed<F, T> b1) {
  return TFixed<F, T>::make((static_cast<int64_t>(a0.data()) * b0.data() +
                            static_cast<int64_t>(a1.data()) * b1.data()) >>
                           TFixed<F, T>::FRAC);
}

inline float MulSub(float a0, float b0, float a1, float b1) {
  return a0 * b0 - a1 * b1;
}

template <uint32_t F, typename T>
inline TFixed<F, T> MulSub(TFixed<F, T> a0, TFixed<F, T> b0, TFixed<F, T> a1,
                          TFixed<F, T> b1) {
  return TFixed<F, T>::make((static_cast<int64_t>(a0.data()) * b0.data() -
                            static_cast<int64_t>(a1.data()) * b1.data()) >>
                           TFixed<F, T>::FRAC);
}

template <>
inline float FromUNORM8<float>(int rhs) {
  return rhs * (1.0f / 255);
}

template <>
inline fixed16 FromUNORM8<fixed16>(int rhs) {
  return fixed16::make((rhs << fixed16::FRAC) / 255);
}
template <>
inline int FromUNORM8<int>(int rhs) {
  return rhs;
}

inline int Mul8(int lhs, int rhs) {
  return (lhs * rhs + 0xff) >> 8;
}

inline int Add8(int lhs, int rhs) {
  return std::min<int>(lhs + rhs, 0xff);
}

inline int Lerp8(int lhs, int rhs, int frac) {
  return lhs + ((frac * (rhs - lhs) + 0xff) >> 8);
}

template <typename T>
inline T Sat(T rhs) {
  return std::min<T>(std::max<T>(rhs, Const<T>::Zero(), Const<T>::One()));
}

template <typename T>
inline bool IsAlmostZero(T rhs) {
  return std::abs(rhs) <= Const<T>::Epsilon();
}

//////////////////////////////////////////////////////////////////////////////

template <typename R>
inline R Ceil(float rhs) {
  return static_cast<R>(::ceil(rhs));
}

template <typename R>
inline R Floori(float rhs) {
  return static_cast<R>(::floor(rhs));
}

template <typename R>
inline R Roundi(float rhs) {
  return static_cast<R>(rhs + 0.5f);
}

template <>
inline int ToUNORM8(float rhs) {
  assert((rhs >= 0.0f) && (rhs <= 1.0f));
  return static_cast<int>(0xff * rhs);
}

template <>
inline int ToUNORM16(float rhs) {
  assert((rhs >= 0.0f) && (rhs <= 1.0f));
  return static_cast<int>(0xffff * rhs);
}

inline float FastMul(float lhs, float rhs) {
  return lhs * rhs;
}

template <typename R>
inline R Mul(float lhs, float rhs) {
  return static_cast<R>(lhs * rhs);
}

template <typename R>
inline R Mul(float lhs, int rhs) {
  return static_cast<R>(lhs * rhs);
}

template <typename R, uint32_t F, typename T>
inline R Mul(float lhs, TFixed<F, T> rhs) {
  return static_cast<R>(lhs * rhs);
}

inline float MulDiv(float lhs, float rhs) {
  return lhs / rhs;
}

inline float Lerpf(float lhs, float rhs, float scalar) {
  assert((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + (rhs - lhs) * scalar;
}

inline int Lerp(int lhs, int rhs, float scalar) {
  assert((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + static_cast<int>((rhs - lhs) * scalar);
}

template <typename R>
inline R MulRnd(float a, float b) {
  return static_cast<R>(a * b);
}

template <typename R>
inline R MulRnd(float a, float b, float c) {
  return static_cast<R>(a * b * c);
}

template <typename R>
inline R MulSub(float a, float b, float c, float d) {
  return static_cast<R>(a * b - c * d);
}

template <typename R>
inline R ShiftLeft(float lhs, int rhs) {
  float scale = static_cast<float>(1 << rhs);
  return static_cast<R>(lhs * scale);
}

template <typename R>
inline R ShiftRight(float lhs, int rhs) {
  float scale = 1.0f / static_cast<float>(1 << rhs);
  return static_cast<R>(lhs * scale);
}

//////////////////////////////////////////////////////////////////////////////

template <typename R, uint32_t F, typename T>
inline R Ceil(TFixed<F, T> rhs) {
  int value = rhs.data() + TFixed<F, T>::MASK;
  return static_cast<R>(value >> TFixed<F, T>::FRAC);
}

template <typename R, uint32_t F, typename T>
inline R Floori(TFixed<F, T> rhs) {
  return static_cast<R>(rhs.data() >> TFixed<F, T>::FRAC);
}

template <typename R, uint32_t F, typename T>
inline R Roundi(TFixed<F, T> rhs) {
  return static_cast<R>((rhs.data() + TFixed<F, T>::HALF) >> TFixed<F, T>::FRAC);
}

template <uint32_t F, typename T>
inline int ToUNORM8(TFixed<F, T> rhs) {
  assert((rhs >= Zero<TFixed<F, T>>()) &&
         (rhs <= One<TFixed<F, T>>()));
  return (0xff * rhs.data()) >> TFixed<F, T>::FRAC;
}

template <uint32_t F, typename T>
inline int ToUNORM16(TFixed<F, T> rhs) {
  assert((rhs >= Zero<TFixed<F, T>>()) &&
         (rhs <= One<TFixed<F, T>>()));
  return (0xffff * rhs.data()) >> TFixed<F, T>::FRAC;
}

template <typename R, uint32_t F, typename T>
inline R Inverse(TFixed<F, T> rhs) {
  return detail::Inverter<R, F, T>::Invert(rhs);
}

template <typename R>
inline R Inverse(float rhs) {
  assert(rhs != 0.0f);
  return static_cast<R>(1.0f / rhs);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R FastMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  assert((static_cast<int64_t>(lhs.data()) * rhs.data()) ==
         (lhs.data() * rhs.data()));
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  return R::make((lhs.data() * rhs.data()) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R FastDiv(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  assert((static_cast<int64_t>(lhs.data()) << FRAC) == (lhs.data() << FRAC));
  return R::make((lhs.data() << FRAC) / rhs.data());
}

template <typename R, uint32_t F, typename T>
inline R MulRnd(TFixed<F, T> lhs, int rhs) {
  int FRAC = TFixed<F, T>::FRAC - R::FRAC;
  int HALF = 1 << (FRAC - 1);
  auto value = static_cast<int64_t>(lhs.data()) * rhs;
  return R::make((value + HALF) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R MulRnd(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  int HALF = 1 << (FRAC - 1);
  auto value = static_cast<int64_t>(lhs.data()) * rhs.data();
  return R::make((value + HALF) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R MulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, int c) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto HALF = static_cast<int64_t>(1) << (FRAC - 1);
  auto value = static_cast<int64_t>(a.data()) * b.data() * c;
  return R::make((value + HALF) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, uint32_t F3, typename T1,
          typename T2, typename T3>
inline R MulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F3, T3> c) {
  int FRAC =
      TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC + TFixed<F3, T3>::FRAC - R::FRAC;
  auto HALF = static_cast<int64_t>(1) << (FRAC - 1);
  int64_t value = static_cast<int64_t>(a.data()) * b.data() * c.data();
  return R::make((value + HALF) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R MulAdd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c,
                TFixed<F2, T2> d) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto ab = static_cast<int64_t>(a.data()) * b.data();
  auto cd = static_cast<int64_t>(c.data()) * d.data();
  return R::make((ab + cd) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R MulSub(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c,
                TFixed<F2, T2> d) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto ab = static_cast<int64_t>(a.data()) * b.data();
  auto cd = static_cast<int64_t>(c.data()) * d.data();
  return R::make((ab - cd) >> FRAC);
}

template <typename R, uint32_t F, typename T>
inline R Mul(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  int FRAC = TFixed<F, T>::FRAC + TFixed<F, T>::FRAC - R::FRAC;
  auto value = static_cast<int64_t>(lhs.data()) * rhs.data();
  return R::make(value >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R Mul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto value = static_cast<int64_t>(lhs.data()) * rhs.data();
  return R::make(value >> FRAC);
}

template <typename R, uint32_t F, typename T>
inline R Mul(TFixed<F, T> lhs, int rhs) {
  int FRAC = TFixed<F, T>::FRAC - R::FRAC;
  auto value = static_cast<int64_t>(lhs.data()) * rhs;
  return R::make(value >> FRAC);
}

template <typename R, uint32_t F, typename T>
R Mul(TFixed<F, T> lhs, float rhs) {
  return static_cast<R>(rhs * lhs);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R MulShift(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs, R a, int shift) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto value = static_cast<int64_t>(lhs.data()) * rhs.data();
  return R::make((value >> FRAC) - (a.data() >> shift));
}

template <uint32_t F1, uint32_t F2, typename T1, typename T2>
inline TFixed<F1, T1> Lerpf(TFixed<F1, T1> lhs, TFixed<F1, T1> rhs,
                           TFixed<F2, T2> scalar) {
  assert((scalar >= Zero<TFixed<F2, T2>>()) &&
         (scalar <= One<TFixed<F2, T2>>()));
  return lhs + (rhs - lhs) * scalar;
}

template <uint32_t F, typename T>
inline TFixed<F, T> Lerpf(TFixed<F, T> lhs, TFixed<F, T> rhs, float scalar) {
  assert((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + (rhs - lhs) * scalar;
}

template <uint32_t F, typename T>
inline int Lerp(int lhs, int rhs, TFixed<F, T> scalar) {
  assert((scalar >= Zero<TFixed<F, T>>()) &&
         (scalar <= One<TFixed<F, T>>()));
  return lhs + static_cast<int>((static_cast<int64_t>(rhs - lhs) * scalar.data()) >> TFixed<F, T>::FRAC);
}

template <typename R, uint32_t F, typename T>
inline R ShiftLeft(TFixed<F, T> lhs, int rhs) {
  return R(lhs) << rhs;
}

template <typename R, uint32_t F, typename T>
inline R ShiftRight(TFixed<F, T> lhs, int rhs) {
  return R(lhs) >> rhs;
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline fixed16 Dot<fixed16>(const TVector3<fixed16> &lhs, const TVector3<fixed16> &rhs) {
  auto xx = static_cast<int64_t>(lhs.x.data()) * rhs.x.data();
  auto yy = static_cast<int64_t>(lhs.y.data()) * rhs.y.data();
  auto zz = static_cast<int64_t>(lhs.z.data()) * rhs.z.data();
  return fixed16::make((xx + yy + zz) >> fixed16::FRAC);
}

template <>
inline fixed16 Dot<fixed16>(const TVector4<fixed16> &lhs, const TVector4<fixed16> &rhs) {
  auto xx = static_cast<int64_t>(lhs.x.data()) * rhs.x.data();
  auto yy = static_cast<int64_t>(lhs.y.data()) * rhs.y.data();
  auto zz = static_cast<int64_t>(lhs.z.data()) * rhs.z.data();
  auto ww = static_cast<int64_t>(lhs.w.data()) * rhs.w.data();
  return fixed16::make((xx + yy + zz + ww) >> fixed16::FRAC);
}

template <>
inline float Dot<float>(const TVector3<float> &lhs, const TVector3<float> &rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <>
inline float Dot<float>(const TVector4<float> &lhs, const TVector4<float> &rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template <typename T>
void Mul(TVector4<T> *pvOut, const TVector3<T> &vIn, const TMatrix44<T> &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32) + mat._42;
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33) + mat._43;
  pvOut->w = MulAdd(x, mat._14, y, mat._24, z, mat._34) + mat._44;
}

template <typename T>
void Mul4x3(TVector3<T> *pvOut, const TVector3<T> &vIn, const TMatrix44<T> &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32) + mat._42;
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33) + mat._43;
}

template <typename T>
void Mul(TVector3<T> *pvOut, const TVector3<T> &vIn, const TMatrix44<T> &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32);
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33);
}

template <typename T>
void Mul(TVector4<T> *pvOut, const TVector4<T> &vIn, const TMatrix44<T> &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;
  auto w = vIn.w;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31, w, mat._41);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32, w, mat._42);
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33, w, mat._43);
  pvOut->w = MulAdd(x, mat._14, y, mat._24, z, mat._34, w, mat._44);
}

template <typename T>
void Mul(TVector3<T> *pvOut, const TVector4<T> &vIn, const TMatrix44<T> &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;
  auto w = vIn.w;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31, w, mat._41);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32, w, mat._42);
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33, w, mat._43);
}

template <typename T>
void Mul(TVector2<T> *pvOut, const TVector4<T> &vIn, const TMatrix44<T> &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;
  auto w = vIn.w;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31, w, mat._41);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32, w, mat._42);
}

template <typename T>
void Mul(TVector2<T> *pvOut, const TVector2<T> &vIn, const TMatrix44<T> &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;

  pvOut->x = MulAdd(x, mat._11, y, mat._21) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22) + mat._42;
}

template <typename T>
T Length(const TVector3<T> &vIn) {
  auto fDot = Dot<T>(vIn, vIn);
  if (!IsAlmostZero(fDot - Const<T>::One())) {
    return std::sqrt(fDot);
  }
  return fDot;
}

template <typename T>
void Normalize(TVector3<T> *pvOut) {
  assert(pvOut);
  auto fDot = Dot<T>(*pvOut, *pvOut);
  if (!IsAlmostZero(fDot - Const<T>::One()) &&
      !IsAlmostZero(fDot)) {
    auto fInvLen = RSqrt(fDot);
    pvOut->x = pvOut->x * fInvLen;
    pvOut->y = pvOut->y * fInvLen;
    pvOut->z = pvOut->z * fInvLen;
  }
}

template <typename T>
void Neg(TVector3<T> *pvOut, const TVector3<T> &vIn) {
  pvOut->x = -vIn.x;
  pvOut->y = -vIn.y;
  pvOut->z = -vIn.z;
}

template <typename T>
void Add(TVector3<T> *pvOut, const TVector3<T> &v1, const TVector3<T> &v2) {
  pvOut->x = v1.x + v2.x;
  pvOut->y = v1.y + v2.y;
  pvOut->z = v1.z + v2.z;
}

template <typename T>
void Transpose(TMatrix44<T> *pOut, const TMatrix44<T> &matIn) {
  assert(pOut);

  pOut->_11 = matIn._11;
  pOut->_12 = matIn._21;
  pOut->_13 = matIn._31;
  pOut->_14 = matIn._41;

  pOut->_21 = matIn._12;
  pOut->_22 = matIn._22;
  pOut->_23 = matIn._32;
  pOut->_24 = matIn._42;

  pOut->_31 = matIn._13;
  pOut->_32 = matIn._23;
  pOut->_33 = matIn._33;
  pOut->_34 = matIn._43;

  pOut->_41 = matIn._14;
  pOut->_42 = matIn._24;
  pOut->_43 = matIn._34;
  pOut->_44 = matIn._44;
}

template <typename T>
void Transpose33(TMatrix44<T> *pOut, const TMatrix44<T> &matIn) {
  assert(pOut);

  pOut->_11 = matIn._11;
  pOut->_12 = matIn._21;
  pOut->_13 = matIn._31;

  pOut->_21 = matIn._12;
  pOut->_22 = matIn._22;
  pOut->_23 = matIn._32;

  pOut->_31 = matIn._13;
  pOut->_32 = matIn._23;
  pOut->_33 = matIn._33;
}

template <typename T>
void Scale(TMatrix44<T> *pOut, T x, T y, T z) {
  assert(pOut);

  pOut->toIdentity();

  pOut->_11 = x;
  pOut->_22 = y;
  pOut->_33 = z;
}

template <typename T>
void Translate(TMatrix44<T> *pOut, T x, T y, T z) {
  assert(pOut);

  pOut->toIdentity();

  pOut->_41 = x;
  pOut->_42 = y;
  pOut->_43 = z;
}

inline bool IsPowerOf2(int value) {
  return 0 == (value & (value - 1));
}

template <typename T>
inline T DegToRad(T value) {
  return value * (Const<T>::PI() / Const<T>::F180());
}

template <typename T>
inline T RadToDeg(T value) {
  return value * (Const<T>::F180() / Const<T>::PI());
}

inline int iLog2(int value) {
  return 31 - __builtin_clz(value);
}

template <typename T>
void Mul(TMatrix44<T> *pOut, const TMatrix44<T> &mat1, const TMatrix44<T> &mat2) {
  assert(pOut);

  pOut->_11 = MulAdd(mat1._11, mat2._11, mat1._21, mat2._12, mat1._31,
                        mat2._13, mat1._41, mat2._14);
  pOut->_12 = MulAdd(mat1._12, mat2._11, mat1._22, mat2._12, mat1._32,
                        mat2._13, mat1._42, mat2._14);
  pOut->_13 = MulAdd(mat1._13, mat2._11, mat1._23, mat2._12, mat1._33,
                        mat2._13, mat1._43, mat2._14);
  pOut->_14 = MulAdd(mat1._14, mat2._11, mat1._24, mat2._12, mat1._34,
                        mat2._13, mat1._44, mat2._14);

  pOut->_21 = MulAdd(mat1._11, mat2._21, mat1._21, mat2._22, mat1._31,
                        mat2._23, mat1._41, mat2._24);
  pOut->_22 = MulAdd(mat1._12, mat2._21, mat1._22, mat2._22, mat1._32,
                        mat2._23, mat1._42, mat2._24);
  pOut->_23 = MulAdd(mat1._13, mat2._21, mat1._23, mat2._22, mat1._33,
                        mat2._23, mat1._43, mat2._24);
  pOut->_24 = MulAdd(mat1._14, mat2._21, mat1._24, mat2._22, mat1._34,
                        mat2._23, mat1._44, mat2._24);

  pOut->_31 = MulAdd(mat1._11, mat2._31, mat1._21, mat2._32, mat1._31,
                        mat2._33, mat1._41, mat2._34);
  pOut->_32 = MulAdd(mat1._12, mat2._31, mat1._22, mat2._32, mat1._32,
                        mat2._33, mat1._42, mat2._34);
  pOut->_33 = MulAdd(mat1._13, mat2._31, mat1._23, mat2._32, mat1._33,
                        mat2._33, mat1._43, mat2._34);
  pOut->_34 = MulAdd(mat1._14, mat2._31, mat1._24, mat2._32, mat1._34,
                        mat2._33, mat1._44, mat2._34);

  pOut->_41 = MulAdd(mat1._11, mat2._41, mat1._21, mat2._42, mat1._31,
                        mat2._43, mat1._41, mat2._44);
  pOut->_42 = MulAdd(mat1._12, mat2._41, mat1._22, mat2._42, mat1._32,
                        mat2._43, mat1._42, mat2._44);
  pOut->_43 = MulAdd(mat1._13, mat2._41, mat1._23, mat2._42, mat1._33,
                        mat2._43, mat1._43, mat2._44);
  pOut->_44 = MulAdd(mat1._14, mat2._41, mat1._24, mat2._42, mat1._34,
                        mat2._43, mat1._44, mat2._44);
}

template <typename T>
void Inverse33(TMatrix44<T> *pOut, const TMatrix44<T> &matIn) {
  assert(pOut);

  pOut->_11 = MulSub(matIn._22, matIn._33, matIn._23, matIn._32);
  pOut->_12 = -MulSub(matIn._21, matIn._33, matIn._23, matIn._31);
  pOut->_13 = MulSub(matIn._21, matIn._32, matIn._22, matIn._31);

  pOut->_21 = -MulSub(matIn._12, matIn._33, matIn._13, matIn._32);
  pOut->_22 = MulSub(matIn._11, matIn._33, matIn._13, matIn._31);
  pOut->_23 = -MulSub(matIn._11, matIn._32, matIn._12, matIn._31);

  pOut->_31 = MulSub(matIn._12, matIn._23, matIn._13, matIn._22);
  pOut->_32 = -MulSub(matIn._11, matIn._23, matIn._13, matIn._21);
  pOut->_33 = MulSub(matIn._11, matIn._22, matIn._12, matIn._21);

  auto fDet = MulAdd(matIn._11, pOut->_11, matIn._21, pOut->_21,
                     matIn._31, pOut->_31);

  if (!IsAlmostZero(fDet - Const<T>::One())) {
    auto fDetInv = Inverse<T>(fDet);

    pOut->_11 *= fDetInv;
    pOut->_12 *= fDetInv;
    pOut->_13 *= fDetInv;

    pOut->_21 *= fDetInv;
    pOut->_22 *= fDetInv;
    pOut->_23 *= fDetInv;

    pOut->_31 *= fDetInv;
    pOut->_32 *= fDetInv;
    pOut->_33 *= fDetInv;
  }
}

template <typename T>
void Inverse(TMatrix44<T> *pOut, const TMatrix44<T> &matIn) {
  assert(pOut);

  auto fB0 = MulSub(matIn._13, matIn._24, matIn._23, matIn._14);
  auto fB1 = MulSub(matIn._13, matIn._34, matIn._33, matIn._14);
  auto fB2 = MulSub(matIn._13, matIn._44, matIn._43, matIn._14);
  auto fB3 = MulSub(matIn._23, matIn._34, matIn._33, matIn._24);
  auto fB4 = MulSub(matIn._23, matIn._44, matIn._43, matIn._24);
  auto fB5 = MulSub(matIn._33, matIn._44, matIn._43, matIn._34);

  pOut->_11 = (matIn._22 * fB5 - matIn._32 * fB4 + matIn._42 * fB3);
  pOut->_12 = (-matIn._12 * fB5 + matIn._32 * fB2 - matIn._42 * fB1);
  pOut->_13 = (matIn._12 * fB4 - matIn._22 * fB2 + matIn._42 * fB0);
  pOut->_14 = (-matIn._12 * fB3 + matIn._22 * fB1 - matIn._32 * fB0);

  pOut->_21 = (-matIn._21 * fB5 + matIn._31 * fB4 - matIn._41 * fB3);
  pOut->_22 = (matIn._11 * fB5 - matIn._31 * fB2 + matIn._41 * fB1);
  pOut->_23 = (-matIn._11 * fB4 + matIn._21 * fB2 - matIn._41 * fB0);
  pOut->_24 = (matIn._11 * fB3 - matIn._21 * fB1 + matIn._31 * fB0);

  auto fA0 = MulSub(matIn._11, matIn._22, matIn._21, matIn._12);
  auto fA1 = MulSub(matIn._11, matIn._32, matIn._31, matIn._12);
  auto fA2 = MulSub(matIn._11, matIn._42, matIn._41, matIn._12);
  auto fA3 = MulSub(matIn._21, matIn._32, matIn._31, matIn._22);
  auto fA4 = MulSub(matIn._21, matIn._42, matIn._41, matIn._22);
  auto fA5 = MulSub(matIn._31, matIn._42, matIn._41, matIn._32);

  pOut->_31 = (matIn._24 * fA5 - matIn._34 * fA4 + matIn._44 * fA3);
  pOut->_32 = (-matIn._14 * fA5 + matIn._34 * fA2 - matIn._44 * fA1);
  pOut->_33 = (matIn._14 * fA4 - matIn._24 * fA2 + matIn._44 * fA0);
  pOut->_34 = (-matIn._14 * fA3 + matIn._24 * fA1 - matIn._34 * fA0);

  pOut->_41 = (-matIn._23 * fA5 + matIn._33 * fA4 - matIn._43 * fA3);
  pOut->_42 = (matIn._13 * fA5 - matIn._33 * fA2 + matIn._43 * fA1);
  pOut->_43 = (-matIn._13 * fA4 + matIn._23 * fA2 - matIn._43 * fA0);
  pOut->_44 = (matIn._13 * fA3 - matIn._23 * fA1 + matIn._33 * fA0);

  auto fDet = MulAdd(fA0, fB5, fA2, fB3, fA3, fB2, fA5, fB0) -
              MulAdd(fA1, fB4, fA4, fB1);

  if (!IsAlmostZero(fDet - Const<T>::One())) {
    auto fDetInv = Inverse<T>(fDet);

    pOut->_11 *= fDetInv;
    pOut->_12 *= fDetInv;
    pOut->_13 *= fDetInv;
    pOut->_14 *= fDetInv;

    pOut->_21 *= fDetInv;
    pOut->_22 *= fDetInv;
    pOut->_23 *= fDetInv;
    pOut->_24 *= fDetInv;

    pOut->_31 *= fDetInv;
    pOut->_32 *= fDetInv;
    pOut->_33 *= fDetInv;
    pOut->_34 *= fDetInv;

    pOut->_41 *= fDetInv;
    pOut->_42 *= fDetInv;
    pOut->_43 *= fDetInv;
    pOut->_44 *= fDetInv;
  }
}

template <typename T>
void Ortho(TMatrix44<T> *pOut, T left, T right, T bottom, T top, T zNear, T zFar) {
  assert(pOut);

  auto fWidth = right - left;
  auto fHeight = top - bottom;
  auto fDepth = zFar - zNear;

  auto fInvWidth = (fWidth != Const<T>::Zero())
                       ? Inverse<T>(fWidth)
                       : Const<T>::Zero();
  auto fInvHeight = (fHeight != Const<T>::Zero())
                        ? Inverse<T>(fHeight)
                        : Const<T>::Zero();
  auto fInvDepth = (fDepth != Const<T>::Zero())
                       ? Inverse<T>(fDepth)
                       : Const<T>::Zero();

  pOut->_11 = Const<T>::Two() * fInvWidth;
  pOut->_12 = Const<T>::Zero();
  pOut->_13 = Const<T>::Zero();
  pOut->_14 = Const<T>::Zero();

  pOut->_21 = Const<T>::Zero();
  pOut->_22 = Const<T>::Two() * fInvHeight;
  pOut->_23 = Const<T>::Zero();
  pOut->_24 = Const<T>::Zero();

  pOut->_31 = Const<T>::Zero();
  pOut->_32 = Const<T>::Zero();
  pOut->_33 = -Const<T>::Two() * fInvDepth;
  pOut->_34 = Const<T>::Zero();

  pOut->_41 = -(right + left) * fInvWidth;
  pOut->_42 = -(top + bottom) * fInvHeight;
  pOut->_43 = -(zFar + zNear) * fInvDepth;
  pOut->_44 = Const<T>::One();
}

template <typename T>
void Frustum(TMatrix44<T> *pOut, T left, T right, T bottom, T top, T zNear, T zFar) {
  assert(pOut);

  auto fWidth = right - left;
  auto fHeight = top - bottom;
  auto fDepth = zFar - zNear;

  auto fInvWidth = (fWidth != Const<T>::Zero())
                       ? Inverse<T>(fWidth)
                       : Const<T>::Zero();
  auto fInvHeight = (fHeight != Const<T>::Zero())
                        ? Inverse<T>(fHeight)
                        : Const<T>::Zero();
  auto fInvDepth = (fDepth != Const<T>::Zero())
                       ? Inverse<T>(fDepth)
                       : Const<T>::Zero();

  auto fTwoNear = Const<T>::Two() * zNear;

  pOut->_11 = fTwoNear * fInvWidth;
  pOut->_12 = Const<T>::Zero();
  pOut->_13 = Const<T>::Zero();
  pOut->_14 = Const<T>::Zero();

  pOut->_21 = Const<T>::Zero();
  pOut->_22 = fTwoNear * fInvHeight;
  pOut->_23 = Const<T>::Zero();
  pOut->_24 = Const<T>::Zero();

  pOut->_31 = (right + left) * fInvWidth;
  pOut->_32 = (top + bottom) * fInvHeight;
  pOut->_33 = -(zFar + zNear) * fInvDepth;
  pOut->_34 = -Const<T>::One();

  pOut->_41 = Const<T>::Zero();
  pOut->_42 = Const<T>::Zero();
  pOut->_43 = -zFar * (fTwoNear * fInvDepth);
  pOut->_44 = Const<T>::Zero();
}

template <typename T>
void Rotate(TMatrix44<T> *pOut, T angle, T x, T y, T z) {
  assert(pOut);

  TVector3<T> vAxis(x, y, z);
  Normalize(&vAxis);

  auto fSin = std::sin(angle);
  auto fCos = std::cos(angle);
  auto fICos = Const<T>::One() - fCos;

  auto _fA = fICos * vAxis.x * vAxis.y;
  auto _fB = fICos * vAxis.x * vAxis.z;
  auto _fC = fICos * vAxis.y * vAxis.z;

  pOut->_11 = fICos * vAxis.x * vAxis.x + fCos;
  pOut->_12 = _fA + vAxis.z * fSin;
  pOut->_13 = _fB - vAxis.y * fSin;
  pOut->_14 = Const<T>::Zero();

  pOut->_21 = _fA - vAxis.z * fSin;
  pOut->_22 = fICos * vAxis.y * vAxis.y + fCos;
  pOut->_23 = _fC + vAxis.x * fSin;
  pOut->_24 = Const<T>::Zero();

  pOut->_31 = _fB + vAxis.y * fSin;
  pOut->_32 = _fC - vAxis.x * fSin;
  pOut->_33 = fICos * vAxis.z * vAxis.z + fCos;
  pOut->_34 = Const<T>::Zero();

  pOut->_41 = Const<T>::Zero();
  pOut->_42 = Const<T>::Zero();
  pOut->_43 = Const<T>::Zero();
  pOut->_44 = Const<T>::One();
}

template <typename T>
bool Intersect(const TRect<T> *pSrc1, const TRect<T> *pSrc2) {
  return !(pSrc1->left >= pSrc2->right 
        || pSrc1->right <= pSrc2->left 
        || pSrc1->top >= pSrc2->bottom 
        || pSrc1->bottom <= pSrc2->top);
}

template <typename T>
bool Intersect(TRect<T> *pDst, const TRect<T> *pSrc1, const TRect<T> *pSrc2) {
  assert(pDst);
  auto left   = std::max<T>(pSrc1->left,   pSrc2->left);
  auto top    = std::max<T>(pSrc1->top,    pSrc2->top);
  auto right  = std::max<T>(pSrc1->right,  pSrc2->right);
  auto bottom = std::max<T>(pSrc1->bottom, pSrc2->bottom);
  if (left >= right || top >= bottom)
    return false;
  pDst->left   = left;
  pDst->top    = top;
  pDst->right  = right;
  pDst->bottom = bottom;
  return true;
}

}