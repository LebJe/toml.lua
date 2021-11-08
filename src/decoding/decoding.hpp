#ifndef DECODING_H
#define DECODING_H

#include "DateAndTime/dateAndTime.hpp"
#include <cstddef>
#include <sol/sol.hpp>
#include <string>
#include <toml.hpp>

/// Convert `luaTable` into a `toml::table`.
void tomlToLuaTable(toml::table & table, sol::table & lTable);

void insertNodeInTable(sol::table & luaTable, std::string * key, size_t * index, void * value);

void tomlArrayToLuaArray(toml::array & tomlArray, sol::table & luaTable);

#endif /* DECODING_H */
