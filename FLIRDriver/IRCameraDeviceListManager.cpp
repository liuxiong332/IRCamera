#include "stdafx.h"

#include <Ipmib.h>

#include "IRCameraDeviceListManager.h"
#include "NLMHelper.h"
#include "ByteAddr.h"
#include "IRCameraDevice.h"


class IpNetTable {
public:
  typedef std::vector<std::pair<ByteAddr, ByteAddr> >  Table;
  static Table GetIpPhysicalAddrTable();
};

IpNetTable::Table IpNetTable::GetIpPhysicalAddrTable() {
  Table  table;
  MIB_IPNETTABLE*  net_table;
  ULONG tablesize = 0;
  DWORD ret = GetIpNetTable(NULL, &tablesize, TRUE);
  if (ret == ERROR_INSUFFICIENT_BUFFER) {
    net_table = (MIB_IPNETTABLE*)malloc(tablesize);
    ret = GetIpNetTable(net_table, &tablesize, TRUE);

    if (ret == NO_ERROR) {
      for (int i = 0; i < (int)net_table->dwNumEntries; ++i) {
        if (net_table->table[i].dwType == MIB_IPNET_TYPE_DYNAMIC) {
          ByteAddr  physical_addr, ip_addr;
          physical_addr.Set(net_table->table[i].bPhysAddr, net_table->table[i].dwPhysAddrLen);
          ip_addr.Set( (BYTE*)&net_table->table[i].dwAddr, sizeof(DWORD));
          table.push_back(std::make_pair(physical_addr, ip_addr));
        }
      }
    }
    free(net_table);
  }
  return table;
}
 
IRCameraDeviceListManager::IRCameraDeviceListManager() {
  nlm_helper_ = new NLMHelper(this);
  NetworkInfos infos;
  nlm_helper_->GetNetworkInfo(&infos);
  for (NetworkInfos::iterator iter = infos.begin(); iter != infos.end(); ++iter) {

  }
  
}

IRCameraDeviceListManager::~IRCameraDeviceListManager() {
  delete nlm_helper_;
}

void IRCameraDeviceListManager::FlushDeviceList() {
  IpNetTable::Table table = IpNetTable::GetIpPhysicalAddrTable();
  for (IpNetTable::Table::iterator iter = table.begin(); iter != table.end(); ++iter) {
    
  }
}

bool  IRCameraDeviceListManager::FindInDeviceList(const ByteAddr& addr) {
  for (DeviceList::iterator iter = device_list_.begin(); iter != device_list_.end(); ++iter) {
    if ((*iter)->GetPhysicalAddr().Equal(addr))
      return true;
  }
  return false;
}

void  IRCameraDeviceListManager::AddIpPhysicalAddr(const ByteAddr& physical_addr,const ByteAddr& ip_addr) {

}