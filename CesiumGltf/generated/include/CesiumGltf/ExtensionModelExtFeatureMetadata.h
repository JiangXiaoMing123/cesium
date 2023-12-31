// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/ExtensionExtFeatureMetadataFeatureTable.h"
#include "CesiumGltf/ExtensionExtFeatureMetadataFeatureTexture.h"
#include "CesiumGltf/ExtensionExtFeatureMetadataSchema.h"
#include "CesiumGltf/ExtensionExtFeatureMetadataStatistics.h"
#include "CesiumGltf/Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <optional>
#include <string>
#include <unordered_map>

namespace CesiumGltf {
/**
 * @brief glTF extension that assigns metadata to features in a model.
 */
struct CESIUMGLTF_API ExtensionModelExtFeatureMetadata final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName =
      "ExtensionModelExtFeatureMetadata";
  static inline constexpr const char* ExtensionName = "EXT_feature_metadata";

  /**
   * @brief An object defining classes and enums.
   */
  std::optional<CesiumGltf::ExtensionExtFeatureMetadataSchema> schema;

  /**
   * @brief The URI (or IRI) of the external schema file.
   */
  std::optional<std::string> schemaUri;

  /**
   * @brief An object containing statistics about features.
   */
  std::optional<CesiumGltf::ExtensionExtFeatureMetadataStatistics> statistics;

  /**
   * @brief A dictionary, where each key is a feature table ID and each value is
   * an object defining the feature table.
   */
  std::unordered_map<
      std::string,
      CesiumGltf::ExtensionExtFeatureMetadataFeatureTable>
      featureTables;

  /**
   * @brief A dictionary, where each key is a feature texture ID and each value
   * is an object defining the feature texture.
   */
  std::unordered_map<
      std::string,
      CesiumGltf::ExtensionExtFeatureMetadataFeatureTexture>
      featureTextures;
};
} // namespace CesiumGltf
