#include <arespch.h>
#include "Engine/Utility/Data.h"

namespace Ares::Utility {

	std::string FormatSize(const size_t bytes)
	{
		const size_t KB = 1024;
		const size_t MB = KB * 1024;
		const size_t GB = MB * 1024;
		const size_t TB = GB * 1024;

		std::ostringstream result;
		result << std::fixed << std::setprecision(2); // Set precision to 2 decimal places

		if (bytes >= TB)
		{
			result << static_cast<double>(bytes) / TB << " TiB";
		}
		else if (bytes >= GB)
		{
			result << static_cast<double>(bytes) / GB << " GiB";
		}
		else if (bytes >= MB)
		{
			result << static_cast<double>(bytes) / MB << " MiB";
		}
		else if (bytes >= KB)
		{
			result << static_cast<double>(bytes) / KB << " KiB";
		}
		else
		{
			result << bytes << " bytes";
		}

		return result.str();
	}

}