// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/ExtensionExtMeshGpuInstancing.h>
#include <CesiumJsonReader/DictionaryJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
}

namespace CesiumGltfReader {
class ExtensionExtMeshGpuInstancingJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler,
      public CesiumJsonReader::IExtensionJsonHandler {
public:
  using ValueType = CesiumGltf::ExtensionExtMeshGpuInstancing;

  static inline constexpr const char* ExtensionName = "EXT_mesh_gpu_instancing";

  ExtensionExtMeshGpuInstancingJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(
      IJsonHandler* pParentHandler,
      CesiumGltf::ExtensionExtMeshGpuInstancing* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

  virtual void reset(
      IJsonHandler* pParentHandler,
      CesiumUtility::ExtensibleObject& o,
      const std::string_view& extensionName) override;

  virtual IJsonHandler& getHandler() override { return *this; }

protected:
  IJsonHandler* readObjectKeyExtensionExtMeshGpuInstancing(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::ExtensionExtMeshGpuInstancing& o);

private:
  CesiumGltf::ExtensionExtMeshGpuInstancing* _pObject = nullptr;
  CesiumJsonReader::DictionaryJsonHandler<
      int32_t,
      CesiumJsonReader::IntegerJsonHandler<int32_t>>
      _attributes;
};
} // namespace CesiumGltfReader
