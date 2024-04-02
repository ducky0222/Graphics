#pragma once

#include "Source.h"
#include "IBindable.h"
#include "BufferResource.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

class Sink
{
public:
	Sink() = default;
	virtual ~Sink() = default;

	virtual void BindSink(Source& source) abstract;
	virtual void PostLinkValidate() const abstract;

	void SetTarget(const std::string& passName, const std::string& outputName);

	const std::string& GetRegisteredName() const { return m_registeredName; }
	const std::string& GetPassName() const { return m_passName; }
	const std::string& GetOutputName() const { return m_outputName; }

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

#pragma region DirectBufferSinkFunc
template<class T>
void DirectBufferSink<T>::PostLinkValidate() const
{
	assert(m_isLinked);
}

template<class T>
void DirectBufferSink<T>::BindSink(Source& source)
{
	auto p = std::dynamic_pointer_cast<T>(source.YieldBuffer());
	assert(p != nullptr);

	m_target = std::move(p);
	m_isLinked = true;
}

template<class T>
std::unique_ptr<Sink> DirectBufferSink<T>::Make(std::string registeredName, std::shared_ptr<T>& target)
{
	return std::make_unique<DirectBufferSink>(std::move(registeredName), target);
}

template<class T>
DirectBufferSink<T>::DirectBufferSink(std::string registeredName, std::shared_ptr<T>& bind)
	: Sink(std::move(registeredName)), m_target(bind)
{

}
#pragma endregion

template<class T>
class ContainerBindableSink : public Sink
{
	static_assert(std::is_base_of_v<IBindable, T>, "T mush be IBindable");

public:
	ContainerBindableSink(std::string registeredName, std::vector<std::shared_ptr<IBindable>>& container, size_t index);
	virtual ~ContainerBindableSink() = default;

	// Sink을(를) 통해 상속됨
	void BindSink(Source& source) override;
	void PostLinkValidate() const override;

private:
	std::vector<std::shared_ptr<IBindable>>& m_container;
	unsigned m_index = 0;
	bool m_isLinked = false;

};

#pragma region ContainerBindableSinkFunc
template<class T>
void ContainerBindableSink<T>::PostLinkValidate() const
{
	assert(m_isLinked);
}

template<class T>
void ContainerBindableSink<T>::BindSink(Source& source)
{
	auto p = std::dynamic_pointer_cast<T>(source.YieldBindable());
	assert(p != nullptr);

	m_container[m_index] = std::move(p);
	m_isLinked = true;
}

template<class T>
ContainerBindableSink<T>::ContainerBindableSink(std::string registeredName, std::vector<std::shared_ptr<IBindable>>& container, size_t index)
	: Sink(std::move(registeredName)), m_container(container), m_index(index)
{

}
#pragma endregion 