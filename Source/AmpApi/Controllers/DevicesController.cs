using AmpApi.Wrappers;
using Asp.Versioning;

namespace AmpApi.Controllers;


[ApiController]
[ApiVersion("1.0")]
[Route("api/v{version:apiVersion}/[controller]")]
public class DevicesController(IAudioEngineWrapper audioEngineWrapper) : ControllerBase
{
    private readonly IAudioEngineWrapper _audioEngineWrapper = audioEngineWrapper;

    [HttpGet]
    public ActionResult GetDevicesAvailable()
    {
        return Ok(_audioEngineWrapper.GetAvailableDevices());
    }

    [HttpGet("current")]
    public ActionResult GetDeviceDetails()
    {
        return Ok(_audioEngineWrapper.GetDeviceDetails());
    }


}
