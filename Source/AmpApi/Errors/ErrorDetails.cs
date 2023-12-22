namespace AmpApi.Errors;

public class ErrorDetails : ProblemDetails
{
    public Guid Id { get; init; }
}
