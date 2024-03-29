--- Encodes, decodes, and converts TOML documents.
local record toml
	--- Decodes a TOML document into a Lua table. If `tomlStr` is invalid, an error table that describes the error in the document is returned.
	--- @param tomlStr string: The string containing a TOML document.
	--- @param options DecodingOptions|nil: See docs for DecodingOptions.
	--- @return table: The decoded TOML document.
	decode: function(tomlStr: string, options: DecodingOptions|nil): { string: any }
	
	--- Decodes a TOML document into a Lua table. If `tomlStr` is invalid, an error table that describes the error in the document is returned.
	--- @param filePath string: The path to the TOML document.
	--- @param options DecodingOptions|nil: See docs for DecodingOptions.
	--- @return table: The decoded TOML document.
	decodeFromFile: function(filePath: string, options: DecodingOptions|nil): { string: any }

	--- Encodes a Lua table into a TOML document.
	--- @param data table: The Lua table to be encoded.
	--- @param options FormattingOptions|nil: Options for formatting the YAML document. Pass `{}` to remove all options, or no table to use the default options.
	--- @return string: The encoded TOML document.
	encode: function(data: { string: any }, options: FormattingOptions|nil): string
	
	--- Encodes a Lua table into a TOML document, and **appends** that document to a file.
	--- @param data table: The Lua table to be encoded.
	--- @param file string: The file to write the document to/
	--- @param options FormattingOptions|nil: Options for formatting the YAML document. Pass `{}` to remove all options, or no table to use the default options.
	encodeToFile: function(data: { string: any }, fileOrOptions: string|EncodeToFileOptions, options: FormattingOptions|nil)
	
	--- Converts a table, or a TOML document into a JSON document.
	--- @param toml string|{ string: any }: A string containing a TOML document, or a table.
	--- @param options FormattingOptions|nil: Options for formatting the YAML document. Pass `{}` to remove all options, or no table to use the default options.
	--- @return string: The converted JSON document.
	toJSON: function(toml: string|{ string: any }, options: FormattingOptions|nil): string
	
	--- Converts a table, or a TOML document into a YAML document.
	--- @param toml string|{ string: any }: A string containing a TOML document, or a table.
	--- @param options FormattingOptions|nil: Options for formatting the YAML document. Pass `{}` to remove all options, or no table to use the default options.
	--- @return string: The converted YAML document.
	toYAML: function(toml: string|{ string: any }, options: FormattingOptions|nil): string

	record Date
		userdata
		year: number
		month: number
		day: number
		
		new: function(year: number, month: number, day: number): Date

		metamethod __tostring: function(Date): string
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

		metamethod __tostring: function(Time): string
	end
	
	record TimeOffset
		userdata
		minutes: number
		
		new: function (hours: number, minutes: number): TimeOffset

		metamethod __tostring: function(TimeOffset): string
	end
	
	record DateTime
		userdata
		date: Date
		time: Time
		TimeOffset: nil | TimeOffset
		
		new: function(date: Date, time: Time): DateTime
		new: function(date: Date, time: Time, timeOffset: TimeOffset): DateTime

		metamethod __tostring: function(DateTime): string
	end
	
	record Int
		userdata
		int: number
		
		new: function(int: number, flags: formatting.int): Int

		metamethod __tostring: function(Int): string
	end
	
	record formatting
		record int
			binary: number
			octal: number
			hexadecimal: number
		end
	end

	record DecodingOptions
		formattedIntsAsUserData: boolean
		temporalTypesAsUserData: boolean
	end
	
	record EncodeToFileOptions
		file: string
		overwrite: boolean
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
		
		--- Emit floating-point values with relaxed (human-friendly) precision.
		--- Warning: Setting this flag may cause serialized documents to no longer round-
		--- trip correctly since floats might have a less precise value upon being written out
		--- than they did when being read in. Use this flag at your own risk.
		relaxedFloatPrecision: boolean
	
		--- Avoids the use of whitespace around key-value pairs.
		terseKeyValuePairs: boolean
	end
end

return toml
