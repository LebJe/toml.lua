lu = require("luaunit")
local toml = require("toml")
local data = require("tests/tables")

TestEncoder = {}
TestDecoder = {}
TestFormatters = {}
TestUserdataAccessors = {}

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
	lu.assertEquals(data.tableForTestConfigToml, toml.decodeFromFile("tests/test-data/testConfig.toml"))
	lu.assertEquals(data.tableForTestConfig2Toml, toml.decodeFromFile("tests/test-data/testConfig2.toml"))
end

function TestDecoder:testDecodeMassiveTable()
	lu.assertEquals(data.tableForMassiveToml, toml.decodeFromFile("tests/test-data/massive.toml"))
end

function TestDecoder:testInvalidInputs()
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

	local succeeded1, table1 = pcall(toml.decodeFromFile, "tests/test-data/invalidTable.toml")
	local succeeded2, table2 = pcall(toml.decodeFromFile, "tests/test-data/multipleDotKeyInvalid.toml")

	lu.assertFalse(succeeded1)
	lu.assertFalse(succeeded2)
	lu.assertEquals(table1, expectedError1)
	lu.assertEquals(table2, expectedError2)
end

function TestDecoder:testFormattingOptions()
	-- Temporal types
	local temporalTypesAsTables = toml.decode(
		toml.encode(data.tableForFormattingTest.temporalTypes.asUserdata),
		{ temporalTypesAsUserData = false }
	)
	lu.assertEquals(temporalTypesAsTables, data.tableForFormattingTest.temporalTypes.asTables)

	local temporalTypesAsUserData = toml.decode(
		toml.encode(data.tableForFormattingTest.temporalTypes.asUserdata),
		{ temporalTypesAsUserData = true }
	)
	lu.assertEquals(temporalTypesAsUserData, data.tableForFormattingTest.temporalTypes.asUserdata)

	-- Formatted Integers

	local unformattedIntegers = toml.decode(
		toml.encode(data.tableForFormattingTest.integers.asUserdata),
		{ allowBinaryIntegers = false, allowHexadecimalIntegers = false, allowOctalIntegers = false }
	)
	lu.assertEquals(unformattedIntegers, data.tableForFormattingTest.integers.asTables)

	local formattedIntegers =
		toml.decode(toml.encode(data.tableForFormattingTest.integers.asUserdata), { formattedIntsAsUserData = true })
	lu.assertEquals(formattedIntegers, data.tableForFormattingTest.integers.asUserdata)
end

local function testFormatters(fileType, formatter)
	local path = "tests/test-data/testConfig"
	local filePath = path .. "." .. fileType
	local tomlPath = path .. ".toml"

	local file = read(filePath)
	local tomlTable = toml.decodeFromFile(tomlPath)

	lu.assertEquals(formatter(tomlTable), file)
	lu.assertEquals(formatter(read(tomlPath)), file)
end

function TestFormatters:testJSONFormatter()
	testFormatters("json", toml.toJSON)
end

function TestFormatters:testYAMLFormatter()
	testFormatters("yml", toml.toYAML)
end

function TestUserdataAccessors:testTemporalTypesAccessors()
	local dateTime = data.tableForFormattingTest.temporalTypes.asUserdata.dob
	local dateTimeTable = data.tableForFormattingTest.temporalTypes.asTables.dob

	-- time offset
	lu.assertEquals(dateTime.timeOffset.minutes, dateTimeTable.timeOffset.minutes)

	-- time
	lu.assertEquals(dateTime.time.hour, dateTimeTable.time.hour)
	lu.assertEquals(dateTime.time.minute, dateTimeTable.time.minute)
	lu.assertEquals(dateTime.time.second, dateTimeTable.time.second)
	lu.assertEquals(dateTime.time.nanoSecond, dateTimeTable.time.nanoSecond)

	-- date
	lu.assertEquals(dateTime.date.day, dateTimeTable.date.day)
	lu.assertEquals(dateTime.date.month, dateTimeTable.date.month)
	lu.assertEquals(dateTime.date.year, dateTimeTable.date.year)
end

function TestUserdataAccessors:testIntAccessor()
	-- Formatted
	local fI = data.tableForFormattingTest.integers.asUserdata.formattedIntegers

	-- Unformatted
	local uFI = data.tableForFormattingTest.integers.asTables.formattedIntegers

	lu.assertEquals(fI.int1.int, uFI.int1)
	lu.assertEquals(fI.int1.flags, toml.formatting.int.octal)

	lu.assertEquals(fI.int2.int, uFI.int2)
	lu.assertEquals(fI.int2.flags, toml.formatting.int.binary)

	lu.assertEquals(fI.int3.int, uFI.int3)
	lu.assertEquals(fI.int3.flags, toml.formatting.int.hexadecimal)

	-- fI.table, uFI.table
	lu.assertEquals(fI.table.int1.int, uFI.table.int1)
	lu.assertEquals(fI.table.int1.flags, toml.formatting.int.octal)

	lu.assertEquals(fI.table.int2.int, uFI.table.int2)
	lu.assertEquals(fI.table.int2.flags, toml.formatting.int.binary)

	lu.assertEquals(fI.table.int3.int, uFI.table.int3)
	lu.assertEquals(fI.table.int3.flags, toml.formatting.int.hexadecimal)

	-- fI.array, uFI.array
	lu.assertEquals(fI.array[1].int, uFI.array[1])
	lu.assertEquals(fI.array[1].flags, toml.formatting.int.octal)

	lu.assertEquals(fI.array[2].int, uFI.array[2])
	lu.assertEquals(fI.array[2].flags, toml.formatting.int.binary)

	lu.assertEquals(fI.array[3].int, uFI.array[3])
	lu.assertEquals(fI.array[3].flags, toml.formatting.int.hexadecimal)
end

os.exit(lu.LuaUnit.run())
