using System.Runtime.InteropServices;
namespace RustEngine.Interop;

public sealed class RustEngineSession : IDisposable
{
    private const string Library = "rust_engine";
    private nint _engine;

    public RustEngineSession()
    {
        _engine = Create();
        if (_engine == 0) throw new InvalidOperationException("rust_engine_create returned null.");
    }

    public EarthRenderState State { get { ThrowIfDisposed(); return RenderState(_engine); } }

    public EarthRenderState Tick(ControlInput input, float deltaSeconds)
    {
        ThrowIfDisposed();
        input.RotateX = Math.Clamp(input.RotateX, -1f, 1f);
        input.RotateY = Math.Clamp(input.RotateY, -1f, 1f);
        input.Zoom = Math.Clamp(input.Zoom, -1f, 1f);
        SetControlInput(_engine, input);
        NativeTick(_engine, Math.Clamp(deltaSeconds, 0f, 0.1f));
        return RenderState(_engine);
    }

    public void Dispose()
    {
        if (_engine == 0) return;
        Destroy(_engine);
        _engine = 0;
    }

    private void ThrowIfDisposed() => ObjectDisposedException.ThrowIf(_engine == 0, this);

    [DllImport(Library, EntryPoint = "rust_engine_create")]
    private static extern nint Create();
    [DllImport(Library, EntryPoint = "rust_engine_destroy")]
    private static extern void Destroy(nint engine);
    [DllImport(Library, EntryPoint = "rust_engine_set_control_input")]
    private static extern void SetControlInput(nint engine, ControlInput input);
    [DllImport(Library, EntryPoint = "rust_engine_tick")]
    private static extern void NativeTick(nint engine, float deltaSeconds);
    [DllImport(Library, EntryPoint = "rust_engine_render_state")]
    private static extern EarthRenderState RenderState(nint engine);
}
