#include <algorithm>

#include <GDOM/HTMLElement.hpp>
#include <GDOM/LengthResolver.hpp>
#include <GDOM/BoxResolver.hpp>
#include <GDOM/layout/BlockLayout.hpp>
#include <GDOM/layout/FlexLayout.hpp>

using namespace geode::prelude;

namespace gdom
{

    void HTMLElement::appendChild(
        HTMLElement *child)
    {
        if (!child)
        {
            return;
        }

        child->m_parentElement = this;

        m_children.push_back(child);
    }

    HTMLElement *
    HTMLElement::getParentElement() const
    {
        return m_parentElement;
    }

    const std::vector<HTMLElement *> &
    HTMLElement::getChildren() const
    {
        return m_children;
    }

    CCSize HTMLElement::getContentSize() const
    {
        return m_resolvedSize;
    }

    void HTMLElement::setResolvedSize(
        const CCSize &size)
    {
        m_resolvedSize = size;
        m_hasResolvedSize = true;
    }

    bool HTMLElement::hasResolvedSize() const
    {
        return m_hasResolvedSize;
    }

    CCSize HTMLElement::resolveSize(
        const CCSize &parentSize) const
    {
        return resolveSize(
            parentSize,
            parentSize);
    }

    CCSize HTMLElement::resolveSize(
        const CCSize &containingSize,
        const CCSize &availableSize) const
    {
        float width = 0.f;

        if (LengthResolver::isAuto(style.width))
        {
            width = availableSize.width;
        }
        else
        {
            width = LengthResolver::resolve(
                style.width,
                containingSize.width);
        }

        float height = 0.f;

        if (LengthResolver::isAuto(style.height))
        {
            height = measureAutoHeight(
                {width,
                 availableSize.height},
                width);
        }
        else
        {
            height = LengthResolver::resolve(
                style.height,
                containingSize.height);
        }

        return {
            std::max(0.f, width),
            std::max(0.f, height)};
    }

    float HTMLElement::measureAutoHeight(
        const CCSize &parentSize,
        float resolvedWidth) const
    {
        const auto padding =
            BoxResolver::resolve(
                style.padding,
                style.paddingTop,
                style.paddingRight,
                style.paddingBottom,
                style.paddingLeft,
                resolvedWidth,
                parentSize.height);

        const float gap =
            LengthResolver::resolve(
                style.gap,
                parentSize.height);

        if (m_children.empty())
        {
            return padding.top +
                   padding.bottom;
        }

        const float innerWidth =
            std::max(
                0.f,
                resolvedWidth -
                    padding.left -
                    padding.right);

        const CCSize childContainingSize{
            innerWidth,
            parentSize.height};

        if (
            style.display == "flex" &&
            style.flexDirection == "row")
        {
            float largestChildHeight =
                0.f;

            for (auto *child : m_children)
            {
                if (!child)
                {
                    continue;
                }

                const auto margin =
                    BoxResolver::resolve(
                        child->style.margin,
                        child->style.marginTop,
                        child->style.marginRight,
                        child->style.marginBottom,
                        child->style.marginLeft,
                        innerWidth,
                        parentSize.height);

                const CCSize availableSize{
                    std::max(
                        0.f,
                        innerWidth -
                            margin.left -
                            margin.right),

                    parentSize.height};

                const auto childSize =
                    child->resolveSize(
                        childContainingSize,
                        availableSize);

                const float totalHeight =
                    margin.top +
                    childSize.height +
                    margin.bottom;

                largestChildHeight =
                    std::max(
                        largestChildHeight,
                        totalHeight);
            }

            return padding.top +
                   largestChildHeight +
                   padding.bottom;
        }

        float contentHeight =
            0.f;

        int childCount =
            0;

        for (auto *child : m_children)
        {
            if (!child)
            {
                continue;
            }

            const auto margin =
                BoxResolver::resolve(
                    child->style.margin,
                    child->style.marginTop,
                    child->style.marginRight,
                    child->style.marginBottom,
                    child->style.marginLeft,
                    innerWidth,
                    parentSize.height);

            const CCSize availableSize{
                std::max(
                    0.f,
                    innerWidth -
                        margin.left -
                        margin.right),

                parentSize.height};

            const auto childSize =
                child->resolveSize(
                    childContainingSize,
                    availableSize);

            contentHeight +=
                margin.top +
                childSize.height +
                margin.bottom;

            childCount++;
        }

        if (childCount > 1)
        {
            contentHeight +=
                gap *
                static_cast<float>(
                    childCount - 1);
        }

        return padding.top +
               contentHeight +
               padding.bottom;
    }

    void HTMLElement::renderChildren(
        CCNode *node)
    {
        if (!node)
        {
            return;
        }

        if (style.display == "flex")
        {
            layout::FlexLayout::render(
                this,
                node);

            return;
        }

        layout::BlockLayout::render(
            this,
            node);
    }

}