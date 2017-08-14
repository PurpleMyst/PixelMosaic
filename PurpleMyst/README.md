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
