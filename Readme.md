
# hello-gbc

<img src="res/art/accidental_transparency.gif" width="320" height="64" alt="&quot;HELLO, WORLD!&quot" align="right">

An experiment with GBDK for a TRPG concept for the GB Compo 2025. We don't have a name for this yet.

## Build Instructions

### Prerequisites

- a GBDK release extracted somewhere
- a `GBDK_HOME` enviornment variable with the path where you extracted GBDK

### Building `obj/Example.gb`

Just invoke `make` or launch `build_and_run.bat` depending on your system.

Then launch `obj\Example.gb` on your favourite emulator:

<https://github.com/user-attachments/assets/7f867e8b-3e77-4b5d-a714-b58fe6636e25>

## Development

can be done on Linux and Mac with GBDK and **GNU Make** installed.

For VS Code, `.vscode/tasks.json` provides Build Tasks that invoke `make` directly
on Linux and through WSL on Windows[^1] and `.vscode/launch.json` allows you to
build the ROM, open it with Emulicious and connect to it to debug the ROM from
VS Code through the
[Emulicious Debugger](https://marketplace.visualstudio.com/items?itemName=emulicious.emulicious-debugger)
extension.
You should also be able to invoke all GBDK tools from the integrated Terminal
if you setup your `GBDK_HOME` enviornment variable correctly in your system.

[^1]: you need to have both make and GBDK installed in your default WSL distro
and a `$GBDK_HOME` enviornment variable with the path where you installed GBDK
in the shell invoked by make.

### When changing the graphics in `res/`

Regenerate the matching `.c` and `.h` files with `png2asset` by running `make`
or launching `_make_gfx.bat` depending on your system.
