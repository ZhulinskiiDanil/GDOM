#include <GDOM/layout/FlexLayout.hpp>

#include <GDOM/HTMLElement.hpp>
#include <GDOM/LengthResolver.hpp>
#include <GDOM/BoxResolver.hpp>

#include <algorithm>
#include <vector>

using namespace geode::prelude;

namespace gdom::layout
{

    namespace
    {

        struct FlexMargin
        {
            float top = 0.f;
            float right = 0.f;
            float bottom = 0.f;
            float left = 0.f;
        };

        struct FlexItem
        {
            HTMLElement *element =
                nullptr;

            FlexMargin margin;

            CCSize baseSize{
                0.f,
                0.f};

            CCSize finalSize{
                0.f,
                0.f};

            float grow =
                0.f;

            float shrink =
                1.f;

            bool frozen =
                false;
        };

        float getMainSize(
            const CCSize &size,
            bool isRow)
        {
            return isRow
                       ? size.width
                       : size.height;
        }

        void setMainSize(
            CCSize &size,
            bool isRow,
            float value)
        {
            value =
                std::max(
                    0.f,
                    value);

            if (isRow)
            {
                size.width =
                    value;
            }
            else
            {
                size.height =
                    value;
            }
        }

        float getMainMargin(
            const FlexMargin &margin,
            bool isRow)
        {
            if (isRow)
            {
                return margin.left +
                       margin.right;
            }

            return margin.top +
                   margin.bottom;
        }

        float clampMainSize(
            HTMLElement *element,
            float value,
            bool isRow,
            const CCSize &containingSize)
        {
            if (!element)
            {
                return std::max(
                    0.f,
                    value);
            }

            value =
                std::max(
                    0.f,
                    value);

            const auto &minValue =
                isRow
                    ? element->style.minWidth.get()
                    : element->style.minHeight.get();

            const auto &maxValue =
                isRow
                    ? element->style.maxWidth.get()
                    : element->style.maxHeight.get();

            const float reference =
                isRow
                    ? containingSize.width
                    : containingSize.height;

            if (!minValue.empty())
            {
                value =
                    std::max(
                        value,
                        LengthResolver::resolve(
                            minValue,
                            reference));
            }

            if (!maxValue.empty())
            {
                value =
                    std::min(
                        value,
                        LengthResolver::resolve(
                            maxValue,
                            reference));
            }

            return std::max(
                0.f,
                value);
        }

        float calculateOccupiedMain(
            const std::vector<FlexItem> &items,
            bool isRow,
            float gap)
        {
            float occupied =
                0.f;

            for (const auto &item :
                 items)
            {
                occupied +=
                    getMainSize(
                        item.finalSize,
                        isRow) +
                    getMainMargin(
                        item.margin,
                        isRow);
            }

            if (items.size() > 1)
            {
                occupied +=
                    gap *
                    static_cast<float>(
                        items.size() - 1);
            }

            return occupied;
        }

        void distributeGrow(
            std::vector<FlexItem> &items,
            bool isRow,
            float availableMainSize,
            float gap,
            const CCSize &containingSize)
        {
            constexpr int maxIterations =
                16;

            for (
                int iteration = 0;
                iteration < maxIterations;
                ++iteration)
            {
                const float occupied =
                    calculateOccupiedMain(
                        items,
                        isRow,
                        gap);

                const float freeSpace =
                    availableMainSize -
                    occupied;

                if (freeSpace <= 0.01f)
                {
                    break;
                }

                float totalGrow =
                    0.f;

                for (const auto &item :
                     items)
                {
                    if (
                        item.element &&
                        !item.frozen &&
                        item.grow > 0.f)
                    {
                        totalGrow +=
                            item.grow;
                    }
                }

                if (totalGrow <= 0.f)
                {
                    break;
                }

                bool anyClamped =
                    false;

                for (auto &item :
                     items)
                {
                    if (
                        !item.element ||
                        item.frozen ||
                        item.grow <= 0.f)
                    {
                        continue;
                    }

                    const float current =
                        getMainSize(
                            item.finalSize,
                            isRow);

                    const float share =
                        freeSpace *
                        (item.grow /
                         totalGrow);

                    const float requested =
                        current +
                        share;

                    const float clamped =
                        clampMainSize(
                            item.element,
                            requested,
                            isRow,
                            containingSize);

                    setMainSize(
                        item.finalSize,
                        isRow,
                        clamped);

                    if (
                        std::abs(
                            clamped -
                            requested) >
                        0.01f)
                    {
                        item.frozen =
                            true;

                        anyClamped =
                            true;
                    }
                }

                if (!anyClamped)
                {
                    break;
                }
            }
        }

