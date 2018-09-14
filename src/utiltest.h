// Copyright (c) 2016 The Zeroinsight developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "wallet/wallet.h"
#include "zeroinsight/JoinSplit.hpp"
#include "zeroinsight/Note.hpp"
#include "zeroinsight/NoteEncryption.hpp"

CWalletTx GetValidReceive(ZCJoinSplit& params,
                          const libzeroinsight::SpendingKey& sk, CAmount value,
                          bool randomInputs);
libzeroinsight::Note GetNote(ZCJoinSplit& params,
                       const libzeroinsight::SpendingKey& sk,
                       const CTransaction& tx, size_t js, size_t n);
CWalletTx GetValidSpend(ZCJoinSplit& params,
                        const libzeroinsight::SpendingKey& sk,
                        const libzeroinsight::Note& note, CAmount value);
