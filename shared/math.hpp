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

#include <math.h>

template <class T> struct TConst {};

template <> struct TConst<float> {
  inline static float Zero() { return 0.0f; }

  inline static float Half() { return 0.5f; }

  inline static float One() { return 1.0f; }

  inline static float Two() { return 2.0f; }

  inline static float Min() { return -2147483647.0f; }

  inline static float Max() { return 2147483647.0f; }

  inline static float Epsilon() { return 0.00000001f; }

  inline static float PI() { return 3.14159265f; }

  inline static float F128() { return 128.0f; }

  inline static float F90() { return 90.0f; }

  inline static float F180() { return 180.0f; }

  inline static float F02() { return 0.2f; }

  inline static float F08() { return 0.8f; }

  inline static float E() { return 2.71828183f; }

  inline static float LogE() { return 1.442695041f; }
};

template <uint32_t F, typename T> struct TConst<TFixed<F, T>> {
  inline static auto Zero() { return TFixed<F, T>::make(0); }

  inline static auto Half() { return TFixed<F, T>::make(TFixed<F, T>::HALF); }

  inline static auto One() { return TFixed<F, T>::make(TFixed<F, T>::ONE); }

  inline static auto Two() { return TFixed<F, T>::make(TFixed<F, T>::TWO); }

  inline static auto Min() {
    return TFixed<F, T>::make(-(1 << (8 * sizeof(T) - 1)));
  }

  inline static auto Max() {
    return TFixed<F, T>::make((1 << (8 * sizeof(T) - 1)) - 1);
  }

  inline static auto Epsilon() { return TFixed<F, T>::make(1); }

  inline static auto PI() {
    return TFixed<F, T>::make(3.1415926535f * TFixed<F, T>::ONE);
  }

  inline static auto F128() {
    return TFixed<F, T>::make(128 * TFixed<F, T>::ONE);
  }

  inline static auto F90() {
    return TFixed<F, T>::make(90 * TFixed<F, T>::ONE);
  }

  inline static auto F180() {
    return TFixed<F, T>::make(180 * TFixed<F, T>::ONE);
  }

  inline static auto F02() {
    return TFixed<F, T>::make(0.2f * TFixed<F, T>::ONE);
  }

  inline static auto F08() {
    return TFixed<F, T>::make(0.8f * TFixed<F, T>::ONE);
  }

  inline static auto E() {
    return TFixed<F, T>::make(2.71828183f * TFixed<F, T>::ONE);
  }

  inline static auto LogE() {
    return TFixed<F, T>::make(1.442695041f * TFixed<F, T>::ONE);
  }
};

template <class T> struct TVector1 {
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

  TVector1(T X) : x(X) {}

  TVector1(const TVector1 &rhs) { this->x = rhs.x; }
};

template <class T> struct TVector2 {
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

  TVector2(T X, T Y) : x(X), y(Y) {}

  TVector2(const TVector2 &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
  }
};

