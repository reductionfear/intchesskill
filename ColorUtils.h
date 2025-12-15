//---------------------------------------------------------------------------
#ifndef ColorUtilsH
#define ColorUtilsH

#include <cmath>

//---------------------------------------------------------------------------
// Color utility functions for robust color matching and detection
//---------------------------------------------------------------------------

// Structure to hold RGB color values
struct RGB {
    int r, g, b;
    RGB() : r(0), g(0), b(0) {}
    RGB(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

// Structure to hold HSV color values
struct HSV {
    double h, s, v;
    HSV() : h(0), s(0), v(0) {}
    HSV(double hue, double saturation, double value) : h(hue), s(saturation), v(value) {}
};

// Extract RGB components from packed color integer
inline RGB UnpackColor(int color) {
    RGB result;
    result.r = color & 255;
    result.g = (color >> 8) & 255;
    result.b = (color >> 16) & 255;
    return result;
}

// Pack RGB components into color integer
inline int PackColor(const RGB& rgb) {
    return rgb.r | (rgb.g << 8) | (rgb.b << 16);
}

// Calculate Euclidean distance between two colors in RGB space
inline double ColorDistance(int color1, int color2) {
    RGB c1 = UnpackColor(color1);
    RGB c2 = UnpackColor(color2);
    
    int dr = c1.r - c2.r;
    int dg = c1.g - c2.g;
    int db = c1.b - c2.b;
    
    return sqrt((double)(dr*dr + dg*dg + db*db));
}

// Calculate color distance with RGB components
inline double ColorDistanceRGB(const RGB& c1, const RGB& c2) {
    int dr = c1.r - c2.r;
    int dg = c1.g - c2.g;
    int db = c1.b - c2.b;
    
    return sqrt((double)(dr*dr + dg*dg + db*db));
}

// Convert RGB to HSV color space
inline HSV RGBtoHSV(const RGB& rgb) {
    HSV hsv;
    double r = rgb.r / 255.0;
    double g = rgb.g / 255.0;
    double b = rgb.b / 255.0;
    
    double max = r;
    if (g > max) max = g;
    if (b > max) max = b;
    
    double min = r;
    if (g < min) min = g;
    if (b < min) min = b;
    
    double delta = max - min;
    
    // Value
    hsv.v = max;
    
    // Saturation
    if (max > 0.0) {
        hsv.s = delta / max;
    } else {
        hsv.s = 0.0;
    }
    
    // Hue
    if (delta > 0.0) {
        if (max == r) {
            hsv.h = 60.0 * fmod((g - b) / delta, 6.0);
        } else if (max == g) {
            hsv.h = 60.0 * ((b - r) / delta + 2.0);
        } else {
            hsv.h = 60.0 * ((r - g) / delta + 4.0);
        }
        if (hsv.h < 0.0) {
            hsv.h += 360.0;
        }
    } else {
        hsv.h = 0.0;
    }
    
    return hsv;
}

// Convert HSV to RGB color space
inline RGB HSVtoRGB(const HSV& hsv) {
    RGB rgb;
    
    double c = hsv.v * hsv.s;
    double x = c * (1.0 - fabs(fmod(hsv.h / 60.0, 2.0) - 1.0));
    double m = hsv.v - c;
    
    double r, g, b;
    
    if (hsv.h < 60.0) {
        r = c; g = x; b = 0;
    } else if (hsv.h < 120.0) {
        r = x; g = c; b = 0;
    } else if (hsv.h < 180.0) {
        r = 0; g = c; b = x;
    } else if (hsv.h < 240.0) {
        r = 0; g = x; b = c;
    } else if (hsv.h < 300.0) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }
    
    rgb.r = (int)((r + m) * 255.0);
    rgb.g = (int)((g + m) * 255.0);
    rgb.b = (int)((b + m) * 255.0);
    
    return rgb;
}

// Check if color is within threshold distance of target color
inline bool ColorMatches(int color, int targetColor, double threshold) {
    return ColorDistance(color, targetColor) <= threshold;
}

// Check if color is grayscale (low saturation)
inline bool IsGrayscale(int color, double saturationThreshold = 0.1) {
    RGB rgb = UnpackColor(color);
    HSV hsv = RGBtoHSV(rgb);
    return hsv.s < saturationThreshold;
}

// Get brightness/value of a color
inline double GetBrightness(int color) {
    RGB rgb = UnpackColor(color);
    HSV hsv = RGBtoHSV(rgb);
    return hsv.v;
}

// Check if color is dark (low brightness)
inline bool IsDark(int color, double brightnessThreshold = 0.3) {
    return GetBrightness(color) < brightnessThreshold;
}

// Check if color is light (high brightness)
inline bool IsLight(int color, double brightnessThreshold = 0.7) {
    return GetBrightness(color) > brightnessThreshold;
}

#endif
