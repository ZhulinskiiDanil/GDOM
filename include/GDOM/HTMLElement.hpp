#pragma once

#include <Geode/Geode.hpp>
#include <GDOM/Style.hpp>

#include <string>
#include <vector>
#include <functional>

using namespace geode::prelude;

namespace gdom
{

    namespace layout
    {
        class BlockLayout;
        class FlexLayout;
    }

    class HTMLElement
    {
    public:
        virtual ~HTMLElement() = default;

        Style style;
        std::string textContent;
        std::string value;
        std::string placeholder;

        void appendChild(
            HTMLElement *child);

        // Handlers
        std::function<void()> onClick;

        std::function<void(
            const std::string &)>
            onInput;

        HTMLElement *getParentElement() const;

        const std::vector<HTMLElement *> &
        getChildren() const;

        CCSize getContentSize() const;

        void setResolvedSize(
            const CCSize &size);

        bool hasResolvedSize() const;

        virtual CCSize resolveSize(
            const CCSize &containingSize,
            const CCSize &availableSize) const;

        virtual CCSize resolveSize(
            const CCSize &parentSize) const;

        float measureAutoHeight(
            const CCSize &parentSize,
            float resolvedWidth) const;

    protected:
        virtual CCNode *render(
            const CCSize &parentSize,
            const CCPoint &flowOffset = {
                0.f,
                0.f}) = 0;

        void renderChildren(
            CCNode *node);

        HTMLElement *m_parentElement =
            nullptr;

        std::vector<HTMLElement *>
            m_children;

        CCSize m_resolvedSize{
            0.f,
            0.f};

        bool m_hasResolvedSize = false;

        friend class GDOMDocument;
        friend class layout::BlockLayout;
        friend class layout::FlexLayout;
    };

}