template <class T> struct TVector3 {
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

  TVector3(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

  TVector3(const TVector3 &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
  }
};

template <class T> struct TVector4 {
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

  TVector4(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

  TVector4(const TVector4 &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->w = rhs.w;
  }
};

template <class T> struct TMatrix44 {
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

  TMatrix44(T m11, T m12, T m13, T m14, T m21, T m22, T m23, T m24, T m31,
            T m32, T m33, T m34, T m41, T m42, T m43, T m44) {
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
    this->_11 = TConst<T>::One();
    this->_12 = TConst<T>::Zero();
    this->_13 = TConst<T>::Zero();
    this->_14 = TConst<T>::Zero();
    this->_21 = TConst<T>::Zero();
    this->_22 = TConst<T>::One();
    this->_23 = TConst<T>::Zero();
    this->_24 = TConst<T>::Zero();
    this->_31 = TConst<T>::Zero();
    this->_32 = TConst<T>::Zero();
    this->_33 = TConst<T>::One();
    this->_34 = TConst<T>::Zero();
    this->_41 = TConst<T>::Zero();
    this->_42 = TConst<T>::Zero();
    this->_43 = TConst<T>::Zero();
    this->_44 = TConst<T>::One();
  }
};

typedef TFixed<4> fixed4;
typedef TFixed<4> fixed6;
typedef TFixed<8> fixed8;
typedef TFixed<10> fixed10;
typedef TFixed<12> fixed12;
typedef TFixed<14> fixed14;
typedef TFixed<16> fixed16;
typedef TFixed<18> fixed18;
typedef TFixed<20> fixed20;
typedef TFixed<22> fixed22;
typedef TFixed<24> fixed24;
typedef TFixed<26> fixed26;
typedef TFixed<28> fixed28;
typedef TFixed<30> fixed30;

#ifdef COCOGL_PIXEDPOINT

typedef fixed4 float4;
typedef fixed6 float6;
typedef fixed8 float8;
typedef fixed10 float10;
typedef fixed12 float12;
typedef fixed14 float14;
typedef fixed16 floatf;
typedef fixed18 float18;
typedef fixed20 float20;
typedef fixed24 float24;
typedef fixed26 float26;
typedef fixed28 float28;
typedef fixed30 float30;

#else

typedef float float4;
typedef float float6;
typedef float float8;
typedef float float10;
typedef float float12;
typedef float float14;
typedef float floatf;
typedef float float18;
typedef float float20;
typedef float float24;
typedef float float26;
typedef float float28;
typedef float float30;

#endif

typedef TVector1<floatf> VECTOR1;
typedef TVector2<floatf> VECTOR2;
typedef TVector3<floatf> VECTOR3;
typedef TVector4<floatf> VECTOR4;
typedef TMatrix44<floatf> MATRIX44;

#define fZERO TConst<floatf>::Zero()
#define fHALF TConst<floatf>::Half()
#define fONE TConst<floatf>::One()
#define fTWO TConst<floatf>::Two()
#define fPI TConst<floatf>::PI()
#define f128 TConst<floatf>::F128()
#define f90 TConst<floatf>::F90()
#define f180 TConst<floatf>::F180()
#define f02 TConst<floatf>::F02()
#define f08 TConst<floatf>::F08()
#define fEPS TConst<floatf>::Epsilon()

namespace Math {

uint32_t iLog2(uint32_t rhs);

uint32_t iSqrt(uint32_t rhs);

int Mul8(int lhs, int rhs);

int Add8(int lhs, int rhs);

int Lerp8(int lhs, int rhs, int frac);

int Inverse32(int value);

template <class R> R TCast(float rhs);

template <class R> R TCast(int rhs);

template <class R> R TCast(uint32_t rhs);

template <class R> R TCast(short rhs);

template <class R> R TCast(uint16_t rhs);

template <class R> R TCast(char rhs);

template <class R> R TCast(uint8_t rhs);

template <class R> R TCast(bool rhs);

template <class R, uint32_t F, typename T> R TCast(TFixed<F, T> rhs);

template <class R> R TFromUNORM8(int rhs);

template <class T> int TToUNORM8(T rhs);

template <class T> int TToUNORM16(T rhs);

template <class T> bool TIsZero(T rhs);

template <class T> T TAbs(T rhs);

template <class T> T TMin(T lhs, T rhs);

template <class T> T TMax(T lhs, T rhs);

template <class T> T TClamp(T rhs, T min, T max);

template <class T> T TSat(T rhs);

template <class R, class T> R TCeili(T rhs);

template <class R, class T> R TFloori(T rhs);

template <class R, class T> R TRoundi(T rhs);

template <class R, class T> R TCeilf(T rhs);

template <class R, class T> R TFloorf(T rhs);

template <class R, class T> R TRoundf(T rhs);

template <class T> T TPow(T lhs, T rhs);

template <class T> T TPow2(T rhs);

template <class T> T TExp(T rhs);

template <class T> T TLog(T rhs);

template <class T> T TMod(T lhs, T rhs);

template <class R, class T> R TDiv(T lhs, T rhs);

template <class T> T TSqrt(T rhs);

template <class T> T TInvSqrt(T rhs);

template <class T> T TSin(T rhs);

template <class T> T TCos(T rhs);

//////////////////////////////////////////////////////////////////////////////

float TFastMul(float lhs, float rhs);

template <class R> R TMul(float lhs, float rhs);

template <class R> R TMul(float lhs, int rhs);

template <class R, uint32_t F, typename T> R TMul(float lhs, TFixed<F, T> rhs);

float MulDiv(float lhs, float rhs);

float Lerpf(float lhs, float rhs, float scalar);

int Lerp(int lhs, int rhs, float scalar);

template <class R> R TMul(float fOffset, float fRhw, int iScale);

template <class R> R TMul(float fOffset, float fRhw, float fScale);

template <class R> R TMulRnd(float lhs, int rhs);

template <class R> R TMulRnd(float lhs, float rhs);

template <class R> R TMulRnd(float a, float b, int c);

template <class R> R TMulRnd(float a, float b, float c);

template <class R> R TMulSub(float a, float b, float c, float d);

template <class R> R TShiftLeft(float lhs, int rhs);

template <class R> R TShiftRight(float lhs, int rhs);

//////////////////////////////////////////////////////////////////////////////

template <class R, uint32_t F, typename T> 
R TInv(TFixed<F, T> rhs);

template <class R> 
R TInv(float rhs);

template <class R, uint32_t F1, uint32_t F2, typename T1, typename T2>
R TFastMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs);

template <class R, uint32_t F1, uint32_t F2, typename T1, typename T2>
R TFastDiv(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs);

template <class R, uint32_t F1, uint32_t F2, typename T1, typename T2>
R TMulRnd(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs);

template <class R, uint32_t F1, uint32_t F2, typename T1, typename T2>
R TMulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, int c);

template <class R, uint32_t F1, uint32_t F2, uint32_t F3, typename T1,
          typename T2, typename T3>
R TMulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F3, T3> c);

template <class R, uint32_t F, typename T> 
R TMulRnd(TFixed<F, T> lhs, int rhs);

template <class R, uint32_t F1, uint32_t F2, typename T1, typename T2>
R TMulAdd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c, TFixed<F2, T2> d);

template <class R, uint32_t F1, uint32_t F2, typename T1, typename T2>
R TMulSub(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c, TFixed<F2, T2> d);

template <class R, uint32_t F, typename T> 
R TMul(TFixed<F, T> lhs, int rhs);

template <class R, uint32_t F, typename T> 
R TMul(TFixed<F, T> lhs, float rhs);

template <class R, uint32_t F1, uint32_t F2, typename T1, typename T2>
R TMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs);

template <class R, uint32_t F, typename T>
R TMul(TFixed<F, T> lhs, TFixed<F, T> rhs);

template <uint32_t F1, uint32_t F2, typename T1, typename T2>
TFixed<F1, T1> TLerpf(TFixed<F1, T1> lhs, TFixed<F1, T1> rhs, TFixed<F2, T2> scalar);

template <uint32_t F, typename T>
TFixed<F, T> TLerpf(TFixed<F, T> lhs, TFixed<F, T> rhs, float scalar);

template <class R, uint32_t F, typename T>
R TShiftLeft(TFixed<F, T> lhs, int rhs);

template <class R, uint32_t F, typename T>
R TShiftRight(TFixed<F, T> lhs, int rhs);

//////////////////////////////////////////////////////////////////////////////

void Mul(VECTOR4 *pvOut, const VECTOR3 &vIn, const MATRIX44 &mat);

void Mul4x3(VECTOR3 *pvOut, const VECTOR3 &vIn, const MATRIX44 &mat);

void Mul(VECTOR3 *pvOut, const VECTOR3 &vIn, const MATRIX44 &mat);

void Mul(VECTOR4 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat);

void Mul(VECTOR3 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat);

void Mul(VECTOR2 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat);

void Mul(VECTOR2 *pvOut, const VECTOR2 &vIn, const MATRIX44 &mat);

void Transpose(MATRIX44 *pmatOut, const MATRIX44 &mIn);

void Transpose33(MATRIX44 *pmatOut, const MATRIX44 &mIn);

void Mul(MATRIX44 *pmatOut, const MATRIX44 &mat1, const MATRIX44 &mat2);

void Normalize(VECTOR3 *pvOut);

floatf Length(const VECTOR3 &vIn);

void Neg(VECTOR3 *pvOut, const VECTOR3 &vIn);

void Add(VECTOR3 *pvOut, const VECTOR3 &v1, const VECTOR3 &v2);

void Inverse(MATRIX44 *pmatOut, const MATRIX44 &mat);

void Inverse33(MATRIX44 *pmatOut, const MATRIX44 &mat);

