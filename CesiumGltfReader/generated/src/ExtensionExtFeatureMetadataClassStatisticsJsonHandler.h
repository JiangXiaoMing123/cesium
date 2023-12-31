// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "ExtensionExtFeatureMetadataPropertyStatisticsJsonHandler.h"

#include <CesiumGltf/ExtensionExtFeatureMetadataClassStatistics.h>
#include <CesiumJsonReader/DictionaryJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
}

namespace CesiumGltfReader {
class ExtensionExtFeatureMetadataClassStatisticsJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = CesiumGltf::ExtensionExtFeatureMetadataClassStatistics;

  ExtensionExtFeatureMetadataClassStatisticsJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(
      IJsonHandler* pParentHandler,
      CesiumGltf::ExtensionExtFeatureMetadataClassStatistics* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyExtensionExtFeatureMetadataClassStatistics(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::ExtensionExtFeatureMetadataClassStatistics& o);

private:
  CesiumGltf::ExtensionExtFeatureMetadataClassStatistics* _pObject = nullptr;
  CesiumJsonReader::IntegerJsonHandler<int64_t> _count;
  CesiumJsonReader::DictionaryJsonHandler<
      CesiumGltf::ExtensionExtFeatureMetadataPropertyStatistics,
      ExtensionExtFeatureMetadataPropertyStatisticsJsonHandler>
      _properties;
};
} // namespace CesiumGltfReader
