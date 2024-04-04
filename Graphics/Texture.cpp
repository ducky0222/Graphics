#include "pch.h"
#include "Texture.h"
#include "ResourceManager.h"

Texture::Texture(FlatGraphics& graphics, const std::string& path, unsigned slot /*= 0*/)
	: m_path(path)
{
	// Surface가 필요한가
}

std::shared_ptr<Texture> Texture::Create(FlatGraphics& graphics, const std::string& path, unsigned slot /*= 0*/)
{

}

std::string Texture::GenerateUID(const std::string& path, unsigned slot /*= 0*/)
{

}

void Texture::Bind(FlatGraphics& graphics)
{
}

std::string Texture::GetUID() const
{

}

unsigned Texture::calculateNumberOfMipLevels(unsigned width, unsigned height)
{

}
