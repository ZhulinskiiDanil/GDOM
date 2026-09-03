#include <GDOM/LengthResolver.hpp>
#include <GDOM/LengthExpressionResolver.hpp>

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

    bool LengthResolver::equalsIgnoreCase(
        const std::string &left,
        const std::string &right)
    {
        if (
            left.size() !=
            right.size())
        {
            return false;
        }

        for (
            std::size_t i = 0;
            i < left.size();
            ++i)
        {
            const auto leftChar =
                static_cast<unsigned char>(
                    left[i]);

            const auto rightChar =
                static_cast<unsigned char>(
                    right[i]);

            if (
                std::tolower(leftChar) !=
                std::tolower(rightChar))
            {
                return false;
            }
        }

        return true;
    }

    bool LengthResolver::isAuto(
        const std::string &rawValue)
    {
        return equalsIgnoreCase(
            trim(rawValue),
            "auto");
    }

    float LengthResolver::resolve(
        const std::string &rawValue,
        float referenceSize)
    {
        const auto value =
            trim(rawValue);

        if (
            value.empty() ||
            isAuto(value))
        {
            return 0.f;
        }

        auto result =
            LengthExpressionResolver::resolve(
                value,
                referenceSize);

        if (!result.has_value())
        {
            log::warn(
                "GDOM: invalid length expression '{}'",
                rawValue);

            return 0.f;
        }

        return *result;
    }

}
