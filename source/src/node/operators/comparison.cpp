#include "signalflow/node/operators/comparison.h"

namespace signalflow
{

Equal::Equal(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "equals";
}

void Equal::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = (int) (input0->out[channel][frame] == input1->out[channel][frame]);
}

NotEqual::NotEqual(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "not-equal";
}

void NotEqual::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = (int) (input0->out[channel][frame] != input1->out[channel][frame]);
}

GreaterThan::GreaterThan(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "greater-than";
}

void GreaterThan::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = (int) (input0->out[channel][frame] > input1->out[channel][frame]);
}

GreaterThanOrEqual::GreaterThanOrEqual(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "greater-than-or-equal";
}

void GreaterThanOrEqual::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = (int) (input0->out[channel][frame] >= input1->out[channel][frame]);
}

LessThan::LessThan(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "less-than";
}

void LessThan::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = (int) (input0->out[channel][frame] < input1->out[channel][frame]);
}

LessThanOrEqual::LessThanOrEqual(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "less-than-or-equal";
}

void LessThanOrEqual::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = (int) (input0->out[channel][frame] <= input1->out[channel][frame]);
}

Modulo::Modulo(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "modulo";
}

void Modulo::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = fmodf(input0->out[channel][frame], input1->out[channel][frame]);
}

Abs::Abs(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "abs";
}

void Abs::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = fabsf(input->out[channel][frame]);
}

If::If(NodeRef a, NodeRef value_if_true, NodeRef value_if_false)
    : UnaryOpNode(a), value_if_true(value_if_true), value_if_false(value_if_false)
{
    this->name = "if";

    this->create_input("value_if_true", this->value_if_true);
    this->create_input("value_if_false", this->value_if_false);
}

void If::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
        for (int frame = 0; frame < num_frames; frame++)
            out[channel][frame] = input->out[channel][frame] ? value_if_true->out[channel][frame] : value_if_false->out[channel][frame];
}

}
