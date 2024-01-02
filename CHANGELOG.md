# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.4.0](https://github.com/LebJe/toml.lua/releases/tag/0.4.0) - 2024-01-02

### Added

-   `toml.Int` for formatted integers.

```lua
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

-   `formattedIntsAsUserdata` can be passed to the options table of `toml.decode` (see "Decoding Options" in the README).
-   Updated to toml++ v3.4.0.
-   Updated to MagicEnum v0.9.5.
-   toml.lua compiles with MSVC.

## [0.3.0](https://github.com/LebJe/toml.lua/releases/tag/0.3.0) - 2023-02-19

### Added

-   `toml.decodeFromFile(filePath: string)`:
    -   Decodes a TOML document at `filePath`. Throws the same errors as `toml.decode`.
-   `toml.encodeToFile(data: table, fileOrOptions: string|table)`:
    -   Encodes `data` to the file specified in `fileOrOptions`. the file will be created if it doesn't exist.
        -   When `fileOrOptions` is a string, it simply is the file path.
        -   When `fileOrOptions` is a table, it should have`file`, and optionally, `overwrite` as keys. `file` is the file path, and `overwrite` should be `true` when `file` should be overwritten with `data`, and `false` when `data` should be appended to `file`.
-   Added tests that cover:
    -   The property accessors of `toml.Date`, `toml.Time`, `toml.DateTime`, and `toml.TimeOffset`.
    -   `toml.toJSON` and `toml.toYAML`.

### Changed

-   `toml.tomlToJSON` and `toml.tomlToYAML` have been renamed to `toml.toJSON` and `toml.toYAML`.
    -   They have been renamed because they now have two functions: converting a TOML string to JSON/YAML (as before), or converting a table into JSON/YAML.
    -   The first parameter can be a string containing TOML (as before), or a table.

## [0.2.0](https://github.com/LebJe/toml.lua/releases/tag/0.2.0) - 2023-02-12

### Added

-   Updated to toml++ v3.3.0
-   Added `terseKeyValuePairs` to the list of formatting options for `toml.encode`
-   Tables can be made inline.
-   `toml.decode` can decode date, time and date-time into userdata (as before) or plain tables:

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
    	date = <userdata 1> -- 1979-05-27,
    	time = <userdata 2> -- 07:32:00,
    	datetime = <userdata 3> -- 1979-05-27T07:32:00-07:00
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
    		timeOffset = { minutes = -420 }
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

-   Test suite now runs on Windows

### Changed

-   Use [luaunit](https://github.com/bluebird75/luaunit) instead of [busted](https://github.com/lunarmodules/busted) for testing
-   `quoteDatesAndTimes` now defaults to `false`.

### Fixed

-   Boolean values are decoded as booleans instead of integers. (#6)

## [0.1.1](https://github.com/LebJe/toml.lua/releases/tag/0.1.1) - 2022-06-14

### Added

-   Windows support.

## [0.1.0](https://github.com/LebJe/toml.lua/releases/tag/0.1.0) - 2022-04-08

### Added

-   Upgrade to [toml++ 3.0.1](https://github.com/marzer/tomlplusplus/releases/tag/v3.0.1)
-   TOML documents can be converted to JSON or YAML.
-   Formatting options can be passed to the `encode`, `tomlToJSON`, and `tomlToYAML` functions.

### Removed

-   Removed `formattedReason` from decoding error messages.

## [0.0.4](https://github.com/LebJe/toml.lua/releases/tag/0.0.4) - 2021-11-24

### Added

-   toml.lua is now tested with `g++` and `clang++` with Lua 5.3, 5.2, 5.1 and LuaJIT.

### Changed

-   toml.lua now depends on Lua 5.1, instead of Lua 5.3.

## [0.0.3](https://github.com/LebJe/toml.lua/releases/tag/0.0.3) - 2021-11-08

### Added

-   Date, time, and date-time types.

### Fixed

-   Integers are now inserted into TOML documents as integers, not floating-point numbers.

## [0.0.2](https://github.com/LebJe/toml.lua/releases/tag/0.0.2) - 2021-10-29

### Added

-   The `decode` and `tomlToJSON` functions now return a table containing error details instead of a string if parsing a TOML document fails.

## [0.0.1](https://github.com/LebJe/toml.lua/releases/tag/0.0.1) - 2021-10-27

### Added

-   Initial Release.
