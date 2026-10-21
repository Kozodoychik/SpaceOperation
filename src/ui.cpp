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