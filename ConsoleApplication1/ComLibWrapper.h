#pragma once

enum class ComLibConnectionStatus : int {
	None = 0,
	/*CableDisconnected,*/
	SessionConnectedAndStandby,
	SessionConnectedAndActive,
	SessionDisconnected,
};

typedef void(*ComLibStatusCallbackPtr)(ComLibConnectionStatus status);

class IComLibWrapper {
public:
	virtual bool RequestSupport() = 0;
	virtual bool StopSupport() = 0;
	virtual void SetStatusCallback(ComLibStatusCallbackPtr callbackPtr) = 0;
	virtual void RemoveStatusCallback() = 0;
};

// dll exports

#ifdef DLL_EXPORT
	#define API_CALL __declspec(dllexport)
#else
	#define API_CALL __declspec(dllimport)
	#pragma comment(lib, "ComLibWrapperDll.lib")
#endif

API_CALL IComLibWrapper * __cdecl CreateComLibWrapper();
API_CALL void __cdecl DestroyComLibWrapper(IComLibWrapper * ptr); 

#undef API_CALL
