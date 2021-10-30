#include "toml.hpp"
#include "decoding/decoding.hpp"
#include "encoding/encoding.hpp"
#include "kaguya.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

/// Converts `toml::source_position` into a formatted `std::string`.
///
/// Example: "line 194, column 17"
std::string sourcePositionToString(toml::source_position s) {
	return "line " + std::to_string(s.line) + ", column " + std::to_string(s.column);
}

/// Converts `toml:parse_error` into a formatted `std::string`.
///
/// Examples:
///
/// * If `source.begin == source.end`: "Error while parsing value: could not determine value type
/// (at line 58, column 17)"
/// * If `source.begin != source.end`: "Error while parsing value: could not determine value type
/// (from line 58, column 17 to line 59, column 4)"

std::string parseErrorToString(toml::parse_error e) {
	auto source = e.source();

	return std::string(e.what()) + " (" +
		   (source.begin == source.end ? "at " + sourcePositionToString(source.begin)
									   : "from " + sourcePositionToString(source.begin) + "to " +
											 sourcePositionToString(source.end)) +
		   ")";
}

/// Inserts the values in `e` into `table`.
void parseErrorToTable(toml::parse_error e, kaguya::LuaTable & table) {
	auto source = e.source();

	auto beginTable = kaguya::LuaTable(table.state());
	auto endTable = kaguya::LuaTable(table.state());

	table["reason"] = std::string(e.what());
	beginTable["line"] = source.begin.line;
	beginTable["column"] = source.begin.column;
	endTable["line"] = source.end.line;
	endTable["column"] = source.end.column;
	table["begin"] = beginTable;
	table["end"] = endTable;
	table["formattedReason"] = parseErrorToString(e);
}

#ifdef __cplusplus
extern "C" {
#endif

	int encode(lua_State * L) {
		kaguya::State state(L);
		kaguya::LuaTable table;
		try {
			table = state.popFromStack();
		} catch (kaguya::LuaTypeMismatch & e) {
			return luaL_argerror(
				L, 1, "A Lua table with strings as keys should be the first and only argument");
		}

		toml::table tomlTable;

		try {
			tomlTable = tomlTableFromLuaTable(table);
		} catch (std::exception & e) {
			luaL_error(L, (std::string("An error occurred during encoding: ") + e.what()).c_str());
		}

		std::stringstream ss;

		ss << toml::default_formatter(tomlTable);

		state.pushToStack(ss.str());

		return 1;
	}

	int decode(lua_State * L) {
		kaguya::State state(L);

		try {

			std::string document = state.popFromStack();

			auto res = toml::parse(document);

			kaguya::LuaTable table = state.newTable();

			tomlToLuaTable(res, table);

			return table.push();
		} catch (kaguya::LuaTypeMismatch & e) {
			return luaL_argerror(
				L, 1, "A string containing a TOML document should be the first and only argument");
		} catch (toml::parse_error & e) {
			auto source = e.source();

			auto table = kaguya::LuaTable(state.state());

			parseErrorToTable(e, table);

			state.pushToStack(table);
			return lua_error(state.state());
		}
	}

	int tomlToJSON(lua_State * L) {
		kaguya::State state(L);

		try {

			std::string document = state.popFromStack();

			auto res = toml::parse(document);

			std::stringstream ss;

			ss << toml::json_formatter(res);

			state.pushToStack(ss.str());

			return 1;
		} catch (kaguya::LuaTypeMismatch & e) {
			return luaL_argerror(
				L, 1, "A string containing a TOML document should be the first and only argument");
		} catch (toml::parse_error & e) {
			auto source = e.source();

			auto table = kaguya::LuaTable(state.state());

			parseErrorToTable(e, table);

			state.pushToStack(table);
			return lua_error(state.state());
		}
	}

#ifdef __cplusplus
}
#endif

extern "C" __attribute__((visibility("default"))) int luaopen_toml(lua_State * L) {
	kaguya::State state(L);
	kaguya::LuaTable module = state.newTable();

	module["encode"] = kaguya::luacfunction(&encode);
	module["decode"] = kaguya::luacfunction(&decode);
	module["tomlToJSON"] = kaguya::luacfunction(&tomlToJSON);

	return module.push();
}
