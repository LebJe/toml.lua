# toml.lua

**TOML v1.0.0 parser and serializer for Lua. Powered by [toml++](https://github.com/marzer/tomlplusplus/).**

[![Build and Test on MacOS](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-MacOS.yml/badge.svg)](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-MacOS.yml)
[![Build and Test on Linux](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-Linux.yml/badge.svg)](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-Linux.yml)
[![Build and Test on Windows](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-Windows.yml/badge.svg)](https://github.com/LebJe/toml.lua/actions/workflows/buildAndTest-Windows.yml)

toml.lua is a [Lua](https://www.lua.org) wrapper around [toml++](https://github.com/marzer/tomlplusplus/), allowing you to parse and serialize [TOML](https://toml.io) in Lua.

## Table of Contents

<!--ts-->

-   [toml.lua](#tomllua)
    -   [Table of Contents](#table-of-contents)
    -   [Installation](#installation)
        -   [Requirements](#requirements)
        -   [LuaRocks](#luarocks)
            -   [MacOS and Linux](#macos-and-linux)
            -   [Windows](#windows)
                -   [LLVM](#llvm)
                -   [MinGW](#mingw)
        -   [Manual Compilation](#manual-compilation)
            -   [MacOS and Linux](#macos-and-linux-1)
            -   [Windows](#windows-1)
                -   [Build with MinGW](#build-with-mingw)
                -   [Build with LLVM](#build-with-llvm)
                -   [Install LuaJIT](#install-luajit)
    -   [Usage](#usage)
        -   [Decoding](#decoding)
            -   [Decoding Options](#decoding-options)
                -   [temporalTypesAsUserData](#temporaltypesasuserdata)
        -   [Encoding](#encoding)
        -   [Error Handling](#error-handling)
        -   [Inline Tables](#inline-tables)
        -   [TOML Conversion](#toml-conversion)
            -   [JSON](#json)
            -   [YAML](#yaml)
        -   [Output Formatting](#output-formatting)
            -   [Formatting TOML, JSON, or YAML](#formatting-toml-json-or-yaml)
        -   [Date and Time](#date-and-time)
    -   [Dependencies](#dependencies)
    -   [Licenses](#licenses)
    -   [Contributing](#contributing)

<!-- Added by: lebje, at: Sun Feb 19 22:27:48 EST 2023 -->

<!--te-->

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)

## Installation

### Requirements

-   A C++ 17 compiler
-   [CMake](https://cmake.org)
-   Lua C headers (`lua.h`, `lualib.h`, and `lauxlib.h`)
-   Lua library (e.g. `liblua51.<so|dylib|dll>`)
-   Lua >= 5.1 or LuaJIT

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

If LuaJIT is not installed, or your installation does not have the Lua headers, go to [install LuaJIT](#install-luajit).

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

##### Install LuaJIT

If you don't have LuaJIT, or your installation does not have the Lua headers, you can:

1. Install [MinGW](https://www.mingw-w64.org/) (`choco install mingw`)

2. Run `scripts\buildLuaJIT.ps1`:

```powershell
powershell scripts\buildLuaJIT.ps1 -installDir "LuaJIT"
```

to build and install LuaJIT.

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

-- Decode from string
local succeeded, table = pcall(toml.decode, tomlStr)

-- Decode from file
succeeded, table = pcall(toml.decodeFromFile, "configuration.toml")

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

#### Decoding Options

##### `temporalTypesAsUserData`

-   `temporalTypesAsUserData = true`: The userdata types `toml.Date`, `toml.Time`, and `toml.DateTime` are used to represent TOML date and time types.

-   `temporalTypesAsUserData = false`: Lua tables are used to represent TOML date and time types.

```lua
local tomlStr = [[
date = 1979-05-27
time = 07:32:00
datetime = 1979-05-27T07:32:00-07:00
]]

local table1 = toml.decode(tomlStr, { temporalTypesAsUserData = true })
local table2 = toml.decode(tomlStr, { temporalTypesAsUserData = false })

print(inspect(table1))
--[[
{
	date = <userdata 1> -- 1979-05-27, <-- toml.Date
	time = <userdata 2> -- 07:32:00, <-- toml.Time
	datetime = <userdata 3> -- 1979-05-27T07:32:00-07:00 <-- toml.DateTime
}
--]]

print(inspect(table2))
--[[
{
  date = {
    day = 27,
    month = 5,
    year = 1979
  },
  datetime = {
    date = {
      day = 27,
      month = 5,
      year = 1979
    },
    time = {
      hour = 7,
      minute = 32,
      nanoSecond = 0,
      second = 0
    },
    timeOffset = {
      minutes = -420
    }
  },
  time = {
    hour = 7,
    minute = 32,
    nanoSecond = 0,
    second = 0
  }
}
--]]
```

### Encoding

```lua
local toml = require("toml")

-- Inline tables: https://toml.io/en/v1.0.0#inline-table
local inlineTable = {
	a = 1275892,
	b = "Hello, World!",
	c = true,
	d = 124.2548,
}

-- Make the table inline.
setmetatable(inlineTable, { inline = true })

local table = {

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
	},
	inlineTable = inlineTable
}

-- Encode to string
local succeeded, documentOrErrorMessage = pcall(toml.encode, table)

-- Encode to file, this will **append** to the file.
succeeded, documentOrErrorMessage = pcall(toml.encodeToFile, table, "configuration.toml")

-- Encode to file, this will **overwrite** the file.
succeeded, documentOrErrorMessage = pcall(toml.encodeToFile, table, { file = "configuration.toml", overwrite = true })

if succeeded then
	-- Successfully encoded to string / wrote to file
	print(tomlDocumentOrErrorMessage)
else
-- Error occurred
	print(tomlDocumentOrErrorMessage)
end

--[[
inlineTable = { a = 1275892, b = "Hello, World!", c = true, d = 124.2548 }

[e]
f = [ 1, 2, 3, "4", 5.1420000000000003 ]
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

### Inline Tables

Use `setmetatable(myTable, { inline = true })` to create an [inline table](https://toml.io/en/v1.0.0#inline-table).

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
-- Convert from a string
local json = toml.toJSON(tomlStr)

-- or from a table
json = toml.toJSON(toml.decode(tomlStr))

print(json)
```

#### YAML

```lua
local yaml = toml.toYAML(tomlStr)
yaml = toml.toYAML(toml.decode(tomlStr))
print(yaml)
```

### Output Formatting

<!---
Uncomment once toml.Int works.

#### Formatting Integers
```lua
local toml = require("toml")

local normalIntegers = {
	int1 = 2582
	int2 = 3483
	int3 = 5971
}
print(toml.encode(normalIntegers))
--[[
int1 = 2582
int2 = 3483
int3 = 5791
--]]

local formattedIntegers = {
	int1 = toml.Int.new(2582, toml.formatting.int.octal),
	int2 = toml.Int.new(3483, toml.formatting.int.binary),
	int3 = toml.Int.new(5791, toml.formatting.int.hexadecimal)
}

print(toml.encode(formattedIntegers))
--[[
int1 = 0o5026
int2 = 0b110110011011
int3 = 0x169F
--]]
```
-->

#### Formatting TOML, JSON, or YAML

`toml.encode`, `toml.encodeToFile`, `toml.toJSON`, and `toml.toYAML` all take an optional second parameter: a table containing keys that disable or enable different formatting options.
Passing an empty table removes all options, while not providing a table will use the default options.

```lua
{
	--- Dates and times will be emitted as quoted strings.
	quoteDatesAndTimes = false,

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

	--- Allow integers with `toml.formatting.int.binary` to be emitted as binary. (Not implemented yet)
	allowBinaryIntegers = true,

	--- Allow integers with `toml.formatting.int.octal` to be emitted as octal. (Not implemented yet)
	allowOctalIntegers = true,

	--- Allow integers with `toml.formatting.int.hexadecimal` to be emitted as hexadecimal. (Not implemented yet)
	allowHexadecimalIntegers = true,

	--- Apply indentation to tables nested within other tables/arrays.
	indentSubTables = true,

	--- Apply indentation to array elements when the array is forced to wrap over multiple lines.
	indentArrayElements = true,

	--- Combination of `indentSubTables` and `indentArrayElements`.
	indentation = true,

	--- Emit floating-point values with relaxed (human-friendly) precision.
	---
	--- Warning: Setting this flag may cause serialized documents to no longer round-
	--- trip correctly since floats might have a less precise value upon being written out
	--- than they did when being read in. Use this flag at your own risk.
	relaxedFloatPrecision = false,

	--- Avoids the use of whitespace around key-value pairs.
	terseKeyValuePairs = false
}
```

### Date and Time

(Creating Date, Time, and DateTime is shown in [the encoding section](#encoding))

```lua
	record Date
		year: number
		month: number
		day: number

		new: function(year: number, month: number, day: number): Date
	end

	record Time
		hour: number
		minute: number
		second: number
		nanoSecond: number

		new: function (
			hour: number,
			minute: number,
			second: number,
			nanoSecond: number
		): Time
	end

	record TimeOffset
		minutes: number

		new: function (hours: number, minutes: number): TimeOffset
	end

	record DateTime
		date: Date
		time: Time
		TimeOffset: nil | TimeOffset

		new: function(date: Date, time: Time): DateTime
		new: function(date: Date, time: Time, timeOffset: TimeOffset): DateTime
	end
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
