using AmpApi.Errors;
using Microsoft.AspNetCore.Diagnostics;
using Microsoft.AspNetCore.Http;
using System.Threading;
using System.Threading.Tasks;

namespace AmpApi.Middlewares;

public class ExceptionMiddleware(ILogger<ExceptionMiddleware> logger) : IExceptionHandler
{
    private readonly ILogger<ExceptionMiddleware> _logger = logger;

    public async ValueTask<bool> TryHandleAsync(HttpContext httpContext, Exception exception, CancellationToken cancellationToken)
    {
        var problem = new ErrorDetails
        {
            Title = "Unexpected error",
            Status = 500,
            Detail = "An unexpected error occrred.",
            Id = Guid.NewGuid()
        };

        httpContext.Response.StatusCode = problem.Status ?? 500;
        httpContext.Response.ContentType = "application/json";

        _logger.LogError(exception, "Http request failed {Id} {Message} {Type} {StatusCode}", problem.Id, exception.Message, nameof(exception), problem.Status);

        await httpContext.Response.WriteAsJsonAsync(problem);

        return true;
    }
}
