#pragma once

#include <GDOM/HTMLElement.hpp>

namespace gdom
{

  class HTMLButtonElement : public HTMLElement
  {
  public:
    static HTMLButtonElement *create();

    CCSize resolveSize(
        const CCSize &containingSize,
        const CCSize &availableSize) const override;

    CCSize resolveSize(
        const CCSize &parentSize) const override;

  protected:
    CCNode *render(
        const CCSize &parentSize,
        const CCPoint &flowOffset = {0.f, 0.f}) override;

  private:
    CCSize measureText() const;
  };

}