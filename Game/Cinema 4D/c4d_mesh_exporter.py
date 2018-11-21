import os
import re
import c4d

#Parameters
scale = 1.0 / 100.0
script_directory = os.path.dirname(os.path.realpath(__file__))
output_directory = os.path.join(script_directory, "../Sources/")
output_filename = "Mesh_{}"
header = "Mesh@WiredPickup"

def GetOutputFilename(name):
    return os.path.join(output_directory,output_filename.format(name))

def ToCFloat(x):
    return str(x)

def ToCInt(x):
    return str(x)

def ToCSV(list, stringify):
    csv = ""
    for i,item in enumerate(list):
        if i > 0:
            csv += ","
        csv += stringify(item)
    return csv

def Encode(verts,inds):
    cnt = ""
    cnt += header + "\n"
    cnt += ToCInt(len(verts)) + "\n"
    cnt += ToCSV(verts, ToCFloat) + "\n"
    cnt += ToCInt(len(inds)) + "\n"
    cnt += ToCSV(inds, ToCInt) + "\n"
    return cnt

def WriteFile(filename,content):
    with open(filename, "w") as file:
        file.write(content)

def ExportPolygonObject(obj):
    if obj.GetType() != c4d.Opolygon:
        raise RuntimeError("Object is not a polygon object")
    filename = GetOutputFilename(obj.GetName())
    inds = []
    for poly in obj.GetAllPolygons():
        if not poly.IsTriangle():
            raise RuntimeError("Polygon object is not triangulated")
        inds += [poly.a, poly.b, poly.c]
    WriteFile(filename, Encode(obj.GetAllPoints(), inds))

def main():
    doc = c4d.documents.GetActiveDocument()
    objs = doc.GetActiveObjects(c4d.GETACTIVEOBJECTFLAGS_0)
    print("---- Cinema 4D Mesh Exporter for WiredPickup project ----")
    names = []
    for obj in objs:
        try:
            if any(obj.GetName() in name for name in names):
                raise RuntimeError("Object name already exists")
            ExportPolygonObject(obj)
            names += [obj.GetName()]
            print("Object '{}' exported successfully".format(obj.GetName()))
        except Exception as ex:
            print("Exception occurred while exporting object '{}'".format(obj.GetName()))
            print(ex)
    print("Done.")

if __name__ == "__main__":
    main()