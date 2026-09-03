#include <GDOM/LengthResolver.hpp>

#include <Geode/Geode.hpp>

#include <algorithm>
#include <cctype>

using namespace geode::prelude;

namespace gdom
{

  std::string LengthResolver::trim(
      const std::string &value)
  {
    auto start =
        std::find_if_not(
            value.begin(),
            value.end(),
            [](unsigned char c)
            {
              return std::isspace(c);
            });

    auto end =
        std::find_if_not(
            value.rbegin(),
            value.rend(),
            [](unsigned char c)
            {
              return std::isspace(c);
            })
            .base();

    if (start >= end)
    {
      return "";
    }

    return std::string(
        start,
        end);
  }

  bool LengthResolver::endsWith(
      const std::string &value,
      const std::string &suffix)
  {
    if (value.size() < suffix.size())
    {
      return false;
    }

    return value.compare(
               value.size() - suffix.size(),
               suffix.size(),
               suffix) == 0;
  }

  bool LengthResolver::isAuto(
      const std::string &rawValue)
  {
    return trim(rawValue) == "auto";
  }

  float LengthResolver::resolve(
      const std::string &rawValue,
      float referenceSize)
  {
    const auto value =
        trim(rawValue);

    if (value.empty() ||
        value == "auto")
    {
      return 0.f;
    }

    std::string numberPart =
        value;

    float multiplier =
        1.f;

    if (endsWith(value, "px"))
    {
      numberPart =
          value.substr(
              0,
              value.size() - 2);
    }
    else if (endsWith(value, "rem"))
    {
      numberPart =
          value.substr(
              0,
              value.size() - 3);

      multiplier =
          10.f;
    }
    else if (endsWith(value, "%"))
    {
      numberPart =
          value.substr(
              0,
              value.size() - 1);

      multiplier =
          referenceSize / 100.f;
    }

    auto number =
        numFromString<float>(
            trim(numberPart));

    if (number.isErr())
    {
      log::warn(
          "GDOM: invalid length '{}'",
          rawValue);

      return 0.f;
    }

    return number.unwrap() *
           multiplier;
  }

}