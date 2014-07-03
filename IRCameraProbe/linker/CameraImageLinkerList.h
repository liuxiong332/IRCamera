#pragma once
#include "pref/CameraInfoPrefObserver.h"
#include <vector>
#include <memory>

class CameraImageBuilder;
class CameraImageListUI;
class CameraImageContainerUI;

namespace camera {

class CameraImageContainerDeviceLinker;
class CameraInfoPref;
class CameraImageLinkerCreator;

class CameraImageLinkerList: public CameraInfoPrefObserver {
public:
  void  Init(CameraImageBuilder*, CameraImageListUI* list_ui);

  ~CameraImageLinkerList();
private:
  virtual void CameraInfoPrefChanged() override;

  void ReloadCameraList();
  void InsertIntoList(CameraImageContainerDeviceLinker* linker);

  CameraImageListUI*  list_ui_;
  CameraImageBuilder* camera_image_builder_;

  typedef std::unique_ptr<CameraImageContainerDeviceLinker> ImageDeviceLinkerPtr;
  //the link list is used for delete the unused device linker
  std::vector<ImageDeviceLinkerPtr>   delete_link_list_; 
  std::vector<ImageDeviceLinkerPtr>  device_linker_list_;

};
}