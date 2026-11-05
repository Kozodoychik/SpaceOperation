#include <SDL.h>
#include <format>
#include "menu.hpp"
#include "ui.hpp"
#include "events.hpp"
#include "game.hpp"
#include "logging.hpp"


MenuManager::MenuManager(SDL_Renderer* renderer) {

	this->main_menu = new UIRenderer(renderer);
	this->pause_menu = new UIRenderer(renderer);
	this->gameplay_menu = new UIRenderer(renderer);
	this->gameover_menu = new UIRenderer(renderer);

	this->state = MAIN;

	this->selected_entry = 0;

	// ==========ÝËÅÌÅÍÒÛ ÌÅÍÞ==========

	// ÝËÅÌÅÍÒÛ ÃËÀÂÍÎÃÎ ÌÅÍÞ

	this->title = new Label(renderer, "SPACE OPERATION", 53, 250, 25);
	this->play_button = new Label(renderer, "PLAY", 208, 312, 16);
	this->exit_button = new Label(renderer, "EXIT", 208, 344, 16);

	this->main_menu->AddWidget(this->title);
	this->main_menu->AddWidget(this->play_button);
	this->main_menu->AddWidget(this->exit_button);

	// ÝËÅÌÅÍÒÛ ÈÃÐÎÂÎÃÎ ÏÐÎÖÅÑÑÀ

	this->hp_bar_back = new Image(renderer, "assets/hp_bar.png", 0, 0, 244, 20);
	this->hp_bar = new ProgressBar(renderer, 100, 0, 0, 240);
	this->hp_label = new Label(renderer, "HP: 0", 0, 20, 16);
	this->score_counter = new Label(renderer, "00000000", 352, 20, 16);
	this->level_label = new Label(renderer, "LEVEL: 0", 0, 40, 16);

	this->gameplay_menu->AddWidget(this->hp_bar_back);
	this->gameplay_menu->AddWidget(this->hp_bar);
	this->gameplay_menu->AddWidget(this->hp_label);
	this->gameplay_menu->AddWidget(this->score_counter);
	this->gameplay_menu->AddWidget(this->level_label);

	// ÝËÅÌÅÍÒÛ ÌÅÍÞ ÏÀÓÇÛ

	this->pause_label = new Label(renderer, "PAUSE", 165, 250, 30);
	this->resume_button = new Label(renderer, "RESUME", 192, 344, 16);
	this->back_button = new Label(renderer, "BACK TO TITLE", 136, 376, 16);

	this->pause_menu->AddWidget(this->pause_label);
	this->pause_menu->AddWidget(this->resume_button);
	this->pause_menu->AddWidget(this->back_button);

	// ÝËÅÌÅÍÒÛ ÌÅÍÞ ÏÐÎÈÃÐÛØÀ

	this->gameover_label = new Label(renderer, "GAME OVER", 105, 250, 30);
	this->retry_button = new Label(renderer, "RETRY", 160, 344, 16);
	this->gameover_back_button = new Label(renderer, "BACK TO TITLE", 136, 376, 16);
	this->gameover_exit_button = new Label(renderer, "QUIT GAME", 160, 408, 16);

	this->gameover_menu->AddWidget(this->gameover_label);
	this->gameover_menu->AddWidget(this->retry_button);
	this->gameover_menu->AddWidget(this->gameover_back_button);
	this->gameover_menu->AddWidget(this->gameover_exit_button);

	// =================================
}

MenuManager::~MenuManager() {
	
	// Âñå âèäæåòû óíè÷òîæàþòñÿ âìåñòå ñ ðåíäåðåðàìè

	logging::Info(MODULE_NAME, "Destroying menu manager...");

	delete this->main_menu;
	delete this->pause_menu;
	delete this->gameplay_menu;
	delete this->gameover_menu;

}

