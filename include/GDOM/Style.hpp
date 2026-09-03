#pragma once

#include <Geode/Geode.hpp>

#include <GDOM/ReactiveProperty.hpp>

#include <functional>
#include <string>

using namespace geode::prelude;

namespace gdom
{

    struct Style
    {
        //
        // Layout
        //

        ReactiveProperty<std::string>
            display{
                "block",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            flexDirection{
                "row",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            justifyContent{
                "flex-start",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            alignItems{
                "flex-start",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            overflow{
                "visible",
                DirtyFlags::Layout};

        //
        // Position / size
        //

        ReactiveProperty<std::string>
            left{
                "0px",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            top{
                "0px",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            width{
                "0px",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            height{
                "0px",
                DirtyFlags::Layout};

        //
        // Margin
        //

        ReactiveProperty<std::string>
            margin{
                "0px",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            marginTop{
                "",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            marginRight{
                "",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            marginBottom{
                "",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            marginLeft{
                "",
                DirtyFlags::Layout};

        //
        // Padding
        //

        ReactiveProperty<std::string>
            padding{
                "0px",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            paddingTop{
                "",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            paddingRight{
                "",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            paddingBottom{
                "",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            paddingLeft{
                "",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            gap{
                "0px",
                DirtyFlags::Layout};

        //
        // Typography
        //

        ReactiveProperty<std::string>
            fontSize{
                "16px",
                DirtyFlags::Layout};

        ReactiveProperty<ccColor4B>
            color{
                {255, 255, 255, 255},
                DirtyFlags::Paint};

        ReactiveProperty<ccColor4B>
            placeholderColor{
                {150, 150, 150, 255},
                DirtyFlags::Paint};

        //
        // Box rendering
        //

        ReactiveProperty<ccColor4B>
            backgroundColor{
                {0, 0, 0, 0},
                DirtyFlags::Paint};

        ReactiveProperty<ccColor4B>
            borderColor{
                {255, 255, 255, 255},
                DirtyFlags::Paint};

        ReactiveProperty<std::string>
            borderWidth{
                "0px",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            borderRadius{
                "0px",
                DirtyFlags::Paint};

        void bind(
            std::function<void(DirtyFlags)>
                callback)
        {
            display.bind(callback);
            flexDirection.bind(callback);
            justifyContent.bind(callback);
            alignItems.bind(callback);
            overflow.bind(callback);

            left.bind(callback);
            top.bind(callback);

            width.bind(callback);
            height.bind(callback);

            margin.bind(callback);
            marginTop.bind(callback);
            marginRight.bind(callback);
            marginBottom.bind(callback);
            marginLeft.bind(callback);

            padding.bind(callback);
            paddingTop.bind(callback);
            paddingRight.bind(callback);
            paddingBottom.bind(callback);
            paddingLeft.bind(callback);

            gap.bind(callback);

            fontSize.bind(callback);
            color.bind(callback);
            placeholderColor.bind(callback);

            backgroundColor.bind(callback);
            borderColor.bind(callback);
            borderWidth.bind(callback);
            borderRadius.bind(callback);
        }
    };

}