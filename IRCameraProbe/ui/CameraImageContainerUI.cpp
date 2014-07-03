#include "CameraImageContainerUI.h"
#include "CameraImageUI.h"
#include "CameraImageContainerUIObserver.h"
#include "CommonUIOperator.h"
#include "UIlib.h"
#include <wtypes.h>
 
CameraImageContainerUI::CameraImageContainerUI() : observer_(NULL) {
}

CameraImageContainerUI::CameraImageContainerUI(DuiLib::CContainerUI* container_ui) {
  Init(container_ui);
}

void CameraImageContainerUI::Init(DuiLib::CContainerUI* container_ui) {
  container_ui_.reset(container_ui);
  camera_status_label_ = CommonUIOperator::FindSubLabelByName(container_ui_.get(), _T("camera_status_label"));
  camera_error_label_ = CommonUIOperator::FindSubLabelByName(container_ui_.get(), _T("camera_error_label"));

  sample_btn_ = CommonUIOperator::FindSubButtonByName(container_ui_.get(), _T("sample_btn"));
  sample_btn_->OnNotify += DuiLib::MakeDelegate(this, &CameraImageContainerUI::OnSampleButtonClick);

  connect_btn_ = CommonUIOperator::FindSubButtonByName(container_ui_.get(), _T("connect_btn"));
  connect_btn_->OnNotify += DuiLib::MakeDelegate(this, &CameraImageContainerUI::OnConnectButtonClick);

  disconnect_btn_ = CommonUIOperator::FindSubButtonByName(container_ui_.get(), _T("disconnect_btn"));
  disconnect_btn_->OnNotify += DuiLib::MakeDelegate(this, &CameraImageContainerUI::OnDisconnectButtonClick);
}
//set status label text
void CameraImageContainerUI::SetStatusText(LPCTSTR status_text) {
  camera_status_label_->SetText(status_text);
}
void CameraImageContainerUI::ShowStatus(bool is_show) {
  camera_status_label_->SetVisible(is_show);
}

//set error status label text
void CameraImageContainerUI::SetErrorStatusText(LPCTSTR status_text) {
  camera_error_label_->SetText(status_text);
}
void CameraImageContainerUI::ShowErrorStatus(bool is_show) {
  camera_error_label_->SetVisible(is_show);
}

bool CameraImageContainerUI::IsSampleButtonVisible() const {
  return sample_btn_->IsVisible();
}
//show or hide the sample button
void CameraImageContainerUI::ShowSampleButton(bool is_show) {
  sample_btn_->SetVisible(is_show);
}

void CameraImageContainerUI::EnableConnectButton(bool is_enable) {
  connect_btn_->SetEnabled(is_enable);
}

void CameraImageContainerUI::EnableDisconnectButton(bool is_enable) {
  disconnect_btn_->SetEnabled(is_enable);
}

void CameraImageContainerUI::EnableSampleButton(bool is_enable) {
  sample_btn_->SetEnabled(is_enable);
}

CameraImageUI*  CameraImageContainerUI::GetCameraImageUI() {
  return static_cast<CameraImageUI*>(CommonUIOperator::FindSubControlByName(container_ui_.get(), _T("camera_image")));
}

bool CameraImageContainerUI::OnConnectButtonClick(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    if (observer_)
      observer_->OnConnectButtonClicked();
    return true;
  }
  return false;
}

bool CameraImageContainerUI::OnDisconnectButtonClick(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    if (observer_)
      observer_->OnDisconnectButtonClicked();
    return true;
  }
  return false;
}
bool CameraImageContainerUI::OnSampleButtonClick(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("click")) {
    if (observer_)
      observer_->OnSampleButtonClicked();
    return true;
  }
  return false;
}