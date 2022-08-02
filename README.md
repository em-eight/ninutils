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
their own tools for performance reasons, and subtly puts pressure on projects using less performant 
tools to migrate.

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
### `librel`
C++ library for manipulating REL files. Quickstart:
```c++
Rel rel(relpath);
SymbolTable symtab(rel);
rel.print(std::out, true, true, true, true); // Print all REL info to std::out
symtab.print(std::out);                      // Print symbols inferred from REL relocs to std::out
```
### `readrel`
CLI introspection tool for RELs. Inspired by `readelf`.
```
Usage: readrel <option(s)> rel_file
 Display information about the contents of REL files
 Options are:
        -h  Print REL header
        -S  Print REL section table
        -i  Print REL imp table
        -r  Print REL relocations
        -s  Print REL inferred symbols
        -w  Print in raw format, aka as is from the file
```

## Documentation
Currently in progress. The header files are relatively small for now and contain plenty of comments.

## Roadmap
- DOL support.
- Support for providing symbol names.
- `reldump`/`doldump` tools.
- Support providing presets on game-specific info (e.g. REL load address)
- Disassembly and `disas` CLI tool.
- python bindings

## Acknowledgements
Thanks to Seeky, terorie, riidefi, Chadderz, wiki tockdom contributors and all others that helped me 
understand how these formats work.