# GDOM

GDOM is a **DOM-like UI framework for Geode mods**.

It provides a small HTML/CSS-inspired API on top of cocos2d, allowing mod developers to build interfaces with familiar elements, layouts, styles, inputs, and DOM-style mutations instead of manually positioning every node.

## Features

- `div`, `span`, `button`, and `input` elements
- block and flex layouts
- `flexGrow` and `flexShrink`
- `justifyContent` and `alignItems`
- margin, padding, and `gap`
- `px`, `%`, `rem`, and `calc(...)` lengths
- background colors, borders, and border radius
- `display: none`
- vertical `overflow: auto` / `scroll`
- click and input events
- native Geode `TextInput`
- `focus()` / `blur()`
- `onFocus` / `onBlur`
- `appendChild`, `removeChild`, `replaceChild`, and reparenting
- reactive style updates
- partial layout re-rendering
- document-owned element lifetime

## Example

Add GDOM as a dependency:

```json
"dependencies": {
  "zhuliss.gdom": ">=v0.1.0-beta.1"
}
```

Then include the public API:

```cpp
#include <zhuliss.gdom/include/GDOM/GDOMDocument.hpp>
```

Create a document and UI:

```cpp
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
    geode::log::info(
        "Clicked!");
};

root->appendChild(title);
root->appendChild(button);

document->appendChild(root);
document->render();
```

Runtime style changes are reactive:

```cpp
button->style.width =
    "180px";

button->style.backgroundColor = {
    80,
    150,
    220,
    255
};

document->update();
```

## Inputs

GDOM inputs use Geode's native `TextInput` internally.

```cpp
auto input =
    document->createElement("input");

input->placeholder =
    "Enter text";

input->onInput =
    [](
        const std::string &value)
{
    geode::log::info(
        "{}",
        value);
};

input->onFocus =
    []()
{
    geode::log::info(
        "Focused");
};

input->onBlur =
    []()
{
    geode::log::info(
        "Blurred");
};
```