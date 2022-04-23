#include <DateAndTime/dateAndTime.hpp>
#include <cstddef>
#include <decoding/decoding.hpp>
#include <encoding/encoding.hpp>
#include <exception>
#include <iostream>
#include <optional>
#include <sol/sol.hpp>
#include <string>
#include <toml.hpp>
#include <utilities/utilities.hpp>

#ifdef __cplusplus
extern "C" {
#endif

	int encode(lua_State * L) {
		sol::state_view state(L);
		sol::table table;

		sol::stack::check<sol::table>(
			L, 1, [](lua_State * s, int, sol::type, sol::type, const char * = nullptr) {
				return luaL_argerror(
					s, 1, "A Lua table with strings as keys should be the first and only argument");
			});
		table = sol::stack::get<sol::table>(L, 1);

		auto flags = tableToFormatFlags(sol::stack::check_get<sol::table>(L, 2));

		toml::table tomlTable;

		try {
			tomlTable = tomlTableFromLuaTable(table);
		} catch (std::exception & e) {
			return luaL_error(
				L, (std::string("An error occurred during encoding: ") + e.what()).c_str());
		}

		std::stringstream ss;

		ss << toml::toml_formatter(tomlTable, flags);

		return sol::stack::push(L, ss.str());

		return 1;
	}

	int decode(lua_State * L) {
		sol::state_view state(L);
		auto res = getTableFromStringInState(state);

		try {
			try {
				auto tomlTable = std::get<toml::table>(res);

				auto luaTable = state.create_table();

				tomlToLuaTable(tomlTable, luaTable);

				return luaTable.push();
			} catch (std::bad_variant_access) { return std::get<int>(res); }
		} catch (std::exception & e) {
			return luaL_error(
				L, (std::string("An error occurred during decoding: ") + e.what()).c_str());
		}
	}

	int tomlToJSON(lua_State * L) {
		auto flags = tableToFormatFlags(sol::stack::check_get<sol::table>(L, 2));
		return tomlTo<toml::json_formatter>(sol::state_view(L), flags);
	}

	int tomlToYAML(lua_State * L) {
		auto flags = tableToFormatFlags(sol::stack::check_get<sol::table>(L, 2));
		return tomlTo<toml::yaml_formatter>(sol::state_view(L), flags);
	}

#ifdef __cplusplus
}
#endif

extern "C" {
	LUALIB_API int luaopen_toml(lua_State * L) {
		sol::state_view state(L);
		sol::table module = state.create_table();

		// Setup functions.

		module["encode"] = &encode;
		module["decode"] = &decode;
		module["tomlToJSON"] = &tomlToJSON;
		module["tomlToYAML"] = &tomlToYAML;

		// Setup UserType - Date

		sol::usertype<TOMLDate> tomlDate = module.new_usertype<TOMLDate>(
			"Date", sol::constructors<TOMLDate(uint16_t, uint8_t, uint8_t)>());

		tomlDate["year"] = sol::property(&TOMLDate::getYear, &TOMLDate::setYear);
		tomlDate["month"] = sol::property(&TOMLDate::getMonth, &TOMLDate::setMonth);
		tomlDate["day"] = sol::property(&TOMLDate::getDay, &TOMLDate::setDay);

		// Setup UserType - Time

		sol::usertype<TOMLTime> tomlTime = module.new_usertype<TOMLTime>(
			"Time", sol::constructors<TOMLTime(uint8_t, uint8_t, uint8_t, uint16_t)>());

		tomlTime["hour"] = sol::property(&TOMLTime::getHour, &TOMLTime::setHour);
		tomlTime["minute"] = sol::property(&TOMLTime::getMinute, &TOMLTime::setMinute);
		tomlTime["second"] = sol::property(&TOMLTime::getSecond, &TOMLTime::setSecond);
		tomlTime["nanoSecond"] = sol::property(&TOMLTime::getNanoSecond, &TOMLTime::setNanoSecond);

		// Setup UserType - TimeOffset

		sol::usertype<TOMLTimeOffset> tomlTimeOffset = module.new_usertype<TOMLTimeOffset>(
			"TimeOffset", sol::constructors<TOMLTimeOffset(int8_t, int8_t)>());

		tomlTimeOffset["minutes"] = sol::property(&TOMLTimeOffset::minutes);

		// Setup UserType - DateTime

		sol::usertype<TOMLDateTime> tomlDateTime = module.new_usertype<TOMLDateTime>(
			"DateTime",
			sol::constructors<
				TOMLDateTime(TOMLDate, TOMLTime), TOMLDateTime(TOMLDate, TOMLTime, TOMLTimeOffset)>());

		tomlDateTime["date"] = sol::property(&TOMLDateTime::getDate, &TOMLDateTime::setDate);
		tomlDateTime["time"] = sol::property(&TOMLDateTime::getTime, &TOMLDateTime::setTime);
		tomlDateTime["timeOffset"] =
			sol::property(&TOMLDateTime::getTimeOffset, &TOMLDateTime::setTimeOffset);

		return module.push();
	}
}