void Ortho(MATRIX44 *pmatOut, floatf left, floatf right, floatf bottom,
           floatf top, floatf zNear, floatf zFar);

void Scale(MATRIX44 *pmatOut, floatf x, floatf y, floatf z);

void Translate(MATRIX44 *pmatOut, floatf x, floatf y, floatf z);

void Rotate(MATRIX44 *pmatOut, floatf angle, floatf x, floatf y, floatf z);

void Frustum(MATRIX44 *pmatOut, floatf left, floatf right, floatf bottom,
             floatf top, floatf zNear, floatf zFar);

template <class T> 
T TDot(const TVector3<T> &lhs, const TVector3<T> &rhs);

template <class T> 
T TDot(const TVector4<T> &lhs, const TVector4<T> &rhs);

template <class R> 
R TDiv(int lhs, int rhs);

bool IsPowerOf2(int value);

floatf DegToRad(floatf rhs);

floatf RadToDeg(floatf rhs);

template <class T> 
T MulAdd(T a0, T b0, T a1, T b1, T a2, T b2, T a3, T b3);

template <class T> 
T MulAdd(T a0, T b0, T a1, T b1, T a2, T b2);

template <class T> 
T MulAdd(T a0, T b0, T a1, T b1);

template <class T>
T MulSub(T a0, T b0, T a1, T b1);

///////////////////////////////////////////////////////////////////////////////

namespace detail {

template <typename Type, uint32_t Shift> struct TShiftInverter {
public:
  inline static Type call(int value) {
    if constexpr (Shift >= 32) {
      return value
                 ? static_cast<Type>((static_cast<int64_t>(1) << Shift) / value)
                 : 0;
    } else if (Shift == 32) {
      Math::Inverse32(value);
    } else {
      return static_cast<Type>((1 << Shift) / value);
    }
  }
};

} // namespace detail

//////////////////////////////////////////////////////////////////////////////

