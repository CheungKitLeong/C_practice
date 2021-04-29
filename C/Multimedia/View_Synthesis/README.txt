The file viewSynthesis.cpp included all the features of Basic part and Enhancement part mentioned in the specification.
It supports the input of a full 3D view-plane, and variable focal length.
Execution format:
C:\> veiwSynthesis <LF_dir> <viewpoint_coord> <focal_length>
In details:
C:\> veiwSynthesis <LF_dir> <x_coord> <y_coord> <z_coord> <focal_length>
Input domain:
<z_coord> >= 0
<focal_length> > 0, which is optional
<x_coord>, <y_coord> < -120 or > 120 is acceptable but return a black output image.
There is no any "other interesting features", but I implemented my rescaling algorithm to help me do the above task, which I think is better than the resize function in bmp.cpp.