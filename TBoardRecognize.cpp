//---------------------------------------------------------------------------
#include "TBoardRecognize.h"
#include "find_pos.h"

#define X_COORD(x)  ((x)&7)
#define Y_COORD(y)  ((y)>>3)

TBoardRecognize::TBoardRecognize()
{
}

TBoardRecognize::~TBoardRecognize()
{
}

void TBoardRecognize::ShowSquare(int sq)
{
  int sq64 = SQUARE_TO_64(sq);
  int size = BoardCapture.BoardSize/8;
  int delta = size*BoardCapture.BitmapSizeX;
  int corner = 1/16;
  int *start = BoardCapture.StartPixel + Y_COORD(sq64)*delta + X_COORD(sq64)*size +
                  BoardCapture.BitmapSizeX*corner + corner;
  int real_size = size - 2*corner;
  SquareViewForm->gSquarePixels->ColCount = real_size;
  SquareViewForm->gSquarePixels->RowCount = real_size;
  for (int i=0; i<real_size; i++) {
    int *cur = start;
    for (int j=0; j<real_size; j++) {
      if (IsPixelBlack(*cur))
        SquareViewForm->PaintSquare(i,j,clBlack);
      else {
        if (IsPixelWhite(*cur))
          SquareViewForm->PaintSquare(i,j,clWhite);
        else
          SquareViewForm->PaintSquare(i,j,clBlue);
      }
      if (IsPixelMark(*cur))
        SquareViewForm->PaintSquare(i,j,clRed);
      cur++;
    }
    start += BoardCapture.BitmapSizeX;
  }
}

bool TBoardRecognize::IsPixelBlackAdaptive(int x)
{
  // Use adaptive threshold based on detected square colors
  SiteDetectionConfig& config = BoardCapture.DetectionConfig;
  
  if (config.piece.useAdaptive && config.colorsCalibrated) {
    // Use contrast-based detection relative to dark square color
    RGB pixelColor = UnpackColor(x);
    HSV pixelHSV = RGBtoHSV(pixelColor);
    HSV darkSquareHSV = RGBtoHSV(config.darkSquareColor);
    
    // Black piece should be significantly darker than dark square
    return pixelHSV.v < (darkSquareHSV.v - 0.2);
  }
  
  // Fallback to configured threshold
  RGB pixelColor = UnpackColor(x);
  return pixelColor.r < config.piece.blackThreshold && 
         pixelColor.g < config.piece.blackThreshold && 
         pixelColor.b < config.piece.blackThreshold;
}

bool TBoardRecognize::IsPixelBlack(int x)
{
  int r = x & 255;
  int g = (x>>8) & 255;
  int b = (x>>16) & 255;
  switch (ProgramType) {
     case chessbase:
        return r < 100 && g < 100 && b < 100;
     case bereg: case instantchess: case kurnik:
     case winboard: case chessassistant: case chessgate: case spinchat:
     case chessclub_dasher:
        return r < 50 && g < 50 && b < 50;
  }
  return false;
}



bool TBoardRecognize::IsPixelWhiteAdaptive(int x)
{
  // Use adaptive threshold based on detected square colors
  SiteDetectionConfig& config = BoardCapture.DetectionConfig;
  
  if (config.piece.useAdaptive && config.colorsCalibrated) {
    // Use contrast-based detection relative to light square color
    RGB pixelColor = UnpackColor(x);
    HSV pixelHSV = RGBtoHSV(pixelColor);
    HSV lightSquareHSV = RGBtoHSV(config.lightSquareColor);
    
    // White piece should be significantly brighter than light square
    return pixelHSV.v > (lightSquareHSV.v + 0.15);
  }
  
  // Fallback to configured threshold
  RGB pixelColor = UnpackColor(x);
  return pixelColor.r > config.piece.whiteThreshold && 
         pixelColor.g > config.piece.whiteThreshold && 
         pixelColor.b > config.piece.whiteThreshold;
}

