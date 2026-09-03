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

        //
        // Root cocos node of this div
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

        //
        // DOM position
        //

        const float left =
            LengthResolver::resolve(
                style.left,
                parentSize.width);

        const float top =
            LengthResolver::resolve(
                style.top,
                parentSize.height);

        container->setPosition({flowOffset.x +
                                    left,

                                parentSize.height -
                                    flowOffset.y -
                                    top});

        //
        // Border
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

        //
        // Background
        //
        // Background belongs to viewport,
        // so it must NOT scroll with children.
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

        //
        // Overflow mode
        //

        const bool overflowAuto =
            style.overflow == "auto";

        const bool overflowScroll =
            style.overflow == "scroll";

        //
        // Normal div
        //

        if (
            !overflowAuto &&
            !overflowScroll)
        {
            renderChildren(
                container);

            return container;
        }

        //
        // Measure real content height
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

        //
        // overflow:auto only becomes scrollable
        // if content is actually larger.
        //

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

            return container;
        }

        //
        // ------------------------------------------------
        // Scroll backend
        // ------------------------------------------------
        //

        auto scroll =
            ScrollLayer::create(
                size);

        if (!scroll)
        {
            renderChildren(
                container);

            return container;
        }

        scroll->setPosition({0.f,
                             0.f});

        //
        // Tell ScrollLayer how large the entire
        // scrollable content actually is.
        //

        scroll->setContentLayerSize({size.width,
                                     contentHeight});

        //
        // ------------------------------------------------
        // GDOM layout host
        // ------------------------------------------------
        //
        // IMPORTANT:
        //
        // renderChildren() must NOT receive m_contentLayer
        // directly because m_contentLayer can be 400-500px
        // high while our viewport is only 140px.
        //
        // GDOM uses a top-left coordinate system and expects
        // the parent height to represent the visible box.
        //
        // So we create a normal CCNode with the ORIGINAL
        // viewport size and let GDOM layout against that.
        //
        // Children are allowed to extend below this CCNode;
        // CCNode itself does not clip them.
        //

        //
        // Temporary GDOM layout host
        //

        auto layoutHost =
            CCNode::create();

        if (!layoutHost)
        {
            renderChildren(
                container);

            return container;
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

        //
        // Let GDOM calculate everything normally.
        //

        renderChildren(
            layoutHost);

        //
        // IMPORTANT:
        //
        // Do NOT add layoutHost to ScrollLayer.
        //
        // ScrollLayer performs visibility optimization
        // on direct children of m_contentLayer.
        //
        // If the whole layoutHost becomes invisible,
        // every GDOM element inside disappears at once.
        //
        // Instead move every rendered GDOM child directly
        // into m_contentLayer while preserving its position.
        //

        std::vector<CCNode *> nodes;

        for (auto node : layoutHost->getChildrenExt())
        {
            if (node)
            {
                nodes.push_back(node);
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
                ->addChild(node);

            node->release();
        }

        scroll->scrollToTop();

        container->addChild(
            scroll);

        return container;
    }

}