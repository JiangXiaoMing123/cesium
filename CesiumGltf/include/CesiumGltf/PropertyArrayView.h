#pragma once

#include "CesiumGltf/PropertyType.h"
#include "getOffsetFromOffsetsBuffer.h"

#include <CesiumUtility/SpanHelper.h>

#include <gsl/span>
#include <mpark/variant.hpp>

#include <cassert>
#include <cstddef>
#include <vector>

namespace CesiumGltf {
/**
 * @brief A view on an array element of a {@link PropertyTableProperty}
 * or {@link PropertyTextureProperty}.
 *
 * Provides utility to retrieve the data stored in the array of
 * elements via the array index operator.
 */
template <typename ElementType> class PropertyArrayView {
public:
  /**
   * @brief Constructs an empty array view.
   */
  PropertyArrayView() noexcept;

  /**
   * @brief Constructs an array view from a buffer.
   *
   * @param buffer The buffer containing the values.
   */
  PropertyArrayView(const gsl::span<const std::byte>& buffer) noexcept;

  /**
   * @brief Constructs an array view from a vector of values. This is mainly
   * used when the values cannot be viewed in place.
   *
   * @param values The vector containing the values.
   */
  PropertyArrayView(std::vector<ElementType>&& values) noexcept;

  const ElementType& operator[](int64_t index) const noexcept;

  int64_t size() const noexcept;

  bool operator==(const PropertyArrayView<ElementType>& other) const noexcept;
  bool operator!=(const PropertyArrayView<ElementType>& other) const noexcept;

private:
  using ArrayType =
      mpark::variant<gsl::span<const ElementType>, std::vector<ElementType>>;
  ArrayType _values;
};

template <typename ElementType>
PropertyArrayView<ElementType>::PropertyArrayView() noexcept : _values{} {}

template <typename ElementType>
PropertyArrayView<ElementType>::PropertyArrayView(
    const gsl::span<const std::byte>& buffer) noexcept
    : _values{CesiumUtility::reintepretCastSpan<const ElementType>(buffer)} {}

template <typename ElementType>
PropertyArrayView<ElementType>::PropertyArrayView(
    std::vector<ElementType>&& values) noexcept
    : _values{std::move(values)} {}

template <typename ElementType>
const ElementType&
PropertyArrayView<ElementType>::operator[](int64_t index) const noexcept {
  switch (_values.index()) {
  case 0:
    return (*mpark::get_if<0>(&_values))[index];
  case 1:
    return (*mpark::get_if<1>(&_values))[index];
  default:
    abort();
  }
}

template <typename ElementType>
int64_t PropertyArrayView<ElementType>::size() const noexcept {
  switch (_values.index()) {
  case 0:
    return mpark::get_if<0>(&_values)->size();
  case 1:
    return mpark::get_if<1>(&_values)->size();
  default:
    abort();
  }
}

template <typename ElementType>
bool PropertyArrayView<ElementType>::operator==(
    const PropertyArrayView<ElementType>& other) const noexcept {
  if (this->size() != other.size()) {
    return false;
  }

  for (int64_t i = 0; i < size(); i++) {
    if ((*this)[i] != other[i]) {
      return false;
    }
  }

  return true;
}

template <typename ElementType>
bool PropertyArrayView<ElementType>::operator!=(
    const PropertyArrayView<ElementType>& other) const noexcept {
  return !operator==(other);
}

template <> class PropertyArrayView<bool> {
public:
  /**
   * @brief Constructs an empty array view.
   */
  PropertyArrayView() noexcept;

  /**
   * @brief Constructs an array view from a buffer.
   *
   * @param buffer The buffer containing the values.
   * @param bitOffset The offset into the buffer where the values actually
   * begin.
   * @param size The number of values in the array.
   */
  PropertyArrayView(
      const gsl::span<const std::byte>& buffer,
      int64_t bitOffset,
      int64_t size) noexcept;

  bool operator[](int64_t index) const noexcept;

  int64_t size() const noexcept;

  bool operator==(const PropertyArrayView<bool>& other) const noexcept;
  bool operator!=(const PropertyArrayView<bool>& other) const noexcept;

private:
  gsl::span<const std::byte> _values;
  int64_t _bitOffset;
  int64_t _size;
};

template <> class PropertyArrayView<std::string_view> {
public:
  /**
   * @brief Constructs an empty array view.
   */
  PropertyArrayView() noexcept;

  /**
   * @brief Constructs an array view from buffers and their information.
   *
   * @param values The buffer containing the values.
   * @param stringOffsets The buffer containing the string offsets.
   * @param stringOffsetType The component type of the string offsets.
   * @param size The number of values in the array.
   */
  PropertyArrayView(
      const gsl::span<const std::byte>& values,
      const gsl::span<const std::byte>& stringOffsets,
      PropertyComponentType stringOffsetType,
      int64_t size) noexcept;

  std::string_view operator[](int64_t index) const noexcept;

  int64_t size() const noexcept;

  bool
  operator==(const PropertyArrayView<std::string_view>& other) const noexcept;
  bool
  operator!=(const PropertyArrayView<std::string_view>& other) const noexcept;

private:
  gsl::span<const std::byte> _values;
  gsl::span<const std::byte> _stringOffsets;
  PropertyComponentType _stringOffsetType;
  int64_t _size;
};
} // namespace CesiumGltf
