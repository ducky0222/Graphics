#include "pch.h"
#include "Sink.h"

Sink::Sink(std::string registeredName)
	: m_registeredName(std::move(registeredName))
{
	if (m_registeredName.empty())
		assert(false);

	// �̸��� ��� ��Ұ� ����(������ �ݿ�)�̰ų� ������̸� ���
	const bool nameCharsValid = std::all_of(m_registeredName.begin(), m_registeredName.end(), [](char c)
		{
			return std::isalnum(c) || c == '_';
		});

	if (!nameCharsValid || std::isdigit(m_registeredName.front()))
	{
		assert(false);
	}
}

void Sink::SetTarget(const std::string& passName, const std::string& outputName)
{
	assert(!passName.empty());
	assert(!outputName.empty());

	{
		// �н� ���� ��ȿ�� üũ
		const bool nameCharsValid = std::all_of(passName.begin(), passName.end(), [](char c)
			{
				return std::isalnum(c) || c == '_';
			});

		if (passName != "$" && (!nameCharsValid || std::isdigit(passName.front())))
		{
			assert(false);
		}

		m_passName = passName;
	}

	{
		// ��� ���� ��ȿ�� üũ
		const bool nameCharsValid = std::all_of(outputName.begin(), outputName.end(), [](char c)
			{
				return std::isalnum(c) || c == '_';
			});

		if (!nameCharsValid || std::isdigit(outputName.front()))
		{
			assert(false);
		}

		m_outputName = outputName;
	}
}
