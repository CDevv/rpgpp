local function ternary(cond, if_true, if_false)
	if cond then
		return if_true
	else
		return if_false
	end
end

local function tableLength(t)
	local count = 0
	for _ in pairs(t) do
		count = count + 1
	end
	return count
end

local function _FlattenJSON(data, flattened, prefix)
	for k, v in pairs(data) do
		local key = ternary(prefix == nil, k, (prefix or "") .. "." .. k)

		if type(v) == "table" then
			_FlattenJSON(v, flattened, key)
		elseif type(v) == "string" then
			flattened[key] = v;
		else
			flattened[key] = tostring(v)
		end
	end
end

local function _LoadAndFlatten(filepath)
	import("core.base.json")
	local raw = io.readfile(filepath)
	local data = json.decode(raw)

	local flattened = {}
	_FlattenJSON(data, flattened, nil)
	return flattened
end

local function _CompareTranslation(base, target, filename)
	local missing = {}
	local extra = {}
	local untranslated = {}

	for k, v in pairs(base) do
		if target[k] == nil then
			table.insert(missing, k)
		elseif target[k] == v then
			table.insert(untranslated, k)
		end
	end

	for k, _ in pairs(target) do
		if base[k] == nil then
			table.insert(extra, k)
		end
	end

	local translatedCnt = tableLength(base) - tableLength(missing) - tableLength(untranslated)
	local percent = translatedCnt / tableLength(base) * 100

	print("\n" .. "===" .. filename .. "===")
	print("Completion: " .. percent .. "%")

	table.sort(missing)
	table.sort(extra)
	table.sort(untranslated)

	if #missing ~= 0 then
		print("\n" .. "Missing keys:")
		for _, k in ipairs(missing) do
			print(" - " .. k)
		end
	end

	if #untranslated ~= 0 then
		print("\n" .. "Untranslated keys (same as base):")
		for _, k in ipairs(untranslated) do
			print(" - " .. k)
		end
	end

	if #extra ~= 0 then
		print("\n" .. "Extra keys:")
		for _, k in ipairs(extra) do
			print(" - " .. k)
		end
	end

	if #missing == 0 and #untranslated == 0 and #extra == 0 then
		print("\n" .. "Translation is up to date!")
	end
end

function Main()
	local baseDir = "resources/translations/"

	local translatedFiles = os.files(baseDir .. "*.json")
	local baseFile = baseDir .. "en_us.json"

	local base = _LoadAndFlatten(baseFile)
	for _, filepath in ipairs(translatedFiles) do
		if filepath == baseFile then
			goto continue
		end

		local target = _LoadAndFlatten(filepath)
		_CompareTranslation(base, target, path.filename(filepath))
		::continue::
	end
end
