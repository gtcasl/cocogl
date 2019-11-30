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

template <uint32_t F, typename T = int32_t> class TFixed {
public:
  using data_type = T;

  enum {
    FRAC = F,
    INT = sizeof(T) * 8 - FRAC,
    HFRAC = FRAC >> 1,
  };

  static const T ONE = static_cast<T>(1) << FRAC;
  static const T MASK = ONE - 1;
  static const T IMASK = ~MASK;
  static const T HALF = ONE >> 1;
  static const T TWO = ONE << 1;

  TFixed() {}

  explicit TFixed(float rhs)
      : data_(static_cast<T>(rhs * (static_cast<T>(1) << F))) {
    assert(data_ == static_cast<T>(rhs * ONE));
  }

  explicit TFixed(int64_t rhs) : data_(static_cast<T>(rhs << FRAC)) {
    assert((static_cast<int64_t>(rhs) << FRAC) == data_);
  }

  explicit TFixed(uint64_t rhs) : data_(static_cast<T>(rhs << FRAC)) {
    assert((static_cast<int64_t>(rhs) << FRAC) == data_);
  }

  explicit TFixed(int32_t rhs) : data_(static_cast<T>(rhs << FRAC)) {
    assert((static_cast<int64_t>(rhs) << FRAC) == data_);
  }

  explicit TFixed(uint32_t rhs) : data_(static_cast<T>(rhs << FRAC)) {
    assert((static_cast<int64_t>(rhs) << FRAC) == data_);
  }

  explicit TFixed(int16_t rhs) : data_(static_cast<T>(rhs << FRAC)) {
    assert((static_cast<int64_t>(rhs) << FRAC) == data_);
  }

  explicit TFixed(uint16_t rhs) : data_(static_cast<T>(rhs << FRAC)) {
    assert((static_cast<int64_t>(rhs) << FRAC) == data_);
  }

  explicit TFixed(int8_t rhs) : data_(static_cast<T>(rhs << FRAC)) {
    assert((static_cast<int64_t>(rhs) << FRAC) == data_);
  }

  explicit TFixed(uint8_t rhs) : data_(static_cast<T>(rhs << FRAC)) {
    assert((static_cast<int64_t>(rhs) << FRAC) == data_);
  }

  template <uint32_t F2, typename T2> explicit TFixed(TFixed<F2, T2> rhs) {
    if constexpr (sizeof(T) > sizeof(T2)) {
      if constexpr (F2 > F) {
        data_ = static_cast<T>(rhs) >> (F2 - F);
      } else {
        data_ = static_cast<T>(rhs) << (F - F2);
      }
    } else {
      if constexpr (F2 > F) {
        data_ = static_cast<T>(rhs >> (F2 - F));
      } else {
        data_ = static_cast<T>(rhs << (F - F2));
      }
    }
  }

  auto operator==(TFixed rhs) const { return (data_ == rhs.data_); }

  auto operator!=(TFixed rhs) const { return (data_ != rhs.data_); }

  auto operator<(TFixed rhs) const { return (data_ < rhs.data_); }

  auto operator<=(TFixed rhs) const { return (data_ <= rhs.data_); }

  auto operator>(TFixed rhs) const { return (data_ > rhs.data_); }

  auto operator>=(TFixed rhs) const { return (data_ >= rhs.data_); }

  auto operator-() const { return make(-data_); }

  auto operator+=(TFixed rhs) {
    *this = (*this) + rhs;
    return *this;
  }

  auto operator-=(TFixed rhs) {
    *this = (*this) - rhs;
    return *this;
  }

  auto operator*=(TFixed rhs) {
    *this = (*this) * rhs;
    return *this;
  }

  auto operator/=(TFixed rhs) {
    *this = (*this) / rhs;
    return *this;
  }

  template <uint32_t F2, typename T2> auto operator*=(TFixed<F2, T2> rhs) {
    *this = (*this) * rhs;
    return *this;
  }

  template <uint32_t F2, typename T2> auto operator/=(TFixed<F2, T2> rhs) {
    *this = (*this) / rhs;
    return *this;
  }

  auto operator*=(int32_t rhs) {
    *this = (*this) * rhs;
    return *this;
  }

  auto operator*=(uint32_t rhs) {
    *this = (*this) * rhs;
    return *this;
  }

  auto operator*=(float rhs) {
    *this = (*this) * rhs;
    return *this;
  }

  auto operator/=(int32_t rhs) {
    *this = (*this) / rhs;
    return *this;
  }

  auto operator/=(uint32_t rhs) {
    *this = (*this) / rhs;
    return *this;
  }

  auto operator/=(float rhs) {
    *this = (*this) / rhs;
    return *this;
  }

  friend auto operator+(TFixed lhs, TFixed rhs) {
    assert((static_cast<int64_t>(lhs.data_) + rhs.data_) ==
           (lhs.data_ + rhs.data_));
    return TFixed::make(lhs.data_ + rhs.data_);
  }

  friend auto operator-(TFixed lhs, TFixed rhs) {
    assert((static_cast<int64_t>(lhs.data_) - rhs.data_) ==
           (lhs.data_ - rhs.data_));
    return TFixed::make(lhs.data_ - rhs.data_);
  }

  friend auto operator*(TFixed lhs, TFixed rhs) {
    return TFixed::make((static_cast<int64_t>(lhs.data_) * rhs.data_) >> FRAC);
  }

  template <uint32_t F2, typename T2>
  friend auto operator*(TFixed lhs, TFixed<F2, T2> rhs) {
    return TFixed::make((static_cast<int64_t>(lhs.data()) * rhs.data()) >>
                        FRAC);
  }

  friend auto operator/(TFixed lhs, TFixed rhs) {
    assert(rhs.data_ != 0);
    return TFixed::make((static_cast<int64_t>(lhs.data_) << FRAC) / rhs.data_);
  }

  template <uint32_t F2, typename T2>
  friend auto operator/(TFixed lhs, TFixed<F2, T2> rhs) {
    assert(rhs.data() != 0);
    return TFixed::make((static_cast<int64_t>(lhs.data()) << FRAC) /
                        rhs.data());
  }

  friend auto operator*(TFixed lhs, float rhs) {
    return TFixed(static_cast<float>(lhs) * rhs);
  }

  friend auto operator*(float lhs, TFixed rhs) {
    return TFixed(lhs * static_cast<float>(rhs));
  }

  friend auto operator/(TFixed lhs, float rhs) {
    return TFixed(static_cast<float>(lhs) / rhs);
  }

  friend auto operator/(float lhs, TFixed rhs) {
    return TFixed(lhs / static_cast<float>(rhs));
  }

  friend auto operator*(TFixed lhs, char rhs) {
    return lhs * static_cast<int32_t>(rhs);
  }

  friend auto operator*(char lhs, TFixed rhs) { return rhs * lhs; }

  friend auto operator/(TFixed lhs, char rhs) {
    return lhs / static_cast<int32_t>(rhs);
  }

  friend auto operator/(char lhs, TFixed rhs) { return rhs / lhs; }

  friend auto operator*(TFixed lhs, uint8_t rhs) {
    return lhs * static_cast<int32_t>(rhs);
  }

  friend auto operator*(uint8_t lhs, TFixed rhs) { return rhs * lhs; }

  friend auto operator/(TFixed lhs, uint8_t rhs) {
    return lhs / static_cast<int32_t>(rhs);
  }

  friend auto operator/(uint8_t lhs, TFixed rhs) { return rhs / lhs; }

  friend auto operator*(TFixed lhs, short rhs) {
    return lhs * static_cast<int32_t>(rhs);
  }

  friend auto operator*(short lhs, TFixed rhs) { return rhs * lhs; }

  friend auto operator/(TFixed lhs, short rhs) {
    return lhs / static_cast<int32_t>(rhs);
  }

  friend auto operator/(short lhs, TFixed rhs) { return rhs / lhs; }

  friend auto operator*(TFixed lhs, uint16_t rhs) {
    return lhs * static_cast<int32_t>(rhs);
  }

  friend auto operator*(uint16_t lhs, TFixed rhs) { return rhs * lhs; }

  friend auto operator/(TFixed lhs, uint16_t rhs) {
    return lhs / static_cast<int32_t>(rhs);
  }

  friend auto operator/(uint16_t lhs, TFixed rhs) { return rhs / lhs; }

  friend auto operator*(TFixed lhs, int32_t rhs) {
    auto value = static_cast<T>(lhs.data_ * rhs);
    assert((lhs.data_ * static_cast<int64_t>(rhs)) == value);
    return TFixed::make(value);
  }

  friend auto operator*(int32_t lhs, TFixed rhs) { return rhs * lhs; }

  friend auto operator/(TFixed lhs, int32_t rhs) {
    assert(rhs);
    auto value = static_cast<T>(lhs.data_ / rhs);
    return TFixed::make(value);
  }

  friend auto operator/(int32_t lhs, TFixed rhs) { return rhs / lhs; }

  friend auto operator*(TFixed lhs, uint32_t rhs) {
    auto value = static_cast<T>(lhs.data_ << rhs);
    assert((lhs.data_ << static_cast<int64_t>(rhs)) == value);
    return TFixed::make(value);
  }

  friend auto operator*(uint32_t lhs, TFixed rhs) { return rhs * lhs; }

  friend auto operator/(TFixed lhs, uint32_t rhs) {
    assert(rhs);
    auto value = static_cast<T>(lhs.data_ / rhs);
    return TFixed::make(value);
  }

  friend auto operator/(uint32_t lhs, TFixed rhs) { return rhs / lhs; }

  friend auto operator<<(TFixed lhs, int32_t rhs) {
    auto value = static_cast<T>(lhs.data_ << rhs);
    assert((lhs.data_ << static_cast<int64_t>(rhs)) == value);
    return TFixed::make(value);
  }

  friend auto operator>>(TFixed lhs, int32_t rhs) {
    auto value = static_cast<T>(lhs.data_ >> rhs);
    return TFixed::make(value);
  }

  friend auto operator<<(TFixed lhs, uint32_t rhs) {
    auto value = static_cast<T>(lhs.data_ << rhs);
    assert((lhs.data_ << static_cast<int64_t>(rhs)) == value);
    return TFixed::make(value);
  }

  friend auto operator>>(TFixed lhs, uint32_t rhs) {
    auto value = static_cast<T>(lhs.data_ >> rhs);
    return TFixed::make(value);
  }

  static auto make(T value) {
    TFixed ret;
    ret.data_ = value;
    return ret;
  }

  operator float() const {
    return static_cast<float>(data_) / (static_cast<T>(1) << F);
  }

  template <uint32_t F2, typename T2> operator TFixed<F2, T2>() const {
    return TFixed<F2, T2>(*this);
  }

  auto data() const { return data_; }

private:
  template <uint32_t F2, typename T2> friend class TFixed;

  T data_;
};