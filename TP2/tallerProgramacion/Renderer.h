#pragma once
#include <thread>

class Renderer {
public:
	Renderer();
	bool iniciarRenderer();
private:
	std::thread t_draw;
	void draw();
	bool rendererActivo;
};