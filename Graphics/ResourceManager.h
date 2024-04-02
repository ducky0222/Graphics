#pragma once

#include "IBindable.h"

class FlatGraphics;

class ResourceManager
{
public:
	static ResourceManager& Get();

	template<typename T, typename... Params>
	static std::shared_ptr<T> Create(FlatGraphics& graphics, Params&&... params);

	void Destroy()
	{
		m_bindables.clear();
	}

private:
	ResourceManager() = default;
	~ResourceManager() = default;

	template<typename T, typename... Params>
	std::shared_ptr<T> create(FlatGraphics& graphics, Params&&... params);

private:
	std::unordered_map<std::string, std::weak_ptr<IBindable>> m_bindables;
};

template<typename T, typename... Params>
static std::shared_ptr<T>
ResourceManager::Create(FlatGraphics& graphics, Params&&... params)
{
	static_assert(std::is_base_of<IBindable, T>::value, "T must be IBindable");
	return Get().create<T>(graphics, std::forward<Params>(params)...);
}

template<typename T, typename... Params>
std::shared_ptr<T>
ResourceManager::create(FlatGraphics& graphics, Params&&... params)
{
	const auto key = T::GenerateUID(std::forward<Params>(params)...);
	const auto find = m_bindables.find(key);

	if (find == m_bindables.end())
	{
		auto bindable = std::make_shared<T>(graphics, std::forward<Params>(params)...);
		m_bindables[key] = bindable;

		return bindable;
	}
	else
	{
		auto lock = find->second.lock();

		if (lock == nullptr)
		{
			auto bindable = std::make_shared<T>(graphics, std::forward<Params>(params)...);
			m_bindables[key] = bindable;

			return bindable;
		}

		return std::static_pointer_cast<T>(lock);
	}
}

