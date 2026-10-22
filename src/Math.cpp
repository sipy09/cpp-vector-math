#include "math3d/Math.hpp"

#include <cmath>
#include <stdexcept>

namespace math3d {

float Radians(float degrees) { return degrees * kPi / 180.0f; }

Vec3 Vec3::operator+(const Vec3& rhs) const {
    return {x + rhs.x, y + rhs.y, z + rhs.z};
}

Vec3 Vec3::operator-(const Vec3& rhs) const {
    return {x - rhs.x, y - rhs.y, z - rhs.z};
}

Vec3 Vec3::operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

float Dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Cross(const Vec3& a, const Vec3& b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

float Length(const Vec3& value) {
    return std::sqrt(Dot(value, value));
}

Vec3 Normalize(const Vec3& value) {
    const float len = Length(value);
    if (len <= 1e-6f) {
        throw std::invalid_argument("Cannot normalize a zero-length vector");
    }
    return value * (1.0f / len);
}

Mat4 Mat4::Identity() {
    Mat4 result;
    for (int i = 0; i < 4; ++i) result.m[i][i] = 1.0f;
    return result;
}

Mat4 Mat4::Translation(const Vec3& delta) {
    Mat4 result = Identity();
    result.m[0][3] = delta.x;
    result.m[1][3] = delta.y;
    result.m[2][3] = delta.z;
    return result;
}

Mat4 Mat4::Scale(float factor) {
    Mat4 result = Identity();
    for (int i = 0; i < 3; ++i) result.m[i][i] = factor;
    return result;
}

Mat4 Mat4::RotationX(float radians) {
    Mat4 result = Identity();
    const float c = std::cos(radians);
    const float s = std::sin(radians);
    result.m[1][1] = c;
    result.m[1][2] = -s;
    result.m[2][1] = s;
    result.m[2][2] = c;
    return result;
}

Mat4 Mat4::RotationY(float radians) {
    Mat4 result = Identity();
    const float c = std::cos(radians);
    const float s = std::sin(radians);
    result.m[0][0] = c;
    result.m[0][2] = s;
    result.m[2][0] = -s;
    result.m[2][2] = c;
    return result;
}

Mat4 Mat4::RotationZ(float radians) {
    Mat4 result = Identity();
    const float c = std::cos(radians);
    const float s = std::sin(radians);
    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;
    return result;
}

Mat4 Mat4::Perspective(float fov, float aspect, float nearPlane, float farPlane) {
    if (!(fov > 0.0f && fov < kPi && aspect > 0.0f &&
          nearPlane > 0.0f && farPlane > nearPlane)) {
        throw std::invalid_argument("Invalid perspective projection parameters");
    }

    const float f = 1.0f / std::tan(fov * 0.5f);
    Mat4 result;
    result.m[0][0] = f / aspect;
    result.m[1][1] = f;
    result.m[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
    result.m[2][3] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
    result.m[3][2] = -1.0f;
    return result;
}

Mat4 Mat4::operator*(const Mat4& rhs) const {
    Mat4 result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int k = 0; k < 4; ++k) {
                result.m[row][col] += m[row][k] * rhs.m[k][col];
            }
        }
    }
    return result;
}

Vec4 Mat4::operator*(const Vec4& v) const {
    const std::array<float, 4> a{v.x, v.y, v.z, v.w};
    std::array<float, 4> b{};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            b[row] += m[row][col] * a[col];
        }
    }
    return {b[0], b[1], b[2], b[3]};
}

std::optional<Vec2> ProjectPoint(const Vec3& point, const Mat4& mvp,
                                 int viewportWidth, int viewportHeight) {
    if (viewportWidth <= 0 || viewportHeight <= 0) return std::nullopt;

    const Vec4 clip = mvp * Vec4{point.x, point.y, point.z, 1.0f};
    if (clip.w <= 1e-6f) return std::nullopt;

    const float ndcX = clip.x / clip.w;
    const float ndcY = clip.y / clip.w;
    const float ndcZ = clip.z / clip.w;
    if (!(std::isfinite(ndcX) && std::isfinite(ndcY) &&
          std::isfinite(ndcZ)) || ndcZ < -1.0f || ndcZ > 1.0f) {
        return std::nullopt;
    }

    return Vec2{(ndcX + 1.0f) * 0.5f * viewportWidth,
                (1.0f - ndcY) * 0.5f * viewportHeight};
}

}  // namespace math3d
