#include "math3d/Math.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
int failures = 0;
int checks = 0;

void Check(bool ok, const std::string& name) {
    ++checks;
    if (!ok) {
        ++failures;
        std::cerr << "FAIL: " << name << '\n';
    }
}

bool Near(float a, float b, float tolerance = 0.0001f) {
    return std::fabs(a - b) <= tolerance;
}

void CheckVec(math3d::Vec3 v, math3d::Vec3 expected, const std::string& name) {
    Check(Near(v.x, expected.x) && Near(v.y, expected.y) &&
          Near(v.z, expected.z), name);
}
}  // namespace

int main() {
    using namespace math3d;

    CheckVec(Vec3{1, 2, 3} + Vec3{4, 5, 6}, {5, 7, 9}, "vector addition");
    CheckVec(Vec3{6, 5, 4} - Vec3{1, 2, 3}, {5, 3, 1}, "vector subtraction");
    CheckVec(Vec3{2, -1, 3} * 2.0f, {4, -2, 6}, "scalar multiplication");
    Check(Near(Dot({1, 2, 3}, {4, 5, 6}), 32.0f), "dot product");
    CheckVec(Cross({1, 0, 0}, {0, 1, 0}), {0, 0, 1}, "right-handed cross product");
    CheckVec(Cross({0, 1, 0}, {1, 0, 0}), {0, 0, -1}, "cross product orientation");
    Check(Near(Length({3, 4, 0}), 5.0f), "vector length");
    CheckVec(Normalize({0, 3, 4}), {0, 0.6f, 0.8f}, "normalize vector");
    try {
        Normalize({0, 0, 0});
        Check(false, "reject zero-length normalization");
    } catch (const std::invalid_argument&) {
        Check(true, "reject zero-length normalization");
    }
    Check(Near(Radians(180.0f), kPi), "degrees to radians");

    const Vec4 p{1, 2, 3, 1};
    const Vec4 id = Mat4::Identity() * p;
    Check(Near(id.x, 1) && Near(id.y, 2) && Near(id.z, 3) && Near(id.w, 1),
          "identity matrix");

    const Vec4 t = Mat4::Translation({5, 6, 7}) * p;
    Check(Near(t.x, 6) && Near(t.y, 8) && Near(t.z, 10) && Near(t.w, 1),
          "translation matrix");
    const Vec4 s = Mat4::Scale(2.0f) * p;
    Check(Near(s.x, 2) && Near(s.y, 4) && Near(s.z, 6) && Near(s.w, 1),
          "uniform scale matrix");

    const Vec4 rx = Mat4::RotationX(kPi * 0.5f) * Vec4{0, 1, 0, 1};
    Check(Near(rx.x, 0) && Near(rx.y, 0) && Near(rx.z, 1), "rotate 90deg around X");
    const Vec4 ry = Mat4::RotationY(kPi * 0.5f) * Vec4{1, 0, 0, 1};
    Check(Near(ry.x, 0) && Near(ry.z, -1), "rotate 90deg around Y");
    const Vec4 rz = Mat4::RotationZ(kPi * 0.5f) * Vec4{1, 0, 0, 1};
    Check(Near(rz.x, 0) && Near(rz.y, 1), "rotate 90deg around Z");

    const Vec4 composed =
        (Mat4::Translation({5, 0, 0}) * Mat4::Scale(2)) * Vec4{1, 0, 0, 1};
    Check(Near(composed.x, 7), "matrix multiplication order");

    const Mat4 projection = Mat4::Perspective(Radians(60), 800.0f / 600, 0.1f, 100);
    const Mat4 view = Mat4::Translation({0, 0, -5});
    const auto center = ProjectPoint({0, 0, 0}, projection * view, 800, 600);
    Check(center && Near(center->x, 400) && Near(center->y, 300),
          "center projects to viewport center");
    const auto right = ProjectPoint({1, 0, 0}, projection * view, 800, 600);
    Check(right && right->x > 400, "positive x projects right");
    const auto up = ProjectPoint({0, 1, 0}, projection * view, 800, 600);
    Check(up && up->y < 300, "positive y projects upwards on screen");
    Check(!ProjectPoint({0, 0, 10}, projection * view, 800, 600),
          "point behind camera is rejected");
    Check(!ProjectPoint({0, 0, 0}, projection * view, 0, 600),
          "invalid viewport is rejected");
    Check(!ProjectPoint({0, 0, 4.95f}, projection * view, 800, 600),
          "point before near plane is rejected");
    try {
        Mat4::Perspective(Radians(60), 1, 5, 1);
        Check(false, "reject invalid near/far plane");
    } catch (const std::invalid_argument&) {
        Check(true, "reject invalid near/far plane");
    }

    if (failures == 0) {
        std::cout << "PASS: " << checks << " checks (Vec3, Mat4, projection)\n";
        return 0;
    }
    std::cerr << "FAILED: " << failures << " of " << checks << " checks\n";
    return 1;
}
