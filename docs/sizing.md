# Sizing

## Supported length syntax

```text
px
%
rem
auto
```

Raw numeric strings may also be accepted.

## px
Maps directly to GDOM/cocos2d layout units.

## %
Resolves against the relevant containing dimension.

## rem
Current MVP scale: `1rem = 10 GDOM/cocos2d units`.

## auto

For normal block layout, automatic width fills available width after relevant parent padding and element margins.

For text-like elements, automatic width is based on measured content.

For block / column flow, automatic height is measured from content, margins, gaps and vertical padding.

For a flex row, automatic container height uses the tallest participating child plus vertical box spacing.

## Min/max sizing

> **MVP target**

```cpp
style.minWidth
style.maxWidth
style.minHeight
style.maxHeight
```

Resolution order should be:

```text
requested size
→ automatic/content resolution
→ min/max constraints
→ final non-negative size
```

GDOM intentionally implements simplified, predictable CSS-inspired sizing.
