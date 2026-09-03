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
                textContent.get().c_str(),
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

        auto buttonContent =
            CCNode::create();

        if (!buttonContent)
        {
            return finishRender(
                container);
        }

        buttonContent->setContentSize(
            size);

        buttonContent->setAnchorPoint({0.f,
                                       0.f});

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

            buttonContent->addChild(
                m_background,
                0);
        }

        m_label =
            CCLabelBMFont::create(
                textContent.get().c_str(),
                "bigFont.fnt");

        if (m_label)
        {
            const auto rawSize =
                m_label->getContentSize();

            if (rawSize.height > 0.f)
            {
                const float requestedFontSize =
                    LengthResolver::resolve(
                        style.fontSize,
                        rawSize.height);

                m_label->setScale(
                    requestedFontSize /
                    rawSize.height);
            }

            m_label->setAnchorPoint({0.5f,
                                     0.5f});

            m_label->setPosition({size.width / 2.f,
                                  size.height / 2.f});

            buttonContent->addChild(
                m_label,
                1);
        }

        applyPaint();

        auto callbackTarget =
            ButtonCallbackTarget::create(
                this);

        if (!callbackTarget)
        {
            return finishRender(
                container);
        }

        container->addChild(
            callbackTarget);

        auto menu =
            CCMenu::create();

        if (!menu)
        {
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
                buttonContent,
                callbackTarget,
                menu_selector(
                    ButtonCallbackTarget::onPressed));

        if (!item)
        {
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

        return finishRender(
            container);
    }

    void HTMLButtonElement::applyPaint()
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

        if (m_label)
        {
            const auto &color =
                style.color.get();

            m_label->setColor({color.r,
                               color.g,
                               color.b});

            m_label->setOpacity(
                color.a);
        }
    }

}