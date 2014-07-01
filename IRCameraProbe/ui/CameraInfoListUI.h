#pragma once

namespace DuiLib {
class CContainerUI;
}
class CameraInfoUI;
class CameraInfoListUI {
public:
  void Init(DuiLib::CContainerUI* ui);
  int GetCameraInfoCount() const;

  CameraInfoUI* GetCameraInfo(int i);
private:
  DuiLib::CContainerUI* info_list_ui_;
};