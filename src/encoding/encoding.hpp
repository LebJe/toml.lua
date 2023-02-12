#ifndef ENCODING_H
#define ENCODING_H

#include <cstddef>
#include <sol/sol.hpp>
#include <string>
#include <toml.hpp>
#include <utilities/utilities.hpp>

/// Convert `LuaTable` into a `toml::table`.
toml::table * tomlTableFromLuaTable(sol::table luaTable);
toml::array * tomlArrayFromLuaArray(sol::table luaArray);
toml::node * luaValueToTomlNode(sol::object & luaValue, toml::value_flags * flags);

#endif /* ENCODING_H */
