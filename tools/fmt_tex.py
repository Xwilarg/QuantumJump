from inc_noesis import *

def registerNoesisTypes():
    handle = noesis.register("Texture", ".tex")
    noesis.setHandlerWriteRGBA(handle, writeTexture)

    return 1

def writeTexture(data, width, height, bs):
    bs.writeUShort(width)
    bs.writeUShort(height)

    data = rapi.imageEncodeRaw(data, width, height, "r8g8b8")

    bs.writeBytes(data)

    return 1