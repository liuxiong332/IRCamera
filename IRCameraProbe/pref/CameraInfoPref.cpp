#include "CameraInfoPref.h"
#include "CameraInfoPrefObserver.h"
#include <tchar.h>
#include <algorithm>
#include <utility>

namespace camera {

CameraInfoPref::CameraInfoPref() {
}

int CameraInfoPref::GetCameraCount() const {
  return static_cast<int>(camera_infos_.size());
}

const CameraInfoPref::CameraInfo&   CameraInfoPref::GetCameraAt(int index) const {
  return camera_infos_[index];
}

void CameraInfoPref::PushBack(const TString& ip_addr, const TString& name) {
  CameraInfo info = { ip_addr, name };
  camera_infos_.push_back( std::move(info));
}

void  CameraInfoPref::InsertAt(int pos, const TString& ip_addr, const TString& name) {
  CameraInfo info = { ip_addr, name };
  camera_infos_.insert(camera_infos_.begin() + pos, std::move(info));
}

void  CameraInfoPref::ReplaceAt(int pos, const TString& ip_addr, const TString& name) {
  camera_infos_[pos] = { ip_addr, name };
}

void CameraInfoPref::Clear() {
  camera_infos_.clear();
}

void CameraInfoPref::AddObserver(CameraInfoPrefObserver* observer) {
  observers_.push_back(observer);
}

void CameraInfoPref::RemoveObserver(CameraInfoPrefObserver* observer) {
  auto iter = std::find(observers_.begin(), observers_.end(), observer);
  observers_.erase(iter);
}

void CameraInfoPref::CameraInfoPrefChanged() {
  std::for_each(observers_.begin(), observers_.end(), [](CameraInfoPrefObserver* observer) {
    observer->CameraInfoPrefChanged();
  });
}

CameraInfoPref* CameraInfoPref::GetInstance() {
  static CameraInfoPref kInfPref;
  return &kInfPref;
}

}