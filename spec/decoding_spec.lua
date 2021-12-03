describe("toml.decode()", function()
	local toml = require("toml")
	local data = require("spec/tables")

	it("can decode sample TOML document", function()
		local fh = io.open("spec/test-data/test.toml")
		local tomlStr = fh:read("*all")
		fh:close()

		local table = toml.decode(tomlStr)

		assert.are.same(data.tableForTestToml, table)
	end)

	it("can decode another sample TOML document", function()
		local fh = io.open("spec/test-data/test2.toml")
		local tomlStr = fh:read("*all")
		fh:close()

		local table = toml.decode(tomlStr)

		assert.are.same(data.tableForTest2Toml, table)
	end)

	it("can decode massive table", function()
		local fh = io.open("spec/test-data/massive.toml")
		local massiveTOML = fh:read("*all")
		fh:close()

		local table = toml.decode(massiveTOML)
		assert.are.same(data.tableForMassiveToml, table)
	end)

	it("fails on invalid input", function()
		local fh1 = io.open("spec/test-data/invalidTable.toml")
		local tomlStr1 = fh1:read("*all")
		fh1:close()

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

		assert.has_error(function()
			toml.decode(tomlStr1)
		end, expectedError1)

		local fh2 = io.open("spec/test-data/multipleDotKeyInvalid.toml")
		local tomlStr2 = fh2:read("*all")
		fh2:close()

		local expectedError2 = {
			begin = {
				line = 10,
				column = 7,
			},
			["end"] = {
				line = 10,
				column = 7,
			},
			reason = "Error while parsing key-value pair: cannot redefine existing table as dotted key-value pair",
		}

		assert.has_error(function()
			toml.decode(tomlStr2)
		end, expectedError2)
	end)
end)
