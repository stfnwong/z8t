# Z8T
A Z80 assembler, disassembler, and emulator.

[![Build Status](https://travis-ci.com/stfnwong/z8t.svg?token=8stNvsTq48pbK7bVmwzi&branch=master)](https://travis-ci.com/stfnwong/z8t)

And maybe eventually RTL as well.

### TODO : Most of everything
- Assembler is in progress.
- Disassembler is not started.
- Emulator is started, but very incomplete.

The careful reader will be able to deduce that I have no format background in
parsing, lexing, etc.

## Requirements 
- Catch2 (included)
- gcc/clang with C++17 support. Even so, there aren't many features used here that should require more than C++11, but I build with `-std=c++17`.
