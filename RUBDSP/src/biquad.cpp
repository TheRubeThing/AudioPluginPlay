#include <cmath>

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
    _state_array.fill(0.0);
    return true;
}

double Biquad::processAudioSample(double sample)
{
    double xn = sample;
    double yn = 0.0;
    double wn = 0.0;
    switch (_parameters.biquad_calc_type)
    {
    case BIQUAD_ALGORITHM::DIRECT:
        // Filter calculation
        yn = _coeff_array[a0] * xn +
                    _coeff_array[a1] * _state_array[x_z1] +
                    _coeff_array[a2] * _state_array[x_z2] -
                    _coeff_array[b1] * _state_array[y_z1] -
                    _coeff_array[b2] * _state_array[y_z2];

        // Pirkle checks for underflow here

        // Update state
        _state_array[x_z2] = _state_array[x_z1];
        _state_array[x_z1] = xn;

        _state_array[y_z2] = _state_array[y_z1];
        _state_array[y_z1] = yn;

        return yn;
    
    case BIQUAD_ALGORITHM::CANONICAL:
        // Filter calculation
        wn = xn - (_coeff_array[b1] * _state_array[x_z1])
                       - (_coeff_array[b2] * _state_array[x_z2]);

        yn = _coeff_array[a0] * wn +
                    _coeff_array[a1] * _state_array[x_z1] +
                    _coeff_array[a2] * _state_array[x_z1];

        // Prikle checks for underflow here

        // Update state
        _state_array[x_z2] = _state_array[x_z1];
        _state_array[x_z1] = wn;

        return yn;

    case BIQUAD_ALGORITHM::TRANSPOSE_DIRECT:
        // Filter calculation
        wn = xn + _state_array[y_z1];

        yn = _coeff_array[a0] * wn + _state_array[x_z1];

        // Prikle checks for underflow here

        // Update state
        _state_array[y_z1] = - _coeff_array[b1] * wn + _state_array[y_z2]; 
        _state_array[y_z2] = - _coeff_array[b2] * wn;

        _state_array[x_z1] = _coeff_array[a1] * wn + _state_array[x_z2];
        _state_array[x_z2] = _coeff_array[a2] * wn;

        return yn;
    
    case BIQUAD_ALGORITHM::TRANSPOSE_CANONICAL:
        // Filter calculation
        yn = _coeff_array[a0] * xn + _state_array[x_z1];

        // Prikle checks for underflow here

        // Update state
        _state_array[x_z1] = _coeff_array[a1] * xn -
                             _coeff_array[b1] * yn + _state_array[x_z2];
        _state_array[x_z2] = _coeff_array[a2] * xn - _coeff_array[b2] * yn;

        return yn;
    }       
    
}

double Biquad::getMagnitudedB(double frequency)
{
    return 20 * std::log10(getMagnitudeGain(frequency));
}

double Biquad::getMagnitudeGain(double frequency)
{
    double num = std::pow(_coeff_array[a1], 2) +
                 std::pow(_coeff_array[a0] - _coeff_array[a2], 2) +
                 2 * _coeff_array[a1] * (_coeff_array[a0] + _coeff_array[a2]) * std::cos(frequency) +
                 4 * _coeff_array[a0] * _coeff_array[a2] * std::pow(std::cos(frequency), 2);

    double denom = std::pow(_coeff_array[b1], 2) +
                   std::pow(1 - _coeff_array[b2], 2) +
                   2 * _coeff_array[b1] * (1 + _coeff_array[b2]) * std::cos(frequency) +
                   4 * _coeff_array[b2] * std::pow(std::cos(frequency), 2);

    return std::sqrt(num / denom);
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