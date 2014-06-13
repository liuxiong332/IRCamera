#pragma  once
#include <vector>

#include "IRCameraBasic.h"
#include "ConnectivityChangeObserver.h"
#include "ByteAddr.h"

class IRCameraDevice;
class IRCameraDeviceListManagerObserver;
class NLMHelper;

class IRCAMERA_API IRCameraDeviceListManager: public ConnectivityChangeObserver {
public:
  typedef std::vector<IRCameraDevice*>  DeviceList;
  IRCameraDeviceListManager();
  ~IRCameraDeviceListManager();
  /**
   * force to retrieve the device list 
   */
  void  FlushDeviceList();

  const DeviceList&  GetDeviceList() const;

  /** 
   * add and remove the observer of device list
   */
  void  AddObserver(IRCameraDeviceListManagerObserver* observer);
  void  RemoveObserver(IRCameraDeviceListManagerObserver* observer);
private:
  bool  FindInDeviceList(const ByteAddr& addr);

  void  AddIpPhysicalAddr(const ByteAddr& physical_addr, const ByteAddr& ip_addr);
  virtual void  OnConnectivityChanged();

  typedef std::vector<IRCameraDeviceListManagerObserver*>  ObserverList;
  ObserverList  observers_;
  DeviceList    device_list_;

  NLMHelper*     nlm_helper_;
};