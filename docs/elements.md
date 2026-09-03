# Elements

GDOM's MVP element set is:

```text
div
span
button
input
```

Check [Status](status.md) for exact current implementation state.

## div

```cpp
auto card = document->createElement("div");
card->style.width = "320px";
card->style.height = "auto";
card->style.padding = "20px";
card->style.backgroundColor = {20, 22, 30, 240};
card->style.borderRadius = "16px";
```

A `div` can contain other elements and participate in block or flex layout.

## span

```cpp
auto title = document->createElement("span");
title->textContent = "GDOM";
title->style.width = "auto";
title->style.height = "auto";
title->style.fontSize = "22px";
title->style.color = {255, 255, 255, 255};
```

`width: auto` and `height: auto` use measured text size.

## button

```cpp
auto button = document->createElement("button");
button->textContent = "Continue";
button->style.width = "180px";
button->style.height = "44px";
button->style.backgroundColor = {50, 120, 255, 255};
button->style.borderRadius = "10px";
button->onClick = []() { log::info("Clicked"); };
```

A button uses a normal GDOM container for layout. cocos2d menu nodes are internal interaction details.

## input

```cpp
auto input = document->createElement("input");
input->placeholder = "Enter username";
input->value = "";
input->style.width = "100%";
input->style.height = "42px";
input->style.padding = "8px 12px";
input->style.backgroundColor = {35, 38, 50, 255};
input->style.borderColor = {70, 130, 255, 255};
input->style.borderWidth = "1px";
input->style.borderRadius = "10px";
input->onInput = [](const std::string& value) { log::info("{}", value); };
```

GDOM renders the visible box, text and caret itself. Native Geode input infrastructure may be used internally only for keyboard / IME handling.

**MVP target:** focus/blur callbacks, password mode, max length, focus styles.
