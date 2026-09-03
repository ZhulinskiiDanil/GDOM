#include <algorithm>

#include <GDOM/HTMLElement.hpp>
#include <GDOM/GDOMDocument.hpp>

#include <GDOM/LengthResolver.hpp>
#include <GDOM/BoxResolver.hpp>

#include <GDOM/layout/BlockLayout.hpp>
#include <GDOM/layout/FlexLayout.hpp>

using namespace geode::prelude;

namespace gdom
{

    HTMLElement::HTMLElement()
    {
        auto invalidateCallback =
            [this](DirtyFlags flags)
        {
            invalidate(flags);
        };

        style.bind(
            invalidateCallback);

        textContent.bind(
            invalidateCallback);

        value.bind(
            invalidateCallback);

        placeholder.bind(
            invalidateCallback);
    }

    bool HTMLElement::wouldCreateCycle(
        HTMLElement *child) const
    {
        if (!child)
        {
            return false;
        }

        for (
            auto *current =
                const_cast<HTMLElement *>(this);
            current;
            current =
                current->m_parentElement)
        {
            if (current == child)
            {
                return true;
            }
        }

        return false;
    }

    void HTMLElement::clearRenderedStateRecursive()
    {
        m_renderedNode =
            nullptr;

        m_mounted =
            false;

        for (auto *child :
             m_children)
        {
            if (child)
            {
                child
                    ->clearRenderedStateRecursive();
            }
        }
    }

    void HTMLElement::appendChild(
        HTMLElement *child)
    {
        if (!child)
        {
            return;
        }

        if (
            child == this ||
            wouldCreateCycle(child))
        {
            log::warn(
                "GDOM: appendChild would create a cycle");

            return;
        }

        if (
            std::find(
                m_children.begin(),
                m_children.end(),
                child) !=
            m_children.end())
        {
            return;
        }

        if (child->m_parentElement)
        {
            child->m_parentElement
                ->removeChild(
                    child);
        }
        else if (
            child->m_document &&
            child->m_document == m_document)
        {
            child->m_document
                ->removeChild(
                    child);
        }
        else if (
            child->m_document &&
            child->m_document != m_document)
        {
            log::warn(
                "GDOM: cannot move an element "
                "between different documents");

            return;
        }

        child->m_parentElement =
            this;

        child->setDocument(
            m_document);

        m_children.push_back(
            child);

        invalidateTree();
    }

    bool HTMLElement::removeChild(
        HTMLElement *child)
    {
        if (!child)
        {
            return false;
        }

        const auto iterator =
            std::find(
                m_children.begin(),
                m_children.end(),
                child);

        if (
            iterator ==
            m_children.end())
        {
            return false;
        }

        if (m_document)
        {
            auto *focused =
                m_document
                    ->getFocusedElement();

            for (
                auto *current =
                    focused;
                current;
                current =
                    current->m_parentElement)
            {
                if (current == child)
                {
                    focused->blur();
                    break;
                }
            }
        }

        auto *renderedNode =
            child->getRenderedNode();

        if (
            renderedNode &&
            renderedNode->getParent())
        {
            renderedNode
                ->removeFromParentAndCleanup(
                    true);
        }

        m_children.erase(
            iterator);

        child->m_parentElement =
            nullptr;

        //
        // Keep ownership document after detach.
        //
        child->setDocument(
            m_document);

        child
            ->clearRenderedStateRecursive();

        child
            ->resetResolvedSizeRecursive();

        child->m_lastFlowOffset =
            CCPoint(
                0.f,
                0.f);

        child->m_hasLastFlowOffset =
            false;

        invalidateTree();

        return true;
    }

