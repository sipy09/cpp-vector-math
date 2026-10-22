#pragma once

#include <array>
#include <optional>

namespace math3d {

constexpr float kPi = 3.14159265358979323846f;
float Radians(float degrees);

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3 operator+(const Vec3& rhs) const;
    Vec3 operator-(const Vec3& rhs) const;
    Vec3 operator*(float scalar) const;
};

struct Vec4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;
};

float Dot(const Vec3& a, const Vec3& b);
Vec3 Cross(const Vec3& a, const Vec3& b);
float Length(const Vec3& value);
// Throws std::invalid_argument for vectors too short to normalize.
Vec3 Normalize(const Vec3& value);

// Row-major storage, column vectors: transformed = matrix * vector.
struct Mat4 {
    std::array<std::array<float, 4>, 4> m{};

    static Mat4 Identity();
    static Mat4 Translation(const Vec3& delta);
    static Mat4 Scale(float factor);
    static Mat4 RotationX(float radians);
    static Mat4 RotationY(float radians);
    static Mat4 RotationZ(float radians);
    // Right-handed camera, looking down -Z; OpenGL clip-space z in [-1, 1].
    static Mat4 Perspective(float verticalFovRadians, float aspect,
                            float nearPlane, float farPlane);

    Mat4 operator*(const Mat4& rhs) const;
    Vec4 operator*(const Vec4& rhs) const;
};

// Returns empty for points behind the camera or outside the near/far planes.
// Visibility at the sides of the viewport is deliberately not clipped.
std::optional<Vec2> ProjectPoint(const Vec3& point, const Mat4& mvp,
                                 int viewportWidth, int viewportHeight);

}  // namespace math3d
