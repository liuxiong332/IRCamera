#include "CameraImageLinkerList.h"
#include "ui/CameraImageListUI.h"
#include "CameraImageContainerDeviceLinker.h"
#include "ui/CameraImageBuilder.h"

#include "pref/CameraPref.h"
#include "common/Time.h"

#include "IRCameraBasic.h"
#include <map>
#include <memory>
#include <utility>
#include <assert.h>
#include <algorithm>

namespace camera {

CameraImageContainerDeviceLinker* CreateNewLinker(LPCTSTR ip_addr, LPCTSTR name, CameraImageContainerUI* ui) {
  CameraImageContainerDeviceLinker* linker = new CameraImageContainerDeviceLinker;
  linker->Init(ip_addr, name, ui);
  return linker;
}
////////////////////////////////////////CameraImageLinkerList/////////////////////////////////////////////
void CameraImageLinkerList::Init(CameraImageBuilder* builder, CameraImageListUI* list_ui) {
  list_ui_ = list_ui;
  list_ui->SetObserver(this);

  camera_image_builder_ = builder;
  assert(list_ui->GetCount() == 0);
  CameraPref::GetInstance()->GetCameraInfoPref()->AddObserver(this);
  ReloadCameraList();

  InitSampleModePref();
}

CameraImageLinkerList::~CameraImageLinkerList() {
  CameraPref::GetInstance()->GetCameraInfoPref()->RemoveObserver(this);
}

void CameraImageLinkerList::ReloadCameraList()
{
  CameraInfoPref* pref = CameraPref::GetInstance()->GetCameraInfoPref();
  for (int i = 0; i < pref->GetCameraCount(); ++i) {
    const CameraInfoPref::CameraInfo& camera_info = pref->GetCameraAt(i);
    
    //in the delete link list find the linker that IP is the same
    auto iter = std::find_if(delete_link_list_.begin(), delete_link_list_.end(), 
      [&camera_info](const ImageDeviceLinkerPtr& linker) {
      return linker->GetIPAddr() == camera_info.ip_addr;
    });
    CameraImageContainerDeviceLinker* linker; 
    if (iter == delete_link_list_.end()) {
      //not found in the origin list, create a new linker
      CameraImageContainerUI* container_ui = camera_image_builder_->CreateCameraImageContainerUI();
      linker = CreateNewLinker(camera_info.ip_addr.c_str(), camera_info.name.c_str(), container_ui);
    } else {
      linker = iter->release();
      delete_link_list_.erase(iter);  //reuse the origin linker
    }
    InsertIntoList(linker);
  }
}

void  CameraImageLinkerList::OnSampleTimeChanged() {
  StableSampleTimePref* pref = CameraPref::GetInstance()->GetStableSampleTime();
  Time time = Time::GetCurrentTime();
  Time end_time(24, 0, 0);
  Time next_day_time(static_cast<short>(pref->GetSampleHour()), 0, 0);
  long long milliseconds = end_time.HourMinuteSecondToMilli() - time.HourMinuteSecondToMilli() +
    next_day_time.HourMinuteSecondToMilli();
  assert(milliseconds < 0x7FFFFFFF);
  list_ui_->BeginTimer(kStableSampleTimerID, TimeDelta::FromMilliseconds(milliseconds));
}

void  CameraImageLinkerList::OnTimer(int id) {
  void (CameraImageContainerDeviceLinker::* sample_func)();
  switch (id) {
//   case kRealTimeSampleTimerID:  sample_func = CameraImageContainerDeviceLinker::RealTimeSample; break;
  case kStableSampleTimerID:    sample_func = &CameraImageContainerDeviceLinker::StableSample;   break;
  case kUnstableSampleTimerID:  sample_func = &CameraImageContainerDeviceLinker::Sample;         break;
  default:  sample_func = NULL;
  }
  std::for_each(device_linker_list_.begin(), device_linker_list_.end(), 
    [sample_func](const ImageDeviceLinkerPtr& linker) {
    (linker.get()->*sample_func)();
  });
}
 

void CameraImageLinkerList::InsertIntoList(CameraImageContainerDeviceLinker* linker) {
  list_ui_->PushBack(linker->GetContainerUI());
  device_linker_list_.push_back(ImageDeviceLinkerPtr(linker));
}

void CameraImageLinkerList::CameraInfoPrefChanged() {
  std::swap(delete_link_list_, device_linker_list_);
  list_ui_->RemoveAll();
  ReloadCameraList();
}

void CameraImageLinkerList::InitSampleModePref()
{
  SampleModePref* pref = CameraPref::GetInstance()->GetSampleMode();
  pref->InitWithObserver(this);
  OnSampleModeChanged(pref->GetSampleMode());
  OnTimeDeltaChanged(pref->GetSampleInterval());
}

void CameraImageLinkerList::OnSampleModeChanged(SampleMode mode) {
  if (mode == AUTO_MODE) {
    list_ui_->BeginTimer(kUnstableSampleTimerID, CameraPref::GetInstance()->GetSampleMode()->GetSampleInterval());
  } else {
    list_ui_->EndTimer(kUnstableSampleTimerID);
  }
}

void CameraImageLinkerList::OnTimeDeltaChanged(const TimeDelta& delta) {
  if (CameraPref::GetInstance()->GetSampleMode()->GetSampleMode() == AUTO_MODE) {
    list_ui_->EndTimer(kUnstableSampleTimerID);        //reinitialize the timer
    list_ui_->BeginTimer(kUnstableSampleTimerID, delta);
  }
}

}