// qrvmone: Fast Quantum Resistant Virtual Machine implementation
// Copyright 2022 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <ethash/keccak.hpp>
#include <qrvmc/hex.hpp>
#include <qrvmc/qrvmc.hpp>
#include <cstring>

namespace qrvmone
{
using qrvmc::address;
using qrvmc::bytes;
using qrvmc::bytes32;
using qrvmc::bytes_view;
using namespace qrvmc::literals;

/// Default type for 256-bit hash.
///
/// Better than ethash::hash256 because has some additional handy constructors.
using hash256 = bytes32;

/// Computes Keccak hash out of input bytes (wrapper of ethash::keccak256).
inline hash256 keccak256(bytes_view data) noexcept
{
    const auto eh = ethash::keccak256(data.data(), data.size());
    hash256 h;
    std::memcpy(h.bytes, eh.bytes, sizeof(h));  // TODO: Use std::bit_cast.
    return h;
}
}  // namespace qrvmone

std::ostream& operator<<(std::ostream& out, const qrvmone::address& a);
std::ostream& operator<<(std::ostream& out, const qrvmone::bytes32& b);
