using AmpApi.Wrappers;
using System.Runtime.InteropServices;

namespace AmpApi.Extensions;

public static class StringVectorResultExtension
{
    internal static List<string> ToList(this StringVectorResult stringVectors)
    {
        var list = new List<string>(stringVectors.count);
        nint stringPtr = stringVectors.strings;

        for (int i = 0; i < stringVectors.count; i++)
        {
            var currentPtr = Marshal.ReadIntPtr(stringPtr, i * nint.Size);
            list.Add(Marshal.PtrToStringAnsi(currentPtr) ?? "");
        }

        return list;
    }
}
