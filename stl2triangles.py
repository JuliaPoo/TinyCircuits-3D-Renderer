import numpy as np
from stl import mesh
import warnings
import os
import shutil

# Render variables
SPRITE_STL_FILE = r"Low_Poly_Dog.stl" # Model made by AndrewSink https://www.thingiverse.com/thing:2797399
SCALE = "1.2"
BACKGROUND_COL = r"0xC5" # 8-bit color
PHONG_AMBIANCE_COL = r"{1, .6, .6}"
PHONG_AMBIANCE_INTENSITY = ".4"
PHONG_DIFFUSE_COL = r"{1, .6, .6}"
PHONG_DIFFUSE_INTENSITY = ".7"
PHONG_SPECTRAL_COL = r"{1, 1, 1}"
PHONG_SPECTRAL_INTENSITY = ".7"
SHINYNESS = "10"

def process(triangles):
    '''Scale triangles'''
    
    T = triangles

    min_xyz = [min([min([p[i] for p in t]) for t in T]) for i in range(3)]
    max_xyz = [max([max([p[i] for p in t]) for t in T]) for i in range(3)]
    norm = max([a-b for a,b in zip(max_xyz, min_xyz)])

    scale = lambda t: [tuple([int((p[i]-0.5*(min_xyz[i] + max_xyz[i]))/(norm) * 127) for i in range(3)]) for p in t]
    return [scale(t) for t in T]

def GetString(triangles):
    '''Get a string with triangle coordinates'''
    
    S = []
    for R in triangles:
        for p in R:
            for x in p:
                S.append(x)
    S = [str(n) for n in S]
    
    return "{" + ", ".join(S) + "}"

def CalcRenderVars():
    
    global SPRITE
    global N_TRIANGLES
    global SCALE
    
    mesh_ = mesh.Mesh.from_file(SPRITE_STL_FILE)
    triangles = [tuple([tuple(p) for p in [T[:3], T[3:6], T[6:]]]) for T in mesh_.points]
    T = process(triangles)

    SPRITE = GetString(T)
    N_TRIANGLES = str(len(T))
    
    if eval(N_TRIANGLES) >= 450:
        msg = '''Number of triangles in STL is more than 450 ({}). RAM in tinycircuits might not be enough to render model.'''.format(N_TRIANGLES)
        warnings.warn(msg)
    
    SCALE = str(eval(SCALE) * 0.004)
        
def CreateTinyCircuitRenderCode():
    
    template_folder = ".\\TinyDuino_Code_Template\\tinycircuits_3D_shading\\"
    generated_folder = ".\\TinyDuino_Code_Generated\\tinycircuits_3D_shading\\"
    
    if not os.path.exists(".\\TinyDuino_Code_Generated"):
        os.mkdir(".\\TinyDuino_Code_Generated")
        
    if not os.path.exists(generated_folder):
        os.mkdir(generated_folder)
        
    files = os.listdir(template_folder)
    for file in files:
        file = os.path.join(template_folder, file)
        shutil.copy(file, generated_folder)
    
    f = open(generated_folder + "tinycircuits_3D_shading.ino", "r")
    c = f.read()
    c = c.replace("<<BACKGROUND_COL>>", BACKGROUND_COL)
    c = c.replace("<<N_TRIANGLES>>", N_TRIANGLES)
    c = c.replace("<<SPRITE>>", SPRITE)
    c = c.replace("<<SCALE>>", SCALE)
    f = open(generated_folder + "tinycircuits_3D_shading.ino", "w")
    f.write(c)
    
    f = open(generated_folder + "Shading.h", "r")
    c = f.read()
    c = c.replace("<<PHONG_AMBIANCE_COL>>", PHONG_AMBIANCE_COL)
    c = c.replace("<<PHONG_AMBIANCE_INTENSITY>>", PHONG_AMBIANCE_INTENSITY)
    c = c.replace("<<PHONG_DIFFUSE_COL>>", PHONG_DIFFUSE_COL)
    c = c.replace("<<PHONG_DIFFUSE_INTENSITY>>", PHONG_DIFFUSE_INTENSITY)
    c = c.replace("<<PHONG_SPECTRAL_COL>>", PHONG_SPECTRAL_COL)
    c = c.replace("<<PHONG_SPECTRAL_INTENSITY>>", PHONG_SPECTRAL_INTENSITY)
    c = c.replace("<<SHINYNESS>>", SHINYNESS)
    f = open(generated_folder + "Shading.h", "w")
    f.write(c)
    
    f.close()

def main():
    CalcRenderVars()
    CreateTinyCircuitRenderCode()
    
main()