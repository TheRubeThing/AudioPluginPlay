#ifndef RUBDSP_AUDIO_SIGNAL_PROCESSOR_INTERFACE_H
#define RUBDSP_AUDIO_SIGNAL_PROCESSOR_INTERFACE_H

namespace rubdsp
{

/**
 * @brief Interface for audio signal processors.
 * Use if processor takes audio input and process for output.
 * Includes possible support for aux input.
 * 
 */
class IAudioSignalProcessor
{
    /**
     * @brief Reset the processor state and configure the sample rate.
     *  Should be called before processing starts.
     * 
     * @param sample_rate The sample rate to configure the plugin with.
     * @return true if succesful
     * @return false if not succesful
     */
    virtual bool reset(double sample_rate) = 0;

    /**
     * @brief Process one audio sample at a time.
     * 
     * @param sample one sample to process.
     * @return double one Sample returned from the processing.
     */
    virtual double processAudioSample(double sample) = 0;
    
    /**
     * @brief Check wether the processor can do frame by frame processing or not.
     * 
     * @return true 
     * @return false 
     */
    virtual bool canProcessAudioFrame() = 0;

    /**
     * @brief Process a single audio frame.
     * 
     * @param input_frame array of one sample per channel to process. 
     * @param output_frame array of one sample per channel to be returned from the process.
     * @param n_input_channels number of input channels.
     * @param n_output_channels number of input channels.
     * @return true if processing was succesful.
     * @return false if processing was not succesful.
     */
    virtual bool processAudioFrame(const float* input_frame,
                                         float* output_frame,
                                         int n_input_channels,
                                         int n_output_channels)
    {
        return false;
    }

    /**
     * @brief Set the process to allow aux input.
     * 
     * @param enable 
     */
    virtual void enableAuxInput(bool enable) = 0;

    /**
     * @brief Process and use the aux input if enabled.
     * 
     * @param sample Sample to process.
     * @return double Sample returned from processing.
     */
    virtual double processAuxInputAudioSample(double sample)
    {
        return sample;
    }
};

} // namespace rubdsp

#endif // RUBDSP_AUDIO_SIGNAL_PROCESSOR_INTERFACE_H