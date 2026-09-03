#pragma once

#include <Geode/Geode.hpp>

#include <GDOM/HTMLElement.hpp>

#include <memory>
#include <string>
#include <vector>

using namespace geode::prelude;

namespace gdom
{

    class GDOMDocumentLifetimeNode;

    class GDOMDocument
    {
    public:
        static GDOMDocument *create(
            CCNode *host);

        ~GDOMDocument();

        HTMLElement *createElement(
            const std::string &tagName);

        void appendChild(
            HTMLElement *element);

        bool removeChild(
            HTMLElement *element);

        bool replaceChild(
            HTMLElement *newElement,
            HTMLElement *oldElement);

        HTMLElement *
        getFocusedElement() const;

        void render();

        void update();

        void requestUpdate();

        bool needsUpdate() const;

    private:
        explicit GDOMDocument(
            CCNode *host);

        bool attachLifetimeNode();

        void handleHostDestroyed(
            GDOMDocumentLifetimeNode *node);

        template <typename T>
        T *ownElement(
            T *element)
        {
            if (!element)
            {
                return nullptr;
            }

            auto owned =
                std::unique_ptr<HTMLElement>(
                    element);

            auto *raw =
                element;

            m_ownedElements.push_back(
                std::move(
                    owned));

            raw->setDocument(
                this);

            return raw;
        }

        bool ownsElement(
            const HTMLElement *element) const;

        bool focusElement(
            HTMLElement *element,
            bool nativeAlreadyFocused);

        void blurElement(
            HTMLElement *element,
            bool nativeAlreadyBlurred);

        void renderRoots();

        void removeRenderedRoots();

        void applyPaintRecursive(
            HTMLElement *element);

        void collectLayoutDirty(
            HTMLElement *element,
            std::vector<HTMLElement *> &elements);

        HTMLElement *findLayoutBoundary(
            HTMLElement *element) const;

        bool rerenderBoundary(
            HTMLElement *element);

        bool isAncestorOf(
            HTMLElement *ancestor,
            HTMLElement *element) const;

        void addBoundary(
            std::vector<HTMLElement *> &boundaries,
            HTMLElement *boundary) const;

        void clearDescendantLayoutRecursive(
            HTMLElement *element);

        //
        // Real render host supplied by the user.
        // GDOM roots are rendered directly into this node.
        //
        CCNode *m_host =
            nullptr;

        //
        // Invisible child used only to observe host destruction.
        // It is NOT a render parent.
        //
        GDOMDocumentLifetimeNode *
            m_lifetimeNode =
                nullptr;

        std::vector<HTMLElement *>
            m_children;

        std::vector<
            std::unique_ptr<HTMLElement>>
            m_ownedElements;

        HTMLElement *
            m_focusedElement =
                nullptr;

        bool m_updateRequested =
            false;

        bool m_hostDestroyed =
            false;

        bool m_deleteQueued =
            false;

        friend class HTMLElement;
        friend class GDOMDocumentLifetimeNode;
    };

}
