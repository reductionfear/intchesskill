//---------------------------------------------------------------------------
#ifndef DetectionConfigH
#define DetectionConfigH

#include "ColorUtils.h"

//---------------------------------------------------------------------------
// Configuration structures for adaptive board and piece detection
//---------------------------------------------------------------------------

// Border detection configuration
struct BorderConfig {
    int color;                    // Target border color
    double colorThreshold;        // Distance threshold for color matching
    bool useExactMatch;          // Use exact match vs threshold-based
    
    BorderConfig() : color(0), colorThreshold(30.0), useExactMatch(false) {}
};

// Piece detection configuration
struct PieceConfig {
    int whiteThreshold;          // Brightness threshold for white pieces
    int blackThreshold;          // Brightness threshold for black pieces
    bool useAdaptive;            // Use adaptive thresholds based on square colors
    double contrastRatio;        // Minimum contrast ratio for piece detection
    
    PieceConfig() : whiteThreshold(190), blackThreshold(50), 
                     useAdaptive(false), contrastRatio(1.5) {}
};

// Recognition parameters
struct RecognitionParams {
    int width;                   // Width for piece scanning
    int depth;                   // Depth for piece scanning
    int corner;                  // Corner offset
    int recognizeType;           // Recognition type (0=pixels, 1=square, 2=hybrid)
    int blackMax;                // Maximum black pixel count
    bool calcWhite;              // Calculate white pixels
    
    RecognitionParams() : width(0), depth(0), corner(0), recognizeType(1),
                           blackMax(60), calcWhite(false) {}
};

// Complete detection configuration for a site/theme
struct SiteDetectionConfig {
    BorderConfig border;
    PieceConfig piece;
    RecognitionParams recognition;
    
    // Adaptive color sampling state
    RGB lightSquareColor;        // Detected light square color
    RGB darkSquareColor;         // Detected dark square color
    bool colorsCalibrated;       // Whether colors have been calibrated
    
    SiteDetectionConfig() : colorsCalibrated(false) {}
    
    // Calculate adaptive thresholds based on detected square colors
    void CalibrateThresholds() {
        if (!colorsCalibrated) {
            return;
        }
        
        // Calculate average brightness of light and dark squares
        HSV lightHSV = RGBtoHSV(lightSquareColor);
        HSV darkHSV = RGBtoHSV(darkSquareColor);
        
        // Set thresholds based on square colors
        double midBrightness = (lightHSV.v + darkHSV.v) / 2.0;
        
        // White pieces should be brighter than light squares
        piece.whiteThreshold = (int)((lightHSV.v + 0.1) * 255);
        if (piece.whiteThreshold > 255) piece.whiteThreshold = 255;
        
        // Black pieces should be darker than dark squares
        piece.blackThreshold = (int)((darkHSV.v - 0.1) * 255);
        if (piece.blackThreshold < 0) piece.blackThreshold = 0;
        
        piece.useAdaptive = true;
    }
};

// Default configurations for different sites
inline SiteDetectionConfig GetDefaultConfig(int programType) {
    SiteDetectionConfig config;
    
    switch (programType) {
        case 0: // chessbase
            config.border.color = 0;
            config.border.useExactMatch = true;
            config.piece.whiteThreshold = 190;
            config.piece.blackThreshold = 100;
            config.recognition.recognizeType = 2;
            config.recognition.blackMax = 180;
            config.recognition.calcWhite = true;
            break;
            
        case 1: // chessassistant
            config.border.color = (113<<16) + (111<<8) + 100;
            config.border.colorThreshold = 20.0;
            config.piece.whiteThreshold = 220;
            config.piece.blackThreshold = 50;
            config.recognition.recognizeType = 1;
            break;
            
        case 2: // bereg
            config.border.color = 0;
            config.border.colorThreshold = 20.0;
            config.piece.whiteThreshold = 240;
            config.piece.blackThreshold = 50;
            config.recognition.recognizeType = 1;
            break;
            
        case 3: // instantchess
            config.border.color = 0;
            config.border.colorThreshold = 20.0;
            config.piece.whiteThreshold = 250;
            config.piece.blackThreshold = 50;
            config.recognition.recognizeType = 1;
            config.recognition.blackMax = 60;
            break;
            
        case 4: // kurnik
            config.border.color = 0;
            config.border.colorThreshold = 20.0;
            config.piece.whiteThreshold = 250;
            config.piece.blackThreshold = 50;
            config.recognition.recognizeType = 1;
            break;
            
        case 5: // winboard
            config.border.color = 0;
            config.border.colorThreshold = 20.0;
            config.piece.whiteThreshold = 190;
            config.piece.blackThreshold = 50;
            config.recognition.recognizeType = 1;
            break;
            
        case 6: // chessgate
            config.border.color = (155<<16) + (155<<8) + 155;
            config.border.colorThreshold = 20.0;
            config.piece.whiteThreshold = 254;
            config.piece.blackThreshold = 50;
            config.recognition.recognizeType = 2;
            config.recognition.calcWhite = false;
            config.recognition.blackMax = 60;
            break;
            
        case 7: // spinchat
            config.border.color = (255<<16) + (234<<8) + 157;
            config.border.colorThreshold = 30.0;
            config.piece.whiteThreshold = 190;
            config.piece.blackThreshold = 50;
            config.recognition.recognizeType = 1;
            break;
            
        case 8: // chessclub_dasher
            config.border.color = (58<<16) + (36<<8) + 20;
            config.border.colorThreshold = 5.0;
            config.piece.whiteThreshold = 220;
            config.piece.blackThreshold = 50;
            config.recognition.recognizeType = 1;
            break;
    }
    
    return config;
}

#endif
