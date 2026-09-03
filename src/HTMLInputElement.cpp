#include <GDOM/HTMLInputElement.hpp>
#include <GDOM/RoundedRectNode.hpp>

#include <GDOM/BoxResolver.hpp>
#include <GDOM/LengthResolver.hpp>

#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>

#include <algorithm>

using namespace geode::prelude;

namespace gdom
{

    HTMLInputElement *HTMLInputElement::create()
    {
        return new HTMLInputElement();
    }

    CCSize HTMLInputElement::resolveSize(
        const CCSize &parentSize) const
    {
        return resolveSize(
            parentSize,
            parentSize);
    }

    CCSize HTMLInputElement::resolveSize(
        const CCSize &containingSize,
        const CCSize &availableSize) const
    {
        float width =
            0.f;

        if (
            LengthResolver::isAuto(
                style.width))
        {
            width =
                std::min(
                    200.f,
                    availableSize.width);
        }
        else
        {
            width =
                LengthResolver::resolve(
                    style.width,
                    containingSize.width);
        }

        if (!style.minWidth.get().empty())
        {
            width =
                std::max(
                    width,
                    LengthResolver::resolve(
                        style.minWidth,
                        containingSize.width));
        }

        if (!style.maxWidth.get().empty())
        {
            width =
                std::min(
                    width,
                    LengthResolver::resolve(
                        style.maxWidth,
                        containingSize.width));
        }

        width =
            std::max(
                0.f,
                width);

        float height =
            0.f;

        if (
            LengthResolver::isAuto(
                style.height))
        {
            height =
                40.f;
        }
        else
        {
            height =
                LengthResolver::resolve(
                    style.height,
                    containingSize.height);
        }

        if (!style.minHeight.get().empty())
        {
            height =
                std::max(
                    height,
                    LengthResolver::resolve(
                        style.minHeight,
                        containingSize.height));
        }

        if (!style.maxHeight.get().empty())
        {
            height =
                std::min(
                    height,
                    LengthResolver::resolve(
                        style.maxHeight,
                        containingSize.height));
        }

        height =
            std::max(
                0.f,
                height);

        return {
            width,
            height};
    }

    CCNode *HTMLInputElement::render(
        const CCSize &parentSize,
        const CCPoint &flowOffset)
    {
        if (!hasResolvedSize())
        {
            setResolvedSize(
                resolveSize(
                    parentSize));
        }

        const auto size =
            getContentSize();

        m_background =
            nullptr;

        m_input =
            nullptr;

        auto container =
            CCNode::create();

        if (!container)
        {
            return nullptr;
        }

        container->setContentSize(
            size);

        container->setAnchorPoint({
            0.f,
            1.f});

        const float left =
            LengthResolver::resolve(
                style.left,
                parentSize.width);

        const float top =
            LengthResolver::resolve(
                style.top,
                parentSize.height);

        container->setPosition({
            flowOffset.x +
                left,

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

        m_background =
            RoundedRectNode::create(
                size,
                style.backgroundColor.get(),
                borderRadius,
                borderWidth,
                style.borderColor.get());

        if (m_background)
        {
            m_background->setAnchorPoint({
                0.f,
                0.f});

            m_background->setPosition({
                0.f,
                0.f});

            container->addChild(
                m_background,
                1);
        }

        const auto padding =
            BoxResolver::resolve(
                style.padding,
                style.paddingTop,
                style.paddingRight,
                style.paddingBottom,
                style.paddingLeft,
                size.width,
                size.height);

        const float inputWidth =
            std::max(
                1.f,
                size.width -
                    padding.left -
                    padding.right);

        m_input =
            TextInput::create(
                inputWidth,
                placeholder.get(),
                "bigFont.fnt");

        if (!m_input)
        {
            applyPaint();

            return finishRender(
                container);
        }

        m_input->hideBG();

        m_input->setTextAlign(
            TextInputAlign::Left);

        m_input->setString(
            value.get(),
            false);

        m_input->setPosition({
            padding.left +
                inputWidth / 2.f,

            size.height / 2.f});

        m_input->setCallback(
            [this](
                const std::string &newValue)
            {
                value =
                    newValue;

                if (onInput)
                {
                    onInput(
                        newValue);
                }
            });

        container->addChild(
            m_input,
            5);

        applyNativeStyle();
        applyPaint();

        return finishRender(
            container);
    }

    void HTMLInputElement::applyNativeStyle()
    {
        if (!m_input)
        {
            return;
        }

        auto inputNode =
            m_input->getInputNode();

        if (!inputNode)
        {
            return;
        }

        const auto &textColor =
            style.color.get();

        const auto &placeholderColor =
            style.placeholderColor.get();

        inputNode->setLabelNormalColor({
            textColor.r,
            textColor.g,
            textColor.b});

        inputNode->setLabelPlaceholderColor({
            placeholderColor.r,
            placeholderColor.g,
            placeholderColor.b});

        auto label =
            inputNode->getTextLabel();

        if (!label)
        {
            return;
        }

        const auto rawSize =
            label->getContentSize();

        if (rawSize.height <= 0.f)
        {
            return;
        }

        const float fontSize =
            std::max(
                0.f,
                LengthResolver::resolve(
                    style.fontSize,
                    rawSize.height));

        const float scale =
            fontSize /
            rawSize.height;

        inputNode->setMaxLabelScale(
            scale);

        inputNode->setLabelPlaceholderScale(
            scale);

        label->setScale(
            scale);

        label->setOpacity(
            textColor.a);
    }

    void HTMLInputElement::applyPaint()
    {
        if (m_background)
        {
            m_background->setFillColor(
                style.backgroundColor.get());

            m_background->setBorderColor(
                style.borderColor.get());

            const auto size =
                getContentSize();

            const float borderRadius =
                std::max(
                    0.f,
                    LengthResolver::resolve(
                        style.borderRadius,
                        std::min(
                            size.width,
                            size.height)));

            m_background->setRadius(
                borderRadius);
        }

        applyNativeStyle();
    }

}
