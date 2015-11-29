#pragma once

#include "../constants.h"
#include "../unit.h"
#include "../registry.h"

namespace signum::op
{

class Multiply : public BinaryOpUnit
{

public:

	Multiply(UnitRef a = 1.0, UnitRef b = 1.0) : BinaryOpUnit(a, b)
	{
		this->name = "multiply";
	}

	static bool init()
	{
		NodeRegistry *reg = NodeRegistry::global();
		reg->add<Multiply>("multiply");
		return false;
	}

	virtual void next(sample **out, int num_frames)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			for (int channel = 0; channel < this->channels_out; channel++)
			{
				out[channel][frame] = input0->out[channel][frame] * input1->out[channel][frame];
			}
		}
	}
};


REGISTER(Multiply, "multiply");
// NodeRegistry::global()->add<class>(name);
// static bool Multiply_ok = NodeRegistry::global()->add<Multiply>("multiply");


}

