# toml.lua

**TOML v1.0.0 parser and serializer for Lua. Powered by [toml++](https://github.com/marzer/tomlplusplus/).**

[![Build and Test on MacOS](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-MacOS.yml/badge.svg)](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-MacOS.yml)
[![Build and Test on Linux](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-Linux.yml/badge.svg)](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-Linux.yml)
[![Build and Test on Windows](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-Windows.yml/badge.svg)](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-Windows.yml)

toml.lua is a [Lua](https://www.lua.org) wrapper around [toml++](https://github.com/marzer/tomlplusplus/), allowing you to parse and serialize [TOML](https://toml.io) files in Lua.

## Table of Contents

<!--ts-->

-   [toml.lua](#tomllua)
    -   [Table of Contents](#table-of-contents)
    -   [Installation](#installation)
        -   [Requirements](#requirements)
        -   [Install LuaJIT on Windows](#install-luajit-on-windows)
        -   [LuaRocks](#luarocks)
            -   [MacOS and Linux](#macos-and-linux)
        -   [Manual Compilation](#manual-compilation)
            -   [MacOS and Linux](#macos-and-linux-1)
            -   [Windows](#windows)
                -   [Build with MinGW](#build-with-mingw)
                -   [Build with LLVM](#build-with-llvm)
    -   [Usage](#usage)
        -   [Decoding](#decoding)
        -   [Encoding](#encoding)
        -   [Error Handling](#error-handling)
        -   [TOML Conversion](#toml-conversion)
            -   [JSON](#json)
            -   [YAML](#yaml)
        -   [Output Formatting](#output-formatting)
    -   [Dependencies](#dependencies)
    -   [Licenses](#licenses)
    -   [Contributing](#contributing)

<!-- Added by: lebje, at: Sun Jun 12 16:12:23 EDT 2022 -->

<!--te-->

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)

## Installation

### Requirements

-   A C++ 17 compiler
-   [CMake](https://cmake.org)
-   Lua C headers (`lua.h`, `lualib.h`, and `lauxlib.h`)
-   Lua library (e.g. `liblua51.<so|dylib|dll>`)
-   Lua >= 5.1 or LuaJIT

### Install LuaJIT on Windows

If you don't have LuaJIT, or your installation does not have the Lua headers, you can:

1. Install [MinGW](https://www.mingw-w64.org/) (`choco install mingw`)

2. Run `scripts\buildLuaJIT.ps1`:

```powershell
powershell scripts\buildLuaJIT.ps1 -installDir "LuaJIT"
```

to build and install LuaJIT.

### LuaRocks

#### MacOS and Linux

```bash
luarocks install toml
```

#### Windows

##### LLVM

If you have installed Clang, and CMake is configured to use it, you can run:

```powershell
luarocks install toml
```

##### MinGW

If you have installed MinGW, and CMake is configured to use it, you can run:

```powershell
luarocks config variables.LINK_FLAGS "path\to\LuaJIT\bin\lua51.dll"
luarocks install toml
luarocks config variables.LINK_FLAGS --unset
```

### Manual Compilation

#### MacOS and Linux

1. Run `cmake -S . -B build -G <generator-name>` to generate the required files.

> If you have a non standard Lua install location, add the environment variable `LUA_DIR` and have it point to the directory containing the `include` and `lib` folders for your Lua installation. For example:
> `LUA_DIR=/usr/local/openresty/luajit cmake -S . -B build -G <generator-name>`

2. Run `cmake --build build --config Release` to build the project.
3. You will find the `toml.so` dynamic library in the `build` folder.

> Tip: use `cmake --help` to see a list of available generator names.

> The above is based off of [xpol/lua-rapidjson's README](https://github.com/xpol/lua-rapidjson#usage-without-luarocks).

#### Windows

If LuaJIT is not installed, or your installation does not have the Lua headers, Follow [this part of the README](#install-luajit-on-windows) to install LuaJIT.

##### Build with MinGW

Install [MinGW](https://www.mingw-w64.org/) (`choco install mingw`), then:

```powershell
cmake.exe -S . -B build -G "MinGW Makefiles" -DLUA_INCLUDE_DIR="path\to\LuaJIT\include" -DLINK_FLAGS="path\to\LuaJIT\bin\lua51.dll"

cmake.exe --build build --config Release
```

You'll find the `toml.dll` file in the `build` directory.

##### Build with LLVM

Install [LLVM](https://llvm.org) and [Ninja](https://ninja-build.org/) (`choco install llvm ninja`), then:

```powershell
cmake.exe -S . -B build -G "Ninja Multi-Config" -DLUA_INCLUDE_DIR="path\to\LuaJIT\include"

cmake.exe --build build --config Release
```

You'll find the `toml.dll` file in the `build` directory.

## Usage

### Decoding

```lua
local tomlStr = [[
a = 1275892
b = 'Hello, World!'
c = true
d = 124.2548

[e]
f = [ 1, 2, 3, '4', 5.142 ]
g = 1979-05-27
h = 07:32:00
i = 1979-05-27T07:32:00-07:00
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
	c = true,
	d = 124.2548,
	e = {
		f = { 1, 2, 3, "4", 5.142 },
		g = <userdata 1> -- 1979-05-27,
		h = <userdata 2> -- 07:32:00,
		i = <userdata 3> -- 1979-05-27T07:32:00-07:00
	}
}
--]]
```

### Encoding

```lua
local toml = require("toml")

local table = {
	a = 1275892,
	b = "Hello, World!",
	c = true,
	d = 124.2548,
	e = {
		f = { 1, 2, 3, "4", 5.142 },
		g = toml.Date.new(1979,   05,     27),
		--                year   month   day

		h = toml.Time.new( 7,     32,      0,        0),
		--                hour   minute  second   nanoSecond

		i = toml.DateTime.new(
			toml.Date.new(1979, 05, 27),
			toml.Time.new(7, 32, 0, 0),

			toml.TimeOffset.new(  -7,     0)
			--                   hour   minute
		)
	}
}

local tomlDocument = toml.encode(table)

print(tomlDocument)

--[[
a = 1275892
b = 'Hello, World!'
c = true
d = 124.2548

[e]
f = [ 1, 2, 3, '4', 5.142 ]
g = 1979-05-27
h = 07:32:00
i = 1979-05-27T07:32:00-07:00
--]]
```

### Error Handling

```lua
local tomlStr = [[
a = 1275892
b = 'Hello, World!'
c = true
d = 124. # <-- ERROR: "Expected decimal digit"

[e]
f = [ 1, 2, 3, '4', 5.142 ]
g = 1979-05-27
h = 07:32:00
i = 1979-05-27T07:32:00-07:00
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
		reason = "Error while parsing floating-point: expected decimal digit, saw '\\n'"
	}
--]]
end
```

### TOML Conversion

```lua
local toml = require("toml")

local tomlStr = [[
a = 1275892
b = 'Hello, World!'
c = true
d = 124.2548

[e]
f = [ 1, 2, 3, '4', 5.142 ]
g = 1979-05-27
h = 07:32:00
i = 1979-05-27T07:32:00-07:00
]]
```

#### JSON

```lua
local json = toml.tomlToJSON(tomlStr)
print(json)
```

#### YAML

```lua
local yaml = toml.tomlToYAML(tomlStr)
print(yaml)
```

### Output Formatting

`toml.encode`, `toml.tomlToJSON`, and `toml.tomlToYAML` all take an optional second parameter: a table containing keys that disable or enable different formatting options.
Passing an empty table removes all options, while not providing a table will use the default options.

```lua
{
	--- Dates and times will be emitted as quoted strings.
	quoteDatesAndTimes = true,

	--- Infinities and NaNs will be emitted as quoted strings.
	quoteInfinitesAndNaNs = false,

	--- Strings will be emitted as single-quoted literal strings where possible.
	allowLiteralStrings = false,

	--- Strings containing newlines will be emitted as triple-quoted 'multi-line' strings where possible.
	allowMultiLineStrings = false,

	--- Allow real tab characters in string literals (as opposed to the escaped form `\t`).
	allowRealTabsInStrings = false,

	--- Allow non-ASCII characters in strings (as opposed to their escaped form, e.g. `\u00DA`).
	allow_unicode_strings = true,

	--- Allow integers with #value_flags::format_as_binary to be emitted as binary.
	allowBinaryIntegers = true,

	--- Allow integers with #value_flags::format_as_octal to be emitted as octal.
	allowOctalIntegers = true,

	--- Allow integers with #value_flags::format_as_hexadecimal to be emitted as hexadecimal.
	allowHexadecimalIntegers = true,

	--- Apply indentation to tables nested within other tables/arrays.
	indentSubTables = true,

	--- Apply indentation to array elements when the array is forced to wrap over multiple lines.
	indentArrayElements = true,

	--- Combination of `indentSubTables` and `indentArrayElements`.
	indentation = true,

	--- Emit floating-point values with relaxed (human-friendly) precision.
	relaxedFloatPrecision = false
}
```

> The comments for the options are from [the tomlplusplus documentation](https://marzer.github.io/tomlplusplus/namespacetoml.html#a2102aa80bc57783d96180f36e1f64f6a)

## Dependencies

-   [toml++](https://github.com/marzer/tomlplusplus/)
-   [sol2](https://github.com/ThePhD/sol2)
-   [magic_enum](https://github.com/Neargye/magic_enum)

## Licenses

The [toml++](https://github.com/marzer/tomlplusplus/) license is available at [https://github.com/marzer/tomlplusplus/blob/master/LICENSE](https://github.com/marzer/tomlplusplus/blob/master/LICENSE).

The [sol2](https://github.com/ThePhD/sol2) license is available at [https://github.com/ThePhD/sol2/blob/develop/LICENSE.txt](https://github.com/ThePhD/sol2/blob/develop/LICENSE.txt).

The [magic_enum](https://github.com/Neargye/magic_enum) license is available at [https://github.com/Neargye/magic_enum/blob/master/LICENSE](https://github.com/Neargye/magic_enum/blob/master/LICENSE).

## Contributing

Before committing, please install [pre-commit](https://pre-commit.com), [clang-format](https://clang.llvm.org/docs/ClangFormat.html), [StyLua](https://github.com/JohnnyMorganz/StyLua), and [Prettier](https://prettier.io), then install the pre-commit hooks. On MacOS, it would look like:

```bash
$ brew bundle # install the packages specified in Brewfile
$ pre-commit install

# Commit your changes.
```

To install pre-commit on other platforms, refer to the [documentation](https://pre-commit.com/#install).
