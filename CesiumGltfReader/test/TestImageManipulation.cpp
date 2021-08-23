#include "CesiumGltf/ImageCesium.h"
#include "CesiumGltf/ImageManipulation.h"
#include <algorithm>
#include <catch2/catch.hpp>

using namespace CesiumGltf;

TEST_CASE("ImageManipulation::unsafeBlitImage entire image") {
  size_t width = 10;
  size_t height = 10;
  size_t imagePixels = width * height;
  size_t bufferPixels = 2;
  size_t bytesPerPixel = 2;

  std::vector<std::byte> target(
      (imagePixels + bufferPixels) * bytesPerPixel,
      std::byte(1));
  std::vector<std::byte> source(imagePixels * bytesPerPixel, std::byte(2));
  ImageManipulation::unsafeBlitImage(
      target.data() + bytesPerPixel,
      width * bytesPerPixel,
      source.data(),
      width * bytesPerPixel,
      width,
      height,
      bytesPerPixel);

  // Verify we haven't overflowed the target
  CHECK(target[0] == std::byte(1));
  CHECK(target[1] == std::byte(1));
  CHECK(target[target.size() - 1] == std::byte(1));
  CHECK(target[target.size() - 2] == std::byte(1));

  // Verify we did copy the expected bytes
  for (size_t i = 2; i < target.size() - 2; ++i) {
    CHECK(target[i] == std::byte(2));
  }
}

TEST_CASE("ImageManipulation::unsafeBlitImage subset of target") {
  size_t targetWidth = 10;
  size_t targetHeight = 10;
  size_t targetImagePixels = targetWidth * targetHeight;
  size_t bufferPixels = 2;
  size_t bytesPerPixel = 2;

  size_t sourceWidth = 4;
  size_t sourceHeight = 7;
  size_t sourceImagePixels = sourceWidth * sourceHeight;

  std::vector<std::byte> target(
      (targetImagePixels + bufferPixels) * bytesPerPixel,
      std::byte(1));
  std::vector<std::byte> source(
      sourceImagePixels * bytesPerPixel,
      std::byte(2));
  ImageManipulation::unsafeBlitImage(
      target.data() + bytesPerPixel,
      targetWidth * bytesPerPixel,
      source.data(),
      sourceWidth * bytesPerPixel,
      sourceWidth,
      sourceHeight,
      bytesPerPixel);

  // Verify we haven't overflowed the target
  CHECK(target[0] == std::byte(1));
  CHECK(target[1] == std::byte(1));
  CHECK(target[target.size() - 1] == std::byte(1));
  CHECK(target[target.size() - 2] == std::byte(1));

  // Verify we did copy the expected bytes
  for (size_t j = 0; j < targetHeight; ++j) {
    for (size_t i = 0; i < targetWidth; ++i) {
      std::byte expected =
          i < sourceWidth && j < sourceHeight ? std::byte(2) : std::byte(1);
      CHECK(target[(1 + j * targetWidth + i) * bytesPerPixel] == expected);
      CHECK(target[(1 + j * targetWidth + i) * bytesPerPixel + 1] == expected);
    }
  }
}

TEST_CASE("ImageManipulation::unsafeBlitImage subset of source") {
  size_t targetWidth = 10;
  size_t targetHeight = 10;
  size_t targetImagePixels = targetWidth * targetHeight;
  size_t bufferPixels = 2;
  size_t bytesPerPixel = 2;

  size_t sourceTotalWidth = 12;
  size_t sourceWidth = 4;
  size_t sourceHeight = 7;
  size_t sourceImagePixels = sourceTotalWidth * sourceHeight;

  std::vector<std::byte> target(
      (targetImagePixels + bufferPixels) * bytesPerPixel,
      std::byte(1));
  std::vector<std::byte> source(
      sourceImagePixels * bytesPerPixel,
      std::byte(2));
  ImageManipulation::unsafeBlitImage(
      target.data() + bytesPerPixel,
      targetWidth * bytesPerPixel,
      source.data(),
      sourceTotalWidth * bytesPerPixel,
      sourceWidth,
      sourceHeight,
      bytesPerPixel);

  // Verify we haven't overflowed the target
  CHECK(target[0] == std::byte(1));
  CHECK(target[1] == std::byte(1));
  CHECK(target[target.size() - 1] == std::byte(1));
  CHECK(target[target.size() - 2] == std::byte(1));

  // Verify we did copy the expected bytes
  for (size_t j = 0; j < targetHeight; ++j) {
    for (size_t i = 0; i < targetWidth; ++i) {
      std::byte expected =
          i < sourceWidth && j < sourceHeight ? std::byte(2) : std::byte(1);
      CHECK(target[(1 + j * targetWidth + i) * bytesPerPixel] == expected);
      CHECK(target[(1 + j * targetWidth + i) * bytesPerPixel + 1] == expected);
    }
  }
}

