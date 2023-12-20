using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace AmpApi;

public class AudioEngineWrapper : IAudioEngineWrapper
{
    const string DLL_PATH = "AmpProcessingWrapper.dll";

    [StructLayout(LayoutKind.Sequential)]
    public struct StringVectorResult
    {
        public IntPtr strings;
        public int count;
    }

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

    public record DeviceDetails(string Name,
                                long InputChannels,
                                long OutputChannels,
                                long MinBufferSize,
                                long MaxBufferSize,
                                long PrefferedBufferSize,
                                long GranularityBuffer,
                                float SampleRate);

    [DllImport(DLL_PATH)]
    private static extern IntPtr CreateAudioEngine();
    [DllImport(DLL_PATH)]
    private static extern void DestroyAudioEngine(IntPtr engine);
    [DllImport(DLL_PATH)]
    private static extern void InitializeAudioEngine(IntPtr engine);
    [DllImport(DLL_PATH)]
    private static extern float GetInputDbLevel(IntPtr engine);
    [DllImport(DLL_PATH)]
    private static extern float GetOutputDbLevel(IntPtr engine);
    [DllImport(DLL_PATH)]
    private static extern void SetDesiredInputDbLevel(IntPtr engine, int dblevel);
    [DllImport(DLL_PATH)]
    private static extern void SetDesiredOutputDbLevel(IntPtr engine, int dblevel);
    [DllImport(DLL_PATH)]
    private static extern StringVectorResult GetAvailableDevices(IntPtr engine);
    [DllImport(DLL_PATH)]
    private static extern void FreeVectorStringResult(StringVectorResult result);
    [DllImport(DLL_PATH)]
    private static extern DeviceDetailsDto GetDeviceDetails(IntPtr engine);

    private IntPtr enginePtr;

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

    public void SetInputDbLevel(int level)
    {
        SetDesiredInputDbLevel(enginePtr, level);
    }

    public void SetOutputDbLevel(int level)
    {
        SetDesiredOutputDbLevel(enginePtr, level);
    }

    public List<string> GetAvailableDevices()
    {
        var devicesRaw = GetAvailableDevices(enginePtr);
        var list = Convert(devicesRaw);

        FreeVectorStringResult(devicesRaw);

        return list;
    }

    public DeviceDetails GetDeviceDetails()
    {
        var detailsDto = GetDeviceDetails(enginePtr);
        var details = new DeviceDetails(Convert(detailsDto.name).Single(),
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

    private List<string> Convert(StringVectorResult stringVectors)
    {
        var list = new List<string>(stringVectors.count);
        IntPtr stringPtr = stringVectors.strings;

        for (int i = 0; i < stringVectors.count; i++)
        {
            var currentPtr = Marshal.ReadIntPtr(stringPtr, i * IntPtr.Size);
            list.Add(Marshal.PtrToStringAnsi(currentPtr) ?? "");
        }

        return list;
    }
}