bool TBoardRecognize::IsPixelWhite(int x)
{
  int r = x & 255;
  int g = (x>>8) & 255;
  int b = (x>>16) & 255;
  switch (ProgramType) {
     case chessassistant:
        return r > 220 && g >220 && b > 220;
     case chessbase:
        return r > 190 && g >190 && b > 190;
     case bereg:
        return r > 240 && g >240 && b > 240;
     case instantchess:
        return r > 250 && g >250 && b > 250;
     case kurnik:
        return r > 250 && g >250 && b > 250;
     case winboard:
        return r > 190 && g >190 && b > 190;
     case chessgate:
        return r > 254 && g >254 && b > 254;
     case spinchat:
        return r > 190 && g >190 && b > 190;
     case chessclub_dasher:
        return r > 220 && g >220 && b > 220;
  }
  return false;
}

bool TBoardRecognize::IsPixelMark(int x)
{
  int b = x & 255;
  int g = (x>>8) & 255;
  int r = (x>>16) & 255;
  switch (ProgramType) {
     case instantchess:
        return r == 0 && g == 102 && b == 153;
  }
  return false;
}

void TBoardRecognize::Recognize()
{
  BoardCapture.CaptureBoard();
  if (BoardCapture.Captured) {
    FindPos.Init();
    ConvertBoardToFindPos();
  }
}

void TBoardRecognize::ShowPosition()
{
  if (BoardCapture.Captured) {
     for (int y=0; y<8; y++)
       for (int x=0; x<8; x++) {
          int sq = y*8+x;
          if (sq == 22) {
            int k; k++;
          }
          if (SquareIsMarked(y*8+x)) {
            BoardScanerWindow->PaintScreenSquare(x,y,clRed);
            continue;
          }
          if (FindPos.GetBit(White,y*8+x)) {
            BoardScanerWindow->PaintScreenSquare(x,y,clWhite);
            continue;
          }
          if (FindPos.GetBit(Black,y*8+x)) {
            BoardScanerWindow->PaintScreenSquare(x,y,clBlack);
            continue;
          }
          BoardScanerWindow->PaintScreenSquare(x,y,clBlue);
       }
  }
  else {
     for (int y=0; y<8; y++)
       for (int x=0; x<8; x++)
          BoardScanerWindow->PaintScreenSquare(x,y,clBlue);
  }
}

void TBoardRecognize::ConvertBoardToFindPos()
{
   int size = BoardCapture.BoardSize/8;
   int delta = size*BoardCapture.BitmapSizeX;
   int corner = size/8;
   int depth;
   int width;
   int recognize_type; //0 by pixels, 1 by square
   int black_max;
   bool calc_white = false;
   
   // Load from configuration first
   RecognitionParams& params = BoardCapture.DetectionConfig.recognition;
   
   switch (ProgramType) {
      case chessassistant:
         width = size/4;
         depth = 2;
         corner = size/16;
         recognize_type = 1;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         break;
      case chessbase:
         width = size/4;
         depth = 3;
         corner = size/16;
         recognize_type = 2;
         black_max = 180;
         calc_white = true;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         params.blackMax = black_max;
         params.calcWhite = calc_white;
         break;
      case bereg:
         width = size/8;
         depth = 4;
         corner = size/16;
         recognize_type = 1;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         break;
      case instantchess:
         width = 7;
         depth = 4;
         corner = size/8;
         recognize_type = 1;
         black_max = 60;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         params.blackMax = black_max;
         break;
      case kurnik:
         width = size/8;
         depth = width;
         corner = size/16;
         recognize_type = 1;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         break;
      case winboard:
         width = size/4;
         depth = 3;
         corner = size/16;
         recognize_type = 1;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         break;
      case chessgate:
         width = size/4;
         depth = 3;
         corner = size/16;
         recognize_type = 2;
         calc_white = false;
         black_max = 60;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         params.calcWhite = calc_white;
         params.blackMax = black_max;
         break;
      case spinchat:
         width = size/6;
         depth = 2;
         corner = size/16;
         recognize_type = 1;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         break;
      case chessclub_dasher:
         width = size/6;
         depth = 3;
         corner = size/16;
         recognize_type = 1;
         params.width = width;
         params.depth = depth;
         params.corner = corner;
         params.recognizeType = recognize_type;
         break;
   }
   FindPos.Init();
   for (int i=0; i<64; i++) {
      int *start = BoardCapture.StartPixel + Y_COORD(i)*delta + X_COORD(i)*size +
                  BoardCapture.BitmapSizeX*corner + corner;

      int real_size = size - 2*corner;
      switch (recognize_type) {
        case 1:
          do {
            int *cur = start;
            bool white_finded = false;
            for (int j=0; j<real_size-depth+1; j++) {
              if (CheckForWhite(cur,real_size,width,depth)) {
                FindPos.SetBit(White,i);
                white_finded = true;
                break;
              }
              cur += BoardCapture.BitmapSizeX;
            }
            if (!white_finded) {
              cur = start;
              for (int j=0; j<real_size-depth-1; j++) {
                if (CheckForBlack(cur,real_size,width,depth)) {
                  FindPos.SetBit(Black,i);
                  break;
                }
                cur += BoardCapture.BitmapSizeX;
              }
            }
          }
          while (false);
          break;
        case 2:
          do {
            int *temp_start = start;
            int black_cnt = 0;
            for (int y=0; y<real_size; y++) {
              int *cur = temp_start;
              for (int x=0; x<real_size; x++) {
                if (IsPixelBlack(*cur) || (calc_white && IsPixelWhite(*cur)))
                  black_cnt++;
                cur++;
              }
              temp_start += BoardCapture.BitmapSizeX;
            }
            if (black_cnt > black_max) {
              int *cur = start;
              int black_finded = false;
              for (int j=0; j<real_size-depth-1; j++) {
                if (CheckForBlack(cur,real_size,width,depth)) {
                  FindPos.SetBit(Black,i);
                  black_finded = true;
                  break;
                }
                cur += BoardCapture.BitmapSizeX;
              }
              if (!black_finded)
                FindPos.SetBit(White,i);
            }
          }
          while (false);
          break;
        case 3:
          break;
      }
   }
}

