#include "../kaguya.hpp"
#include "../toml.hpp"
#include <cstddef>
#include <string>

/// Convert `luaTable` into a `toml::table`.
void tomlToLuaTable(toml::table & table, kaguya::LuaTable & lTable);

void insertNodeInTable(
	kaguya::LuaTable & luaTable, std::string * key, size_t * index, void * value);

void tomlArrayToLuaArray(toml::array & tomlArray, kaguya::LuaTable & luaTable);
