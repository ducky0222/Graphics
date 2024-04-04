#pragma once

class Utils
{
public:
	template<typename Iter>
	static void SplitStringIter(const std::string& str, const std::string& delim, Iter out)
	{
		if (delim.empty())
			*out++ = str;
		else
		{
			size_t a = 0, b = str.find(delim);
			for (; b != std::string::npos; a = b + delim.length(), b = str.find(delim, a))
			{
				*out++ = std::move(str.substr(a, b - a));
			}
			*out++ = std::move(str.substr(a, str.length() - a));
		}
	}

	static std::vector<std::string> SplitString(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> strings;
		SplitStringIter(str, delim, std::back_inserter(strings));
		return strings;
	}

	static std::wstring ToWide(const std::string& str)
	{
		int num_chars = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
		std::wstring wstrTo;
		if (num_chars)
		{
			wstrTo.resize(num_chars);
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), &wstrTo[0], num_chars);
		}
		return wstrTo;
	}
};

