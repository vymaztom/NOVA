/*
 * Generic Interrupt Controller: Distributor (GICD)
 *
 * Copyright (C) 2019 Udo Steinberg, BedRock Systems, Inc.
 *
 * This file is part of the NOVA microhypervisor.
 *
 * NOVA is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NOVA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License version 2 for more details.
 */

#pragma once

#include "coresight.hpp"
#include "intid.hpp"
#include "memory.hpp"
#include "spinlock.hpp"
#include "types.hpp"

class Gicd : private Coresight, private Intid
{
    private:
        static Spinlock lock;

        enum Group
        {
            GROUP0 = 0x00000000,
            GROUP1 = 0xffffffff,
        };

        enum class Register32
        {
            CTLR        = 0x0000,   // v2 v3 rw Control Register
            TYPER       = 0x0004,   // v2 v3 ro Type Register
            IIDR        = 0x0008,   // v2 v3 ro Implementer Identification Register
            STATUSR     = 0x0010,   // -- v3 rw Error Reporting Status Register
            SETSPI_NSR  = 0x0040,   // -- v3 wo Set SPI Register, Non-Secure
            CLRSPI_NSR  = 0x0048,   // -- v3 wo Clr SPI Register, Non-Secure
            SETSPI_SR   = 0x0050,   // -- v3 wo Set SPI Register, Secure
            CLRSPI_SR   = 0x0058,   // -- v3 wo Clr SPI Register, Secure
            SGIR        = 0x0f00,   // v2 -- wo Software Generated Interrupt Register
        };

        enum class Array32
        {
            IGROUPR     = 0x0080,   // v2 v3 rw Interrupt Group Registers
            ISENABLER   = 0x0100,   // v2 v3 rw Interrupt Set-Enable Registers
            ICENABLER   = 0x0180,   // v2 v3 rw Interrupt Clr-Enable Registers
            ISPENDR     = 0x0200,   // v2 v3 rw Interrupt Set-Pending Registers
            ICPENDR     = 0x0280,   // v2 v3 rw Interrupt Clr-Pending Registers
            ISACTIVER   = 0x0300,   // v2 v3 rw Interrupt Set-Active Registers
            ICACTIVER   = 0x0380,   // v2 v3 rw Interrupt Clr-Active Registers
            IPRIORITYR  = 0x0400,   // v2 v3 rw Interrupt Priority Registers
            ITARGETSR   = 0x0800,   // v2 -- rw Interrupt Processor Targets Registers
            ICFGR       = 0x0c00,   // v2 v3 rw Interrupt Configuration Registers
            IGRPMODR    = 0x0d00,   // -- v3 -- Interrupt Group Modifier Registers
            NSACR       = 0x0e00,   // v2 v3 rw Non-Secure Access Control Registers
            CPENDSGIR   = 0x0f10,   // v2 -- rw SGI Clr-Pending Registers
            SPENDSGIR   = 0x0f20,   // v2 -- rw SGI Set-Pending Registers
            IGROUPRE    = 0x1000,   // -- v3 rw Extended Interrupt Group Registers
            ISENABLERE  = 0x1200,   // -- v3 rw Extended Interrupt Set-Enable Registers
            ICENABLERE  = 0x1400,   // -- v3 rw Extended Interrupt Clr-Enable Registers
            ISPENDRE    = 0x1600,   // -- v3 rw Extended Interrupt Set-Pending Registers
            ICPENDRE    = 0x1800,   // -- v3 rw Extended Interrupt Clr-Pending Registers
            ISACTIVERE  = 0x1a00,   // -- v3 rw Extended Interrupt Set-Active Registers
            ICACTIVERE  = 0x1c00,   // -- v3 rw Extended Interrupt Clr-Active Registers
            IPRIORITYRE = 0x2000,   // -- v3 rw Extended Interrupt Priority Registers
            ICFGRE      = 0x3000,   // -- v3 rw Extended Interrupt Configuration Registers
            IGRPMODRE   = 0x3400,   // -- v3 rw Extended Interrupt Group Modifier Registers
            NSACRE      = 0x3600,   // -- v3 rw Extended Non-Secure Access Control Registers
        };

        enum class Array64
        {
            IROUTER     = 0x6000,   // -- v3 rw Interrupt Routing Registers
            IROUTERE    = 0x8000,   // -- v3 rw Extended Interrupt Routing Registers
        };

        static inline auto read  (Register32 r)             { return *reinterpret_cast<uint32 volatile *>(DEV_GLOBL_GICD + static_cast<mword>(r)); }
        static inline auto read  (Array32 r, unsigned n)    { return *reinterpret_cast<uint32 volatile *>(DEV_GLOBL_GICD + static_cast<mword>(r) + n * sizeof (uint32)); }
        static inline auto read  (Array64 r, unsigned n)    { return *reinterpret_cast<uint64 volatile *>(DEV_GLOBL_GICD + static_cast<mword>(r) + n * sizeof (uint64)); }

        static inline void write (Register32 r,          uint32 v) { *reinterpret_cast<uint32 volatile *>(DEV_GLOBL_GICD + static_cast<mword>(r)) = v; }
        static inline void write (Array32 r, unsigned n, uint32 v) { *reinterpret_cast<uint32 volatile *>(DEV_GLOBL_GICD + static_cast<mword>(r) + n * sizeof (uint32)) = v; }
        static inline void write (Array64 r, unsigned n, uint64 v) { *reinterpret_cast<uint64 volatile *>(DEV_GLOBL_GICD + static_cast<mword>(r) + n * sizeof (uint64)) = v; }

        static void wait_rwp();

    public:
        static unsigned arch;
        static unsigned ints;
        static Group group;

        static void init();

        static void conf (unsigned, bool = true, unsigned = 0);

        static void mask (unsigned, bool);

        static void send_sgi (unsigned, unsigned);
};
