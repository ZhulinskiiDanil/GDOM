# Getting Started

## Requirements

- Geode 5.8.2
- Geometry Dash 2.2081
- Current Geode C++ toolchain

## Create a document

```cpp
auto document =
    gdom::GDOMDocument::create(this);
```

The passed `CCNode` becomes the root host. Nested GDOM elements do not need direct access to the cocos2d parent.

## Create elements

```cpp
auto card = document->createElement("div");
auto title = document->createElement("span");
auto input = document->createElement("input");
auto button = document->createElement("button");
```

## Build the tree

```cpp
title->textContent = "GDOM";
input->placeholder = "Enter your name";
button->textContent = "Continue";

card->appendChild(title);
card->appendChild(input);
card->appendChild(button);

document->appendChild(card);
```

## Style it

```cpp
card->style.width = "320px";
card->style.height = "auto";
card->style.display = "flex";
card->style.flexDirection = "column";
card->style.padding = "20px";
card->style.gap = "12px";
card->style.backgroundColor = {20, 22, 30, 240};
card->style.borderRadius = "16px";
```

## Render

```cpp
document->render();
```

For current implementation coverage, see [Status](status.md).
