#include "Cesium3DTiles/Cesium3DTilesWriter.h"

#include <catch2/catch.hpp>

using namespace Cesium3DTiles;
using namespace CesiumUtility;

namespace {
std::string removeWhitespace(const std::string& string) {
  std::string compact = string;
  compact.erase(
      remove_if(compact.begin(), compact.end(), isspace),
      compact.end());
  return compact;
}
} // namespace

TEST_CASE("Writes tileset JSON") {
  using namespace std::string_literals;

  Content content1;
  content1.uri = "1.gltf";

  BoundingVolume contentBoundingVolume;
  contentBoundingVolume.sphere = {30, 31, 32, 33};

  Content content2;
  content2.uri = "2.gltf";
  content2.boundingVolume = contentBoundingVolume;

  Tile tile1, tile2, tile3;
  tile1.boundingVolume.box = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  tile1.geometricError = 15.0;
  tile1.refine = Tile::Refine::ADD;
  tile1.content = content1;

  tile2.boundingVolume.box = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};
  tile2.viewerRequestVolume = BoundingVolume();
  tile2.viewerRequestVolume
      ->box = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41};
  tile2.geometricError = 25.0;
  tile2.content = content2;

  tile3.boundingVolume.box = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  tile3.geometricError = 35.0;
  tile3.children = {tile1, tile2};
  tile3.refine = Tile::Refine::REPLACE;

  Asset asset;
  asset.version = "1.0";
  asset.tilesetVersion = "1.2.3";

  Properties property1, property2;
  property1.maximum = 10.0;
  property1.minimum = 0.0;
  property2.maximum = 5.0;
  property2.minimum = 1.0;

  std::unordered_map<std::string, Properties> properties = {
      {"property1", property1},
      {"property2", property2}};

  Tileset tileset;
  tileset.asset = asset;
  tileset.root = tile3;
  tileset.properties = properties;
  tileset.geometricError = 45.0;

  Cesium3DTilesWriter writer;
  TilesetWriterResult result = writer.writeTileset(tileset);
  const auto asBytes = result.tilesetBytes;

  REQUIRE(result.errors.empty());
  REQUIRE(result.warnings.empty());

  std::string expectedString = removeWhitespace(R"(
    {
      "asset": {
        "version": "1.0",
        "tilesetVersion": "1.2.3"
      },
      "properties": {
        "property2": {
          "maximum": 5.0,
          "minimum": 1.0
        },
        "property1": {
          "maximum": 10.0,
          "minimum": 0.0
        }
      },
      "geometricError": 45.0,
      "root": {
        "boundingVolume": {
          "box": [20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0]
        },
        "geometricError": 35.0,
        "refine": "REPLACE",
        "transform": [1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0],
        "children": [
          {
            "boundingVolume": {
              "box": [0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0]
            },
            "geometricError": 15.0,
            "refine": "ADD",
            "transform": [1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0],
            "content": {
              "uri": "1.gltf"
            }
          },
          {
            "boundingVolume": {
              "box": [10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0]
            },
            "viewerRequestVolume": {
              "box": [30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0]
            },
            "geometricError": 25.0,
            "transform": [1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0],
            "content": {
              "boundingVolume": {
                "sphere": [30.0, 31.0, 32.0, 33.0]
              },
              "uri": "2.gltf"
            }
          }
        ]
      }
    }
  )");

  const std::string extractedString(
      reinterpret_cast<const char*>(asBytes.data()),
      asBytes.size());

  REQUIRE(expectedString == extractedString);
}

TEST_CASE("Writes tileset JSON with extras") {
  using namespace std::string_literals;

  Tile root;
  root.boundingVolume.box = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  root.geometricError = 15.0;
  root.refine = Tile::Refine::ADD;
  root.extras = {{"year", 2001}, {"resolution", 0.5}};

  Asset asset;
  asset.version = "1.0";

  Tileset tileset;
  tileset.asset = asset;
  tileset.root = root;
  tileset.geometricError = 45.0;

  std::map<std::string, JsonValue> nestedExtras = {
      {"type", "tileset"},
      {"authors", {"A", "B", "C"}},
      {"year", 2000}};

  tileset.extras = {{"final", true}, {"info", nestedExtras}};

  Cesium3DTilesWriter writer;
  TilesetWriterResult result = writer.writeTileset(tileset);
  const auto asBytes = result.tilesetBytes;

  CHECK(result.errors.empty());
  CHECK(result.warnings.empty());

  std::string expectedString = removeWhitespace(R"(
    {
      "asset": {
        "version": "1.0"
      },
      "geometricError": 45.0,
      "root": {
        "boundingVolume": {
          "box": [0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0]
        },
        "geometricError": 15.0,
        "refine": "ADD",
        "transform": [1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0],
        "extras": {
          "resolution": 0.5,
          "year": 2001
        }
      },
      "extras": {
        "final": true,
        "info": {
          "authors": ["A","B","C"],
          "type": "tileset",
          "year": 2000
        }
      }
    }
  )");

  const std::string extractedString(
      reinterpret_cast<const char*>(asBytes.data()),
      asBytes.size());

  REQUIRE(expectedString == extractedString);
}