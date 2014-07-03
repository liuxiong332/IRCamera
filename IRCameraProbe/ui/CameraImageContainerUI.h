#pragma once
#include "IRCameraBasic.h"
#include <memory>

namespace DuiLib {
class CContainerUI;
class CLabelUI;
class CButtonUI;
}

class CameraImageUI;
class CameraImageContainerUIObserver;

class CameraImageContainerUI {
public:
  CameraImageContainerUI();
  explicit CameraImageContainerUI(DuiLib::CContainerUI* container_ui);

  void Init(DuiLib::CContainerUI* container_ui);
  void SetObserver(CameraImageContainerUIObserver* observer) {
    observer_ = observer;
  }

  //set status label text
  void SetStatusText(LPCTSTR  status_text);
  void ShowStatus(bool is_show);

  //set error status label text
  void SetErrorStatusText(LPCTSTR status_text);
  void ShowErrorStatus(bool is_show);

  bool IsSampleButtonVisible() const;
  //show or hide the sample button
  void ShowSampleButton(bool is_show);

  void EnableConnectButton(bool is_enable);
  void EnableDisconnectButton(bool is_enable);
  void EnableSampleButton(bool is_enable);

  DuiLib::CContainerUI* GetUnderlyingControl() {
    return container_ui_.get();
  }
  CameraImageUI*  GetCameraImageUI();
private:
  bool OnConnectButtonClick(void* param);
  bool OnDisconnectButtonClick(void* param);
  bool OnSampleButtonClick(void* param);

  CameraImageContainerUIObserver* observer_;

  std::unique_ptr<DuiLib::CContainerUI> container_ui_;
  DuiLib::CLabelUI*   camera_status_label_;
  DuiLib::CLabelUI*   camera_error_label_;

  DuiLib::CButtonUI*  connect_btn_;
  DuiLib::CButtonUI*  sample_btn_;
  DuiLib::CButtonUI*  disconnect_btn_;
};