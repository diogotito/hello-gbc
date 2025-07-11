hello-gbc
=========

An experiment with GBDK for a TRPG concept for the GB Compo 2025. We don't have a name for this yet.

Build Instructions
-------------------

### Prerequisites

- GNU Make
- a GBDK release extracted somewhere
- a `GBDK_HOME` enviornment variable with the path where you extracted GBDK

### General steps

```sh
make
```

### After changing the graphics in `res/`

```sh
cd res
png2asset ruins.png -map -tile_origin 0 -use_map_attributes
png2asset dude-sheet.png -tile_origin 80 -sw 16 -sh 16 -px 0 -py 0

cd ..
make
```

Development
-----------

can be done on Linux and Mac with GBDK and GNU Make installed.

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

Footage
-------
https://github.com/user-attachments/assets/a8d67034-f2ca-4366-a0cd-7279e450dfbd
