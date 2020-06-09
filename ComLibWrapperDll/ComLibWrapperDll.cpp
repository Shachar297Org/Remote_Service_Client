#include <vcclr.h>

#include "ComLibWrapper.h"

#using <COMM.dll>
#using <Interfaces.dll>

// Implementation

#pragma managed

class WrapperImpl : public IComLibWrapper {
public:
	WrapperImpl();
	~WrapperImpl();

	virtual bool RequestSupport() override;
	virtual bool StopSupport() override;
	virtual void SetStatusCallback(ComLibStatusCallbackPtr callbackPtr) override;
	virtual void RemoveStatusCallback() override;

	void RaiseStatusEvent(ComLibConnectionStatus status);
private:
	gcroot<COMM::ComLib^> * m_ObjPtr = nullptr;
	volatile ComLibStatusCallbackPtr m_CallbackPtr = nullptr;
};

ref class CallbackHandler {
public:
	CallbackHandler(WrapperImpl * handlerPtr) {
		m_handlerPtr = handlerPtr;
	}

	System::Void HandleStatusCallback(COMM::StatusEvent^ pStatusEvent) {
		auto status = pStatusEvent->Status;
		m_handlerPtr->RaiseStatusEvent((ComLibConnectionStatus)status);
	}
private:
	WrapperImpl * m_handlerPtr;
};

inline COMM::ComLib^ get_managed(void * p) {
	return (COMM::ComLib^)(*static_cast<gcroot<COMM::ComLib^>*>(p));
}

WrapperImpl::WrapperImpl() {
	auto p = gcnew COMM::ComLib();
	auto cbh = gcnew CallbackHandler(this);
	p->_newStatusArrived += gcnew COMM::ComLib::StatusDel(cbh, &CallbackHandler::HandleStatusCallback);
	m_ObjPtr = new gcroot<COMM::ComLib^>(p);
}

WrapperImpl::~WrapperImpl() {
	delete m_ObjPtr;
	m_ObjPtr = nullptr;
}

bool WrapperImpl::RequestSupport() {
	{
		try
		{
			return (*m_ObjPtr)->RequestSupport();
		}
		catch (...)
		{
			return false;
		}
	}
	
}

bool WrapperImpl::StopSupport()
{
	try
	{
		return (*m_ObjPtr)->StopSupport();
	}
	catch (...)
	{
		return false;
	}
}

void WrapperImpl::SetStatusCallback(ComLibStatusCallbackPtr callbackPtr) {
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

IComLibWrapper * CreateComLibWrapper() {
	return new WrapperImpl();
}
void DestroyComLibWrapper(IComLibWrapper * ptr) {
	delete ptr;
}
