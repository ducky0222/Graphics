#include "Source.h"

std::shared_ptr<IBindable> Source::YieldBindable()
{
	assert(false);
	throw;
}

std::shared_ptr<BufferResource> Source::YieldBuffer()
{
	assert(false);
	throw;
}

Source::Source(std::string name)
	: m_name(std::move(name))
{
	assert(!m_name.empty());

	const bool nameCharsValid = std::all_of(m_name.begin(), m_name.end(), [](char c)
		{
			return std::isalnum(c) || c == '_';
		});

	if (!nameCharsValid || std::isdigit(m_name.front()))
		assert(false);
}
