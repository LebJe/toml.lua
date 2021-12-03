#include "utilities.hpp"
#include "toml.hpp"

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

void parseErrorToTable(toml::parse_error e, sol::table & table) {
	auto source = e.source();

	auto beginTable = sol::table(table.lua_state(), sol::create);
	auto endTable = sol::table(table.lua_state(), sol::create);

	table["reason"] = std::string(e.what());
	beginTable["line"] = source.begin.line;
	beginTable["column"] = source.begin.column;
	endTable["line"] = source.end.line;
	endTable["column"] = source.end.column;
	table["begin"] = beginTable;
	table["end"] = endTable;
	// table["formattedReason"] = parseErrorToString(e);
}

toml::format_flags tableToFormatFlags(sol::optional<sol::table> t) {
	auto flags = toml::format_flags();

	// Set default flags.
	if (!t) {
		flags |= toml::format_flags::quote_dates_and_times;
		flags |= toml::format_flags::allow_binary_integers;
		flags |= toml::format_flags::allow_hexadecimal_integers;
		flags |= toml::format_flags::allow_octal_integers;
		flags |= toml::format_flags::indentation;
		return flags;
	}

	auto table = t.value();
	if (table["quoteDatesAndTimes"] == true) {
		flags |= toml::format_flags::quote_dates_and_times;
	};
	if (table["quoteInfinitesAndNaNs"] == true) {
		flags |= toml::format_flags::quote_infinities_and_nans;
	};
	if (table["allowLiteralStrings"] == true) {
		flags |= toml::format_flags::allow_literal_strings;
	};
	if (table["allowMultiLineStrings"] == true) {
		flags |= toml::format_flags::allow_multi_line_strings;
	};
	if (table["allowRealTabsInStrings"] == true) {
		flags |= toml::format_flags::allow_real_tabs_in_strings;
	};
	if (table["allowBinaryIntegers"] == true) {
		flags |= toml::format_flags::allow_binary_integers;
	};
	if (table["allowOctalIntegers"] == true) { flags |= toml::format_flags::allow_octal_integers; };
	if (table["allowHexadecimalIntegers"] == true) {
		flags |= toml::format_flags::allow_hexadecimal_integers;
	};
	if (table["indentSubTables"] == true) { flags |= toml::format_flags::indent_sub_tables; };
	if (table["indentArrayElements"] == true) {
		flags |= toml::format_flags::indent_array_elements;
	};
	if (table["indentation"] == true) { flags |= toml::format_flags::indentation; }

	return flags;
}

std::optional<std::string> keyToString(sol::object key) {
	if (key.is<std::string>()) {
		return key.as<std::string>();
	} else if (key.is<int64_t>()) {
		return std::to_string(key.as<int64_t>());
	} else if (key.is<double>()) {
		return std::to_string(key.as<double>());
	} else if (key.is<bool>()) {
		return std::string(key.as<bool>() ? "true" : "false");
	}

	return std::optional<std::string>();
}

std::string solLuaDataTypeToString(sol::type type, bool withPrefix) {
	switch (type) {
		case sol::type::lightuserdata:
			return std::string("lightUserData");
		case sol::type::userdata:
			return "userData";
		case sol::type::boolean:
			return std::string(withPrefix ? "a " : "") + "foolean";
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
			return "Poly";
		case sol::type::string:
			return std::string(withPrefix ? "a " : "") + "string";
	}
}

std::variant<int, toml::table> getTableFromStringInState(sol::state_view state) {
	try {
		sol::stack::check<std::string>(
			state.lua_state(), 1,
			[](lua_State * s, int, sol::type, sol::type, const char * = nullptr) {
				return luaL_argerror(
					s, 1,
					"A string containing a TOML document should be the first and only "
					"argument");
			});

		std::string document = sol::stack::get<std::string>(state.lua_state(), 1);

		auto res = toml::parse(document);

		return res;

	} catch (toml::parse_error & e) {
		auto source = e.source();

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