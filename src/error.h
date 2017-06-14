// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef XBY_ERROR_H
#define XBY_ERROR_H

#include <stdexcept>

class cryptokey_error : public std::runtime_error
{
public:
    explicit cryptokey_error(const std::string& str) : std::runtime_error(str) {}
};

#endif // XBY_ERROR_H