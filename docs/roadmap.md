# Roadmap

The roadmap is organized by milestones rather than individual commits. For exact current state, see [Status](status.md).

## MVP Foundation

The MVP is complete when the contract in [MVP Specification](mvp-spec.md) is satisfied.

Core areas:

```text
DOM tree
elements
box model
sizing
block layout
flex layout
text
events
input
updates
ownership
```

## MVP completion work

### Interaction
- focus
- blur
- password input
- max length

### Sizing
- min/max width
- min/max height

### Flex
- flex grow
- flex shrink
- flex wrap if retained in MVP

### Rendering lifecycle
- invalidation
- explicit update/rerender
- safe replacement of rendered nodes

### Ownership
- document owns created elements
- predictable cleanup
- no user-managed deletion requirement

### Positioning / clipping
- relative positioning
- absolute positioning
- overflow hidden

### Text polish
- text alignment
- configurable font resource if retained in MVP

## Post-MVP

```text
grid
CSS classes
selectors
stylesheet parser
HTML-like markup
component system
transitions
animations
advanced text layout
browser-like CSS compatibility
```

## Scope

GDOM is not intended to become a browser engine. Prefer a small, predictable CSS-inspired API that maps well to Geometry Dash / cocos2d UI needs.
