#ifdef __APPLE__

#include "signal/node/fft/find-peaks.h"
#include "signal/core/graph.h"

namespace libsignal
{

int find_prev_bin_with_magnitude(sample *bins, int index)
{
    sample magnitude = bins[index];
    for (int bin_index = index - 1; bin_index > 1; bin_index--)
    {
        if (bins[bin_index] >= magnitude)
        {
            return bin_index;
        }
    }
    return 0;
}

int find_next_bin_with_magnitude(sample *bins, int index, int num_bins)
{
    sample magnitude = bins[index];
    for (int bin_index = index + 1; bin_index < num_bins; bin_index++)
    {
        if (bins[bin_index] >= magnitude)
        {
            return bin_index;
        }
    }
    return num_bins - 1;
}

sample get_min_magnitude_in_range(sample *bins, int index_left, int index_right)
{
    sample min_magnitude = 1e6;
    for (int bin_index = index_left; bin_index < index_right; bin_index++)
    {
        if (bins[bin_index] < min_magnitude)
        {
            min_magnitude = bins[bin_index];
        }
    }
    return min_magnitude;
}

FFTFindPeaks::FFTFindPeaks(NodeRef input, int count, NodeRef prominence)
    : FFTOpNode(input), count(count), prominence(prominence)
{
    this->name = "fft-find-peaks";
    this->num_output_channels = this->min_output_channels = this->max_output_channels = count;
    this->update_channels();

    this->add_input("prominence", this->prominence);
}

void FFTFindPeaks::process(sample **out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    std::vector <sample>peaks(this->count);
    int peak_count = 0;

    for (int hop = 0; hop < 1; hop++)
    {
        sample *mags_in = this->input->out[hop];
        for (int bin_index = 2; bin_index < this->num_bins - 1; bin_index++)
        {
            if (mags_in[bin_index] > mags_in[bin_index - 1] && mags_in[bin_index] > mags_in[bin_index + 1])
            {
                // is a peak
                int index_left = find_prev_bin_with_magnitude(mags_in, bin_index);
                int index_right = find_next_bin_with_magnitude(mags_in, bin_index, this->num_bins);
                sample min_contour_left = get_min_magnitude_in_range(mags_in, index_left, bin_index);
                sample min_contour_right = get_min_magnitude_in_range(mags_in, bin_index + 1, index_right);
                sample min_contour = MIN(min_contour_left, min_contour_right);
                if (min_contour == 0)
                    min_contour = 1e-9;
                sample prominence = mags_in[bin_index] / min_contour;
                if (prominence > this->prominence->out[0][0] && peak_count < count)
                {
                    float peak_freq = (float) bin_index * graph->get_sample_rate() / this->fft_size;
                    // printf("Found peak at frequency: %f, prominence = %f\n", peak_freq, prominence);
                    peaks[peak_count++] = peak_freq;
                }
            }
        }
        for (int channel = 0; channel < count; channel++)
        {
            if (channel < peak_count)
            {
                int peak_index = channel;
                for (int bin_index = 0; bin_index < num_frames; bin_index++)
                {
                    out[peak_index][bin_index] = peaks[peak_index];
                }
            }
            else
            {
                for (int bin_index = 0; bin_index < num_frames; bin_index++)
                {
                    out[channel][bin_index] = 0;
                }
            }
        }
    }
}



}


#endif