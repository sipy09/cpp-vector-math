#pragma once
#include <cmath>
#include <stdexcept>
namespace math3d {
struct Vec3 {
 double x{},y{},z{};
 Vec3 operator+(Vec3 b) const {return {x+b.x,y+b.y,z+b.z};}
 Vec3 operator-(Vec3 b) const {return {x-b.x,y-b.y,z-b.z};}
 Vec3 operator*(double s) const {return {x*s,y*s,z*s};}
 double dot(Vec3 b) const {return x*b.x+y*b.y+z*b.z;}
 Vec3 cross(Vec3 b) const {return {y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x};}
 double length() const {return std::sqrt(dot(*this));}
 Vec3 normalized() const {auto l=length(); if(l<1e-12) throw std::domain_error("zero vector"); return *this*(1.0/l);}
};
struct Mat4 {
 double m[4][4]{};
 static Mat4 identity(){Mat4 a;for(int i=0;i<4;i++)a.m[i][i]=1;return a;}
 static Mat4 rotationX(double t){auto a=identity();a.m[1][1]=std::cos(t);a.m[1][2]=-std::sin(t);a.m[2][1]=std::sin(t);a.m[2][2]=std::cos(t);return a;}
 static Mat4 rotationY(double t){auto a=identity();a.m[0][0]=std::cos(t);a.m[0][2]=std::sin(t);a.m[2][0]=-std::sin(t);a.m[2][2]=std::cos(t);return a;}
 static Mat4 translation(Vec3 v){auto a=identity();a.m[0][3]=v.x;a.m[1][3]=v.y;a.m[2][3]=v.z;return a;}
 Mat4 operator*(const Mat4& b)const {Mat4 c;for(int i=0;i<4;i++)for(int j=0;j<4;j++)for(int k=0;k<4;k++)c.m[i][j]+=m[i][k]*b.m[k][j];return c;}
 Vec3 transformPoint(Vec3 v)const {double p[4]={v.x,v.y,v.z,1},o[4]{};for(int i=0;i<4;i++)for(int j=0;j<4;j++)o[i]+=m[i][j]*p[j];if(std::abs(o[3])<1e-12)throw std::domain_error("invalid homogeneous w");return {o[0]/o[3],o[1]/o[3],o[2]/o[3]};}
};
}
