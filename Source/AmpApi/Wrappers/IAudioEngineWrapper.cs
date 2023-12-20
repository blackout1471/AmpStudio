using System.Collections.Generic;

namespace AmpApi
{
    public interface IAudioEngineWrapper
    {
        List<string> GetAvailableDevices();
        AudioEngineWrapper.DeviceDetails GetDeviceDetails();
        float GetInputDbLevel();
        float GetOutputDbLevel();
        void Init();
        void SetInputDbLevel(int level);
        void SetOutputDbLevel(int level);
    }
}