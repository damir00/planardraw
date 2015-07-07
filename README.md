# Planardraw

Render gPlanarity level files to PNG. Those ASCII files are tipically located in `~/.gPlanarity/boards/`. Most of the time those files will be solved levels, but sometimes they can be partially solved or completely unsolved. Planardraw tries to replicate gPlanarity graphics.

#Building

Project depends on cairo.
Project uses CMake, so follow the usual CMake build process:

```
cmake .
make
```

# Usage

`./planardraw input_file output_file`

#Showcase

![Montage of levels rendered with Planardraw](montage.jpg?raw=true)

Generated with:

`montage rendered/* -geometry 512x512 montage.jpg`

