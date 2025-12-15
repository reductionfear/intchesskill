# Quick Reference: VCL vs. MinGW Version

## Overview

This guide helps users transition from the original VCL-based build to the new MinGW/MSVC build.

## What Changed

### User-Visible Changes

| Feature | VCL Version | MinGW Version |
|---------|-------------|---------------|
| **GUI** | Windows Forms with buttons, menus | Console-based with hotkeys |
| **Build Tool** | Borland C++ Builder IDE | CMake + MinGW/MSVC |
| **Debug Windows** | Visual board display, square viewer | Not available (core logic works) |
| **Settings Dialog** | Form with dropdowns and inputs | Edit `detection.ini` file |
| **Engine Config** | Dialog with file picker | Hardcoded in `main_win32.cpp` |
| **Status Display** | Form controls (labels, shapes) | Console text output |

### What Still Works

✅ **All core functionality is preserved:**

- Screen capture and board detection
- Piece recognition (all supported sites)
- Chess engine integration (UCI)
- Automatic move execution
- Hotkey controls (W, B, A, Z, S, X)
- Configuration files (detection.ini)

### What's Different

#### Starting the Program

**VCL Version:**
```
Double-click internetchesskiller.exe → GUI window opens
```

**MinGW Version:**
```
Double-click internetchesskiller.exe → Console window opens
Shows status messages and waits for board detection
```

#### Configuration

**VCL Version:**
- Menu → Parameters → Select site type from dropdown
- Menu → Engine → Browse for engine executable

**MinGW Version:**
- Edit `detection.ini` for site-specific settings
- Edit `main_win32.cpp` line 12 to change default site:
  ```cpp
  const int DEFAULT_PROGRAM_TYPE = 9; // chess.com
  ```
- Place engine as `stockfish.exe` in same directory

#### Hotkeys

**Both versions support the same hotkeys:**

| Key | Function |
|-----|----------|
| W | Toggle White autoplay |
| B | Toggle Black autoplay |
| A | Increase White time (+1s) |
| Z | Decrease White time (-1s) |
| S | Increase Black time (+1s) |
| X | Decrease Black time (-1s) |
| ESC | Exit (MinGW only) |

#### Debug/Status Information

**VCL Version:**
- Debug → Show Debug Window (visual board)
- Debug → Square Debug (pixel viewer)
- Debug → Screen Debug (capture viewer)
- Main form shows colored indicators (green/red)

**MinGW Version:**
- Console shows text status messages
- No visual debug windows (board still detected correctly)
- To debug: check console output for "Board captured" messages

## Migration Guide

### For End Users

1. **Download new build** (when available) or build from source
2. **Copy your settings:**
   - `detection.ini` → works with both versions
   - `standard.lrn` → works with both versions
3. **Engine setup:**
   - Place UCI engine (e.g., `stockfish.exe`) in same directory
4. **Run the program:**
   - Console window will appear
   - Follow on-screen instructions
   - Use hotkeys as before

### For Developers

If you were modifying the VCL version:

**GUI Changes:**
- VCL Forms → Now stubbed out
- To add GUI: Use Win32 API, Qt, or wxWidgets
- See `main_win32.cpp` for entry point

**Build System:**
- `*.bpr` files → Now `CMakeLists.txt` and `Makefile.mingw`
- Borland compiler → MinGW-w64, MSVC, or Clang
- See [BUILD.md](BUILD.md) for instructions

**Code Structure:**
- `internetchesskiller.cpp` (VCL) → `main_win32.cpp` (Win32)
- `TThread` → `std::thread`
- `AnsiString` → `std::string`
- VCL components → Removed/stubbed

**API Changes:**
- `TEngine::TEngine(AnsiString)` → `TEngine::TEngine(const std::string&)`
- `TUCIInterface::TUCIInterface(AnsiString)` → `TUCIInterface::TUCIInterface(const std::string&, int)`
- `TMainThread(bool)` → `TMainThread(TEngine*)`

See [PORTING_SUMMARY.md](PORTING_SUMMARY.md) for complete list of changes.

## Supported Platforms

### VCL Version
- ✅ Windows (Borland C++ Builder required)
- ❌ Cannot be built with free compilers

### MinGW Version
- ✅ Windows (MinGW-w64, MSVC, Clang)
- ✅ Free/open-source compilers
- ✅ CI/CD friendly
- ❌ Still requires Windows (Win32 API for screen capture)

## Site Support

Both versions support the same chess sites:

- ✅ chess.com (ProgramType = 9)
- ✅ lichess.org (ProgramType = 10)
- ✅ All legacy sites (playchess, kurnik, winboard, etc.)

Configuration is identical via `detection.ini`.

## Performance

**No performance difference** - the core algorithms are unchanged:
- Screen capture: Same Win32 GDI calls
- Board detection: Same pixel analysis
- Engine: Same UCI communication

## Troubleshooting

### "No GUI window appears"

This is expected! The MinGW version is console-based. The console window **is** the main interface.

### "How do I change settings?"

Edit `detection.ini` for site-specific settings. For engine path, currently modify `main_win32.cpp` and rebuild, or ensure engine is named `stockfish.exe`.

### "Board not detected"

1. Check console for error messages
2. Verify `detection.ini` matches your chess site
3. See [CHESS_COM_LICHESS_SUPPORT.md](CHESS_COM_LICHESS_SUPPORT.md) for site-specific setup

### "I miss the GUI"

The GUI can be re-added using:
- **Win32 API**: For lightweight native Windows GUI
- **Qt**: For cross-platform modern GUI
- **wxWidgets**: For cross-platform native look
- **Dear ImGui**: For game-style overlay

Contributions welcome! The core logic is fully separated.

## Feature Comparison

| Feature | VCL | MinGW |
|---------|-----|-------|
| Core detection | ✅ | ✅ |
| Engine integration | ✅ | ✅ |
| Move execution | ✅ | ✅ |
| Hotkeys | ✅ | ✅ |
| Configuration files | ✅ | ✅ |
| **GUI windows** | ✅ | ❌ |
| **Visual debug** | ✅ | ❌ |
| **Settings dialogs** | ✅ | ❌ |
| **Open source build** | ❌ | ✅ |
| **CI/CD** | ❌ | ✅ |
| **Free compilers** | ❌ | ✅ |

## Future Enhancements

Possible additions to the MinGW version:

1. **Win32 GUI**: Lightweight native Windows interface
2. **Advanced console**: Color ANSI output, status bar
3. **Config GUI**: Separate settings editor utility
4. **Web UI**: Local web server for browser-based control
5. **Logging**: Detailed file-based logging

See GitHub Issues for planned features and contribute ideas!

## Getting Help

- **Build Issues**: See [BUILD.md](BUILD.md)
- **Site Detection**: See [CHESS_COM_LICHESS_SUPPORT.md](CHESS_COM_LICHESS_SUPPORT.md)
- **Porting Details**: See [PORTING_SUMMARY.md](PORTING_SUMMARY.md)
- **General Usage**: See [README.md](README.md)
- **Report Bugs**: Open GitHub Issue

## Contributing

The MinGW version is fully open source! Contributions welcome for:

- GUI additions (Win32, Qt, wxWidgets)
- Additional chess site support
- Build improvements
- Bug fixes
- Documentation

See repository for contribution guidelines.

---

**Both versions detect boards and play chess equally well - just with different interfaces!**
