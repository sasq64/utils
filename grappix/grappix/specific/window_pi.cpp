#include "../window.h"
#include "../tween.h"
#include "../resources.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <stdio.h>
#include <unordered_map>
#include <functional>

#include <bcm_host.h>

using namespace std;

//bool initEGL(EGLConfig& eglConfig, EGLContext& eglContext, EGLDisplay& eglDisplay);
bool initEGL(EGLConfig& eglConfig, EGLContext& eglContext, EGLDisplay& eglDisplay, EGLSurface &eglSurface, EGLNativeWindowType nativeWin);

namespace grappix {

Window::Window() : RenderTarget(), winOpen(false), bmCounter(0), lockIt(false) {
	NO_CLICK.x = NO_CLICK.y = NO_CLICK.button = -1;
}

void Window::open(bool fs) {
	open(0,0,fs);
}

Window::click Window::NO_CLICK = { -1, -1, -1};

static function<void(uint32_t)> renderLoopFunction;

static EGL_DISPMANX_WINDOW_T nativewindow;

static EGLConfig eglConfig;
static EGLContext eglContext;
static EGLDisplay eglDisplay;
static EGLSurface eglSurface;

void Window::open(int w, int h, bool fs) {

	if(winOpen)
		return;

	bcm_host_init();

	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

	uint32_t display_width;
	uint32_t display_height;

	// create an EGL window surface, passing context width/height
	int success = graphics_get_display_size(0 /* LCD */, &display_width, &display_height);
	if(success < 0)
		throw display_exception("Cound not get display size");

	// You can hardcode the resolution here:
	//display_width = 640;
	//display_height = 480;
	LOGD("Display %dx%d", display_width, display_height);

	//display_width = 640;
	//display_height = 480;

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = display_width;
	dst_rect.height = display_height;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = display_width << 16;
	src_rect.height = display_height << 16;

	dispman_display = vc_dispmanx_display_open(0 /* LCD */);
	dispman_update = vc_dispmanx_update_start(0);

	dispman_element = vc_dispmanx_element_add(dispman_update, 
	dispman_display, 0/*layer*/, &dst_rect, 0/*src*/,
	&src_rect, DISPMANX_PROTECTION_NONE, nullptr /*alpha*/, 
	nullptr/*clamp*/, DISPMANX_NO_ROTATE);
	LOGD("Dispelement %d", dispman_element);

	nativewindow.element = dispman_element;
	nativewindow.width = display_width;
	nativewindow.height = display_height;
	vc_dispmanx_update_submit_sync(dispman_update);

	initEGL(eglConfig, eglContext, eglDisplay, eglSurface, &nativewindow);

	//eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, &nativewindow, NULL);
/*
	LOGI("Surface %p", eglSurface);

	if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
		LOGI("Unable to eglMakeCurrent");
		return;
	}
*/

	setup(display_width, display_height);

	atexit([](){
		if(!renderLoopFunction) {
			while(screen.is_open()) {
				screen.update_callbacks();
				screen.flip();
			}
		}	
	});
};


void Window::render_loop(function<void(uint32_t)> f, int fps) {
	renderLoopFunction = f;

	//atexit([](){
		auto lastMs = utils::getms();
		while(screen.is_open()) {
			screen.update_callbacks();
			auto ms = utils::getms();
			uint32_t rate = ms - lastMs;
			lastMs = ms;
			renderLoopFunction(rate);
			//while(screen.locked()) {
			//	utils::sleepms(5);
			//}
		}
	//});
}

void Window::flip() {
	if(eglDisplay != EGL_NO_DISPLAY) {
		eglSwapBuffers(eglDisplay, eglSurface);
		//eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &screenWidth);
		//eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &screenHeight);
	}
	auto t = chrono::high_resolution_clock::now();
	auto ms = chrono::duration_cast<chrono::microseconds>(t - startTime).count();
	tween::Tween::updateTweens(ms / 1000000.0f);
}

bool Window::mouse_pressed() {
	return false;
}

tuple<int, int> Window::mouse_position() {
	return make_tuple(-1,-1);
}

bool Window::key_pressed(key k) {
	return false;
}

bool Window::key_pressed(char k) {
	return false;
}

Window::click Window::get_click(bool peek) {
	return NO_CLICK;
}

Window::key Window::get_key(bool peek) {
	return NO_KEY;
};

Window screen;

}