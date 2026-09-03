#include <GDOM/GDOMDocument.hpp>

#include <GDOM/HTMLDivElement.hpp>
#include <GDOM/HTMLSpanElement.hpp>
#include <GDOM/HTMLButtonElement.hpp>
#include <GDOM/HTMLInputElement.hpp>

#include <GDOM/BoxResolver.hpp>
#include <GDOM/LengthResolver.hpp>

#include <algorithm>

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

    return new GDOMDocument(
        host);
  }

  GDOMDocument::GDOMDocument(
      CCNode *host)
      : m_host(host)
  {
  }

  HTMLElement *GDOMDocument::createElement(
      const std::string &tagName)
  {
    HTMLElement *element =
        nullptr;

    if (tagName == "div")
    {
      element =
          HTMLDivElement::create();
    }
    else if (tagName == "span")
    {
      element =
          HTMLSpanElement::create();
    }
    else if (tagName == "button")
    {
      element =
          HTMLButtonElement::create();
    }
    else if (tagName == "input")
    {
      element =
          HTMLInputElement::create();
    }
    else
    {
      log::warn(
          "GDOM: unsupported element <{}>",
          tagName);

      return nullptr;
    }

    if (element)
    {
      element->setDocument(
          this);
    }

    return element;
  }

  void GDOMDocument::appendChild(
      HTMLElement *element)
  {
    if (!element)
    {
      return;
    }

    if (
        std::find(
            m_children.begin(),
            m_children.end(),
            element) !=
        m_children.end())
    {
      return;
    }

    element->m_parentElement =
        nullptr;

    element->setDocument(
        this);

    m_children.push_back(
        element);

    requestUpdate();
  }

  void GDOMDocument::requestUpdate()
  {
    m_updateRequested =
        true;
  }

  bool GDOMDocument::needsUpdate() const
  {
    return m_updateRequested;
  }

  void GDOMDocument::render()
  {
    if (!m_host)
    {
      return;
    }

    removeRenderedRoots();

    for (auto *element :
         m_children)
    {
      if (!element)
      {
        continue;
      }

      element
          ->resetResolvedSizeRecursive();
    }

    renderRoots();

    m_updateRequested =
        false;
  }

  void GDOMDocument::update()
  {
    if (!m_host)
    {
      return;
    }

    if (!m_updateRequested)
    {
      return;
    }

    std::vector<HTMLElement *>
        dirtyElements;

    for (auto *element :
         m_children)
    {
      collectLayoutDirty(
          element,
          dirtyElements);
    }

    if (dirtyElements.empty())
    {
      for (auto *element :
           m_children)
      {
        applyPaintRecursive(
            element);
      }

      m_updateRequested =
          false;

      return;
    }

    std::vector<HTMLElement *>
        boundaries;

    for (auto *element :
         dirtyElements)
    {
      auto boundary =
          findLayoutBoundary(
              element);

      addBoundary(
          boundaries,
          boundary);
    }

    bool requiresRootRender =
        false;

    for (auto *boundary :
         boundaries)
    {
      if (
          boundary &&
          !boundary->getParentElement())
      {
        requiresRootRender =
            true;

        break;
      }
    }

    if (requiresRootRender)
    {
      removeRenderedRoots();

      for (auto *element :
           m_children)
      {
        if (!element)
        {
          continue;
        }

        element
            ->resetResolvedSizeRecursive();
      }

      renderRoots();

      m_updateRequested =
          false;

      return;
    }

    for (auto *boundary :
         boundaries)
    {
      if (!boundary)
      {
        continue;
      }

      if (!rerenderBoundary(
              boundary))
      {
        removeRenderedRoots();

        for (auto *element :
             m_children)
        {
          if (!element)
          {
            continue;
          }

          element
              ->resetResolvedSizeRecursive();
        }

        renderRoots();

        m_updateRequested =
            false;

        return;
      }
    }

    for (auto *element :
         m_children)
    {
      applyPaintRecursive(
          element);

      clearDescendantLayoutRecursive(
          element);
    }

    m_updateRequested =
        false;
  }

  void GDOMDocument::collectLayoutDirty(
      HTMLElement *element,
      std::vector<HTMLElement *> &elements)
  {
    if (!element)
    {
      return;
    }

    if (
        element->isLayoutDirty() ||
        element->isTreeDirty())
    {
      elements.push_back(
          element);
    }

    for (auto *child :
         element->getChildren())
    {
      collectLayoutDirty(
          child,
          elements);
    }
  }

  HTMLElement *
  GDOMDocument::findLayoutBoundary(
      HTMLElement *element) const
  {
    if (!element)
    {
      return nullptr;
    }

    auto boundary =
        element;

    if (boundary->m_parentElement)
    {
      boundary =
          boundary->m_parentElement;
    }

    bool changed =
        true;

    while (
        changed &&
        boundary &&
        boundary->m_parentElement)
    {
      changed =
          false;

      if (
          LengthResolver::isAuto(
              boundary->style.width) ||
          LengthResolver::isAuto(
              boundary->style.height))
      {
        boundary =
            boundary->m_parentElement;

        changed =
            true;

        continue;
      }

      HTMLElement *highestOverflow =
          nullptr;

      for (
          auto *ancestor =
              boundary->m_parentElement;
          ancestor;
          ancestor =
              ancestor->m_parentElement)
      {
        if (
            ancestor->style.overflow == "auto" ||
            ancestor->style.overflow == "scroll")
        {
          highestOverflow =
              ancestor;
        }
      }

      if (highestOverflow)
      {
        boundary =
            highestOverflow;

        changed =
            true;
      }
    }

    return boundary;
  }

  bool GDOMDocument::rerenderBoundary(
      HTMLElement *element)
  {
    if (!element)
    {
      return false;
    }

    auto *parentElement =
        element->m_parentElement;

    if (!parentElement)
    {
      return false;
    }

    auto *oldNode =
        element->getRenderedNode();

    if (
        !oldNode ||
        !oldNode->getParent())
    {
      return false;
    }

    auto *cocosParent =
        oldNode->getParent();

    const auto oldPosition =
        oldNode->getPosition();

    const int oldZOrder =
        oldNode->getZOrder();

    const auto parentSize =
        parentElement->getContentSize();

    const auto parentPadding =
        BoxResolver::resolve(
            parentElement->style.padding,
            parentElement->style.paddingTop,
            parentElement->style.paddingRight,
            parentElement->style.paddingBottom,
            parentElement->style.paddingLeft,
            parentSize.width,
            parentSize.height);

    const CCSize containingSize{
        std::max(
            0.f,
            parentSize.width -
                parentPadding.left -
                parentPadding.right),

        std::max(
            0.f,
            parentSize.height -
                parentPadding.top -
                parentPadding.bottom)};

    const auto margin =
        BoxResolver::resolve(
            element->style.margin,
            element->style.marginTop,
            element->style.marginRight,
            element->style.marginBottom,
            element->style.marginLeft,
            containingSize.width,
            containingSize.height);

    const CCSize availableSize{
        std::max(
            0.f,
            containingSize.width -
                margin.left -
                margin.right),

        std::max(
            0.f,
            containingSize.height -
                margin.top -
                margin.bottom)};

    const float left =
        LengthResolver::resolve(
            element->style.left,
            parentSize.width);

    const float top =
        LengthResolver::resolve(
            element->style.top,
            parentSize.height);

    const CCPoint flowOffset{
        oldPosition.x -
            left,

        parentSize.height -
            oldPosition.y -
            top};

    oldNode
        ->removeFromParentAndCleanup(
            true);

    element
        ->resetResolvedSizeRecursive();

    const auto elementSize =
        element->resolveSize(
            containingSize,
            availableSize);

    element->setResolvedSize(
        elementSize);

    auto *newNode =
        element->render(
            parentSize,
            flowOffset);

    if (!newNode)
    {
      return false;
    }

    cocosParent->addChild(
        newNode,
        oldZOrder);

    return true;
  }

  bool GDOMDocument::isAncestorOf(
      HTMLElement *ancestor,
      HTMLElement *element) const
  {
    if (
        !ancestor ||
        !element)
    {
      return false;
    }

    auto *current =
        element;

    while (current)
    {
      if (current == ancestor)
      {
        return true;
      }

      current =
          current->m_parentElement;
    }

    return false;
  }

  void GDOMDocument::addBoundary(
      std::vector<HTMLElement *> &boundaries,
      HTMLElement *boundary) const
  {
    if (!boundary)
    {
      return;
    }

    for (auto *existing :
         boundaries)
    {
      if (
          isAncestorOf(
              existing,
              boundary))
      {
        return;
      }
    }

    boundaries.erase(
        std::remove_if(
            boundaries.begin(),
            boundaries.end(),
            [this, boundary](
                HTMLElement *existing)
            {
              return isAncestorOf(
                  boundary,
                  existing);
            }),
        boundaries.end());

    boundaries.push_back(
        boundary);
  }

  void GDOMDocument::applyPaintRecursive(
      HTMLElement *element)
  {
    if (!element)
    {
      return;
    }

    element->updatePaint();

    for (auto *child :
         element->getChildren())
    {
      applyPaintRecursive(
          child);
    }
  }

  void GDOMDocument::clearDescendantLayoutRecursive(
      HTMLElement *element)
  {
    if (!element)
    {
      return;
    }

    element->clearDirty(
        DirtyFlags::DescendantLayout);

    for (auto *child :
         element->getChildren())
    {
      clearDescendantLayoutRecursive(
          child);
    }
  }

  void GDOMDocument::removeRenderedRoots()
  {
    for (auto *element :
         m_children)
    {
      if (!element)
      {
        continue;
      }

      auto *node =
          element->getRenderedNode();

      if (
          node &&
          node->getParent())
      {
        node
            ->removeFromParentAndCleanup(
                true);
      }
    }
  }

  void GDOMDocument::renderRoots()
  {
    if (!m_host)
    {
      return;
    }

    const auto parentSize =
        m_host->getContentSize();

    float currentY =
        0.f;

    for (auto *element :
         m_children)
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

      const CCPoint flowOffset{
          margin.left,
          currentY};

      auto *node =
          element->render(
              parentSize,
              flowOffset);

      if (node)
      {
        m_host->addChild(
            node);
      }

      currentY +=
          elementSize.height +
          margin.bottom;
    }
  }

}