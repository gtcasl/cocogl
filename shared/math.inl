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

namespace Math {

namespace detail {

template <typename Type, unsigned Shift> struct TShiftInverter {
public:
  inline static Type call(int value) {
    if
      constexpr(Shift >= 32) {
        return value ? static_cast<Type>(((int64_t)1 << Shift) / value) : 0;
      }
    else if (Shift == 32) {
      Math::Inverse32(value);
    } else {
      return static_cast<Type>((1 << Shift) / value);
    }
  }
};
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline float MulAdd(float a0, float b0, float a1, float b1, float a2, float b2,
                    float a3, float b3) {
  return a0 * b0 + a1 * b1 + a2 * b2 + a3 * b3;
}

template <unsigned int F, typename T>
inline TFixed<F, T> MulAdd(TFixed<F, T> a0, TFixed<F, T> b0, TFixed<F, T> a1,
                           TFixed<F, T> b1, TFixed<F, T> a2, TFixed<F, T> b2,
                           TFixed<F, T> a3, TFixed<F, T> b3) {
  return TFixed<F, T>::Make((static_cast<int64_t>(a0.GetRaw()) * b0.GetRaw() +
                             static_cast<int64_t>(a1.GetRaw()) * b1.GetRaw() +
                             static_cast<int64_t>(a2.GetRaw()) * b2.GetRaw() +
                             static_cast<int64_t>(a3.GetRaw()) * b3.GetRaw()) >>
                            TFixed<F, T>::FRAC);
}

template <>
inline float MulAdd(float a0, float b0, float a1, float b1, float a2,
                    float b2) {
  return a0 * b0 + a1 * b1 + a2 * b2;
}

template <unsigned int F, typename T>
inline TFixed<F, T> MulAdd(TFixed<F, T> a0, TFixed<F, T> b0, TFixed<F, T> a1,
                           TFixed<F, T> b1, TFixed<F, T> a2, TFixed<F, T> b2) {
  return TFixed<F, T>::Make((static_cast<int64_t>(a0.GetRaw()) * b0.GetRaw() +
                             static_cast<int64_t>(a1.GetRaw()) * b1.GetRaw() +
                             static_cast<int64_t>(a2.GetRaw()) * b2.GetRaw()) >>
                            TFixed<F, T>::FRAC);
}

template <> inline float MulAdd(float a0, float b0, float a1, float b1) {
  return a0 * b0 + a1 * b1;
}

template <unsigned int F, typename T>
inline TFixed<F, T> MulAdd(TFixed<F, T> a0, TFixed<F, T> b0, TFixed<F, T> a1,
                           TFixed<F, T> b1) {
  return TFixed<F, T>::Make((static_cast<int64_t>(a0.GetRaw()) * b0.GetRaw() +
                             static_cast<int64_t>(a1.GetRaw()) * b1.GetRaw()) >>
                            TFixed<F, T>::FRAC);
}

template <> inline float MulSub(float a0, float b0, float a1, float b1) {
  return a0 * b0 - a1 * b1;
}

template <unsigned int F, typename T>
inline TFixed<F, T> MulSub(TFixed<F, T> a0, TFixed<F, T> b0, TFixed<F, T> a1,
                           TFixed<F, T> b1) {
  return TFixed<F, T>::Make((static_cast<int64_t>(a0.GetRaw()) * b0.GetRaw() -
                             static_cast<int64_t>(a1.GetRaw()) * b1.GetRaw()) >>
                            TFixed<F, T>::FRAC);
}

template <typename R> inline R TCast(float rhs) { return static_cast<R>(rhs); }

template <typename R> inline R TCast(int rhs) { return static_cast<R>(rhs); }

template <typename R> inline R TCast(unsigned int rhs) {
  return static_cast<R>(rhs);
}

template <typename R> inline R TCast(short rhs) { return static_cast<R>(rhs); }

template <typename R> inline R TCast(unsigned short rhs) {
  return static_cast<R>(rhs);
}

template <typename R> inline R TCast(char rhs) { return static_cast<R>(rhs); }

template <typename R> inline R TCast(unsigned char rhs) {
  return static_cast<R>(rhs);
}

template <typename R> inline R TCast(bool rhs) { return static_cast<R>(rhs); }

template <typename R, unsigned int F, typename T>
inline R TCast(TFixed<F, T> rhs) {
  return rhs.template To<R>();
}

template <> inline float TFromUNORM8<float>(int rhs) {
  return rhs * (1.0f / 255);
}

template <> inline fixed16 TFromUNORM8<fixed16>(int rhs) {
  return fixed16::Make((rhs << fixed16::FRAC) / 255);
}

template <> inline fixed20 TFromUNORM8<fixed20>(int rhs) {
  return fixed20::Make((rhs << fixed20::FRAC) / 255);
}

template <> inline int TFromUNORM8<int>(int rhs) { return rhs; }

inline int Mul8(int lhs, int rhs) { return (lhs * rhs + 0xff) >> 8; }

inline int Add8(int lhs, int rhs) { return Math::TMin(lhs + rhs, 0xff); }

inline int Lerp8(int lhs, int rhs, int frac) {
  return lhs + ((frac * (rhs - lhs) + 0xff) >> 8);
}

template <> inline int TAbs<int>(int rhs) { return ::abs(rhs); }

template <> inline float TAbs<float>(float rhs) { return ::fabsf(rhs); }

template <unsigned int F> TFixed<F> TAbs(TFixed<F> rhs) {
  return TFixed<F>::Make(::abs(rhs.GetRaw()));
}

template <unsigned int F>
inline TFixed<F, int64_t> TAbs(TFixed<F, int64_t> rhs) {
  return TFixed<F, int64_t>::Make(labs(rhs.GetRaw()));
}

template <class T> inline T TMin(T lhs, T rhs) {
  return (lhs <= rhs) ? lhs : rhs;
}

template <class T> inline T TMax(T lhs, T rhs) {
  return (lhs >= rhs) ? lhs : rhs;
}

template <class T> inline T TClamp(T lhs, T min, T max) {
  ASSERT(max >= min);
  const T tmp = (lhs >= min) ? lhs : min;
  return (tmp <= max) ? tmp : max;
}

template <class T> inline T TSat(T rhs) {
  return Math::TClamp(rhs, TConst<T>::Zero(), TConst<T>::One());
}

template <class T> inline bool TIsZero(T rhs) {
  return Math::TAbs(rhs) <= TConst<T>::Epsilon();
}

//////////////////////////////////////////////////////////////////////////////

template <typename R> inline R TCeili(float rhs) {
  return static_cast<R>(::ceil(rhs));
}

template <> inline float TCeilf(float rhs) {
  return static_cast<float>(::ceil(rhs));
}

template <typename R> inline R TFloori(float rhs) {
  return static_cast<R>(::floor(rhs));
}

template <> inline float TFloorf(float rhs) {
  return static_cast<float>(::floor(rhs));
}

template <typename R> inline R TRoundi(float rhs) {
  return static_cast<R>(rhs + 0.5f);
}

template <typename R> inline R TRoundf(float rhs) {
  return static_cast<R>(rhs + 0.5f);
}

template <> inline float TDiv(float lhs, float rhs) {
  ASSERT(rhs != 0.0f);
  return static_cast<float>(lhs / rhs);
}

template <> inline float TDiv(int lhs, int rhs) {
  ASSERT(rhs != 0);
  return static_cast<float>(lhs / rhs);
}

template <> inline float TPow(float lhs, float rhs) {
  return static_cast<float>(::pow(lhs, rhs));
}

template <> inline float TPow2(float rhs) {
  return static_cast<float>(::pow(2.0f, rhs));
}

template <> inline float TMod(float lhs, float rhs) {
  return static_cast<float>(::fmod(lhs, rhs));
}

template <> inline float TExp(float rhs) {
  return static_cast<float>(::exp(rhs));
}

template <> inline float TLog(float rhs) {
  return static_cast<float>(::log(rhs));
}

template <> inline float TSqrt(float rhs) {
  return static_cast<float>(::sqrt(rhs));
}

template <> inline float TInvSqrt(float rhs) {
  ASSERT(rhs != 0.0f);
  return 1.0f / Math::TSqrt(rhs);
}

template <> inline float TSin(float rhs) {
  return static_cast<float>(::sin(static_cast<float>(rhs)));
}

template <> inline float TCos(float rhs) {
  return static_cast<float>(::cos(static_cast<float>(rhs)));
}

template <> inline int TToUNORM8(float rhs) {
  ASSERT((rhs >= 0.0f) && (rhs <= 1.0f));
  return static_cast<int>(0xff * rhs);
}

template <> inline int TToUNORM16(float rhs) {
  ASSERT((rhs >= 0.0f) && (rhs <= 1.0f));
  return static_cast<int>(0xffff * rhs);
}

inline float TFastMul(float lhs, float rhs) { return lhs * rhs; }

template <typename R> inline R TMul(float lhs, float rhs) {
  return static_cast<R>(lhs * rhs);
}

template <typename R> inline R TMul(float lhs, int rhs) {
  return static_cast<R>(lhs * rhs);
}

template <typename R, unsigned int F, typename T>
inline R TMul(float lhs, TFixed<F, T> rhs) {
  return static_cast<R>(lhs * rhs);
}

inline float MulDiv(float lhs, float rhs) { return lhs / rhs; }

inline float Lerpf(float lhs, float rhs, float scalar) {
  ASSERT((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + (rhs - lhs) * scalar;
}

inline int Lerp(int lhs, int rhs, float scalar) {
  ASSERT((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + static_cast<int>((rhs - lhs) * scalar);
}

template <> inline fixed4 TMul<fixed4>(float fOffset, float fRhw, int iScale) {
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

template <typename R> inline R TMulRnd(float a, float b) {
  return static_cast<R>(a * b);
}

template <typename R> inline R TMulRnd(float lhs, int rhs) {
  return static_cast<R>(lhs * rhs);
}

template <typename R> inline R TMulRnd(float a, float b, int c) {
  return static_cast<R>(a * b * c);
}

template <typename R> inline R TMulRnd(float a, float b, float c) {
  return static_cast<R>(a * b * c);
}

template <typename R> inline R TMulSub(float a, float b, float c, float d) {
  return static_cast<R>(a * b - c * d);
}

template <typename R> inline R TShiftLeft(float lhs, int rhs) {
  const float scale = (float)(1 << rhs);
  return static_cast<R>(lhs * scale);
}

template <typename R> inline R TShiftRight(float lhs, int rhs) {
  const float scale = 1.0f / (float)(1 << rhs);
  return static_cast<R>(lhs * scale);
}

//////////////////////////////////////////////////////////////////////////////

template <unsigned int F, typename T>
inline TFixed<F, T> TExp(TFixed<F, T> rhs) {
  return static_cast<TFixed<F, T>>((float)exp(Math::TCast<float>(rhs)));
}

template <typename R, unsigned int F, typename T>
inline R TCeili(TFixed<F, T> rhs) {
  const int value = rhs.GetRaw() + TFixed<F, T>::MASK;
  return static_cast<R>(value >> TFixed<F, T>::FRAC);
}

template <typename R, unsigned int F, typename T>
inline R TCeilf(TFixed<F, T> rhs) {
  const int value = (rhs.GetRaw() + TFixed<F, T>::MASK) & TFixed<F, T>::IMASK;
  return R::Make(value >> (TFixed<F, T>::FRAC - R::FRAC));
}

template <typename R, unsigned int F, typename T>
inline R TFloori(TFixed<F, T> rhs) {
  return static_cast<R>(rhs.GetRaw() >> TFixed<F, T>::FRAC);
}

template <typename R, unsigned int F, typename T>
inline R TFloorf(TFixed<F, T> rhs) {
  const int value = rhs.GetRaw() & TFixed<F, T>::IMASK;
  return R::Make(value >> (TFixed<F, T>::FRAC - R::FRAC));
}

template <typename R, unsigned int F, typename T>
inline R TRoundi(TFixed<F, T> rhs) {
  return static_cast<R>((rhs.GetRaw() + TFixed<F, T>::HALF) >>
                        TFixed<F, T>::FRAC);
}

template <typename R, unsigned int F, typename T>
inline R TRoundf(TFixed<F, T> rhs) {
  const int value = (rhs.GetRaw() + TFixed<F, T>::HALF) & TFixed<F, T>::IMASK;
  return R::Make(value >> (TFixed<F, T>::FRAC - R::FRAC));
}

template <typename R, unsigned int F, typename T>
inline R TDiv(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  ASSERT(rhs.GetRaw() != 0);
  return R::Make((static_cast<int64_t>(lhs.GetRaw()) << R::FRAC) /
                 rhs.GetRaw());
}

template <> inline fixed28 TDiv(int lhs, int rhs) {
  ASSERT(rhs != 0);
  return fixed28::Make((static_cast<int64_t>(lhs) << fixed28::FRAC) / rhs);
}

template <> inline fixed16 TSqrt(fixed16 rhs) {
  ASSERT(rhs.GetRaw() > 0);
  return fixed16::Make(Math::iSqrt(rhs.GetRaw()) << 8);
}

template <unsigned int F, typename T> inline int TToUNORM8(TFixed<F, T> rhs) {
  ASSERT((rhs >= TConst<TFixed<F, T>>::Zero()) &&
         (rhs <= TConst<TFixed<F, T>>::One()));
  return (0xff * rhs.GetRaw()) >> TFixed<F, T>::FRAC;
}

template <unsigned int F, typename T> inline int TToUNORM16(TFixed<F, T> rhs) {
  ASSERT((rhs >= TConst<TFixed<F, T>>::Zero()) &&
         (rhs <= TConst<TFixed<F, T>>::One()));
  return (0xffff * rhs.GetRaw()) >> TFixed<F, T>::FRAC;
}

template <typename R, unsigned int F, typename T> class TInvSelect {
public:
  inline static R Invert(TFixed<F, T> rhs) {
    return R::Make(detail::TShiftInverter<typename R::Type,
                                          TFixed<F, T>::FRAC +
                                              R::FRAC>::call(rhs.GetRaw()));
  }
};

template <unsigned int F, typename T> class TInvSelect<float, F, T> {
public:
  inline static float Invert(TFixed<F, T> rhs) {
    return 1.0f / Math::TCast<float>(rhs);
  }
};

template <typename R, unsigned int F, typename T>
inline R TInv(TFixed<F, T> rhs) {
  return TInvSelect<R, F, T>::Invert(rhs);
}

template <typename R> inline R TInv(float rhs) {
  ASSERT(rhs != 0.0f);
  return static_cast<R>(1.0f / rhs);
}

template <typename R, unsigned int F1, unsigned int F2, typename T1,
          typename T2>
inline R TFastMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  ASSERT((static_cast<int64_t>(lhs.GetRaw()) * rhs.GetRaw()) ==
         (lhs.GetRaw() * rhs.GetRaw()));
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  return R::Make((lhs.GetRaw() * rhs.GetRaw()) >> FRAC);
}

template <typename R, unsigned int F1, unsigned int F2, typename T1,
          typename T2>
inline R TFastDiv(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  ASSERT((static_cast<int64_t>(lhs.GetRaw()) << FRAC) ==
         (lhs.GetRaw() << FRAC));
  return R::Make((lhs.GetRaw() << FRAC) / rhs.GetRaw());
}

template <typename R, unsigned int F, typename T>
inline R TMulRnd(TFixed<F, T> lhs, int rhs) {
  const int FRAC = TFixed<F, T>::FRAC - R::FRAC;
  const int HALF = 1 << (FRAC - 1);
  const int64_t value = static_cast<int64_t>(lhs.GetRaw()) * rhs;
  return R::Make((value + HALF) >> FRAC);
}

template <typename R, unsigned int F1, unsigned int F2, typename T1,
          typename T2>
inline R TMulRnd(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  const int HALF = 1 << (FRAC - 1);
  const int64_t value = static_cast<int64_t>(lhs.GetRaw()) * rhs.GetRaw();
  return R::Make((value + HALF) >> FRAC);
}

template <typename R, unsigned int F1, unsigned int F2, typename T1,
          typename T2>
inline R TMulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, int c) {
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  const int64_t HALF = static_cast<int64_t>(1) << (FRAC - 1);
  const int64_t value = static_cast<int64_t>(a.GetRaw()) * b.GetRaw() * c;
  return R::Make((value + HALF) >> FRAC);
}

template <typename R, unsigned int F1, unsigned int F2, unsigned int F3,
          typename T1, typename T2, typename T3>
inline R TMulRnd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F3, T3> c) {
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC +
                   TFixed<F3, T3>::FRAC - R::FRAC;
  const int64_t HALF = static_cast<int64_t>(1) << (FRAC - 1);
  const int64_t value =
      static_cast<int64_t>(a.GetRaw()) * b.GetRaw() * c.GetRaw();
  return R::Make((value + HALF) >> FRAC);
}

template <typename R, unsigned int F1, unsigned int F2, typename T1,
          typename T2>
inline R TMulAdd(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c,
                 TFixed<F2, T2> d) {
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  const int64_t ab = static_cast<int64_t>(a.GetRaw()) * b.GetRaw();
  const int64_t cd = static_cast<int64_t>(c.GetRaw()) * d.GetRaw();
  return R::Make((ab + cd) >> FRAC);
}

template <typename R, unsigned int F1, unsigned int F2, typename T1,
          typename T2>
inline R TMulSub(TFixed<F1, T1> a, TFixed<F2, T2> b, TFixed<F1, T1> c,
                 TFixed<F2, T2> d) {
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  const int64_t ab = static_cast<int64_t>(a.GetRaw()) * b.GetRaw();
  const int64_t cd = static_cast<int64_t>(c.GetRaw()) * d.GetRaw();
  return R::Make((ab - cd) >> FRAC);
}

template <typename R, unsigned int F, typename T>
inline R TMul(TFixed<F, T> lhs, TFixed<F, T> rhs) {
  const int FRAC = TFixed<F, T>::FRAC + TFixed<F, T>::FRAC - R::FRAC;
  const int64_t value = static_cast<int64_t>(lhs.GetRaw()) * rhs.GetRaw();
  return R::Make(value >> FRAC);
}

template <typename R, unsigned int F1, unsigned int F2, typename T1,
          typename T2>
inline R TMul(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs) {
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  const int64_t value = static_cast<int64_t>(lhs.GetRaw()) * rhs.GetRaw();
  return R::Make(value >> FRAC);
}

template <typename R, unsigned int F, typename T>
inline R TMul(TFixed<F, T> lhs, int rhs) {
  const int FRAC = TFixed<F, T>::FRAC - R::FRAC;
  const int64_t value = static_cast<int64_t>(lhs.GetRaw()) * rhs;
  return R::Make(value >> FRAC);
}

template <typename R, unsigned int F, typename T>
R TMul(TFixed<F, T> lhs, float rhs) {
  return static_cast<R>(rhs * lhs);
}

template <typename R, unsigned int F1, unsigned int F2, typename T1,
          typename T2>
inline R TMulShift(TFixed<F1, T1> lhs, TFixed<F2, T2> rhs, R a, int shift) {
  const int FRAC = TFixed<F1, T1>::FRAC + TFixed<F2, T2>::FRAC - R::FRAC;
  const int64_t value = static_cast<int64_t>(lhs.GetRaw()) * rhs.GetRaw();
  return R::Make((value >> FRAC) - (a.GetRaw() >> shift));
}

template <unsigned int F1, unsigned int F2, typename T1, typename T2>
inline TFixed<F1, T1> Lerpf(TFixed<F1, T1> lhs, TFixed<F1, T1> rhs,
                            TFixed<F2, T2> scalar) {
  ASSERT((scalar >= TConst<TFixed<F2, T2>>::Zero()) &&
         (scalar <= TConst<TFixed<F2, T2>>::One()));
  return lhs + (rhs - lhs) * scalar;
}

template <unsigned int F, typename T>
inline TFixed<F, T> Lerpf(TFixed<F, T> lhs, TFixed<F, T> rhs, float scalar) {
  ASSERT((scalar >= 0.0f) && (scalar <= 1.0f));
  return lhs + (rhs - lhs) * scalar;
}

template <unsigned int F, typename T>
inline int Lerp(int lhs, int rhs, TFixed<F, T> scalar) {
  ASSERT((scalar >= TConst<TFixed<F, T>>::Zero()) &&
         (scalar <= TConst<TFixed<F, T>>::One()));
  return lhs +
         static_cast<int>((static_cast<int64_t>(rhs - lhs) * scalar.GetRaw()) >>
                          TFixed<F, T>::FRAC);
}

template <typename R, unsigned int F, typename T>
inline R TShiftLeft(TFixed<F, T> lhs, int rhs) {
  return R(lhs) << rhs;
}

template <typename R, unsigned int F, typename T>
inline R TShiftRight(TFixed<F, T> lhs, int rhs) {
  return R(lhs) >> rhs;
}

//////////////////////////////////////////////////////////////////////////////

template <>
inline fixed16 TDot<fixed16>(const TVector3<fixed16> &lhs,
                             const TVector3<fixed16> &rhs) {
  const int64_t xx = static_cast<int64_t>(lhs.x.GetRaw()) * rhs.x.GetRaw();
  const int64_t yy = static_cast<int64_t>(lhs.y.GetRaw()) * rhs.y.GetRaw();
  const int64_t zz = static_cast<int64_t>(lhs.z.GetRaw()) * rhs.z.GetRaw();
  return fixed16::Make((xx + yy + zz) >> fixed16::FRAC);
}

template <>
inline fixed16 TDot<fixed16>(const TVector4<fixed16> &lhs,
                             const TVector4<fixed16> &rhs) {
  const int64_t xx = static_cast<int64_t>(lhs.x.GetRaw()) * rhs.x.GetRaw();
  const int64_t yy = static_cast<int64_t>(lhs.y.GetRaw()) * rhs.y.GetRaw();
  const int64_t zz = static_cast<int64_t>(lhs.z.GetRaw()) * rhs.z.GetRaw();
  const int64_t ww = static_cast<int64_t>(lhs.w.GetRaw()) * rhs.w.GetRaw();
  return fixed16::Make((xx + yy + zz + ww) >> fixed16::FRAC);
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
  ASSERT(pvOut);

  const floatf x = vIn.x;
  const floatf y = vIn.y;
  const floatf z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32) + mat._42;
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33) + mat._43;
  pvOut->w = MulAdd(x, mat._14, y, mat._24, z, mat._34) + mat._44;
}

inline void Mul4x3(VECTOR3 *pvOut, const VECTOR3 &vIn, const MATRIX44 &mat) {
  ASSERT(pvOut);

  const floatf x = vIn.x;
  const floatf y = vIn.y;
  const floatf z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32) + mat._42;
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33) + mat._43;
}

