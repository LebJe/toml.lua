#include "decoding.hpp"

void insertNodeInTable(
	kaguya::LuaTable & luaTable, std::string * key, size_t * index, void * value) {
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
			auto v = (int64_t)*node->as_integer();

			if (index != NULL) {
				luaTable[*index] = v;
			} else {
				luaTable[*key] = v;
			}

			break;
		}

		case toml::node_type::floating_point: {
			auto v = (double)*node->as_floating_point();

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

		case toml::node_type::none:
		case toml::node_type::table:
		case toml::node_type::array:
		case toml::node_type::date:
		case toml::node_type::time:
		case toml::node_type::date_time:
			break;
	}
}

void tomlArrayToLuaArray(toml::array & tomlArray, kaguya::LuaTable & luaTable) {
	size_t size = tomlArray.size();

	for (size_t i = 0; i < size; i++) {
		auto element = tomlArray.get(i);
		size_t index = i + 1;

		switch (element->type()) {
			case toml::node_type::table: {
				auto newLTable = kaguya::LuaTable(luaTable.state());

				tomlToLuaTable(*element->as_table(), newLTable);

				newLTable.push();
				luaTable[index] = newLTable;

				break;
			}

			case toml::node_type::array: {
				auto newLTable = kaguya::LuaTable(luaTable.state());

				tomlArrayToLuaArray(*element->as_array(), newLTable);

				newLTable.push();
				luaTable[index] = newLTable;

				break;
			}
			case toml::node_type::none:
			case toml::node_type::string:
			case toml::node_type::integer:
			case toml::node_type::floating_point:
			case toml::node_type::boolean:
			case toml::node_type::date:
			case toml::node_type::time:
			case toml::node_type::date_time: {
				insertNodeInTable(luaTable, NULL, &index, reinterpret_cast<void *>(element));
				break;
			}
		}
	};
}

/// Convert `luaTable` into a `toml::table`.
void tomlToLuaTable(toml::table & table, kaguya::LuaTable & lTable) {
	for (auto && [key, value] : table) {
		switch (value.type()) {
			case toml::node_type::table: {
				auto newLTable = kaguya::LuaTable(lTable.state());

				tomlToLuaTable(*value.as_table(), newLTable);

				newLTable.push();
				lTable[key] = newLTable;

				break;
			}

			case toml::node_type::array: {
				auto newLTable = kaguya::LuaTable(lTable.state());
				auto array = value.as_array();

				tomlArrayToLuaArray(*array, newLTable);

				newLTable.push();
				lTable[key] = newLTable;
				break;
			}

			case toml::node_type::string:
			case toml::node_type::integer:
			case toml::node_type::floating_point:
			case toml::node_type::boolean:
			case toml::node_type::date:
			case toml::node_type::time:
			case toml::node_type::date_time:
			case toml::node_type::none: {
				auto k = std::string(key);
				insertNodeInTable(lTable, &k, NULL, reinterpret_cast<void *>(&value));
				break;
			}
		}
	}
}
