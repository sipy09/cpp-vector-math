#include "raylib.h"
#include "Math3D.hpp"
#include <array>
#include <cmath>
using math3d::Vec3;
using math3d::Mat4;
int main(){
 InitWindow(960,640,"C++ Vector Math | Rotating Wireframe Cube");SetTargetFPS(60);
 constexpr std::array<Vec3,8> corners={{{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}}};
 constexpr std::array<std::array<int,2>,12> edges={{{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}}};
 double angle=0;bool paused=false;
 while(!WindowShouldClose()){
  if(IsKeyPressed(KEY_SPACE))paused=!paused;
  if(IsKeyPressed(KEY_R))angle=0;
  if(!paused)angle+=GetFrameTime()*0.7;
  Mat4 model=Mat4::rotationY(angle)*Mat4::rotationX(angle*0.65);
  std::array<Vector2,8> screen{};
  for(size_t i=0;i<corners.size();++i){Vec3 p=model.transformPoint(corners[i]);double z=p.z+5.0;double f=420.0/z;screen[i]={(float)(480+p.x*f),(float)(330-p.y*f)};}
  BeginDrawing();ClearBackground(Color{16,22,35,255});
  DrawText("3D VECTOR MATH",30,24,30,RAYWHITE);
  DrawText("Custom Vec3 + Mat4 | Perspective projection",30,66,19,LIGHTGRAY);
  for(auto e:edges)DrawLineEx(screen[e[0]],screen[e[1]],3,SKYBLUE);
  for(auto p:screen)DrawCircleV(p,5,ORANGE);
  DrawText("SPACE: pause/resume     R: reset angle     ESC: quit",30,590,18,LIGHTGRAY);
  if(paused)DrawText("PAUSED",410,105,25,YELLOW);
  EndDrawing();
 }
 CloseWindow();
}
