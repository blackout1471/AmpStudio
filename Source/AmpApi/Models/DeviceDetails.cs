namespace AmpApi.Models;

public record DeviceDetails(string Name,
                                long InputChannels,
                                long OutputChannels,
                                long MinBufferSize,
                                long MaxBufferSize,
                                long PrefferedBufferSize,
                                long GranularityBuffer,
                                float SampleRate);
