using AmpProcessingWrapper;
using Asp.Versioning;

namespace AmpApi.Controllers;


[ApiController]
[ApiVersion("1.0")]
[Route("api/v{version:apiVersion}/[controller]")]
public class DevicesController(AudioEngineWrapper audioEngineWrapper) : ControllerBase
{
    private readonly AudioEngineWrapper _audioEngineWrapper = audioEngineWrapper;

    [HttpGet]
    public ActionResult<IEnumerable<string>> GetDevicesAvailable()
    {
        return Ok(_audioEngineWrapper.GetAvailableDevices());
    }

    [HttpGet("current")]
    public ActionResult<DeviceDetailsWrapper> GetDeviceDetails()
    {
        return Ok(_audioEngineWrapper.GetDeviceDetails());
    }

    [HttpPost("samplerate")]
    public ActionResult<bool> SetSampleRate([FromQuery] uint sampleRate)
    {
        return Ok(_audioEngineWrapper.SetSampleRate(sampleRate));
    }

    [HttpPost("buffersize")]
    public ActionResult<bool> SetBufferSize([FromQuery] uint bufferSize)
    {
        return Ok(_audioEngineWrapper.SetBufferSize(bufferSize));
    }
}