        void distributeShrink(
            std::vector<FlexItem> &items,
            bool isRow,
            float availableMainSize,
            float gap,
            const CCSize &containingSize)
        {
            constexpr int maxIterations =
                16;

            for (
                int iteration = 0;
                iteration < maxIterations;
                ++iteration)
            {
                const float occupied =
                    calculateOccupiedMain(
                        items,
                        isRow,
                        gap);

                const float overflow =
                    occupied -
                    availableMainSize;

                if (overflow <= 0.01f)
                {
                    break;
                }

                float totalWeightedShrink =
                    0.f;

                for (const auto &item :
                     items)
                {
                    if (
                        !item.element ||
                        item.frozen ||
                        item.shrink <= 0.f)
                    {
                        continue;
                    }

                    const float baseMain =
                        getMainSize(
                            item.baseSize,
                            isRow);

                    totalWeightedShrink +=
                        item.shrink *
                        baseMain;
                }

                if (totalWeightedShrink <= 0.f)
                {
                    break;
                }

                bool anyClamped =
                    false;

                for (auto &item :
                     items)
                {
                    if (
                        !item.element ||
                        item.frozen ||
                        item.shrink <= 0.f)
                    {
                        continue;
                    }

                    const float baseMain =
                        getMainSize(
                            item.baseSize,
                            isRow);

                    const float current =
                        getMainSize(
                            item.finalSize,
                            isRow);

                    const float weightedFactor =
                        item.shrink *
                        baseMain;

                    const float share =
                        overflow *
                        (weightedFactor /
                         totalWeightedShrink);

                    const float requested =
                        current -
                        share;

                    const float clamped =
                        clampMainSize(
                            item.element,
                            requested,
                            isRow,
                            containingSize);

                    setMainSize(
                        item.finalSize,
                        isRow,
                        clamped);

                    if (
                        std::abs(
                            clamped -
                            requested) >
                        0.01f)
                    {
                        item.frozen =
                            true;

                        anyClamped =
                            true;
                    }
                }

                if (!anyClamped)
                {
                    break;
                }
            }
        }

    }

