# Spacing

GDOM uses CSS-like margin, padding and gap.

## Shorthand

```cpp
style.padding = "10px";
style.padding = "10px 20px";
style.padding = "10px 20px 30px";
style.padding = "10px 20px 30px 40px";
```

The same syntax works for margin.

Expansion rules:

```text
1 value: all sides
2 values: vertical horizontal
3 values: top horizontal bottom
4 values: top right bottom left
```

Specific sides override shorthand values.

```cpp
style.margin = "10px";
style.marginLeft = "30px";
```

## gap

```cpp
container->style.gap = "10px";
```

Gap participates in block and flex layout.
