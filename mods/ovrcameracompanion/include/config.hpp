#include "modloader/shared/modloader.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

struct Config_t
{
    bool enabled;
    bool useXRUnderlay;
    int renderOrder;
    float opacity;
};

bool LoadConfig();
void SetupConfig();
