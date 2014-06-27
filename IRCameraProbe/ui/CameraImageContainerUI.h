#pragma once
#include "IRCameraBasic.h"

namespace DuiLib {
class CContainerUI;
class CLabelUI;
class CButtonUI;
}

class CameraImageUI;

class CameraImageContainerUI {
public:
  CameraImageContainerUI();
  explicit CameraImageContainerUI(DuiLib::CContainerUI* container_ui);

  void Init(DuiLib::CContainerUI* container_ui);

  //set status label text
  void SetStatusText(LPCTSTR  status_text);
  void ShowStatus(bool is_show);

  //set error status label text
  void SetErrorStatusText(LPCTSTR status_text);
  void ShowErrorStatus(bool is_show);

  bool IsSampleButtonVisible() const;
  //show or hide the sample button
  void ShowSampleButton(bool is_show);

  DuiLib::CContainerUI* GetUnderlyingControl() {
    return container_ui_;
  }
  CameraImageUI*  GetCameraImageUI();
private:
  DuiLib::CContainerUI* container_ui_;
  DuiLib::CLabelUI*   camera_status_label_;
  DuiLib::CLabelUI*   camera_error_label_;
  DuiLib::CButtonUI*  sample_btn_;
};