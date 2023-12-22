using AmpApi.Extensions;
using AmpApi.Models;
using System.Linq;

namespace AmpApi.Wrappers;

public partial class AudioEngineWrapper : IAudioEngineWrapper
{
    private nint enginePtr;

    public AudioEngineWrapper()
    {
        enginePtr = CreateAudioEngine();
    }

    ~AudioEngineWrapper()
    {
        DestroyAudioEngine(enginePtr);
    }

    public void Init()
    {
        InitializeAudioEngine(enginePtr);
    }

    public float GetInputDbLevel()
    {
        return GetInputDbLevel(enginePtr);
    }

    public float GetOutputDbLevel()
    {
        return GetOutputDbLevel(enginePtr);
    }

    public void SetDesiredInputDbLevel(int level)
    {
        SetDesiredInputDbLevel(enginePtr, level);
    }

    public void SetDesiredOutputDbLevel(int level)
    {
        SetDesiredOutputDbLevel(enginePtr, level);
    }

    public List<string> GetAvailableDevices()
    {
        var devicesRaw = GetAvailableDevices(enginePtr);
        var list = devicesRaw.ToList();

        FreeVectorStringResult(devicesRaw);

        return list;
    }

    public DeviceDetails GetDeviceDetails()
    {
        var detailsDto = GetDeviceDetails(enginePtr);
        var details = new DeviceDetails(detailsDto.name.ToList().Single(),
                                 detailsDto.inputChannels,
                                 detailsDto.outputChannels,
                                 detailsDto.minBufferSize,
                                 detailsDto.maxBufferSize,
                                 detailsDto.prefferedBufferSize,
                                 detailsDto.granularityBuffer,
                                 detailsDto.sampleRate);

        FreeVectorStringResult(detailsDto.name);
        return details;
    }

    public int GetDesiredInputDbLevel()
    {
        return GetDesiredInputDbLevel(enginePtr);
    }

    public int GetDesiredOutputDbLevel()
    {
        return GetDesiredOutputDbLevel(enginePtr);
    }

    public List<string> GetAvailableEffects()
    {
        var effects = GetAvailableEffects(enginePtr);



        EffectProcessorsResultDto.FreeVectorEffectProcessor(effects);

        return new List<string>();
    }
}