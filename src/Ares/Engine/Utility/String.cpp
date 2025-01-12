#include <arespch.h>
#include "Engine/Utility/String.h"

namespace Ares::Utility {

	std::string IncrementStringSuffix(const std::string& input)
	{
		// Regular expression to match strings with a numeric suffix
		std::regex pattern(R"((.+)\((\d+)\)$)");
		std::smatch matches;

		// If the input matches the pattern with a numeric suffix
		if (std::regex_match(input, matches, pattern))
		{
			// Extract the base string and the current number
			std::string baseString = matches[1].str();
			uint32_t currentNumber = std::stoi(matches[2].str());

			// Increment the number
			return baseString + "(" + std::to_string(currentNumber + 1) + ")";
		}

		// If no suffix exists, add (2) to the end
		return input + "(2)";
	}

}