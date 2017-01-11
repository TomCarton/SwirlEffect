# SwirlEffect

![](https://github.com/TomCarton/SwirlEffect/blob/master/images/swirl.jpg)

While sorting files and organizing my backups (so much to clean) I found back an old effect that I developed for GBA back in 2001.

The compiled *.bin* file is included. It can be runned using any GBA emulator.

###The effect###

This sample changes the screen to another picture using a
grey picture as reference (kind of a pixel table).
This allows to perform some swirl effects.

There is also a fade in/fade out made using the palette.

###Commands###
- A Button : Swirl effect
- B Button : Palette Fade

###How it works###

This effect is based on sending pixels to the frame buffer following a 'swirl' table.

![](https://raw.githubusercontent.com/TomCarton/SwirlEffect/master/images/swirlref_1.bmp)

A swirl table can be obtained by drawing an image with gradients.
The effect picks pixels with color#0, then color#1, ... to color#255.

Thus you can influct the way the next image appears.

![](https://raw.githubusercontent.com/TomCarton/SwirlEffect/master/images/swirlref_2.bmp)
![](https://raw.githubusercontent.com/TomCarton/SwirlEffect/master/images/swirlref_3.bmp)
