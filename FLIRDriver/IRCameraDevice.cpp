#include "stdafx.h"
#include "IRCameraDevice.h"
#include "IRCameraDeviceImpl.h"
#include "IRCameraDeviceMock.h"

IRCameraDevice* IRCameraDevice::Create() {
  return new IRCameraDeviceImpl;
}

IRCameraDevice* IRCameraDevice::CreateForTest() {
  return new IRCameraDeviceMock;
}

void IRCameraDevice::SetName(const TString& name) {
  dev_name_ = name;
}

void IRCameraDevice::SetIPAddr(DWORD addr) {
  dev_ip_addr_.Set((BYTE*)&addr, sizeof(addr));
}

void IRCameraDevice::SetIPAddr(const ByteAddr& addr) {
  dev_ip_addr_ = addr;
}

const TString& IRCameraDevice::GetName() const {
  return dev_name_;
}

const ByteAddr& IRCameraDevice::GetIPAddr() const {
  return dev_ip_addr_;
}

void IRCameraDevice::SetPhysicalAddr(BYTE* addr, int len) {
  physical_addr_.Set(addr, len);
}

void IRCameraDevice::SetPhysicalAddr(const ByteAddr& addr) {
  physical_addr_ = addr;
}

const ByteAddr& IRCameraDevice::GetPhysicalAddr() const {
  return physical_addr_;
}

