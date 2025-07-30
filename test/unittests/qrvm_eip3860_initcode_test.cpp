// qrvmone: Fast Quantum Resistant Virtual Machine implementation
// Copyright 2022 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

/// This file contains QRVM unit tests for EIP-3860 "Limit and meter initcode"
/// https://eips.ethereum.org/EIPS/eip-3860

#include "qrvm_fixture.hpp"

using namespace qrvmc::literals;
using qrvmone::test::qrvm;

inline constexpr size_t initcode_size_limit = 0xc000;

TEST_P(qrvm, create_initcode_limit)
{
    host.call_result.create_address = "Q02"_address;
    for (const auto& c : {create().input(0, calldataload(0)) + ret_top(),
             create2().input(0, calldataload(0)) + ret_top()})
    {
        for (const auto r : {QRVMC_SHANGHAI})
        {
            rev = r;
            for (const auto s : {initcode_size_limit, initcode_size_limit + 1})
            {
                execute(c, qrvmc::uint256be{s});
                if (rev >= QRVMC_SHANGHAI && s > initcode_size_limit)
                {
                    EXPECT_STATUS(QRVMC_OUT_OF_GAS);
                }
                else
                {
                    EXPECT_OUTPUT_INT(2);
                }
            }
        }
    }
}

TEST_P(qrvm, create_initcode_gas_cost)
{
    rev = QRVMC_SHANGHAI;
    const auto code = create().input(0, calldataload(0));
    execute(44300, code, qrvmc::uint256be{initcode_size_limit});
    EXPECT_GAS_USED(QRVMC_SUCCESS, 44300);
    execute(44299, code, qrvmc::uint256be{initcode_size_limit});
    EXPECT_STATUS(QRVMC_OUT_OF_GAS);
}

TEST_P(qrvm, create2_initcode_gas_cost)
{
    rev = QRVMC_SHANGHAI;
    const auto code = create2().input(0, calldataload(0));
    execute(53519, code, qrvmc::uint256be{initcode_size_limit});
    EXPECT_GAS_USED(QRVMC_SUCCESS, 53519);
    execute(53518, code, qrvmc::uint256be{initcode_size_limit});
    EXPECT_STATUS(QRVMC_OUT_OF_GAS);
}
