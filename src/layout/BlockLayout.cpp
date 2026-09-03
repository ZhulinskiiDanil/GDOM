#include <GDOM/layout/BlockLayout.hpp>

#include <GDOM/HTMLElement.hpp>
#include <GDOM/LengthResolver.hpp>
#include <GDOM/BoxResolver.hpp>

#include <algorithm>

using namespace geode::prelude;

namespace gdom::layout
{

    void BlockLayout::render(
        HTMLElement *element,
        CCNode *node)
    {
        if (!element || !node)
        {
            return;
        }

        const auto parentSize =
            element->getContentSize();

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
                parentSize.height);

        float currentY =
            padding.top;

        bool firstChild = true;

        for (auto *child : element->m_children)
        {
            if (!child)
            {
                continue;
            }

            if (!firstChild)
            {
                currentY += gap;
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

            const CCSize containingSize{
                innerWidth,
                innerHeight};

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

            currentY +=
                margin.top;

            CCPoint flowOffset{
                padding.left +
                    margin.left,

                currentY};

            auto childNode =
                child->render(
                    parentSize,
                    flowOffset);

            if (childNode)
            {
                node->addChild(
                    childNode);
            }

            if (
                child->style.display == "block" ||
                child->style.display == "flex")
            {
                currentY +=
                    childSize.height +
                    margin.bottom;
            }

            firstChild = false;
        }
    }

}