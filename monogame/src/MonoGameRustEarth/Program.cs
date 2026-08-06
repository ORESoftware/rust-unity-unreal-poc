namespace MonoGameRustEarth;

internal static class Program
{
    private static void Main()
    {
        using var game = new RustEarthGame();
        game.Run();
    }
}
