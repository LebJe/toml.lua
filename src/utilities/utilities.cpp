#include "utilities.hpp"
#include "Options.hpp"
#include "toml.hpp"
#include <iostream>
#include <magic_enum.hpp>
#include <map>
#include <sol/sol.hpp>

using toml::format_flags;

static std::map<format_flags, bool> defaultFlags =
	std::map<toml::format_flags, bool> { { format_flags::quote_dates_and_times, false },
										 { format_flags::quote_infinities_and_nans, false },
										 { format_flags::allow_literal_strings, false },
										 { format_flags::allow_multi_line_strings, false },
										 { format_flags::allow_real_tabs_in_strings, false },
										 { format_flags::allow_unicode_strings, true },
										 { format_flags::allow_binary_integers, true },
										 { format_flags::allow_hexadecimal_integers, true },
										 { format_flags::allow_octal_integers, true },
										 { format_flags::indent_sub_tables, false },
										 { format_flags::indentation, true },
										 { format_flags::relaxed_float_precision, false },
										 { format_flags::terse_key_value_pairs, false } };

// Based on https://codereview.stackexchange.com/a/263761
std::string camelCase(std::string s) noexcept {
	bool tail = false;
	std::size_t n = 0;

	for (unsigned char c : s) {
		if (c == '-' || c == '_') {
			tail = false;
		} else if (tail) {
			s[n++] = c;
		} else {
			tail = true;

			if (/* Don't uppercase first letter */ n != 0) {
				s[n++] = std::toupper(c);
			} else {
				s[n++] = c;
			}
		}
	}

	s.resize(n);
	return s;
}

std::string camelCase(std::string_view s) noexcept { return camelCase(std::string(s)); }

std::string sourcePositionToString(toml::source_position s) {
	return "line " + std::to_string(s.line) + ", column " + std::to_string(s.column);
}

std::string parseErrorToString(toml::parse_error e) {
	auto source = e.source();

	return std::string(e.what()) + " (" +
		   (source.begin == source.end ? "at " + sourcePositionToString(source.begin)
									   : "from " + sourcePositionToString(source.begin) + "to " +
											 sourcePositionToString(source.end)) +
		   ")";
}

inline void sourcePositionToTable(sol::table & table, toml::source_position s) {
	table["line"] = s.line;
	table["column"] = s.column;
};

void parseErrorToTable(toml::parse_error e, sol::table & table) {
	auto source = e.source();

	auto beginTable = sol::table(table.lua_state(), sol::create);
	auto endTable = sol::table(table.lua_state(), sol::create);

	sourcePositionToTable(beginTable, source.begin);
	sourcePositionToTable(endTable, source.end);
	table["reason"] = std::string(e.what());
	table["begin"] = beginTable;
	table["end"] = endTable;
}

inline toml::format_flags defaultFormatFlags() {
	auto flags = format_flags();

	for (auto [flag, isAllowed] : defaultFlags) {
		flags |= isAllowed ? flag : flags;
	}

	return flags;
}

void addFlag(toml::format_flags & flags, sol::table & flagsTable, toml::format_flags flagToAdd) {
	auto tableFlag = flagsTable[camelCase(magic_enum::enum_name(flagToAdd))];

	if (tableFlag.valid()) {
		flags |= tableFlag.get<bool>() ? flagToAdd : flags;
	} else {
		// Use default
		flags |= defaultFlags[flagToAdd] ? flagToAdd : flags;
	};
}

toml::format_flags tableToFormatFlags(sol::optional<sol::table> t) {
	auto flags = format_flags();

	// Set default flags.
	if (!t) {
		flags = defaultFormatFlags();
		return flags;
	}

	auto table = t.value();

	// User passed an empty table to clear all flags.
	if (table.empty()) return flags;

	constexpr auto f = magic_enum::enum_values<format_flags>();
	for (auto flag : f) {
		addFlag(flags, table, flag);
	}

	// `format_flags::indentation` is not returned from `enum_values`.
	addFlag(flags, table, format_flags::indentation);
	return flags;
}

Options tableToOptions(sol::optional<sol::table> t) {
	if (!t) return Options();

	auto table = t.value();
	auto options = Options();
	options.formattedIntsAsUserData =
		table["formattedIntsAsUserData"].get_or(Options().formattedIntsAsUserData);
	options.temporalTypesAsUserData =
		table["temporalTypesAsUserData"].get_or(Options().temporalTypesAsUserData);

	return options;
}

std::optional<std::string> keyToString(sol::object key) {
	switch (key.get_type()) {
		case sol::type::string:
			return key.as<std::string>();
			break;
		case sol::type::number:
			return std::to_string(key.as<int64_t>());
			break;
		case sol::type::boolean:
			return std::string(key.as<bool>() ? "true" : "false");
			break;
		case sol::type::none:
		case sol::type::lua_nil:
			return "<nil>";
			break;
		case sol::type::thread:
			return "<thread>";
			break;
		case sol::type::function:
			return "<function>";
			break;
		case sol::type::userdata:
			return "<userdata>";
			break;
		case sol::type::lightuserdata:
			return "<userdata>";
			break;
		case sol::type::table:
			return "<table>";
			break;
		case sol::type::poly:
			return "<poly>";
			break;
	}
	return std::optional<std::string>();
}

std::string solLuaDataTypeToString(sol::type type, bool withPrefix) {
	switch (type) {
		case sol::type::lightuserdata:
			return "lightUserData";
		case sol::type::userdata:
			return "userData";
		case sol::type::boolean:
			return std::string(withPrefix ? "a " : "") + "boolean";
		case sol::type::function:
			return std::string(withPrefix ? "a" : "") + "function";
		case sol::type::lua_nil:
			return "nil";
		case sol::type::none:
			return "none";
		case sol::type::number:
			return std::string(withPrefix ? "a " : "") + "number";
		case sol::type::thread:
			return std::string(withPrefix ? "a " : "") + "thread";
		case sol::type::table:
			return std::string(withPrefix ? "a " : "") + "table";
		case sol::type::poly:
			return "poly";
		case sol::type::string:
			return std::string(withPrefix ? "a " : "") + "string";
		default: return "none";
	}
}

std::variant<int, toml::table *> getTableFromStringInState(sol::state_view state, int index) {
	auto L = state.lua_state();
	try {
		if (auto toml = sol::stack::get<std::optional<std::string>>(L, 1)) {
			std::string document = sol::stack::get<std::string>(L, index);

			auto res = new toml::table(toml::parse(document));
			return res;
		} else {
			return luaL_argerror(
					L, index,
					"A string containing a TOML document should be the first argument");
		}
	} catch (toml::parse_error & e) {
		auto table = state.create_table();
		parseErrorToTable(e, table);
		sol::stack::push(state.lua_state(), table);
		return lua_error(state.lua_state());
	} catch (std::exception & e) {
		return luaL_error(
			state.lua_state(),
			(std::string("An error occurred during conversion: ") + e.what()).c_str());
	}
}