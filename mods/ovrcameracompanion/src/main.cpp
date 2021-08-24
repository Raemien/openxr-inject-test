#include "config.hpp"

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "GlobalNamespace/OVRPlugin_LayerFlags.hpp"
#include "GlobalNamespace/OVRLayerAttribute.hpp"
#include "GlobalNamespace/OVRCameraRig.hpp"
#include "GlobalNamespace/OVROverlay.hpp"
#include "GlobalNamespace/OVRDisplay.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/XR/XRNode.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Color.hpp"


static ModInfo modInfo;


Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

MAKE_HOOK_MATCH(OVRCameraRig_Awake, &GlobalNamespace::OVRCameraRig::Awake, void, GlobalNamespace::OVRCameraRig* instance)
{
    OVRCameraRig_Awake(instance);
    auto& modcfg = getConfig().config;

    UnityEngine::GameObject* camobj = instance->get_gameObject();
    GlobalNamespace::OVROverlay* syslayer = camobj->AddComponent<GlobalNamespace::OVROverlay*>();

    syslayer->isProtectedContent = true;
    syslayer->isAlphaPremultiplied = false;
    static auto ovr_CreatePassthroughLayer = reinterpret_cast<function_ptr_t<bool, GlobalNamespace::OVROverlay*, GlobalNamespace::OVRPlugin::LayerFlags>>(il2cpp_functions::resolve_icall("OculusDisplayProvider::CreateLayer"));
    (void)ovr_CreatePassthroughLayer(syslayer, (int)512); // system layer for guardian/passthrough

    instance->get_rightEyeCamera()->set_backgroundColor(UnityEngine::Color(0.0f, 0.0f, 0.0f, 0.0f));
    instance->get_leftEyeCamera()->set_backgroundColor(UnityEngine::Color(0.0f, 0.0f, 0.0f, 0.0f));
}


extern "C" void setup(ModInfo& info) {

    info.id = "OVRPassthrough";
    info.version = "0.1.1";
    modInfo = info;
    getConfig().Load();
}

extern "C" void load() {
    if(!LoadConfig()) SetupConfig();
    il2cpp_functions::Init();
    INSTALL_HOOK(getLogger(), OVRCameraRig_Awake);
}
