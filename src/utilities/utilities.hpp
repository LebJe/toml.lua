#ifndef UTILITIES_H
#define UTILITIES_H

#include <magic_enum.hpp>
#include <Options.hpp>
#include <cstddef>
#include <optional>
#include <sol/sol.hpp>
#include <string>
#include <toml.hpp>
#include <variant>

/// Converts a string into [Camel Case](https://en.wikipedia.org/wiki/Camel_case).
///
/// The code in this function is based on https://en.wikipedia.org/wiki/Camel_case
std::string camelCase(std::string s) noexcept;

/// Version of `camelCase` that accepts a `string_view`.
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

/// Takes a Lua table, with keys representing flag names, and values
toml::format_flags tableToFormatFlags(sol::optional<sol::table> t);

Options tableToOptions(sol::optional<sol::table> t);

/// `std::string` description of `sol::type`.
std::string solLuaDataTypeToString(sol::type type, bool withPrefix = false);

std::optional<std::string> keyToString(sol::object key);

/// Gets a string containing TOML from `index` in the Lua stack, and converts it to a
/// `toml::table`, and returns that table.
///
/// If conversion fails, an error table is pushed to the stack, and an integer from `luaL_error` is
/// returned.
///
/// If a string is not on the stack, then an integer from `luaL_argerror` is returned.
std::variant<int, toml::table *> getTableFromStringInState(sol::state_view state, int index = 1);

#endif /* UTILITIES_H */
