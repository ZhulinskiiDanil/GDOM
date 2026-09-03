# Geode Guidelines

GDOM is intended to follow Geode mod development guidelines.

## Numeric parsing

Avoid exception-based parsing such as `std::stof` / `std::stoi`.

Use Geode-compatible result-based parsing and check a Result before `unwrap()`.

## Exceptions

GDOM code should not depend on `try`, `catch`, or `throw` for ordinary parsing/control flow when result handling is available.

## Guideline checker

```bash
python scripts/check-geode-guidelines.py
```

The checker is a local lint aid and does not guarantee manual review approval.
