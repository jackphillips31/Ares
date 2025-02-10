#pragma once

namespace Ares::Utility {

	std::string IncrementStringSuffix(const std::string& input);

	constexpr size_t StringLength(const char* str)
	{
		size_t len = 0;
		while (str[len] != '\0')
			++len;

		return len;
	}

	constexpr void StringCopy(char* destination, const char* source)
	{
		for (size_t i = 0; source[i] != '\0'; i++)
		{
			destination[i] = source[i];
		}
		destination[size_t(-1)] = '\0';
	}

	constexpr void StringConcat(char* destination, const char* source)
	{
		while (*destination)
			++destination;

		while (*source)
		{
			*destination = *source;
			++destination;
			++source;
		}

		*destination = '\0';
	}

	constexpr void StringAppendInteger(char* destination, uint32_t value)
	{
		while (*destination)
			++destination;

		if (value == 0)
		{
			*destination = '0';
			++destination;
		}
		else
		{
			char buffer[10];
			int32_t index = 0;

			while (value > 0)
			{
				buffer[index++] = '0' + (value % 10);
				value /= 10;
			}

			for (int32_t i = index - 1; i >= 0; i--)
			{
				*destination = buffer[i];
				++destination;
			}
		}

		*destination = '\0';
	}

}