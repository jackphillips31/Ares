#include <arespch.h>
#include "Engine/Utility/String.h"

#include <EASTL/string.h>

namespace Ares::Utility {

	void IncrementStringSuffix(String& input)
	{
		// Find the position of the last '(' and ')'
		size_t openParenPos = input.find_last_of('(');
		size_t closeParenPos = input.find_last_of(')');

		// If there's a '(' and ')' at the end of the string
		if (openParenPos != eastl::string::npos && closeParenPos != eastl::string::npos && closeParenPos > openParenPos)
		{
			String numberPart = input.substr(openParenPos + 1, closeParenPos - openParenPos - 1);

			// Check if the part inside the parentheses is a valid number
			try
			{
				// Convert the number part
				uint32_t currentNumber = std::stoi(numberPart.c_str());

				// Modify the input string in-place: remove the current number and append the incremented one
				input.erase(openParenPos);
				input.append("(");
				input.append(String(std::to_string(currentNumber + 1).c_str()));
				input.append(")");
			}
			catch (const std::invalid_argument&)
			{
				// If the string inside the parentheses isn't a valid number, treat it as if no numeric suffix was found
				input.append("(2)");
			}
		}
		else
		{
			// If no numeric suffix exists, just append "(2)"
			input.append("(2)");
		}
	}

}