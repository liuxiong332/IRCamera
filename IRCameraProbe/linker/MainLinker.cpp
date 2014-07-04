#include "MainLinker.h"
#include "CameraImageLinkerList.h"
#include "CameraPrefLinker.h"
#include "CameraInfoListLinker.h"
#include "SampleModeLinker.h"

#include "ui/CameraInfoPrefLayout.h"
#include "ui/CameraInfoListUI.h"
#include "ui/CameraImageContainerUI.h"
#include "ui/CameraInfoUI.h"

#include "ui/MainWindow.h"
#include "ui/CameraImageListUI.h"
#include "ui/CameraPrefLayoutUI.h"

#include "linker/CameraImageContainerDeviceLinker.h"

namespace camera {
void MainLinker::Init() {
  main_window_.reset(new MainWindow);
  main_window_->ShowWindow();
  linker_list_.reset(new CameraImageLinkerList);
  linker_list_->Init(main_window_.get() , main_window_->GetCameraImageListUI());

  camera_pref_linker_.reset(new CameraPrefLinker);
  camera_pref_linker_->Init(main_window_->GetCameraPrefLayoutUI());
}
}