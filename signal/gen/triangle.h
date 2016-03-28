#pragma once 

#include "../node.h"

namespace libsignal
{
	class Triangle : public Node
	{
	public:
		Triangle(NodeRef frequency = 440) : frequency(frequency)
		{
			this->name = "triangle";
			this->add_param("frequency", this->frequency);
			memset(this->phase, 0, sizeof(this->phase));
		};

		NodeRef frequency;

		float phase[SIGNAL_MAX_CHANNELS];

		virtual void next(sample **out, int num_frames);
	};

	REGISTER(Triangle, "triangle");
}