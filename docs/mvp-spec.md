# MVP Specification

This page defines what GDOM must provide before the MVP is considered complete.

## 1. DOM tree

Required:

- `GDOMDocument`
- `createElement`
- `appendChild`
- parent/child relationships
- nested rendering
- top-left DOM-like coordinates
- predictable ownership and lifetime management
- safe rendered-node replacement/removal

## 2. Elements

Required MVP elements:

```text
div
span
button
input
```

### div
General-purpose box/container.

### span
Text element with automatic text measurement.

### button
Clickable box with text.

### input
Editable single-line text field with custom GDOM rendering.

Required input behavior before MVP completion:

- value
- placeholder
- focus
- blur
- text input callback
- custom caret
- password mode
- max length
- custom background, border and radius

## 3. Box styling

Required:

- `backgroundColor`
- `borderColor`
- `borderWidth`
- `borderRadius`
- `width`
- `height`
- `minWidth`
- `maxWidth`
- `minHeight`
- `maxHeight`
- margin
- padding

Fill and border must be separate geometric regions so translucent fills do not alter border color.

## 4. Length values

Required:

```text
px
%
rem
auto
```

Raw numeric strings may also be accepted.

## 5. Block layout

Required:

- vertical normal flow
- margins
- padding
- gap
- `width: auto`
- `height: auto`
- nested block elements

## 6. Flex layout

Required:

- `display: flex`
- row
- column
- `justifyContent`
- `alignItems`
- gap
- margins
- padding
- `flexGrow`
- `flexShrink`

**MVP target:** `flexWrap`.

## 7. Text styling

Required:

- `textContent`
- `fontSize`
- `color`

**MVP target:** `textAlign`, clipping rules, configurable font resource.

## 8. Events

Required:

```cpp
element->onClick
input->onInput
```

**MVP target:**

```cpp
element->onFocus
element->onBlur
```

## 9. Rendering and updates

Required before MVP completion:

- initial `render()`
- explicit update/rerender mechanism
- resolved-size invalidation
- style/text/value changes after initial render
- prevention of duplicate rendered trees

Automatic reactive dependency tracking is not required.

## 10. Positioning and clipping

MVP target:

- normal flow
- explicit `left` / `top`
- relative positioning
- absolute positioning
- `overflow: hidden`

## 11. Out of MVP scope

- grid
- stylesheet parser
- CSS selectors
- HTML parser
- component framework
- transitions
- keyframe animations
- complete CSS compatibility
