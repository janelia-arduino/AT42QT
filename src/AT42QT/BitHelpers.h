// ----------------------------------------------------------------------------
// BitHelpers.h
//
// Portable bit and field helpers that avoid compiler-defined bitfield layout.
// ----------------------------------------------------------------------------

#ifndef AT42QT_BIT_HELPERS_H
#define AT42QT_BIT_HELPERS_H

#include <stdint.h>

namespace at42qt {
namespace bits {

template <typename Storage, unsigned Pos> struct Bit {
  static constexpr Storage mask = static_cast<Storage>(Storage(1) << Pos);

  static bool get(Storage reg) { return (reg & mask) != 0; }

  static void set(Storage &reg, bool on) {
    reg = on ? static_cast<Storage>(reg | mask)
             : static_cast<Storage>(reg & static_cast<Storage>(~mask));
  }
};

template <typename Storage, unsigned Pos, unsigned Width> struct Field {
  static constexpr Storage mask =
      static_cast<Storage>(((uint64_t(1) << Width) - 1u) << Pos);

  static Storage get(Storage reg) {
    return static_cast<Storage>((reg & mask) >> Pos);
  }

  static void set(Storage &reg, Storage value) {
    reg = static_cast<Storage>((reg & static_cast<Storage>(~mask)) |
                               ((value << Pos) & mask));
  }
};

template <typename Storage, unsigned Pos> class BitRef {
public:
  explicit BitRef(Storage &reg) : reg_ptr_(&reg) {}

  BitRef &operator=(bool on) {
    Bit<Storage, Pos>::set(*reg_ptr_, on);
    return *this;
  }

  operator bool() const { return Bit<Storage, Pos>::get(*reg_ptr_); }

private:
  Storage *reg_ptr_;
};

template <typename Storage, unsigned Pos, unsigned Width> class FieldRef {
public:
  explicit FieldRef(Storage &reg) : reg_ptr_(&reg) {}

  FieldRef &operator=(Storage value) {
    Field<Storage, Pos, Width>::set(*reg_ptr_, value);
    return *this;
  }

  operator Storage() const {
    return Field<Storage, Pos, Width>::get(*reg_ptr_);
  }

private:
  Storage *reg_ptr_;
};

} // namespace bits
} // namespace at42qt

#endif
