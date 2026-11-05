#ifndef MENU_HPP
#define MENU_HPP

#include <SDL.h>
#include "ui.hpp"


class MenuManager {
private:

	typedef enum {

		MAIN,
		PAUSE,
		GAMEPLAY,
		GAMEOVER

	} MenuState;

	UIRenderer* main_menu;
	UIRenderer* pause_menu;
	UIRenderer* gameplay_menu;
	UIRenderer* gameover_menu;

	// ==========щкелемрш лемч==========

	// щкелемрш цкюбмнцн лемч

	Label* title;
	Label* play_button;
	Label* exit_button;

	// щкелемрш хцпнбнцн опнжеяяю

	Image* hp_bar_back;
	ProgressBar* hp_bar;
	Label* hp_label;
	Label* score_counter;
	Label* level_label;

	// щкелемрш лемч оюсгш

	Label* pause_label;
	Label* resume_button;
	Label* back_button;

	// щкелемрш лемч опнхцпшью

	Label* gameover_label;
	Label* retry_button;
	Label* gameover_back_button;
	Label* gameover_exit_button;

	// =================================

	MenuState state;

	int selected_entry;

public:

	MenuManager(SDL_Renderer* renderer);
	~MenuManager();

	void Render(double dt);
	void HandleEvent(SDL_Event e);

};

#endif