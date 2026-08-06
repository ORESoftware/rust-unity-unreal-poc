using System.Runtime.InteropServices;

namespace RustEngine.Interop;

public sealed class RustEngineSession : IDisposable
{
    private readonly RustEngineHandle _handle = RustEngineHandle.Create();
    private bool _disposed;

    public EarthRenderState State
    {
        get
        {
            ThrowIfDisposed();
            return RustEngineNative.RenderState(_handle.DangerousGetHandle());
        }
    }

    public EarthRenderState Tick(ControlInput input, float deltaSeconds)
    {
        ThrowIfDisposed();
        input.RotateX = Math.Clamp(input.RotateX, -1f, 1f);
        input.RotateY = Math.Clamp(input.RotateY, -1f, 1f);
        input.Zoom = Math.Clamp(input.Zoom, -1f, 1f);
        deltaSeconds = Math.Clamp(deltaSeconds, 0f, 0.1f);
        var pointer = _handle.DangerousGetHandle();
        RustEngineNative.SetControlInput(pointer, input);
        RustEngineNative.Tick(pointer, deltaSeconds);
        return RustEngineNative.RenderState(pointer);
    }

    public SurfacePatch[] GetSurfacePatches()
    {
        ThrowIfDisposed();
        var view = RustEngineNative.SurfacePatches(_handle.DangerousGetHandle());
        if (view.Pointer == 0 || view.Length == 0)
        {
            return Array.Empty<SurfacePatch>();
        }

        if (view.Length > 10_000)
        {
            throw new InvalidDataException("Rust surface patch length exceeded the safety bound.");
        }

        var stride = Marshal.SizeOf<SurfacePatch>();
        var patches = new SurfacePatch[checked((int)view.Length)];
        for (var index = 0; index < patches.Length; index++)
        {
            patches[index] = Marshal.PtrToStructure<SurfacePatch>(view.Pointer + index * stride);
        }
        return patches;
    }

    public void Dispose()
    {
        if (_disposed) return;
        _handle.Dispose();
        _disposed = true;
    }

    private void ThrowIfDisposed() => ObjectDisposedException.ThrowIf(_disposed, this);
}
