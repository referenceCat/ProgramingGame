import bpy
import os

scene = bpy.context.scene
image_dir = "/home/reference_cat/Projects/ProgramingGame/resources/assets/"
# image_dir = "/home/reference_cat/test"

def get_parent_collection(collection):
    for parent_collection in bpy.data.collections:
        if collection.name in parent_collection.children.keys():
            return parent_collection
    return None


def get_parent_collection_names(collection, parent_names):
    for parent_collection in bpy.data.collections:
        if collection.name in parent_collection.children.keys():
            parent_names.append(parent_collection.name)
            get_parent_collection_names(parent_collection, parent_names)
            return

def enable_render(collection):
    if collection == None:
        return
    print(collection.name)
    collection.hide_render=False
    enable_render(get_parent_collection(collection));
    
def disable_render_all():
    for col in bpy.data.collections: # hiding all collections
        col.hide_render=True
     
  
def turn_collection_hierarchy_into_path(obj):
    parent_collection = obj.users_collection[0]
    parent_names    = []
    parent_names.append(parent_collection.name)
    get_parent_collection_names(parent_collection, parent_names)
    parent_names.reverse()
    return '/'.join(parent_names)

cameras = []

for ob in scene.objects:
    if ob.type == 'CAMERA' and (len(cameras) == 0 or ob.name in cameras):
        bpy.context.scene.camera = ob # setting camera
        print('Set camera %s' % ob.name)
        for col in ob.users_collection[0].children:
            disable_render_all()
            enable_render(col)
            file = os.path.join(image_dir, turn_collection_hierarchy_into_path(ob), col.name[:-4])
            bpy.context.scene.render.filepath = file
            bpy.ops.render.render( write_still=True)