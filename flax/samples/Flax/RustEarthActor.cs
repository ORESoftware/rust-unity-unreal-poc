using FlaxEngine;
using RustEngine.Interop;

public sealed class RustEarthActor : Script
{
    private RustEngineSession? _session;

    public override void OnStart()
    {
        _session = new RustEngineSession();
        Apply(_session.State);
    }

    public override void OnUpdate()
    {
        if (_session is null) return;
        var input = new ControlInput
        {
            RotateX = Axis(KeyboardKeys.Down, KeyboardKeys.Up),
            RotateY = Axis(KeyboardKeys.Left, KeyboardKeys.Right),
            Zoom = Axis(KeyboardKeys.PageDown, KeyboardKeys.PageUp),
            Reset = Input.GetKeyDown(KeyboardKeys.R) ? 1U : 0U,
        };
        Apply(_session.Tick(input, Time.DeltaTime));
    }

    public override void OnDestroy()
    {
        _session?.Dispose();
        _session = null;
    }

    private static float Axis(KeyboardKeys negative, KeyboardKeys positive) =>
        (Input.GetKey(positive) ? 1f : 0f) - (Input.GetKey(negative) ? 1f : 0f);

    private void Apply(EarthRenderState state)
    {
        Actor.LocalOrientation = Quaternion.EulerAngles(
            Mathf.RadiansToDegrees * state.RotationX,
            Mathf.RadiansToDegrees * state.RotationY,
            0f);
    }
}
