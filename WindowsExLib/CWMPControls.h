// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装器类

#import "C:\\Windows\\System32\\wmp.dll" no_namespace
// CWMPControls 包装器类

class CWMPControls : public COleDispatchDriver
{
public:
	CWMPControls() {} // 调用 COleDispatchDriver 默认构造函数
	CWMPControls(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPControls(const CWMPControls& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IWMPControls 方法
public:
	BOOL get_isAvailable(LPCTSTR bstrItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, bstrItem);
		return result;
	}
	void play()
	{
		InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void stop()
	{
		InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void pause()
	{
		InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void fastForward()
	{
		InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void fastReverse()
	{
		InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	double get_currentPosition()
	{
		double result;
		InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, nullptr);
		return result;
	}
	void put_currentPosition(double newValue)
	{
		static BYTE parms[] = VTS_R8;
		InvokeHelper(0x38, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	CString get_currentPositionString()
	{
		CString result;
		InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	void next()
	{
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void previous()
	{
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	LPDISPATCH get_currentItem()
	{
		LPDISPATCH result;
		InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	void put_currentItem(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH;
		InvokeHelper(0x3c, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	long get_currentMarker()
	{
		long result;
		InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_currentMarker(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x3d, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	void playItem(LPDISPATCH pIWMPMedia)
	{
		static BYTE parms[] = VTS_DISPATCH;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pIWMPMedia);
	}

	// IWMPControls 属性
public:

};
