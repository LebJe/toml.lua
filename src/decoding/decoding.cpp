#include "decoding.hpp"

void insertNodeInTable(sol::table & luaTable, std::string * key, size_t * index, void * value) {
	auto node = reinterpret_cast<toml::node *>(value);

	switch (node->type()) {
		case toml::node_type::string: {
			auto v = std::string(*node->as_string());

			if (index != NULL) {
				luaTable[*index] = v.c_str();
			} else {
				luaTable[*key] = v.c_str();
			}
			break;
		}

		case toml::node_type::integer: {
			auto v = int64_t { *node->as_integer() };

			if (index != NULL) {
				luaTable[*index] = v;
			} else {
				luaTable[*key] = v;
			}

			break;
		}

		case toml::node_type::floating_point: {
			auto v = double { *node->as_floating_point() };

			if (index != NULL) {
				luaTable[*index] = v;
			} else {
				luaTable[*key] = v;
			}

			break;
		}

		case toml::node_type::boolean: {
			auto v = *node->as_boolean() ? 1 : 0;

			if (index != NULL) {
				luaTable[*index] = v;
			} else {
				luaTable[*key] = v;
			}

			break;
		}
		case toml::node_type::array: {
			auto newLTable = sol::table(luaTable.lua_state(), sol::create);

			tomlArrayToLuaArray(*node->as_array(), newLTable);

			newLTable.push();

			if (index != NULL) {
				luaTable[*index] = newLTable;
			} else {
				luaTable[*key] = newLTable;
			}

			break;
		}
		case toml::node_type::table: {
			auto newLTable = sol::table(luaTable.lua_state(), sol::create);

			tomlToLuaTable(*node->as_table(), newLTable);

			newLTable.push();

			if (index != NULL) {
				luaTable[*index] = newLTable;
			} else {
				luaTable[*key] = newLTable;
			}

			break;
		}
		case toml::node_type::date: {
			auto v = TOMLDate(*(*node->as_date()));

			if (index != NULL) {
				luaTable[*index] = v;
			} else {
				luaTable[*key] = v;
			}

			break;
		}
		case toml::node_type::time: {
			auto v = TOMLTime(*(*node->as_time()));

			if (index != NULL) {
				luaTable[*index] = v;
			} else {
				luaTable[*key] = v;
			}

			break;
		}
		case toml::node_type::date_time: {
			auto v = *(*node->as_date_time());

			auto dt = TOMLDateTime(TOMLDate(v.date), TOMLTime(v.time));

			if (v.offset.has_value()) { dt.setTimeOffset(TOMLTimeOffset(v.offset.value())); }

			if (index != NULL) {
				luaTable[*index] = dt;
			} else {
				luaTable[*key] = dt;
			}

			break;
		}

		default:
			break;
	}
}

void tomlArrayToLuaArray(toml::array & tomlArray, sol::table & luaTable) {
	size_t size = tomlArray.size();

	for (size_t i = 0; i < size; i++) {
		auto element = tomlArray.get(i);
		size_t index = i + 1;
		insertNodeInTable(luaTable, NULL, &index, reinterpret_cast<void *>(element));
	};
}

/// Convert `luaTable` into a `toml::table`.
void tomlToLuaTable(toml::table & table, sol::table & lTable) {
	for (auto && [key, value] : table) {
		auto k = std::string(key);
		insertNodeInTable(lTable, &k, NULL, reinterpret_cast<void *>(&value));
	}
}
