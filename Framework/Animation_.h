#pragma once
#include <string>
#include <vector>
//#include "Rect.h"
#include "GameObject.h"
#include "TextureRegion.h"
#include "Transition_.h"
using namespace std;

// In a state we have many transitions such as:
//	 + Idle -> Attack
//	 + Idle -> Run
//   + Idle -> Jump
//   + Idle -> Die

// In a state we many images such as: metroid idle state we have 5 image


class Animation_
{
public:
	bool loop;
	string name;
	int size;
	vector<TextureRegion*> regions;
	vector<Transition_*> transitions;
	float duration;
	Animation_() {}
	function<void()> enter;
	function<void()> update;
	function<void()> exit;
	Animation_(string _name, function<void()> _enter, function<void()> _update, function<void()> _exit, bool loop);


	//	void AddTransition(Transition transition);
	void AddTransition(string name, std::initializer_list<Condition_*> c);
	void AddRegion(const TextureRegion &region);

	void AddRegion(const std::vector<TextureRegion> &region);

	void SetFrameInterval(float value);

	~Animation_();
};