void MenuManager::Render(double dt) {

	this->hp_bar->value = Game::level_manager->player->hp;
	this->hp_label->text = std::format("HP: {}", Game::level_manager->player->hp);
	this->score_counter->text = std::format("{:0>8}", Game::level_manager->player->score);
	this->level_label->text = std::format("LEVEL: {}", Game::level_manager->difficulty);

	switch (this->state) {
	case MAIN: {
		if (this->selected_entry == 0) {
			this->play_button->size = { 20, 20 };
			this->exit_button->size = { 16, 16 };

			this->play_button->pos.x = 200;
			this->exit_button->pos.x = 208;
		}
		else {
			this->play_button->size = { 16, 16 };
			this->exit_button->size = { 20, 20 };

			this->play_button->pos.x = 208;
			this->exit_button->pos.x = 200;
		}

		this->main_menu->Render(dt);
		break;
	}
	case PAUSE: {
		if (this->selected_entry == 0) {
			this->resume_button->size = { 20, 20 };
			this->back_button->size = { 16, 16 };

			this->resume_button->pos.x = 180;
			this->back_button->pos.x = 136;
		}
		else {
			this->resume_button->size = { 16, 16 };
			this->back_button->size = { 20, 20 };

			this->resume_button->pos.x = 192;
			this->back_button->pos.x = 110;
		}

		this->pause_menu->Render(dt);
		break;
	}
	case GAMEOVER: {
		switch (this->selected_entry) {
		case 0: {
			this->retry_button->size = { 20, 20 };
			this->gameover_back_button->size = { 16, 16 };
			this->gameover_exit_button->size = { 16, 16 };

			this->retry_button->pos.x = 190;
			this->gameover_back_button->pos.x = 136;
			this->gameover_exit_button->pos.x = 168;
			break;
		}
		case 1: {
			this->retry_button->size = { 16, 16 };
			this->gameover_back_button->size = { 20, 20 };
			this->gameover_exit_button->size = { 16, 16 };

			this->retry_button->pos.x = 200;
			this->gameover_back_button->pos.x = 110;
			this->gameover_exit_button->pos.x = 168;
			break;
		}
		case 2: {
			this->retry_button->size = { 16, 16 };
			this->gameover_back_button->size = { 16, 16 };
			this->gameover_exit_button->size = { 20, 20 };

			this->retry_button->pos.x = 200;
			this->gameover_back_button->pos.x = 136;
			this->gameover_exit_button->pos.x = 150;
		}
		}

		this->gameover_menu->Render(dt);
		break;
	}
	case GAMEPLAY: this->gameplay_menu->Render(dt); break;
	}

}

void MenuManager::HandleEvent(SDL_Event e) {
	if (e.type == EVENT_GAMEOVER) {
		this->state = GAMEOVER;
		this->selected_entry = 0;
	}

	if (e.type != SDL_KEYDOWN) return;

	switch (this->state) {
	case MAIN: {

		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_W: {
			if (this->selected_entry > 0)
				this->selected_entry--;
			break;
		}
		case SDL_SCANCODE_S: {
			if (this->selected_entry < 1)
				this->selected_entry++;
			break;
		}
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_RETURN: {
			if (this->selected_entry == 0) {
				this->state = GAMEPLAY;

				SDL_Event e{ EVENT_GAME_UNFREEZE };
				SDL_PushEvent(&e);
			}
			else {
				SDL_Event e{ SDL_QUIT };
				SDL_PushEvent(&e);
			}
		}
		}

		break;
	}
	case GAMEPLAY: {
		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
			this->state = PAUSE;
			this->selected_entry = 0;

			SDL_Event e{ EVENT_GAME_FREEZE };
			SDL_PushEvent(&e);
		}
		break;
	}
	case PAUSE: {

		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_W: {
			if (this->selected_entry > 0)
				this->selected_entry--;
			break;
		}
		case SDL_SCANCODE_S: {
			if (this->selected_entry < 1)
				this->selected_entry++;
			break;
		}
		case SDL_SCANCODE_ESCAPE: {
			this->state = GAMEPLAY;
			this->selected_entry = 0;

			SDL_Event e{ EVENT_GAME_UNFREEZE };
			SDL_PushEvent(&e);
			break;
		}
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_RETURN: {
			if (this->selected_entry == 0) {
				this->state = GAMEPLAY;

				SDL_Event e{ EVENT_GAME_UNFREEZE };
				SDL_PushEvent(&e);
			}
			else {
				this->state = MAIN;
				this->selected_entry = 0;

				SDL_Event e{ EVENT_GAME_RESET };
				SDL_PushEvent(&e);
			}
		}
		}
		break;
	}
	case GAMEOVER: {

		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_W: {
			if (this->selected_entry > 0)
				this->selected_entry--;
			break;
		}
		case SDL_SCANCODE_S: {
			if (this->selected_entry < 2)
				this->selected_entry++;
			break;
		}
		case SDL_SCANCODE_SPACE:
		case SDL_SCANCODE_RETURN: {
			switch (this->selected_entry) {
			case 0: {
				this->state = GAMEPLAY;

				SDL_Event e{ EVENT_GAME_RESTART };
				SDL_PushEvent(&e);
				break;
			}
			case 1: {
				this->state = MAIN;

				SDL_Event e{ EVENT_GAME_RESET };
				SDL_PushEvent(&e);
				break;
			}
			case 2: {
				SDL_Event e{ SDL_QUIT };
				SDL_PushEvent(&e);
			}
			}
		}
		}

	}
	}
}