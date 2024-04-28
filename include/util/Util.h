#ifndef IOBOT_UTIL_H
#define IOBOT_UTIL_H

#include <sstream>

namespace Util{
	std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
	std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
	std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");

	bool checkCF(uint32_t res);
	bool checkSF(uint16_t res);
	bool checkZF(uint16_t res);

	/**
	 * Parses an int from a string, assuming it is trimmed. (At least at the beginning.)
	 *
	 * Works with hex (prefixed with 0x), binary (prefixed with 0b), and decimal (no prefix)
	 * @param str The string to parse, with no beginning whitespace.
	 * @return The parsed integer.
	 */
	int parseNumber(std::string& str);
}

#endif //IOBOT_UTIL_H
