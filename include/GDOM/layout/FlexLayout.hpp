#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace gdom
{

  class HTMLElement;

  namespace layout
  {

    class FlexLayout
    {
    public:
      static void render(
          HTMLElement *element,
          CCNode *node);
    };

  }

}