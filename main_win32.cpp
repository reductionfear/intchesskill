//---------------------------------------------------------------------------
// InternetChessKiller - Minimal Win32 Entry Point (No VCL)
// Ported from Borland C++ Builder VCL to standard Win32 API
//---------------------------------------------------------------------------

#include <windows.h>
#include <iostream>
#include <string>
#include "TEngine.h"
#include "TMainThreadObject.h"

// Global variables
TEngine* g_Engine = nullptr;
TMainThread* g_MainThread = nullptr;
bool g_Running = true;

// Default configuration
const int DEFAULT_PROGRAM_TYPE = 9; // chess.com
const int DEFAULT_MULTIPV = 1;
const int DEFAULT_HASH_SIZE = 64;

//---------------------------------------------------------------------------
// Console output helper
void LogMessage(const char* message) {
    std::cout << message << std::endl;
}

//---------------------------------------------------------------------------
// Initialize the engine and detection system
bool InitializeEngine() {
    try {
        LogMessage("Initializing InternetChessKiller...");
        
        // Look for chess engine executable
        std::string enginePath = "stockfish.exe";
        
        // Check if engine exists
        DWORD attrib = GetFileAttributesA(enginePath.c_str());
        if (attrib == INVALID_FILE_ATTRIBUTES) {
            LogMessage("Warning: stockfish.exe not found in current directory");
            LogMessage("Attempting to use any available UCI engine...");
            // Try common alternatives
            const char* alternatives[] = {
                "fruit.exe",
                "engine.exe",
                "chess_engine.exe"
            };
            bool found = false;
            for (const char* alt : alternatives) {
                attrib = GetFileAttributesA(alt);
                if (attrib != INVALID_FILE_ATTRIBUTES) {
                    enginePath = alt;
                    found = true;
                    break;
                }
            }
            if (!found) {
                LogMessage("ERROR: No UCI chess engine found!");
                LogMessage("Please place stockfish.exe or another UCI engine in the same directory.");
                return false;
            }
        }
        
        LogMessage(("Using engine: " + enginePath).c_str());
        
        // Create engine instance
        g_Engine = new TEngine(enginePath);
        g_Engine->BoardRecognize.ProgramType = DEFAULT_PROGRAM_TYPE;
        g_Engine->BoardRecognize.BoardCapture.ProgramType = DEFAULT_PROGRAM_TYPE;
        
        LogMessage("Engine initialized successfully");
        LogMessage("Board recognition configured for chess.com");
        
        // Create and start main thread
        g_MainThread = new TMainThread(g_Engine);
        g_MainThread->Start();
        
        LogMessage("Main thread started");
        LogMessage("");
        LogMessage("=== InternetChessKiller Running ===");
        LogMessage("Hotkeys:");
        LogMessage("  W - Toggle White autoplay");
        LogMessage("  B - Toggle Black autoplay");
        LogMessage("  A/Z - Adjust White time (+/-)");
        LogMessage("  S/X - Adjust Black time (+/-)");
        LogMessage("  ESC - Quit");
        LogMessage("");
        LogMessage("The program will automatically detect chessboards on screen.");
        LogMessage("Press ESC to exit...");
        
        return true;
    }
    catch (const char* error) {
        std::cerr << "ERROR: " << error << std::endl;
        return false;
    }
    catch (...) {
        std::cerr << "ERROR: Unknown exception during initialization" << std::endl;
        return false;
    }
}

//---------------------------------------------------------------------------
// Cleanup on exit
void Shutdown() {
    LogMessage("Shutting down...");
    
    if (g_MainThread) {
        g_MainThread->Stop();
        delete g_MainThread;
        g_MainThread = nullptr;
    }
    
    if (g_Engine) {
        delete g_Engine;
        g_Engine = nullptr;
    }
    
    LogMessage("Shutdown complete");
}

//---------------------------------------------------------------------------
// Main message loop
void MessageLoop() {
    while (g_Running) {
        // Check for ESC key to exit
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            LogMessage("ESC pressed - exiting...");
            g_Running = false;
            break;
        }
        
        // Sleep to avoid busy-waiting
        Sleep(100);
    }
}

//---------------------------------------------------------------------------
// Windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    // Allocate console for output
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
    
    std::cout << "=====================================" << std::endl;
    std::cout << "  InternetChessKiller v1.71 (MinGW)" << std::endl;
    std::cout << "  Open Source Win32 Build" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;
    
    int result = 0;
    
    try {
        if (!InitializeEngine()) {
            std::cerr << "Failed to initialize engine" << std::endl;
            result = 1;
        } else {
            MessageLoop();
        }
    }
    catch (const char* error) {
        std::cerr << "FATAL ERROR: " << error << std::endl;
        result = 1;
    }
    catch (...) {
        std::cerr << "FATAL ERROR: Unknown exception" << std::endl;
        result = 1;
    }
    
    Shutdown();
    
    std::cout << std::endl;
    std::cout << "Press ENTER to exit...";
    std::cin.get();
    
    FreeConsole();
    
    return result;
}
//---------------------------------------------------------------------------
