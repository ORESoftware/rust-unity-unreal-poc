#include "RustEngineClient.hpp"
#include <cassert>
int main() {
    RustEarth::RustEngineClient client;
    const auto before = client.State();
    const auto after = client.Tick(ControlInput{1,0,0,0}, 0.1F);
    assert(after.rotation_x > before.rotation_x);
    assert(client.SurfacePatches().size() == 1U);
    return 0;
}
