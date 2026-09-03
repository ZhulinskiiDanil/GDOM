#pragma once

#include <GDOM/HTMLElement.hpp>

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
    RoundedRectNode *m_background =
        nullptr;

    CCLabelBMFont *m_label =
        nullptr;
  };

}