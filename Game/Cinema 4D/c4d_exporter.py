import os
import traceback
import c4d

#Parameters
scale = 1.0 / 100.0
script_directory = os.path.dirname(os.path.realpath(__file__))
header = "Mesh@WiredPickup"
cutter_ind = -1
debug_log_enabled = True

def GetOutputFilePath(name):
    return os.path.join(output_directory, name)

def ToCSV(list, stringify):
    csv = ""
    for i,item in enumerate(list):
        if i > 0:
            csv += ","
        csv += stringify(item)
    return csv

def WriteFile(filename, content):
    with open(filename, "w") as file:
        file.write(content)

class Mesh:

    def __init__(self, verts, triangle_inds, line_inds):
        self.verts = verts
        self.triangle_inds = triangle_inds
        self.line_inds = line_inds

def GatherPolygonIndices(poly_obj):
    inds = []
    for poly in poly_obj.GetAllPolygons():
        if not poly.IsTriangle():
            raise ValueError("Polygon object is not triangulated")
        inds += [poly.a, poly.b, poly.c]
    return inds

def GatherSplineIndices(spline_obj, cutter_ind, ind_map):
    segs = [spline_obj.GetSegment(i) for i in range(spline_obj.GetSegmentCount())]
    inds = []
    last_ind = 0
    for seg_ind, seg in enumerate(segs):
        if seg_ind > 0:
            inds += [cutter_ind]
        inds += [ind_map[i + last_ind] for i in range(seg["cnt"])]
        if seg["closed"]:
            inds += [ind_map[last_ind]]
        last_ind += seg["cnt"]
    return inds

def MapPoints(src_points, new_points):
    ind_map = []
    for new_point in new_points:
        found = False
        for i, src_point in enumerate(src_points):
            if src_point == new_point:
                ind_map += [i]
                found = True
                break
        if not found:
            raise ValueError("Cannot merge point '{}'".format(str(spline_point)))
    return ind_map

def BuildMesh(poly_obj, spline_obj):
    if poly_obj.GetType() != c4d.Opolygon:
        raise ValueError("Not a polygon object")
    if spline_obj.GetType() != c4d.Ospline:
        raise ValueError("Not a spline object")
    points = poly_obj.GetAllPoints()
    triangle_inds = GatherPolygonIndices(poly_obj)
    line_inds = GatherSplineIndices(spline_obj, cutter_ind, MapPoints(points, spline_obj.GetAllPoints()))
    return Mesh(points, triangle_inds, line_inds)

def EncodeMesh(mesh):
    txt = ""
    txt += str(len(mesh.verts)) + "\n"
    txt += ToCSV(mesh.verts, lambda v : str(v.x * scale) + "," + str(v.y * scale) + "," + str(v.z * scale)) + "\n"
    txt += str(len(mesh.triangle_inds)) + "\n"
    txt += ToCSV(mesh.triangle_inds, str) + "\n"
    txt += str(len(mesh.line_inds)) + "\n"
    txt += ToCSV(mesh.line_inds, str) + "\n"
    return txt

def Log(msg):
    print(msg)

def Export():
    doc = c4d.documents.GetActiveDocument()
    objs = doc.GetActiveObjects(c4d.GETACTIVEOBJECTFLAGS_0)

    if len(objs) == 2 and objs[0].GetType() == c4d.Opolygon and objs[1].GetType() == c4d.Ospline:
        poly_obj = objs[0]
        spline_obj = objs[1]
    elif len(objs) == 2 and objs[0].GetType() == c4d.Ospline and objs[1].GetType() == c4d.Opolygon:
        poly_obj = objs[1]
        spline_obj = objs[0]
    else:
        raise ValueError("This script requires a Polygon object and a Spline object to be active")

    if debug_log_enabled:
        Log("Exporting Polygon object '{}' and Spline object '{}'".format(poly_obj.GetName(), spline_obj.GetName()))

    mesh = BuildMesh(poly_obj, spline_obj)

    content = header + "\n" + EncodeMesh(mesh)
    filepath = c4d.storage.SaveDialog(title="Export mesh", def_path=script_directory)
    if filepath is None:
        raise ValueError("No path choosen")
    
    WriteFile(filepath, content)

    Log("Successfully exported to file '{}'".format(filepath))

    if debug_log_enabled:
        Log("Raw P. point count {}".format(str(poly_obj.GetPointCount())))
        Log("Raw P. triangle count {}".format(str(poly_obj.GetPolygonCount())))
        Log("Raw S. point count {}".format(str(spline_obj.GetPointCount())))
        Log("Raw S. segments count {}".format(str(spline_obj.GetSegmentCount())))
        spline_raw_line_list_ind_count = 0
        for seg in [spline_obj.GetSegment(i) for i in range(spline_obj.GetSegmentCount())]:
            spline_raw_line_list_ind_count += (seg["cnt"] - 1) * 2 + (1 if seg["closed"] else 0)
        Log("Raw S. line list index count {}".format(str(spline_raw_line_list_ind_count)))
        Log("Exported vertex count {}".format(str(len(mesh.verts))))
        Log("Exported triangle count {}".format(str(len(mesh.triangle_inds) / 3)))
        Log("Exported line strip index count {}".format(str(len(mesh.line_inds))))
        unused_verts = 0
        for vi in range(len(mesh.verts)):
            if not (any(vi == i for i in mesh.triangle_inds) or any(vi == i for i in mesh.line_inds)):
                unused_verts += 1
        Log("Unused vertex count {}".format(str(unused_verts)))


def main():
    print("-- Cinema 4D Mesh Exporter for WiredPickup project --")
    try:
        Export()
    except Exception:
        print("-- Exception occurred --")
        traceback.print_exc()
    print("-- Bye --")

if __name__ == "__main__":
    main()