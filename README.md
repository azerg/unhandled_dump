## Unhandled Dump library
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

## Overview

This C++ library is designed to catch uncaught exceptions and generate detailed dumps, providing critical insights for debugging. Useful because certain exceptions, like division by zero, cannot be caught by standard C++ exception handling mechanisms. By using unhandled_dump, developers can receive crash dump from user, instead of words "it crashes without any logs".

## Usage:

    #include "last_dumper.h"
    int main() {
      // you are protected from unhandled exceptions as long as "dumper" object exists
      const auto dumper = InitLastDumper("");
      ...
    }

## Todo:

 - [ ] release pipeline
 - [ ] Unix support
 - [ ] Right now library is able to dump itself only, but there are plans to extend it to "client-server" - one place to setup dumping engine, that will be linked into other (parent?) processes.
 
## Overview

#Dependencies
- cmake
- vcpkg