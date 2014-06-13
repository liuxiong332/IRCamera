#include "stdafx.h"
#include "NLMEventHandler.h"
#include "ConnectivityChangeObserver.h"

NLMEventHandler::NLMEventHandler(ConnectivityChangeObserver* observer) {
  con_change_observer_ = observer;
  ref_count_ = 0;
}

STDMETHODIMP NLMEventHandler::QueryInterface(REFIID riid, _COM_Outptr_ void **ppvObject) {
  *ppvObject = NULL;
  if (riid == IID_IUnknown || riid == __uuidof(INetworkListManagerEvents)) {
    *ppvObject = this;
  }
  if (!*ppvObject == NULL)
    return E_NOINTERFACE;
  ((IUnknown*)*ppvObject)->AddRef();
  return S_OK;
}

STDMETHODIMP_(ULONG)  NLMEventHandler::AddRef() {
  ++ref_count_;
  return ref_count_;
}

STDMETHODIMP_(ULONG)  NLMEventHandler::Release() {
  --ref_count_;
  if (ref_count_ == 0)
    delete this;
  return ref_count_;
}
 
STDMETHODIMP NLMEventHandler::ConnectivityChanged(NLM_CONNECTIVITY connectivity) {
  con_change_observer_->OnConnectivityChanged();
  return S_OK;
}