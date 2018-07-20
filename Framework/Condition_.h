#pragma once
#include <vector>
#include <string>
#include <map>
using namespace std;

class Para { };

enum Parameter
{
	_Float,
	_Int,
	_Bool,
	_Trigger
};

enum IntCondition
{
	_Greater,
	_Less,
	_Equal,
	_NotEqual
};

enum FloatCondition
{
	Greater,
	Less
};

class Bool : public Para
{
public:
	Bool() {}
	Bool(bool value)
	{
		this->value = value;
	}
	bool value;
};

class Int : public Para
{
public:
	Int() {}
	Int(float value)
	{
		this->value = value;
	}
	Int(IntCondition intCondition, float value)
	{
		this->intCondition = intCondition;
		this->value = value;
	}
	IntCondition intCondition;
	int value;
};

class Float :public Para
{
public:
	Float() {}
	Float(float value)
	{
		this->value = value;
	}
	Float(FloatCondition floatCondition, float value)
	{
		this->floatCondition = floatCondition;
		this->value = value;
	}
	FloatCondition floatCondition;
	float value;
};

class Parameters
{
public:

	map<string, Para*> paras;

	Parameters()
	{

	}

	void Add(string name, bool value)
	{
		Bool * b = new Bool(value);
		paras.emplace(name, b);
	}
	void Add(string name, int value)
	{
		Int* i = new Int(value);
		paras.emplace(name, i);
	}
	void Add(string name, float value)
	{
		Float* f = new Float(value);
		paras.emplace(name, f);
	}

	void SetInterger(string name, int value)
	{
		Int* data = static_cast<Int*>(paras[name]);
		data->value = value;
	}

	void SetFloat(string name, float value)
	{
		Float* data = static_cast<Float*>(paras[name]);
		data->value = value;
	}

	void SetBool(string name, bool value)
	{
		Bool* data = static_cast<Bool*>(paras[name]);
		data->value = value;
	}

	~Parameters()
	{
		if (paras.size() > 0)
		{
			for (auto itr = paras.begin(); itr != paras.end(); itr++) {
				if (itr->second) { delete itr->second; itr->second = nullptr; }
			}
			paras.clear();
		}
	}
};

class Condition_
{
public:
	Parameter parameter; // bool - float - int - trigger
	string name; // variable name
	Para *para; // data(value)
	Condition_(string name, bool value);
	Condition_(string name, FloatCondition fCondition, float value);
	Condition_(string name, IntCondition iCondition, int value);
	~Condition_();
	void AddCondition(string name, bool value);
	void AddCondition(string name, FloatCondition fCondition, float value);
	void AddCondition(string name, IntCondition iCondition, int value);
};

