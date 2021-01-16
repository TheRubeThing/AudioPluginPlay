#ifndef RUBDSP_BIQUAD_H
#define RUBDSP_BIQUAD_H

#include <array>

#include "audio_signal_processor_interface.h"

namespace rubdsp
{

enum BIQUAD_COEFF { a0, a1, a2, b1, b2, c0, d0, num_coeffs};
enum BIQUAD_STATE { x_z1, x_z2, y_z1, y_z2, num_states};
enum class BIQUAD_ALGORITHM
{
    DIRECT,
    CANONICAL,
    TRANSPOSE_DIRECT,
    TRANSPOSE_CANONICAL
};

struct BiquadParameters
{
    BiquadParameters() {}
    BiquadParameters& operator=(const BiquadParameters& parameters)
    {
        if (this == &parameters)
        {
			return *this;
        }

        biquad_calc_type = parameters.biquad_calc_type;

        return *this;
    }

    BIQUAD_ALGORITHM biquad_calc_type = BIQUAD_ALGORITHM::DIRECT;
};

class Biquad : IAudioSignalProcessor
{
public: 
    Biquad() {}
    ~Biquad() = default;

    BiquadParameters getParameters();

    void setParameters(const BiquadParameters& parameters);

    bool reset(double sample_rate) override;

    double processAudioSample(double sample) override;

    void setCoefficients(const std::array<double, num_coeffs>& coeff_array);

    std::array<double, num_coeffs> getCoefficients();

    std::array<double, num_states> getStateArray();

    double getMagnitudedB(double frequency);

    double getMagnitudeGain(double frequency);

private:

    std::array<double, num_coeffs> _coeff_array{0.0};
    std::array<double, num_states> _state_array{0.0};
    BiquadParameters _parameters;
};

}

#endif // RUBDSP_BIQUAD_H