TEST_CASE("ImageManipulation::blitImage") {
  ImageCesium target;
  target.bytesPerChannel = 2;
  target.channels = 4;
  target.width = 15;
  target.height = 9;
  target.pixelData = std::vector<std::byte>(
      target.width * target.height * target.channels * target.bytesPerChannel,
      std::byte(1));

  ImageCesium source;
  source.bytesPerChannel = 2;
  source.channels = 4;
  source.width = 10;
  source.height = 11;
  source.pixelData = std::vector<std::byte>(
      source.width * source.height * source.channels * source.bytesPerChannel,
      std::byte(2));

  PixelRectangle sourceRect;
  sourceRect.x = 1;
  sourceRect.y = 2;
  sourceRect.width = 3;
  sourceRect.height = 4;

  PixelRectangle targetRect;
  targetRect.x = 6;
  targetRect.y = 5;
  targetRect.width = 3;
  targetRect.height = 4;

  auto verifyTargetUnchanged = [&target]() {
    CHECK(std::all_of(
        target.pixelData.begin(),
        target.pixelData.end(),
        [](std::byte b) { return b == std::byte(1); }));
  };

  auto verifySuccessfulCopy = [&target, &targetRect]() {
    for (size_t j = 0; j < target.height; ++j) {
      bool copiedRow =
          j >= targetRect.y && j < targetRect.y + targetRect.height;
      size_t rowOffset =
          j * target.width * target.bytesPerChannel * target.channels;
      for (size_t i = 0; i < target.width; ++i) {
        bool copiedColumn =
            i >= targetRect.x && i < targetRect.x + targetRect.width;
        std::byte expected =
            copiedRow && copiedColumn ? std::byte(2) : std::byte(1);
        size_t offset =
            rowOffset + i * target.bytesPerChannel * target.channels;
        for (size_t k = 0; k < target.channels * target.bytesPerChannel; ++k) {
          CHECK(target.pixelData[offset + k] == expected);
        }
      }
    }
  };

  SECTION("succeeds for non-scaled blit") {
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        true);
    verifySuccessfulCopy();
  }

  SECTION("succeeds for a scaled-up blit") {
    // Resizing is currently only supported for images that use one byte per
    // channel.
    target.bytesPerChannel = 1;
    source.bytesPerChannel = 1;

    targetRect.y = 4;
    targetRect.width = 4;
    targetRect.height = 5;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        true);
    verifySuccessfulCopy();
  }

  SECTION("succeeds for a scaled-down blit") {
    // Resizing is currently only supported for images that use one byte per
    // channel.
    target.bytesPerChannel = 1;
    source.bytesPerChannel = 1;

    targetRect.width = 2;
    targetRect.height = 3;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        true);
    verifySuccessfulCopy();
  }

  SECTION("returns false for mismatched bytesPerChannel") {
    target.bytesPerChannel = 1;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false for mismatched channels") {
    target.channels = 3;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false when target X is outside target image") {
    targetRect.x = 14;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false when target Y is outside target image") {
    targetRect.y = 6;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false when target X is negative") {
    targetRect.x = -1;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false when target Y is negative") {
    targetRect.y = -1;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false when source X is outside source image") {
    sourceRect.x = 9;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false when source Y is outside source image") {
    sourceRect.y = 9;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false when source X is negative") {
    sourceRect.x = -1;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false when source Y is negative") {
    sourceRect.y = -1;
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }

  SECTION("returns false for a too-small target") {
    target.pixelData.resize(10);
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
  }

  SECTION("returns false for a too-small source") {
    source.pixelData.resize(10);
    CHECK(
        ImageManipulation::blitImage(target, targetRect, source, sourceRect) ==
        false);
    verifyTargetUnchanged();
  }
}