bool TBoardRecognize::CheckForWhite(int *cur, int size, int width, int depth)
{
  if (depth <= 0)
    return true;
  int phase = 0;
  int *start_ptr;
  for (int i=0; i<size; i++) {
    if (cur < BoardCapture.StartPixel || cur > BoardCapture.StartPixel + 1280*1024) {
      int k; k++;
    }
    switch (phase) {
      case 0:
         if (IsPixelWhite(*cur)) {
           start_ptr = cur;
           phase = 1;
         }
         break;
      case 1:
         if (!IsPixelWhite(*cur)) {
           if(cur-start_ptr>width) {
             if (CheckForWhite(start_ptr + BoardCapture.BitmapSizeX, cur-start_ptr, width, depth - 1))
               return true;
           }
           phase = 0;
         }
         break;
    }
    cur++;
  }

  if (phase == 1 && cur-start_ptr > width)
    if (CheckForWhite(start_ptr + BoardCapture.BitmapSizeX,cur-start_ptr, width, depth - 1))
      return true;

  return false;
}

bool TBoardRecognize::CheckForBlack(int *cur, int size, int width, int depth)
{
  if (depth <= 0)
    return true;
  int phase = 0;
  int *start_ptr;
  for (int i=0; i<size; i++) {
    switch (phase) {
      case 0:
         if (IsPixelBlack(*cur)) {
           start_ptr = cur;
           phase = 1;
         }
         break;
      case 1:
         if (!IsPixelBlack(*cur)) {
           if(cur-start_ptr>width) {
             if (CheckForBlack(start_ptr + BoardCapture.BitmapSizeX, cur-start_ptr, width, depth - 1))
               return true;
           }
           phase = 0;
         }
         break;
    }
    cur++;
  }

  if (phase == 1 && cur-start_ptr > width)
    if (CheckForBlack(start_ptr + BoardCapture.BitmapSizeX,cur-start_ptr, width, depth - 1))
      return true;

  return false;
}

bool TBoardRecognize::SquareIsMarked(int sq)
{
  int size = BoardCapture.BoardSize/8;
  int delta = size*BoardCapture.BitmapSizeX;
  int *start = BoardCapture.StartPixel + Y_COORD(sq)*delta + X_COORD(sq)*size + BoardCapture.BoardSize/16;
  for (int i=0; i<4; i++) {
    if (IsPixelMark(*start))
      return true;
    start += BoardCapture.BitmapSizeX;
  }
  return false;
}

