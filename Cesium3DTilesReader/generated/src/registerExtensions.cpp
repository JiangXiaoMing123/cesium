// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!

#include "registerExtensions.h"

#include "Extension3dTilesBoundingVolumeS2JsonHandler.h"

#include <Cesium3DTiles/BoundingVolume.h>
#include <CesiumJsonReader/JsonReaderOptions.h>

namespace Cesium3DTilesReader {

void registerExtensions(CesiumJsonReader::JsonReaderOptions& options) {
  (void)options;
  options.registerExtension<
      Cesium3DTiles::BoundingVolume,
      Extension3dTilesBoundingVolumeS2JsonHandler>();
}
} // namespace Cesium3DTilesReader
