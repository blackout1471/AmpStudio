using AmpApi.Models;

namespace AmpApi.Wrappers;

public interface IAudioEngineWrapper
{
    List<string> GetAvailableDevices();
    DeviceDetails GetDeviceDetails();
    float GetInputDbLevel();
    float GetOutputDbLevel();
    void Init();
    void SetDesiredInputDbLevel(int level);
    void SetDesiredOutputDbLevel(int level);
    int GetDesiredInputDbLevel();
    int GetDesiredOutputDbLevel();
}