#ifndef GRAPPIX_BASIC_BUFFER_H
#define GRAPPIX_BASIC_BUFFER_H

#include <vector>
#include <stdint.h>

class basic_buffer {
public:

	basic_buffer() : frameBuffer(0), _width(0), _height(0) {}
	basic_buffer(unsigned int buffer, int width, int height) : frameBuffer(buffer), _width(width), _height(height) {
	}

	template <typename T> void line(T p0, T p1, uint32_t color) {
		line(p0[0], p0[1], p1[0], p1[1], color);
	}

	void line(const std::vector<int> &p0, const std::vector<int> &p1, uint32_t color) {
		line(p0[0], p0[1], p1[0], p1[1], color);
	}
	void clear();
	void line(float x0, float y0, float x1, float y1, uint32_t color);

	template <typename T> void circle(T xy, float radius, uint32_t color) {
		circle(xy[0], xy[1], radius, color);
	}

	template <typename T> void rectangle(T pos, T size, uint32_t color) {
		rectangle(pos[0], pos[1], size[0], size[1], color);
	}

	template <typename T> void rectangle(T pos, int w, int h, uint32_t color) {
		rectangle(pos[0], pos[1], w, h, color);
	}

	void rectangle(int x, int y, int w, int h, uint32_t color);

	void circle(int x, int y, float radius, uint32_t color);
	void draw_texture(int texture, int x0, int y0, int w, int h);
	int width() { return _width; }
	int height() { return _height; }

protected:
	unsigned int frameBuffer;
	int _width;
	int _height;
};

#endif // GRAPPIX_BASIC_BUFFER_H
