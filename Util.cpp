#include "Util.h"

namespace Util{
	std::string& ltrim(std::string& str, const std::string& chars){
		str.erase(0, str.find_first_not_of(chars));
		return str;
	}

	std::string& rtrim(std::string& str, const std::string& chars){
		str.erase(str.find_last_not_of(chars) + 1);
		return str;
	}

	std::string& trim(std::string& str, const std::string& chars){
		return ltrim(rtrim(str, chars), chars);
	}

	int parseNumber(std::string& str){
		bool neg = str[0] == '-';
		if(str[0 + neg] == '0' && str[1 + neg] == 'x'){
			int ret = std::stoi(str.substr(2 + (unsigned int)neg), nullptr, 16);
			if(neg) ret *= -1;
			return ret;
		}else if(str[0 + neg] == '0' && str[1 + neg] == 'b'){
			int ret = std::stoi(str.substr(2 + (unsigned int)neg), nullptr, 2);
			if(neg) ret *= -1;
			return ret;
		}else{
			return std::stoi(str);
		}
	}
}