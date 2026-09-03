# Flex Layout

Enable flex:

```cpp
container->style.display = "flex";
```

## flexDirection

```text
row
column
```

## justifyContent

```text
flex-start
center
flex-end
space-between
space-around
space-evenly
```

## alignItems

```text
flex-start
center
flex-end
```

## gap / spacing

```cpp
container->style.padding = "20px 30px";
container->style.gap = "10px";
child->style.margin = "10px";
```

## flexGrow / flexShrink

> **MVP target**

```cpp
child->style.flexGrow = 1.f;
child->style.flexShrink = 1.f;
```

Required behavior:

- remaining main-axis space can be distributed by `flexGrow`
- insufficient main-axis space can be reduced by `flexShrink`
- margins and gap remain part of the calculation

## flexWrap

> **MVP target**

```cpp
container->style.flexWrap = "wrap";
```

Expected values:

```text
nowrap
wrap
```

Full browser Flexbox compatibility is not an MVP goal.
