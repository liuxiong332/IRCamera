#pragma once

namespace camera {
const static float kKelvinTransform = 273.15f;
inline static float TransformKelvinToCelsius(float kelvin) {
  return kelvin - kKelvinTransform;
}
}