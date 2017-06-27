// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

// REDFAT Database - REdundant Distributed FAult Tolerant Database

#include "leveldb/db.h"
#include "redfat.h"

REDFAT redfat;

REDFAT::REDFAT() {		
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options,"xtrabytes-test.db",&db);
}