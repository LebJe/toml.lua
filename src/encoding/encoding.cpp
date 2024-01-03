#include "encoding.hpp"
#include "toml.hpp"
#include <DataTypes/DateAndTime/dateAndTime.hpp>
#include <DataTypes/TOMLInt/TOMLInt.hpp>
#include <cassert>
#include <sol/sol.hpp>
#include <string>
#include <utilities/utilities.hpp>
#include <variant>

void unpack(
	std::variant<std::tuple<toml::table *, std::string>, toml::array *> tableOrArray,
	std::function<void(toml::table * table, std::string key)> tableFunc,
	std::function<void(toml::array * array)> arrayFunc) {
	try {
		auto t = std::get<std::tuple<toml::table *, std::string>>(tableOrArray);
		tableFunc(std::get<0>(t), std::get<1>(t));
	} catch (std::bad_variant_access const &) {
		auto a = std::get<toml::array *>(tableOrArray);
		arrayFunc(a);
	}
}

void luaValueIntoTOMLTableOrArray(
	sol::object luaValue,
	std::variant<std::tuple<toml::table *, std::string>, toml::array *> tableOrArray) {
	switch (luaValue.get_type()) {
		case sol::type::number: {
			int64_t intVal = luaValue.as<int64_t>();
			double doubleVal = luaValue.as<double>();

			toml::node * node = NULL;

			if (doubleVal == intVal) {
				node = new toml::value(intVal);

			} else {
				node = new toml::value(doubleVal);
			}

			unpack(
				tableOrArray, [node](auto table, auto key) { table->insert(key, *node); },
				[node](auto array) { array->push_back(*node); });

			break;
		}
		case sol::type::boolean: {
			auto node = new toml::value(luaValue.as<bool>());
			unpack(
				tableOrArray, [node](auto table, auto key) { table->insert(key, *node); },
				[node](auto array) { array->push_back(*node); });

			break;
		}
		case sol::type::string: {
			auto node = new toml::value(luaValue.as<std::string>());
			unpack(
				tableOrArray, [node](auto table, auto key) { table->insert(key, *node); },
				[node](auto array) { array->push_back(*node); });

			break;
		}
		case sol::type::userdata: {
			if (luaValue.is<TOMLDate>()) {
				auto node = new toml::value(luaValue.as<TOMLDate>().date);
				unpack(
					tableOrArray, [node](auto table, auto key) { table->insert(key, *node); },
					[node](auto array) { array->push_back(*node); });
			} else if (luaValue.is<TOMLTime>()) {
				auto node = new toml::value(luaValue.as<TOMLTime>().time);
				unpack(
					tableOrArray, [node](auto table, auto key) { table->insert(key, *node); },
					[node](auto array) { array->push_back(*node); });
			} else if (luaValue.is<TOMLDateTime>()) {
				auto node = new toml::value(luaValue.as<TOMLDateTime>().asDateTime());
				unpack(
					tableOrArray, [node](auto table, auto key) { table->insert(key, *node); },
					[node](auto array) { array->push_back(*node); });
			} else if (luaValue.is<TOMLInt>()) {
				auto tomlInt = luaValue.as<TOMLInt>().tomlInt;

				auto node = new toml::value(tomlInt);

				unpack(
					tableOrArray,
					[node, &tomlInt](auto table, auto key) {
						table->insert(key, *node, tomlInt.flags());
					},
					[node, &tomlInt](auto array) { array->push_back(*node, tomlInt.flags()); });
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
					auto k = keyToString(key);
					throw sol::error((k->empty() ? "The indexes in a table"
												 : std::string("The index") + k.value() +
													   " should be a integer or a string, not " +
													   solLuaDataTypeToString(key.get_type(), true))
										 .c_str());
				}
			}

			if (keyIsInt) {
				auto node = tomlArrayFromLuaArray(luaValue.as<sol::table>());

				unpack(
					tableOrArray, [node](auto table, auto key) { table->insert(key, *node); },
					[node](auto array) { array->push_back(*node); });
			} else {
				auto node = tomlTableFromLuaTable(luaValue.as<sol::table>());
				unpack(
					tableOrArray, [node](auto table, auto key) { table->insert(key, *node); },
					[node](auto array) { array->push_back(*node); });
			}

			break;
		}
		default:
			break;
	}
}

toml::array * tomlArrayFromLuaArray(sol::table luaArray) {
	auto tomlArray = new toml::array();

	for (auto && [key, value] : luaArray) {
		if (!key.is<int64_t>()) {
			auto k = keyToString(key);
			throw sol::error((k->empty() ? "The indexes in an array"
										 : std::string("The index ") + k.value() +
											   " should be a integer, not " +
											   solLuaDataTypeToString(key.get_type(), true))
								 .c_str());
		}

		luaValueIntoTOMLTableOrArray(value, tomlArray);
	}

	return tomlArray;
}

toml::table * tomlTableFromLuaTable(sol::table luaTable) {
	auto table = new toml::table();
	table->is_inline(luaTable[sol::metatable_key]["inline"].get_or(false));

	for (auto && [key, value] : luaTable) {
		if (!key.is<std::string>()) {
			auto k = keyToString(key);
			throw sol::error((k->empty() ? "The keys in a table"
										 : std::string("The key ") + "\"" + k.value() + "\"" +
											   " should be a string, not " +
											   solLuaDataTypeToString(key.get_type(), true))
								 .c_str());
		}

		luaValueIntoTOMLTableOrArray(value, std::make_tuple(table, key.as<std::string>()));
	}

	return table;
}