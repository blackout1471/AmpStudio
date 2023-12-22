using System.Runtime.InteropServices;

namespace AmpApi.Wrappers;

[StructLayout(LayoutKind.Sequential)]
public struct EffectProcessorsResultDto
{
    public nint Effects;
    public nint Count;

    [DllImport("AmpProcessingWrapper.dll")]
    public static extern void FreeVectorEffectProcessor(EffectProcessorsResultDto vector);
}

