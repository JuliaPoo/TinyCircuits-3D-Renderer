# 3D Renderer on TinyDuino

The python file will generate .ino files to render any stl file with lesser than 450 triangles with phong shading on TinyCircuits. Why? I don't know the answer to that either.

## Usage

### Materials
<ul>
  <li> <p>TinyDuino Processor Board</p> </li>
  <li> <p>USB TinyShield</p> </li>
  <li> <p>TinyScreen - OLED TinyShield</p> </li>
</ul>

### Software Required
<ul>
  <li> <p>Arduino IDE to compile the code</p> </li>
  <li> <p>Python to generate the TinyCircuits code</p> </li>
</ul>

### Software Dependencies
These are required to use the ```stl2triangles.py``` file.

```
numpy
numpy-stl
```

### Instructions

Modify these variables in ```stl2triangles.py```:

```
SPRITE_STL_FILE
SCALE
BACKGROUND_COL
PHONG_AMBIANCE_COL
PHONG_AMBIANCE_INTENSITY
PHONG_DIFFUSE_COL
PHONG_DIFFUSE_INTENSITY
PHONG_SPECTRAL_COL
PHONG_SPECTRAL_INTENSITY
SHINYNESS
```

Where ```SPRITE_STL_FILE``` is the filepath to the STL file, ```SCALE``` determines how big the render will be on the screen, ```BACKGROUND_COL``` is the 8-bit colour of the background on the screen and the rest are material variables according to the [Phong Reflection Model](https://en.wikipedia.org/wiki/Phong_reflection_model)

Running ```stl2triangles.py``` with Python will generate a folder ```.\TinyDuino_Code_Generated\tinycircuits_3D_shading\``` containing the TinyCircuit program to render.

Open said folder in Arduino IDE, compile and upload to the TinyDuino Processor Board. Here's a [setup tutorial](https://learn.tinycircuits.com/Processors/TinyDuino_Setup_Tutorial/) to do so.

## Demo

<img src="https://github.com/JuliaPoo/TinyCircuits-3D-Renderer/blob/master/tinyduino_dog_render.gif" alt="Process video result" width="400">

## Acknowledgments

```DrawTriangles.ino``` is modified from the source code of Adafruit-GFX-Library/Adafruit_GFX.cpp for use in TinyCircuits
