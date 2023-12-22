using System.Runtime.InteropServices;

namespace AmpApi.Wrappers;

[StructLayout(LayoutKind.Sequential)]
public struct StringVectorResult
{
    public nint strings;
    public int count;
}
