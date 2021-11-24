# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.0.4](https://github.com/LebJe/toml.lua/releases/tag/0.0.3) - 2021-11-24

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
