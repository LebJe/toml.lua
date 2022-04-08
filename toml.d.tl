--- Encodes, decodes, and converts TOML documents.
local record toml
	--- Decodes a TOML document into a Lua table. If `tomlStr` is invalid, an error message that describes the error in the document is returned.
	--- @param tomlStr string: The string containing a TOML document.
	--- @return table: The decoded TOML document.
	decode: function(tomlStr: string): { string: any }

	--- Encodes a Lua table into a TOML document.
	--- @param data table: The Lua table to be encoded.
	--- @param options FormattingOptions: Options for formatting the YAML document. Pass `{}` to remove all options, or no table to use the default options.
	--- @return string: The encoded TOML document.
	encode: function(data: { string: any }, options: FormattingOptions): string
	
	--- Converts a TOML document into a JSON document.
	--- @param tomlStr string: The string containing a TOML document.
	--- @param options FormattingOptions: Options for formatting the YAML document. Pass `{}` to remove all options, or no table to use the default options.
	--- @return string: The converted JSON document.
	tomlToJSON: function(tomlStr: string, options: FormattingOptions): string
	
	--- Converts a TOML document into a YAML document.
	--- @param tomlStr string: The string containing a TOML document.
	--- @param options FormattingOptions: Options for formatting the YAML document. Pass `{}` to remove all options, or no table to use the default options.
	--- @return string: The converted YAML document.
	tomlToYAML: function(tomlStr: string, options: FormattingOptions): string
	
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
	
	record FormattingOptions
		--- Dates and times will be emitted as quoted strings.
		quoteDatesAndTimes: boolean

		--- Infinities and NaNs will be emitted as quoted strings.
		quoteInfinitesAndNaNs: boolean

		--- Strings will be emitted as single-quoted literal strings where possible.
		allowLiteralStrings: boolean

		--- Strings containing newlines will be emitted as triple-quoted 'multi-line' strings where possible.
		allowMultiLineStrings: boolean

		--- Allow real tab characters in string literals (as opposed to the escaped form `\t`).
		allowRealTabsInStrings: boolean

		--- Allow integers with formatAsBinary to be emitted as binary.
		allowBinaryIntegers: boolean

		--- Allow integers with formatAsOctal to be emitted as octal.
		allowOctalIntegers: boolean

		--- Allow integers with formatAsHexadecimal to be emitted as hexadecimal.
		allowHexadecimalIntegers: boolean

		--- Apply indentation to tables nested within other tables/arrays.
		indentSubTables: boolean

		--- Apply indentation to array elements when the array is forced to wrap over multiple lines.
		indentArrayElements: boolean

		--- Combination of `indentSubTables` and `indentArrayElements`.
		indentation: boolean
	end
end

return toml
