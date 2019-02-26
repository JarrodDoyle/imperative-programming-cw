Overworld map generator:
My extra work for this week is a map generator using my own implementation of Perlin noise
which is displayed using a modified version of the display module used in the first 50% of
the assessment.
The user can customise the maps using an interface in the console. One issue is that on
launching the program, the SDL window takes priority so you have to click back on the console
to use the UI. The SDL window also has a "This program is non-responsive" window pop up not
long after generating a map. This is because I couldn't work out how to make it inform the
OS that it's still working but you should be able to just click "wait".

Things the user can customise:
 - Width and Height of the map
 - Frequency of the noise (essentially the zoom level)
 - Number of noise octaves (how many layers of noise. Higher values make map less smooth)
 - Persistence (how quickly the amplitudes diminish for each successive octave)
 - Lacunarity (how quickly the frequency increases for each successive octave)

Some values that can give appealing maps:
 - Frequency = 4
 - Octaves = 6
 - Persistence = 0.5
 - Lacunarity = 1
