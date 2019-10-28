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


template <unsigned int F, typename T> struct TConst<TFixed<F, T>> {
  inline static TFixed<F, T> Zero() { return TFixed<F, T>::Make(0); }

  inline static TFixed<F, T> Half() {
    return TFixed<F, T>::Make(TFixed<F, T>::HALF);
  }

  inline static TFixed<F, T> One() {
    return TFixed<F, T>::Make(TFixed<F, T>::ONE);
  }

  inline static TFixed<F, T> Two() {
    return TFixed<F, T>::Make(TFixed<F, T>::TWO);
  }

  inline static TFixed<F, T> Min() {
    return TFixed<F, T>::Make(-(1 << (8 * sizeof(T) - 1)));
  }

  inline static TFixed<F, T> Max() {
    return TFixed<F, T>::Make((1 << (8 * sizeof(T) - 1)) - 1);
  }

  inline static TFixed<F, T> Epsilon() { return TFixed<F, T>::Make(1); }

  inline static TFixed<F, T> PI() {
    return TFixed<F, T>::Make(3.1415926535f * TFixed<F, T>::ONE);
  }

  inline static TFixed<F, T> F128() {
    return TFixed<F, T>::Make(128 * TFixed<F, T>::ONE);
  }

  inline static TFixed<F, T> F90() {
    return TFixed<F, T>::Make(90 * TFixed<F, T>::ONE);
  }

  inline static TFixed<F, T> F180() {
    return TFixed<F, T>::Make(180 * TFixed<F, T>::ONE);
  }

  inline static TFixed<F, T> F02() {
    return TFixed<F, T>::Make(0.2f * TFixed<F, T>::ONE);
  }

  inline static TFixed<F, T> F08() {
    return TFixed<F, T>::Make(0.8f * TFixed<F, T>::ONE);
  }

  inline static TFixed<F, T> E() {
    return TFixed<F, T>::Make(2.71828183f * TFixed<F, T>::ONE);
  }

  inline static TFixed<F, T> LogE() {
    return TFixed<F, T>::Make(1.442695041f * TFixed<F, T>::ONE);
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

  void ToIdentity() {
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

#ifdef GL_FIXEDPOINT

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
unsigned int iLog2(unsigned int rhs);

unsigned int iSqrt(unsigned int rhs);

int Mul8(int lhs, int rhs);

int Add8(int lhs, int rhs);

int Lerp8(int lhs, int rhs, int frac);

int Inverse32(int value);

template <class R> R TCast(float rhs);

template <class R> R TCast(int rhs);

template <class R> R TCast(unsigned int rhs);

template <class R> R TCast(short rhs);

template <class R> R TCast(unsigned short rhs);

template <class R> R TCast(char rhs);

template <class R> R TCast(unsigned char rhs);

template <class R> R TCast(bool rhs);

template <class R, unsigned int F, typename T> R TCast(TFixed<F, T> rhs);

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

template <class R, unsigned int F, typename T>
R TMul(float lhs, TFixed<F, T> rhs);

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

template <class R, unsigned int F, typename T> R TInv(TFixed<F, T> rhs);

template <class R> R TInv(float rhs);

template <class R, unsigned int F1, unsigned int F2, typename T1, typename T2>
R TFastMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs);

template <class R, unsigned int F1, unsigned int F2, typename T1, typename T2>
R TFastDiv(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs);

template <class R, unsigned int F1, unsigned int F2, typename T1, typename T2>
R TMulRnd(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs);

template <class R, unsigned int F1, unsigned int F2, typename T1, typename T2>
R TMulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, int c);

template <class R, unsigned int F1, unsigned int F2, unsigned int F3,
          typename T1, typename T2, typename T3>
R TMulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F3, T3> c);

template <class R, unsigned int F, typename T>
R TMulRnd(TFixed<F, T> lhs, int rhs);

template <class R, unsigned int F1, unsigned int F2, typename T1, typename T2>
R TMulAdd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c,
          TFixed<F2, T2> d);

template <class R, unsigned int F1, unsigned int F2, typename T1, typename T2>
R TMulSub(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c,
          TFixed<F2, T2> d);

template <class R, unsigned int F, typename T>
R TMul(TFixed<F, T> lhs, int rhs);

template <class R, unsigned int F, typename T>
R TMul(TFixed<F, T> lhs, float rhs);

template <class R, unsigned int F1, unsigned int F2, typename T1, typename T2>
R TMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs);

template <class R, unsigned int F, typename T>
R TMul(TFixed<F, T> lhs, TFixed<F, T> rhs);

template <unsigned int F1, unsigned int F2, typename T1, typename T2>
TFixed<F1, T1> TLerpf(TFixed<F1, T1> lhs, TFixed<F1, T1> rhs,
                      TFixed<F2, T2> scalar);

template <unsigned int F, typename T>
TFixed<F, T> TLerpf(TFixed<F, T> lhs, TFixed<F, T> rhs, float scalar);

template <class R, unsigned int F, typename T>
R TShiftLeft(TFixed<F, T> lhs, int rhs);

template <class R, unsigned int F, typename T>
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

template <class T> T TDot(const TVector3<T> &lhs, const TVector3<T> &rhs);

template <class T> T TDot(const TVector4<T> &lhs, const TVector4<T> &rhs);

template <class R> R TDiv(int lhs, int rhs);

bool IsPowerOf2(int value);

floatf DegToRad(floatf rhs);

floatf RadToDeg(floatf rhs);

template <class T> T MulAdd(T a0, T b0, T a1, T b1, T a2, T b2, T a3, T b3);

template <class T> T MulAdd(T a0, T b0, T a1, T b1, T a2, T b2);

template <class T> T MulAdd(T a0, T b0, T a1, T b1);

template <class T> T MulSub(T a0, T b0, T a1, T b1);

} // namespace Math
