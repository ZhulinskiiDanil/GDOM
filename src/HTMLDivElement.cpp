#include <GDOM/HTMLDivElement.hpp>
#include <GDOM/RoundedRectNode.hpp>

#include <GDOM/LengthResolver.hpp>
#include <GDOM/BoxResolver.hpp>

#include <Geode/ui/ScrollLayer.hpp>

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
            m_background->setPosition({0.f,
                                       0.f});

            container->addChild(
                m_background,
                -1);
        }

        const bool overflowAuto =
            style.overflow == "auto";

        const bool overflowScroll =
            style.overflow == "scroll";

        if (
            !overflowAuto &&
            !overflowScroll)
        {
            renderChildren(
                container);

            return finishRender(
                container);
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

        const float innerWidth =
            std::max(
                0.f,
                size.width -
                    padding.left -
                    padding.right);

        const float measuredHeight =
            measureAutoHeight(
                size,
                innerWidth);

        const float contentHeight =
            std::max(
                size.height,
                measuredHeight);

        const bool needsScroll =
            overflowScroll ||
            (overflowAuto &&
             contentHeight >
                 size.height +
                     0.01f);

        if (!needsScroll)
        {
            renderChildren(
                container);

            return finishRender(
                container);
        }

        auto scroll =
            ScrollLayer::create(
                size);

        if (!scroll)
        {
            renderChildren(
                container);

            return finishRender(
                container);
        }

        scroll->setPosition({0.f,
                             0.f});

        scroll->setContentLayerSize({size.width,
                                     contentHeight});

        auto layoutHost =
            CCNode::create();

        if (!layoutHost)
        {
            renderChildren(
                container);

            return finishRender(
                container);
        }

        layoutHost->setContentSize(
            size);

        layoutHost->setAnchorPoint({0.f,
                                    0.f});

        const CCPoint layoutHostPosition = {
            0.f,
            contentHeight -
                size.height};

        layoutHost->setPosition(
            layoutHostPosition);

        renderChildren(
            layoutHost);

        std::vector<CCNode *> nodes;

        for (auto node :
             layoutHost->getChildrenExt())
        {
            if (node)
            {
                nodes.push_back(
                    node);
            }
        }

        for (auto node : nodes)
        {
            node->retain();

            const auto position =
                node->getPosition();

            node->removeFromParentAndCleanup(
                false);

            node->setPosition({position.x +
                                   layoutHostPosition.x,

                               position.y +
                                   layoutHostPosition.y});

            scroll->m_contentLayer
                ->addChild(
                    node);

            node->release();
        }

        scroll->scrollToTop();

        container->addChild(
            scroll);

        return finishRender(
            container);
    }

    void HTMLDivElement::applyPaint()
    {
        if (!m_background)
        {
            return;
        }

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

}