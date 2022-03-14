#pragma once

struct Texture {
    unsigned int m_id;
    int m_width;
    int m_height;
};

namespace Textures {
    static Texture* whiteTexture = new Texture();
    static Texture* blackTexture = new Texture();
	static Texture* homeBackground = new Texture();
	static Texture* spaceBackground = new Texture();
	static Texture* blueRectangle = new Texture();
	static Texture* bomberboy1 = new Texture();
	static Texture* bomberboy2 = new Texture();
	static Texture* bomberboy3 = new Texture();
}
