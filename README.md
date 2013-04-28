(c) Daniel Leblanc 2013

PhotoManipulator
================

The goal of this project is to convert a color image into a grey-scale sketch or grey-scale painting that preserves salient features.

References:

A. Hertzmann, Painterly rendering with curved brush strokes of multiple sizes, SIGGRAPH 1998.
Winnemoller, H., XDoG: advanced image stylization with eXtended Difference-of-Gaussians NPAR 2011

Ideas:
    Scale into a single color intensity scale instead of just a gray-scale
    Click on a color, preserve just that color and turn the rest into gray-scale
    
Task Breakdown:
    Open an image
    Save an image
    Convert RGB to Lab color space
    Color selector for 'gray' scale (sepia tones and similar)
    Color selector for region exclusion from grayscale process
        allow user to select a spot on the color image
        exclude areas that are nearby and similar in color from the conversion
    implement xDoG process
    implement photo to painting process
    implement salient preserving grayscale conversion
    