template <>
inline float MulAdd(float a0, float b0, float a1, float b1, float a2, float b2,
                    float a3, float b3) {
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

template <>
inline float MulAdd(float a0, float b0, float a1, float b1, float a2,
                    float b2) {
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

template <> 
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

template <> 
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

template <typename R> 
inline R TCast(float rhs) { 
  return static_cast<R>(rhs); 
}

template <typename R> 
inline R TCast(int rhs) { 
  return static_cast<R>(rhs); 
}

template <typename R>
inline R TCast(uint32_t rhs) {
  return static_cast<R>(rhs);
}

template <typename R> 
inline R TCast(short rhs) { 
  return static_cast<R>(rhs); 
}

template <typename R> 
inline R TCast(uint16_t rhs) {
  return static_cast<R>(rhs);
}

template <typename R> 
inline R TCast(char rhs) { 
  return static_cast<R>(rhs); 
}

template <typename R> 
inline R TCast(uint8_t rhs) {
  return static_cast<R>(rhs);
}

template <typename R> 
inline R TCast(bool rhs) { return static_cast<R>(rhs); }

template <typename R, uint32_t F, typename T> 
inline R TCast(TFixed<F, T> rhs) {
  return static_cast<R>(rhs);
}

template <> 
inline float TFromUNORM8<float>(int rhs) {
  return rhs * (1.0f / 255);
}

template <> 
inline fixed16 TFromUNORM8<fixed16>(int rhs) {
  return fixed16::make((rhs << fixed16::FRAC) / 255);
}

template <> 
inline fixed20 TFromUNORM8<fixed20>(int rhs) {
  return fixed20::make((rhs << fixed20::FRAC) / 255);
}

template <> 
inline int TFromUNORM8<int>(int rhs) { 
  return rhs; 
}

inline int Mul8(int lhs, int rhs) { 
  return (lhs * rhs + 0xff) >> 8; 
}

inline int Add8(int lhs, int rhs) { 
  return Math::TMin(lhs + rhs, 0xff); 
}

inline int Lerp8(int lhs, int rhs, int frac) {
  return lhs + ((frac * (rhs - lhs) + 0xff) >> 8);
}

template <> 
inline int TAbs<int>(int rhs) { 
  return ::abs(rhs); 
}

template <> 
inline float TAbs<float>(float rhs) { 
  return ::fabsf(rhs); 
}

template <uint32_t F> 
TFixed<F> TAbs(TFixed<F> rhs) {
  return TFixed<F>::make(::abs(rhs.data()));
}

template <uint32_t F> 
inline TFixed<F, int64_t> TAbs(TFixed<F, int64_t> rhs) {
  return TFixed<F, int64_t>::make(labs(rhs.data()));
}

template <class T> 
inline T TMin(T lhs, T rhs) {
  return (lhs <= rhs) ? lhs : rhs;
}

template <class T> 
inline T TMax(T lhs, T rhs) {
  return (lhs >= rhs) ? lhs : rhs;
}

template <class T> 
inline T TClamp(T lhs, T min, T max) {
  assert(max >= min);
  T tmp = (lhs >= min) ? lhs : min;
  return (tmp <= max) ? tmp : max;
}

template <class T> 
inline T TSat(T rhs) {
  return Math::TClamp(rhs, TConst<T>::Zero(), TConst<T>::One());
}

template <class T> 
inline bool TIsZero(T rhs) {
  return Math::TAbs(rhs) <= TConst<T>::Epsilon();
}

//////////////////////////////////////////////////////////////////////////////

template <typename R> 
inline R TCeili(float rhs) {
  return static_cast<R>(::ceil(rhs));
}

template <> 
inline float TCeilf(float rhs) {
  return static_cast<float>(::ceil(rhs));
}

template <typename R> 
inline R TFloori(float rhs) {
  return static_cast<R>(::floor(rhs));
}

template <> 
inline float TFloorf(float rhs) {
  return static_cast<float>(::floor(rhs));
}

template <typename R> 
inline R TRoundi(float rhs) {
  return static_cast<R>(rhs + 0.5f);
}

template <typename R> 
inline R TRoundf(float rhs) {
  return static_cast<R>(rhs + 0.5f);
}

template <> 
inline float TDiv(float lhs, float rhs) {
  assert(rhs != 0.0f);
  return static_cast<float>(lhs / rhs);
}

template <> 
inline float TDiv(int lhs, int rhs) {
  assert(rhs != 0);
  return static_cast<float>(lhs / rhs);
}

template <> 
inline float TPow(float lhs, float rhs) {
  return static_cast<float>(::pow(lhs, rhs));
}

template <> 
inline float TPow2(float rhs) {
  return static_cast<float>(::pow(2.0f, rhs));
}

template <> 
inline float TMod(float lhs, float rhs) {
  return static_cast<float>(::fmod(lhs, rhs));
}

template <> 
inline float TExp(float rhs) {
  return static_cast<float>(::exp(rhs));
}

template <> 
inline float TLog(float rhs) {
  return static_cast<float>(::log(rhs));
}

template <> 
inline float TSqrt(float rhs) {
  return static_cast<float>(::sqrt(rhs));
}

template <> 
inline float TInvSqrt(float rhs) {
  assert(rhs != 0.0f);
  return 1.0f / Math::TSqrt(rhs);
}

template <> 
inline float TSin(float rhs) {
  return static_cast<float>(::sin(static_cast<float>(rhs)));
}

template <> 
inline float TCos(float rhs) {
  return static_cast<float>(::cos(static_cast<float>(rhs)));
}

template <> 
inline int TToUNORM8(float rhs) {
  assert((rhs >= 0.0f) && (rhs <= 1.0f));
  return static_cast<int>(0xff * rhs);
}

template <> 
inline int TToUNORM16(float rhs) {
  assert((rhs >= 0.0f) && (rhs <= 1.0f));
  return static_cast<int>(0xffff * rhs);
}

inline float TFastMul(float lhs, float rhs) { return lhs * rhs; }

template <typename R> 
inline R TMul(float lhs, float rhs) {
  return static_cast<R>(lhs * rhs);
}

template <typename R> 
inline R TMul(float lhs, int rhs) {
  return static_cast<R>(lhs * rhs);
}

template <typename R, uint32_t F, typename T>
inline R TMul(float lhs, TFixed<F, T> rhs) {
  return static_cast<R>(lhs * rhs);
}

inline float MulDiv(float lhs, float rhs) { return lhs / rhs; }

inline float Lerpf(float lhs, float rhs, float scalar) {
  assert((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + (rhs - lhs) * scalar;
}

inline int Lerp(int lhs, int rhs, float scalar) {
  assert((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + static_cast<int>((rhs - lhs) * scalar);
}

template <> 
inline fixed4 TMul<fixed4>(float fOffset, float fRhw, int iScale) {
  return fixed4(fOffset * fRhw * iScale);
}

template <>
inline fixed20 TMul<fixed20>(float fOffset, float fRhw, float fScale) {
  return fixed20(fOffset * fRhw * fScale);
}

template <>
inline fixed24 TMul<fixed24>(float fOffset, float fRhw, float fScale) {
  return fixed24(fOffset * fRhw * fScale);
}

template <typename R> 
inline R TMulRnd(float a, float b) {
  return static_cast<R>(a * b);
}

template <typename R> 
inline R TMulRnd(float lhs, int rhs) {
  return static_cast<R>(lhs * rhs);
}

template <typename R> 
inline R TMulRnd(float a, float b, int c) {
  return static_cast<R>(a * b * c);
}

template <typename R> 
inline R TMulRnd(float a, float b, float c) {
  return static_cast<R>(a * b * c);
}

template <typename R> 
inline R TMulSub(float a, float b, float c, float d) {
  return static_cast<R>(a * b - c * d);
}

template <typename R> 
inline R TShiftLeft(float lhs, int rhs) {
  float scale = static_cast<float>(1 << rhs);
  return static_cast<R>(lhs * scale);
}

template <typename R> 
inline R TShiftRight(float lhs, int rhs) {
  float scale = 1.0f / static_cast<float>(1 << rhs);
  return static_cast<R>(lhs * scale);
}

//////////////////////////////////////////////////////////////////////////////

template <uint32_t F, typename T> 
inline TFixed<F, T> TExp(TFixed<F, T> rhs) {
  return static_cast<TFixed<F, T>>((float)exp(Math::TCast<float>(rhs)));
}

template <typename R, uint32_t F, typename T>
inline R TCeili(TFixed<F, T> rhs) {
  int value = rhs.data() + TFixed<F, T>::MASK;
  return static_cast<R>(value >> TFixed<F, T>::FRAC);
}

template <typename R, uint32_t F, typename T>
inline R TCeilf(TFixed<F, T> rhs) {
  int value = (rhs.data() + TFixed<F, T>::MASK) & TFixed<F, T>::IMASK;
  return R::make(value >> (TFixed<F, T>::FRAC - R::FRAC));
}

template <typename R, uint32_t F, typename T>
inline R TFloori(TFixed<F, T> rhs) {
  return static_cast<R>(rhs.data() >> TFixed<F, T>::FRAC);
}

template <typename R, uint32_t F, typename T>
inline R TFloorf(TFixed<F, T> rhs) {
  int value = rhs.data() & TFixed<F, T>::IMASK;
  return R::make(value >> (TFixed<F, T>::FRAC - R::FRAC));
}

template <typename R, uint32_t F, typename T>
inline R TRoundi(TFixed<F, T> rhs) {
  return static_cast<R>((rhs.data() + TFixed<F, T>::HALF) >>
                        TFixed<F, T>::FRAC);
}

template <typename R, uint32_t F, typename T>
inline R TRoundf(TFixed<F, T> rhs) {
  int value = (rhs.data() + TFixed<F, T>::HALF) & TFixed<F, T>::IMASK;
  return R::make(value >> (TFixed<F, T>::FRAC - R::FRAC));
}

template <typename R, uint32_t F, typename T>
inline R TDiv(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  assert(rhs.data() != 0);
  return R::make((static_cast<int64_t>(lhs.data()) << R::FRAC) / rhs.data());
}

template <> 
inline fixed28 TDiv(int lhs, int rhs) {
  assert(rhs != 0);
  return fixed28::make((static_cast<int64_t>(lhs) << fixed28::FRAC) / rhs);
}

template <> 
inline fixed16 TSqrt(fixed16 rhs) {
  assert(rhs.data() > 0);
  return fixed16::make(Math::iSqrt(rhs.data()) << 8);
}

template <uint32_t F, typename T> 
inline int TToUNORM8(TFixed<F, T> rhs) {
  assert((rhs >= TConst<TFixed<F, T>>::Zero()) && (rhs <= TConst<TFixed<F, T>>::One()));
  return (0xff * rhs.data()) >> TFixed<F, T>::FRAC;
}

template <uint32_t F, typename T> 
inline int TToUNORM16(TFixed<F, T> rhs) {
  assert((rhs >= TConst<TFixed<F, T>>::Zero()) && (rhs <= TConst<TFixed<F, T>>::One()));
  return (0xffff * rhs.data()) >> TFixed<F, T>::FRAC;
}

template <typename R, uint32_t F, typename T> class TInvSelect {
public:
  inline static R Invert(TFixed<F, T> rhs) {
    return R::make(detail::TShiftInverter<typename R::data_type,
                   TFixed<F, T>::FRAC + R::FRAC>::call(rhs.data()));
  }
};

template <uint32_t F, typename T> class TInvSelect<float, F, T> {
public:
  inline static float Invert(TFixed<F, T> rhs) {
    return 1.0f / Math::TCast<float>(rhs);
  }
};

template <typename R, uint32_t F, typename T> 
inline R TInv(TFixed<F, T> rhs) {
  return TInvSelect<R, F, T>::Invert(rhs);
}

template <typename R> 
inline R TInv(float rhs) {
  assert(rhs != 0.0f);
  return static_cast<R>(1.0f / rhs);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R TFastMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  assert((static_cast<int64_t>(lhs.data()) * rhs.data()) == (lhs.data() * rhs.data()));
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  return R::make((lhs.data() * rhs.data()) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R TFastDiv(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  assert((static_cast<int64_t>(lhs.data()) << FRAC) == (lhs.data() << FRAC));
  return R::make((lhs.data() << FRAC) / rhs.data());
}

template <typename R, uint32_t F, typename T>
inline R TMulRnd(TFixed<F, T> lhs, int rhs) {
  int FRAC = TFixed<F, T>::FRAC - R::FRAC;
  int HALF = 1 << (FRAC - 1);
  auto value = static_cast<int64_t>(lhs.data()) * rhs;
  return R::make((value + HALF) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R TMulRnd(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  int HALF = 1 << (FRAC - 1);
  auto value = static_cast<int64_t>(lhs.data()) * rhs.data();
  return R::make((value + HALF) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R TMulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, int c) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto HALF = static_cast<int64_t>(1) << (FRAC - 1);
  auto value = static_cast<int64_t>(a.data()) * b.data() * c;
  return R::make((value + HALF) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, uint32_t F3, typename T1,
          typename T2, typename T3>
inline R TMulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F3, T3> c) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC + TFixed<F3, T3>::FRAC - R::FRAC;
  auto HALF = static_cast<int64_t>(1) << (FRAC - 1);
  int64_t value = static_cast<int64_t>(a.data()) * b.data() * c.data();
  return R::make((value + HALF) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R TMulAdd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c,
                 TFixed<F2, T2> d) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto ab = static_cast<int64_t>(a.data()) * b.data();
  auto cd = static_cast<int64_t>(c.data()) * d.data();
  return R::make((ab + cd) >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R TMulSub(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c,
                 TFixed<F2, T2> d) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto ab = static_cast<int64_t>(a.data()) * b.data();
  auto cd = static_cast<int64_t>(c.data()) * d.data();
  return R::make((ab - cd) >> FRAC);
}

template <typename R, uint32_t F, typename T>
inline R TMul(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  int FRAC = TFixed<F, T>::FRAC + TFixed<F, T>::FRAC - R::FRAC;
  auto value = static_cast<int64_t>(lhs.data()) * rhs.data();
  return R::make(value >> FRAC);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R TMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto value = static_cast<int64_t>(lhs.data()) * rhs.data();
  return R::make(value >> FRAC);
}

template <typename R, uint32_t F, typename T>
inline R TMul(TFixed<F, T> lhs, int rhs) {
  int FRAC = TFixed<F, T>::FRAC - R::FRAC;
  auto value = static_cast<int64_t>(lhs.data()) * rhs;
  return R::make(value >> FRAC);
}

template <typename R, uint32_t F, typename T>
R TMul(TFixed<F, T> lhs, float rhs) {
  return static_cast<R>(rhs * lhs);
}

template <typename R, uint32_t F1, uint32_t F2, typename T1, typename T2>
inline R TMulShift(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs, R a, int shift) {
  int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  auto value = static_cast<int64_t>(lhs.data()) * rhs.data();
  return R::make((value >> FRAC) - (a.data() >> shift));
}

template <uint32_t F1, uint32_t F2, typename T1, typename T2>
inline TFixed<F1, T1> Lerpf(TFixed<F1, T1> lhs, TFixed<F1, T1> rhs,
                            TFixed<F2, T2> scalar) {
  assert((scalar >= TConst<TFixed<F2, T2>>::Zero()) &&
         (scalar <= TConst<TFixed<F2, T2>>::One()));
  return lhs + (rhs - lhs) * scalar;
}

template <uint32_t F, typename T>
inline TFixed<F, T> Lerpf(TFixed<F, T> lhs, TFixed<F, T> rhs, float scalar) {
  assert((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + (rhs - lhs) * scalar;
}

template <uint32_t F, typename T>
inline int Lerp(int lhs, int rhs, TFixed<F, T> scalar) {
  assert((scalar >= TConst<TFixed<F, T>>::Zero()) &&
         (scalar <= TConst<TFixed<F, T>>::One()));
  return lhs + static_cast<int>(
    (static_cast<int64_t>(rhs - lhs) * scalar.data()) >> TFixed<F, T>::FRAC);
}

template <typename R, uint32_t F, typename T>
inline R TShiftLeft(TFixed<F, T> lhs, int rhs) {
  return R(lhs) << rhs;
}

template <typename R, uint32_t F, typename T>
inline R TShiftRight(TFixed<F, T> lhs, int rhs) {
  return R(lhs) >> rhs;
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline fixed16 TDot<fixed16>(const TVector3<fixed16> &lhs,
                             const TVector3<fixed16> &rhs) {
  auto xx = static_cast<int64_t>(lhs.x.data()) * rhs.x.data();
  auto yy = static_cast<int64_t>(lhs.y.data()) * rhs.y.data();
  auto zz = static_cast<int64_t>(lhs.z.data()) * rhs.z.data();
  return fixed16::make((xx + yy + zz) >> fixed16::FRAC);
}

template <>
inline fixed16 TDot<fixed16>(const TVector4<fixed16> &lhs,
                             const TVector4<fixed16> &rhs) {
  auto xx = static_cast<int64_t>(lhs.x.data()) * rhs.x.data();
  auto yy = static_cast<int64_t>(lhs.y.data()) * rhs.y.data();
  auto zz = static_cast<int64_t>(lhs.z.data()) * rhs.z.data();
  auto ww = static_cast<int64_t>(lhs.w.data()) * rhs.w.data();
  return fixed16::make((xx + yy + zz + ww) >> fixed16::FRAC);
}

template <>
inline float TDot<float>(const TVector3<float> &lhs,
                         const TVector3<float> &rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <>
inline float TDot<float>(const TVector4<float> &lhs,
                         const TVector4<float> &rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

inline void Mul(VECTOR4 *pvOut, const VECTOR3 &vIn, const MATRIX44 &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32) + mat._42;
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33) + mat._43;
  pvOut->w = MulAdd(x, mat._14, y, mat._24, z, mat._34) + mat._44;
}

inline void Mul4x3(VECTOR3 *pvOut, const VECTOR3 &vIn, const MATRIX44 &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32) + mat._42;
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33) + mat._43;
}

inline void Mul(VECTOR3 *pvOut, const VECTOR3 &vIn, const MATRIX44 &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32);
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33);
}

inline void Mul(VECTOR4 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat) {
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

inline void Mul(VECTOR3 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;
  auto w = vIn.w;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31, w, mat._41);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32, w, mat._42);
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33, w, mat._43);
}

inline void Mul(VECTOR2 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;
  auto z = vIn.z;
  auto w = vIn.w;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31, w, mat._41);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32, w, mat._42);
}

inline void Mul(VECTOR2 *pvOut, const VECTOR2 &vIn, const MATRIX44 &mat) {
  assert(pvOut);

  auto x = vIn.x;
  auto y = vIn.y;

  pvOut->x = MulAdd(x, mat._11, y, mat._21) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22) + mat._42;
}

inline floatf Length(const VECTOR3 &vIn) {
  auto fDot = Math::TDot<floatf>(vIn, vIn);
  if (!Math::TIsZero(fDot - fONE)) {
    return Math::TSqrt(fDot);
  }
  return fDot;
}

inline void Normalize(VECTOR3 *pvOut) {
  assert(pvOut);

  auto fDot = Math::TDot<floatf>(*pvOut, *pvOut);
  if (!Math::TIsZero(fDot - fONE) && !Math::TIsZero(fDot)) {
    auto fInvLen = Math::TInvSqrt(fDot);
    pvOut->x = pvOut->x * fInvLen;
    pvOut->y = pvOut->y * fInvLen;
    pvOut->z = pvOut->z * fInvLen;
  }
}

inline void Neg(VECTOR3 *pvOut, const VECTOR3 &vIn) {
  pvOut->x = -vIn.x;
  pvOut->y = -vIn.y;
  pvOut->z = -vIn.z;
}

inline void Add(VECTOR3 *pvOut, const VECTOR3 &v1, const VECTOR3 &v2) {
  pvOut->x = v1.x + v2.x;
  pvOut->y = v1.y + v2.y;
  pvOut->z = v1.z + v2.z;
}

inline void Transpose(MATRIX44 *pmatOut, const MATRIX44 &matIn) {
  assert(pmatOut);

  pmatOut->_11 = matIn._11;
  pmatOut->_12 = matIn._21;
  pmatOut->_13 = matIn._31;
  pmatOut->_14 = matIn._41;

  pmatOut->_21 = matIn._12;
  pmatOut->_22 = matIn._22;
  pmatOut->_23 = matIn._32;
  pmatOut->_24 = matIn._42;

  pmatOut->_31 = matIn._13;
  pmatOut->_32 = matIn._23;
  pmatOut->_33 = matIn._33;
  pmatOut->_34 = matIn._43;

  pmatOut->_41 = matIn._14;
  pmatOut->_42 = matIn._24;
  pmatOut->_43 = matIn._34;
  pmatOut->_44 = matIn._44;
}

inline void Transpose33(MATRIX44 *pmatOut, const MATRIX44 &matIn) {
  assert(pmatOut);

  pmatOut->_11 = matIn._11;
  pmatOut->_12 = matIn._21;
  pmatOut->_13 = matIn._31;

  pmatOut->_21 = matIn._12;
  pmatOut->_22 = matIn._22;
  pmatOut->_23 = matIn._32;

  pmatOut->_31 = matIn._13;
  pmatOut->_32 = matIn._23;
  pmatOut->_33 = matIn._33;
}

inline void Scale(MATRIX44 *pmatOut, floatf x, floatf y, floatf z) {
  assert(pmatOut);

  pmatOut->toIdentity();

  pmatOut->_11 = x;
  pmatOut->_22 = y;
  pmatOut->_33 = z;
}

inline void Translate(MATRIX44 *pmatOut, floatf x, floatf y, floatf z) {
  assert(pmatOut);

  pmatOut->toIdentity();

  pmatOut->_41 = x;
  pmatOut->_42 = y;
  pmatOut->_43 = z;
}

inline bool IsPowerOf2(int value) { return 0 == (value & (value - 1)); }

inline floatf DegToRad(floatf rhs) { return rhs * (fPI / f180); }

inline floatf RadToDeg(floatf rhs) { return rhs * (f180 / fPI); }

inline uint32_t iLog2(uint32_t rhs) { return 31 - Clz(rhs); }

inline void Mul(MATRIX44 *pmatOut, const MATRIX44 &mat1, const MATRIX44 &mat2) {
  assert(pmatOut);

  pmatOut->_11 = MulAdd(mat1._11, mat2._11, mat1._21, mat2._12, mat1._31,
                        mat2._13, mat1._41, mat2._14);
  pmatOut->_12 = MulAdd(mat1._12, mat2._11, mat1._22, mat2._12, mat1._32,
                        mat2._13, mat1._42, mat2._14);
  pmatOut->_13 = MulAdd(mat1._13, mat2._11, mat1._23, mat2._12, mat1._33,
                        mat2._13, mat1._43, mat2._14);
  pmatOut->_14 = MulAdd(mat1._14, mat2._11, mat1._24, mat2._12, mat1._34,
                        mat2._13, mat1._44, mat2._14);

  pmatOut->_21 = MulAdd(mat1._11, mat2._21, mat1._21, mat2._22, mat1._31,
                        mat2._23, mat1._41, mat2._24);
  pmatOut->_22 = MulAdd(mat1._12, mat2._21, mat1._22, mat2._22, mat1._32,
                        mat2._23, mat1._42, mat2._24);
  pmatOut->_23 = MulAdd(mat1._13, mat2._21, mat1._23, mat2._22, mat1._33,
                        mat2._23, mat1._43, mat2._24);
  pmatOut->_24 = MulAdd(mat1._14, mat2._21, mat1._24, mat2._22, mat1._34,
                        mat2._23, mat1._44, mat2._24);

  pmatOut->_31 = MulAdd(mat1._11, mat2._31, mat1._21, mat2._32, mat1._31,
                        mat2._33, mat1._41, mat2._34);
  pmatOut->_32 = MulAdd(mat1._12, mat2._31, mat1._22, mat2._32, mat1._32,
                        mat2._33, mat1._42, mat2._34);
  pmatOut->_33 = MulAdd(mat1._13, mat2._31, mat1._23, mat2._32, mat1._33,
                        mat2._33, mat1._43, mat2._34);
  pmatOut->_34 = MulAdd(mat1._14, mat2._31, mat1._24, mat2._32, mat1._34,
                        mat2._33, mat1._44, mat2._34);

  pmatOut->_41 = MulAdd(mat1._11, mat2._41, mat1._21, mat2._42, mat1._31,
                        mat2._43, mat1._41, mat2._44);
  pmatOut->_42 = MulAdd(mat1._12, mat2._41, mat1._22, mat2._42, mat1._32,
                        mat2._43, mat1._42, mat2._44);
  pmatOut->_43 = MulAdd(mat1._13, mat2._41, mat1._23, mat2._42, mat1._33,
                        mat2._43, mat1._43, mat2._44);
  pmatOut->_44 = MulAdd(mat1._14, mat2._41, mat1._24, mat2._42, mat1._34,
                        mat2._43, mat1._44, mat2._44);
}

inline void Inverse33(MATRIX44 *pmatOut, const MATRIX44 &matIn) {
  assert(pmatOut);

  pmatOut->_11 = MulSub(matIn._22, matIn._33, matIn._23, matIn._32);
  pmatOut->_12 = -MulSub(matIn._21, matIn._33, matIn._23, matIn._31);
  pmatOut->_13 = MulSub(matIn._21, matIn._32, matIn._22, matIn._31);

  pmatOut->_21 = -MulSub(matIn._12, matIn._33, matIn._13, matIn._32);
  pmatOut->_22 = MulSub(matIn._11, matIn._33, matIn._13, matIn._31);
  pmatOut->_23 = -MulSub(matIn._11, matIn._32, matIn._12, matIn._31);

  pmatOut->_31 = MulSub(matIn._12, matIn._23, matIn._13, matIn._22);
  pmatOut->_32 = -MulSub(matIn._11, matIn._23, matIn._13, matIn._21);
  pmatOut->_33 = MulSub(matIn._11, matIn._22, matIn._12, matIn._21);

  auto fDet = MulAdd(matIn._11, pmatOut->_11, matIn._21, pmatOut->_21,
                     matIn._31, pmatOut->_31);

  if (!Math::TIsZero(fDet - fONE)) {
    auto fDetInv = Math::TInv<floatf>(fDet);

    pmatOut->_11 *= fDetInv;
    pmatOut->_12 *= fDetInv;
    pmatOut->_13 *= fDetInv;

    pmatOut->_21 *= fDetInv;
    pmatOut->_22 *= fDetInv;
    pmatOut->_23 *= fDetInv;

    pmatOut->_31 *= fDetInv;
    pmatOut->_32 *= fDetInv;
    pmatOut->_33 *= fDetInv;
  }
}

inline void Inverse(MATRIX44 *pmatOut, const MATRIX44 &matIn) {
  assert(pmatOut);

  auto fB0 = MulSub(matIn._13, matIn._24, matIn._23, matIn._14);
  auto fB1 = MulSub(matIn._13, matIn._34, matIn._33, matIn._14);
  auto fB2 = MulSub(matIn._13, matIn._44, matIn._43, matIn._14);
  auto fB3 = MulSub(matIn._23, matIn._34, matIn._33, matIn._24);
  auto fB4 = MulSub(matIn._23, matIn._44, matIn._43, matIn._24);
  auto fB5 = MulSub(matIn._33, matIn._44, matIn._43, matIn._34);

  pmatOut->_11 = (matIn._22 * fB5 - matIn._32 * fB4 + matIn._42 * fB3);
  pmatOut->_12 = (-matIn._12 * fB5 + matIn._32 * fB2 - matIn._42 * fB1);
  pmatOut->_13 = (matIn._12 * fB4 - matIn._22 * fB2 + matIn._42 * fB0);
  pmatOut->_14 = (-matIn._12 * fB3 + matIn._22 * fB1 - matIn._32 * fB0);

  pmatOut->_21 = (-matIn._21 * fB5 + matIn._31 * fB4 - matIn._41 * fB3);
  pmatOut->_22 = (matIn._11 * fB5 - matIn._31 * fB2 + matIn._41 * fB1);
  pmatOut->_23 = (-matIn._11 * fB4 + matIn._21 * fB2 - matIn._41 * fB0);
  pmatOut->_24 = (matIn._11 * fB3 - matIn._21 * fB1 + matIn._31 * fB0);

  auto fA0 = MulSub(matIn._11, matIn._22, matIn._21, matIn._12);
  auto fA1 = MulSub(matIn._11, matIn._32, matIn._31, matIn._12);
  auto fA2 = MulSub(matIn._11, matIn._42, matIn._41, matIn._12);
  auto fA3 = MulSub(matIn._21, matIn._32, matIn._31, matIn._22);
  auto fA4 = MulSub(matIn._21, matIn._42, matIn._41, matIn._22);
  auto fA5 = MulSub(matIn._31, matIn._42, matIn._41, matIn._32);

  pmatOut->_31 = (matIn._24 * fA5 - matIn._34 * fA4 + matIn._44 * fA3);
  pmatOut->_32 = (-matIn._14 * fA5 + matIn._34 * fA2 - matIn._44 * fA1);
  pmatOut->_33 = (matIn._14 * fA4 - matIn._24 * fA2 + matIn._44 * fA0);
  pmatOut->_34 = (-matIn._14 * fA3 + matIn._24 * fA1 - matIn._34 * fA0);

  pmatOut->_41 = (-matIn._23 * fA5 + matIn._33 * fA4 - matIn._43 * fA3);
  pmatOut->_42 = (matIn._13 * fA5 - matIn._33 * fA2 + matIn._43 * fA1);
  pmatOut->_43 = (-matIn._13 * fA4 + matIn._23 * fA2 - matIn._43 * fA0);
  pmatOut->_44 = (matIn._13 * fA3 - matIn._23 * fA1 + matIn._33 * fA0);

  auto fDet = MulAdd(fA0, fB5, fA2, fB3, fA3, fB2, fA5, fB0) -
              MulAdd(fA1, fB4, fA4, fB1);

  if (!Math::TIsZero(fDet - fONE)) {
    auto fDetInv = Math::TInv<floatf>(fDet);

    pmatOut->_11 *= fDetInv;
    pmatOut->_12 *= fDetInv;
    pmatOut->_13 *= fDetInv;
    pmatOut->_14 *= fDetInv;

    pmatOut->_21 *= fDetInv;
    pmatOut->_22 *= fDetInv;
    pmatOut->_23 *= fDetInv;
    pmatOut->_24 *= fDetInv;

    pmatOut->_31 *= fDetInv;
    pmatOut->_32 *= fDetInv;
    pmatOut->_33 *= fDetInv;
    pmatOut->_34 *= fDetInv;

    pmatOut->_41 *= fDetInv;
    pmatOut->_42 *= fDetInv;
    pmatOut->_43 *= fDetInv;
    pmatOut->_44 *= fDetInv;
  }
}

inline void Ortho(MATRIX44 *pmatOut, floatf left, floatf right, floatf bottom,
                  floatf top, floatf zNear, floatf zFar) {
  assert(pmatOut);

  auto fWidth = right - left;
  auto fHeight = top - bottom;
  auto fDepth = zFar - zNear;

  auto fInvWidth = (fWidth != fZERO) ? Math::TInv<floatf>(fWidth) : fZERO;
  auto fInvHeight = (fHeight != fZERO) ? Math::TInv<floatf>(fHeight) : fZERO;
  auto fInvDepth = (fDepth != fZERO) ? Math::TInv<floatf>(fDepth) : fZERO;

  pmatOut->_11 = fTWO * fInvWidth;
  pmatOut->_12 = fZERO;
  pmatOut->_13 = fZERO;
  pmatOut->_14 = fZERO;

  pmatOut->_21 = fZERO;
  pmatOut->_22 = fTWO * fInvHeight;
  pmatOut->_23 = fZERO;
  pmatOut->_24 = fZERO;

  pmatOut->_31 = fZERO;
  pmatOut->_32 = fZERO;
  pmatOut->_33 = -fTWO * fInvDepth;
  pmatOut->_34 = fZERO;

  pmatOut->_41 = -(right + left) * fInvWidth;
  pmatOut->_42 = -(top + bottom) * fInvHeight;
  pmatOut->_43 = -(zFar + zNear) * fInvDepth;
  pmatOut->_44 = fONE;
}

inline void Frustum(MATRIX44 *pmatOut, floatf left, floatf right, floatf bottom,
                    floatf top, floatf zNear, floatf zFar) {
  assert(pmatOut);

  auto fWidth = right - left;
  auto fHeight = top - bottom;
  auto fDepth = zFar - zNear;

  auto fInvWidth = (fWidth != fZERO) ? Math::TInv<floatf>(fWidth) : fZERO;
  auto fInvHeight = (fHeight != fZERO) ? Math::TInv<floatf>(fHeight) : fZERO;
  auto fInvDepth = (fDepth != fZERO) ? Math::TInv<floatf>(fDepth) : fZERO;

  auto fTwoNear = fTWO * zNear;

  pmatOut->_11 = fTwoNear * fInvWidth;
  pmatOut->_12 = fZERO;
  pmatOut->_13 = fZERO;
  pmatOut->_14 = fZERO;

  pmatOut->_21 = fZERO;
  pmatOut->_22 = fTwoNear * fInvHeight;
  pmatOut->_23 = fZERO;
  pmatOut->_24 = fZERO;

  pmatOut->_31 = (right + left) * fInvWidth;
  pmatOut->_32 = (top + bottom) * fInvHeight;
  pmatOut->_33 = -(zFar + zNear) * fInvDepth;
  pmatOut->_34 = -fONE;

  pmatOut->_41 = fZERO;
  pmatOut->_42 = fZERO;
  pmatOut->_43 = -zFar * (fTwoNear * fInvDepth);
  pmatOut->_44 = fZERO;
}

inline void Rotate(MATRIX44 *pmatOut, floatf angle, floatf x, floatf y,
                   floatf z) {
  assert(pmatOut);

  VECTOR3 vAxis(x, y, z);
  Math::Normalize(&vAxis);

  auto fSin = Math::TSin(angle);
  auto fCos = Math::TCos(angle);
  auto fICos = fONE - fCos;

  auto _fA = fICos * vAxis.x * vAxis.y;
  auto _fB = fICos * vAxis.x * vAxis.z;
  auto _fC = fICos * vAxis.y * vAxis.z;

  pmatOut->_11 = fICos * vAxis.x * vAxis.x + fCos;
  pmatOut->_12 = _fA + vAxis.z * fSin;
  pmatOut->_13 = _fB - vAxis.y * fSin;
  pmatOut->_14 = fZERO;

  pmatOut->_21 = _fA - vAxis.z * fSin;
  pmatOut->_22 = fICos * vAxis.y * vAxis.y + fCos;
  pmatOut->_23 = _fC + vAxis.x * fSin;
  pmatOut->_24 = fZERO;

  pmatOut->_31 = _fB + vAxis.y * fSin;
  pmatOut->_32 = _fC - vAxis.x * fSin;
  pmatOut->_33 = fICos * vAxis.z * vAxis.z + fCos;
  pmatOut->_34 = fZERO;

  pmatOut->_41 = fZERO;
  pmatOut->_42 = fZERO;
  pmatOut->_43 = fZERO;
  pmatOut->_44 = fONE;
}

} // namespace Math
