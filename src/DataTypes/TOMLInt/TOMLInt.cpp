#include "magic_enum.hpp"
#include "toml.hpp"
#include <DataTypes/TOMLInt/TOMLInt.hpp>

std::ostream & operator<<(std::ostream & os, const TOMLInt & tomlInt) {
	switch (tomlInt.tomlInt.flags()) {
		case toml::value_flags::none:
			os << tomlInt.getInt();
			break;
		case toml::value_flags::format_as_binary:
			os << "0b" << std::bitset<8>(tomlInt.getInt());
			break;
		case toml::value_flags::format_as_octal:
			os << "0o" << std::oct << tomlInt.getInt();
			break;
		case toml::value_flags::format_as_hexadecimal:
			os << "0x" << std::hex << tomlInt.getInt();
			break;
	}

	return os;
}
