#pragma once

#include "STLHeaders.h"
#include <cassert>

class IBindable;
class BufferResource;

class Source
{
public:
	virtual ~Source() = default;

	virtual void PostLinkValidate() const abstract;

	virtual std::shared_ptr<IBindable> YieldBindable();
	virtual std::shared_ptr<BufferResource> YieldBuffer();

	std::string GetName() const { return m_name; }
protected:
	Source(std::string name);

private:
	std::string m_name;
};

template<typename T>
class DirectBufferSource : public Source
{
public:
	DirectBufferSource(std::string name, std::shared_ptr<T>& buffer);
	virtual ~DirectBufferSource() = default;

	static std::unique_ptr<DirectBufferSource<T>> Make(std::string name, std::shared_ptr<T>& buffer);

	void PostLinkValidate() const;
	std::shared_ptr<BufferResource> YieldBuffer() override;

private:
	std::shared_ptr<T>& m_buffer = nullptr;
	bool m_isLinked = false;
};

#pragma region DirectBufferSourceFunc
template<typename T>
std::shared_ptr<BufferResource> DirectBufferSource<T>::YieldBuffer()
{
	assert(!m_isLinked);
	m_isLinked = true;
	return m_buffer;
}

template<typename T>
void DirectBufferSource<T>::PostLinkValidate() const
{

}

template<typename T>
std::unique_ptr<DirectBufferSource<T>> DirectBufferSource<T>::Make(std::string name, std::shared_ptr<T>& buffer)
{
	return std::make_unique<DirectBufferSource>(std::move(name), buffer);
}

template<typename T>
DirectBufferSource<T>::DirectBufferSource(std::string name, std::shared_ptr<T>& buffer)
	: Source(std::move(name)), m_buffer(buffer)
{

}
#pragma endregion

template<typename T>
class DirectBindableSource : public Source
{
public:
	DirectBindableSource(std::string name, std::shared_ptr<T>& bind);
	virtual ~DirectBindableSource() = default;

	static std::unique_ptr<DirectBindableSource> Make(std::string name, std::shared_ptr<T>& buffer);

	void PostLinkValidate() const;
	std::shared_ptr<IBindable> YieldBindable() override;

private:
	std::shared_ptr<T>& m_bind = nullptr;
};

#pragma region DirectBindableSourceFunc
template<typename T>
std::shared_ptr<IBindable> DirectBindableSource<T>::YieldBindable()
{
	return m_bind;
}

template<typename T>
void DirectBindableSource<T>::PostLinkValidate() const
{

}

template<typename T>
std::unique_ptr<DirectBindableSource<T>> DirectBindableSource<T>::Make(std::string name, std::shared_ptr<T>& buffer)
{
	return std::make_unique<DirectBindableSource>(std::move(name), buffer);
}

template<typename T>
DirectBindableSource<T>::DirectBindableSource(std::string name, std::shared_ptr<T>& bind)
	: Source(std::move(name)), m_bind(bind)
{

}
#pragma endregion
