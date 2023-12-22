#pragma once

namespace AmpProcessingWrapper {
    public ref class DeviceDetailsWrapper
    {
    public:
        property System::String^ Name;

        property long InputChannels;
        property long OutputChannels;

        property long MinBufferSize;
        property long MaxBufferSize;
        property long PreferredBufferSize;
        property long GranularityBuffer;

        property float SampleRate;
    };
}