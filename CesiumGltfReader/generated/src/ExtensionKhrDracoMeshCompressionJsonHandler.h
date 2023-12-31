// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/ExtensionKhrDracoMeshCompression.h>
#include <CesiumJsonReader/DictionaryJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
}

namespace CesiumGltfReader {
class ExtensionKhrDracoMeshCompressionJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler,
      public CesiumJsonReader::IExtensionJsonHandler {
public:
  using ValueType = CesiumGltf::ExtensionKhrDracoMeshCompression;

  static inline constexpr const char* ExtensionName =
      "KHR_draco_mesh_compression";

  ExtensionKhrDracoMeshCompressionJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(
      IJsonHandler* pParentHandler,
      CesiumGltf::ExtensionKhrDracoMeshCompression* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

  virtual void reset(
      IJsonHandler* pParentHandler,
      CesiumUtility::ExtensibleObject& o,
      const std::string_view& extensionName) override;

  virtual IJsonHandler& getHandler() override { return *this; }

protected:
  IJsonHandler* readObjectKeyExtensionKhrDracoMeshCompression(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::ExtensionKhrDracoMeshCompression& o);

private:
  CesiumGltf::ExtensionKhrDracoMeshCompression* _pObject = nullptr;
  CesiumJsonReader::IntegerJsonHandler<int32_t> _bufferView;
  CesiumJsonReader::DictionaryJsonHandler<
      int32_t,
      CesiumJsonReader::IntegerJsonHandler<int32_t>>
      _attributes;
};
} // namespace CesiumGltfReader
