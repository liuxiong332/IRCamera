#pragma once
#include "IRCameraBasic.h"
#include <vector>
#include <memory>

namespace DuiLib {
class CContainerUI;
}
class CameraImageContainerUI;

class CameraImageListUI {
public:
  CameraImageListUI();

  void Init(DuiLib::CContainerUI* container_ui);

  CameraImageContainerUI* GetItemAt(int i);

  int GetCount() const;

  void PushBack(CameraImageContainerUI* container_ui);
  void InsertAt(int i, CameraImageContainerUI* container_ui);
  void Remove(CameraImageContainerUI* container_ui);
  void RemoveAt(int i);

  void RemoveAll();
private:
  DuiLib::CContainerUI* image_list_ui_;
  std::vector<CameraImageContainerUI*>  camera_image_ui_list_;
};