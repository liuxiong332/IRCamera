#pragma once
#include <vector>
#include <memory>

namespace DuiLib {
class CContainerUI;
class CControlUI;
}
class CameraInfoUI;
class CameraInfoPrefBuilder;

class CameraInfoListUI {
public:
  void Init(DuiLib::CContainerUI* ui, CameraInfoPrefBuilder*  builder);
  int GetCameraInfoCount() const;

  CameraInfoUI* GetCameraInfo(int i);
  
  void AddCameraInfo();
  //remove all of the camera info ui that the checkbox is checked
  void RemoveAllCheckedCameraInfo();
  //enable or disable the checkbox before each camera info
  void EnableCheckbox(bool is_enable);
private:
  CameraInfoPrefBuilder*  camera_info_builder_;
  DuiLib::CControlUI*   edit_hide_control_;
  DuiLib::CContainerUI* info_list_ui_;

  typedef std::unique_ptr<CameraInfoUI> CameraInfoUIPtr;
  std::vector<CameraInfoUIPtr>  camera_info_list_;
};