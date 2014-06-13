#include "stdafx.h"
#include "NLMHelper.h"
#include <netlistmgr.h>
#include "NLMEventHandler.h"
#include <afxctl.h>

NLMHelper::NLMHelper(ConnectivityChangeObserver* observer) {
  nlm_.CoCreateInstance(CLSID_NetworkListManager);
  event_sink_ = new NLMEventHandler(observer);
  AfxConnectionAdvise(nlm_, __uuidof(INetworkListManagerEvents), event_sink_, FALSE, &cookie_);
}


NLMHelper::~NLMHelper()
{
  AfxConnectionUnadvise(nlm_, IID_INetworkListManagerEvents, event_sink_, FALSE, cookie_);
  event_sink_->Release();
}

void  AddNetworkInfo(NetworkInfos* networks, INetwork* net) {
  NetworkInfo info;

  NLM_CONNECTIVITY nlm_connectivity;
  net->GetConnectivity(&nlm_connectivity);
  switch (nlm_connectivity) {
  case NLM_CONNECTIVITY_DISCONNECTED:  
    info.connectivity = NetworkInfo::DISCONNECTED;  break;
  case NLM_CONNECTIVITY_IPV4_LOCALNETWORK:
    info.connectivity = NetworkInfo::IPV4_LOCALNETWORK; break;
  case NLM_CONNECTIVITY_IPV4_INTERNET:
    info.connectivity = NetworkInfo::IPV4_INTERNET; break;
  default:
    info.connectivity = NetworkInfo::DISCONNECTED;
  }
  
  _bstr_t name;
  net->GetName(name.GetAddress());
  info.name = name;
  networks->push_back(info);
}

NLMHelper::StatusCode  NLMHelper::GetNetworkInfo(NetworkInfos* networks) {
  IEnumNetworks*  net_enum = NULL;
  if (nlm_ == NULL)
    return FAIL;

  HRESULT hr = nlm_->GetNetworks(NLM_ENUM_NETWORK_CONNECTED, &net_enum);
  if (hr == S_OK) {
    DWORD  ret_val;
    while (true) {
      CComPtr<INetwork> network;
      hr = net_enum->Next(1, &network, &ret_val);
      if (hr == S_OK && ret_val > 0) {
        AddNetworkInfo(networks, network);
      } else {
        return SUCCESS;
      }
    }
  }
  return FAIL;
}
