//---------------------------------------------------------------------------
#ifndef DebugOverlayH
#define DebugOverlayH

#include "ColorUtils.h"
#include "DetectionConfig.h"

//---------------------------------------------------------------------------
// Debug visualization utilities for detection confidence and issues
//---------------------------------------------------------------------------

// Detection confidence levels
enum ConfidenceLevel {
    CONFIDENCE_NONE = 0,
    CONFIDENCE_LOW = 1,
    CONFIDENCE_MEDIUM = 2,
    CONFIDENCE_HIGH = 3
};

// Square detection result with confidence
struct SquareDetection {
    bool hasWhitePiece;
    bool hasBlackPiece;
    bool isEmpty;
    ConfidenceLevel confidence;
    RGB averageColor;
    double brightness;
    int pixelCount;
    
    SquareDetection() : hasWhitePiece(false), hasBlackPiece(false), 
                        isEmpty(true), confidence(CONFIDENCE_NONE),
                        brightness(0.0), pixelCount(0) {}
};

// Calculate detection confidence based on pixel statistics
inline ConfidenceLevel CalculateDetectionConfidence(int whitePixels, int blackPixels, int totalPixels, double contrast) {
    if (totalPixels == 0) {
        return CONFIDENCE_NONE;
    }
    
    double whiteRatio = (double)whitePixels / totalPixels;
    double blackRatio = (double)blackPixels / totalPixels;
    
    // High confidence: strong signal, clear piece detection
    if ((whiteRatio > 0.3 || blackRatio > 0.3) && contrast > 2.0) {
        return CONFIDENCE_HIGH;
    }
    
    // Medium confidence: moderate signal
    if ((whiteRatio > 0.15 || blackRatio > 0.15) && contrast > 1.5) {
        return CONFIDENCE_MEDIUM;
    }
    
    // Low confidence: weak signal
    if (whiteRatio > 0.05 || blackRatio > 0.05) {
        return CONFIDENCE_LOW;
    }
    
    return CONFIDENCE_NONE;
}

// Analyze a square and return detection result with confidence
inline SquareDetection AnalyzeSquare(int* buffer, int startX, int startY, int squareSize, 
                                      int bufferWidth, const SiteDetectionConfig& config,
                                      bool (*isWhite)(int), bool (*isBlack)(int)) {
    SquareDetection result;
    
    int whiteCount = 0;
    int blackCount = 0;
    int totalCount = 0;
    long totalR = 0, totalG = 0, totalB = 0;
    
    for (int y = 0; y < squareSize; y++) {
        for (int x = 0; x < squareSize; x++) {
            int px = startX + x;
            int py = startY + y;
            int idx = py * bufferWidth + px;
            int pixel = buffer[idx];
            
            RGB rgb = UnpackColor(pixel);
            totalR += rgb.r;
            totalG += rgb.g;
            totalB += rgb.b;
            totalCount++;
            
            if (isWhite(pixel)) {
                whiteCount++;
            } else if (isBlack(pixel)) {
                blackCount++;
            }
        }
    }
    
    if (totalCount > 0) {
        result.averageColor.r = (int)(totalR / totalCount);
        result.averageColor.g = (int)(totalG / totalCount);
        result.averageColor.b = (int)(totalB / totalCount);
        
        HSV hsv = RGBtoHSV(result.averageColor);
        result.brightness = hsv.v;
        result.pixelCount = totalCount;
        
        // Calculate contrast
        double maxBrightness = 1.0;
        double minBrightness = 0.0;
        if (config.colorsCalibrated) {
            HSV lightHSV = RGBtoHSV(config.lightSquareColor);
            HSV darkHSV = RGBtoHSV(config.darkSquareColor);
            maxBrightness = lightHSV.v;
            minBrightness = darkHSV.v;
        }
        
        double contrast = (maxBrightness > 0.01) ? result.brightness / maxBrightness : 1.0;
        
        // Determine piece presence
        double whiteRatio = (double)whiteCount / totalCount;
        double blackRatio = (double)blackCount / totalCount;
        
        if (whiteRatio > 0.1) {
            result.hasWhitePiece = true;
            result.isEmpty = false;
        } else if (blackRatio > 0.1) {
            result.hasBlackPiece = true;
            result.isEmpty = false;
        }
        
        // Calculate confidence
        result.confidence = CalculateDetectionConfidence(whiteCount, blackCount, totalCount, contrast);
    }
    
    return result;
}

// Get overlay color for confidence level (for visualization)
inline RGB GetConfidenceColor(ConfidenceLevel level) {
    switch (level) {
        case CONFIDENCE_HIGH:
            return RGB(0, 255, 0);      // Green
        case CONFIDENCE_MEDIUM:
            return RGB(255, 255, 0);    // Yellow
        case CONFIDENCE_LOW:
            return RGB(255, 165, 0);    // Orange
        default:
            return RGB(255, 0, 0);      // Red
    }
}

// Generate debug report string for a detection result
inline const char* GetConfidenceString(ConfidenceLevel level) {
    switch (level) {
        case CONFIDENCE_HIGH:
            return "HIGH";
        case CONFIDENCE_MEDIUM:
            return "MEDIUM";
        case CONFIDENCE_LOW:
            return "LOW";
        default:
            return "NONE";
    }
}

// Validate entire board detection and report issues
struct BoardValidation {
    int totalSquares;
    int highConfidenceSquares;
    int mediumConfidenceSquares;
    int lowConfidenceSquares;
    int failedSquares;
    double averageConfidence;
    bool boardValid;
    
    BoardValidation() : totalSquares(0), highConfidenceSquares(0), 
                        mediumConfidenceSquares(0), lowConfidenceSquares(0),
                        failedSquares(0), averageConfidence(0.0), boardValid(false) {}
    
    void Calculate() {
        if (totalSquares > 0) {
            averageConfidence = (highConfidenceSquares * 3.0 + 
                                mediumConfidenceSquares * 2.0 + 
                                lowConfidenceSquares * 1.0) / totalSquares;
            
            // Board is valid if most squares have at least medium confidence
            boardValid = (highConfidenceSquares + mediumConfidenceSquares) >= (totalSquares * 0.7);
        }
    }
};

#endif