    bool HTMLElement::replaceChild(
        HTMLElement *newChild,
        HTMLElement *oldChild)
    {
        if (
            !newChild ||
            !oldChild)
        {
            return false;
        }

        if (newChild == oldChild)
        {
            return std::find(
                       m_children.begin(),
                       m_children.end(),
                       oldChild) !=
                   m_children.end();
        }

        if (
            newChild == this ||
            wouldCreateCycle(
                newChild))
        {
            log::warn(
                "GDOM: replaceChild would create a cycle");

            return false;
        }

        auto oldIterator =
            std::find(
                m_children.begin(),
                m_children.end(),
                oldChild);

        if (
            oldIterator ==
            m_children.end())
        {
            return false;
        }

        if (newChild->m_parentElement)
        {
            newChild->m_parentElement
                ->removeChild(
                    newChild);
        }
        else if (
            newChild->m_document &&
            newChild->m_document == m_document)
        {
            newChild->m_document
                ->removeChild(
                    newChild);
        }
        else if (
            newChild->m_document &&
            newChild->m_document != m_document)
        {
            log::warn(
                "GDOM: cannot replace with an element "
                "owned by another document");

            return false;
        }

        oldIterator =
            std::find(
                m_children.begin(),
                m_children.end(),
                oldChild);

        if (
            oldIterator ==
            m_children.end())
        {
            return false;
        }

        if (m_document)
        {
            auto *focused =
                m_document
                    ->getFocusedElement();

            for (
                auto *current =
                    focused;
                current;
                current =
                    current->m_parentElement)
            {
                if (current == oldChild)
                {
                    focused->blur();
                    break;
                }
            }
        }

        auto *oldRenderedNode =
            oldChild->getRenderedNode();

        if (
            oldRenderedNode &&
            oldRenderedNode->getParent())
        {
            oldRenderedNode
                ->removeFromParentAndCleanup(
                    true);
        }

        oldChild->m_parentElement =
            nullptr;

        oldChild->setDocument(
            m_document);

        oldChild
            ->clearRenderedStateRecursive();

        oldChild
            ->resetResolvedSizeRecursive();

        oldChild->m_lastFlowOffset =
            CCPoint(
                0.f,
                0.f);

        oldChild->m_hasLastFlowOffset =
            false;

        newChild->m_parentElement =
            this;

        newChild->setDocument(
            m_document);

        *oldIterator =
            newChild;

        invalidateTree();

        return true;
    }

    bool HTMLElement::focus()
    {
        if (
            !m_document ||
            !isFocusable())
        {
            return false;
        }

        return m_document
            ->focusElement(
                this,
                false);
    }

    void HTMLElement::blur()
    {
        if (!m_document)
        {
            return;
        }

        m_document
            ->blurElement(
                this,
                false);
    }

    bool HTMLElement::isFocused() const
    {
        return m_document &&
               m_document
                       ->getFocusedElement() ==
                   this;
    }

    bool HTMLElement::isFocusable() const
    {
        return false;
    }

    void HTMLElement::focusNative()
    {
    }

    void HTMLElement::blurNative()
    {
    }

    void HTMLElement::notifyNativeFocus()
    {
        if (!m_document)
        {
            return;
        }

        m_document
            ->focusElement(
                this,
                true);
    }

