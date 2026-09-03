#pragma once

#include <Geode/Geode.hpp>

#include <GDOM/HTMLElement.hpp>

#include <string>
#include <vector>

using namespace geode::prelude;

namespace gdom
{

    class GDOMDocument
    {
    public:
        static GDOMDocument *create(
            CCNode *host);

        HTMLElement *createElement(
            const std::string &tagName);

        void appendChild(
            HTMLElement *element);

        bool removeChild(
            HTMLElement *element);

        bool replaceChild(
            HTMLElement *newElement,
            HTMLElement *oldElement);

        void render();

        void update();

        void requestUpdate();

        bool needsUpdate() const;

    private:
        explicit GDOMDocument(
            CCNode *host);

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

        CCNode *m_host =
            nullptr;

        std::vector<HTMLElement *>
            m_children;

        bool m_updateRequested =
            false;
    };

}
