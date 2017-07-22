// XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
// Copyright (c) 2017, Zoltan Szabo. All rights reserved.
// Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
// Licensed under GNU General Public License Version 3 or later (the "GPL")

#ifndef XBY_THREADS_H
#define XBY_THREADS_H

void StartThreads();

template <typename Callable>
void ThreadWrapper(std::string ThreadName, Callable ThreadFunc);

void ThreadSyncXbridge();
void ThreadServerDICOM();
void ThreadModuleHeartBeat();
void ThreadCron_1s();

#endif // XBY_THREADS_H
