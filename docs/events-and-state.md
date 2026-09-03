# Events & State

## Click

```cpp
button->onClick = []()
{
    log::info("Clicked");
};
```

## Input

```cpp
input->onInput = [](const std::string& value)
{
    log::info("{}", value);
};
```

The element's `value` is updated as input changes.

## Focus and blur

> **MVP target**

```cpp
input->onFocus = []() {};
input->onBlur = []() {};
```

Focus state must be usable for visual styling without reaching into the native input helper.

## Input-specific state

MVP target:

```cpp
input->value
input->placeholder
input->maxLength
input->inputType
```

Expected `inputType` values:

```text
text
password
```

Password mode must mask rendered characters while preserving the actual `value`.
