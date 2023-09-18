# Later Gator
<img width="678" alt="Screenshot 2023-08-14 at 2 54 06 PM" src="https://github.com/MasonSelf/SynthsOfSelf_LaterGator/assets/55724853/b7597a55-b1a3-424b-b68b-354b570fdcf7">


Later Gator is a stereo audio effect plugin made with [JUCE](https://github.com/juce-framework/JUCE). In the style of a "sample delay" utility, 
Later Gator applies a delay to the left and/or right audio channel, 
useful for simulating stereo width, or simply adjusting timing. Unlike some sample delays, Later Gator employs a 
fractional delay line to facilitate smooth changing of delay time parameters. Under modulation, Lator Gator becomes
a performance tool ready to be interacted with in real time. 

## Submodules

To facilitate code sharing with [Shave N Fade](https://github.com/MasonSelf/SynthsOfSelf_ShaveNFade) 
(and more plugins in the future), much of the codebase for this project is included in the following submodules:
* [sos_IAudioProcessor](https://github.com/MasonSelf/sos_IAudioProcessor)
* [sos_drop_down_window](https://github.com/MasonSelf/sos_drop_down_window)
* [sos_modulator](https://github.com/MasonSelf/sos_modulator)
* [sos_maths](https://github.com/MasonSelf/sos_maths)
* [sos_curve_adjuster](https://github.com/MasonSelf/sos_curve_adjuster)
* [sos_slider](https://github.com/MasonSelf/sos_sliders)

These submodules are included via the [Projucer](https://docs.juce.com/master/tutorial_new_projucer_project.html) as 
"user modules" in 
[LatorGator.jucer](https://github.com/MasonSelf/SynthsOfSelf_LaterGator/blob/main/LaterGator.jucer). 

## End User Documentation

* [Getting Started With Later Gator Video](https://youtu.be/ejBIt9U87zo?si=i24O7zv_caMcFMyN)
* [Written User Guide](https://www.masonself.com/later-gator-manual)




