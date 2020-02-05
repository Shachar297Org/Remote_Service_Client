#pragma once

/// Duplicates ConnectionStatus enum from managed code
enum class ComLibConnectionStatus : int {
	None = 0,
	CableDisconnected,
	SessionInStandby,
	SessionIsActive,
	SessionDisconnected,
};

/// Wraps managed code
class ComLibWrapper {
public:
	typedef void (*StatusCallbackPtr)(ComLibConnectionStatus status);

	ComLibWrapper();
	~ComLibWrapper();

	void RequestSupport();
	void StopSupport();
	void SetStatusCallback(StatusCallbackPtr callbackPtr);
	void RemoveStatusCallback();
private:
	void * impl;
};
