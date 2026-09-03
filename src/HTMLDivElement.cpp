#include <GDOM/HTMLDivElement.hpp>
#include <GDOM/RoundedRectNode.hpp>

#include <GDOM/LengthResolver.hpp>

#include <algorithm>

using namespace geode::prelude;

namespace gdom
{

    HTMLDivElement *
    HTMLDivElement::create()
    {
        return new HTMLDivElement();
    }

    CCNode *HTMLDivElement::render(
        const CCSize &parentSize,
        const CCPoint &flowOffset)
    {
        if (!hasResolvedSize())
        {
            setResolvedSize(
                resolveSize(parentSize));
        }

        auto container =
            CCNode::create();

        if (!container)
        {
            return nullptr;
        }

        const auto size =
            getContentSize();

        container->setContentSize(
            size);

        container->setAnchorPoint({0.f,
                                   1.f});

        const float left =
            LengthResolver::resolve(
                style.left,
                parentSize.width);

        const float top =
            LengthResolver::resolve(
                style.top,
                parentSize.height);

        container->setPosition({flowOffset.x + left,

                                parentSize.height -
                                    flowOffset.y -
                                    top});

        const float borderWidth =
            std::max(
                0.f,
                LengthResolver::resolve(
                    style.borderWidth,
                    std::min(
                        size.width,
                        size.height)));

        const float borderRadius =
            std::max(
                0.f,
                LengthResolver::resolve(
                    style.borderRadius,
                    std::min(
                        size.width,
                        size.height)));

        //
        // Background
        //

        auto background =
            RoundedRectNode::create(
                size,
                style.backgroundColor,
                borderRadius,
                borderWidth,
                style.borderColor);

        if (background)
        {
            background->setPosition({0.f,
                                     0.f});

            container->addChild(
                background,
                -1);
        }

        renderChildren(
            container);

        return container;
    }

}