#pragma once

#include <GDOM/HTMLElement.hpp>

namespace gdom
{

    class HTMLDivElement final : public HTMLElement
    {
    public:
        static HTMLDivElement *create();

    protected:
        CCNode *render(
            const CCSize &parentSize,
            const CCPoint &flowOffset = {0.f, 0.f}) override;
    };

}