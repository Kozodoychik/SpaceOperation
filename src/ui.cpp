#include <string>
#include <format>
#include <cstdlib>
#include "ui.hpp"
#include "spritesheet.hpp"
#include "logging.hpp"

Widget::Widget(SDL_Renderer* renderer, int x, int y) {
	this->pos = { (double)x, (double)y };
	this->size = { 0, 0 };

	this->renderer = renderer;
}

Widget::~Widget() {}

void Widget::Render(double dt) {}

Label::Label(SDL_Renderer* renderer, std::string text, int x, int y, int size) : Widget(renderer, x, y) {
	this->text = text;
	this->size = { (double)size, (double)size };

	this->font = new Spritesheet(renderer, "assets/font.png", 16, 16);
}

Label::~Label() {
	delete this->font;
}

void Label::Render(double dt) {
	for (int i = 0; i < this->text.length(); i++) {
		SDL_Rect rect = {
			(int)(this->pos.x + (i * this->size.w)), (int)this->pos.y,
			(int)this->size.w, (int)this->size.h
		};
		char c = this->text[i];
		int charX = c % 16;
		int charY = c / 16;

		this->font->CopyTileTo(charX, charY, &rect);
	}
}

ProgressBar::ProgressBar(SDL_Renderer* renderer, double max, int x, int y, int w) : Widget(renderer, x, y) {
	this->texture = new Spritesheet(renderer, "assets/progressbar.png", 1, 16);
	this->max = max;
	this->width = w;
	this->value = 0;
}

ProgressBar::~ProgressBar() {
	delete this->texture;
}

void ProgressBar::Render(double dt) {
	SDL_Rect base = {
		this->pos.x, this->pos.y,
		this->width, 16
	};

	SDL_Rect filled = {
		this->pos.x, this->pos.y,
		this->width * (this->value / this->max), 16
	};

	this->texture->CopyTileTo(1, 0, &base);
	this->texture->CopyTileTo(0, 0, &filled);
}

Image::Image(SDL_Renderer* renderer, const char* path, int x, int y, int w, int h) : Widget(renderer, x, y) {
	this->texture = new Spritesheet(renderer, path, w, h);
	this->size.w = w;
	this->size.h = h;
}

Image::~Image() {
	delete this->texture;
}

void Image::Render(double dt) {
	SDL_Rect rect = {
		this->pos.x, this->pos.y,
		this->size.w, this->size.h
	};

	this->texture->CopyTileTo(0, 0, &rect);
}

UIRenderer::UIRenderer(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

UIRenderer::~UIRenderer() {
	for (Widget* widget : this->widgets) {
		delete widget;
	}
}

void UIRenderer::Render(double dt) {
	for (Widget* widget : this->to_destroy) {
		auto it = std::find(this->widgets.begin(), this->widgets.end(), widget);

		if (it != this->widgets.end()) {
			this->widgets.erase(it);
			delete widget;
		}
	}
	this->to_destroy.clear();

	std::size_t count = this->widgets.size();

	for (int i = 0; i < count; i++) {
		this->widgets[i]->Render(dt);
	}
}

void UIRenderer::AddWidget(Widget* widget) {
	this->widgets.push_back(widget);
}

void UIRenderer::DestroyWidget(Widget* widget) {
	auto it = this->widgets.begin();

	it = std::find(this->widgets.begin(), this->widgets.end(), widget);

	if (it != this->widgets.end()) {
		this->to_destroy.push_back(widget);
	}
}