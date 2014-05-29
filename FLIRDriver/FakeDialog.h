#pragma once
#include "lvcamctrl.h"
#include "resource.h"
#include "IRCameraDriver.h"
// FakeDialog 对话框

class FakeDialogEventObserver {
public:
  virtual void OnEvent(int event_type) = 0;
  virtual ~FakeDialogEventObserver()  {}
};

class FakeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FakeDialog)

public:
	FakeDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FakeDialog();

// 对话框数据
	enum { IDD = IDD_FORMVIEW };

  void  SetEventHandler(FakeDialogEventObserver* observer);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

  void OnCameraEvent(long Id);

	DECLARE_MESSAGE_MAP()
  DECLARE_EVENTSINK_MAP()
public:
  FakeDialogEventObserver*  event_observer;
  CLvcamctrl camera_control;
};
