# Verification Checklist - MinGW/MSVC Port

## Pre-Build Verification ✅

- [x] All VCL includes removed from core files
- [x] No references to MainForm, BoardScanerWindow, etc. in compiled code
- [x] All AnsiString replaced with std::string
- [x] TThread replaced with std::thread
- [x] CMakeLists.txt created and tested
- [x] Makefile.mingw created
- [x] All Borland pragmas removed from core files
- [x] Documentation complete (BUILD.md, PORTING_SUMMARY.md, etc.)

## Build Testing (Requires Windows)

### MinGW-w64 Build

- [ ] CMake configuration succeeds
  ```bash
  mkdir build && cd build
  cmake -G "MinGW Makefiles" ..
  ```
  
- [ ] Compilation succeeds without errors
  ```bash
  mingw32-make
  ```

- [ ] All object files generated (check build/ directory)

- [ ] Executable created (build/bin/internetchesskiller.exe)

- [ ] Direct Makefile build succeeds
  ```bash
  mingw32-make -f Makefile.mingw
  ```

### MSVC Build

- [ ] Visual Studio CMake configuration succeeds
  
- [ ] Compilation succeeds in VS 2019/2022

- [ ] Executable created

- [ ] Command-line build succeeds
  ```cmd
  cmake -G "Visual Studio 16 2019" ..
  cmake --build . --config Release
  ```

## Runtime Testing (Requires Windows + Chess Site)

### Basic Functionality

- [ ] Program starts without crash
  ```
  .\bin\internetchesskiller.exe
  ```

- [ ] Console window appears with status messages

- [ ] No VCL-related errors

- [ ] ESC key exits program cleanly

### Engine Integration

- [ ] Place stockfish.exe in bin/ directory

- [ ] Program detects and initializes engine

- [ ] No "engine not found" errors

- [ ] UCI communication works (check console output)

### Screen Capture

- [ ] Open chess.com or lichess.org in browser

- [ ] Start a game (or view a position)

- [ ] Program detects screen capture

- [ ] Console shows "Board captured" message

### Board Recognition

- [ ] With chessboard visible on screen

- [ ] Program recognizes board position

- [ ] Console shows "Position recognized" message

- [ ] Piece detection works correctly

### Autoplay Testing

- [ ] Press W key (White autoplay toggle)

- [ ] Console indicates White autoplay enabled

- [ ] Press B key (Black autoplay toggle)

- [ ] Console indicates Black autoplay enabled

- [ ] Engine makes moves automatically (if it's the engine's turn)

- [ ] Moves are executed on the board (mouse simulation)

### Time Controls

- [ ] Press A/Z keys (adjust White time)

- [ ] Press S/X keys (adjust Black time)

- [ ] No crashes or errors

### Configuration Files

- [ ] detection.ini loaded successfully

- [ ] standard.lrn loaded (if present)

- [ ] Custom detection settings applied

## Stress Testing

### Long Session

- [ ] Run for 10+ moves without crashes

- [ ] Memory usage stays stable

- [ ] No resource leaks

### Multiple Sites

- [ ] Test with chess.com

- [ ] Test with lichess.org

- [ ] Change ProgramType in main_win32.cpp and rebuild

- [ ] Detection works for each site

## Edge Cases

### Missing Files

- [ ] Run without detection.ini (should use defaults)

- [ ] Run without standard.lrn (should work)

- [ ] Run without engine (should show error, not crash)

### Invalid Input

- [ ] Rapid hotkey presses (no crashes)

- [ ] Close chess website mid-game (graceful handling)

### Window Management

- [ ] Minimize/restore browser window

- [ ] Move browser window around screen

- [ ] Board still detected after window movement

## Documentation Verification

- [ ] BUILD.md instructions work correctly

- [ ] All links in README.md valid

- [ ] PORTING_SUMMARY.md accurately reflects changes

- [ ] VCL_VS_MINGW.md helpful for users

## Release Preparation

### Binary Distribution

- [ ] Create bin/ folder with:
  - [ ] internetchesskiller.exe
  - [ ] detection.ini
  - [ ] standard.lrn
  - [ ] README.txt (user guide)

- [ ] Test on clean Windows machine (no dev tools)

- [ ] Document minimum Windows version

### Source Distribution

- [ ] All source files included

- [ ] CMakeLists.txt and Makefile.mingw included

- [ ] Documentation complete

- [ ] .gitignore excludes build artifacts

## Performance Comparison

### vs. VCL Version

- [ ] Screen capture speed comparable

- [ ] Board detection speed comparable

- [ ] Engine response time comparable

- [ ] Memory usage similar or better

## Known Issues

Document any issues found:

1. [ ] None yet (to be filled during testing)

## Sign-Off

- [ ] All tests passed
- [ ] Documentation reviewed
- [ ] Ready for release

---

**Testing Date:** _____________

**Tester:** _____________

**Platform:** Windows __ (__ bit)

**Compiler:** MinGW ____ / MSVC ____

**Notes:**
