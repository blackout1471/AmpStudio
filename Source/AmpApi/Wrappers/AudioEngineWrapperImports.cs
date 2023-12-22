using System.Runtime.InteropServices;

namespace AmpApi.Wrappers;

public partial class AudioEngineWrapper
{
    const string DLL_PATH = "AmpProcessingWrapper.dll";

    [DllImport(DLL_PATH)]
    private static extern nint CreateAudioEngine();
    [DllImport(DLL_PATH)]
    private static extern void DestroyAudioEngine(nint engine);
    [DllImport(DLL_PATH)]
    private static extern void InitializeAudioEngine(nint engine);
    [DllImport(DLL_PATH)]
    private static extern float GetInputDbLevel(nint engine);
    [DllImport(DLL_PATH)]
    private static extern float GetOutputDbLevel(nint engine);
    [DllImport(DLL_PATH)]
    private static extern void SetDesiredInputDbLevel(nint engine, int dblevel);
    [DllImport(DLL_PATH)]
    private static extern void SetDesiredOutputDbLevel(nint engine, int dblevel);
    [DllImport(DLL_PATH)]
    private static extern int GetDesiredInputDbLevel(nint engine);
    [DllImport(DLL_PATH)]
    private static extern int GetDesiredOutputDbLevel(nint engine);
    [DllImport(DLL_PATH)]
    private static extern StringVectorResult GetAvailableDevices(nint engine);
    [DllImport(DLL_PATH)]
    private static extern void FreeVectorStringResult(StringVectorResult result);
    [DllImport(DLL_PATH)]
    private static extern DeviceDetailsDto GetDeviceDetails(nint engine);
    [DllImport(DLL_PATH)]
    private static extern EffectProcessorsResultDto GetAvailableEffects(nint engine);
}
