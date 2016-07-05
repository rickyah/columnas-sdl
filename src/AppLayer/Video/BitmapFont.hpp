//
//  BitmapFont.hpp
//
//  Created by Ricardo Amores Hernández on 18/6/16.
//
//

#ifndef BitmapFont_hpp
#define BitmapFont_hpp

#include <memory>
#include <vector>

#include "Texture2d.hpp"

struct Glyph {
    int id;
    int x;
    int y;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int advance;
};

inline bool operator<(const Glyph& glyph, int id) { return glyph.id < id; }
inline bool operator<(int id, const Glyph& glyph) { return id < glyph.id; }

class BitmapFont
{
public:
    BitmapFont(Texture2d * ptrFontTexture, std::vector<Glyph> fontGlyphData)
    :pTexture(std::unique_ptr<Texture2d>(ptrFontTexture)), mGlyphsInTexture(fontGlyphData)
    {}
    
    Texture2d * const  fontTexture() const { return pTexture.get(); }

    const Glyph& FindGlyphForChar(char c) const {
        auto found = std::lower_bound(std::begin(mGlyphsInTexture), std::end(mGlyphsInTexture), c);
        
        if (found == std::end(mGlyphsInTexture) || c < *found) {
            found = std::lower_bound(std::begin(mGlyphsInTexture), std::end(mGlyphsInTexture), static_cast<int>('_'));
        }
        
        return *found;
    }
    
    Color colorTint() const
    {
        if (auto ptrTex = pTexture.get())
        {
            return ptrTex->colorTint();
        }
        
        return {255,255,255};
    }
    
    Color colorTint(Color rgbColor)
    {
        if (auto ptrTex = pTexture.get())
        {
            return ptrTex->colorTint(rgbColor);
        }
        
        return {255,255,255};
    }
    
    
private:
    
    const std::unique_ptr<Texture2d> pTexture;
    const std::vector<Glyph> mGlyphsInTexture;
    
    BitmapFont(const BitmapFont &) = delete;
    BitmapFont(BitmapFont &&) = delete;
    BitmapFont& operator=(const BitmapFont &) = delete;
    BitmapFont& operator=(BitmapFont &&) = delete;
};
#endif /* BitmapFont_hpp */
