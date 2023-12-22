using System.Runtime.InteropServices;

namespace AmpApi.Wrappers;

[StructLayout(LayoutKind.Sequential)]
public struct DeviceDetailsDto
{
    public StringVectorResult name;

    public int inputChannels;
    public int outputChannels;

    public int minBufferSize;
    public int maxBufferSize;
    public int prefferedBufferSize;
    public int granularityBuffer;

    public float sampleRate;
};