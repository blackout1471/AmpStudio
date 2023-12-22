using AmpApi.Wrappers;
using Asp.Versioning;

namespace AmpApi.Controllers;

[ApiController]
[ApiVersion(1.0)]
[Route("api/v{version:apiVersion}/[controller]")]
public class EffectsController(IAudioEngineWrapper audioEngineWrapper) : ControllerBase
{

    private readonly IAudioEngineWrapper _audioEngineWrapper = audioEngineWrapper;

    [HttpGet("available")]
    public ActionResult<IEnumerable<string>> GetEffectsAvailable()
    {
        var effects = _audioEngineWrapper.GetAvailableEffects();

        return Ok();
    }
}
