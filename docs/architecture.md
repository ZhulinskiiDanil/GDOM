# Architecture

## Public DOM layer

```text
GDOMDocument
└── HTMLElement
    ├── HTMLDivElement
    ├── HTMLSpanElement
    ├── HTMLButtonElement
    └── HTMLInputElement
```

`GDOMDocument` connects a GDOM tree to a cocos2d host.

`HTMLElement` contains shared DOM-facing state such as style, parent, children and resolved size.

## Layout

```text
Layout
├── BlockLayout
└── FlexLayout
```

Layout logic is separated from element rendering.

## Resolvers

```text
Resolvers
├── LengthResolver
└── BoxResolver
```

`LengthResolver` resolves `px`, `%`, `rem`, `auto`, and raw numeric strings.

`BoxResolver` expands CSS-like 1–4 value margin/padding shorthand and applies per-side overrides.

## Box rendering

```text
RoundedRectNode
```

Box-like elements share one rounded renderer for fill, border and border radius.

Current users:

```text
div
button
input
```

Fill and border are separate geometric regions so translucent fill does not blend over the border.

## Input architecture

```text
HTMLInputElement
├── GDOM background / border
├── GDOM text
├── GDOM caret
└── native input helper
```

The native Geode text input exists only for keyboard / IME behavior and should not define public GDOM appearance.

## Event adapters

When cocos2d requires a `CCObject` callback target, GDOM uses internal adapter nodes instead of forcing `HTMLElement` to inherit from cocos2d callback classes.

## MVP architectural requirements

- reliable ownership / lifetime policy
- render invalidation
- explicit document update/rerender
- min/max sizing
- flex grow/shrink
- focus/blur state
- safe rendered-node replacement/removal
