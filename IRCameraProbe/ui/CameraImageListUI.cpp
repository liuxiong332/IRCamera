#include "CameraImageListUI.h"
#include "CameraImageContainerUI.h"
#include <assert.h>
#include <UIlib.h>

CameraImageListUI::CameraImageListUI() : image_list_ui_(NULL) {
}

void CameraImageListUI::Init(DuiLib::CContainerUI* container_ui) {
  image_list_ui_ = container_ui;
}

CameraImageContainerUI* CameraImageListUI::GetItemAt(int i) {
  assert(i < GetLength());
  DuiLib::CContainerUI* ui = static_cast<DuiLib::CContainerUI*>(image_list_ui_->GetItemAt(i));
  auto iter = camera_image_ui_map_.find(ui);
  if (iter == camera_image_ui_map_.end()) {
    iter = camera_image_ui_map_.insert(std::make_pair(ui,
      ScopedCameraImageContainerUIPtr(new CameraImageContainerUI(image_list_ui_)))).first;
  }
  return iter->second.get();
}
 

int CameraImageListUI::GetLength() const {
  return image_list_ui_->GetCount();
}

void CameraImageListUI::PushBack(CameraImageContainerUI* container_ui) {
  DuiLib::CContainerUI* control = container_ui->GetUnderlyingControl();
  assert(camera_image_ui_map_.find(control) == camera_image_ui_map_.end());
  image_list_ui_->Add(control);
  camera_image_ui_map_.insert(std::make_pair(control, ScopedCameraImageContainerUIPtr(container_ui)));
}

void CameraImageListUI::InsertAt(int i, CameraImageContainerUI* container_ui) {
  DuiLib::CContainerUI* control = container_ui->GetUnderlyingControl();
  assert(camera_image_ui_map_.find(control) == camera_image_ui_map_.end());
  image_list_ui_->AddAt(control, i);
  camera_image_ui_map_.insert(std::make_pair(control, ScopedCameraImageContainerUIPtr(container_ui)));
}

void CameraImageListUI::Remove(CameraImageContainerUI* container_ui) {
  DuiLib::CContainerUI* control = container_ui->GetUnderlyingControl();
  auto iter = camera_image_ui_map_.find(control);
  if (iter != camera_image_ui_map_.end()) {
    camera_image_ui_map_.erase(iter);
  }
  image_list_ui_->Remove(control);
}

void CameraImageListUI::RemoveAt(int i) {
  DuiLib::CContainerUI* control = static_cast<DuiLib::CContainerUI*>(image_list_ui_->GetItemAt(i));
  auto iter = camera_image_ui_map_.find(control);
  if (iter != camera_image_ui_map_.end()) {
    camera_image_ui_map_.erase(iter);
  }
  image_list_ui_->RemoveAt(i);
}