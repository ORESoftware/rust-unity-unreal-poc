using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace MonoGameRustEarth;

internal sealed class RustEarthGame : Game
{
    private readonly GraphicsDeviceManager _graphics;
    private readonly RustEngineSession _simulation = new();
    private SpriteBatch? _spriteBatch;
    private Texture2D? _earth;
    private Texture2D? _pixel;
    private EarthRenderState _state;

    internal RustEarthGame()
    {
        _graphics = new GraphicsDeviceManager(this)
        {
            PreferredBackBufferWidth = 1280,
            PreferredBackBufferHeight = 720,
        };
        IsMouseVisible = true;
        Window.Title = "Rust-owned Earth — MonoGame";
        _state = _simulation.State;
    }

    protected override void LoadContent()
    {
        _spriteBatch = new SpriteBatch(GraphicsDevice);
        _pixel = new Texture2D(GraphicsDevice, 1, 1);
        _pixel.SetData([Color.White]);
        _earth = CreateEarthTexture(512);
    }

    protected override void Update(GameTime gameTime)
    {
        var keyboard = Keyboard.GetState();
        if (keyboard.IsKeyDown(Keys.Escape)) Exit();

        static float Axis(KeyboardState state, Keys negative, Keys positive) =>
            (state.IsKeyDown(positive) ? 1f : 0f) - (state.IsKeyDown(negative) ? 1f : 0f);

        _state = _simulation.Tick(
            new ControlInput
            {
                RotateX = Axis(keyboard, Keys.Down, Keys.Up),
                RotateY = Axis(keyboard, Keys.Left, Keys.Right),
                Zoom = Axis(keyboard, Keys.PageDown, Keys.PageUp),
                Reset = keyboard.IsKeyDown(Keys.R) ? 1U : 0U,
            },
            (float)gameTime.ElapsedGameTime.TotalSeconds);

        base.Update(gameTime);
    }

    protected override void Draw(GameTime gameTime)
    {
        GraphicsDevice.Clear(new Color(5, 10, 24));
        if (_spriteBatch is null || _earth is null || _pixel is null) return;

        var viewport = GraphicsDevice.Viewport;
        var zoom = Math.Clamp(4.2f / _state.CameraDistance, 0.45f, 1.8f);
        var size = (int)(Math.Min(viewport.Width, viewport.Height) * 0.62f * zoom);
        var center = new Vector2(viewport.Width / 2f, viewport.Height / 2f);
        var rotation = _state.RotationY;

        _spriteBatch.Begin(samplerState: SamplerState.LinearClamp);
        _spriteBatch.Draw(
            _earth,
            center,
            sourceRectangle: null,
            color: Color.White,
            rotation: rotation,
            origin: new Vector2(_earth.Width / 2f, _earth.Height / 2f),
            scale: (float)size / _earth.Width,
            effects: SpriteEffects.None,
            layerDepth: 0f);

        _spriteBatch.Draw(_pixel, new Rectangle(24, 24, 360, 4), Color.LimeGreen);
        _spriteBatch.End();
        base.Draw(gameTime);
    }

    protected override void Dispose(bool disposing)
    {
        if (disposing) _simulation.Dispose();
        base.Dispose(disposing);
    }

    private Texture2D CreateEarthTexture(int size)
    {
        var texture = new Texture2D(GraphicsDevice, size, size);
        var pixels = new Color[size * size];
        var radius = size * 0.49f;
        var center = (size - 1) / 2f;
        for (var y = 0; y < size; y++)
        for (var x = 0; x < size; x++)
        {
            var dx = x - center;
            var dy = y - center;
            var distance = MathF.Sqrt(dx * dx + dy * dy);
            if (distance > radius) pixels[y * size + x] = Color.Transparent;
            else
            {
                var lighting = 0.45f + 0.55f * MathF.Max(0f, 1f - distance / radius);
                pixels[y * size + x] = new Color(0.04f * lighting, 0.25f * lighting, 0.82f * lighting, 1f);
            }
        }
        texture.SetData(pixels);
        return texture;
    }
}
