using Microsoft.Win32.SafeHandles;

namespace RustEngine.Interop;

internal sealed class RustEngineHandle : SafeHandleZeroOrMinusOneIsInvalid
{
    private RustEngineHandle() : base(ownsHandle: true) { }

    internal static RustEngineHandle Create()
    {
        var pointer = RustEngineNative.Create();
        if (pointer == 0)
        {
            throw new InvalidOperationException("rust_engine_create returned null.");
        }

        var handle = new RustEngineHandle();
        handle.SetHandle(pointer);
        return handle;
    }

    protected override bool ReleaseHandle()
    {
        RustEngineNative.Destroy(handle);
        return true;
    }
}
