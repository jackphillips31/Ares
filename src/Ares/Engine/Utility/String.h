#pragma once
#include "Engine/Containers/String.h"

namespace Ares::Utility {

	void IncrementStringSuffix(String& input);

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