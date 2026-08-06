using System.Runtime.InteropServices;

namespace RustEngine.Interop;

internal static partial class RustEngineNative
{
    private const string Library = "rust_engine";

    [LibraryImport(Library, EntryPoint = "rust_engine_create")]
    internal static partial nint Create();

    [LibraryImport(Library, EntryPoint = "rust_engine_destroy")]
    internal static partial void Destroy(nint engine);

    [LibraryImport(Library, EntryPoint = "rust_engine_set_control_input")]
    internal static partial void SetControlInput(nint engine, ControlInput input);

    [LibraryImport(Library, EntryPoint = "rust_engine_tick")]
    internal static partial void Tick(nint engine, float deltaSeconds);

    [LibraryImport(Library, EntryPoint = "rust_engine_render_state")]
    internal static partial EarthRenderState RenderState(nint engine);

    [LibraryImport(Library, EntryPoint = "rust_engine_surface_patches")]
    internal static partial SurfacePatchView SurfacePatches(nint engine);
}
