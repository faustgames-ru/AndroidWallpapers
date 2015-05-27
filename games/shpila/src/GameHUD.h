#ifndef GAMEHUD_H_
#define GAMEHUD_H_

#include "gameplay.h"
using namespace gameplay;

typedef void(*ControlEventHandler) (Game* game);

struct ControlBind
{
	Control* _control;
	Control::Listener::EventType _evt;
	ControlEventHandler _handler;
	ControlBind()
		: _control(NULL), _evt(Control::Listener::EventType::PRESS), _handler(NULL) {}
	ControlBind(Control* control, Control::Listener::EventType evt, ControlEventHandler handler)
		: _control(control), _evt(evt), _handler(handler) {}
};

class GameHUD: public Control::Listener
{
public:
	GameHUD();

	virtual void controlEvent(Control* control, EventType evt);
	virtual void initialize(Game* game, Scene* scene);
	virtual void finalize(Game* game);
	void bind(char* control, Control::Listener::EventType evt, ControlEventHandler handler);
	void update(Game* game, float elapsedTime);
	void render(Game* game, float elapsedTime);
	Form* form();

private:
	Form* _form;
	Game* _game;
	std::vector<ControlBind> _binds;
};

#endif