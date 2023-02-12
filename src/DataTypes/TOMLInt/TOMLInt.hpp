#ifndef TOMLINT_H
#define TOMLINT_H

#include <cstddef>
#include <toml.hpp>

/// Wrapper for `toml::value<int64_t>` (with `toml::value_flags`).
struct TOMLInt {
	toml::value<int64_t> tomlInt;

	TOMLInt(toml::value<int64_t> tomlInt) : tomlInt(tomlInt) {};

	TOMLInt(toml::value<int64_t> tomlInt, toml::value_flags valueFlags)
		: TOMLInt(
			  tomlInt.get(),
			  static_cast<std::underlying_type<toml::value_flags>::type>(valueFlags)) {}

	TOMLInt(int64_t int64) { tomlInt = int64; }

	TOMLInt(int64_t int64, toml::value_flags valueFlags)
		: TOMLInt(int64, static_cast<std::underlying_type<toml::value_flags>::type>(valueFlags)) {}

	TOMLInt(int64_t int64, uint16_t valueFlags) {
		this->tomlInt = int64;
		tomlInt = tomlInt.flags(toml::value_flags(valueFlags));
	}

	int64_t getInt() const { return tomlInt.get(); }
	void setInt(int64_t int64) { tomlInt = int64; }

	bool operator==(const TOMLInt & right) const {
		return tomlInt == right.tomlInt && tomlInt.flags() == right.tomlInt.flags();
	}

	bool operator<(const TOMLInt & right) const { return tomlInt < right.tomlInt; }

	bool operator<=(const TOMLInt & right) const { return tomlInt <= right.tomlInt; }
};

std::ostream & operator<<(std::ostream & os, const TOMLInt & tomlInt);

#endif
