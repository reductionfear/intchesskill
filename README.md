InternetChessKiller

## 🎯 Now Open Source - Build with Free Tools!

**NEW:** This project has been ported from Borland C++ Builder to **MinGW/MSVC**, making it fully buildable with free, open-source compilers!

### Quick Start - Building

```bash
# With MinGW-w64
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make

# With Visual Studio
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```

**📖 See [BUILD.md](BUILD.md) for complete build instructions**

---

## Overview

Program created for automatic use computer chess program
help for playing on chess servers

Short description:
program periodically capture screen,
if chess board has been found with start position,
when chess program has been started
if board has been changed,
when analyze has been changed accordingly

Program works on many chess board game websites:

**Modern Sites (Fully Supported with Adaptive Detection):**
- ✅ **chess.com** - Multiple themes including Green, Brown, and Dark Mode
- ✅ **lichess.org** - Multiple themes including Blue, Brown, and Dark Mode

**Legacy Sites:**
www.playchess.com --- www.chessplanet.ru --- www.bereg.ru --- www.instantchess.com ---
www.kurnik.org --- www.kurnik.pl --- www.playok.com --- www.chessgate.de --- www.spinchat.de ---
www.superchat.at --- www.aby.spinchat.de --- www.chessclub.com dasher (Classic Sets - Book) --- winboard ---

and many more.

**See [CHESS_COM_LICHESS_SUPPORT.md](CHESS_COM_LICHESS_SUPPORT.md) for detailed chess.com and lichess.org configuration and testing information.**

---

## 🏗️ Technical Details

### Architecture
- **Screen Capture**: Pure Win32 GDI (`GetDC`, `BitBlt`, `GetDIBits`) - no VCL dependencies
- **Board Detection**: Adaptive color/edge detection supporting multiple chess sites
- **Engine Interface**: UCI protocol implementation for chess engine communication
- **Threading**: Standard C++ `std::thread` for concurrent operation

### Ported Components
✅ **Fully Portable (No VCL):**
- `TBoardCapture` - Screen capture using Win32 API
- `TBoardRecognize` - Board detection and piece recognition
- `TEngine` - Chess engine integration
- `TUCIInterface` - UCI protocol handler
- `fruit/*` - Chess engine library

❌ **Removed (VCL GUI dependencies):**
- VCL Forms and visual controls
- Replaced with console-based interface
- GUI can be re-added using Win32 API or modern frameworks

### Build Requirements
- **Compiler**: MinGW-w64, MSVC 2019+, or Clang
- **CMake**: 3.10 or higher
- **OS**: Windows (uses Win32 API for screen capture)
- **Chess Engine**: Any UCI-compatible engine (Stockfish recommended)

---

## Credits
## Credits

**Original Author**: Vladislav Kovalev
**Contact**: kovalev1997a11@gmail.com
**VCL to MinGW/MSVC Port**: Community Contributors

That is the source code of v 1.71. 
That is the only source code available.
Version above 2, 3, 3.1 DEMO, 3.1 FULL CRACKED 
are only the exe binaries.

You may watch the videos:

http://www.cda.pl/video/2256328f?wersja=1080p

https://www.youtube.com/watch?v=jbeqN6zsMSE

https://www.youtube.com/watch?v=YwiwKUeuahY&spfreload=10

You may talk on forums:

http://www.chessgod101.com/t2647-internet-chess-killer

http://www.chessgod101.com/t1762-internetchesskiller-full-pack

http://www.chessgod101.com/t3390-internet-chess-killer-2013

http://www.chess2u.com/t4958-internet-chess-killer

http://www.chess2u.com/t6614-internet-chess-destroyer-admax

![Tags: Chess, AI, Engine, Hack, Bot, Auto, Autoplay, Autoplayer, Artificial Intelligence, Gra, Game, Source Code Files, Program, Programming, Github, Board, Coding, Kurnik, Playok, Chess Engine, Cheating.](ick0.png "Tags: Chess, AI, Engine, Hack, Bot, Auto, Autoplay, Autoplayer, Artificial Intelligence, Gra, Game, Source Code Files, Program, Programming, Github, Board, Coding, Kurnik, Playok, Chess Engine, Cheating.")

![Tags: Chess, AI, Engine, Hack, Bot, Auto, Autoplay, Autoplayer, Artificial Intelligence, Gra, Game, Source Code Files, Program, Programming, Github, Board, Coding, Kurnik, Playok.](ick1.gif "Tags: Chess, AI, Engine, Hack, Bot, Auto, Autoplay, Autoplayer, Artificial Intelligence, Gra, Game, Source Code Files, Program, Programming, Github, Board, Coding, Kurnik, Playok.")

![InternetChessKiller](ick2.gif "InternetChessKiller")

![InternetChessKiller](ick3.gif "InternetChessKiller")

![InternetChessKiller](ick4.gif "InternetChessKiller")

![InternetChessKiller](ick5.png "InternetChessKiller")

Also a similiar program to Internet Chess Killer on GitHub: 

https://github.com/Sora-cloud

Tags: Chess, AI, Engine, Hack, Bot, Auto, Autoplay, Autoplayer, Artificial Intelligence, Gra, Game, Source Code Files, Program, Programming, Github, Board, Coding, Kurnik, Playok, Chess Engine, Cheating.
