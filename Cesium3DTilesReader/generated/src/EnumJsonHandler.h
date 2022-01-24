// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "EnumValueJsonHandler.h"

#include <Cesium3DTiles/Enum.h>
#include <CesiumJsonReader/ArrayJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumJsonReader {
class ExtensionReaderContext;
}

namespace Cesium3DTilesReader {
class EnumJsonHandler : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = Cesium3DTiles::Enum;

  EnumJsonHandler(
      const CesiumJsonReader::ExtensionReaderContext& context) noexcept;
  void reset(IJsonHandler* pParentHandler, Cesium3DTiles::Enum* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyEnum(
      const std::string& objectType,
      const std::string_view& str,
      Cesium3DTiles::Enum& o);

private:
  Cesium3DTiles::Enum* _pObject = nullptr;
  CesiumJsonReader::StringJsonHandler _name;
  CesiumJsonReader::StringJsonHandler _description;
  CesiumJsonReader::StringJsonHandler _valueType;
  CesiumJsonReader::
      ArrayJsonHandler<Cesium3DTiles::EnumValue, EnumValueJsonHandler>
          _values;
};
} // namespace Cesium3DTilesReader