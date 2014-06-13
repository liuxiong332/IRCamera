#pragma once
#include "IRCameraBasic.h"


#include <vector>

class INetworkListManager;
class INetworkListManagerEvents;
class ConnectivityChangeObserver;

struct NetworkInfo {
  enum Connectivity {
    DISCONNECTED, IPV4_LOCALNETWORK, IPV4_INTERNET
  };
  TString name;
  Connectivity  connectivity;
};
typedef std::vector<NetworkInfo>  NetworkInfos;
/** 
 * the helper of Network List Manager interface
 */
class NLMHelper
{
public:
  enum  StatusCode {
    SUCCESS, FAIL
  };
  NLMHelper(ConnectivityChangeObserver* observer);
  ~NLMHelper();

  StatusCode GetNetworkInfo(NetworkInfos* networks);
private:
  CComPtr<INetworkListManager>  nlm_;
  INetworkListManagerEvents*  event_sink_;
  DWORD  cookie_;
};

