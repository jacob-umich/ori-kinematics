import bpy
import json
import shutil
import numpy as np
from pathlib import Path





def create_mesh(vertices, edges, faces, mesh_name="_name", object_name='_objName'):
    mesh_data = bpy.data.meshes.new(mesh_name)
    mesh_data.from_pydata(vertices, edges, faces)
    obj = bpy.data.objects.new(object_name, mesh_data)
    bpy.context.collection.objects.link(obj)
    return mesh_data, obj
# Example usage:
# vertices, edges, faces = ...  # Define geometry data
# mesh_data, obj = create_mesh(vertices, edges, faces)

def update_geometry(motion, mesh_data,dofs):
    # Implement logic to update the geometry based on the kinematic simulation results
    # Example: motion_index, vertex_positions = motion.get_updated_positions()
    for i, vertex in enumerate(mesh_data.vertices):
        vertex.co = [
            motion[int(dofs[i,0])],
            motion[int(dofs[i,1])],
            motion[int(dofs[i,2])]
            ]
# Example usage:
# motion = ...  # Get the motion data from kinematic simulation
# update_geometry(motion, mesh_data)

def insert_keyframe(fcurves, frame, values):
    for fcu, val in zip(fcurves, values):
        fcu.keyframe_points.insert(frame, val, options={'FAST'})

def add_keyframes(mesh_data, motion_index, fcurve_list):
    for i, vertex in enumerate(mesh_data.vertices):
        insert_keyframe(fcurve_list[i], motion_index, vertex.co)
# Example usage:
# motion_index = ...  # Get the current time step from kinematic simulation
# fcurve_list = ...   # List of fcurves for each vertex
# add_keyframes(mesh_data, motion_index, fcurve_list)

def create_animation(mesh_data, motions,dofs):
    action = bpy.data.actions.new("MeshAnimation")
    mesh_data.animation_data_create()
    mesh_data.animation_data.action = action
    fcurve_list = []
    for v in mesh_data.vertices:
        fcurves = [action.fcurves.new(f"vertices[{v.index}].co", index =  i) for i in range(3)]
        fcurve_list.append(fcurves)

    for motion_index, motion in enumerate(motions):
        update_geometry(motion, mesh_data,dofs)
        add_keyframes(mesh_data, motion_index, fcurve_list)
# Example usage:
# motions = ...   # List of motions from kinematic simulation
# create_animation(mesh_data, motions, fcurve_list)


if __name__ == "__main__":
    blender_bin = shutil.which("blender")
    if blender_bin:
        print("Found:", blender_bin)
        bpy.app.binary_path = blender_bin
    else:
        print("Unable to find blender!")    
    with open('cube_out.json',"r") as f:
        data = json.load(f)

    ndof = data['n_dof']
    nodes = np.zeros((int(ndof/3),3))
    dof = np.zeros((int(ndof/3),3))
    faces = []
    id = []
    for body in data['bodies']:
        for node in body['nodes']:
            nodes[int(node['idg'])]=node['pos']
            dof[int(node['idg'])]=[int(node['dof'][i])for i in range(3)]
        for face in body['faces']:
            newface = []
            for node in face:
                iter=0
                cond = True
                while cond:
                    if(node==body['nodes'][iter]['id']):
                        newface.append(int(body['nodes'][iter]['idg']))
                        cond=False
                    iter+=1
            faces.append(newface)
    max_step = 0
    for vel in data['target_velocities']:
        if max_step<=vel["end_step"]:
            max_step=int(vel["end_step"])
        

    history = data['history']
    name = data['name']
    bpy.data.objects['Cube'].select_set(True)
    bpy.ops.object.delete()
    mesh_data,obj = create_mesh(nodes.tolist(),[],faces,name)
    outpath = Path(__file__)

    create_animation(mesh_data,history,dof)
    bpy.context.scene.frame_end=200
    bpy.context.scene.render.filepath = str(Path.cwd().joinpath("build/example/out.mp4").resolve())
    bpy.context.scene.render.image_settings.file_format = "FFMPEG"
    bpy.ops.render.render(animation = True)

