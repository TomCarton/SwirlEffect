# SwirlEffect

![](https://github.com/TomCarton/SwirlEffect/blob/master/images/swirl.jpg)

While sorting files and organizing my backups (so much to clean). I found back an old effect that I developed for GBA back in 2001.

The compiled bin file is included. It can be runned using an GBA emulator.

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

A swirl table can be obtained by drawing an image with gradients from color 0 to color 255.

![](https://raw.githubusercontent.com/TomCarton/SwirlEffect/master/images/swirlref_1.bmp)
![](https://raw.githubusercontent.com/TomCarton/SwirlEffect/master/images/swirlref_2.bmp)
![](https://raw.githubusercontent.com/TomCarton/SwirlEffect/master/images/swirlref_3.bmp)
