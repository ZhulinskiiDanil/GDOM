#pragma once

#include <cstdint>

namespace gdom
{

  enum class DirtyFlags : uint8_t
  {
    None = 0,

    Paint = 1 << 0,
    Layout = 1 << 1,
    Tree = 1 << 2,

    DescendantLayout = 1 << 3
  };

  inline DirtyFlags operator|(
      DirtyFlags a,
      DirtyFlags b)
  {
    return static_cast<DirtyFlags>(
        static_cast<uint8_t>(a) |
        static_cast<uint8_t>(b));
  }

  inline DirtyFlags operator&(
      DirtyFlags a,
      DirtyFlags b)
  {
    return static_cast<DirtyFlags>(
        static_cast<uint8_t>(a) &
        static_cast<uint8_t>(b));
  }

  inline DirtyFlags &operator|=(
      DirtyFlags &a,
      DirtyFlags b)
  {
    a = a | b;

    return a;
  }

  inline bool hasFlag(
      DirtyFlags value,
      DirtyFlags flag)
  {
    return (
               static_cast<uint8_t>(value) &
               static_cast<uint8_t>(flag)) != 0;
  }

}