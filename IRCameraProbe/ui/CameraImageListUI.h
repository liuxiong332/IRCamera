#pragma once
#include "IRCameraBasic.h"
#include <map>
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
  typedef std::unique_ptr<CameraImageContainerUI> ScopedCameraImageContainerUIPtr;
  typedef std::map<DuiLib::CContainerUI*, ScopedCameraImageContainerUIPtr>  CameraImageMap;
  typedef CameraImageMap::iterator  CameraImageMapIterator;

  CameraImageMap  camera_image_ui_map_;
};