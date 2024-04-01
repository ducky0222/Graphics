#pragma once

#include "Source.h"
//#include "IBindable.h"
#include "BufferResource.h"
//#include "RenderTarget.h"
//#include "DepthStencil.h"

class Sink
{
public:
	Sink() = default;
	virtual ~Sink() = default;

	virtual void BindSink(Source& source) abstract;
	virtual void PostLinkValidate() const abstract;

	void SetTarget(const std::string& passName, const std::string& outputName);

protected:
	Sink(std::string registeredName);

private:
	std::string m_registeredName;
	std::string m_passName;
	std::string m_outputName;
};

template<class T>
class DirectBufferSink : public Sink
{
	static_assert(std::is_base_of_v<BufferResource, T>, "T mush be BufferResource");

public:
	DirectBufferSink(std::string registeredName, std::shared_ptr<T>& bind);
	virtual ~DirectBufferSink() = default;

	static std::unique_ptr<Sink> Make(std::string registeredName, std::shared_ptr<T>& target);

	// Sink을(를) 통해 상속됨
	void BindSink(Source& source) override;
	void PostLinkValidate() const override;

private:
	std::shared_ptr<T>& m_target = nullptr;
	bool m_isLinked = false;

};

template<class T>
void DirectBufferSink<T>::PostLinkValidate() const
{

}

template<class T>
void DirectBufferSink<T>::BindSink(Source& source)
{

}

template<class T>
std::unique_ptr<Sink> DirectBufferSink<T>::Make(std::string registeredName, std::shared_ptr<T>& target)
{

}

template<class T>
DirectBufferSink<T>::DirectBufferSink(std::string registeredName, std::shared_ptr<T>& bind)
	: Sink(std::move(registeredName)), m_target()
{

}
