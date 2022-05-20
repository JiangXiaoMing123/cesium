#pragma once

#include <Cesium3DTilesSelection/Exp_TilesetContentLoader.h>
#include <Cesium3DTilesSelection/Exp_SubtreeAvailability.h>
#include <CesiumGeometry/OctreeTileID.h>

namespace Cesium3DTilesSelection {
using ImplicitOctreeBoundingVolume = std::variant<
    CesiumGeospatial::BoundingRegion,
    CesiumGeometry::OrientedBoundingBox>;

class ImplicitOctreeLoader : public TilesetContentLoader {
public:
  template <typename ImplicitBoundingVolumeType>
  ImplicitOctreeLoader(
      const std::string& baseUrl,
      const std::string& contentUrlTemplate,
      const std::string& subtreeUrlTemplate,
      uint32_t subtreeLevels,
      uint32_t availableLevels,
      ImplicitBoundingVolumeType&& volume)
      : _baseUrl{baseUrl},
        _contentUrlTemplate{contentUrlTemplate},
        _subtreeUrlTemplate{subtreeUrlTemplate},
        _subtreeLevels{subtreeLevels},
        _availableLevels{availableLevels},
        _boundingVolume{std::forward<ImplicitBoundingVolumeType>(volume)},
        _loadedSubtrees(static_cast<size_t>(std::ceil(
            static_cast<float>(availableLevels) /
            static_cast<float>(subtreeLevels)))) {}

  CesiumAsync::Future<TileLoadResult> loadTileContent(
      TilesetContentLoader& currentLoader,
      const TileContentLoadInfo& loadInfo,
      const std::vector<CesiumAsync::IAssetAccessor::THeader>& requestHeaders)
      override;

  uint32_t getSubtreeLevels() const noexcept;

  uint32_t getAvailableLevels() const noexcept;

  const ImplicitOctreeBoundingVolume& getBoundingVolume() const noexcept;

  void addSubtreeAvailability(
      const CesiumGeometry::OctreeTileID& subtreeID,
      SubtreeAvailability&& subtreeAvailability);

private:
  static std::string resolveUrl(
      const std::string& baseUrl,
      const std::string& urlTemplate,
      const CesiumGeometry::OctreeTileID& octreeID);

  std::string _baseUrl;
  std::string _contentUrlTemplate;
  std::string _subtreeUrlTemplate;
  uint32_t _subtreeLevels;
  uint32_t _availableLevels;
  ImplicitOctreeBoundingVolume _boundingVolume;
  std::vector<std::unordered_map<uint64_t, SubtreeAvailability>>
      _loadedSubtrees;
};
} // namespace Cesium3DTilesSelection