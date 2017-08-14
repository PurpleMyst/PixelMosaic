# PurpleMyst

First off, a huge thanks to Crystal for helping me with the algorithm.

The algorithm makes a 4x4 tile with 4 colors, then fills the full 512x512 image
with that.

To keep things interesting, we then do a few passes over the image, randomly
assigning each square to a valid color, including the color it already is. The
script submitted only does 1 pass, but it can be changed by modifying a
constant.

The script can take multiple seconds to run, even around 30 I've found. I
suspect this is due to the slowness of Python combined with the slowness of
randomness.

To run the script, you will just need a copy of
[Python **3**](https://www.python.org/downloads) and the
[Pillow module](http://pillow.readthedocs.io/en/4.2.x/installation.html#basic-installation)

There is also a C++ version of the same algorith included, which has no
dependencies but outputs a NetPBM. To compile it, only a C++11 compliant compiler is required. 
An example compilation and usage, with the outputs of some commands removed for
brevity, is:

```bash
$ clang++ -std=c++11 mosaic_maker.cpp -o mosaic_maker.out
$ ./mosaic_maker.out > mosaic.pbm
$ feh --force-aliasing mosaic.pbm  # Shows the image in feh, which supports PBM images.
$ convert mosaic.pbm mosaic.png  # It's possible to convert the PBM to a PNG using the ImageMagick CLI.
```
