using System.Runtime.InteropServices;
using RustEngine.Interop;

static void Require(bool condition, string message)
{
    if (!condition) throw new InvalidOperationException(message);
}

Require(Marshal.SizeOf<ControlInput>() == 16, "ControlInput ABI size changed.");
Require(Marshal.SizeOf<EarthRenderState>() == 36, "EarthRenderState ABI size changed.");
Require(Marshal.SizeOf<SurfacePatch>() == 20, "SurfacePatch ABI size changed.");
Require(Marshal.OffsetOf<ControlInput>(nameof(ControlInput.Reset)).ToInt32() == 12, "ControlInput.Reset offset changed.");
Require(Marshal.OffsetOf<EarthRenderState>(nameof(EarthRenderState.CameraDistance)).ToInt32() == 20, "CameraDistance offset changed.");
Console.WriteLine("Rust engine managed ABI contract is valid.");
