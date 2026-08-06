#include "RustEarthController.hpp"

#include <cassert>

int main() {
    RustEarthController controller;
    const EarthRenderState before = controller.state();
    const EarthRenderState after = controller.update(ControlInput{0.5F, 1.0F, 1.0F, 0U}, 0.1F);

    assert(after.rotation_x > before.rotation_x);
    assert(after.rotation_y > before.rotation_y);
    assert(after.camera_distance < before.camera_distance);
    assert(controller.surface_patches().size() == 1U);
    return 0;
}
