using RustEngine.Interop;
using Stride.Core.Mathematics;
using Stride.Engine;
using Stride.Input;

public sealed class RustEarthSyncScript : SyncScript
{
    private RustEngineSession? _session;

    public override void Start()
    {
        _session = new RustEngineSession();
        Apply(_session.State);
    }

    public override void Update()
    {
        if (_session is null) return;
        var input = new ControlInput
        {
            RotateX = Axis(Keys.Down, Keys.Up),
            RotateY = Axis(Keys.Left, Keys.Right),
            Zoom = Axis(Keys.PageDown, Keys.PageUp),
            Reset = Input.IsKeyPressed(Keys.R) ? 1U : 0U,
        };
        var elapsed = (float)Game.UpdateTime.Elapsed.TotalSeconds;
        Apply(_session.Tick(input, elapsed));
    }

    public override void Cancel()
    {
        _session?.Dispose();
        _session = null;
        base.Cancel();
    }

    private float Axis(Keys negative, Keys positive) =>
        (Input.IsKeyDown(positive) ? 1f : 0f) - (Input.IsKeyDown(negative) ? 1f : 0f);

    private void Apply(EarthRenderState state)
    {
        Entity.Transform.RotationEulerXYZ = new Vector3(state.RotationX, state.RotationY, 0f);
    }
}
