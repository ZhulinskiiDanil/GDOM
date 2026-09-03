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

        void render();

    private:
        explicit GDOMDocument(
            CCNode *host);

        CCNode *m_host = nullptr;

        std::vector<HTMLElement *>
            m_children;
    };

}