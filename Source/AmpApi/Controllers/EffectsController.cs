using AmpProcessingWrapper;
using Asp.Versioning;

namespace AmpApi.Controllers;

[ApiController]
[ApiVersion(1.0)]
[Route("api/v{version:apiVersion}/[controller]")]
public class EffectsController(AudioEngineWrapper audioEngineWrapper) : ControllerBase
{

    private readonly AudioEngineWrapper _audioEngineWrapper = audioEngineWrapper;

    [HttpGet("available")]
    public ActionResult<IEnumerable<EffectProcessorWrapper>> GetEffectsAvailable()
    {
        var effects = _audioEngineWrapper.GetAvailableEffects();

        return Ok(effects);
    }
}
