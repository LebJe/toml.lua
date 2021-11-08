#ifndef ENCODING_H
#define ENCODING_H

#include <DateAndTime/dateAndTime.hpp>
#include <cstddef>
#include <sol/sol.hpp>
#include <string>
#include <toml.hpp>
#include <utilities/utilities.hpp>

/// Convert `LuaTable` into a `toml::table`.
toml::table tomlTableFromLuaTable(sol::table luaTable);
toml::array tomlArrayFromLuaArray(sol::table luaArray);
void * luaValueToTomlNode(sol::object & luaValue);

#endif /* ENCODING_H */