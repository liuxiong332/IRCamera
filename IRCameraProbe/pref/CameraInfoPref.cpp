#include "CameraInfoPref.h"
#include "CameraInfoPrefObserver.h"
#include <tchar.h>
#include <algorithm>


namespace camera {

CameraInfoPref::CameraInfoPref() {
  PushBack(_T("127.0.0.1"), _T("nimei"));
  PushBack(_T("127.0.0.2"), _T("HELLO"));
  PushBack(_T("127.0.0.3"), _T("nimei"));
  PushBack(_T("127.0.0.4"), _T("HELLO"));
}

int CameraInfoPref::GetCameraCount() const {
  return static_cast<int>(camera_infos_.size());
}

const CameraInfoPref::CameraInfo&   CameraInfoPref::GetCameraAt(int index) const {
  return camera_infos_[index];
}

void CameraInfoPref::PushBack(LPCTSTR ip_addr, LPCTSTR name) {
  camera_infos_.push_back({ ip_addr, name });
}

void  CameraInfoPref::InsertAt(int pos, LPCTSTR ip_addr, LPCTSTR name) {
  CameraInfo info = { ip_addr, name };
  camera_infos_.insert(camera_infos_.begin() + pos, info);
}

void  CameraInfoPref::ReplaceAt(int pos, LPCTSTR ip_addr, LPCTSTR name) {
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