// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "ExtensionExtFeatureMetadataTextureAccessorJsonHandler.h"

#include <CesiumGltf/ExtensionExtFeatureMetadataFeatureTexture.h>
#include <CesiumJsonReader/DictionaryJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
}

namespace CesiumGltfReader {
class ExtensionExtFeatureMetadataFeatureTextureJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = CesiumGltf::ExtensionExtFeatureMetadataFeatureTexture;

  ExtensionExtFeatureMetadataFeatureTextureJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(
      IJsonHandler* pParentHandler,
      CesiumGltf::ExtensionExtFeatureMetadataFeatureTexture* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyExtensionExtFeatureMetadataFeatureTexture(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::ExtensionExtFeatureMetadataFeatureTexture& o);

private:
  CesiumGltf::ExtensionExtFeatureMetadataFeatureTexture* _pObject = nullptr;
  CesiumJsonReader::StringJsonHandler _classProperty;
  CesiumJsonReader::DictionaryJsonHandler<
      CesiumGltf::ExtensionExtFeatureMetadataTextureAccessor,
      ExtensionExtFeatureMetadataTextureAccessorJsonHandler>
      _properties;
};
} // namespace CesiumGltfReader
