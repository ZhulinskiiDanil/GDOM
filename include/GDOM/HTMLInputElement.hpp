#pragma once

#include "HTMLElement.hpp"

#include <Geode/binding/TextInputDelegate.hpp>

namespace geode
{
    class TextInput;
}

class CCTextInputNode;

namespace gdom
{

    class RoundedRectNode;

    class HTMLInputElement :
        public HTMLElement,
        public TextInputDelegate
    {
    public:
        static HTMLInputElement *create();

        CCSize resolveSize(
            const CCSize &containingSize,
            const CCSize &availableSize) const override;

        CCSize resolveSize(
            const CCSize &parentSize) const override;

        void textChanged(
            CCTextInputNode *input) override;

        void textInputOpened(
            CCTextInputNode *input) override;

        void textInputClosed(
            CCTextInputNode *input) override;

    protected:
        CCNode *render(
            const CCSize &parentSize,
            const CCPoint &flowOffset = {
                0.f,
                0.f}) override;

        void applyPaint() override;

        bool isFocusable() const override;

        void focusNative() override;

        void blurNative() override;

    private:
        void applyNativeStyle();

        RoundedRectNode *m_background =
            nullptr;

        geode::TextInput *m_input =
            nullptr;
    };

}
