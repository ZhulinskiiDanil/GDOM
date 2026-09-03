#pragma once

#include <Geode/Geode.hpp>
#include <string>

using namespace geode::prelude;

namespace gdom
{

    struct Style
    {
        std::string display = "block";
        std::string flexDirection = "row";
        std::string justifyContent = "flex-start";
        std::string alignItems = "flex-start";

        std::string overflow = "visible";

        ccColor4B backgroundColor{
            0,
            0,
            0,
            0};

        ccColor4B borderColor{
            255,
            255,
            255,
            255};

        ccColor4B placeholderColor{
            150,
            150,
            150,
            255};

        std::string borderRadius = "0px";
        std::string borderWidth = "0px";

        std::string left = "0px";
        std::string top = "0px";

        std::string width = "0px";
        std::string height = "0px";

        // Shorthand
        std::string margin = "0px";
        std::string padding = "0px";

        // Empty = use shorthand value
        std::string marginTop = "";
        std::string marginRight = "";
        std::string marginBottom = "";
        std::string marginLeft = "";

        std::string paddingTop = "";
        std::string paddingRight = "";
        std::string paddingBottom = "";
        std::string paddingLeft = "";

        std::string gap = "0px";

        // Text
        std::string fontSize = "16px";

        ccColor4B color{
            255,
            255,
            255,
            255};
    };

}