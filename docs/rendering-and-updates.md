# Rendering & Updates

## Initial render

```cpp
document->render();
```

The document resolves layout and creates the corresponding cocos2d node tree.

## Resolved size cache

GDOM may cache resolved sizes internally to avoid duplicate calculations within a render pass. That cache is not part of the public API.

## MVP update contract

> **MVP target**

The completed MVP must support explicit updates after initial render.

Target usage:

```cpp
title->textContent = "Updated";
card->style.width = "400px";
document->update();
```

The final method name may be `update()`, `rerender()`, or equivalent, but semantics must include:

- invalidate stale resolved sizes
- recompute affected layout
- update text/value/style changes
- avoid duplicate rendered trees
- preserve event behavior
- safely replace or reuse cocos2d nodes

Automatic reactive dependency tracking is not required.

## Ownership

Before MVP completion, GDOM must define clear ownership for documents, elements, internal cocos2d nodes, callback targets and native input helpers.

Users should not need to manually delete elements created through `GDOMDocument`.
