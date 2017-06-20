// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef XBY_GENESIS_H
#define XBY_GENESIS_H

class Genesis_Block
{
 public:
   void Get_Genesis_Block();
   static const char* GetSTaTiCbyID( const char* id );
   static const char* GetSTaTiCbyPubKey( const char* id );
};


#endif // XBY_BLOCK_H