void TBoardRecognize::CalibrateSquareColors()
{
  // Auto-calibrate square colors from captured board
  // Sample empty squares to determine light and dark square colors
  // Use squares that are typically empty in starting position (d4, e5)
  
  if (!BoardCapture.Captured) {
    return;
  }
  
  int size = BoardCapture.BoardSize / 8;
  int delta = size * BoardCapture.BitmapSizeX;
  int corner = size / 8;
  int sampleSize = size / 4;
  
  // Sample empty squares from the center of the board
  // d4 square (index 27) - light square in standard chess board
  // d5 square (index 35) - dark square in standard chess board
  int lightSquareIdx = 27; // d4
  int darkSquareIdx = 35;  // d5
  
  // Sample light square (d4)
  int *lightStart = BoardCapture.StartPixel + 
                    Y_COORD(lightSquareIdx) * delta + 
                    X_COORD(lightSquareIdx) * size +
                    BoardCapture.BitmapSizeX * corner + corner;
  
  // Sample dark square (d5)
  int *darkStart = BoardCapture.StartPixel + 
                   Y_COORD(darkSquareIdx) * delta + 
                   X_COORD(darkSquareIdx) * size +
                   BoardCapture.BitmapSizeX * corner + corner;
  
  // Thresholds for filtering out piece pixels (too bright or too dark)
  const int MIN_SQUARE_BRIGHTNESS = 20;   // Exclude very dark (black pieces)
  const int MAX_SQUARE_BRIGHTNESS = 240;  // Exclude very bright (white pieces)
  const int MIN_LIGHT_BRIGHTNESS = 100;   // Light squares should be reasonably bright
  const int MAX_DARK_BRIGHTNESS = 200;    // Dark squares should not be too bright
  
  // Average colors from center of each square
  long lightR = 0, lightG = 0, lightB = 0;
  long darkR = 0, darkG = 0, darkB = 0;
  int lightSampleCount = 0;
  int darkSampleCount = 0;
  
  for (int y = 0; y < sampleSize; y++) {
    int *lightCur = lightStart + y * BoardCapture.BitmapSizeX;
    int *darkCur = darkStart + y * BoardCapture.BitmapSizeX;
    
    for (int x = 0; x < sampleSize; x++) {
      RGB lightRGB = UnpackColor(*lightCur);
      RGB darkRGB = UnpackColor(*darkCur);
      
      // Calculate average brightness across all channels
      int lightBrightness = (lightRGB.r + lightRGB.g + lightRGB.b) / 3;
      int darkBrightness = (darkRGB.r + darkRGB.g + darkRGB.b) / 3;
      
      // Skip pixels that are clearly pieces (very light or very dark)
      // Use overall brightness instead of just red channel
      if (lightBrightness > MIN_LIGHT_BRIGHTNESS && lightBrightness < MAX_SQUARE_BRIGHTNESS) {
        lightR += lightRGB.r;
        lightG += lightRGB.g;
        lightB += lightRGB.b;
        lightSampleCount++;
      }
      
      if (darkBrightness > MIN_SQUARE_BRIGHTNESS && darkBrightness < MAX_DARK_BRIGHTNESS) {
        darkR += darkRGB.r;
        darkG += darkRGB.g;
        darkB += darkRGB.b;
        darkSampleCount++;
      }
      
      lightCur++;
      darkCur++;
    }
  }
  
  // Require minimum sample count for reliable calibration
  const int MIN_SAMPLES_REQUIRED = 10; // Need at least 10 valid pixels per square
  
  if (lightSampleCount >= MIN_SAMPLES_REQUIRED && darkSampleCount >= MIN_SAMPLES_REQUIRED) {
    BoardCapture.DetectionConfig.lightSquareColor.r = (int)(lightR / lightSampleCount);
    BoardCapture.DetectionConfig.lightSquareColor.g = (int)(lightG / lightSampleCount);
    BoardCapture.DetectionConfig.lightSquareColor.b = (int)(lightB / lightSampleCount);
    
    BoardCapture.DetectionConfig.darkSquareColor.r = (int)(darkR / darkSampleCount);
    BoardCapture.DetectionConfig.darkSquareColor.g = (int)(darkG / darkSampleCount);
    BoardCapture.DetectionConfig.darkSquareColor.b = (int)(darkB / darkSampleCount);
    
    BoardCapture.DetectionConfig.colorsCalibrated = true;
    BoardCapture.DetectionConfig.CalibrateThresholds();
  }
}

