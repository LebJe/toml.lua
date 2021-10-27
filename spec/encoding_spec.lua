
describe("toml.encode()", function()
	local toml = require("toml")
	local data = require("spec/tables")

	it("can encode sample TOML document", function()
		local fh = io.open("spec/test.toml")
		local expectedTOML = fh:read("*all")
		fh:close()

		assert.are.same(toml.encode(data.tableForTestToml), expectedTOML)
	end)

	it("can encode another sample TOML document", function()
		local fh = io.open("spec/test2.toml")
		local expectedTOML = fh:read("*all")
		fh:close()

		assert.are.same(toml.encode(data.tableForTest2Toml), expectedTOML)
	end)

	it("can encode massive table", function()
		local returnedTOML = toml.encode(data.tableForMassiveToml)

		local fh = io.open("spec/massive.toml")
		local expectedTOML = fh:read("*all")
		fh:close()

		assert.equal(returnedTOML, expectedTOML)
	end)
end)
