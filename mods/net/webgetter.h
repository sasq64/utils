
#include <string>
#include <functional>
#include <vector>
#include <coreutils/thread.h>
#include <atomic>

namespace utils {
	class File;
}

namespace net {

class HttpSession {
public:

	typedef std::function<void(HttpSession&, const std::vector<uint8_t>&)> Callback;

	struct State;

	HttpSession(const std::string &url);
	//HttpSession(const std::string &host, const std::string &path, int port);
	HttpSession(HttpSession &&h);
	HttpSession& operator=(HttpSession &&h);
	~HttpSession();

	void connect();

	void stream(const Callback &cb);
	void getData(const Callback &cb);

	Callback getCallback() const;
	std::vector<uint8_t> getContent();
	int contentSize() const;
	int returnCode() const;
	void update();
	bool gotHeaders() const;
	bool done() const;
	bool error() const;

	std::string getHeader(const std::string &what) const;
	std::string getUrl() const;
	int getPort() const;
	bool isRedirect() const;

	std::string getHost() const;

	void stop();

	static std::atomic<int> ongoingCalls;

	enum NetState { NONE, ERROR, CONNECT, HEADERS, DONE, EMPTY };
	NetState getState();

private:



	void sendRequest(const std::string &path, const std::string &host);
	void parseHeader();
	void readContent();

	// NOTE: Should be unique if it worked with PIMPL
	std::shared_ptr<State> impl;

};

class WebGetter {
public:
	WebGetter(const std::string &cacheDir = "");
	~WebGetter();

	std::shared_ptr<HttpSession> getFile(const std::string &url, std::function<void(const utils::File&)> callback);
	std::shared_ptr<HttpSession> streamData(const std::string &url, std::function<bool(const uint8_t* data, int size)> callback);
	//static void getFile(const std::string &url, std::function<void(const utils::File&)> callback);
	void getData(const std::string &url, std::function<void(const std::vector<uint8_t> &data)>);

	void update();

	static int inProgress();
	bool inCache(const std::string &name) const;

	void setErrorCallback(std::function<void(int, const std::string&)>);

private:
	const std::string cacheDir;
	std::thread workerThread;
	std::atomic<bool> quit;
	std::vector<std::shared_ptr<HttpSession>> sessions;
	std::mutex m;
	std::function<void(int, const std::string&)> errorCallback;
};

}