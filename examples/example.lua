-- Run `luarocks install inspect` first!

local toml = require("toml")
local inspect = require("inspect")

local tomlStr = [[
# This is a TOML document

title = "TOML Example"

[owner]
name = "Tom Preston-Werner"
dob = 1979-05-27T07:32:00-08:00

[database]
enabled = true
ports = [ 8000, 8001, 8002 ]
data = [ ["delta", "phi"], [3.14] ]
temp_targets = { cpu = 79.5, case = 72.0 }

[servers]

[servers.alpha]
ip = "10.0.0.1"
role = "frontend"

[servers.beta]
ip = "10.0.0.2"
role = "backend"
]]

print("Decoding: \n")

local succeeded, data = pcall(toml.decode, tomlStr)

if succeeded then
	print(inspect(data))
else
	print("An error occurred:\n")
	print(inspect(data))
end

print("\n\nEncoding:\n")

print(toml.encode(data, { indentation = true }))

print("\n\nTOML to JSON:\n")
print(toml.toJSON(tomlStr, { indentation = true }))

print("\n\nTOML to YAML:\n")
print(toml.toYAML(tomlStr, { indentation = true }))
