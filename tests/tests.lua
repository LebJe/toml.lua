lu = require("luaunit")
local toml = require("toml")
local data = require("tests/tables")

TestEncoder = {}
TestDecoder = {}

---Reads the contents of `file` at `path`
---@param path string The path to the file
---@return string
local function read(path)
	local fh = io.open(path)
	local contents = fh:read("*all")
	fh:close()
	return contents
end

function TestEncoder:testEncodeSamples()
	local testConfig = read("tests/test-data/testConfig.toml")
	local testConfig2 = read("tests/test-data/testConfig2.toml")
	lu.assertEquals(toml.encode(data.tableForTestConfigToml), testConfig)
	lu.assertEquals(toml.encode(data.tableForTestConfig2Toml), testConfig2)
end

function TestEncoder:testEncodeMassiveTable()
	local massive = read("tests/test-data/massive.toml")
	lu.assertEquals(toml.encode(data.tableForMassiveToml), massive)
end

function TestDecoder:testDecodeSamples()
	local testConfig = read("tests/test-data/testConfig.toml")
	local testConfig2 = read("tests/test-data/testConfig2.toml")

	lu.assertEquals(data.tableForTestConfigToml, toml.decode(testConfig))
	lu.assertEquals(data.tableForTestConfig2Toml, toml.decode(testConfig2))
end

function TestDecoder:testDecodeMassiveTable()
	local massive = read("tests/test-data/massive.toml")
	lu.assertEquals(data.tableForMassiveToml, toml.decode(massive))
end

function TestDecoder:testInvalidInputs()
	local invalid1 = read("tests/test-data/invalidTable.toml")
	local invalid2 = read("tests/test-data/multipleDotKeyInvalid.toml")
	local expectedError1 = {
		begin = {
			line = 7,
			column = 1,
		},
		["end"] = {
			line = 7,
			column = 1,
		},
		reason = "Error while parsing table header: cannot redefine existing table 'fruit' as array-of-tables",
	}
	local expectedError2 = {
		begin = {
			line = 10,
			column = 7,
		},
		["end"] = {
			line = 10,
			column = 7,
		},
		reason = "Error while parsing key-value pair: cannot redefine existing integer as dotted key-value pair",
	}

	local succeeded1, table1 = pcall(toml.decode, invalid1)
	local succeeded2, table2 = pcall(toml.decode, invalid2)

	lu.assertFalse(succeeded1)
	lu.assertFalse(succeeded2)
	lu.assertEquals(table1, expectedError1)
	lu.assertEquals(table2, expectedError2)
end

function TestDecoder:testFormatting()
	local asTables =
		toml.decode(toml.encode(data.tableForFormattingTest.asUserdata), { temporalTypesAsUserData = false })
	lu.assertEquals(asTables, data.tableForFormattingTest.asTables)
end

os.exit(lu.LuaUnit.run())
