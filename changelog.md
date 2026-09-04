## v0.1.0

Initial public release.

### Added

- DOM-like document and element tree.
- `div`, `span`, `button`, and `input` elements.
- Document-owned element lifetime management.
- Host-bound `GDOMDocument` lifecycle.
- Block layout.
- Flex layout with:
  - `row`
  - `column`
  - `justifyContent`
  - `alignItems`
  - `gap`
  - `flexGrow`
  - `flexShrink`
- Width and height handling.
- Min/max width and height.
- Margin and padding.
- Length support for:
  - `px`
  - `%`
  - `rem`
  - `calc(...)`
- `display: none`.
- Vertical `overflow: auto` and `overflow: scroll`.
- Background colors.
- Borders and border radius.
- Text color and font size.
- Rounded rectangle rendering.
- Reactive style properties.
- Paint-only updates.
- Partial layout re-rendering.
- Preserved layout flow context during partial re-renders.
- DOM mutations:
  - `appendChild`
  - `removeChild`
  - `replaceChild`
  - reparenting
- Cycle protection for DOM mutations.
- Native Geode `TextInput` integration.
- Input value and placeholder support.
- `onInput`.
- `focus()` and `blur()`.
- `onFocus` and `onBlur`.
- Single focused element tracking per document.
- Programmatic focus support.
- Button click handling.
- Cross-document move protection.
- Fallback to larger layout boundaries when local re-rendering is unsafe.

### Fixed

- Incorrect layout positioning during partial re-renders.
- Auto-height issues with hidden elements.
- Flex grow redistribution when min/max constraints are hit.
- Flex shrink redistribution with weighted shrink values.
- Invalid `calc(...)` expressions being partially accepted.
- Stale layout state after DOM mutations.
- Input focus state after element removal or replacement.
- Element lifetime issues caused by detached DOM nodes.
- Multiple stale-object crashes caused by ABI changes during development.