#include <GDOM/layout/FlexLayout.hpp>

#include <GDOM/HTMLElement.hpp>
#include <GDOM/LengthResolver.hpp>
#include <GDOM/BoxResolver.hpp>

#include <algorithm>

using namespace geode::prelude;

namespace gdom::layout
{

    void FlexLayout::render(
        HTMLElement *element,
        CCNode *node)
    {
        if (!element || !node)
        {
            return;
        }

        const auto parentSize =
            element->getContentSize();

        const bool isRow =
            element->style.flexDirection == "row";

        const auto padding =
            BoxResolver::resolve(
                element->style.padding,
                element->style.paddingTop,
                element->style.paddingRight,
                element->style.paddingBottom,
                element->style.paddingLeft,
                parentSize.width,
                parentSize.height);

        const float innerWidth =
            std::max(
                0.f,
                parentSize.width -
                    padding.left -
                    padding.right);

        const float innerHeight =
            std::max(
                0.f,
                parentSize.height -
                    padding.top -
                    padding.bottom);

        const float gap =
            LengthResolver::resolve(
                element->style.gap,
                isRow
                    ? innerWidth
                    : innerHeight);

        float childrenMainSize =
            0.f;

        int childCount =
            0;

        const CCSize containingSize{
            innerWidth,
            innerHeight};

        for (auto *child : element->m_children)
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
                    innerHeight);

            const CCSize availableSize{
                std::max(
                    0.f,
                    innerWidth -
                        margin.left -
                        margin.right),

                std::max(
                    0.f,
                    innerHeight -
                        margin.top -
                        margin.bottom)};

            const auto childSize =
                child->resolveSize(
                    containingSize,
                    availableSize);

            if (isRow)
            {
                childrenMainSize +=
                    margin.left +
                    childSize.width +
                    margin.right;
            }
            else
            {
                childrenMainSize +=
                    margin.top +
                    childSize.height +
                    margin.bottom;
            }

            childCount++;
        }

        const float baseGapSize =
            childCount > 1
                ? gap *
                      static_cast<float>(
                          childCount - 1)
                : 0.f;

        const float totalMainSize =
            childrenMainSize +
            baseGapSize;

        const float availableMainSize =
            isRow
                ? innerWidth
                : innerHeight;

        const float freeSpace =
            availableMainSize -
            totalMainSize;

        float mainOffset =
            0.f;

        float distributedGap =
            gap;

        const auto &justifyContent =
            element->style.justifyContent;

        if (justifyContent == "center")
        {
            mainOffset =
                freeSpace / 2.f;
        }
        else if (
            justifyContent == "flex-end")
        {
            mainOffset =
                freeSpace;
        }
        else if (
            justifyContent == "space-between" &&
            childCount > 1)
        {
            distributedGap =
                gap +
                freeSpace /
                    static_cast<float>(
                        childCount - 1);
        }
        else if (
            justifyContent == "space-around" &&
            childCount > 0)
        {
            const float space =
                freeSpace /
                static_cast<float>(
                    childCount);

            distributedGap =
                gap + space;

            mainOffset =
                space / 2.f;
        }
        else if (
            justifyContent == "space-evenly" &&
            childCount > 0)
        {
            const float space =
                freeSpace /
                static_cast<float>(
                    childCount + 1);

            distributedGap =
                gap + space;

            mainOffset =
                space;
        }

        float currentMain =
            mainOffset;

        for (auto *child : element->m_children)
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
                    innerHeight);

            const CCSize availableSize{
                std::max(
                    0.f,
                    innerWidth -
                        margin.left -
                        margin.right),

                std::max(
                    0.f,
                    innerHeight -
                        margin.top -
                        margin.bottom)};

            const auto childSize =
                child->resolveSize(
                    containingSize,
                    availableSize);

            child->setResolvedSize(
                childSize);

            float x =
                padding.left;

            float y =
                padding.top;

            if (isRow)
            {
                currentMain +=
                    margin.left;

                x +=
                    currentMain;

                if (
                    element->style.alignItems ==
                    "center")
                {
                    y +=
                        (innerHeight -
                         childSize.height) /
                        2.f;
                }
                else if (
                    element->style.alignItems ==
                    "flex-end")
                {
                    y +=
                        innerHeight -
                        childSize.height -
                        margin.bottom;
                }
                else
                {
                    y +=
                        margin.top;
                }
            }
            else
            {
                currentMain +=
                    margin.top;

                y +=
                    currentMain;

                if (
                    element->style.alignItems ==
                    "center")
                {
                    x +=
                        (innerWidth -
                         childSize.width) /
                        2.f;
                }
                else if (
                    element->style.alignItems ==
                    "flex-end")
                {
                    x +=
                        innerWidth -
                        childSize.width -
                        margin.right;
                }
                else
                {
                    x +=
                        margin.left;
                }
            }

            CCPoint flowOffset{
                x,
                y};

            auto childNode =
                child->render(
                    parentSize,
                    flowOffset);

            if (childNode)
            {
                node->addChild(
                    childNode);
            }

            if (isRow)
            {
                currentMain +=
                    childSize.width +
                    margin.right +
                    distributedGap;
            }
            else
            {
                currentMain +=
                    childSize.height +
                    margin.bottom +
                    distributedGap;
            }
        }
    }

}