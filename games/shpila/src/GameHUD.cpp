#include "GameHUD.h"

GameHUD::GameHUD()
: _form(), _game(), _binds()
{}

void GameHUD::controlEvent(Control* control, EventType evt)
{
	for (std::vector<ControlBind>::iterator it = _binds.begin(); it != _binds.end(); ++it)
	{
		if ((it->_control == control) && (it->_evt == evt))
		{
			it->_handler(_game);
			break;
		}
	}
}

void GameHUD::initialize(Game* game, Scene* scene)
{
	_game = game;
	_form = Form::create("res/hud.form");
}
void GameHUD::finalize(Game* game)
{
	SAFE_RELEASE(_form);
}

void GameHUD::bind(char* control, Control::Listener::EventType evt, ControlEventHandler handler)
{
	Control* ctrl = _form->getControl(control);
	ctrl->addListener(this, evt);
	_binds.push_back(ControlBind(ctrl, evt, handler));
}

void GameHUD::update(Game* game, float elapsedTime)
{

}

void GameHUD::render(Game* game, float elapsedTime)
{
	_form->draw();
}

Form* GameHUD::form()
{
	return _form;
}