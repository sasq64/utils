#ifndef GRAPPIX_BITMAP_H
#define GRAPPIX_BITMAP_H

#include <coreutils/log.h>
#include <coreutils/format.h>

#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

template <typename T = uint32_t> class basic_bitmap {
public:
	basic_bitmap() : w(0), h(0) {}

	basic_bitmap(basic_bitmap &&b) : pixels(std::move(b.pixels)), w(b.w), h(b.h) {}

	basic_bitmap& operator=(const basic_bitmap &b) {
		return *this;
	}

	basic_bitmap& operator=(const basic_bitmap &&b) {
		pixels = std::move(b.pixels);
		w = b.w;
		h = b.h;
		return *this;
	}

	basic_bitmap(int width, int height) : w(width), h(height) {
		pixels.resize(width * height);
	}
	T& operator[](const int &i) { 
		return pixels[i];
	}

	T operator[](const int &i) const { return pixels[i]; }

	basic_bitmap operator+(T &pixel) {
		basic_bitmap rb = *this;
		for(T &p : rb.pixels)
			p += pixel;
		return rb;
	}

	void clear(uint32_t color = 0) {
		if(!color)
			memset(&pixels[0], 0, w*h*sizeof(T));
		else {
			for(int i=0; i<w*h; i++)
				pixels[i] = color;
		}
	}

	basic_bitmap cut(int x, int y, int ww, int hh) const {
		basic_bitmap dest(ww, hh);
		for(int yy=0; yy<hh; yy++)
			for(int xx=0; xx<ww; xx++) {
				dest[xx+yy*ww] = pixels[xx+x+(yy+y)*this->w];
			}
		return dest;
	}

	std::vector<basic_bitmap> split(int w, int h) {
		std::vector<basic_bitmap> rv;
		return rv;
	}

	const T* data() const { return &pixels[0]; }

	const T* flipped() const {

		int l = sizeof(T) * w;
		flipPixels.resize(w*h);
		for(int y=0; y<h; y++)
			std::memcpy(&flipPixels[y*w], &pixels[(h-y-1)*w], l);
		return &flipPixels[0];
	}


	int width() const { return w; }
	int height() const { return h; }
	int size() const { return w*h; }
private:
	std::vector<T> pixels;
	mutable std::vector<T> flipPixels;
	bool dirty;
	int w;
	int h;
};

typedef basic_bitmap<uint32_t> bitmap;

bitmap read_png_file(const std::string &file_name);


#endif // GRAPPIX_BITMAP_H