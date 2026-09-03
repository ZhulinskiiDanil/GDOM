# Block Layout

`div` uses block layout by default.

```cpp
container->style.display = "block";
container->style.padding = "20px";
container->style.gap = "10px";
```

## MVP behavior

Block flow accounts for:

- parent padding
- child margins
- gap
- child resolved sizes
- `width: auto`
- `height: auto`
- nested block / flex containers

**MVP target:** min/max sizing, overflow clipping, relative/absolute-positioned children.

Absolute-positioned children should not contribute to normal-flow height.
