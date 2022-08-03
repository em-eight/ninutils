# ninutils
A binutils-like C++ library/CLI introspection tool for nintendo GameCube and Wii binary formats (DOL and REL).

## Abstract
Projects that handle DOL/REL files frequently reinvent the wheel by implementing their own tools from 
near scratch. This project attempts to cover most generic binary manipulation needs or at least 
provide a solid baseline for developing custom tools.
It also offers nice CLI utilities that don't suffer from the information loss from modeling in an 
existing binary manipulation library (e.g. BFD/radare2).

To achieve such thing, this project aims to:
- offer as simple and flexible interfaces as possible. Devs should be able to skim the header files
and get a good idea on how to achieve what they want.
- cover most common operations on DOL/REL binaries anyone would want. (please contribute issues/code
 if you know any!)
- have a CLI that resembles the generally familiar binutils.
- be very fast and make close to zero performance compromises. This prevents people from considering 
their own tools for performance reasons.

## Building/Installation
```
mkdir build
cd build
cmake ..
make
make install
```
You may need to adjust your PATH/LD_LIBRARY_PATH to the path of the installed executables/libraries.

## Tools/Libraries
### `libdolrel`
C++ library for manipulating DOL/REL files. Quickstart:
```c++
Dol dol(dolpath);
Rel rel(relpath);
SymbolTable symtab(rel);
dol.print(std::out);                         // Print DOL info to std::out
rel.print(std::out, true, true, true, true); // Print all REL info to std::out
symtab.print(std::out);                      // Print symbols inferred from REL relocs to std::out
```
### `readdol`
CLI introspection tool for DOLs. Inspired by `readelf`.
```
Usage: readdol <option(s)> dol_file
 Display information about the contents of DOL files
 Options are:
        -w  Print in raw format, aka as is from the file
```
### `readrel`
CLI introspection tool for RELs. Inspired by `readelf`.
```
Usage: readrel <option(s)> rel_file
 Display information about the contents of REL files
 Options are:
        -h           Print REL header
        -S           Print REL section table
        -i           Print REL imp table
        -r           Print REL relocations
        -s           Print REL inferred symbols
        -w           Print in raw format, aka as is from the file
        -p <preset>  Specify a preset to get game-specific info (e.g. VMAs of REL symbols)
Available presets:
mkw-pal     Mario Kart Wii PAL
```
## External information
Aside from the binaries themselves, external information about the context that the binaries are loaded
can be provided with the class `ExtraInfo`, to enable functionality that requires them.
The library also contains presets of the objects that can be passed to the CLI tools that can 
enrich/extend their output. To add a preset for your game of interest to this library, simply add an
 `else if` statement to the ExtraInfo constructor at `src/extra_info.cpp`
## Documentation
Currently in progress. The header files are relatively small for now and contain plenty of comments.

## Roadmap
- Support for providing symbol names.
- `reldump`/`doldump` tools.
- Disassembly and `disas` CLI tool.
- python bindings

## Acknowledgements
Thanks to Seeky, terorie, riidefi, Chadderz, wiki tockdom contributors and all others that helped me 
understand how these formats work.