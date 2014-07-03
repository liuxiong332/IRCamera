#pragma once

class CameraInfoListUI;
namespace camera {
class CameraInfoPref;
class CameraInfoListLinker {
public:
  void Init(CameraInfoListUI*);

  void CameraInfoUIChanged();
private:
  CameraInfoListUI*   list_ui_;
};
}