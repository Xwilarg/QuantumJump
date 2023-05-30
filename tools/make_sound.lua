if #arg < 1 then
    print("Usage: lua make_sound.lua [sound file]")
    return
end

local input = arg[1]
local output = input:sub(1, input:find(".", 1, true) - 1)

--os.execute(string.format("ffmpeg -i %s -ac 1 -f wav -acodec pcm_s16le temp.wav", input))
os.execute(string.format("adpcmencode3 %s temp.wav", input))

local wav = io.open("temp.wav", "rb")

-- not how you should read a RIFF file
wav:seek("set", 24)
local sampleRate, _, blockAlign = string.unpack("<IIH", wav:read(10))

-- look for the data chunk
while true do
    local chunk = wav:read(4)
    if chunk == "data" then break end

    wav:seek("cur", -3)
end

-- read the data chunk
local size = string.unpack("<I", wav:read(4))

-- open output file
local file = io.open(output .. ".sound", "w+b")

-- write the sound header
file:write(string.pack("<IHH", size, sampleRate, blockAlign))

-- write the sound data
file:write(wav:read(size))

file:close()
wav:close()