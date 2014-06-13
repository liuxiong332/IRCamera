#include "stdafx.h"
#include "ByteAddr.h"

ByteAddr::ByteAddr(const ByteAddr& other) {
  *this = other;
}

ByteAddr& ByteAddr::operator=(const ByteAddr& other) {
  if (&other != this) {
    alloc_len_ = other.alloc_len_;
    addr_len_ = other.addr_len_;
    addr_ = (BYTE*)malloc(alloc_len_);
    memcpy(addr_, other.addr_, addr_len_);
  }
  return *this;
}

void  ByteAddr::Set(BYTE* addr, int len) {
  if (!addr_) {
    addr_ = (BYTE*)malloc(len);
    alloc_len_ = len;
  }
  else if (alloc_len_ < len) {
    addr_ = (BYTE*)realloc(addr_, len);
    alloc_len_ = len;
  }
  addr_len_ = len;
  memcpy(addr_, addr, len);
}

bool  ByteAddr::Equal(const ByteAddr& phy_addr) const {
  if (addr_len_ == phy_addr.addr_len_) {
    for (int i = 0; i < addr_len_; ++i) {
      if (addr_[i] != phy_addr.addr_[i])
        return false;
    }
    return true;
  }
  return false;
}

TString ByteAddr::ConvertToIPString() const {
  TCHAR sz[100];
  swprintf(sz, 100, _T("%d.%d.%d.%d"), addr_[0], addr_[1], addr_[2], addr_[3]);
  return TString(sz);
}