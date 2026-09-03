#pragma once

#include <string>

namespace gdom
{

  struct BoxValues
  {
    float top = 0.f;
    float right = 0.f;
    float bottom = 0.f;
    float left = 0.f;
  };

  class BoxResolver
  {
  public:
    static BoxValues resolve(
        const std::string &shorthand,
        const std::string &top,
        const std::string &right,
        const std::string &bottom,
        const std::string &left,
        float horizontalReference,
        float verticalReference);
  };

}