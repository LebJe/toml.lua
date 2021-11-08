--- Encodes, decodes, and converts TOML documents.
local record toml
	-- Decodes a TOML document into a Lua table. If `tomlStr` is invalid, an error message that describes the error in the document is returned.
	--- @param tomlStr string: The string containing a TOML document.
	--- @return table: The decoded TOML document.
	decode: function(tomlStr: string): { string: any }

	-- Encodes a Lua table into a TOML document.
	--- @param data table: The Lua table to be encoded.
	--- @return string: The encoded TOML document.
	encode: function(data: { string: any }): string
	
	-- Converts a TOML document into a JSON document.
	--- @param tomlStr string: The string containing a TOML document.
	--- @return string: The converted JSON document.
	tomlToJSON: function(tomlStr: string): string
	
	record Date
		userdata
		year: number
		month: number
		day: number
		
		new: function(year: number, month: number, day: number): Date
	end
	
	record Time
		userdata
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
		userdata
		minutes: number
		
		new: function (hours: number, minutes: number): TimeOffset
	end
	
	record DateTime
		userdata
		date: Date
		time: Time
		TimeOffset: nil | TimeOffset
		
		new: function(date: Date, time: Time): DateTime
		new: function(date: Date, time: Time, timeOffset: TimeOffset): DateTime
	end
end

return toml
