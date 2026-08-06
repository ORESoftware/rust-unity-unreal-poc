#include "RustEngineSession.hpp"
#include <cassert>
int main() {
    RustEngineSession session;
    const auto before = session.state();
    const auto after = session.tick(ControlInput{0,1,0,0}, 0.1F);
    assert(after.rotation_y > before.rotation_y);
    return 0;
}
