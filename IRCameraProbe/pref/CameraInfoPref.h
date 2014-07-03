#pragma once
#include "IRCameraBasic.h"
#include <vector>

namespace camera {
class CameraInfoPrefObserver;
class CameraInfoPref {
public:
  struct CameraInfo {
    TString ip_addr;
    TString name;
  };
  CameraInfoPref();
  int GetCameraCount() const;

  const CameraInfo&   GetCameraAt(int index) const;

  void  PushBack(const TString& ip_addr, const TString& name);
  void  InsertAt(int pos, const TString& ip_addr, const TString& name);
  void  ReplaceAt(int pos, const TString& ip_addr, const TString& name);
  void  Clear();

  void  AddObserver(CameraInfoPrefObserver* observer);
  void  RemoveObserver(CameraInfoPrefObserver* observer);
  //when the camera info preference has changed completely
  void  CameraInfoPrefChanged();

  static CameraInfoPref* GetInstance();
private:
  std::vector<CameraInfo> camera_infos_;
  std::vector<CameraInfoPrefObserver*>  observers_;
};
}