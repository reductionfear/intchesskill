//---------------------------------------------------------------------------
#ifndef EdgeDetectionH
#define EdgeDetectionH

#include <cmath>
#include "ColorUtils.h"

//---------------------------------------------------------------------------
// Edge detection utilities for robust board border detection
// Implements simplified Sobel edge detection
//---------------------------------------------------------------------------

// Calculate gradient magnitude at a pixel using Sobel operator
inline double CalculateGradient(int* buffer, int x, int y, int width, int height) {
    if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1) {
        return 0.0;
    }
    
    // Sobel kernels for X and Y gradients
    // Gx = [-1  0  1]    Gy = [-1 -2 -1]
    //      [-2  0  2]         [ 0  0  0]
    //      [-1  0  1]         [ 1  2  1]
    
    int idx = y * width + x;
    
    // Get brightness values for 3x3 neighborhood
    double p[3][3];
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int pixel = buffer[(y + dy) * width + (x + dx)];
            RGB rgb = UnpackColor(pixel);
            // Use average of RGB as brightness
            p[dy + 1][dx + 1] = (rgb.r + rgb.g + rgb.b) / 3.0;
        }
    }
    
    // Calculate gradients
    double gx = -p[0][0] + p[0][2] 
                -2*p[1][0] + 2*p[1][2]
                -p[2][0] + p[2][2];
                
    double gy = -p[0][0] - 2*p[0][1] - p[0][2]
                +p[2][0] + 2*p[2][1] + p[2][2];
    
    // Gradient magnitude
    return sqrt(gx*gx + gy*gy);
}

// Detect if a horizontal line contains strong edges (potential board border)
inline bool DetectHorizontalEdge(int* buffer, int y, int startX, int endX, int width, int height, double threshold = 50.0) {
    if (y <= 0 || y >= height - 1) {
        return false;
    }
    
    int edgeCount = 0;
    int totalCount = 0;
    
    for (int x = startX; x < endX && x < width - 1; x++) {
        if (x <= 0) continue;
        
        double gradient = CalculateGradient(buffer, x, y, width, height);
        if (gradient > threshold) {
            edgeCount++;
        }
        totalCount++;
    }
    
    // At least 60% of pixels should have strong edges for a board border
    return totalCount > 0 && (double)edgeCount / totalCount > 0.6;
}

// Detect if a vertical line contains strong edges (potential board border)
inline bool DetectVerticalEdge(int* buffer, int x, int startY, int endY, int width, int height, double threshold = 50.0) {
    if (x <= 0 || x >= width - 1) {
        return false;
    }
    
    int edgeCount = 0;
    int totalCount = 0;
    
    for (int y = startY; y < endY && y < height - 1; y++) {
        if (y <= 0) continue;
        
        double gradient = CalculateGradient(buffer, x, y, width, height);
        if (gradient > threshold) {
            edgeCount++;
        }
        totalCount++;
    }
    
    // At least 60% of pixels should have strong edges for a board border
    return totalCount > 0 && (double)edgeCount / totalCount > 0.6;
}

// Validate that a region has a grid pattern (for 8x8 chessboard)
inline bool ValidateGridPattern(int* buffer, int x, int y, int size, int width, int height, double edgeThreshold = 40.0) {
    if (x + size >= width || y + size >= height) {
        return false;
    }
    
    int squareSize = size / 8;
    if (squareSize < 10) {
        return false; // Board too small to validate
    }
    
    int gridLineCount = 0;
    int expectedGridLines = 7; // 7 internal grid lines for 8x8 board
    
    // Check for vertical grid lines (7 internal lines for 8x8 board)
    for (int i = 1; i < 8; i++) {
        int gridX = x + i * squareSize;
        if (DetectVerticalEdge(buffer, gridX, y, y + size, width, height, edgeThreshold)) {
            gridLineCount++;
        }
    }
    
    // Check for horizontal grid lines (7 internal lines for 8x8 board)
    for (int i = 1; i < 8; i++) {
        int gridY = y + i * squareSize;
        if (DetectHorizontalEdge(buffer, gridY, x, x + size, width, height, edgeThreshold)) {
            gridLineCount++;
        }
    }
    
    // 14 total grid lines possible (7 vertical + 7 horizontal)
    // Require detecting at least 7 lines (half of total) for valid 8x8 board
    return gridLineCount >= expectedGridLines;
}

// Enhanced board detection using edge detection
inline bool DetectBoardWithEdges(int* buffer, int x, int y, int size, int width, int height) {
    // Check all four borders for strong edges
    bool topEdge = DetectHorizontalEdge(buffer, y, x, x + size, width, height);
    bool bottomEdge = DetectHorizontalEdge(buffer, y + size - 1, x, x + size, width, height);
    bool leftEdge = DetectVerticalEdge(buffer, x, y, y + size, width, height);
    bool rightEdge = DetectVerticalEdge(buffer, x + size - 1, y, y + size, width, height);
    
    // All four borders should have edges
    if (!(topEdge && bottomEdge && leftEdge && rightEdge)) {
        return false;
    }
    
    // Validate internal grid pattern
    return ValidateGridPattern(buffer, x, y, size, width, height);
}

// Calculate edge density in a region
inline double CalculateEdgeDensity(int* buffer, int x, int y, int width, int height, int regionWidth, int regionHeight, double threshold = 30.0) {
    int edgeCount = 0;
    int totalCount = 0;
    
    for (int dy = 0; dy < regionHeight; dy++) {
        for (int dx = 0; dx < regionWidth; dx++) {
            int px = x + dx;
            int py = y + dy;
            
            if (px <= 0 || px >= width - 1 || py <= 0 || py >= height - 1) {
                continue;
            }
            
            double gradient = CalculateGradient(buffer, px, py, width, height);
            if (gradient > threshold) {
                edgeCount++;
            }
            totalCount++;
        }
    }
    
    return totalCount > 0 ? (double)edgeCount / totalCount : 0.0;
}

#endif
