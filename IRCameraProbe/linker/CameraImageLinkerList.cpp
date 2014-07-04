#include "CameraImageLinkerList.h"
#include "ui/CameraImageListUI.h"
#include "CameraImageContainerDeviceLinker.h"
#include "ui/CameraImageBuilder.h"

#include "pref/CameraInfoPref.h"
#include "pref/SampleModePref.h"

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
  CameraInfoPref::GetInstance()->AddObserver(this);
  ReloadCameraList();

  InitSampleModePref();
}

CameraImageLinkerList::~CameraImageLinkerList() {
  CameraInfoPref::GetInstance()->RemoveObserver(this);
}

void CameraImageLinkerList::ReloadCameraList()
{
  CameraInfoPref* pref = CameraInfoPref::GetInstance();
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

void  CameraImageLinkerList::OnTimer() {
  std::for_each(device_linker_list_.begin(), device_linker_list_.end(), 
    [](const ImageDeviceLinkerPtr& linker) {
    linker->Sample();
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
  SampleModePref* pref = SampleModePref::GetInstance();
  pref->InitWithObserver(this);
  OnSampleModeChanged(pref->GetSampleMode());
  OnTimeDeltaChanged(pref->GetSampleInterval());
}

void CameraImageLinkerList::OnSampleModeChanged(SampleMode mode) {
  if (mode == AUTO_MODE) {
    list_ui_->BeginTimer(SampleModePref::GetInstance()->GetSampleInterval());
  } else {
    list_ui_->EndTimer();
  }
}

void CameraImageLinkerList::OnTimeDeltaChanged(const TimeDelta& delta) {
  if (SampleModePref::GetInstance()->GetSampleMode() == AUTO_MODE) {
    list_ui_->EndTimer();        //reinitialize the timer
    list_ui_->BeginTimer(delta);
  }
}

}