#ifndef UI_HPP
#define UI_HPP

#include <SDL.h>
#include <vector>
#include <string>
#include "types.hpp"
#include "spritesheet.hpp"


class Widget {
protected:
	SDL_Renderer* renderer;

public:
	types::Vector2d pos;
	types::Vector2d size;

	Widget(SDL_Renderer* renderer, int x, int y);
	~Widget();

	virtual void Render(double dt);
};

class Label : public Widget {
private:
	Spritesheet* font;

public:
	std::string text;

	Label(SDL_Renderer* renderer, std::string text, int x, int y, int size);
	~Label();

	void Render(double dt) override;
};

class ProgressBar : public Widget {
private:
	Spritesheet* texture;
	int width;

public:
	double value;
	double max;

	ProgressBar(SDL_Renderer* renderer, double max, int x, int y, int w);
	~ProgressBar();

	void Render(double dt) override;
};

class Image : public Widget {
private:
	Spritesheet* texture;

public:
	Image(SDL_Renderer* renderer, const char* path, int x, int y, int w, int h);
	~Image();

	void Render(double dt) override;
};

class UIRenderer {
private:
	SDL_Renderer* renderer;

	std::vector<Widget*> widgets;
	std::vector<Widget*> to_destroy;

public:

	UIRenderer(SDL_Renderer* renderer);
	~UIRenderer();

	void Render(double dt);
	void AddWidget(Widget* widget);
	void DestroyWidget(Widget* widget);
};

#endif