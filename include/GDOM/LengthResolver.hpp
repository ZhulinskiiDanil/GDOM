#pragma once

#include <string>

namespace gdom
{

  class LengthResolver
  {
  public:
    static float resolve(
        const std::string &value,
        float referenceSize);

    static bool isAuto(
        const std::string &value);

  private:
    static std::string trim(
        const std::string &value);

    static bool endsWith(
        const std::string &value,
        const std::string &suffix);

    static float resolveSingle(
        const std::string &value,
        float referenceSize);

    static float resolveCalc(
        const std::string &value,
        float referenceSize);
  };

}