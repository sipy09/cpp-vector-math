#include "Math3D.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace math3d;
bool near(double a,double b){return std::abs(a-b)<1e-9;}
int main(){
 Vec3 a{1,2,3},b{4,5,6};assert(near(a.dot(b),32));
 auto c=Vec3{1,0,0}.cross({0,1,0});assert(near(c.z,1)&&near(c.x,0));
 assert(near(Vec3{3,4,0}.length(),5));
 auto t=Mat4::translation({2,3,4}).transformPoint({1,1,1});assert(near(t.x,3)&&near(t.y,4)&&near(t.z,5));
 auto r=Mat4::rotationY(3.14159265358979323846/2).transformPoint({1,0,0});assert(near(r.x,0)&&near(r.z,-1));
 auto combined=(Mat4::translation({1,0,0})*Mat4::rotationY(3.14159265358979323846/2)).transformPoint({1,0,0});assert(near(combined.x,1)&&near(combined.z,-1));
 std::cout<<"All math tests passed\n";
}
