# GDOM

GDOM is a DOM-inspired UI and layout layer for Geometry Dash mods built with Geode.

It provides a web-like way to build cocos2d interfaces using nested elements and CSS-inspired styles.

Instead of manually calculating positions:

```cpp
node->setPosition(...);
```

GDOM allows you to describe layout through styles:

```cpp
auto document =
    gdom::GDOMDocument::create(this);

auto container =
    document->createElement("div");

container->style.width = "300px";
container->style.height = "auto";

container->style.padding = "20px";
container->style.gap = "10px";

document->appendChild(container);
document->render();
```

> GDOM is currently an experimental MVP.

## Features

Current GDOM MVP supports:

- DOM-like element tree
- `div`
- nested elements
- top-left DOM-like coordinates
- block layout
- flex layout
- `row`
- `column`
- `justifyContent`
- `alignItems`
- `gap`
- `margin`
- `padding`
- CSS-like spacing shorthand
- per-side spacing overrides
- `px`
- `%`
- `rem`
- `width: auto`
- `height: auto`
- automatic nested layout
- CSS-inspired string style API

Example:

```cpp
wrapper->style.display = "flex";
wrapper->style.flexDirection = "row";

wrapper->style.justifyContent =
    "space-between";

wrapper->style.alignItems =
    "center";

wrapper->style.padding =
    "20px 30px";
```

## Requirements

- Geode 5.8.2
- Geometry Dash 2.2081
- Current Geode C++ toolchain

## Example

```cpp
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include <GDOM/GDOMDocument.hpp>

using namespace geode::prelude;

class $modify(MenuLayer)
{
    bool init()
    {
        if (!MenuLayer::init())
        {
            return false;
        }

        auto document =
            gdom::GDOMDocument::create(this);

        auto wrapper =
            document->createElement("div");

        wrapper->style.left = "100px";
        wrapper->style.top = "50px";

        wrapper->style.width = "350px";
        wrapper->style.height = "160px";

        wrapper->style.display = "flex";
        wrapper->style.flexDirection = "row";

        wrapper->style.justifyContent =
            "space-between";

        wrapper->style.alignItems =
            "center";

        wrapper->style.padding =
            "20px 30px";

        wrapper->style.backgroundColor = {
            255,
            0,
            0,
            180
        };

        auto first =
            document->createElement("div");

        first->style.width = "60px";
        first->style.height = "60px";

        first->style.backgroundColor = {
            0,
            255,
            0,
            255
        };

        auto second =
            document->createElement("div");

        second->style.width = "60px";
        second->style.height = "60px";

        second->style.backgroundColor = {
            0,
            100,
            255,
            255
        };

        wrapper->appendChild(first);
        wrapper->appendChild(second);

        document->appendChild(wrapper);
        document->render();

        return true;
    }
};
```

## CSS-inspired API

GDOM intentionally uses a style API similar to JavaScript DOM styles.

JavaScript:

```js
block.style.width = "100%";
block.style.height = "auto";
block.style.display = "flex";
block.style.flexDirection = "column";
block.style.padding = "20px";
```

GDOM:

```cpp
block->style.width = "100%";
block->style.height = "auto";
block->style.display = "flex";
block->style.flexDirection = "column";
block->style.padding = "20px";
```

## Supported length units

```text
px
%
rem
auto
```

Examples:

```cpp
block->style.width = "300px";
block->style.width = "50%";

block->style.padding = "2rem";

block->style.height = "auto";
```

Current MVP definition:

```text
1rem = 10 cocos2d units
```

## Flexbox

Enable flex layout:

```cpp
container->style.display = "flex";
```

Supported `flexDirection`:

```text
row
column
```

Supported `justifyContent`:

```text
flex-start
center
flex-end
space-between
space-around
space-evenly
```

Supported `alignItems`:

```text
flex-start
center
flex-end
```

## Spacing shorthand

GDOM supports CSS-like shorthand values.

```cpp
style.padding = "10px";
```

```cpp
style.padding = "10px 20px";
```

```cpp
style.padding = "10px 20px 30px";
```

```cpp
style.padding =
    "10px 20px 30px 40px";
```

The same syntax works for margin:

```cpp
style.margin = "10px 20px";
```

Specific sides may override shorthand values:

```cpp
style.margin = "10px";
style.marginLeft = "30px";
```

## Documentation

Full documentation is available in [`docs/`](docs/README.md).

Topics:

- [Getting Started](docs/getting-started.md)
- [Elements](docs/elements.md)
- [Sizing](docs/sizing.md)
- [Spacing](docs/spacing.md)
- [Block Layout](docs/block-layout.md)
- [Flex Layout](docs/flex-layout.md)
- [Architecture](docs/architecture.md)
- [Geode Guidelines](docs/geode-guidelines.md)
- [Roadmap](docs/roadmap.md)

## Architecture

Current internal structure:

```text
GDOMDocument
│
└── HTMLElement
    │
    └── HTMLDivElement

Layout
├── BlockLayout
└── FlexLayout

Resolvers
├── LengthResolver
└── BoxResolver
```

Layout calculation is separated from element rendering so the engine can be extended without turning `HTMLElement` into a large monolithic class.

## Geode Guidelines

GDOM avoids exception-based numeric parsing such as:

```cpp
std::stof(...)
std::stoi(...)
```

and uses Geode-compatible result-based parsing instead.

The repository also contains:

```text
scripts/check-geode-guidelines.py
```

Run:

```bash
python scripts/check-geode-guidelines.py
```

before builds or commits.

## Roadmap

Near-term:

```text
[ ] span / text
[ ] button
[ ] event listeners
[ ] min-width / max-width
[ ] min-height / max-height
[ ] safer ownership and lifetime management
[ ] rerender / update API
[ ] flex-grow
[ ] flex-shrink
```

Possible later features:

```text
[ ] flex-wrap
[ ] overflow / clipping
[ ] absolute positioning
[ ] grid
[ ] CSS classes
[ ] selectors
[ ] stylesheet parser
[ ] HTML-like markup
[ ] components
[ ] transitions / animations
```

## Scope

GDOM is not intended to implement a complete browser engine or the entire CSS specification.

The goal is to provide a practical DOM/CSS-inspired layout system for Geode mods.

## Project

**ID**

```text
zhuliss.gdom
```

**Developer**

```text
Zhuliss
```

**Version**

```text
v0.1.0
```

**Source**

https://github.com/zhuliss/gdom
