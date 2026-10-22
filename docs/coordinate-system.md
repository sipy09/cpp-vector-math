# Coordinate systems and transformations

The library uses row-major matrix **storage**, but **column-vector multiplication** (`Mat4 * Vec4`).

For a local-space vertex `p`, its clip-space position is:

`clip = projection * view * model * Vec4(p.x, p.y, p.z, 1)`

1. **Model** rotates the cube around X/Y. Because `model = rotationY * rotationX`, X rotation is applied first.
2. **View** moves the scene away from the virtual camera using `translation(0, 0, -distance)`. The camera is at the origin, facing the negative Z direction.
3. **Projection** uses a right-handed perspective matrix with an OpenGL-style depth range of `[-1, 1]` after division.
4. **Perspective divide:** `ndc = clip.xyz / clip.w`, where a positive `clip.w` means the vertex is in front of the camera.
5. **Viewport mapping** converts NDC to pixel coordinates. Screen Y increases downward, so we invert NDC Y.

## Why use raylib?

raylib only supplies the window, keyboard input, primitive line drawing, text, and timing. No `Camera3D` or `BeginMode3D` is used — vector/matrix operations and 3D-to-2D projection are implemented in C++ to demonstrate the underlying mathematics.

## About clipping

Vertices outside the front/back clip planes return no screen position. Edges with one missing endpoint are not rendered. Correct clipping of line segments against the near plane would be a useful future extension.
