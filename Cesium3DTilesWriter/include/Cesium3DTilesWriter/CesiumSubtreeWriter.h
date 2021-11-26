#pragma once

#include "Cesium3DTilesWriter/Library.h"

#include <CesiumJsonWriter/ExtensionWriterContext.h>

// forward declarations
namespace Cesium3DTiles {
struct Subtree;
}

namespace Cesium3DTilesWriter {

/**
 * @brief The result of writing a subtree with
 * {@link CesiumSubtreeWriterWriter::writeSubtree}.
 */
struct CESIUM3DTILESWRITER_API SubtreeWriterResult {
  /**
   * @brief The final generated std::vector<std::byte> of the subtree JSON.
   */
  std::vector<std::byte> subtreeBytes;

  /**
   * @brief Errors, if any, that occurred during the write process.
   */
  std::vector<std::string> errors;

  /**
   * @brief Warnings, if any, that occurred during the write process.
   */
  std::vector<std::string> warnings;
};

/**
 * @brief Options for how to write a subtree.
 */
struct CESIUM3DTILESWRITER_API WriteSubtreeOptions {
  /**
   * @brief If the subtree JSON should be pretty printed.
   */
  bool prettyPrint = false;
};

/**
 * @brief Writes subtrees.
 */
class CESIUM3DTILESWRITER_API CesiumSubtreeWriter {
public:
  /**
   * @brief Constructs a new instance.
   */
  CesiumSubtreeWriter();

  /**
   * @brief Gets the context used to control how tileset extensions are written.
   */
  CesiumJsonWriter::ExtensionWriterContext& getExtensions();

  /**
   * @brief Gets the context used to control how tileset extensions are written.
   */
  const CesiumJsonWriter::ExtensionWriterContext& getExtensions() const;

  /**
   * @brief Serializes the provided subtree object into a byte vector using the
   * provided flags to convert.
   *
   * @param subtree The subtree.
   * @param options Options for how to write the subtree.
   * @return The result of writing the subtree.
   */
  SubtreeWriterResult writeSubtree(
      const Cesium3DTiles::Subtree& subtree,
      const WriteSubtreeOptions& options = WriteSubtreeOptions()) const;

private:
  CesiumJsonWriter::ExtensionWriterContext _context;
};

} // namespace Cesium3DTilesWriter
