# Gam

Almost everything you need to make a game.

Gam is a library for making games in C++ with SDL2 and SDL2_mixer.  It is
loosely based on [Reconstructing Cave Story][1].  It is somewhat opinionated and
probably has plenty of warts.

[1]: https://youtu.be/IufkC1IRY2Q?list=PL006xsVEsbKjSKBmLu1clo85yLrwjY67X

## Usage

Gam creates games as one or more independent units called "screens".  As an
example, you might have a title screen which transitions into a gameplay screen
which finally transitions into a game over or victory screen.  Each screen must
implement its own update and draw methods.

### Update

`bool update(Input& input, Audio& audio, unsigned int elapsed)`

This method gets called once per frame (for now - I may change this to do as
many updates as possible per frame or something in the future).  It is passed an
Input object for checking input, an Audio object for playing sound effects, and
the number of ticks (milliseconds) since the last frame.

The update method is responsible for updating any game state and returns true if
the current screen should remain active.  When the update method returns false,
the engine will load the next screen and destroy the current one.

### Draw

`void draw(Graphics& graphics) const`

This method gets called once per frame.  It is passed a Graphics object which
can be used to draw primitives or passed to other objects which draw themselves.
This method is const, so you cannot change any game state during this call.

## Main Classes

### Input

The input processing badly needs an overhaul so this is all bound to change in
the future, once I settle on a design that I don't hate.

The input object can be queried for key presses and releases which are reset
each frame.  Alternatively, it can be queried for if a key is currently held.
Keys are given as `SDL_SCANCODE_` constants to allow the one guy I know that
uses dvorak (in software) to play games with the same keys as normal people.

### Audio

The audio object can be used to play samples (sound effects) or music tracks.
This class uses SDL2_mixer to do software mixing.  As such, several sound
effects can be played simultaneously, but only one music track can be played.

### Graphics

The graphics object has a few basic primitives which are mostly useful for
debugging but can be used to render simple objects if desired.  More commonly,
the `Sprite` and `SpriteMap` classes are used to blit sprites to the screen.

## Resource Loading

Resource loading is currently done on first use.  This will need to change
eventually to something more proactive, including support for a preload state
to show a loading screen.

Currently all resources must live in a directory named `content` in the current
working directory of the binary.  All images must be bitmaps, and the files
must end in ".bmp".  All sound samples must be .wav files, and all music must be
in .ogg files.  Support for other formats is planned for the future.

## Further Documentation

There isn't any, read the headers.

## Contributing

I am happy to read any pull requests that add potentially useful features or
clean up some ugliness.  I am also happy to read suggestions, feature requests,
bug reports, compliments, criticisms, and flaming.
