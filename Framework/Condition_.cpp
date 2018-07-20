#include "Condition_.h"


Condition_::Condition_(string name, bool value)
{
	AddCondition(name, value);
}

Condition_::Condition_(string name, FloatCondition fCondition, float value)
{
	AddCondition(name, fCondition, value);
}

Condition_::Condition_(string name, IntCondition iCondition, int value)
{
	AddCondition(name, iCondition, value);
}

Condition_::~Condition_()
{
	if (para != nullptr) { delete para; para = nullptr; }
}

void Condition_::AddCondition(string name, bool value)
{
	para = new Bool(value);
	this->name = name;
	parameter = Parameter::_Bool;
}

void Condition_::AddCondition(string name, FloatCondition fCondition, float value)
{
	para = new Float(fCondition, value);
	this->name = name;
	parameter = Parameter::_Float;

}

void Condition_::AddCondition(string name, IntCondition iCondition, int value)
{
	para = new Int(iCondition, value);
	this->name = name;
	parameter = Parameter::_Int;

}

