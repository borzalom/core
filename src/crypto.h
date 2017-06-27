// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef XBY_CRYPTO_H
#define XBY_CRYPTO_H

#include "util.h"
#include <openssl/ecdsa.h>
#include <stdint.h>

class CryptoKey {

protected:
    EC_KEY* ec_key;
    BIGNUM *priv;
    EC_POINT *pub;
    BN_CTX *ctx;
    const EC_GROUP *group;

    bool KeyReady;

public:
  CryptoKey();
  ~CryptoKey();
  bool new_keypair(const uint8_t *priv_bytes);
  bool new_pubkey(const uint8_t *pub_bytes, size_t pub_len);
  void NewKey();
  bool GetPrivKey( HexString *privkey );
}; 



#endif // XBY_CRYPTO_H
