#pragma once
#include "core/CameraDeviceObserver.h"
#include "core/CameraRotator.h"

#include "ui/CameraImageContainerUIObserver.h"
#include <wtypes.h>
#include <memory>

class CameraImageContainerUI;
class CameraImageBuffer;

namespace camera {
class CameraDevice;
class CameraRotator;
class RotationBufferAnalyzer;
class ImageUpdateSchedule;

class ImageUIOperator {
public:
  virtual void ShowStatusText(const TString& str) = 0;
  virtual void ShowErrorText(const TString& str) = 0;

  virtual void UpdateImageUI(CameraImageBuffer*) = 0;
  virtual void UpdateRealTimeImageUI(CameraImageBuffer*) = 0;
  virtual ~ImageUIOperator() {}
};

class UpdateCompleteNotify {
public:
  virtual void OnUpdateComplete() = 0;
  virtual ~UpdateCompleteNotify() {}
};

class DeviceOperator {
public:
  virtual void UpdateKelvinImage() = 0;
  virtual ~DeviceOperator() {}
};

class CameraImageContainerDeviceLinker: 
  public CameraDeviceObserver,
  public CameraImageContainerUIObserver,
  public ImageUIOperator,
  public UpdateCompleteNotify,
  public DeviceOperator {
public:
  enum DeviceStatus {
    UNINITIALIZED,
    INITIALIZING,
    INITIALIZED,
    CONNECTING,
    CONNECTED,
    DISCONNECTING
  };

  CameraImageContainerDeviceLinker();

  void Init(LPCTSTR ip_addr, LPCTSTR name, CameraImageContainerUI* container_ui);

  CameraImageContainerUI* GetContainerUI() {
    return container_ui_.get();
  }

  LPCTSTR  GetIPAddr() const;

  void Connect();
  void Disconnect();

  void RealTimeSample();
  void Sample();
  void StableSample();

  ~CameraImageContainerDeviceLinker();
private:
  virtual void OnConnectButtonClicked() override;
  virtual void OnDisconnectButtonClicked() override;
  virtual void OnSampleButtonClicked() override;
   
  virtual void ShowStatusText(const TString& str) override;
  virtual void ShowErrorText(const TString& str) override;

  virtual void UpdateKelvinImage() override;
  virtual void OnUpdateComplete() override;

  //when the camera has init completely
  virtual void  OnInitCamera() override;
  //when connect to the camera successfully
  virtual void  OnConnect() override;
  //when the host has disconnect from the camera
  virtual void  OnDisconnect()  override;
  //when the image has update
  virtual void  OnImageUpdate(CameraImageBuffer*) override;
  virtual void  OnImageUpdateFail(IRCameraStatusCode code) override;
   
  virtual void UpdateImageUI(CameraImageBuffer*) override;
  virtual void UpdateRealTimeImageUI(CameraImageBuffer*) override;

  void  BeginRealTimeSample();
  void  EndRealTimeSample();
  bool  OnTimer(void* param);

  std::unique_ptr<CameraImageContainerUI> container_ui_;
  std::shared_ptr<CameraDevice> camera_device_;
  
  std::unique_ptr<CameraRotator>  camera_rotator_;

  DeviceStatus  device_status_;
 
  std::unique_ptr<RotationBufferAnalyzer> stable_buffer_analyzer_;
 
  std::unique_ptr<ImageUpdateSchedule>  running_update_;
  std::unique_ptr<ImageUpdateSchedule>  realtime_update_;
  std::unique_ptr<ImageUpdateSchedule>  pending_stable_update_;
  std::unique_ptr<ImageUpdateSchedule>  pending_unstable_update_;
};

}