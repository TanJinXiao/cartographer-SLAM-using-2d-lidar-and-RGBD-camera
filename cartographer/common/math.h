/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
common/math.h文件主要实现数学计算，包括：
区间截断.求n次方.求平方.幅度角度转换.归一化.反正切值
*/


#ifndef CARTOGRAPHER_COMMON_MATH_H_
#define CARTOGRAPHER_COMMON_MATH_H_

#include <cmath>
#include <vector>

#include "Eigen/Core"
#include "cartographer/common/port.h"
#include "ceres/ceres.h"

namespace cartographer {
namespace common {

// Clamps 'value' to be in the range ['min', 'max'].
//将val截取到区间min至max中.
template <typename T>
T Clamp(const T value, const T min, const T max) {
  if (value > max) {
    return max;
  }
  if (value < min) {
    return min;
  }
  return value;
}

// Calculates 'base'^'exponent'.计算base的exp次方
template <typename T>
constexpr T Power(T base, int exponent) {
  return (exponent != 0) ? base * Power(base, exponent - 1) : T(1);
}

// Calculates a^2.求平方
template <typename T>
constexpr T Pow2(T a) {
  return Power(a, 2);
}

// Converts from degrees to radians.角度到弧度的转换. 60° -> pi/3
constexpr double DegToRad(double deg) { return M_PI * deg / 180.; }

// Converts form radians to degrees.弧度到角度的转换, pi/3 -> 60°
constexpr double RadToDeg(double rad) { return 180. * rad / M_PI; }

// Bring the 'difference' between two angles into [-pi; pi].将角度差转换为[-pi;pi] 
template <typename T>
T NormalizeAngleDifference(T difference) {
  const T kPi = T(M_PI);
  while (difference > kPi) difference -= 2. * kPi;
  while (difference < -kPi) difference += 2. * kPi;
  return difference;
}
/*
atan2 返回原点至点(x,y)的方位角，即与 x 轴的夹角，
也可以理解为计算复数 x+yi 的辐角,范围是[-pi,pi]
ATAN2(1,1) -> pi/4:以弧度表示点(1,1)的反正切值，即pi/4(0.785398)
*/
template <typename T>
T atan2(const Eigen::Matrix<T, 2, 1>& vector) {
  return ceres::atan2(vector.y(), vector.x());
}
/*
四元数（Quaternion）
四元数一般定义如下： q=w+xi+yj+zk
其中w,x,y,z为实数,且i*i=-1,j*j=-1,k*k=-1
也可以表示为q=[w,v]，其中w是标量，v是矢量v=(x,y,z)

一个四元数（Quaternion）描述了一个旋转轴和一个旋转角度。
这个旋转轴和这个角度可以通过 Quaternion::ToAngleAxis转换得到。

当然也可以随意指定一个角度一个旋转轴来构造一个Quaternion。
这个角度是相对于单位四元数而言的，也可以说是相对于物体的初始方向而言的。

当用一个四元数乘以一个向量时，实际上就是让该向量围绕着这个
四元数所描述的旋转轴，转动这个四元数所描述的角度而得到的向量。
*/
template <typename T>
inline void QuaternionProduct(const double* const z, const T* const w,
                              T* const zw) {
  zw[0] = z[0] * w[0] - z[1] * w[1] - z[2] * w[2] - z[3] * w[3];
  zw[1] = z[0] * w[1] + z[1] * w[0] + z[2] * w[3] - z[3] * w[2];
  zw[2] = z[0] * w[2] - z[1] * w[3] + z[2] * w[0] + z[3] * w[1];
  zw[3] = z[0] * w[3] + z[1] * w[2] - z[2] * w[1] + z[3] * w[0];
}

}  // namespace common
}  // namespace cartographer

#endif  // CARTOGRAPHER_COMMON_MATH_H_

/*
参考文章：
https://blog.csdn.net/hany3000/article/details/9248081
*/