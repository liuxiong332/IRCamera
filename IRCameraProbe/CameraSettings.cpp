#include "CameraSettings.h"


CameraSettings::CameraSettings()
    : stable_max_temp_(40),
    unstable_average_max_delta_temp_(20),
    unstable_max_delta_temp_(20),
    can_measure_min_temp_(0),
    can_measure_max_temp_(100) {
}
 