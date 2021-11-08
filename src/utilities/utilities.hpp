#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstddef>
#include <optional>
#include <sol/sol.hpp>
#include <string>
#include <toml.hpp>

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

/// `std::string` description of `sol::type`.
std::string solLuaDataTypeToString(sol::type type);

std::optional<std::string> keyToString(sol::object key);

#endif /* UTILITIES_H */
