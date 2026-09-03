#pragma once

#include <GDOM/HTMLElement.hpp>

namespace gdom
{

    class RoundedRectNode;

    class HTMLDivElement final : public HTMLElement
    {
    public:
        static HTMLDivElement *create();

    protected:
        CCNode *render(
            const CCSize &parentSize,
            const CCPoint &flowOffset = {
                0.f,
                0.f}) override;

        void applyPaint() override;

    private:
        RoundedRectNode *m_background =
            nullptr;
    };

}