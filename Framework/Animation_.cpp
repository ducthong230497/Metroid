#include "Animation_.h"



//_Animation::Animation(const std::vector<TextureRegion> &regions, float frameInterval)
//{
//	_Count = regions.size();
//
//	for (int i = 0; i < _Count; i++)
//	{
//		TextureRegion* textureRegion = new TextureRegion();
//		*textureRegion = regions[i];
//		_Regions.push_back(textureRegion);
//	}
//
//	_Index = 0;
//	_StateTime = 0;
//	_FrameInterval = frameInterval;
//}

Animation_::Animation_(string _name, function<void()> _enter, function<void()> _update, function<void()> _exit, bool loop)
{
	this->name = _name;
	this->update = _update;
	this->enter = _enter;
	this->exit = _exit;
	this->loop = loop;
	duration = 0;
	size = 0;

}


//
//void _Animation::AddTransition(Transition  transition)
//{
//	transitions.push_back(transition);
//}

void Animation_::AddTransition(string name, std::initializer_list<Condition_*> c)
{
	Transition_* temp = new Transition_(name, c);
	transitions.push_back(temp);
}

void Animation_::AddRegion(const TextureRegion &region)
{
	TextureRegion* textureRegion = new TextureRegion();
	*textureRegion = region;
	regions.push_back(textureRegion);
	size++;
}

void Animation_::AddRegion(const std::vector<TextureRegion> &regions)
{
	size += regions.size();

	for (int i = 0; i < size; i++)
	{
		TextureRegion* textureRegion = new TextureRegion();
		*textureRegion = regions[i];
		this->regions.push_back(textureRegion);
	}
}

void Animation_::SetFrameInterval(float value)
{
	duration = value;
}

Animation_::~Animation_()
{
	/*for (auto it : transitions) {
	delete it;
	}
	transitions.clear();*/

	for (std::vector<TextureRegion*>::iterator obj = regions.begin(); obj != regions.end(); ++obj)
		delete *obj;
}
