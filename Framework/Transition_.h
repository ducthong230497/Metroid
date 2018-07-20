#pragma once

#include<string>
#include "Condition_.h"
class Transition_
{
public:
	string  to;
	std::vector<Condition_*> ct;
	Transition_(string to, std::initializer_list<Condition_*> c);
	~Transition_();
};

