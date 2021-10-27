
describe("toml.decode()", function()
	local toml = require("toml")
	local data = require("spec/tables")
	
	it("can decode sample TOML document", function()
		local fh = io.open("spec/test.toml")
		local tomlStr = fh:read("*all")
		fh:close()

		local table = toml.decode(tomlStr)

		assert.are.same(data.tableForTestToml, table)
	end)

	it("can decode another sample TOML document", function()
		local fh = io.open("spec/test2.toml")
		local tomlStr = fh:read("*all")
		fh:close()

		local table = toml.decode(tomlStr)

		assert.are.same(data.tableForTest2Toml, table)
	end)

	it("can decode massive table", function()
		local fh = io.open("spec/massive.toml")
		local massiveTOML = fh:read("*all")
		fh:close()

		local table = toml.decode(massiveTOML)
		assert.are.same(data.tableForMassiveToml, table)
	end)
end)