    void FlexLayout::render(
        HTMLElement *element,
        CCNode *node)
    {
        if (
            !element ||
            !node)
        {
            return;
        }

        const auto parentSize =
            element->getContentSize();

        const bool isRow =
            element->style.flexDirection ==
            "row";

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

        const CCSize containingSize{
            innerWidth,
            innerHeight};

        const float availableMainSize =
            isRow
                ? innerWidth
                : innerHeight;

        const float gap =
            std::max(
                0.f,
                LengthResolver::resolve(
                    element->style.gap,
                    availableMainSize));

        std::vector<FlexItem>
            items;

        items.reserve(
            element->m_children.size());

        for (auto *child :
             element->m_children)
        {
            if (
                !child ||
                child->style.display == "none")
            {
                continue;
            }

            const auto resolvedMargin =
                BoxResolver::resolve(
                    child->style.margin,
                    child->style.marginTop,
                    child->style.marginRight,
                    child->style.marginBottom,
                    child->style.marginLeft,
                    innerWidth,
                    innerHeight);

            FlexMargin margin{
                resolvedMargin.top,
                resolvedMargin.right,
                resolvedMargin.bottom,
                resolvedMargin.left};

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

            const auto baseSize =
                child->resolveSize(
                    containingSize,
                    availableSize);

            FlexItem item;

            item.element =
                child;

            item.margin =
                margin;

            item.baseSize =
                baseSize;

            item.finalSize =
                baseSize;

            item.grow =
                std::max(
                    0.f,
                    child->style.flexGrow.get());

            item.shrink =
                std::max(
                    0.f,
                    child->style.flexShrink.get());

            items.push_back(
                item);
        }

        const float initialOccupied =
            calculateOccupiedMain(
                items,
                isRow,
                gap);

        if (
            initialOccupied <
            availableMainSize)
        {
            distributeGrow(
                items,
                isRow,
                availableMainSize,
                gap,
                containingSize);
        }
        else if (
            initialOccupied >
            availableMainSize)
        {
            distributeShrink(
                items,
                isRow,
                availableMainSize,
                gap,
                containingSize);
        }

        const int childCount =
            static_cast<int>(
                items.size());

        const float finalOccupied =
            calculateOccupiedMain(
                items,
                isRow,
                gap);

        const float justifyFreeSpace =
            std::max(
                0.f,
                availableMainSize -
                    finalOccupied);

        float mainOffset =
            0.f;

        float distributedGap =
            gap;

        const auto &justifyContent =
            element->style.justifyContent;

        if (
            justifyContent ==
            "center")
        {
            mainOffset =
                justifyFreeSpace /
                2.f;
        }
        else if (
            justifyContent ==
            "flex-end")
        {
            mainOffset =
                justifyFreeSpace;
        }
        else if (
            justifyContent ==
                "space-between" &&
            childCount > 1)
        {
            distributedGap =
                gap +
                justifyFreeSpace /
                    static_cast<float>(
                        childCount - 1);
        }
        else if (
            justifyContent ==
                "space-around" &&
            childCount > 0)
        {
            const float space =
                justifyFreeSpace /
                static_cast<float>(
                    childCount);

            distributedGap =
                gap +
                space;

            mainOffset =
                space /
                2.f;
        }
        else if (
            justifyContent ==
                "space-evenly" &&
            childCount > 0)
        {
            const float space =
                justifyFreeSpace /
                static_cast<float>(
                    childCount + 1);

            distributedGap =
                gap +
                space;

            mainOffset =
                space;
        }

        float currentMain =
            mainOffset;

        for (
            std::size_t index = 0;
            index < items.size();
            ++index)
        {
            auto &item =
                items[index];

            auto *child =
                item.element;

            if (!child)
            {
                continue;
            }

            child->setResolvedSize(
                item.finalSize);

            float x =
                padding.left;

            float y =
                padding.top;

            if (isRow)
            {
                currentMain +=
                    item.margin.left;

                x +=
                    currentMain;

                if (
                    element->style.alignItems ==
                    "center")
                {
                    y +=
                        (innerHeight -
                         item.finalSize.height) /
                        2.f;
                }
                else if (
                    element->style.alignItems ==
                    "flex-end")
                {
                    y +=
                        innerHeight -
                        item.finalSize.height -
                        item.margin.bottom;
                }
                else
                {
                    y +=
                        item.margin.top;
                }
            }
            else
            {
                currentMain +=
                    item.margin.top;

                y +=
                    currentMain;

                if (
                    element->style.alignItems ==
                    "center")
                {
                    x +=
                        (innerWidth -
                         item.finalSize.width) /
                        2.f;
                }
                else if (
                    element->style.alignItems ==
                    "flex-end")
                {
                    x +=
                        innerWidth -
                        item.finalSize.width -
                        item.margin.right;
                }
                else
                {
                    x +=
                        item.margin.left;
                }
            }

            const CCPoint flowOffset{
                x,
                y};

            child->m_lastFlowOffset =
                flowOffset;

            child->m_hasLastFlowOffset =
                true;

            auto *childNode =
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
                    item.finalSize.width +
                    item.margin.right;
            }
            else
            {
                currentMain +=
                    item.finalSize.height +
                    item.margin.bottom;
            }

            if (
                index + 1 <
                items.size())
            {
                currentMain +=
                    distributedGap;
            }
        }
    }

}