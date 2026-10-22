# C++ Vector Math — Rotating 3D Cube

A small C++17 linear algebra library with an interactive rotating wireframe cube visualized using raylib.

## Features
- `Vec3`: addition, subtraction, scaling, dot/cross product, length and normalization
- `Mat4`: identity, X/Y rotation, translation, matrix multiplication, homogeneous point transforms
- Perspective projection of 3D vertices onto a 2D screen
- 6 automated math checks via CTest
- SPACE pauses animation; R resets; ESC exits

## Build & run (macOS / Homebrew)

```bash
brew install cmake raylib
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix raylib)"
cmake --build build
ctest --test-dir build --output-on-failure
./build/vector_demo
```

## Run tests without raylib

```bash
cmake -S . -B build-tests -DBUILD_DEMO=OFF
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

## How it works

Vertices are rotated using a 4×4 matrix. For each transformed vertex `(x, y, z)`, the demo uses a simple perspective camera with `depth = z + 5` and focal length `420`: screen coordinates are `(480 + 420*x/depth, 330 - 420*y/depth)`. Edges connect the projected vertices. This is a lightweight projection demo, not a full rendering engine.

## Project layout

- `include/Math3D.hpp` — self-contained vector and matrix operations
- `src/main.cpp` — raylib visualization
- `tests/math_tests.cpp` — unit checks using standard assertions

## Learning goals

Practice linear algebra for graphics, coordinate transformations, C++ data structures, and repeatable builds/tests.

## License

MIT (see LICENSE).
