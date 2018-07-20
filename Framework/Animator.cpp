#include "Animator.h"



void Animator::reset()
{
	index = 0;
	stateTime = 0;
}

Animator::Animator()
{
	//parameters =  Parameters();
	reset();
}

void Animator::AddAnimation(std::initializer_list<Animation_*> animation)
{
	animations = animation;
}


void Animator::ChangeAnimation(Animation_ & animation)
{
	if (&animation != NULL)
	{
		if (currentAnimation.exit != NULL)
			currentAnimation.exit();

		previousAnimation = currentAnimation;
		currentAnimation = animation;
		Trace::Log("%s", currentAnimation.name.c_str());
		if (currentAnimation.enter != NULL)
			currentAnimation.enter();

		frameInterval = currentAnimation.duration;
		reset();
	}
}

void Animator::Update()
{

}

void Animator::SetInteger(string name, int value)
{
	parameters.SetInterger(name, value);
}

void Animator::SetFloat(string name, float value)
{
	parameters.SetFloat(name, value);
}

void Animator::SetBool(string name, bool value)
{
	parameters.SetBool(name, value);
}

void Animator::CheckCondition(float dt)
{
	//	Next(dt);
	// check each transition
	for (auto transition : currentAnimation.transitions)
	{

		int count = 0;
		//	for (auto cond = std::rbegin(transition->c);cond!=std::rend(transition->c);++cond)
		for (auto cond : transition->ct)
		{
			switch (cond->parameter)
			{
			case Parameter::_Bool: {
				Bool * b1 = static_cast<Bool*>(parameters.paras[cond->name]);
				Bool*  b2 = static_cast<Bool*>(cond->para);
				if (b1->value == b2->value)
					count++;
			}
								   break;
			case Parameter::_Int: {
				Int * t1 = static_cast<Int*>(parameters.paras[cond->name]);
				Int * t2 = static_cast<Int*>(cond->para);
				if ((t2->intCondition == IntCondition::_Equal && t1->value == t2->value)
					|| (t2->intCondition == IntCondition::_Greater && t1->value > t2->value)
					|| (t2->intCondition == IntCondition::_Less && t1->value < t2->value)
					|| (t2->intCondition == IntCondition::_NotEqual && t1->value != t2->value))
					count++; }
								  break;
			case Parameter::_Float: {
				Float * f1 = static_cast<Float*>(parameters.paras[cond->name]);
				Float * f2 = static_cast<Float*>(cond->para);
				if ((f2->floatCondition == FloatCondition::Greater && f1->value > f2->value)
					|| (f2->floatCondition == FloatCondition::Less && f1->value < f2->value))
					count++; }
									break;
			}
		}

		// pass all condition:
		if (count == transition->ct.size())
		{
			Animation_* next = FindAnimation(transition->to);
			if (&next != NULL)
			{
				ChangeAnimation(*next);
				//	break;
			}
			else
			{

				if (currentAnimation.update != NULL)
					currentAnimation.update();
			}
		}
	}
}

Animation_ * Animator::FindAnimation(string name)
{
	if (animations.size() <= 0)
		return NULL;
	for (auto &i : animations) {
		if (i->name == name)
			return i;
	}

}

TextureRegion * Animator::GetKeyAnimation()
{
	return currentAnimation.regions[index];
}


TextureRegion* Animator::Next(float deltaTime)
{
	if (currentAnimation.regions.size() == 0)
		return NULL;

	stateTime += deltaTime;

	//if true, next animation
	if (stateTime >= frameInterval)
	{
		//next index
		index = (index + 1) % currentAnimation.size;

		if (!currentAnimation.loop)
			index = currentAnimation.size - 1;

		//reset state time
		stateTime = 0;
	}
	stateTime += deltaTime;

	TextureRegion *currentRegion = currentAnimation.regions[index];

	return  currentRegion;
}

Animator::~Animator()
{
	//delete currentAnimation;
	//delete previousAnimation;
	for (auto it : animations)
		delete it;
}
