#include "CameraImageContainerUI.h"
#include "CameraImageUI.h"
#include "UIlib.h"
#include <wtypes.h>

namespace {

DuiLib::CControlUI* FindSubControlByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
  return parent_ui->GetManager()->FindSubControlByName(parent_ui, name);
}

DuiLib::CLabelUI* FindSubLabelByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
  return static_cast<DuiLib::CLabelUI*>(FindSubControlByName(parent_ui, name));
}

DuiLib::CButtonUI*  FindSubButtonByName(DuiLib::CContainerUI* parent_ui, LPCTSTR name) {
  return static_cast<DuiLib::CButtonUI*>(FindSubControlByName(parent_ui, name));
}

}

CameraImageContainerUI::CameraImageContainerUI() :container_ui_(NULL) {
}

CameraImageContainerUI::CameraImageContainerUI(DuiLib::CContainerUI* container_ui) {
  Init(container_ui);
}

void CameraImageContainerUI::Init(DuiLib::CContainerUI* container_ui) {
  container_ui_ = container_ui;
  camera_status_label_ = FindSubLabelByName(container_ui_, _T("camera_status_label"));
  camera_error_label_ = FindSubLabelByName(container_ui_, _T("camera_error_label"));
  sample_btn_ = FindSubButtonByName(container_ui_, _T("sample_btn"));
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

CameraImageUI*  CameraImageContainerUI::GetCameraImageUI() {
  return static_cast<CameraImageUI*>(FindSubControlByName(container_ui_, _T("camera_image")));
}