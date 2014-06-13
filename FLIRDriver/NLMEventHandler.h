#pragma once
#include <netlistmgr.h>


class IRCameraDeviceListManager;
class ConnectivityChangeObserver;

class NLMEventHandler: public INetworkListManagerEvents 
{
public:
  NLMEventHandler(ConnectivityChangeObserver* observer);;

  STDMETHODIMP QueryInterface(REFIID riid, _COM_Outptr_ void **ppvObject);
  STDMETHODIMP_(ULONG)  AddRef();
  STDMETHODIMP_(ULONG)  Release();
  STDMETHODIMP ConnectivityChanged(NLM_CONNECTIVITY);
private:
  ULONG  ref_count_;
  ConnectivityChangeObserver*  con_change_observer_;
};

