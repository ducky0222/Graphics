#include "pch.h"
#include "ResourceManager.h"

ResourceManager& ResourceManager::Get()
{
	static ResourceManager resourceManager;
	return resourceManager;
}
