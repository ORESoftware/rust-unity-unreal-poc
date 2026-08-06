use bevy::prelude::*;
use rust_engine::{ControlInput, Engine};

#[derive(Component)]
struct Earth;

#[derive(Component)]
struct MainCamera;

struct Simulation(Engine);

fn main() {
    App::new()
        .add_plugins(DefaultPlugins)
        .insert_non_send_resource(Simulation(Engine::new()))
        .add_systems(Startup, setup)
        .add_systems(Update, (drive_simulation, apply_render_state).chain())
        .run();
}

fn setup(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
    simulation: NonSend<Simulation>,
) {
    let state = simulation.0.render_state();

    commands.spawn((
        Mesh3d(meshes.add(Sphere::new(state.radius))),
        MeshMaterial3d(materials.add(StandardMaterial {
            base_color: Color::srgb(0.04, 0.24, 0.72),
            metallic: 0.08,
            perceptual_roughness: 0.62,
            ..default()
        })),
        Transform::from_rotation(Quat::from_euler(
            EulerRot::XYZ,
            state.rotation_x,
            state.rotation_y,
            0.0,
        )),
        Earth,
    ));

    for patch in simulation.0.surface_patches() {
        let latitude = patch.lat_degrees.to_radians();
        let longitude = patch.lon_degrees.to_radians();
        let radius = state.radius * 1.006;
        let position = Vec3::new(
            radius * latitude.cos() * longitude.cos(),
            radius * latitude.sin(),
            radius * latitude.cos() * longitude.sin(),
        );
        let size = (patch.radius_degrees / 180.0).clamp(0.035, 0.22);

        commands.spawn((
            Mesh3d(meshes.add(Sphere::new(size))),
            MeshMaterial3d(materials.add(StandardMaterial {
                base_color: Color::srgb(0.08, 0.62, 0.22),
                perceptual_roughness: 0.8,
                ..default()
            })),
            Transform::from_translation(position).with_scale(Vec3::new(
                patch.stretch_x,
                patch.stretch_y,
                0.25,
            )),
        ));
    }

    commands.spawn((
        PointLight {
            intensity: 3_000_000.0,
            shadows_enabled: true,
            ..default()
        },
        Transform::from_xyz(-4.0, 5.0, 4.0),
    ));

    commands.spawn((
        Camera3d::default(),
        Transform::from_xyz(0.0, 0.0, state.camera_distance).looking_at(Vec3::ZERO, Vec3::Y),
        MainCamera,
    ));
}

fn drive_simulation(
    time: Res<Time>,
    keyboard: Res<ButtonInput<KeyCode>>,
    mut simulation: NonSendMut<Simulation>,
) {
    let axis = |negative: KeyCode, positive: KeyCode| {
        f32::from(keyboard.pressed(positive)) - f32::from(keyboard.pressed(negative))
    };

    simulation.0.set_input(ControlInput {
        rotate_x: axis(KeyCode::ArrowDown, KeyCode::ArrowUp),
        rotate_y: axis(KeyCode::ArrowLeft, KeyCode::ArrowRight),
        zoom: axis(KeyCode::PageDown, KeyCode::PageUp),
        reset: u32::from(keyboard.just_pressed(KeyCode::KeyR)),
    });
    simulation.0.tick(time.delta_secs());
}

fn apply_render_state(
    simulation: NonSend<Simulation>,
    mut earth: Single<&mut Transform, With<Earth>>,
    mut camera: Single<&mut Transform, (With<MainCamera>, Without<Earth>)>,
) {
    let state = simulation.0.render_state();
    earth.rotation = Quat::from_euler(EulerRot::XYZ, state.rotation_x, state.rotation_y, 0.0);
    camera.translation = Vec3::new(0.0, 0.0, state.camera_distance);
    camera.look_at(Vec3::ZERO, Vec3::Y);
}
