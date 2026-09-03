#include <GDOM/GDOMDocument.hpp>

#include <GDOM/HTMLDivElement.hpp>
#include <GDOM/HTMLSpanElement.hpp>
#include <GDOM/HTMLButtonElement.hpp>
#include <GDOM/HTMLInputElement.hpp>
#include <GDOM/BoxResolver.hpp>

using namespace geode::prelude;

namespace gdom
{

  GDOMDocument *GDOMDocument::create(
      CCNode *host)
  {
    if (!host)
    {
      return nullptr;
    }

    return new GDOMDocument(host);
  }

  GDOMDocument::GDOMDocument(
      CCNode *host)
      : m_host(host)
  {
  }

  HTMLElement *GDOMDocument::createElement(
      const std::string &tagName)
  {
    if (tagName == "div")
    {
      return HTMLDivElement::create();
    }

    if (tagName == "span")
    {
      return HTMLSpanElement::create();
    }

    if (tagName == "button")
    {
      return HTMLButtonElement::create();
    }

    if (tagName == "input")
    {
      return HTMLInputElement::create();
    }

    log::warn(
        "GDOM: unsupported element <{}>",
        tagName);

    return nullptr;
  }

  void GDOMDocument::appendChild(
      HTMLElement *element)
  {
    if (!element)
    {
      return;
    }

    element->m_parentElement = nullptr;

    m_children.push_back(element);
  }

  void GDOMDocument::render()
  {
    if (!m_host)
    {
      return;
    }

    const auto parentSize =
        m_host->getContentSize();

    float currentY = 0.f;

    for (auto *element : m_children)
    {
      if (!element)
      {
        continue;
      }

      const auto margin =
          BoxResolver::resolve(
              element->style.margin,
              element->style.marginTop,
              element->style.marginRight,
              element->style.marginBottom,
              element->style.marginLeft,
              parentSize.width,
              parentSize.height);

      const CCSize availableSize{
          std::max(
              0.f,
              parentSize.width -
                  margin.left -
                  margin.right),

          std::max(
              0.f,
              parentSize.height -
                  margin.top -
                  margin.bottom)};

      const auto elementSize =
          element->resolveSize(
              parentSize,
              availableSize);

      element->setResolvedSize(
          elementSize);

      currentY +=
          margin.top;

      CCPoint flowOffset{
          margin.left,
          currentY};

      auto node =
          element->render(
              parentSize,
              flowOffset);

      if (node)
      {
        m_host->addChild(node);
      }

      currentY +=
          elementSize.height +
          margin.bottom;
    }
  }

}