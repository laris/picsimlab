/*
   Copyright (C) 1998 T. Scott Dattalo

This file is part of the libgpsim library of gpsim

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, see
<http://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#ifndef SRC_P16X6X_H_
#define SRC_P16X6X_H_

#include "14bit-processors.h"
#include "14bit-registers.h"
#include "14bit-tmrs.h"
#include "a2dconverter.h"
#include "comparator.h"
#include "eeprom.h"
#include "intcon.h"
#include "ioports.h"
#include "p16x8x.h"
#include "pic-processor.h"
#include "pie.h"
#include "pir.h"
#include "psp.h"
#include "registers.h"
#include "ssp.h"
#include "uart.h"

class PicPSP_PortRegister;
class PicPSP_TrisRegister;
class PicPortGRegister;
class PicPortRegister;
class PicTrisRegister;
class Processor;

class P16C61 : public P16X8X
{
public:
    P16C61(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16C61();

    PROCESSOR_TYPE isa() override { return _P16C61_; }
    unsigned int program_memory_size() const override { return 0x400; }
    void create() override;

    static Processor *construct(const char *name);
};


//
//   -- Define a class to contain most of the registers/peripherals
//      of a 16x6x device (where the second `x' is >= 3
//

class P16X6X_processor :  public Pic14Bit
{
public:
    P16X6X_processor(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16X6X_processor();

    unsigned int program_memory_size() const override { return 0x800; }
    unsigned int register_memory_size() const override { return 0x100; }

    void create_symbols() override;
    void create_sfr_map() override;
    virtual PIR *get_pir2() { return pir2; }
    virtual PIR *get_pir1() { return pir1; }
    virtual PIR_SET *get_pir_set() { return &pir_set_def; }

    PicPortRegister  *m_portc;
    PicTrisRegister  *m_trisc;

    T1CON   t1con;
    PIR    *pir1;
    PIE     pie1;
    PIR    *pir2;
    PIE     pie2;
    T2CON   t2con;
    PR2     pr2;
    TMR2    tmr2;
    TMRL    tmr1l;
    TMRH    tmr1h;
    CCPCON  ccp1con;
    CCPRL   ccpr1l;
    CCPRH   ccpr1h;
    CCPCON  ccp2con;
    CCPRL   ccpr2l;
    CCPRH   ccpr2h;
    PCON    pcon;
    PIR_SET_1 pir_set_def;
    SSP_MODULE   ssp;
};


/*********************************************************************
 *  class definitions for the 16c6x family of processors
 */


