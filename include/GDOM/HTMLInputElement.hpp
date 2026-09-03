#pragma once

#include <GDOM/HTMLElement.hpp>

namespace geode
{
    class TextInput;
}

namespace gdom
{

    class RoundedRectNode;

    class HTMLInputElement : public HTMLElement
    {
    public:
        static HTMLInputElement *create();

        CCSize resolveSize(
            const CCSize &containingSize,
            const CCSize &availableSize) const override;

        CCSize resolveSize(
            const CCSize &parentSize) const override;

    protected:
        CCNode *render(
            const CCSize &parentSize,
            const CCPoint &flowOffset = {
                0.f,
                0.f}) override;

        void applyPaint() override;

    private:
        void applyNativeStyle();

        RoundedRectNode *m_background =
            nullptr;

        geode::TextInput *m_input =
            nullptr;
    };

}
