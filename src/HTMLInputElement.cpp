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

    namespace
    {

        class InputFocusTarget : public CCNode
        {
        public:
            static InputFocusTarget *create(
                TextInput *input,
                CCLayerColor *caret)
            {
                auto target =
                    new InputFocusTarget();

                if (!target)
                {
                    return nullptr;
                }

                if (!target->init())
                {
                    delete target;
                    return nullptr;
                }

                target->m_input =
                    input;

                target->m_caret =
                    caret;

                target->autorelease();

                return target;
            }

            void focus(
                CCObject *)
            {
                if (!m_input)
                {
                    return;
                }

                m_input->focus();

                if (m_caret)
                {
                    m_caret->setVisible(
                        true);
                }
            }

        private:
            TextInput *m_input =
                nullptr;

            CCLayerColor *m_caret =
                nullptr;
        };

    }

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

        return {
            std::max(
                0.f,
                width),

            std::max(
                0.f,
                height)};
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

        auto container =
            CCNode::create();

        if (!container)
        {
            return nullptr;
        }

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

        m_background =
            RoundedRectNode::create(
                size,
                style.backgroundColor.get(),
                borderRadius,
                borderWidth,
                style.borderColor.get());

        if (m_background)
        {
            m_background->setAnchorPoint({0.f,
                                          0.f});

            m_background->setPosition({0.f,
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

        const std::string initialText =
            value.get().empty()
                ? placeholder.get()
                : value.get();

        m_label =
            CCLabelBMFont::create(
                initialText.empty()
                    ? " "
                    : initialText.c_str(),
                "bigFont.fnt");

        if (!m_label)
        {
            return finishRender(
                container);
        }

        const auto rawLabelSize =
            m_label->getContentSize();

        if (rawLabelSize.height > 0.f)
        {
            const float requestedFontSize =
                LengthResolver::resolve(
                    style.fontSize,
                    rawLabelSize.height);

            m_label->setScale(
                requestedFontSize /
                rawLabelSize.height);
        }

        m_label->setAnchorPoint({0.f,
                                 0.5f});

        m_label->setPosition({padding.left,
                              size.height / 2.f});

        container->addChild(
            m_label,
            3);

        auto caret =
            CCLayerColor::create(
                {255,
                 255,
                 255,
                 255},
                1.5f,
                18.f);

        if (caret)
        {
            caret->setVisible(
                false);

            caret->setPosition({padding.left,
                                size.height / 2.f -
                                    9.f});

            container->addChild(
                caret,
                4);
        }

        auto nativeInput =
            TextInput::create(
                size.width,
                "",
                "bigFont.fnt");

        if (!nativeInput)
        {
            applyPaint();

            return finishRender(
                container);
        }

        nativeInput->hideBG();

        nativeInput->setString(
            value.get(),
            false);

        nativeInput->setPosition({-10000.f,
                                  -10000.f});

        auto inputNode =
            nativeInput->getInputNode();

        if (inputNode)
        {
            auto nativeLabel =
                inputNode->getTextLabel();

            if (nativeLabel)
            {
                nativeLabel->setVisible(
                    false);
            }
        }

        nativeInput->setCallback(
            [this, caret, padding, size](
                const std::string &newValue)
            {
                value =
                    newValue;

                if (m_label)
                {
                    const bool showPlaceholder =
                        value.get().empty();

                    const std::string &displayText =
                        showPlaceholder
                            ? placeholder.get()
                            : value.get();

                    m_label->setString(
                        displayText.empty()
                            ? " "
                            : displayText.c_str());

                    applyPaint();

                    if (caret)
                    {
                        const float textWidth =
                            showPlaceholder
                                ? 0.f
                                : m_label
                                      ->getScaledContentSize()
                                      .width;

                        caret->setPosition({padding.left +
                                                textWidth +
                                                2.f,

                                            size.height / 2.f -
                                                caret
                                                        ->getContentSize()
                                                        .height /
                                                    2.f});
                    }
                }

                if (onInput)
                {
                    onInput(
                        newValue);
                }
            });

        container->addChild(
            nativeInput,
            4);

        auto focusTarget =
            InputFocusTarget::create(
                nativeInput,
                caret);

        if (!focusTarget)
        {
            applyPaint();

            return finishRender(
                container);
        }

        container->addChild(
            focusTarget);

        auto hitbox =
            CCLayerColor::create(
                {0,
                 0,
                 0,
                 0},
                size.width,
                size.height);

        if (!hitbox)
        {
            applyPaint();

            return finishRender(
                container);
        }

        auto menu =
            CCMenu::create();

        if (!menu)
        {
            applyPaint();

            return finishRender(
                container);
        }

        menu->setContentSize(
            size);

        menu->setAnchorPoint({0.f,
                              0.f});

        menu->setPosition({0.f,
                           0.f});

        auto item =
            CCMenuItemSpriteExtra::create(
                hitbox,
                focusTarget,
                menu_selector(
                    InputFocusTarget::focus));

        if (!item)
        {
            applyPaint();

            return finishRender(
                container);
        }

        item->setAnchorPoint({0.5f,
                              0.5f});

        item->setPosition({size.width / 2.f,
                           size.height / 2.f});

        menu->addChild(
            item);

        container->addChild(
            menu,
            10);

        applyPaint();

        return finishRender(
            container);
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

        if (!m_label)
        {
            return;
        }

        const bool showPlaceholder =
            value.get().empty();

        const std::string &displayText =
            showPlaceholder
                ? placeholder.get()
                : value.get();

        m_label->setString(
            displayText.empty()
                ? " "
                : displayText.c_str());

        const auto &displayColor =
            showPlaceholder
                ? style.placeholderColor.get()
                : style.color.get();

        m_label->setColor({displayColor.r,
                           displayColor.g,
                           displayColor.b});

        m_label->setOpacity(
            displayColor.a);
    }

}