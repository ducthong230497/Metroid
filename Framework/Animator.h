#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include "Animation_.h"

class Animator
{
private:
	void reset();
public:

	Parameters parameters;

	int index;
	float frameInterval;	//the duration of a frame
	float stateTime;		//current time in one frame
	Animation_ currentAnimation;
	Animation_ previousAnimation;
	vector<Animation_*> animations;

	Animator();

	void AddAnimation( std::initializer_list<Animation_*> animation);
	void ChangeAnimation(Animation_& animation);

	void Update();

	void SetInteger(string name, int value);
	void SetFloat(string name, float value);
	void SetBool(string name, bool value);

	void CheckCondition(float dt);

	TextureRegion* Next(float deltaTime);

	Animation_* FindAnimation(string name);

	TextureRegion* GetKeyAnimation();



	~Animator();
};
#endif