inline void Mul(VECTOR3 *pvOut, const VECTOR3 &vIn, const MATRIX44 &mat) {
  ASSERT(pvOut);

  const floatf x = vIn.x;
  const floatf y = vIn.y;
  const floatf z = vIn.z;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32);
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33);
}

inline void Mul(VECTOR4 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat) {
  ASSERT(pvOut);

  const floatf x = vIn.x;
  const floatf y = vIn.y;
  const floatf z = vIn.z;
  const floatf w = vIn.w;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31, w, mat._41);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32, w, mat._42);
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33, w, mat._43);
  pvOut->w = MulAdd(x, mat._14, y, mat._24, z, mat._34, w, mat._44);
}

inline void Mul(VECTOR3 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat) {
  ASSERT(pvOut);

  const floatf x = vIn.x;
  const floatf y = vIn.y;
  const floatf z = vIn.z;
  const floatf w = vIn.w;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31, w, mat._41);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32, w, mat._42);
  pvOut->z = MulAdd(x, mat._13, y, mat._23, z, mat._33, w, mat._43);
}

inline void Mul(VECTOR2 *pvOut, const VECTOR4 &vIn, const MATRIX44 &mat) {
  ASSERT(pvOut);

  const floatf x = vIn.x;
  const floatf y = vIn.y;
  const floatf z = vIn.z;
  const floatf w = vIn.w;

  pvOut->x = MulAdd(x, mat._11, y, mat._21, z, mat._31, w, mat._41);
  pvOut->y = MulAdd(x, mat._12, y, mat._22, z, mat._32, w, mat._42);
}

