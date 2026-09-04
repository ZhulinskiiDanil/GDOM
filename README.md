# GDOM

A minimal DOM-like UI layer for Geometry Dash mods built with [Geode](https://geode-sdk.org/).

GDOM provides a small HTML/CSS-inspired API on top of cocos2d, with DOM-style elements, reactive styles, block and flex layouts, inputs, focus handling, mutations, and partial re-rendering.

> GDOM is intended for mod developers. It is not a browser DOM implementation and does not aim to implement the full CSS specification.

## Status

GDOM is currently in early development.

Target environment:

- Geode `5.x`
- Geometry Dash `2.2081`
- C++23

## Features

- DOM-like document and element tree
- `div`, `span`, `button`, and `input` elements
- Reactive style properties
- Block layout
- Flex row / column layout
- `flexGrow` and `flexShrink`
- `justifyContent`
- `alignItems`
- `gap`
- Margin and padding
- Width / height
- Min / max sizes
- `px`, `%`, `rem`, and `calc(...)`
- Background, border, radius, text color and font size
- `display: none`
- Vertical `overflow: auto` / `scroll`
- DOM mutations: `appendChild`, `removeChild`, `replaceChild`, and reparenting
- Button click events
- Input events
- Native Geode `TextInput`
- `focus()` / `blur()`
- `onFocus` / `onBlur`
- Partial layout re-rendering
- Document-owned element lifetime management

## Basic Example

```cpp
#include <GDOM/GDOMDocument.hpp>

auto document =
    gdom::GDOMDocument::create(this);

auto root =
    document->createElement("div");

root->style.left = "40px";
root->style.top = "40px";
root->style.width = "400px";
root->style.height = "220px";

root->style.display = "flex";
root->style.flexDirection = "column";
root->style.gap = "10px";
root->style.padding = "16px";

root->style.backgroundColor = {
    30,
    34,
    46,
    255
};

root->style.borderRadius = "8px";

auto title =
    document->createElement("span");

title->textContent =
    "Hello from GDOM";

title->style.width = "auto";
title->style.height = "auto";
title->style.fontSize = "14px";
title->style.flexShrink = 0.f;

auto button =
    document->createElement("button");

button->textContent =
    "Click me";

button->style.width = "120px";
button->style.height = "36px";
button->style.flexShrink = 0.f;

button->onClick =
    []()
{
    geode::log::info(
        "GDOM button clicked");
};

root->appendChild(title);
root->appendChild(button);

document->appendChild(root);
document->render();
```

## Creating Elements

Elements are created through `GDOMDocument`:

```cpp
auto div =
    document->createElement("div");

auto span =
    document->createElement("span");

auto button =
    document->createElement("button");

auto input =
    document->createElement("input");
```

The document owns created elements for its lifetime.

The public API uses raw pointers for convenience, but callers should not manually delete individual `HTMLElement` instances.

## Styling

Styles are assigned directly:

```cpp
element->style.width = "100%";
element->style.height = "48px";

element->style.margin = "8px";
element->style.padding = "12px";

element->style.backgroundColor = {
    40,
    45,
    60,
    255
};

element->style.borderColor = {
    255,
    255,
    255,
    80
};

element->style.borderWidth = "1px";
element->style.borderRadius = "6px";
```

Most style changes are reactive.

After changing styles at runtime, call:

```cpp
document->update();
```

GDOM will apply paint-only updates when possible and perform layout re-rendering when required.

## Lengths

Supported length syntax includes:

```cpp
element->style.width = "120px";
element->style.width = "50%";
element->style.width = "2rem";

element->style.width =
    "calc(100% - 20px)";
```

`calc(...)` currently focuses on addition and subtraction of supported length values. Full CSS math is not implemented.

## Flex Layout

```cpp
container->style.display = "flex";
container->style.flexDirection = "row";
container->style.justifyContent = "space-between";
container->style.alignItems = "center";
container->style.gap = "8px";
```

Children may use:

```cpp
child->style.flexGrow = 1.f;
child->style.flexShrink = 0.f;
```

## Input

GDOM inputs use Geode's native `TextInput` internally.

```cpp
auto input =
    document->createElement("input");

input->placeholder =
    "Enter text";

input->style.width = "260px";
input->style.height = "36px";

input->onInput =
    [](
        const std::string& value)
{
    geode::log::info(
        "Input: {}",
        value);
};
```

Programmatic focus:

```cpp
input->focus();
input->blur();

if (input->isFocused())
{
    // ...
}
```

Focus events:

```cpp
input->onFocus =
    []()
{
    geode::log::info("Focused");
};

input->onBlur =
    []()
{
    geode::log::info("Blurred");
};
```

The currently focused element can be queried with:

```cpp
auto focused =
    document->getFocusedElement();
```

## DOM Mutations

```cpp
parent->appendChild(child);

parent->removeChild(child);

parent->replaceChild(
    newChild,
    oldChild);
```

Appending an existing child to another parent reparents it automatically.

Moving elements between different `GDOMDocument` instances is not supported.

## `display: none`

```cpp
element->style.display =
    "none";

document->update();
```

To show it again:

```cpp
element->style.display =
    "block";

document->update();
```

## Partial Re-rendering

GDOM tracks layout dirtiness and attempts to re-render the smallest safe layout boundary instead of rebuilding the entire document.

Layout contexts such as the resolved flow offset are preserved during rendering so local re-renders can keep their correct position inside block and flex layouts.

When GDOM cannot safely perform a local re-render, it may fall back to rendering a larger boundary or the document root.

## Document Lifetime

Create a document with:

```cpp
auto document =
    gdom::GDOMDocument::create(host);
```

The document is tied to its host lifetime.

Created elements are owned by the document and are released together with it.

Do not manually delete individual GDOM elements.

## Building

Clone the repository:

```bash
git clone https://github.com/zhuliss/gdom.git
cd gdom
```

Configure and build using your Geode development environment:

```bash
cmake -B build
cmake --build build --config Release
```

A clean rebuild is recommended after changing public GDOM headers that alter class layouts.

## Using GDOM as a Dependency

GDOM is designed to be used as a developer dependency by other Geode mods.

The public headers are located under:

```text
include/GDOM/
```

The intended entry points are:

```cpp
#include <GDOM/GDOMDocument.hpp>
#include <GDOM/HTMLElement.hpp>
```

Dependency packaging and stable API guarantees are still being finalized for the first public releases.

## Scope

GDOM intentionally does not currently implement:

- CSS selectors
- stylesheets
- CSS cascade
- grid layout
- animations
- transforms
- full browser event propagation
- full CSS sizing behavior
- the complete CSS `calc()` grammar
- a browser-compatible DOM

The goal is a compact UI abstraction suitable for Geometry Dash mods, not a web engine.

## Reporting Issues

If you find a crash, incorrect layout, rendering issue, or API bug, please open an issue on the GitHub repository and include:

- GDOM version
- Geode version
- Geometry Dash version
- platform
- minimal reproduction code
- crash log, if applicable

## Contributing

Contributions and bug reports are welcome.

When changing layout or rendering behavior, keep changes focused and include a small reproduction or regression test where possible.