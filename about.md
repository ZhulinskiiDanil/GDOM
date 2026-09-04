# GDOM

GDOM is a **DOM-like UI framework for Geode mods**.

It provides a small HTML/CSS-inspired API on top of cocos2d so mod developers can build interfaces using elements, layout rules, reactive styles, inputs, and DOM-style mutations instead of positioning every node manually.

## What GDOM provides

- `div`, `span`, `button`, and `input` elements
- block and flex layouts
- `flexGrow` / `flexShrink`
- `justifyContent` / `alignItems`
- margin, padding, and `gap`
- fixed, percentage, `rem`, and `calc(...)` lengths
- background colors, borders, and border radius
- `display: none`
- vertical scrolling with `overflow: auto` / `scroll`
- click and input events
- native Geode `TextInput`
- `focus()` / `blur()` and focus events
- `appendChild`, `removeChild`, `replaceChild`, and reparenting
- reactive paint and layout updates
- partial layout re-rendering
- document-owned element lifetime management

## Example

```cpp
#include <GDOM/GDOMDocument.hpp>

auto document =
    gdom::GDOMDocument::create(this);

auto root =
    document->createElement("div");

root->style.width = "100%";
root->style.height = "100%";
root->style.display = "flex";
root->style.flexDirection = "column";
root->style.gap = "10px";
root->style.padding = "16px";

auto title =
    document->createElement("span");

title->textContent =
    "Hello from GDOM";

auto button =
    document->createElement("button");

button->textContent =
    "Click me";

button->onClick =
    []()
{
    geode::log::info("Clicked!");
};

root->appendChild(title);
root->appendChild(button);

document->appendChild(root);
document->render();
```

Runtime style changes are reactive. Call `document->update()` after changing properties:

```cpp
button->style.width = "180px";
button->style.backgroundColor = {
    80,
    150,
    220,
    255
};

document->update();
```

## Inputs

GDOM inputs use Geode's native `TextInput` internally:

```cpp
auto input =
    document->createElement("input");

input->placeholder =
    "Enter text";

input->onInput =
    [](const std::string& value)
{
    geode::log::info("{}", value);
};

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

## Scope

GDOM is intentionally **not** a browser engine.

It currently does not implement CSS selectors, stylesheets, the CSS cascade, Grid, animations, transforms, full browser event propagation, or the complete CSS specification.

The goal is to provide a compact and familiar UI abstraction specifically for Geometry Dash / Geode mod development.

## Early release

GDOM `v0.1.0` is the first public release.

The API is usable, but it is still early and may change in future versions.

If you find a crash, layout issue, or API bug, please report it through the GitHub issue tracker.
