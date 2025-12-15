//---------------------------------------------------------------------------
#ifndef ConfigLoaderH
#define ConfigLoaderH

#include <fstream>
#include <string>
#include <map>
#include "DetectionConfig.h"

//---------------------------------------------------------------------------
// Simple configuration file loader for detection parameters
// Uses INI-style format for simplicity and C++ Builder compatibility
//---------------------------------------------------------------------------

class ConfigLoader {
private:
    std::map<std::string, std::string> configValues;
    
    std::string Trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, last - first + 1);
    }
    
    int ParseInt(const std::string& value, int defaultValue) {
        try {
            return std::stoi(value);
        } catch (...) {
            return defaultValue;
        }
    }
    
    double ParseDouble(const std::string& value, double defaultValue) {
        try {
            return std::stod(value);
        } catch (...) {
            return defaultValue;
        }
    }
    
    bool ParseBool(const std::string& value, bool defaultValue) {
        std::string lower = value;
        for (char& c : lower) c = tolower(c);
        if (lower == "true" || lower == "1" || lower == "yes") return true;
        if (lower == "false" || lower == "0" || lower == "no") return false;
        return defaultValue;
    }
    
public:
    // Load configuration from INI-style file
    bool LoadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        configValues.clear();
        std::string line;
        std::string currentSection = "";
        
        while (std::getline(file, line)) {
            line = Trim(line);
            
            // Skip empty lines and comments
            if (line.empty() || line[0] == ';' || line[0] == '#') {
                continue;
            }
            
            // Section header
            if (line[0] == '[' && line[line.length()-1] == ']') {
                currentSection = line.substr(1, line.length()-2);
                continue;
            }
            
            // Key=Value pair
            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = Trim(line.substr(0, equalPos));
                std::string value = Trim(line.substr(equalPos + 1));
                
                if (!currentSection.empty()) {
                    key = currentSection + "." + key;
                }
                
                configValues[key] = value;
            }
        }
        
        file.close();
        return true;
    }
    
    // Get configuration value with default
    std::string GetString(const std::string& key, const std::string& defaultValue = "") {
        auto it = configValues.find(key);
        if (it != configValues.end()) {
            return it->second;
        }
        return defaultValue;
    }
    
    int GetInt(const std::string& key, int defaultValue = 0) {
        return ParseInt(GetString(key), defaultValue);
    }
    
    double GetDouble(const std::string& key, double defaultValue = 0.0) {
        return ParseDouble(GetString(key), defaultValue);
    }
    
    bool GetBool(const std::string& key, bool defaultValue = false) {
        return ParseBool(GetString(key), defaultValue);
    }
    
    // Parse RGB color from "R,G,B" format
    RGB GetColor(const std::string& key, const RGB& defaultValue = RGB()) {
        std::string value = GetString(key);
        if (value.empty()) return defaultValue;
        
        RGB result;
        size_t pos1 = value.find(',');
        if (pos1 == std::string::npos) return defaultValue;
        
        size_t pos2 = value.find(',', pos1 + 1);
        if (pos2 == std::string::npos) return defaultValue;
        
        try {
            result.r = std::stoi(Trim(value.substr(0, pos1)));
            result.g = std::stoi(Trim(value.substr(pos1 + 1, pos2 - pos1 - 1)));
            result.b = std::stoi(Trim(value.substr(pos2 + 1)));
        } catch (...) {
            return defaultValue;
        }
        
        return result;
    }
    
    // Load site detection config from file
    bool LoadSiteConfig(const std::string& filename, int programType, SiteDetectionConfig& config) {
        if (!LoadFromFile(filename)) {
            return false;
        }
        
        // Determine section name based on program type
        std::string section = "site" + std::to_string(programType);
        
        // Load border configuration
        config.border.color = PackColor(GetColor(section + ".border.color", RGB(0,0,0)));
        config.border.colorThreshold = GetDouble(section + ".border.threshold", 30.0);
        config.border.useExactMatch = GetBool(section + ".border.exact", false);
        
        // Load piece configuration
        config.piece.whiteThreshold = GetInt(section + ".piece.white", 190);
        config.piece.blackThreshold = GetInt(section + ".piece.black", 50);
        config.piece.useAdaptive = GetBool(section + ".piece.adaptive", false);
        config.piece.contrastRatio = GetDouble(section + ".piece.contrast", 1.5);
        
        // Load recognition parameters
        config.recognition.recognizeType = GetInt(section + ".recognition.type", 1);
        config.recognition.blackMax = GetInt(section + ".recognition.blackmax", 60);
        config.recognition.calcWhite = GetBool(section + ".recognition.calcwhite", false);
        
        return true;
    }
    
    // Save current configuration to file
    bool SaveToFile(const std::string& filename, const std::map<std::string, std::string>& values) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        std::string currentSection = "";
        for (const auto& pair : values) {
            // Extract section from key
            size_t dotPos = pair.first.find('.');
            if (dotPos != std::string::npos) {
                std::string section = pair.first.substr(0, dotPos);
                if (section != currentSection) {
                    if (!currentSection.empty()) {
                        file << std::endl;
                    }
                    file << "[" << section << "]" << std::endl;
                    currentSection = section;
                }
                std::string key = pair.first.substr(dotPos + 1);
                file << key << " = " << pair.second << std::endl;
            } else {
                file << pair.first << " = " << pair.second << std::endl;
            }
        }
        
        file.close();
        return true;
    }
};

#endif
