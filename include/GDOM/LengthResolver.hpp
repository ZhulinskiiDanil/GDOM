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

        static bool equalsIgnoreCase(
            const std::string &left,
            const std::string &right);
    };

}