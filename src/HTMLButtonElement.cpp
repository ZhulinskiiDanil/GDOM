#include <GDOM/HTMLButtonElement.hpp>
#include <GDOM/RoundedRectNode.hpp>

#include <GDOM/LengthResolver.hpp>

#include <algorithm>

using namespace geode::prelude;

namespace gdom
{

    namespace
    {

        class ButtonCallbackTarget : public CCNode
        {
        public:
            static ButtonCallbackTarget *create(
                HTMLButtonElement *element)
            {
                auto target =
                    new ButtonCallbackTarget();

                if (!target)
                {
                    return nullptr;
                }

                if (!target->init())
                {
                    delete target;
                    return nullptr;
                }

                target->m_element =
                    element;

                target->autorelease();

                return target;
            }

            void onPressed(
                CCObject *)
            {
                if (!m_element)
                {
                    return;
                }

                if (m_element->onClick)
                {
                    m_element->onClick();
                }
            }

        private:
            HTMLButtonElement *m_element =
                nullptr;
        };

    }

    HTMLButtonElement *HTMLButtonElement::create()
    {
        return new HTMLButtonElement();
    }

    CCSize HTMLButtonElement::measureText() const
    {
        auto label =
            CCLabelBMFont::create(
                textContent.c_str(),
                "bigFont.fnt");

        if (!label)
        {
            return {
                0.f,
                0.f};
        }

        const auto rawSize =
            label->getContentSize();

        if (rawSize.height <= 0.f)
        {
            return {
                0.f,
                0.f};
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

    CCSize HTMLButtonElement::resolveSize(
        const CCSize &parentSize) const
    {
        return resolveSize(
            parentSize,
            parentSize);
    }

    CCSize HTMLButtonElement::resolveSize(
        const CCSize &containingSize,
        const CCSize &availableSize) const
    {
        const auto textSize =
            measureText();

        float width =
            0.f;

        if (LengthResolver::isAuto(
                style.width))
        {
            width =
                textSize.width +
                20.f;
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

        if (LengthResolver::isAuto(
                style.height))
        {
            height =
                textSize.height +
                12.f;
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

    CCNode *HTMLButtonElement::render(
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

        //
        // Normal GDOM container.
        //
        // This node participates in layout.
        //

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
        // Button visual content.
        //

        auto buttonContent =
            CCNode::create();

        if (!buttonContent)
        {
            return container;
        }

        buttonContent->setContentSize(
            size);

        buttonContent->setAnchorPoint({0.f,
                                       0.f});

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

            buttonContent->addChild(
                background,
                0);
        }

        //
        // Text
        //

        auto label =
            CCLabelBMFont::create(
                textContent.c_str(),
                "bigFont.fnt");

        if (label)
        {
            const auto rawSize =
                label->getContentSize();

            if (rawSize.height > 0.f)
            {
                const float requestedFontSize =
                    LengthResolver::resolve(
                        style.fontSize,
                        rawSize.height);

                label->setScale(
                    requestedFontSize /
                    rawSize.height);
            }

            label->setColor({style.color.r,
                             style.color.g,
                             style.color.b});

            label->setOpacity(
                style.color.a);

            label->setAnchorPoint({0.5f,
                                   0.5f});

            label->setPosition({size.width / 2.f,
                                size.height / 2.f});

            buttonContent->addChild(
                label,
                1);
        }

        //
        // Callback target
        //

        auto callbackTarget =
            ButtonCallbackTarget::create(
                this);

        if (!callbackTarget)
        {
            return container;
        }

        container->addChild(
            callbackTarget);

        //
        // Menu is now INTERNAL.
        //
        // It no longer controls GDOM layout position.
        //

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

        //
        // Clickable item
        //

        auto item =
            CCMenuItemSpriteExtra::create(
                buttonContent,
                callbackTarget,
                menu_selector(
                    ButtonCallbackTarget::onPressed));

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