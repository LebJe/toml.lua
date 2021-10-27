#include "encoding.hpp"

void * luaValueToTomlNode(kaguya::LuaRef & luaValue) {
	switch (luaValue.type()) {
		case kaguya::LuaTable::TYPE_NUMBER: {
			if (luaValue.isType<double>()) {
				auto node = new toml::value((double)luaValue);
				return reinterpret_cast<void *>(node);
			} else {
				auto node = new toml::value((int64_t)luaValue);
				return reinterpret_cast<void *>(node);
			}
			break;
		}
		case kaguya::LuaTable::TYPE_BOOLEAN: {
			auto node = new toml::value((bool)luaValue);
			return reinterpret_cast<void *>(node);
			break;
		}
		case kaguya::LuaTable::TYPE_STRING: {
			auto node = new toml::value((std::string)luaValue);
			return reinterpret_cast<void *>(node);
			break;
		}
		case kaguya::LuaTable::TYPE_TABLE: {
			if (luaValue.keys().front().isType<int64_t>()) {
				auto table = new kaguya::LuaTable(luaValue);
				return reinterpret_cast<void *>(new toml::array(tomlArrayFromLuaArray(*table)));
			} else {
				auto table = new kaguya::LuaTable(luaValue);
				return reinterpret_cast<void *>(new toml::table(tomlTableFromLuaTable(*table)));
			}

			break;
		}
	}

	return NULL;
}

toml::array tomlArrayFromLuaArray(kaguya::LuaTable & luaArray) {
	auto tomlArray = toml::array();

	auto keysAndValues = luaArray.map();

	for (auto && [key, value] : keysAndValues) {
		if (!key.isType<int64_t>()) {
			throw kaguya::LuaTypeMismatch((std::string("The array index ") + std::string(key) +
										   "should be a integer, not a " + key.typeName())
											  .c_str());
		}
		if (auto v = reinterpret_cast<toml::node *>(luaValueToTomlNode(value))) {

			tomlArray.push_back(*v);
		}
	}

	return tomlArray;
}

toml::table tomlTableFromLuaTable(kaguya::LuaTable & luaTable) {
	auto table = toml::table();

	auto keysAndValues = luaTable.map();

	for (auto && [key, value] : keysAndValues) {
		if (!key.isType<std::string>()) {
			throw kaguya::LuaTypeMismatch((std::string("The key ") + std::string(key) +
										   "should be a string, not a " + key.typeName())
											  .c_str());
		}

		if (auto v = reinterpret_cast<toml::node *>(luaValueToTomlNode(value))) {
			table.insert(std::string(key), *v);
		}
	}

	return table;
}
