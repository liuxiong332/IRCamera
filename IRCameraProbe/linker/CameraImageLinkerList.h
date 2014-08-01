#pragma once
#include "pref/CameraInfoPrefObserver.h"
#include "pref/SampleModePrefObserver.h"
#include "pref/StableSampleTimePrefObserver.h"
#include "ui/CameraImageListUIObserver.h"
#include <vector>
#include <memory>

class CameraImageBuilder;
class CameraImageListUI;
class CameraImageContainerUI;

namespace camera {

class CameraImageContainerDeviceLinker;
class CameraInfoPref;
class CameraImageLinkerCreator;

class CameraImageLinkerList 
  : public CameraInfoPrefObserver,
    public SampleModePrefObserver,
    public StableSampleTimePrefObserver,
    public CameraImageListUIObserver {
public:
  void  Init(CameraImageBuilder*, CameraImageListUI* list_ui);

  ~CameraImageLinkerList();
private:
  virtual void OnSampleTimeChanged() override;

  virtual void OnSampleModeChanged(SampleMode) override;
  virtual void OnTimeDeltaChanged(const TimeDelta&) override;

  virtual void CameraInfoPrefChanged() override;

  virtual void  OnTimer(int id) override;

  void InitSampleModePref();
  void ReloadCameraList();
  void InsertIntoList(CameraImageContainerDeviceLinker* linker);

  CameraImageListUI*  list_ui_;
  CameraImageBuilder* camera_image_builder_;

  typedef std::unique_ptr<CameraImageContainerDeviceLinker> ImageDeviceLinkerPtr;
  //the link list is used for delete the unused device linker
  std::vector<ImageDeviceLinkerPtr>   delete_link_list_; 
  std::vector<ImageDeviceLinkerPtr>  device_linker_list_;

  const static int  kUnstableSampleTimerID = 2;
  const static int  kStableSampleTimerID = 3;
};
}