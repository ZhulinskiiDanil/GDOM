#pragma once

#include <GDOM/HTMLElement.hpp>

#include <string>

namespace gdom
{

  class HTMLSpanElement : public HTMLElement
  {
  public:
    static HTMLSpanElement *create();

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
    CCSize measureText() const;

    CCLabelBMFont *m_label =
        nullptr;
  };

}