#pragma once

struct Texture {
    unsigned int m_id;
    int m_width;
    int m_height;
};

namespace Textures {
    static Texture whiteTexture;
    static Texture blackTexture;
	static Texture homeBackground;
}
