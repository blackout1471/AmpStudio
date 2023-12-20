using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;

namespace AmpApi;

[ApiController]
[Route("api/v1.0/[controller]")]
public class AudioEngineController(ILogger<AudioEngineController> logger, IAudioEngineWrapper audioEngineWrapper) : ControllerBase
{
    private readonly ILogger<AudioEngineController> _logger = logger ?? throw new ArgumentNullException(nameof(logger));
    private readonly IAudioEngineWrapper _audioEngineWrapper = audioEngineWrapper ?? throw new ArgumentNullException(nameof(audioEngineWrapper));

    [HttpGet]
    public ActionResult<IEnumerable<string>> GetDevices()
    {
        return _audioEngineWrapper.GetAvailableDevices();
    }
}