# toml.lua

**TOML parser and serializer for Lua. Powered by [toml++](https://github.com/marzer/tomlplusplus/).**

[![Build and Test](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest.yml/badge.svg)](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest.yml)

toml.lua is a [Lua](https://www.lua.org) wrapper around [toml++](https://github.com/marzer/tomlplusplus/), allowing you to parse and serialize [TOML](https://toml.io) files in Lua.

## Table of Contents

<!--ts-->

-   [toml.lua](#tomllua)
    -   [Table of Contents](#table-of-contents)
    -   [Installation](#installation)
        -   [Requirements](#requirements)
        -   [LuaRocks](#luarocks)
        -   [Manual Compilation](#manual-compilation)
    -   [Usage](#usage)
        -   [Encoding](#encoding)
        -   [Decoding](#decoding)
        -   [Error Handling](#error-handling)
        -   [TOML To JSON](#toml-to-json)
    -   [Dependencies](#dependencies)
    -   [Licenses](#licenses)
    -   [Contributing](#contributing)

<!-- Added by: lebje, at: Fri Oct 29 22:15:04 EDT 2021 -->

<!--te-->

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)

> NOTE: TOML [date](https://toml.io/en/v1.0.0#local-date) and [time](https://toml.io/en/v1.0.0#local-time) and [date-time](https://toml.io/en/v1.0.0#offset-date-time) are not currently supported.

## Installation

### Requirements

-   A C++ 17 compiler
-   [CMake](https://cmake.org)
-   `lua.h`, `lualib.h`, `lauxlib.h`
-   Lua >= 5.3 or LuaJIT

### LuaRocks

```bash
luarocks install toml
```

### Manual Compilation

1. Run `cmake -H. -Bbuild -G<generator-name>` to generate the required files.

> If you have a non standard Lua install location, add the environment variable `LUA_DIR` and have it point to the directory containing the `include` and `lib` folders for your Lua installation. For example:
> `LUA_DIR=/usr/local/openresty/luajit cmake -H. -Bbuild -G<generator-name>`

2. Run `cmake --build build --config Release` to build the project.
3. You will find the `toml.so` (or `toml.dll`) dynamic library in the `build` folder.

> Tip: use `cmake --help` to see a list of available generator names.

> The above is based off of [xpol/lua-rapidjson's README](https://github.com/xpol/lua-rapidjson#usage-without-luarocks).

## Usage

### Encoding

```lua
local tomlStr = [[
a = 1275892
b = "Hello, World!"
c = true
d = 124.2548

[e]
f = [1, 2, 3, "4", 5.142]
]]

local toml = require("toml")
local inspect = require("inspect")

local succeeded, table = pcall(toml.decode, tomlStr)

if succeeded then
-- Use `table`.
	print(inspect(table))
else
-- Error details are in `table`.
end

--[[
{
  a = 1275892,
  b = "Hello, World!",
  c = 1,
  d = 124.2548,
  e = {
    f = { 1, 2, 3, "4", 5.142 }
  }
}
--]]
```

### Decoding

```lua
local table = {
  a = 1275892,
  b = "Hello, World!",
  c = 1,
  d = 124.2548,
  e = {
    f = { 1, 2, 3, "4", 5.142 }
  }
}

local toml = require("toml")
local tomlDocument = toml.encode(table)

print(tomlDocument)

--[[
a = 1275892.0
b = 'Hello, World!'
c = 1.0
d = 124.2548

[e]
f = [ 1.0, 2.0, 3.0, '4', 5.142 ]
--]]
```

### Error Handling

```lua
local tomlStr = [[
a = 1275892
b = "Hello, World!"
c = true
d = 124. # <-- ERROR: "Expected decimal digit"

[e]
f = [1, 2, 3, "4", 5.142]
]]

local toml = require("toml")
local inspect = require("inspect")

local succeeded, table = pcall(toml.decode, tomlStr)

if succeeded then
	-- Use decoded table.
	print(inspect(table))
else
	-- Error details are in `table`.
	print(inspect(table))

	--[[
	{
	  begin = {
	    column = 9,
	    line = 4
	  },
	  end = {
	    column = 9,
	    line = 4
	  },
	  formattedReason = "Error while parsing floating-point: expected decimal digit, saw '\\n' (at line 4, column 9)",
	  reason = "Error while parsing floating-point: expected decimal digit, saw '\\n'"
	}
--]]
end
```

### TOML To JSON

```lua
local tomlStr = [[
a = 1275892
b = "Hello, World!"
c = true
d = 124.2548

[e]
f = [1, 2, 3, "4", 5.142]
]]

local toml = require("toml")
local json = toml.tomlToJSON(tomlStr)

print(json)

--[[
{
	"a" : 1275892,
    "b" : "Hello, World!",
    "c" : true,
    "d" : 124.2548,
    "e" : {
        "f" : [
            1,
            2,
            3,
            "4",
            5.142
        ]
    }
}
--]]
```

## Dependencies

-   [toml++](https://github.com/marzer/tomlplusplus/)
-   [kaguya](https://github.com/satoren/kaguya)

## Licenses

The [toml++](https://github.com/marzer/tomlplusplus/) license is available in the `tomlplusplus` directory in the `LICENSE` file.

The [kaguya](https://github.com/satoren/kaguya) license is available in the `kaguya` directory in the `LICENSE` file.

## Contributing

Before committing, please install [pre-commit](https://pre-commit.com), [clang-format](https://clang.llvm.org/docs/ClangFormat.html), and [Prettier](https://prettier.io), then install the pre-commit hook:

```bash
$ brew bundle # install the packages specified in Brewfile
$ pre-commit install

# Commit your changes.
```

To install pre-commit on other platforms, refer to the [documentation](https://pre-commit.com/#install).
