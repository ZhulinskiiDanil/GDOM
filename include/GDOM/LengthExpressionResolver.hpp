#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace gdom
{

    class LengthExpressionResolver
    {
    public:
        static std::optional<float> resolve(
            std::string_view value,
            float referenceSize);

    private:
        explicit LengthExpressionResolver(
            std::string_view value,
            float referenceSize);

        std::optional<float> parseExpression();

        std::optional<float> parseUnary();

        std::optional<float> parsePrimary();

        std::optional<float> parseLength();

        std::optional<std::string>
        parseIdentifier();

        bool consume(
            char value);

        void skipWhitespace();

        bool isAtEnd() const;

        char peek() const;

        static bool isAsciiLetter(
            char value);

        static char toLowerAscii(
            char value);

        std::string_view m_value;

        std::size_t m_position =
            0;

        float m_referenceSize =
            0.f;
    };

}