class P16C62 : public  P16X6X_processor
{
public:
    P16C62(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16C62()
    {
    }

    static Processor *construct(const char *name);

    PROCESSOR_TYPE isa() override { return _P16C62_; }
    void create_symbols() override;
    void create_sfr_map() override;

    unsigned int program_memory_size() const override { return 0x800; }
    virtual void create_iopin_map();

    void create() override;

    TMR2_MODULE tmr2_module;
};


class P16C63 : public  P16C62
{
public:
    P16C63(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16C63();

    static Processor *construct(const char *name);
    void create() override;
    void create_sfr_map() override;
    PROCESSOR_TYPE isa() override { return _P16C63_; }
    void create_symbols() override;

    unsigned int program_memory_size() const override { return 0x1000; }

    USART_MODULE usart;
};


class P16C64 : public  P16X6X_processor
{
public:
    P16C64(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16C64();

    PIR *get_pir1() override { return pir1_2_reg; }
    PIR_SET *get_pir_set() override { return &pir_set_2_def; }

    static Processor *construct(const char *name);

    PROCESSOR_TYPE isa() override { return _P16C64_; }

    void create_symbols() override;
    void create_sfr_map() override;

    unsigned int program_memory_size() const override { return 0x800; }
    void create() override;
    virtual void create_iopin_map();

    virtual bool hasSPS() { return false; }

    // XXX
    // This pir1_2, pir2_2 stuff is not particularly pretty.  It would be
    // better to just tell C++ to redefine pir1 and pir2 and PIR1v2 and
    // PIR2v2, but C++ only supports covariance in member function return
    // values.
    PIR1v2 *pir1_2_reg;
    PIR_SET_2 pir_set_2_def;

    PicPSP_PortRegister  *m_portd;
    PicTrisRegister  *m_trisd;
    PicPortRegister  *m_porte;
    PicPSP_TrisRegister  *m_trise;
    PSP               psp;

    TMR2_MODULE tmr2_module;
};


class P16C65 : public  P16C64
{
public:
    P16C65(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16C65();

    PROCESSOR_TYPE isa() override { return _P16C65_; }
    void create_symbols() override;

    unsigned int program_memory_size() const override { return 0x1000; }

    static Processor *construct(const char *name);
    void create() override;
    void create_sfr_map() override;

    USART_MODULE usart;
};


class P16F630 :  public _14bit_processor
{
public:
    P16F630(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16F630();

    virtual PIR *get_pir2() { return nullptr; }
    virtual PIR *get_pir1() { return pir1; }
    virtual PIR_SET *get_pir_set() { return &pir_set_def; }

    PROCESSOR_TYPE isa() override { return _P16F630_; }
    static Processor *construct(const char *name);
    void create(int);
    void create_symbols() override;
    void create_sfr_map() override;
    virtual void create_iopin_map();
    void option_new_bits_6_7(unsigned int bits) override;

    unsigned int program_memory_size() const override { return 0x400; }
    unsigned int register_memory_size() const override { return 0x100; }

    virtual void set_eeprom_wide(EEPROM_WIDE *ep) { eeprom = ep; }
    void create_config_memory() override;
    bool set_config_word(unsigned int address, unsigned int cfg_word) override;

    T1CON   t1con;
    PIR    *pir1;
    PIE     pie1;
    TMRL    tmr1l;
    TMRH    tmr1h;
    OSCCAL  osccal;

    EEPROM_WIDE *e;
    PIR1v3 *pir1_3_reg;

    INTCON_14_PIR    intcon_reg;
    ComparatorModule comparator;
    INT_pin	   int_pin;
    PIR_SET_1        pir_set_def;
    WPU              *m_wpu;
    IOC              *m_ioc;

    PicPortGRegister  *m_porta;
    PicTrisRegister  *m_trisa;

    PicPortRegister *m_portc;
    PicTrisRegister  *m_trisc;
};


class P16F676 :  public P16F630
{
public:
    P16F676(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16F676();

    PROCESSOR_TYPE isa() override { return _P16F676_; }
    static Processor *construct(const char *name);
    virtual void create(int);
    void create_sfr_map() override;

    ANSEL  ansel;
    ADCON0_12F adcon0;
    ADCON1_16F adcon1;
    sfr_register  adresh;
    sfr_register  adresl;
};


class P16F610 :  public _14bit_processor
{
public:
    P16F610(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16F610();

    virtual PIR *get_pir1() { return &pir1; }
    virtual PIR_SET *get_pir_set() { return &pir_set_def; }

    PROCESSOR_TYPE isa() override { return _P16F610_; }
    static Processor *construct(const char *name);
    virtual void create(void);
    void create_symbols() override;
    void create_sfr_map() override;
    virtual void create_iopin_map();
    void option_new_bits_6_7(unsigned int bits) override;

    unsigned int program_memory_size() const override { return 0x400; }
    unsigned int register_memory_size() const override { return 0x100; }

    void create_config_memory() override;
    bool set_config_word(unsigned int address, unsigned int cfg_word) override;

    T1CON            t1con;
    TMRL             tmr1l;
    TMRH             tmr1h;

    PCON             pcon;
    OSCTUNE_2	     osctune;

    INTCON_14_PIR    intcon_reg;
    PIE              pie1;
    PIR1v3           pir1;

    ANSEL            ansel;
    ADCON1_16F       adcon1;
    IOC              ioca;
    PicPortGRegister porta;
    PicTrisRegister  trisa;
    WPU              wpua;
    PicPortRegister  portc;
    PicTrisRegister  trisc;
    ComparatorModule2 comparator;
    VRCON_2          vrcon;
    SR_MODULE        sr_module;

    INT_pin	     int_pin;
    PIR_SET_1        pir_set_def;
};


class P16F616 :  public P16F610
{
public:
    P16F616(const char *_name = nullptr, const char *desc = nullptr);
    virtual ~P16F616();

    PROCESSOR_TYPE isa() override { return _P16F616_; }
    static Processor *construct(const char *name);
    virtual void create(void);
    void create_sfr_map() override;

    unsigned int program_memory_size() const override { return 0x800; }

    T2CON        t2con;
    PR2          pr2;
    TMR2         tmr2;
    ECCPAS       eccp1as;
    PWM1CON      pwm1con;
    CCPCON       ccp1con;
    CCPRL        ccpr1l;
    CCPRH        ccpr1h;

    ADCON0_12F       adcon0;
    sfr_register     adresh;
    sfr_register     adresl;
};

#endif
