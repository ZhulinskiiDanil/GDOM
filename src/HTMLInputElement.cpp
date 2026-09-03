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
        float width = 0.f;

        if (LengthResolver::isAuto(
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

        float height = 0.f;

        if (LengthResolver::isAuto(
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

        //
        // Background + border
        //

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

        auto background =
            RoundedRectNode::create(
                size,
                style.backgroundColor,
                borderRadius,
                borderWidth,
                style.borderColor);

        if (background)
        {
            background->setAnchorPoint({0.f,
                                        0.f});

            background->setPosition({0.f,
                                     0.f});

            container->addChild(
                background,
                1);
        }

        //
        // Padding
        //

        const auto padding =
            BoxResolver::resolve(
                style.padding,
                style.paddingTop,
                style.paddingRight,
                style.paddingBottom,
                style.paddingLeft,
                size.width,
                size.height);

        //
        // Visible text
        //

        const std::string initialText =
            value.empty()
                ? placeholder
                : value;

        auto label =
            CCLabelBMFont::create(
                initialText.empty()
                    ? " "
                    : initialText.c_str(),
                "bigFont.fnt");

        if (!label)
        {
            return container;
        }

        const auto rawLabelSize =
            label->getContentSize();

        if (rawLabelSize.height > 0.f)
        {
            const float requestedFontSize =
                LengthResolver::resolve(
                    style.fontSize,
                    rawLabelSize.height);

            label->setScale(
                requestedFontSize /
                rawLabelSize.height);
        }

        label->setAnchorPoint({0.f,
                               0.5f});

        label->setPosition({padding.left,
                            size.height / 2.f});

        if (value.empty())
        {
            label->setColor({style.placeholderColor.r,
                             style.placeholderColor.g,
                             style.placeholderColor.b});

            label->setOpacity(
                style.placeholderColor.a);
        }
        else
        {
            label->setColor({style.color.r,
                             style.color.g,
                             style.color.b});

            label->setOpacity(
                style.color.a);
        }

        container->addChild(
            label,
            3);

        //
        // GDOM caret
        //

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

        //
        // Native TextInput
        //
        // Only keyboard / focus / editing.
        //

        auto nativeInput =
            TextInput::create(
                size.width,
                "",
                "bigFont.fnt");

        if (!nativeInput)
        {
            return container;
        }

        nativeInput->hideBG();

        nativeInput->setString(
            value,
            false);

        // Keep native rendering outside
        // the visible GDOM element.
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

        //
        // Sync native input -> GDOM
        //

        nativeInput->setCallback(
            [this, label, caret, padding, size](
                const std::string &newValue)
            {
                value =
                    newValue;

                const bool showPlaceholder =
                    value.empty();

                const auto &displayText =
                    showPlaceholder
                        ? placeholder
                        : value;

                label->setString(
                    displayText.empty()
                        ? " "
                        : displayText.c_str());

                const auto &displayColor =
                    showPlaceholder
                        ? style.placeholderColor
                        : style.color;

                label->setColor({displayColor.r,
                                 displayColor.g,
                                 displayColor.b});

                label->setOpacity(
                    displayColor.a);

                if (caret)
                {
                    const float textWidth =
                        showPlaceholder
                            ? 0.f
                            : label
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

                if (onInput)
                {
                    onInput(
                        newValue);
                }
            });

        container->addChild(
            nativeInput,
            4);

        //
        // Focus target
        //

        auto focusTarget =
            InputFocusTarget::create(
                nativeInput,
                caret);

        if (!focusTarget)
        {
            return container;
        }

        container->addChild(
            focusTarget);

        //
        // Transparent clickable area
        //

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
            return container;
        }

        auto menu =
            CCMenu::create();

        if (!menu)
        {
            return container;
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
            return container;
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

        return container;
    }

}