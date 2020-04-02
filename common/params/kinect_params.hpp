#pragma once

#include <cmath>

namespace Params::Kinect
{

namespace RGB
{
inline constexpr int width = 1920;
inline constexpr int height = 1080;
using pixel_type = char;
inline constexpr int channel = 4;
inline constexpr int size = width * height * sizeof(pixel_type) * channel;
}  // namespace RGB

namespace Ir
{
inline constexpr int width = 512;
inline constexpr int height = 424;
using pixel_type = float;
inline constexpr int channel = 1;
inline constexpr int size = width * height * sizeof(pixel_type) * channel;
}  // namespace Ir

namespace Depth
{
inline constexpr int width = 512;
inline constexpr int height = 424;
using pixel_type = float;
inline constexpr int channel = 1;
inline constexpr int size = width * height * sizeof(pixel_type) * channel;
}  // namespace Depth


/* 視野角
 * http://neareal.com/629/
 * (水平)84.1 度 (垂直) 53.8 度
 */
namespace ViewAngle
{
constexpr double horizontal = 84.1 * M_PI / 180.0;
constexpr double vertical = 53.8 * M_PI / 180.0;
}  // namespace ViewAngle

}  // namespace Params::Kinect
