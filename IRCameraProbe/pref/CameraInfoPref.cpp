#include "CameraInfoPref.h"
#include "CameraInfoPrefObserver.h"
#include "common/StringConversion.h"
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
 

const static char kCameraInfoStr[] = "camera_info";
const static char kCameraAddr[] = "IP";
const static char  kCameraName[] = "Name";

void  CameraInfoPref::SerializeFromJson(rapidjson::Document& document) {
  camera_infos_.clear();
  if (document.HasMember(kCameraInfoStr)) {
    rapidjson::Value& values = document[kCameraInfoStr];
    if (!values.IsArray())
      return;
    for (rapidjson::SizeType i = 0; i < values.Size(); ++i) {
      rapidjson::Value& element = values[i];
      if (element.HasMember(kCameraAddr) && element.HasMember(kCameraName)) {
        PushBack(StringConversion::UTF8ToUTF16(element[kCameraAddr].GetString()), 
          StringConversion::UTF8ToUTF16(element[kCameraName].GetString()));
      }
    }
  }
}

void  CameraInfoPref::SerializeToJson(rapidjson::Document& document) {
  if (document.HasMember(kCameraInfoStr))
    document.RemoveMember(kCameraInfoStr);
  rapidjson::Value elements(rapidjson::kArrayType);
  std::for_each(camera_infos_.begin(), camera_infos_.end(), [&elements, &document](const CameraInfo& info) {
    rapidjson::Value element(rapidjson::kObjectType);
    element.AddMember(rapidjson::StringRef(kCameraAddr), 
      rapidjson::Value(StringConversion::UTF16ToUTF8(info.ip_addr.c_str()).c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    element.AddMember(rapidjson::StringRef(kCameraName), 
      rapidjson::Value(StringConversion::UTF16ToUTF8(info.name.c_str()).c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    elements.PushBack(element, document.GetAllocator());
  });
  document.AddMember(rapidjson::StringRef(kCameraInfoStr), elements, document.GetAllocator());
}
}