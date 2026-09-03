#include <GDOM/HTMLSpanElement.hpp>

#include <GDOM/LengthResolver.hpp>

#include <algorithm>

using namespace geode::prelude;

namespace gdom
{

    HTMLSpanElement *HTMLSpanElement::create()
    {
        return new HTMLSpanElement();
    }

    CCSize HTMLSpanElement::measureText() const
    {
        auto label =
            CCLabelBMFont::create(
                textContent.c_str(),
                "bigFont.fnt");

        if (!label)
        {
            return {0.f, 0.f};
        }

        const auto rawSize =
            label->getContentSize();

        if (rawSize.height <= 0.f)
        {
            return {0.f, 0.f};
        }

        const float requestedFontSize =
            LengthResolver::resolve(
                style.fontSize,
                rawSize.height);

        const float scale =
            requestedFontSize /
            rawSize.height;

        return {
            rawSize.width * scale,
            rawSize.height * scale};
    }

    CCSize HTMLSpanElement::resolveSize(
        const CCSize &parentSize) const
    {
        return resolveSize(
            parentSize,
            parentSize);
    }

    CCSize HTMLSpanElement::resolveSize(
        const CCSize &containingSize,
        const CCSize &availableSize) const
    {
        const auto textSize =
            measureText();

        float width = 0.f;

        if (LengthResolver::isAuto(style.width))
        {
            width =
                textSize.width;
        }
        else
        {
            width =
                LengthResolver::resolve(
                    style.width,
                    containingSize.width);
        }

        float height = 0.f;

        if (LengthResolver::isAuto(style.height))
        {
            height =
                textSize.height;
        }
        else
        {
            height =
                LengthResolver::resolve(
                    style.height,
                    containingSize.height);
        }

        return {
            std::max(0.f, width),
            std::max(0.f, height)};
    }

    CCNode *HTMLSpanElement::render(
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

        container->setContentSize(
            getContentSize());

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

        auto label =
            CCLabelBMFont::create(
                textContent.c_str(),
                "bigFont.fnt");

        if (!label)
        {
            return container;
        }

        const auto rawSize =
            label->getContentSize();

        if (rawSize.height > 0.f)
        {
            const float requestedFontSize =
                LengthResolver::resolve(
                    style.fontSize,
                    rawSize.height);

            const float scale =
                requestedFontSize /
                rawSize.height;

            label->setScale(scale);
        }

        label->setAnchorPoint({0.f,
                               1.f});

        label->setPosition({0.f,
                            getContentSize().height});

        label->setColor({style.color.r,
                         style.color.g,
                         style.color.b});

        label->setOpacity(
            style.color.a);

        container->addChild(label);

        return container;
    }

}