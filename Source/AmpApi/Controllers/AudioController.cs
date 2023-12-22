using AmpApi.Wrappers;
using Asp.Versioning;

namespace AmpApi.Controllers;

[ApiController]
[ApiVersion(1.0)]
[Route("api/v{version:apiVersion}/[controller]")]
public class AudioController(ILogger<AudioController> logger, IAudioEngineWrapper audioEngineWrapper) : ControllerBase
{

    private readonly ILogger<AudioController> _logger = logger;
    private readonly IAudioEngineWrapper _audioEngineWrapper = audioEngineWrapper;

    [HttpGet("inputdb")]
    public ActionResult<float> GetInputDb()
    {
        return Ok(_audioEngineWrapper.GetInputDbLevel());
    }

    [HttpGet("outputdb")]
    public ActionResult<float> GetOutputDb()
    {
        return Ok(_audioEngineWrapper.GetOutputDbLevel());
    }

    [HttpGet("desiredinputdb")]
    public ActionResult<float> GetDesiredInputDb()
    {
       return Ok(_audioEngineWrapper.GetDesiredInputDbLevel());
    }

    [HttpGet("desiredoutputdb")]
    public ActionResult<float> GetDesiredOutputDb()
    {
        return Ok(_audioEngineWrapper.GetDesiredOutputDbLevel());
    }

    [HttpPost("desiredinputdb")]
    public IActionResult SetDesiredInputDb([FromQuery] int level)
    {
        _audioEngineWrapper.SetDesiredInputDbLevel(level);

        return Ok();
    }

    [HttpPost("desiredoutputdb")]
    public IActionResult SetDesiredOutputDb([FromQuery] int level)
    {
        _audioEngineWrapper.SetDesiredOutputDbLevel(level);

        return Ok();
    }
}
