#include "CameraImageListUI.h"
#include "CameraImageContainerUI.h"
#include <assert.h>
#include <algorithm>
#include <UIlib.h>

CameraImageListUI::CameraImageListUI() : image_list_ui_(NULL) {
}

void CameraImageListUI::Init(DuiLib::CContainerUI* container_ui) {
  image_list_ui_ = container_ui;
  assert(image_list_ui_->GetCount() == 0);
  //disable auto destroy
  image_list_ui_->SetAutoDestroy(false);  
}

CameraImageContainerUI* CameraImageListUI::GetItemAt(int i) {
  assert(i < GetCount());
  return camera_image_ui_list_[i];
}
 

int CameraImageListUI::GetCount() const {
  return static_cast<int>(camera_image_ui_list_.size());
}

void CameraImageListUI::PushBack(CameraImageContainerUI* container_ui) {
  DuiLib::CContainerUI* control = container_ui->GetUnderlyingControl();
  image_list_ui_->Add(control);
  camera_image_ui_list_.push_back(container_ui);
}

void CameraImageListUI::InsertAt(int i, CameraImageContainerUI* container_ui) {
  DuiLib::CContainerUI* control = container_ui->GetUnderlyingControl();
  image_list_ui_->AddAt(control, i);
  camera_image_ui_list_.insert(camera_image_ui_list_.begin() + i, container_ui);
}

void CameraImageListUI::Remove(CameraImageContainerUI* container_ui) {
  DuiLib::CContainerUI* control = container_ui->GetUnderlyingControl();
  image_list_ui_->Remove(control);
  auto iter = std::find(camera_image_ui_list_.begin(), camera_image_ui_list_.end(), container_ui);
  if (iter != camera_image_ui_list_.end())
    camera_image_ui_list_.erase(iter);
}

void CameraImageListUI::RemoveAt(int i) {
  image_list_ui_->RemoveAt(i);
  camera_image_ui_list_.erase(camera_image_ui_list_.begin() + i);
}

void CameraImageListUI::RemoveAll() {
  image_list_ui_->RemoveAll();
  camera_image_ui_list_.clear();
}