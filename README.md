# planardraw
Render gPlanarity level files to PNG. Most of the time those files will be solved levels, but sometimes they can be partially solved or completely unsolved.

#Building

Project depends on cairo.
Project uses CMake, so follow the usual cmake build process:

```
cmake .
make
```

# Usage

./planardraw input_file output_file

#Showcase

![alt tag](blob/master/montage.png)

Generated with:

`montage rendered/* -geometry 512x512 montage.jpg`