    void HTMLElement::notifyNativeBlur()
    {
        if (!m_document)
        {
            return;
        }

        m_document
            ->blurElement(
                this,
                true);
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

    void HTMLElement::setDocument(
        GDOMDocument *document)
    {
        m_document =
            document;

        for (auto *child :
             m_children)
        {
            if (child)
            {
                child->setDocument(
                    document);
            }
        }
    }

    GDOMDocument *
    HTMLElement::getDocument() const
    {
        return m_document;
    }

    void HTMLElement::invalidate(
        DirtyFlags flags)
    {
        if (
            flags ==
            DirtyFlags::None)
        {
            return;
        }

        m_dirtyFlags |=
            flags;

        const bool affectsLayout =
            hasFlag(
                flags,
                DirtyFlags::Layout) ||
            hasFlag(
                flags,
                DirtyFlags::Tree) ||
            hasFlag(
                flags,
                DirtyFlags::DescendantLayout);

        if (affectsLayout)
        {
            m_hasResolvedSize =
                false;

            if (m_parentElement)
            {
                m_parentElement
                    ->invalidate(
                        DirtyFlags::DescendantLayout);
            }
        }

        if (m_document)
        {
            m_document
                ->requestUpdate();
        }
    }

    void HTMLElement::invalidatePaint()
    {
        invalidate(
            DirtyFlags::Paint);
    }

    void HTMLElement::invalidateLayout()
    {
        invalidate(
            DirtyFlags::Layout);
    }

    void HTMLElement::invalidateTree()
    {
        invalidate(
            DirtyFlags::Tree |
            DirtyFlags::Layout);
    }

    DirtyFlags
    HTMLElement::getDirtyFlags() const
    {
        return m_dirtyFlags;
    }

    bool HTMLElement::isDirty() const
    {
        return m_dirtyFlags !=
               DirtyFlags::None;
    }

    bool HTMLElement::isPaintDirty() const
    {
        return hasFlag(
            m_dirtyFlags,
            DirtyFlags::Paint);
    }

    bool HTMLElement::isLayoutDirty() const
    {
        return hasFlag(
            m_dirtyFlags,
            DirtyFlags::Layout);
    }

    bool HTMLElement::isTreeDirty() const
    {
        return hasFlag(
            m_dirtyFlags,
            DirtyFlags::Tree);
    }

    void HTMLElement::clearDirty()
    {
        m_dirtyFlags =
            DirtyFlags::None;
    }

    void HTMLElement::clearDirty(
        DirtyFlags flags)
    {
        const auto current =
            static_cast<uint8_t>(
                m_dirtyFlags);

        const auto remove =
            static_cast<uint8_t>(
                flags);

        m_dirtyFlags =
            static_cast<DirtyFlags>(
                current & ~remove);
    }

    void HTMLElement::updatePaint()
    {
        if (!isPaintDirty())
        {
            return;
        }

        applyPaint();

        clearDirty(
            DirtyFlags::Paint);
    }

    void HTMLElement::applyPaint()
    {
    }

    void HTMLElement::setRenderedNode(
        CCNode *node)
    {
        m_renderedNode =
            node;

        m_mounted =
            node != nullptr;
    }

    CCNode *
    HTMLElement::getRenderedNode() const
    {
        return m_renderedNode;
    }

    bool HTMLElement::isMounted() const
    {
        return m_mounted;
    }

    CCNode *
    HTMLElement::finishRender(
        CCNode *node)
    {
        setRenderedNode(
            node);

        clearDirty();

        return node;
    }

    CCSize
    HTMLElement::getContentSize() const
    {
        return m_resolvedSize;
    }

    void HTMLElement::setResolvedSize(
        const CCSize &size)
    {
        m_resolvedSize =
            size;

        m_hasResolvedSize =
            true;
    }

    bool HTMLElement::hasResolvedSize() const
    {
        return m_hasResolvedSize;
    }

    void HTMLElement::resetResolvedSizeRecursive()
    {
        m_resolvedSize =
            CCSize(
                0.f,
                0.f);

        m_hasResolvedSize =
            false;

        for (auto *child :
             m_children)
        {
            if (child)
            {
                child
                    ->resetResolvedSizeRecursive();
            }
        }
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
        if (style.display == "none")
        {
            return {
                0.f,
                0.f};
        }

        float width =
            0.f;

        if (
            LengthResolver::isAuto(
                style.width))
        {
            width =
                availableSize.width;
        }
        else
        {
            width =
                LengthResolver::resolve(
                    style.width,
                    containingSize.width);
        }

        if (!style.minWidth.get().empty())
        {
            width =
                std::max(
                    width,
                    LengthResolver::resolve(
                        style.minWidth,
                        containingSize.width));
        }

        if (!style.maxWidth.get().empty())
        {
            width =
                std::min(
                    width,
                    LengthResolver::resolve(
                        style.maxWidth,
                        containingSize.width));
        }

        width =
            std::max(
                0.f,
                width);

        float height =
            0.f;

        if (
            LengthResolver::isAuto(
                style.height))
        {
            height =
                measureAutoHeight(
                    {width,
                     availableSize.height},
                    width);
        }
        else
        {
            height =
                LengthResolver::resolve(
                    style.height,
                    containingSize.height);
        }

        if (!style.minHeight.get().empty())
        {
            height =
                std::max(
                    height,
                    LengthResolver::resolve(
                        style.minHeight,
                        containingSize.height));
        }

        if (!style.maxHeight.get().empty())
        {
            height =
                std::min(
                    height,
                    LengthResolver::resolve(
                        style.maxHeight,
                        containingSize.height));
        }

        height =
            std::max(
                0.f,
                height);

        return {
            width,
            height};
    }

    float HTMLElement::measureAutoHeight(
        const CCSize &parentSize,
        float resolvedWidth) const
    {
        if (style.display == "none")
        {
            return 0.f;
        }

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

            for (auto *child :
                 m_children)
            {
                if (
                    !child ||
                    child->style.display == "none")
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

        for (auto *child :
             m_children)
        {
            if (
                !child ||
                child->style.display == "none")
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

            ++childCount;
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
        if (
            !node ||
            style.display == "none")
        {
            return;
        }

        if (
            style.display ==
            "flex")
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