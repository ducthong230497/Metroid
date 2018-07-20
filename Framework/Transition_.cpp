#include "Transition_.h"


Transition_::Transition_(string to, std::initializer_list<Condition_*> c)
{
	this->to = to;
	ct = c;
}


Transition_::~Transition_()
{

}


