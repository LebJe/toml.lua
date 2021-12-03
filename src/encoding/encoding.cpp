#include "encoding.hpp"
#include "sol/sol.hpp"
#include "utilities/utilities.hpp"
#include <cassert>
#include <iostream>

void * luaValueToTomlNode(sol::object & luaValue) {
	switch (luaValue.get_type()) {
		case sol::type::number: {
			int64_t intVal = luaValue.as<int64_t>();
			double doubleVal = luaValue.as<double>();

			if (doubleVal == intVal) {
				auto node = new toml::value(intVal);
				return reinterpret_cast<void *>(node);
			} else {
				auto node = new toml::value(doubleVal);
				return reinterpret_cast<void *>(node);
			}

			break;
		}
		case sol::type::boolean: {
			auto node = new toml::value(luaValue.as<bool>());
			return reinterpret_cast<void *>(node);
			break;
		}
		case sol::type::string: {
			auto node = new toml::value(luaValue.as<std::string>());
			return reinterpret_cast<void *>(node);
			break;
		}
		case sol::type::userdata: {
			if (luaValue.is<TOMLDate>()) {
				auto node = new toml::value(luaValue.as<TOMLDate>().date);
				return reinterpret_cast<void *>(node);
			} else if (luaValue.is<TOMLTime>()) {
				auto node = new toml::value(luaValue.as<TOMLTime>().time);
				return reinterpret_cast<void *>(node);
			} else if (luaValue.is<TOMLDateTime>()) {
				auto node = new toml::value(luaValue.as<TOMLDateTime>().asDateTime());
				return reinterpret_cast<void *>(node);
			}
			break;
		}
		case sol::type::table: {
			auto table = luaValue.as<sol::table>();

			bool keyIsInt = false;

			for (auto && [key, _] : table) {
				if (key.is<std::string>()) {
					keyIsInt = false;
					break;
				} else if (key.is<int64_t>()) {
					keyIsInt = true;
					break;
				} else {
					// TODO: Error!
					auto k = keyToString(key);
					throw sol::error((k->empty() ? "The indexes in a table"
												 : std::string("The index") + k.value() +
													   " should be a integer or a string, not a " +
													   solLuaDataTypeToString(key.get_type()))
										 .c_str());
				}
			}

			if (keyIsInt) {
				return reinterpret_cast<void *>(
					new toml::array(tomlArrayFromLuaArray(luaValue.as<sol::table>())));
			} else {
				return reinterpret_cast<void *>(
					new toml::table(tomlTableFromLuaTable(luaValue.as<sol::table>())));
			}

			break;
		}
		default:
			break;
	}

	return NULL;
}

toml::array tomlArrayFromLuaArray(sol::table luaArray) {
	auto tomlArray = toml::array();

	for (auto && [key, value] : luaArray) {
		if (!key.is<int64_t>()) {
			auto k = keyToString(key);
			throw sol::error((k->empty() ? "The indexes in an array"
										 : std::string("The index ") + k.value() +
											   " should be a integer, not a " +
											   solLuaDataTypeToString(key.get_type()))
								 .c_str());
		}
		if (auto v = reinterpret_cast<toml::node *>(luaValueToTomlNode(value))) {

			tomlArray.push_back(*v);
		}
	}

	return tomlArray;
}

toml::table tomlTableFromLuaTable(sol::table luaTable) {
	auto table = toml::table();

	for (auto && [key, value] : luaTable) {
		if (!key.is<std::string>()) {
			auto k = keyToString(key);
			throw sol::error((k->empty() ? "The keys in a table"
										 : std::string("The key ") + k.value() +
											   " should be a string, not " +
											   solLuaDataTypeToString(key.get_type()))
								 .c_str());
		}

		if (auto v = reinterpret_cast<toml::node *>(luaValueToTomlNode(value))) {
			table.insert(key.as<std::string>(), *v);
		}
	}

	return table;
}
