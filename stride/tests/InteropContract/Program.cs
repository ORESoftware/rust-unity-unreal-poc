using System.Runtime.InteropServices;
using RustEngine.Interop;

if (Marshal.SizeOf<ControlInput>() != 16) throw new InvalidOperationException("ControlInput ABI size changed.");
if (Marshal.SizeOf<EarthRenderState>() != 36) throw new InvalidOperationException("EarthRenderState ABI size changed.");
if (Marshal.SizeOf<SurfacePatch>() != 20) throw new InvalidOperationException("SurfacePatch ABI size changed.");
if (Marshal.OffsetOf<EarthRenderState>(nameof(EarthRenderState.CameraDistance)).ToInt32() != 20) throw new InvalidOperationException("CameraDistance offset changed.");
Console.WriteLine("Rust engine Stride ABI contract is valid.");
