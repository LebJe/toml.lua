#include "utilities.hpp"

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
	table["formattedReason"] = parseErrorToString(e);
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

std::string solLuaDataTypeToString(sol::type type) {
	switch (type) {
		case sol::type::lightuserdata:
			return "lightUserData";
		case sol::type::userdata:
			return "UserData";
		case sol::type::boolean:
			return "Boolean";
		case sol::type::function:
			return "Function";
		case sol::type::lua_nil:
			return "Nil";
		case sol::type::none:
			return "None";
		case sol::type::number:
			return "Number";
		case sol::type::thread:
			return "Thread";
		case sol::type::table:
			return "Table";
		case sol::type::poly:
			return "Poly";
		case sol::type::string:
			return "String";
	}
}