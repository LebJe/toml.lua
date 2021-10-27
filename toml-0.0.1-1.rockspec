package = "toml"
version = "0.0.1-1"

local v = version:gsub("%-%d", "")

source = {
	url = "git://github.com/LebJe/toml.lua.git",
	tag = v,
}

description = {
	summary = "TOML parser and serializer for Lua. Powered by toml++.",
	detailed = "toml.lua is a Lua wrapper around [toml++](https://github.com/marzer/tomlplusplus/), allowing you to parse and serialize TOML files in Lua.",
	license = "MIT",
	homepage = "http://github.com/LebJe/toml.lua",
}

dependencies = { "lua >= 5.3" }

build = {
	type = "cmake",
	variables = {
		CMAKE_INSTALL_PREFIX = "$(PREFIX)",
		LUA_INCLUDE_DIR = "$(LUA_INCDIR)",
		TOML_LUA_VERSION = v,
	},
}
