#include <GDOM/BoxResolver.hpp>
#include <GDOM/LengthResolver.hpp>

#include <sstream>
#include <vector>

namespace gdom
{

  BoxValues BoxResolver::resolve(
      const std::string &shorthand,
      const std::string &top,
      const std::string &right,
      const std::string &bottom,
      const std::string &left,
      float horizontalReference,
      float verticalReference)
  {
    std::vector<std::string> values;

    std::istringstream stream(shorthand);
    std::string value;

    while (stream >> value)
    {
      values.push_back(value);
    }

    std::string resolvedTop = "0px";
    std::string resolvedRight = "0px";
    std::string resolvedBottom = "0px";
    std::string resolvedLeft = "0px";

    if (values.size() == 1)
    {
      resolvedTop = values[0];
      resolvedRight = values[0];
      resolvedBottom = values[0];
      resolvedLeft = values[0];
    }
    else if (values.size() == 2)
    {
      resolvedTop = values[0];
      resolvedBottom = values[0];

      resolvedRight = values[1];
      resolvedLeft = values[1];
    }
    else if (values.size() == 3)
    {
      resolvedTop = values[0];

      resolvedRight = values[1];
      resolvedLeft = values[1];

      resolvedBottom = values[2];
    }
    else if (values.size() == 4)
    {
      resolvedTop = values[0];
      resolvedRight = values[1];
      resolvedBottom = values[2];
      resolvedLeft = values[3];
    }

    if (!top.empty())
    {
      resolvedTop = top;
    }

    if (!right.empty())
    {
      resolvedRight = right;
    }

    if (!bottom.empty())
    {
      resolvedBottom = bottom;
    }

    if (!left.empty())
    {
      resolvedLeft = left;
    }

    return {
        LengthResolver::resolve(
            resolvedTop,
            verticalReference),

        LengthResolver::resolve(
            resolvedRight,
            horizontalReference),

        LengthResolver::resolve(
            resolvedBottom,
            verticalReference),

        LengthResolver::resolve(
            resolvedLeft,
            horizontalReference)};
  }

}