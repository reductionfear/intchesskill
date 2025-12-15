# Build Instructions - InternetChessKiller

## Overview

InternetChessKiller has been ported from Borland C++ Builder (VCL) to use standard C++ and Win32 API, making it compatible with free/open-source compilers like MinGW-w64 and MSVC.

## Prerequisites

### Option 1: MinGW-w64 (Recommended for Open Source)

1. **Install MinGW-w64**
   - Download from: https://www.mingw-w64.org/downloads/
   - Or use MSYS2: https://www.msys2.org/
   
2. **Install CMake**
   - Download from: https://cmake.org/download/
   - Version 3.10 or higher required

3. **Install a UCI Chess Engine** (Required for functionality)
   - Download Stockfish: https://stockfishchess.org/download/
   - Or another UCI-compatible engine (Fruit, etc.)
   - Place `stockfish.exe` in the build output directory

### Option 2: Microsoft Visual Studio / MSVC

1. **Install Visual Studio 2019 or later**
   - Download Community Edition (free): https://visualstudio.microsoft.com/downloads/
   - Select "Desktop development with C++" workload

2. **CMake** (included with VS 2019+)
   - Or install standalone: https://cmake.org/download/

3. **Install a UCI Chess Engine** (same as above)

## Building with MinGW-w64

### Command Line Build

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake (MinGW Makefiles)
cmake -G "MinGW Makefiles" ..

# Build
mingw32-make

# Output will be in build/bin/internetchesskiller.exe
```

### Alternative: MSYS2 Build

```bash
# In MSYS2 MinGW64 shell
mkdir build
cd build
cmake -G "MSYS Makefiles" ..
make
```

## Building with Visual Studio / MSVC

### Visual Studio IDE

1. Open Visual Studio
2. File → Open → CMake...
3. Select the `CMakeLists.txt` file from the repository root
4. Visual Studio will automatically configure the project
5. Build → Build All (or press F7)
6. Executable will be in `out/build/<config>/bin/`

### Command Line (VS Developer Command Prompt)

```cmd
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -G "Visual Studio 16 2019" ..
# Or for VS 2022: cmake -G "Visual Studio 17 2022" ..

# Build
cmake --build . --config Release

# Output will be in build/Release/bin/
```

## Post-Build Setup

1. **Copy Required Files to Output Directory:**
   ```
   bin/
   ├── internetchesskiller.exe
   ├── stockfish.exe          (or other UCI engine)
   ├── detection.ini          (chess site detection config)
   └── standard.lrn           (learning data)
   ```

2. **Run the Program:**
   - Double-click `internetchesskiller.exe`
   - Or run from command line: `.\bin\internetchesskiller.exe`
   - A console window will appear with status messages

## Usage

### Hotkeys

- **W** - Toggle White autoplay (play as White)
- **B** - Toggle Black autoplay (play as Black)
- **A** / **Z** - Adjust White time (+1s / -1s)
- **S** / **X** - Adjust Black time (+1s / -1s)
- **ESC** - Exit program

### Supported Chess Sites

The program automatically detects chessboards on screen:

- **chess.com** (ProgramType = 9, default)
- **lichess.org** (ProgramType = 10)
- Legacy sites: playchess.com, kurnik.org, winboard, etc.

To change site detection, edit `detection.ini` or modify `DEFAULT_PROGRAM_TYPE` in `main_win32.cpp`.

## Configuration Files

### detection.ini

Contains site-specific detection parameters (board colors, edge detection settings):

```ini
[chess.com]
white_square_threshold=180
black_square_threshold=100
adaptive_detection=true
# ... etc
```

See [CHESS_COM_LICHESS_SUPPORT.md](CHESS_COM_LICHESS_SUPPORT.md) for details.

### standard.lrn

Opening book / learning data for the chess engine (optional).

## Troubleshooting

### "No UCI chess engine found!"

- Download Stockfish from https://stockfishchess.org/download/
- Place `stockfish.exe` in the same directory as `internetchesskiller.exe`
- Or specify custom engine path in code (modify `main_win32.cpp`)

### Build Errors

**Missing windows.h on Linux/Mac:**
- This is a Windows-only application. Build on Windows with MinGW or MSVC.

**CMake not found:**
- Install CMake from https://cmake.org/download/
- Add to PATH environment variable

**Linker errors (undefined reference to WinMain):**
- Make sure you're using `-mwindows` flag (MinGW)
- Or set subsystem to Windows in MSVC project settings

### Runtime Issues

**Board not detected:**
- Check `detection.ini` settings for your chess site
- Verify program type matches your site (chess.com = 9, lichess = 10)
- See detection documentation for site-specific configuration

**Engine not responding:**
- Verify engine executable is valid UCI engine
- Check console output for error messages
- Try different engine (Stockfish, Fruit, etc.)

## Development

### Project Structure

```
internetchesskiller/
├── CMakeLists.txt           # Build configuration
├── main_win32.cpp           # Main entry point (Win32)
├── TEngine.cpp/h            # Chess engine interface
├── TUCIInterface.cpp/h      # UCI protocol handler
├── TBoardCapture.cpp/h      # Screen capture (Win32 GDI)
├── TBoardRecognize.cpp/h    # Board detection
├── TMainThreadObject.cpp/h  # Main worker thread
├── fruit/                   # Chess engine library
├── detection.ini            # Site detection config
└── BUILD.md                 # This file
```

### Porting Notes

The following changes were made from the original VCL version:

1. **Replaced VCL types:**
   - `AnsiString` → `std::string`
   - `TThread` → `std::thread`
   - VCL file operations → standard C/C++ file I/O

2. **Removed VCL dependencies:**
   - All `#include <vcl.h>` removed
   - Borland-specific pragmas removed
   - TForm-based GUI removed (console-based now)

3. **Kept Win32 API:**
   - `GetDC`, `BitBlt`, `GetDIBits` for screen capture
   - `CreateProcess` for engine communication
   - Win32 keyboard input (`GetKeyState`)

4. **Build system:**
   - Added CMakeLists.txt for cross-compiler support
   - Compatible with MinGW, MSVC, and potentially Clang

### Contributing

The codebase is now fully open source and buildable with free tools. Contributions welcome!

- Detection improvements: `TBoardCapture.cpp`, `TBoardRecognize.cpp`
- Engine integration: `TEngine.cpp`, `TUCIInterface.cpp`
- UI improvements: `main_win32.cpp` (currently console-only)

## License

See LICENSE file (if present) or README.md for license information.

## Credits

- Original author: Vladislav Kovalev
- VCL to MinGW port: GitHub Copilot / Community Contributors
- Chess engine (Fruit): Fabien Letouzey
- Detection enhancements: See commit history
