#include "decoding.hpp"
#include <DataTypes/TOMLInt/TOMLInt.hpp>
#include <magic_enum.hpp>
#include <sol/sol.hpp>
#include <toml.hpp>

void insertNodeInTable(
	sol::table & luaTable, std::variant<std::string, size_t> keyOrIndex, toml::node * node,
	Options options) {

	switch (node->type()) {
		case toml::node_type::string: {
			auto v = std::string(*node->as_string());
			try {
				luaTable[std::get<std::string>(keyOrIndex)] = v;
			} catch (std::bad_variant_access) { luaTable[std::get<size_t>(keyOrIndex)] = v; }
			break;
		}

		case toml::node_type::integer: {
			auto v = *node->as_integer();
			try {
				if (options.formattedIntAsUserData && v.flags() != toml::value_flags::none) {
					luaTable[std::get<std::string>(keyOrIndex)] = TOMLInt(v, v.flags());
				} else {
					luaTable[std::get<std::string>(keyOrIndex)] = v.get();
				}
			} catch (std::bad_variant_access) {
				if (options.formattedIntAsUserData && v.flags() != toml::value_flags::none) {
					luaTable[std::get<size_t>(keyOrIndex)] = TOMLInt(v, v.flags());
				} else {
					luaTable[std::get<size_t>(keyOrIndex)] = v.get();
				}
			}

			break;
		}

		case toml::node_type::floating_point: {
			auto v = double { *node->as_floating_point() };

			try {
				luaTable[std::get<std::string>(keyOrIndex)] = v;
			} catch (std::bad_variant_access) { luaTable[std::get<size_t>(keyOrIndex)] = v; }

			break;
		}

		case toml::node_type::boolean: {
			auto v = node->as_boolean()->get();

			try {
				luaTable[std::get<std::string>(keyOrIndex)] = v;
			} catch (std::bad_variant_access) { luaTable[std::get<size_t>(keyOrIndex)] = v; }

			break;
		}
		case toml::node_type::array: {
			auto newLTable = sol::table(luaTable.lua_state(), sol::create);

			tomlArrayToLuaArray(*node->as_array(), newLTable, options);

			newLTable.push();

			try {
				luaTable[std::get<std::string>(keyOrIndex)] = newLTable;
			} catch (std::bad_variant_access) {
				luaTable[std::get<size_t>(keyOrIndex)] = newLTable;
			}

			break;
		}
		case toml::node_type::table: {
			auto newLTable = sol::table(luaTable.lua_state(), sol::create);

			tomlToLuaTable(node->as_table(), newLTable, options);

			newLTable.push();

			try {
				luaTable[std::get<std::string>(keyOrIndex)] = newLTable;
			} catch (std::bad_variant_access) {
				luaTable[std::get<size_t>(keyOrIndex)] = newLTable;
			}

			break;
		}
		case toml::node_type::date: {
			auto v = TOMLDate(*(*node->as_date()));

			try {
				if (options.temporalTypesAsUserData) {
					luaTable[std::get<std::string>(keyOrIndex)] = v;
				} else {
					auto t = luaTable.create();
					v.toTable(t);
					luaTable[std::get<std::string>(keyOrIndex)] = t;
				}
			} catch (std::bad_variant_access) {
				if (options.temporalTypesAsUserData) {
					luaTable[std::get<size_t>(keyOrIndex)] = v;
				} else {
					auto t = luaTable.create();
					v.toTable(t);
					luaTable[std::get<size_t>(keyOrIndex)] = t;
				}
			}

			break;
		}
		case toml::node_type::time: {
			auto v = TOMLTime(*(*node->as_time()));

			try {
				if (options.temporalTypesAsUserData) {
					luaTable[std::get<std::string>(keyOrIndex)] = v;
				} else {
					auto t = luaTable.create();
					v.toTable(t);
					luaTable[std::get<std::string>(keyOrIndex)] = t;
				}
			} catch (std::bad_variant_access) {
				if (options.temporalTypesAsUserData) {
					luaTable[std::get<size_t>(keyOrIndex)] = v;
				} else {
					auto t = luaTable.create();
					v.toTable(t);
					luaTable[std::get<size_t>(keyOrIndex)] = t;
				}
			}

			break;
		}
		case toml::node_type::date_time: {
			auto v = *(*node->as_date_time());

			auto dt = TOMLDateTime(TOMLDate(v.date), TOMLTime(v.time));

			if (v.offset.has_value()) { dt.setTimeOffset(TOMLTimeOffset(v.offset.value())); }

			try {
				if (options.temporalTypesAsUserData) {
					luaTable[std::get<std::string>(keyOrIndex)] = dt;
				} else {
					auto t = luaTable.create();
					dt.toTable(t);
					luaTable[std::get<std::string>(keyOrIndex)] = t;
				}
			} catch (std::bad_variant_access) {
				if (options.temporalTypesAsUserData) {
					luaTable[std::get<size_t>(keyOrIndex)] = dt;
				} else {
					auto t = luaTable.create();
					dt.toTable(t);
					luaTable[std::get<size_t>(keyOrIndex)] = t;
				}
			}
			break;
		}

		default:
			break;
	}
}

void tomlArrayToLuaArray(toml::array & tomlArray, sol::table & luaTable, Options options) {
	size_t size = tomlArray.size();

	for (size_t i = 0; i < size; i++) {
		auto element = tomlArray.get(i);
		size_t index = i + 1;
		insertNodeInTable(luaTable, index, element, options);
	};
}

/// Convert `luaTable` into a `toml::table`.
void tomlToLuaTable(toml::table * table, sol::table & lTable, Options options) {
	if (table->is_inline()) {
		auto metatable = lTable.create();
		metatable["inline"] = true;
		lTable[sol::metatable_key] = metatable;
	};

	for (auto && [key, value] : *table) {
		auto k = std::string(key);
		insertNodeInTable(lTable, k, &value, options);
	}
}