inline void Mul(VECTOR2 *pvOut, const VECTOR2 &vIn, const MATRIX44 &mat) {
  ASSERT(pvOut);

  const floatf x = vIn.x;
  const floatf y = vIn.y;

  pvOut->x = MulAdd(x, mat._11, y, mat._21) + mat._41;
  pvOut->y = MulAdd(x, mat._12, y, mat._22) + mat._42;
}

inline floatf Length(const VECTOR3 &vIn) {
  const floatf fDot = Math::TDot<floatf>(vIn, vIn);
  if (!Math::TIsZero(fDot - fONE)) {
    return Math::TSqrt(fDot);
  }

  return fDot;
}

inline void Normalize(VECTOR3 *pvOut) {
  ASSERT(pvOut);

  const floatf fDot = Math::TDot<floatf>(*pvOut, *pvOut);
  if (!Math::TIsZero(fDot - fONE) && !Math::TIsZero(fDot)) {
    const floatf fInvLen = Math::TInvSqrt(fDot);
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
  ASSERT(pmatOut);

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
  ASSERT(pmatOut);

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
  ASSERT(pmatOut);

  pmatOut->ToIdentity();

  pmatOut->_11 = x;
  pmatOut->_22 = y;
  pmatOut->_33 = z;
}

inline void Translate(MATRIX44 *pmatOut, floatf x, floatf y, floatf z) {
  ASSERT(pmatOut);

  pmatOut->ToIdentity();

  pmatOut->_41 = x;
  pmatOut->_42 = y;
  pmatOut->_43 = z;
}

inline bool IsPowerOf2(int value) { return 0 == (value & (value - 1)); }

inline floatf DegToRad(floatf rhs) { return rhs * (fPI / f180); }

inline floatf RadToDeg(floatf rhs) { return rhs * (f180 / fPI); }

inline unsigned int iLog2(unsigned int rhs) { return 31 - Clz(rhs); }

inline void Mul(MATRIX44 *pmatOut, const MATRIX44 &mat1, const MATRIX44 &mat2) {
  ASSERT(pmatOut);

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
  ASSERT(pmatOut);

  pmatOut->_11 = MulSub(matIn._22, matIn._33, matIn._23, matIn._32);
  pmatOut->_12 = -MulSub(matIn._21, matIn._33, matIn._23, matIn._31);
  pmatOut->_13 = MulSub(matIn._21, matIn._32, matIn._22, matIn._31);

  pmatOut->_21 = -MulSub(matIn._12, matIn._33, matIn._13, matIn._32);
  pmatOut->_22 = MulSub(matIn._11, matIn._33, matIn._13, matIn._31);
  pmatOut->_23 = -MulSub(matIn._11, matIn._32, matIn._12, matIn._31);

  pmatOut->_31 = MulSub(matIn._12, matIn._23, matIn._13, matIn._22);
  pmatOut->_32 = -MulSub(matIn._11, matIn._23, matIn._13, matIn._21);
  pmatOut->_33 = MulSub(matIn._11, matIn._22, matIn._12, matIn._21);

  const floatf fDet = MulAdd(matIn._11, pmatOut->_11, matIn._21, pmatOut->_21,
                             matIn._31, pmatOut->_31);

  if (!Math::TIsZero(fDet - fONE)) {
    const floatf fDetInv = Math::TInv<floatf>(fDet);

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
  ASSERT(pmatOut);

  const floatf fB0 = MulSub(matIn._13, matIn._24, matIn._23, matIn._14);
  const floatf fB1 = MulSub(matIn._13, matIn._34, matIn._33, matIn._14);
  const floatf fB2 = MulSub(matIn._13, matIn._44, matIn._43, matIn._14);
  const floatf fB3 = MulSub(matIn._23, matIn._34, matIn._33, matIn._24);
  const floatf fB4 = MulSub(matIn._23, matIn._44, matIn._43, matIn._24);
  const floatf fB5 = MulSub(matIn._33, matIn._44, matIn._43, matIn._34);

  pmatOut->_11 = (matIn._22 * fB5 - matIn._32 * fB4 + matIn._42 * fB3);
  pmatOut->_12 = (-matIn._12 * fB5 + matIn._32 * fB2 - matIn._42 * fB1);
  pmatOut->_13 = (matIn._12 * fB4 - matIn._22 * fB2 + matIn._42 * fB0);
  pmatOut->_14 = (-matIn._12 * fB3 + matIn._22 * fB1 - matIn._32 * fB0);

  pmatOut->_21 = (-matIn._21 * fB5 + matIn._31 * fB4 - matIn._41 * fB3);
  pmatOut->_22 = (matIn._11 * fB5 - matIn._31 * fB2 + matIn._41 * fB1);
  pmatOut->_23 = (-matIn._11 * fB4 + matIn._21 * fB2 - matIn._41 * fB0);
  pmatOut->_24 = (matIn._11 * fB3 - matIn._21 * fB1 + matIn._31 * fB0);

  const floatf fA0 = MulSub(matIn._11, matIn._22, matIn._21, matIn._12);
  const floatf fA1 = MulSub(matIn._11, matIn._32, matIn._31, matIn._12);
  const floatf fA2 = MulSub(matIn._11, matIn._42, matIn._41, matIn._12);
  const floatf fA3 = MulSub(matIn._21, matIn._32, matIn._31, matIn._22);
  const floatf fA4 = MulSub(matIn._21, matIn._42, matIn._41, matIn._22);
  const floatf fA5 = MulSub(matIn._31, matIn._42, matIn._41, matIn._32);

  pmatOut->_31 = (matIn._24 * fA5 - matIn._34 * fA4 + matIn._44 * fA3);
  pmatOut->_32 = (-matIn._14 * fA5 + matIn._34 * fA2 - matIn._44 * fA1);
  pmatOut->_33 = (matIn._14 * fA4 - matIn._24 * fA2 + matIn._44 * fA0);
  pmatOut->_34 = (-matIn._14 * fA3 + matIn._24 * fA1 - matIn._34 * fA0);

  pmatOut->_41 = (-matIn._23 * fA5 + matIn._33 * fA4 - matIn._43 * fA3);
  pmatOut->_42 = (matIn._13 * fA5 - matIn._33 * fA2 + matIn._43 * fA1);
  pmatOut->_43 = (-matIn._13 * fA4 + matIn._23 * fA2 - matIn._43 * fA0);
  pmatOut->_44 = (matIn._13 * fA3 - matIn._23 * fA1 + matIn._33 * fA0);

  const floatf fDet = MulAdd(fA0, fB5, fA2, fB3, fA3, fB2, fA5, fB0) -
                      MulAdd(fA1, fB4, fA4, fB1);

  if (!Math::TIsZero(fDet - fONE)) {
    const floatf fDetInv = Math::TInv<floatf>(fDet);

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
  ASSERT(pmatOut);

  const floatf fWidth = right - left;
  const floatf fHeight = top - bottom;
  const floatf fDepth = zFar - zNear;

  const floatf fInvWidth =
      (fWidth != fZERO) ? Math::TInv<floatf>(fWidth) : fZERO;
  const floatf fInvHeight =
      (fHeight != fZERO) ? Math::TInv<floatf>(fHeight) : fZERO;
  const floatf fInvDepth =
      (fDepth != fZERO) ? Math::TInv<floatf>(fDepth) : fZERO;

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
  ASSERT(pmatOut);

  const floatf fWidth = right - left;
  const floatf fHeight = top - bottom;
  const floatf fDepth = zFar - zNear;

  const floatf fInvWidth =
      (fWidth != fZERO) ? Math::TInv<floatf>(fWidth) : fZERO;
  const floatf fInvHeight =
      (fHeight != fZERO) ? Math::TInv<floatf>(fHeight) : fZERO;
  const floatf fInvDepth =
      (fDepth != fZERO) ? Math::TInv<floatf>(fDepth) : fZERO;

  const floatf fTwoNear = fTWO * zNear;

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
  ASSERT(pmatOut);

  VECTOR3 vAxis(x, y, z);
  Math::Normalize(&vAxis);

  const floatf fSin = Math::TSin(angle);
  const floatf fCos = Math::TCos(angle);
  const floatf fICos = fONE - fCos;

  const floatf _fA = fICos * vAxis.x * vAxis.y;
  const floatf _fB = fICos * vAxis.x * vAxis.z;
  const floatf _fC = fICos * vAxis.y * vAxis.z;

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