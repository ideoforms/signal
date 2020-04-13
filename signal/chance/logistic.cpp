#include "logistic.h"

// #include "../util.h"
#include "../graph.h"

namespace libsignal
{

Logistic::Logistic(NodeRef chaos, NodeRef frequency) : chaos(chaos), frequency(frequency)
{
    this->name = "logistic";
    this->add_input("chaos", this->chaos);
    this->add_input("frequency", this->frequency);

    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
        this->value[i] = 0.5;
    memset(this->steps_remaining, 0, sizeof(int) * SIGNAL_MAX_CHANNELS);
}

void Logistic::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float frequency = this->frequency->out[channel][frame];
            if (!frequency)
                frequency = this->graph->sample_rate;

            if (this->steps_remaining[channel] <= 0)
            {
                float chaos = this->chaos->out[channel][frame];
                this->value[channel] = chaos * this->value[channel] * (1.0 - this->value[channel]);

                if (frequency > 0)
                {
                    this->steps_remaining[channel] = this->graph->sample_rate / (frequency / 2);
                }
                else
                {
                    this->steps_remaining[channel] = 1;
                }
            }

            this->out[channel][frame] = this->value[channel];

            this->steps_remaining[channel]--;
        }
    }
}

}