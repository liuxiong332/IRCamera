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
 
class StateTextShow {
public:
  virtual void ShowStatusText(const TString& str) = 0;
  virtual void ShowErrorText(const TString& str) = 0;
  virtual ~StateTextShow() {}
};

class UpdateCompleteNotify {
public:
  virtual void OnUpdateComplete() = 0;
  virtual ~UpdateCompleteNotify() {}
};

class CameraImageContainerDeviceLinker: 
  public CameraDeviceObserver,
  public CameraImageContainerUIObserver,
  public StateTextShow,
  public UpdateCompleteNotify {
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
  void Sample();

  void StableSample();

  class ImageUpdate {
  public:
    void Init(StateTextShow* text_show);
    virtual bool  OnImageUpdate(CameraImageBuffer* buffer, const CameraRotationPos&) = 0;
    virtual void  OnCompleteUpdate() {}
    virtual ~ImageUpdate() {}
  protected:
    StateTextShow* status_show_;
  };

  ~CameraImageContainerDeviceLinker();
private:
 
  virtual void OnConnectButtonClicked() override;
  virtual void OnDisconnectButtonClicked() override;
  virtual void OnSampleButtonClicked() override;
   
  virtual void ShowStatusText(const TString& str) override;
  virtual void ShowErrorText(const TString& str) override;

  void  BeginRotate();
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
   

  std::unique_ptr<CameraImageContainerUI> container_ui_;
  std::shared_ptr<CameraDevice> camera_device_;
  std::unique_ptr<CameraRotator>  camera_rotator_;

  CameraRotationPos   rotation_pos_;
  DeviceStatus  device_status_;
  bool  stable_sample_is_running_;

  std::unique_ptr<RotationBufferAnalyzer> stable_buffer_analyzer_;
 
  std::unique_ptr<ImageUpdate>  running_update_;
  std::unique_ptr<ImageUpdate>  pending_stable_update_;
  std::unique_ptr<ImageUpdate>  pending_unstable_update_;
};

}