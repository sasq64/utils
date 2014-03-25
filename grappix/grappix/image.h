#ifndef IMAGE_H
#define IMAGE_H

#include "bitmap.h"
#include <stdexcept>

namespace grappix {

class image_exception : public std::exception {
public:
	image_exception(const std::string &msg) : msg(msg) {}
	virtual const char *what() const throw() { return msg.c_str(); }
private:
	std::string msg;
};

bitmap load_png(const std::string &file_name);
void save_png(bitmap bitmap, const std::string &path);

}

#endif // IMAGE_H