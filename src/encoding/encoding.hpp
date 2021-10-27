#include "../kaguya.hpp"
#include "../toml.hpp"
#include <cstddef>
#include <string>

/// Convert `LuaTable` into a `toml::table`.
toml::table tomlTableFromLuaTable(kaguya::LuaTable & luaTable);
toml::array tomlArrayFromLuaArray(kaguya::LuaTable & luaArray);
void * luaValueToTomlNode(kaguya::LuaRef & luaValue);
