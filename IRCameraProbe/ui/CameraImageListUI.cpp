#include "CameraImageListUI.h"
#include "CameraImageContainerUI.h"
#include "CameraImageListUIObserver.h"
#include <assert.h>
#include <algorithm>
#include <UIlib.h>
#include <assert.h>

CameraImageListUI::CameraImageListUI() : image_list_ui_(NULL),observer_(NULL) {
}

void CameraImageListUI::Init(DuiLib::CContainerUI* container_ui) {
  image_list_ui_ = container_ui;
  image_list_ui_->OnNotify += DuiLib::MakeDelegate(this, &CameraImageListUI::OnTimer);
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

void CameraImageListUI::BeginTimer(const TimeDelta& delta) {
  TimeDelta tr_delta = delta;
  if (delta.ToInternalValue() == 0)
    tr_delta = TimeDelta::FromMilliseconds(50);
  image_list_ui_->GetManager()->SetTimer(image_list_ui_, kTimerID, static_cast<UINT>(tr_delta.InMilliseconds()));
}

void CameraImageListUI::EndTimer() {
  image_list_ui_->GetManager()->KillTimer(image_list_ui_, kTimerID);
}

void CameraImageListUI::BeginStableSampleTimer(const TimeDelta& delta) {
  TimeDelta tr_delta = delta;
  if (delta.ToInternalValue() == 0)
    tr_delta = TimeDelta::FromMilliseconds(50);
  image_list_ui_->GetManager()->SetTimer(image_list_ui_, kStableSampleTimerID, static_cast<UINT>(tr_delta.InMilliseconds()));
}

void CameraImageListUI::EndStableSampleTimer() {
  image_list_ui_->GetManager()->KillTimer(image_list_ui_, kStableSampleTimerID);
}

bool CameraImageListUI::OnTimer(void* param) {
  DuiLib::TNotifyUI* notify = static_cast<DuiLib::TNotifyUI*>(param);
  if (notify->sType == _T("timer")) {
    int timer_id = static_cast<int>(notify->wParam);
    if (observer_ && kTimerID == timer_id)  observer_->OnTimer();
    if (observer_ && kStableSampleTimerID == timer_id)
      observer_->OnStableSampleTimer();
    return true;
  }
  return false;
}
void CameraImageListUI::SetObserver(CameraImageListUIObserver* observer) {
  assert(observer_ == NULL);
  observer_ = observer;
}