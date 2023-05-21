from inc_noesis import *

def registerNoesisTypes():
    handle = noesis.register("Texture", ".tex")
    noesis.setHandlerWriteRGBA(handle, writeTexture)

    handle = noesis.register("Mesh", ".mesh")
    noesis.setHandlerWriteModel(handle, writeMesh)

    return 1

def writeTexture(data, width, height, bs):
    bs.writeUShort(width)
    bs.writeUShort(height)

    data = rapi.imageEncodeRaw(data, width, height, "b8g8r8a8")

    bs.writeBytes(data)

    return 1

def writeMesh(mdl, bs):
    mesh = mdl.meshes[0]

    bs.writeUShort(len(mesh.positions))
    bs.writeUShort(len(mesh.indices))

    for position in mesh.positions:
        bs.writeUShort(int(position[0]))
        bs.writeUShort(int(position[1]))
        bs.writeUShort(int(position[2]))

    for uv in mesh.uvs:
        bs.writeFloat(uv[0])
        bs.writeFloat(uv[1])

    for indice in mesh.indices:
        bs.writeUShort(indice)

    return 1