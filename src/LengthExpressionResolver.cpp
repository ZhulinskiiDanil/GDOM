#include <GDOM/LengthExpressionResolver.hpp>

#include <Geode/Geode.hpp>

#include <cctype>
#include <string>

using namespace geode::prelude;

namespace gdom
{

    LengthExpressionResolver::LengthExpressionResolver(
        std::string_view value,
        float referenceSize)
        : m_value(value),
          m_referenceSize(referenceSize)
    {
    }

    std::optional<float>
    LengthExpressionResolver::resolve(
        std::string_view value,
        float referenceSize)
    {
        LengthExpressionResolver parser(
            value,
            referenceSize);

        auto result =
            parser.parseExpression();

        parser.skipWhitespace();

        if (
            !result.has_value() ||
            !parser.isAtEnd())
        {
            return std::nullopt;
        }

        return result;
    }

    std::optional<float>
    LengthExpressionResolver::parseExpression()
    {
        auto result =
            parseUnary();

        if (!result.has_value())
        {
            return std::nullopt;
        }

        while (true)
        {
            skipWhitespace();

            if (consume('+'))
            {
                auto right =
                    parseUnary();

                if (!right.has_value())
                {
                    return std::nullopt;
                }

                *result +=
                    *right;

                continue;
            }

            if (consume('-'))
            {
                auto right =
                    parseUnary();

                if (!right.has_value())
                {
                    return std::nullopt;
                }

                *result -=
                    *right;

                continue;
            }

            break;
        }

        return result;
    }

    std::optional<float>
    LengthExpressionResolver::parseUnary()
    {
        skipWhitespace();

        float sign =
            1.f;

        while (true)
        {
            if (consume('+'))
            {
                skipWhitespace();
                continue;
            }

            if (consume('-'))
            {
                sign =
                    -sign;

                skipWhitespace();
                continue;
            }

            break;
        }

        auto value =
            parsePrimary();

        if (!value.has_value())
        {
            return std::nullopt;
        }

        return
            sign *
            *value;
    }

    std::optional<float>
    LengthExpressionResolver::parsePrimary()
    {
        skipWhitespace();

        const std::size_t savedPosition =
            m_position;

        auto identifier =
            parseIdentifier();

        if (
            identifier.has_value() &&
            *identifier == "calc")
        {
            skipWhitespace();

            if (!consume('('))
            {
                return std::nullopt;
            }

            auto value =
                parseExpression();

            if (!value.has_value())
            {
                return std::nullopt;
            }

            skipWhitespace();

            if (!consume(')'))
            {
                return std::nullopt;
            }

            return value;
        }

        m_position =
            savedPosition;

        if (consume('('))
        {
            auto value =
                parseExpression();

            if (!value.has_value())
            {
                return std::nullopt;
            }

            skipWhitespace();

            if (!consume(')'))
            {
                return std::nullopt;
            }

            return value;
        }

        return parseLength();
    }

    std::optional<float>
    LengthExpressionResolver::parseLength()
    {
        skipWhitespace();

        const std::size_t numberStart =
            m_position;

        bool hasDigits =
            false;

        bool hasDot =
            false;

        while (!isAtEnd())
        {
            const char current =
                peek();

            if (
                current >= '0' &&
                current <= '9')
            {
                hasDigits =
                    true;

                ++m_position;
                continue;
            }

            if (
                current == '.' &&
                !hasDot)
            {
                hasDot =
                    true;

                ++m_position;
                continue;
            }

            break;
        }

        if (!hasDigits)
        {
            return std::nullopt;
        }

        const auto numberText =
            std::string(
                m_value.substr(
                    numberStart,
                    m_position -
                        numberStart));

        auto number =
            numFromString<float>(
                numberText);

        if (number.isErr())
        {
            return std::nullopt;
        }

        skipWhitespace();

        if (consume('%'))
        {
            return
                number.unwrap() *
                (
                    m_referenceSize /
                    100.f);
        }

        const std::size_t unitPosition =
            m_position;

        auto unit =
            parseIdentifier();

        if (!unit.has_value())
        {
            //
            // GDOM compatibility:
            // unitless values are treated as px.
            //
            return number.unwrap();
        }

        if (*unit == "px")
        {
            return number.unwrap();
        }

        if (*unit == "rem")
        {
            return
                number.unwrap() *
                10.f;
        }

        //
        // Unknown unit.
        //
        m_position =
            unitPosition;

        return std::nullopt;
    }

    std::optional<std::string>
    LengthExpressionResolver::parseIdentifier()
    {
        skipWhitespace();

        if (
            isAtEnd() ||
            !isAsciiLetter(
                peek()))
        {
            return std::nullopt;
        }

        std::string result;

        while (
            !isAtEnd() &&
            isAsciiLetter(
                peek()))
        {
            result.push_back(
                toLowerAscii(
                    peek()));

            ++m_position;
        }

        return result;
    }

    bool LengthExpressionResolver::consume(
        char value)
    {
        if (
            isAtEnd() ||
            peek() != value)
        {
            return false;
        }

        ++m_position;

        return true;
    }

    void LengthExpressionResolver::skipWhitespace()
    {
        while (
            !isAtEnd() &&
            std::isspace(
                static_cast<unsigned char>(
                    peek())))
        {
            ++m_position;
        }
    }

    bool LengthExpressionResolver::isAtEnd() const
    {
        return
            m_position >=
            m_value.size();
    }

    char LengthExpressionResolver::peek() const
    {
        if (isAtEnd())
        {
            return '\0';
        }

        return
            m_value[m_position];
    }

    bool LengthExpressionResolver::isAsciiLetter(
        char value)
    {
        return
            (
                value >= 'a' &&
                value <= 'z') ||
            (
                value >= 'A' &&
                value <= 'Z');
    }

    char LengthExpressionResolver::toLowerAscii(
        char value)
    {
        if (
            value >= 'A' &&
            value <= 'Z')
        {
            return static_cast<char>(
                value -
                'A' +
                'a');
        }

        return value;
    }

}
