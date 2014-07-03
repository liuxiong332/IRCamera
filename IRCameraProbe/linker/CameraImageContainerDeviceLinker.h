#pragma once
#include "core/CameraDeviceObserver.h"
#include "ui/CameraImageContainerUIObserver.h"
#include <wtypes.h>
#include <memory>

class CameraImageContainerUI;
class CameraImageBuffer;

namespace camera {
class CameraDevice;
class CameraImageContainerDeviceLinker: 
  public CameraDeviceObserver,
  public CameraImageContainerUIObserver {
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
private:
  virtual void OnConnectButtonClicked() override;
  virtual void OnDisconnectButtonClicked() override;
  virtual void OnSampleButtonClicked() override;

  //when the camera has init completely
  virtual void  OnInitCamera() override;
  //when connect to the camera successfully
  virtual void  OnConnect() override;
  //when the host has disconnect from the camera
  virtual void  OnDisconnect()  override;
  //when the image has update
  virtual void  OnImageUpdate() override;

  void ImageBufferUpdate(CameraImageBuffer*);

  std::unique_ptr<CameraImageContainerUI> container_ui_;
  std::unique_ptr<CameraDevice> camera_device_;
  DeviceStatus  device_status_;
};

}