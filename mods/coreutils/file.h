#ifndef FILE_H
#define FILE_H

#include "utils.h"

#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>

namespace utils {

class io_exception : public std::exception {
public:
	io_exception(const char *ptr = "IO Exception") : msg(ptr) {}
	virtual const char *what() const throw() { return msg; }
private:
	const char *msg;
};

class file_not_found_exception : public std::exception {
public:
	file_not_found_exception(const std::string &fileName = "") : msg(std::string("File not found: ") + fileName) {}
	virtual const char *what() const throw() { return msg.c_str(); }
private:
	std::string msg;
};

//#define THROW(e, args...) throw e(args, __FILE__, __LINE__)

#define XSF(x) SF(x)
#define SF(x) #x
#define APP_NAME_STR XSF(APP_NAME)

class File {
public:

	enum Mode {
		NONE = 0,
		READ = 1,
		WRITE = 2
	};

	static bool exists(const std::string &fileName);
	static void copy(const std::string &from, const std::string &to);
	static std::string resolvePath(const std::string &fileName);
	static File cwd();
	static void remove(const std::string &fileName);

	static const std::string getCacheDir();
	static const std::string getConfigDir();
	static const std::string getAppDir();

	static const std::string getExeDir();

	static void setAppDir(const std::string &a);
	static const std::string getUserDir();
	static File findFile(const std::string &path, const std::string &name);
	static uint64_t getModified(const std::string &fileName);


	File();
	File(const std::string &name, const Mode mode  = NONE);
	File(const File &parent, const std::string &name, const Mode mode  = NONE);
	File(const std::string &parent, const std::string &name, const Mode mode = NONE);
	~File();

	void write(const uint8_t *data, const int size);
	void write(const std::string &text);

	template <typename T> void write(const T &t) {
		open(WRITE);
		if(fwrite(&t, sizeof(T), 1, writeFP) > 0)
			return;
		throw io_exception { "Write failed" };
	}

	void writeln(const std::string &line);

	void close();

	template <typename T> int read(T * const target, const int count) {
		open(READ);
		int rc = fread(target, sizeof(T), (size_t)count, readFP);
		if(rc < 0)
			throw io_exception { "Read failed" };
		return rc;
	}

	template <typename T> T read() {
		open(READ);
		T temp;
		if(fread(&temp, sizeof(T), 1, readFP) >= 0)
			return temp;
		throw io_exception { "Read failed" };
	}

	std::string read();

	std::vector<uint8_t> readAll();

	std::vector<std::string> getLines();

	std::vector<File> listFiles();

	bool exists() const;

	const std::string &getName() const { return fileName; }

	const std::string getDirectory() const { return path_directory(resolvePath(fileName)); }

	int64_t getSize() const;
	uint64_t getModified() const;

	bool isDir() const;

	void remove();
	void rename(const std::string &newName);

	void copyFrom(File &otherFile);
	void copyFrom(const std::string &other);

	bool isChildOf(const File &f) const;

	void seek(int where);

	static File NO_FILE;

private:

	void open(Mode mode);

	static std::string appDir;
	static std::string userDir;

	std::string fileName;
	mutable int64_t size;

	FILE *writeFP;
	FILE *readFP;
};

}

#endif // FILE_H