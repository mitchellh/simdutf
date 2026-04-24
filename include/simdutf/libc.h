#ifndef SIMDUTF_LIBC_H
#define SIMDUTF_LIBC_H

#include <cstddef>
#include <cstdlib>
#include <cstring>

#ifndef SIMDUTF_NO_LIBC
  #define SIMDUTF_NO_LIBC 0
#endif

#if SIMDUTF_NO_LIBC
  #ifdef SIMDUTF_LIBC_MEMCPY
extern "C" void *SIMDUTF_LIBC_MEMCPY(void *, const void *, size_t) noexcept;
  #endif
  #ifdef SIMDUTF_LIBC_MEMMOVE
extern "C" void *SIMDUTF_LIBC_MEMMOVE(void *, const void *, size_t) noexcept;
  #endif
  #ifdef SIMDUTF_LIBC_MEMSET
extern "C" void *SIMDUTF_LIBC_MEMSET(void *, int, size_t) noexcept;
  #endif
  #ifdef SIMDUTF_LIBC_MEMCMP
extern "C" int SIMDUTF_LIBC_MEMCMP(const void *, const void *, size_t) noexcept;
  #endif
  #ifdef SIMDUTF_LIBC_STRLEN
extern "C" size_t SIMDUTF_LIBC_STRLEN(const char *) noexcept;
  #endif
  #ifdef SIMDUTF_LIBC_GETENV
extern "C" const char *SIMDUTF_LIBC_GETENV(const char *) noexcept;
  #endif
#endif

namespace simdutf {
namespace internal {

simdutf_really_inline void *builtin_memcpy(void *dst, const void *src,
                                           size_t count) noexcept {
  auto *output = reinterpret_cast<unsigned char *>(dst);
  const auto *input = reinterpret_cast<const unsigned char *>(src);
  for (size_t i = 0; i < count; i++) {
    output[i] = input[i];
  }
  return dst;
}

simdutf_really_inline void *builtin_memmove(void *dst, const void *src,
                                            size_t count) noexcept {
  auto *output = reinterpret_cast<unsigned char *>(dst);
  const auto *input = reinterpret_cast<const unsigned char *>(src);
  if (output == input || count == 0) {
    return dst;
  }
  if (output < input || output >= input + count) {
    for (size_t i = 0; i < count; i++) {
      output[i] = input[i];
    }
    return dst;
  }
  for (size_t i = count; i != 0; i--) {
    output[i - 1] = input[i - 1];
  }
  return dst;
}

simdutf_really_inline void *builtin_memset(void *dst, int value,
                                           size_t count) noexcept {
  auto *output = reinterpret_cast<unsigned char *>(dst);
  const auto byte = static_cast<unsigned char>(value);
  for (size_t i = 0; i < count; i++) {
    output[i] = byte;
  }
  return dst;
}

simdutf_really_inline int builtin_memcmp(const void *lhs, const void *rhs,
                                         size_t count) noexcept {
  const auto *left = reinterpret_cast<volatile const unsigned char *>(lhs);
  const auto *right = reinterpret_cast<volatile const unsigned char *>(rhs);
  for (size_t i = 0; i < count; i++) {
    if (left[i] != right[i]) {
      return int(left[i]) - int(right[i]);
    }
  }
  return 0;
}

simdutf_really_inline size_t builtin_strlen(const char *input) noexcept {
  const auto *bytes = reinterpret_cast<volatile const unsigned char *>(input);
  size_t length = 0;
  while (bytes[length] != '\0') {
    length++;
  }
  return length;
}

simdutf_really_inline void *memcpy(void *dst, const void *src,
                                   size_t count) noexcept {
#if SIMDUTF_NO_LIBC
  #ifdef SIMDUTF_LIBC_MEMCPY
  return SIMDUTF_LIBC_MEMCPY(dst, src, count);
  #else
  return builtin_memcpy(dst, src, count);
  #endif
#else
  return std::memcpy(dst, src, count);
#endif
}

simdutf_really_inline void *memmove(void *dst, const void *src,
                                    size_t count) noexcept {
#if SIMDUTF_NO_LIBC
  #ifdef SIMDUTF_LIBC_MEMMOVE
  return SIMDUTF_LIBC_MEMMOVE(dst, src, count);
  #else
  return builtin_memmove(dst, src, count);
  #endif
#else
  return std::memmove(dst, src, count);
#endif
}

simdutf_really_inline void *memset(void *dst, int value,
                                   size_t count) noexcept {
#if SIMDUTF_NO_LIBC
  #ifdef SIMDUTF_LIBC_MEMSET
  return SIMDUTF_LIBC_MEMSET(dst, value, count);
  #else
  return builtin_memset(dst, value, count);
  #endif
#else
  return std::memset(dst, value, count);
#endif
}

simdutf_really_inline int memcmp(const void *lhs, const void *rhs,
                                 size_t count) noexcept {
#if SIMDUTF_NO_LIBC
  #ifdef SIMDUTF_LIBC_MEMCMP
  return SIMDUTF_LIBC_MEMCMP(lhs, rhs, count);
  #else
  return builtin_memcmp(lhs, rhs, count);
  #endif
#else
  return std::memcmp(lhs, rhs, count);
#endif
}

simdutf_really_inline size_t strlen(const char *input) noexcept {
#if SIMDUTF_NO_LIBC
  #ifdef SIMDUTF_LIBC_STRLEN
  return SIMDUTF_LIBC_STRLEN(input);
  #else
  return builtin_strlen(input);
  #endif
#else
  return std::strlen(input);
#endif
}

simdutf_really_inline const char *getenv(const char *name) noexcept {
#if SIMDUTF_NO_LIBC
  #ifdef SIMDUTF_LIBC_GETENV
  return SIMDUTF_LIBC_GETENV(name);
  #else
  (void)name;
  return nullptr;
  #endif
#else
  SIMDUTF_PUSH_DISABLE_WARNINGS
  SIMDUTF_DISABLE_DEPRECATED_WARNING
  return std::getenv(name);
  SIMDUTF_POP_DISABLE_WARNINGS
#endif
}

} // namespace internal
} // namespace simdutf

#endif // SIMDUTF_LIBC_H
