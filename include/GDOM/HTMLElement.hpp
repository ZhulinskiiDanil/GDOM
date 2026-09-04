#pragma once

#include <Geode/Geode.hpp>

#include "Export.hpp"

#include "Style.hpp"
#include "DirtyFlags.hpp"
#include "ReactiveProperty.hpp"

#include <functional>
#include <string>
#include <vector>

using namespace geode::prelude;

namespace gdom
{

    class GDOMDocument;

    class GDOM_API HTMLElement;

    namespace layout
    {
        class BlockLayout;
        class FlexLayout;
    }

    class HTMLElement
    {
    public:
        HTMLElement();

        virtual ~HTMLElement() = default;

        Style style;

        ReactiveProperty<std::string>
            textContent{
                "",
                DirtyFlags::Layout};

        ReactiveProperty<std::string>
            value{
                "",
                DirtyFlags::Paint};

        ReactiveProperty<std::string>
            placeholder{
                "",
                DirtyFlags::Paint};

        std::function<void()>
            onClick;

        std::function<void(
            const std::string &)>
            onInput;

        std::function<void()>
            onFocus;

        std::function<void()>
            onBlur;

        void appendChild(
            HTMLElement *child);

        bool removeChild(
            HTMLElement *child);

        bool replaceChild(
            HTMLElement *newChild,
            HTMLElement *oldChild);

        bool focus();

        void blur();

        bool isFocused() const;

        HTMLElement *
        getParentElement() const;

        const std::vector<HTMLElement *> &
        getChildren() const;

        void setDocument(
            GDOMDocument *document);

        GDOMDocument *
        getDocument() const;

        void invalidate(
            DirtyFlags flags);

        void invalidatePaint();

        void invalidateLayout();

        void invalidateTree();

        DirtyFlags
        getDirtyFlags() const;

        bool isDirty() const;

        bool isPaintDirty() const;

        bool isLayoutDirty() const;

        bool isTreeDirty() const;

        void clearDirty();

        void clearDirty(
            DirtyFlags flags);

        void updatePaint();

        CCNode *
        getRenderedNode() const;

        bool isMounted() const;

        CCSize getContentSize() const;

        void setResolvedSize(
            const CCSize &size);

        bool hasResolvedSize() const;

        void resetResolvedSizeRecursive();

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

        virtual void applyPaint();

        virtual bool isFocusable() const;

        virtual void focusNative();

        virtual void blurNative();

        void notifyNativeFocus();

        void notifyNativeBlur();

        void renderChildren(
            CCNode *node);

        void setRenderedNode(
            CCNode *node);

        CCNode *finishRender(
            CCNode *node);

        void clearRenderedStateRecursive();

        bool wouldCreateCycle(
            HTMLElement *child) const;

        HTMLElement *
            m_parentElement =
                nullptr;

        std::vector<HTMLElement *>
            m_children;

        CCSize m_resolvedSize{
            0.f,
            0.f};

        bool m_hasResolvedSize =
            false;

        CCPoint m_lastFlowOffset{
            0.f,
            0.f};

        bool m_hasLastFlowOffset =
            false;

        GDOMDocument *
            m_document =
                nullptr;

        CCNode *
            m_renderedNode =
                nullptr;

        DirtyFlags m_dirtyFlags =
            DirtyFlags::Layout;

        bool m_mounted =
            false;

        friend class GDOMDocument;
        friend class layout::BlockLayout;
        friend class layout::FlexLayout;
    };

}
