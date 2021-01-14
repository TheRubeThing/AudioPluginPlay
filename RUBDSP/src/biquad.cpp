#include "biquad.h"

namespace rubdsp
{

BiquadParameters Biquad::getParameters()
{
    return _parameters;
}

void Biquad::setParameters(const BiquadParameters& parameters)
{
    _parameters = parameters;
}

bool Biquad::reset(double sample_rate)
{
    return true;
}

double Biquad::processAudioSample(double sample)
{
    return sample;
}

void Biquad::setCoefficients(const std::array<double, num_coeffs>& coeff_array)
{
    _coeff_array = coeff_array;
}

std::array<double, num_coeffs> Biquad::getCoefficients()
{
    return _coeff_array;
}

std::array<double, num_states> Biquad::getStateArray()
{
    return _state_array;
}

} // namespace rubdsp