#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装器类

// 注意:  不要修改此文件的内容。  如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CLvcamctrl 包装器类

class CLvcamctrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CLvcamctrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x61A62BE, 0x4A3, 0x4EA2, { 0x84, 0xF9, 0x2E, 0x34, 0x79, 0x88, 0x79, 0x59 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:


// 操作
public:

// _DLVCam

// Functions
//

	short Connect(short Camera, short Port, short Device, short Interface, LPCTSTR Server)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_BSTR ;
		InvokeHelper(0x1f4, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Camera, Port, Device, Interface, Server);
		return result;
	}
	short Disconnect()
	{
		short result;
		InvokeHelper(0x1f5, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	VARIANT GetCameraProperty(short Id)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1f6, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Id);
		return result;
	}
	VARIANT GetImage(short imageType)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1f7, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, imageType);
		return result;
	}
	VARIANT GetLUT(short tableType)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1f8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, tableType);
		return result;
	}
	short SetCameraProperty(short Id, VARIANT Property)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_VARIANT ;
		InvokeHelper(0x1f9, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Id, &Property);
		return result;
	}
	short SetImage(VARIANT Image)
	{
		short result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1fa, DISPATCH_METHOD, VT_I2, (void*)&result, parms, &Image);
		return result;
	}
	short DoCameraAction(short Action)
	{
		short result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1fb, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Action);
		return result;
	}
	CString GetError(short errCode)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1fc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, errCode);
		return result;
	}
	short SubmitCamCommand(LPCTSTR cmd)
	{
		short result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1fd, DISPATCH_METHOD, VT_I2, (void*)&result, parms, cmd);
		return result;
	}
	float ToTemperature(long Raw, float Eps)
	{
		float result;
		static BYTE parms[] = VTS_I4 VTS_R4 ;
		InvokeHelper(0x1fe, DISPATCH_METHOD, VT_R4, (void*)&result, parms, Raw, Eps);
		return result;
	}
	VARIANT GetAbsLUT(short tableType)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1ff, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, tableType);
		return result;
	}
	VARIANT GetObjLUT(short tableType)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x200, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, tableType);
		return result;
	}
	VARIANT GetImages(short imageType, VARIANT imageArray, long imageSize)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 VTS_VARIANT VTS_I4 ;
		InvokeHelper(0x201, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, imageType, &imageArray, imageSize);
		return result;
	}
	VARIANT MLGetImages(short imageType, short imageWidth, short imageHeight, short Image)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0x202, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, imageType, imageWidth, imageHeight, Image);
		return result;
	}
	VARIANT EmissCalc(long x, long y, float knownTemp)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R4 ;
		InvokeHelper(0x203, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, x, y, knownTemp);
		return result;
	}
	short SetEmissMap(VARIANT Map)
	{
		short result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x204, DISPATCH_METHOD, VT_I2, (void*)&result, parms, &Map);
		return result;
	}
	short SetDistanceMap(VARIANT Map)
	{
		short result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x205, DISPATCH_METHOD, VT_I2, (void*)&result, parms, &Map);
		return result;
	}
	BOOL SetZoomAndPan(double zoomFactor, short panCenterX, short panCenterY)
	{
		BOOL result;
		static BYTE parms[] = VTS_R8 VTS_I2 VTS_I2 ;
		InvokeHelper(0x206, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, zoomFactor, panCenterX, panCenterY);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

CString GetVersion()
{
	CString result;
	GetProperty(0x64, VT_BSTR, (void*)&result);
	return result;
}
void SetVersion(CString propVal)
{
	SetProperty(0x64, VT_BSTR, propVal);
}


};
