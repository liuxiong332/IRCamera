#pragma  once
#include "IRCameraBasic.h"

class ByteAddr {
public:
  ByteAddr() :addr_(NULL),addr_len_(0),alloc_len_(0) {}
  
  void  Set(BYTE* addr, int len);

  ~ByteAddr() {
    free(addr_);
  }
  bool  Equal(const ByteAddr& phy_addr) const;

  TString  ConvertToIPString() const;

  ByteAddr(const ByteAddr&);
  ByteAddr& operator=(const ByteAddr&);
private:
  BYTE* addr_;
  int   addr_len_;
  int   alloc_len_;
};
