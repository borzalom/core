// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#include "crypto.h"
#include "error.h"
#include "util.h"
#include <iostream>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <stdint.h>


CryptoKey::CryptoKey() {
    KeyReady = false;    
    // secp256k1 sizes(bits): PrivKey = 279; PubKey = 65; Signature = 72;
    ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (ec_key == NULL)
        throw cryptokey_error("CryptoKey::CryptoKey() : EC_KEY_new_by_curve_name(NID_secp256k1) failed");
    priv = BN_new();
}

void CryptoKey::NewKey()
{
    if (!EC_KEY_generate_key(ec_key))
        throw cryptokey_error("CryptoKey::NewKey() : EC_KEY_generate_key(ec_key) failed");
    KeyReady = true;
}


bool CryptoKey::GetPrivKey( HexString *privkey ) {
     
    const BIGNUM* prikey_bn = BN_new();
    prikey_bn = EC_KEY_get0_private_key(ec_key);
    int privkey_len =  BN_num_bytes(prikey_bn);
    int full_size = (sizeof(unsigned char) * privkey_len);
    unsigned char * privkey_buf[32];
    
    BN_bn2bin(prikey_bn, (unsigned char *)privkey_buf);
        
    privkey->Allocate(privkey_len);
    
    privkey->SetBin((char*)privkey_buf);
    return true;

       
}

CryptoKey::~CryptoKey() {
	
	BN_clear_free(priv);
//	EC_POINT_free(pub);
//	BN_CTX_end(ctx);
//   BN_CTX_free(ctx);
	EC_KEY_free(ec_key);
	
}


bool CryptoKey::new_keypair(const uint8_t *priv_bytes) {
       
    BN_bin2bn(priv_bytes, 32, priv);
    EC_KEY_set_private_key(ec_key, priv);
    
    ctx = BN_CTX_new();
    BN_CTX_start(ctx);

    group = EC_KEY_get0_group(ec_key);
    pub = EC_POINT_new(group);
    EC_POINT_mul(group, pub, priv, NULL, NULL, ctx);
    EC_KEY_set_public_key(ec_key, pub);

    return true;

}

bool CryptoKey::new_pubkey(const uint8_t *pub_bytes, size_t pub_len) {
    
    const uint8_t *pub_bytes_copy;
    
    pub_bytes_copy = pub_bytes;
    o2i_ECPublicKey(&ec_key, &pub_bytes_copy, pub_len);

    return true;
}