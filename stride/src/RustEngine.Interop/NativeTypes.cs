using System.Runtime.InteropServices;
namespace RustEngine.Interop;

[StructLayout(LayoutKind.Sequential)]
public struct ControlInput { public float RotateX; public float RotateY; public float Zoom; public uint Reset; }

[StructLayout(LayoutKind.Sequential)]
public struct EarthRenderState
{
    public float Radius;
    public float AtmosphereRadius;
    public float RotationX;
    public float RotationY;
    public float CloudRotationY;
    public float CameraDistance;
    public float LightX;
    public float LightY;
    public float LightZ;
}

[StructLayout(LayoutKind.Sequential)]
public struct SurfacePatch { public float LatitudeDegrees; public float LongitudeDegrees; public float RadiusDegrees; public float StretchX; public float StretchY; }
