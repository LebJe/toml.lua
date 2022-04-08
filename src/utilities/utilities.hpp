#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstddef>
#include <optional>
#include <sol/sol.hpp>
#include <string>
#include <toml.hpp>

// Based on https://codereview.stackexchange.com/a/263761
std::string camelCase(std::string s) noexcept;

std::string camelCase(std::string_view s) noexcept;

/// Converts `toml::source_position` into a formatted `std::string`.
///
/// Example: "line 194, column 17"
std::string sourcePositionToString(toml::source_position s);

/// Converts `toml:parse_error` into a formatted `std::string`.
///
/// Examples:
///
/// * If `source.begin == source.end`: "Error while parsing value: could not determine value type
/// (at line 58, column 17)"
/// * If `source.begin != source.end`: "Error while parsing value: could not determine value type
/// (from line 58, column 17 to line 59, column 4)"
std::string parseErrorToString(toml::parse_error e);

/// Inserts the values in `e` into `table`.
void parseErrorToTable(toml::parse_error e, sol::table & table);

toml::format_flags tableToFormatFlags(sol::optional<sol::table> t);

/// `std::string` description of `sol::type`.
std::string solLuaDataTypeToString(sol::type type, bool withPrefix = true);

std::optional<std::string> keyToString(sol::object key);

/// Gets a string containing TOML from the first index in the Lua stack, and converts it to a
/// `toml::table`, and returns that table.
///
/// If conversion fails, an error table is pushed to the stack, and an integer from `luaL_error` is
/// returned.
///
/// If a string is not on the stack, then an integer from `luaL_argerror` is returned.
std::variant<int, toml::table> getTableFromStringInState(sol::state_view state);

/// @brief Gets a TOML string from the Lua stack, converts it to a `toml::table`, then converts that
/// table to another format using `T`. The string returned by `T` is then returned.
///
/// @param T: `T` should conform to `toml::impl::formatter`.
template <class T> inline int tomlTo(sol::state_view state, toml::format_flags flags) {
	auto L = state.lua_state();
	auto res = getTableFromStringInState(state);

	try {
		auto table = std::get<toml::table>(res);
		std::stringstream ss;

		ss << T(table, flags);
		return sol::stack::push(L, ss.str());
	} catch (std::bad_variant_access) { return std::get<int>(res); }
}

#endif /* UTILITIES_H */
