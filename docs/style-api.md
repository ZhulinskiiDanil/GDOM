# Style API

Styles are accessed through `element->style`.

```cpp
element->style.width = "100%";
element->style.display = "flex";
element->style.borderRadius = "12px";
```

## Box properties

MVP contract:

```cpp
style.width
style.height
style.minWidth
style.maxWidth
style.minHeight
style.maxHeight
style.left
style.top
style.margin
style.marginTop
style.marginRight
style.marginBottom
style.marginLeft
style.padding
style.paddingTop
style.paddingRight
style.paddingBottom
style.paddingLeft
style.backgroundColor
style.borderColor
style.borderWidth
style.borderRadius
```

## Layout properties

```cpp
style.display
style.flexDirection
style.justifyContent
style.alignItems
style.gap
style.flexGrow
style.flexShrink
style.flexWrap
```

`flexGrow`, `flexShrink`, and `flexWrap` are part of the MVP target.

## Text properties

```cpp
style.fontSize
style.color
style.textAlign
```

`textAlign` is an MVP-target property.

## Positioning / clipping

MVP target:

```cpp
style.position
style.overflow
```

Expected values include:

```text
position: static | relative | absolute
overflow: visible | hidden
```

## Rounded box rendering

`div`, `button`, and `input` share one rounded-box model.

```cpp
element->style.backgroundColor = {20, 22, 30, 180};
element->style.borderColor = {70, 130, 255, 255};
element->style.borderWidth = "2px";
element->style.borderRadius = "12px";
```

Fill and border are separate geometric regions. A translucent fill must not blend over the border.
