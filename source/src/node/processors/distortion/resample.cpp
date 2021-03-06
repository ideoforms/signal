#include "signalflow/node/processors/distortion/resample.h"

#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/constant.h"

#include <stdlib.h>

namespace signalflow
{

Resample::Resample(NodeRef input, NodeRef sample_rate, NodeRef bit_rate)
    : UnaryOpNode(input), sample_rate(sample_rate), bit_rate(bit_rate)
{
    this->phase = 0.0;
    this->phase_last = -1.0;

    this->name = "resample";
    this->create_input("sample_rate", this->sample_rate);
    this->create_input("bit_rate", this->bit_rate);
    this->alloc();
}

void Resample::alloc()
{
    this->sample_last.resize(this->num_output_channels_allocated);
}

void Resample::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float phase_increment = this->sample_rate->out[0][frame] / this->graph->get_sample_rate();

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if (int(phase) > int(phase_last))
            {
                sample_last[channel] = this->input->out[channel][frame];
            }

            out[channel][frame] = sample_last[channel];

            int bits = this->bit_rate->out[channel][frame];
            if (bits < 16)
            {
                int value_16bit = (int) ((out[channel][frame] * 0.5 + 0.5) * 65536);

                out[channel][frame] = (float) (value_16bit >> (16 - bits)) / (65536 >> (16 - bits));
                out[channel][frame] = out[channel][frame] * 2 - 1;
            }
        }

        if (int(phase) > int(phase_last))
        {
            phase_last = phase;
        }

        phase += phase_increment;
    }
}

}
