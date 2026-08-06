using System.Runtime.InteropServices;

namespace MonoGameRustEarth;

[StructLayout(LayoutKind.Sequential)]
internal struct ControlInput
{
    internal float RotateX;
    internal float RotateY;
    internal float Zoom;
    internal uint Reset;
}

[StructLayout(LayoutKind.Sequential)]
internal struct EarthRenderState
{
    internal float Radius;
    internal float AtmosphereRadius;
    internal float RotationX;
    internal float RotationY;
    internal float CloudRotationY;
    internal float CameraDistance;
    internal float LightX;
    internal float LightY;
    internal float LightZ;
}

internal static class RustEngineNative
{
    private const string Library = "rust_engine";

    [DllImport(Library, EntryPoint = "rust_engine_create")]
    internal static extern nint Create();

    [DllImport(Library, EntryPoint = "rust_engine_destroy")]
    internal static extern void Destroy(nint engine);

    [DllImport(Library, EntryPoint = "rust_engine_set_control_input")]
    internal static extern void SetControlInput(nint engine, ControlInput input);

    [DllImport(Library, EntryPoint = "rust_engine_tick")]
    internal static extern void Tick(nint engine, float deltaSeconds);

    [DllImport(Library, EntryPoint = "rust_engine_render_state")]
    internal static extern EarthRenderState RenderState(nint engine);
}

internal sealed class RustEngineSession : IDisposable
{
    private nint _engine = RustEngineNative.Create();

    internal RustEngineSession()
    {
        if (_engine == 0) throw new InvalidOperationException("rust_engine_create returned null.");
    }

    internal EarthRenderState Tick(ControlInput input, float deltaSeconds)
    {
        ThrowIfDisposed();
        RustEngineNative.SetControlInput(_engine, input);
        RustEngineNative.Tick(_engine, Math.Clamp(deltaSeconds, 0f, 0.1f));
        return RustEngineNative.RenderState(_engine);
    }

    internal EarthRenderState State
    {
        get
        {
            ThrowIfDisposed();
            return RustEngineNative.RenderState(_engine);
        }
    }

    public void Dispose()
    {
        if (_engine == 0) return;
        RustEngineNative.Destroy(_engine);
        _engine = 0;
    }

    private void ThrowIfDisposed() => ObjectDisposedException.ThrowIf(_engine == 0, this);
}
