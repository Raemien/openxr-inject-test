#include "modloader/shared/modloader.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

struct Config_t
{
    bool enabled = true;
    bool useXRUnderlay = true;
    int renderOrder = -1;
    float opacity = 0.8f;
}
Settings;

void SetupConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    auto& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("enabled", Settings.enabled, allocator);
    getConfig().config.AddMember("useXRUnderlay", Settings.useXRUnderlay, allocator);
    getConfig().config.AddMember("renderOrder", Settings.renderOrder, allocator);
    getConfig().config.AddMember("opacity", Settings.opacity, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    getConfig().Load();
    if(!getConfig().config.HasMember("enabled") || !getConfig().config["enabled"].IsBool()) return false;
    if(!getConfig().config.HasMember("useXRUnderlay") || !getConfig().config["useXRUnderlay"].IsBool()) return false;
    if(!getConfig().config.HasMember("renderOrder") || !getConfig().config["renderOrder"].IsInt()) return false;
    if(!getConfig().config.HasMember("opacity") || !getConfig().config["opacity"].IsFloat()) return false;
    return true;
}
