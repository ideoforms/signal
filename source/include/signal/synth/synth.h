#pragma once

#include "signal/synth/nodedef.h"
#include "signal/synth/synthspec.h"
#include "signal/synth/synthtemplate.h"

namespace libsignal
{
class Synth
{
public:
    Synth(SynthSpecRef synthspec);
    Synth(SynthTemplateRef synthtemplate);
    Synth(std::string name);
    NodeRef instantiate(NodeDefinition *nodedef);

    void set_input(std::string name, float value);
    void set_input(std::string name, NodeRef value);
    void disconnect();

    NodeRef output;
    std::unordered_map<std::string, NodeRef> inputs;
    std::set<NodeRef> nodes;
};

template <class T>
class SynthRefTemplate : public std::shared_ptr<T>
{
public:
    SynthRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    SynthRefTemplate(Synth *ptr)
        : std::shared_ptr<T>(ptr) {}
    SynthRefTemplate(SynthSpecRef synthspec)
        : std::shared_ptr<T>(new T(synthspec)) {}
};

typedef SynthRefTemplate<Synth> SynthRef;


}