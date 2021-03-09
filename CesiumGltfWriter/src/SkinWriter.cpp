#include "ExtensionWriter.h"
#include "JsonObjectWriter.h"
#include "SkinWriter.h"
void CesiumGltf::writeSkin(
    const std::vector<Skin>& skins,
    CesiumGltf::JsonWriter& jsonWriter) {

    if (skins.empty()) {
        return;
    }

    auto& j = jsonWriter;

    j.Key("skins");
    j.StartArray();
    for (const auto& skin : skins) {
        j.StartObject();
        if (skin.inverseBindMatrices >= 0) {
             j.Key("inverseBindMatrices");
             j.Int(skin.inverseBindMatrices);
        }

        if (skin.skeleton >= 0) {
            j.Key("skeleton");
            j.Int(skin.skeleton);
        }

        j.Key("joints");
        assert(skin.joints.size() >= 1);
        j.StartArray();
        for (const auto& joint : skin.joints) {
            j.Int(joint);
        }
        j.EndArray();

        if (!skin.name.empty()) {
            j.Key("name");
            j.String(skin.name.c_str());
        }
        
        if (!skin.extensions.empty()) {
            writeExtensions(skin.extensions, j);
        }

        if (!skin.extras.empty()) {
            j.Key("extras");
            writeJsonValue(skin.extras, j, false);
        }

        j.EndObject();
    }

    j.EndArray();
}