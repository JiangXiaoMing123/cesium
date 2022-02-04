#pragma once

#include "CesiumGltf/Library.h"

#include <optional>

namespace CesiumGltf {

/**
 * @brief Supported gpu compressed pixel formats.
 */
enum CESIUMGLTF_API GpuCompressedPixelFormat {
  ETC1_RGB,
  ETC2_RGBA,
  BC1_RGB,
  BC3_RGBA,
  BC4_R,
  BC5_RG,
  BC7_RGBA,
  PVRTC1_4_RGB,
  PVRTC1_4_RGBA,
  ASTC_4x4_RGBA,
  PVRTC2_4_RGB,
  PVRTC2_4_RGBA,
  ETC2_EAC_R11,
  ETC2_EAC_RG11
};

/**
 * @brief Represents the status of support for all
 * {@link GpuCompressedPixelFormat}s on a client platform.
 *
 * Clients can use this struct to convey which gpu compressed pixel formats are
 * supported. This can be used to construct a {@link Ktx2TranscodeTargets}.
 *
 * Each entry in the struct is a bool that represents whether the gpu
 * compressed pixel format with the corresponding name is supported.
 */
struct CESIUMGLTF_API SupportedFormats {
  bool ETC1_RGB;
  bool ETC2_RGBA;
  bool BC1_RGB;
  bool BC3_RGBA;
  bool BC4_R;
  bool BC5_RG;
  bool BC7_RGBA;
  bool PVRTC1_4_RGB;
  bool PVRTC1_4_RGBA;
  bool ASTC_4x4_RGBA;
  bool PVRTC2_4_RGB;
  bool PVRTC2_4_RGBA;
  bool ETC2_EAC_R11;
  bool ETC2_EAC_RG11;
};

/**
 * @brief For each possible input transmission format, this struct names
 * the ideal target gpu-compressed pixel format to transcode to.
 *
 * When built with the constructor, these targets can take into account
 * platform-specific support for target formats as reported by the client.
 */
struct CESIUMGLTF_API Ktx2TranscodeTargets {

  /**
   * @brief The gpu pixel compression format to transcode Red ETC1S textures
   * into, if one exists. Otherwise it will be decompressed into raw pixels.
   */
  std::optional<GpuCompressedPixelFormat> ETC1S_R = std::nullopt;

  /**
   * @brief The gpu pixel compression format to transcode Red-Green ETC1S
   * textures into, if one exists. Otherwise it will be decompressed into raw
   * pixels.
   */
  std::optional<GpuCompressedPixelFormat> ETC1S_RG = std::nullopt;

  /**
   * @brief The gpu pixel compression format to transcode Red UASTC textures
   * into, if one exists. Otherwise it will be decompressed into raw pixels.
   */
  std::optional<GpuCompressedPixelFormat> ETC1S_RGB = std::nullopt;

  /**
   * @brief The gpu pixel compression format to transcode Red UASTC textures
   * into, if one exists. Otherwise it will be decompressed into raw pixels.
   */
  std::optional<GpuCompressedPixelFormat> ETC1S_RGBA = std::nullopt;

  /**
   * @brief The gpu pixel compression format to transcode Red UASTC textures
   * into, if one exists. Otherwise it will be decompressed into raw pixels.
   */
  std::optional<GpuCompressedPixelFormat> UASTC_R = std::nullopt;

  /**
   * @brief The gpu pixel compression format to transcode Red-Green UASTC
   * textures into, if one exists. Otherwise it will be decompressed into raw
   * pixels.
   */
  std::optional<GpuCompressedPixelFormat> UASTC_RG = std::nullopt;

  /**
   * @brief The gpu pixel compression format to transcode RGB UASTC textures
   * into, if one exists. Otherwise it will be decompressed into raw pixels.
   */
  std::optional<GpuCompressedPixelFormat> UASTC_RGB = std::nullopt;

  /**
   * @brief The gpu pixel compression format to transcode RGBA UASTC textures
   * into, if one exists. Otherwise it will be decompressed into raw pixels.
   */
  std::optional<GpuCompressedPixelFormat> UASTC_RGBA = std::nullopt;

  Ktx2TranscodeTargets() {}

  /**
   * @brief Determine ideal transcode targets based on a list of supported gpu
   * compressed formats.
   *
   * @param supportedFormats The supported gpu compressed pixel formats.
   */
  Ktx2TranscodeTargets(const SupportedFormats& supportedFormats);
};

} // namespace CesiumGltf