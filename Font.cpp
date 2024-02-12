//#include "Font.h"
//
//Font::Font() { }
//
//Font::~Font() { }
//
//bool Font::initFont(const std::string& fontFilePath,
//    const unsigned int fontSize /*= 32*/, const unsigned int spaceSize /*= 0*/) {
//
//    const unsigned int TOTAL_FONTS = 256;
//
//    static FT_Library library;
//    FT_Error error = FT_Init_FreeType(&library);
//    if (error) {
//        printf("Error at initFont() in Font.cpp\n"
//            "Failed to initialize FreeType! FT error: %X", error);
//        return false;
//    }
//
//    unsigned int maxCellWidth = 0;
//    unsigned int maxCellHeight = 0;
//    int maxBearing = 0;
//    int maxHang = 0;
//
//    std::vector<TextureData> characterTextures;
//    characterTextures.resize(TOTAL_FONTS);
//    
//    std::vector<FT_Glyph_Metrics> characterMetrics;
//    characterMetrics.resize(TOTAL_FONTS);
//
//    FT_Face face = 0;
//    error = FT_New_Face(library, fontFilePath.c_str(), 0, &face);
//    if (error) {
//        printf("Error at initFont() in Font.cpp\n"
//            "Failed to create font face! FT error: %X", error);
//        
//        FT_Done_FreeType(library);
//        return false;
//    }
//
//    error = FT_Set_Pixel_Sizes(face, 0, fontSize);
//    if (error) {
//        printf("Error at initFont() in Font.cpp\n"
//            "Failed to set font size! FT error: %X", error);
//
//        FT_Done_Face(face);
//        FT_Done_FreeType(library);
//        return false;
//    }
//
//    for (int i = 0; i < TOTAL_FONTS; i++) {
//
//        error = FT_Load_Char(face, i, FT_LOAD_RENDER);
//        if (error) {
//            printf("Error at initFont() in Font.cpp\n"
//                "Failed to load character %c ! FT error: %X", i, error);
//
//            FT_Done_Face(face);
//            FT_Done_FreeType(library);
//            return false;
//        }
//
//        characterMetrics[i] = face->glyph->metrics;
//
//
//    }
//
//    FT_Done_FreeType(library);
//    return true;
//}