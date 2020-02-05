#include "ComLibWrapper.h"
#include <vcclr.h>

#using <COMM.dll>

// Implementation

#pragma managed

class WrapperImpl {
public:
	WrapperImpl();
	~WrapperImpl();

	void RequestSupport();
	void StopSupport();
	void SetStatusCallback(ComLibWrapper::StatusCallbackPtr callbackPtr);
	void RemoveStatusCallback();

	void RaiseStatusEvent(ComLibConnectionStatus status);
private:
	void * m_ObjPtr = nullptr;
	volatile ComLibWrapper::StatusCallbackPtr m_CallbackPtr = nullptr;
};

ref class CallbackHandler {
public:
	CallbackHandler(WrapperImpl * handlerPtr) {
		m_handlerPtr = handlerPtr;
	}

	System::Void HandleStatusCallback(COM::StatusEvent^ pStatusEvent) {
		auto status = pStatusEvent->Status;
		m_handlerPtr->RaiseStatusEvent((ComLibConnectionStatus)status);
	}
private:
	WrapperImpl * m_handlerPtr;
};

inline COM::ComLib^ get_managed(void * p) {
	return (COM::ComLib^)(*static_cast<gcroot<COM::ComLib^>*>(p));
}

WrapperImpl::WrapperImpl() {
	auto p = gcnew COM::ComLib();
	auto cbh = gcnew CallbackHandler(this);
	p->_newStatusArrived += gcnew COM::ComLib::StatusDel(cbh, &CallbackHandler::HandleStatusCallback);

	auto* managedPtr = new gcroot<COM::ComLib^>(p);
	m_ObjPtr = static_cast<void *>(managedPtr);
}

WrapperImpl::~WrapperImpl() {
	auto* managedPtr = static_cast<gcroot<COM::ComLib^>*>(m_ObjPtr);
	m_ObjPtr = nullptr;
	delete managedPtr;
}

void WrapperImpl::RequestSupport() {
	get_managed(m_ObjPtr)->RequestSupport();
}

void WrapperImpl::StopSupport() {
	get_managed(m_ObjPtr)->StopSupport();
}

void WrapperImpl::SetStatusCallback(ComLibWrapper::StatusCallbackPtr callbackPtr) {
	m_CallbackPtr = callbackPtr;
}

void WrapperImpl::RemoveStatusCallback() {
	m_CallbackPtr = nullptr;
}

void WrapperImpl::RaiseStatusEvent(ComLibConnectionStatus status) {
	if (m_CallbackPtr != nullptr) {
		m_CallbackPtr(status);
	}
}

#pragma unmanaged

// Wrapper

ComLibWrapper::ComLibWrapper() {
	impl = new WrapperImpl();
}

ComLibWrapper::~ComLibWrapper() {
	delete impl;
	impl = nullptr;
}

void ComLibWrapper::RequestSupport() {
	((WrapperImpl*)impl)->RequestSupport();
}

void ComLibWrapper::StopSupport() {
	((WrapperImpl*)impl)->StopSupport();
}

void ComLibWrapper::SetStatusCallback(StatusCallbackPtr callbackPtr) {
	((WrapperImpl*)impl)->SetStatusCallback(callbackPtr);
}

void ComLibWrapper::RemoveStatusCallback() {
	((WrapperImpl*)impl)->RemoveStatusCallback();
}
