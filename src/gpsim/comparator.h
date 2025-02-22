/*
   Copyright (C) 1998-2002 T. Scott Dattalo
   Copyright (C) 2006,2010,2013 Roy R. Rankin

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

#ifndef SRC_COMPARATOR_H_
#define SRC_COMPARATOR_H_

#include <glib.h>

#include <cstdio>
#include <string>

#include "ioports.h"
#include "processor.h"
#include "registers.h"
#include "stimuli.h"
#include "a2dconverter.h"

class ADCON2_TRIG;
class CLC_BASE;
class COG;
class CM1CON0;
class CM2CON0;
class CM2CON1_V2;
class CMCON;
class CMSignalSource;
class CMxCON1;
class CMxSignalSource;
class CTMU;
class ComparatorModule2;
class ECCPAS;
class InterruptSource;
class PIR_SET;
class SR_MODULE;
class T1GCON;
class TMRL;

/***************************************************************************
 *
 * Include file for: Processors with dual comparators and Voltage Refarence
 *
 *
 *
 ***************************************************************************/

#define CFG_MASK 0xf
#define CFG_SHIFT 4

enum compare_inputs
{
    AN0 = 0,
    AN1,
    AN2,
    AN3,
    AN4,
    AN5,
    VREF = 6,	// use reference voltage
    NO_IN = 7,	// no input port
    V06 = 8		// Reference voltage 0.6
};

enum compare_outputs
{
    OUT0 = 0,
    OUT1,
    ZERO = 6,	// register value == 0
    NO_OUT = 7	// no ouput port
};

class VRCON : public sfr_register
{
public:
    VRCON(Processor *pCpu, const char *pName, const char *pDesc);
    ~VRCON();

    enum VRCON_bits
    {
        VR0 = 1 << 0,	// VR0-3 Value selection
        VR1 = 1 << 1,
        VR2 = 1 << 2,
        VR3 = 1 << 3,
        VRSS = 1 << 4,	// Use external references (16f88x)
        VRR = 1 << 5,	// Range select
        VROE = 1 << 6,	// Output Reference to external pin
        VREN = 1 << 7	// Enable Vref
    };

    void put(unsigned int new_value) override;
    virtual void setIOpin(PinModule *);
    virtual double get_Vref();
    void setValidBits(unsigned int mask)
    {
        valid_bits = mask;
    }

    CMCON *_cmcon = nullptr;

protected:
    unsigned int		valid_bits;
    PinModule 		*vr_PinModule = nullptr;
    double 		vr_Vref = 0.0;
    stimulus		*vr_pu = nullptr;
    stimulus		*vr_pd = nullptr;
    stimulus		*vr_06v = nullptr;
    double		vr_Rhigh = 0.0;
    double		vr_Rlow = 0.0;
    double		Vref_high = 0.0;	// usually VDD
    double		Vref_low = 0.0;	// usually VSS
private:
    std::string pin_name;	// original name of pin
};


// VSCOM class with two comparators as per 16f690
//
class VRCON_2 : public sfr_register
{
public:
    VRCON_2(Processor *pCpu, const char *pName, const char *pDesc);
    VRCON_2(const VRCON_2 &) = delete;
    VRCON_2& operator = (const VRCON_2 &) = delete;
    ~VRCON_2();

    enum VRCON_bits
    {
        VR0 = 1 << 0,	// VR0-3 Value selection
        VR1 = 1 << 1,
        VR2 = 1 << 2,
        VR3 = 1 << 3,
        VP6EN = 1 << 4,	// 0.6V reference enable
        VRR = 1 << 5,	// Range select
        C2VREN = 1 << 6,	// Comparator 2 Reference enable
        C1VREN = 1 << 7	// Comparator 1 Reference enable
    };

    void put(unsigned int new_value) override;

    CMCON *_cmcon = nullptr;

protected:
    unsigned int valid_bits = 0;
    PinModule 		*vr_PinModule = nullptr;
    double 		vr_Vref = 0.0;
    stimulus		*vr_pu;
    stimulus		*vr_pd;
    stimulus		*vr_06v;
    char			*pin_name = nullptr;	// original name of pin
};


class CM_stimulus : public stimulus
{
public:
    explicit CM_stimulus(CMCON *arg, const char *n = nullptr,
                double _Vth = 0.0, double _Zth = 1e12
               );
    ~CM_stimulus();

    CMCON *_cmcon;

    void set_nodeVoltage(double v) override;
};

class VREF_stimulus : public stimulus
{
public:
    explicit VREF_stimulus(ComparatorModule2 *arg, const char *n = nullptr,
                double _Vth = 0.0, double _Zth = 1e12
               );
    ~VREF_stimulus();

    ComparatorModule2 *comp;

    void set_nodeVoltage(double v) override;
};


class CMv2_stimulus : public stimulus
{
public:
    explicit CMv2_stimulus(CM2CON1_V2 *arg, const char *n = nullptr,
                  double _Vth = 0.0, double _Zth = 1e12
                 );
    ~CMv2_stimulus();

    CM2CON1_V2 *_cm2con1;

    void set_nodeVoltage(double v) override;
};


class CMCON1 : public sfr_register
{
public:
    CMCON1(Processor *pCpu, const char *pName, const char *pDesc);
    ~CMCON1();

    enum CMCON1_bits
    {
        CMSYNC = 1 << 0,
        T1GSS  = 1 << 1
    };

    void put(unsigned int) override;

    void set_tmrl(TMRL *arg)
    {
        m_tmrl = arg;
    }

private:
    TMRL *m_tmrl = nullptr;
    unsigned int valid_bits;
};


class CMCON : public sfr_register, public apfpin
{
public:
    CMCON(Processor *pCpu, const char *pName, const char *pDesc);
    ~CMCON();

    enum CMCON_bits
    {
        CM0 = 1 << 0,
        CM1 = 1 << 1,
        CM2 = 1 << 2,
        CIS = 1 << 3,
        C1INV = 1 << 4,
        C2INV = 1 << 5,
        C1OUT = 1 << 6,
        C2OUT = 1 << 7,
    };

    virtual void setINpin(int i, PinModule *, const char *an);
    void setIOpin(PinModule *, int i) override;
    virtual void assign_pir_set(PIR_SET *new_pir_set);
    unsigned int get() override;
    virtual void rename_pins(unsigned int)
    {
        puts("CMCON::rename_pins() should not be called");
    }
    void put(unsigned int) override;
    virtual void set_configuration(int comp, int mode, int il1, int ih1, int il2, int ih2, int out);
    virtual double comp_voltage(int ind, int invert);

    void releasePin(int);

    void set_tmrl(TMRL *arg)
    {
        m_tmrl = arg;
    }
    void set_eccpas(ECCPAS *_eccpas)
    {
        m_eccpas = _eccpas;
    }

    VRCON *_vrcon = nullptr;

protected:
    PinModule *cm_input[4];
    PinModule *cm_output[2];
private:
    std::string cm_input_pin[4];
    std::string cm_an[4];
    std::string cm_output_pin[2];
protected:
    CMSignalSource *cm_source[2];
    bool	cm_source_active[2];
    unsigned int m_CMval[2];
    PIR_SET *pir_set = nullptr;
    TMRL *m_tmrl = nullptr;
    CM_stimulus *cm_stimulus[6];
    ECCPAS 	*m_eccpas = nullptr;

    static const int cMaxConfigurations = 8;
    static const int cMaxComparators = 2;

    guint32 m_configuration_bits[cMaxComparators][cMaxConfigurations];
};


class ComparatorModule
{
public:
    explicit ComparatorModule(Processor *);

    void initialize(PIR_SET *pir_set, PinModule *pin_vr0, PinModule *pin_cm0,
                    PinModule *pin_cm1, PinModule *pin_cm2,
                    PinModule *pin_cm3, PinModule *pin_cm4, PinModule *pin_cm5);
    //protected:
    CMCON cmcon;
    CMCON1 cmcon1;
    VRCON vrcon;
};


/*
 * Compare module for 16f88x processors
 */

/*
 * SRCON SR Latch Control Register
 */
class SRCON  : public sfr_register
{
public:
    SRCON(Processor *pCpu, const char *pName, const char *pDesc);
    ~SRCON() {}

    enum SRCON_bits
    {
        FVREN = 1 << 0, // Fixed Voltage Reference Enable
        PULSR = 1 << 2, // Pulse Reset of SR latch
        PULSS = 1 << 3, // Pulse set of SR Latch
        C2REN = 1 << 4, // C2OUT resets SR latch
        C1SEN = 1 << 5, // C1OUT sets SR latch
        SR0   = 1 << 6, // MUX SR Q out and C1OUT
        SR1   = 1 << 7 // MUX SR -Q out and C2OUT
    };
    int writable_bits;
    bool SR_Q;
    bool set;
    bool reset;

    void put(unsigned int new_value) override;
    void set_sr_out(int cm, bool output, bool out_enable, bool output_enable, CMxSignalSource   *cm_source);
};


/*
 * CM2CON1 Comparator control register 1
 */
class CM2CON1 : public sfr_register
{
public:
    CM2CON1(Processor *pCpu, const char *pName, const char *pDesc);
    ~CM2CON1() {}

    enum CM2CON1_bits
    {
        C2SYNC = 1 << 0, //C2 Output sync bit
        T1GSS  = 1 << 1, // Timer1 Gate Source Select bit
        C2RSEL = 1 << 4, // C2 Reference Select bit
        C1RSEL = 1 << 5, // C1 Reference Select bit
        MC2OUT = 1 << 6, // Mirror C2OUT bit
        MC1OUT = 1 << 7 // Mirror C1OUT bit
    };
    int writable_bits;
    CM1CON0 *m_cm1con0;
    CM2CON0 *m_cm2con0;




    void put(unsigned int new_value) override;
    void link_cm12con0(CM1CON0 *_cm1con0, CM2CON0 *_cm2con0);
};


// The following classes are for comparators which have 3 registers
//

class CMxCON0_base : public sfr_register, public apfpin
{
public:
    CMxCON0_base(Processor *pCpu, const char *pName, const char *pDesc,
                 unsigned int _cm, ComparatorModule2 *cmModule);
    ~CMxCON0_base();

    enum
    {
        ON = 1 << 7,
        OE = 1 << 5
    };
    unsigned int get() override;
    virtual double get_Vpos() { return 0.0; }
    virtual double get_Vneg() { return 0.0; }
    void put(unsigned int) override { puts("Help"); }
    //    virtual int get(){return 0;}
    virtual void setBitMask(unsigned int bm) { mValidBits = bm; }
    virtual void setIntSrc(InterruptSource *_IntSrc) { IntSrc = _IntSrc; }
    virtual double CVref() { return 0.0; }
    virtual void notify() {}
    virtual bool output_active() { return value.get() & (ON | OE); }
    virtual double get_hysteresis() { return 0.0; }
    virtual bool output_high() { return false; }
    virtual void set_output(bool /* output */ ) {}
    virtual bool is_on() { return false; }
    virtual bool out_invert() { return true; }
    virtual void releasePin() { cm_source_active = false; }
    virtual bool old_invert() { return true; }

    unsigned int mValidBits = 0;
    unsigned int  old_value = 0;
    PinModule     *cm_input[5];
    PinModule     *cm_output = nullptr;
    CM2CON1       *m_cm2con1 = nullptr;
    SRCON         *m_srcon = nullptr;
    //  PIR_SET       *pir_set;
    InterruptSource *IntSrc = nullptr;
    //  TMRL          *m_tmrl;
    CM_stimulus   *cm_stimulus[2];
    Stimulus_Node *cm_snode[2];
    ECCPAS        *m_eccpas = nullptr;
    unsigned int      cm;	// comparator number
    CMxCON1	    *m_cmxcon1 = nullptr;
    ComparatorModule2 *m_cmModule;
    CMxSignalSource   *cm_source = nullptr;
    bool              cm_source_active = false;
};


class CMxCON0 : public CMxCON0_base
{
public:
    CMxCON0(Processor *pCpu, const char *pName, const char *pDesc, unsigned int x, ComparatorModule2 *);
    ~CMxCON0();

    enum
    {
        CxSYNC 	= 1 << 0,	// Output Synchronous Mode bit
        CxHYS 	= 1 << 1,	// Hysteresis Enable bit
        CxSP   	= 1 << 2,	// Speed/Power Select bit
        CxZLF	= 1 << 3, // Zero Latency Filter Enable bit
        CxPOL 	= 1 << 4,	// Output polarity select bit
        CxOE  	= 1 << 5,	// Output enable
        CxOUT 	= 1 << 6,	// Output bit
        CxON  	= 1 << 7,	// Enable bit
    };

    void put(unsigned int) override;
    double get_Vpos() override;
    double get_Vneg() override;
    void setBitMask(unsigned int bm) override { mValidBits = bm; }
    bool is_on() override { return value.get() & CxON; }
    bool out_invert() override { return value.get() & CxPOL; }
    double get_hysteresis() override;
    void set_output(bool output) override;
    bool output_high() override { return value.get() & CxOUT; }
};

class CMxCON0_PPS : public CMxCON0
{
public:
    CMxCON0_PPS(Processor *pCpu, const char *pName, const char *pDesc, unsigned int x, ComparatorModule2 *);
    ~CMxCON0_PPS(){}

    enum
    {
        CxSYNC 	= 1 << 0,	// Output Synchronous Mode bit
        CxHYS 	= 1 << 1,	// Hysteresis Enable bit
        CxSP   	= 1 << 2,	// Speed/Power Select bit
        CxZLF	= 1 << 3, // Zero Latency Filter Enable bit
        CxPOL 	= 1 << 4,	// Output polarity select bit
        CxOUT 	= 1 << 6,	// Output bit
        CxON  	= 1 << 7,	// Enable bit
    };

    void put(unsigned int) override;
    void setIOpin(PinModule *pin_cm0, int arg) override;
    void new_pin(PinModule *old_pin, PinModule *new_pin);
    void set_output(bool output) override;
};


class CMxCON0_V2 : public CMxCON0_base
{
public:
    CMxCON0_V2(Processor *pCpu, const char *pName, const char *pDesc,
               unsigned int _cm, ComparatorModule2 *cmModule);
    ~CMxCON0_V2();

    enum
    {
        CxCH0 = 1 << 0,	// Channel select bit 0
        CxCH1 = 1 << 1,	// Channel select bit 1
        CxR   = 1 << 2,	// Reference select bit (non-inverting input)
        CxPOL = 1 << 4,	// Output polarity select bit
        CxOE  = 1 << 5,	// Output enable
        CxOUT = 1 << 6,	// Output bit
        CxON  = 1 << 7,	// Enable bit
        NEG	= 0,
        POS	= 1,
    };

    void   put(unsigned int) override;
    double get_Vpos() override;
    double get_Vneg() override;
    void   setBitMask(unsigned int bm) override { mValidBits = bm; }
    bool   is_on() override { return value.get() & CxON; }
    bool   out_invert() override { return value.get() & CxPOL; }
    bool   old_invert() override { return old_value & CxPOL; }
    double get_hysteresis() override;
    void   set_output(bool output) override;
    bool   output_high() override { return value.get() & CxOUT; }

    PinModule		*stimulus_pin[2];
};


class CMxCON1_base : public sfr_register, public apfpin
{
public:
    CMxCON1_base(Processor *pCpu, const char *pName, const char *pDesc, unsigned int _cm, ComparatorModule2 *);
    ~CMxCON1_base();

    enum
    {
        NEG	= 0,
        POS	= 1
    };

    void setBitMask(unsigned int bm) { mValidBits = bm; }
    PinModule *output_pin(int cm = 0) { return cm_output[cm]; }
    void put(unsigned int /* new_value */ ) override {}
    virtual double get_Vpos(unsigned int arg = 0, unsigned int arg2 = 0)
    {
        (void)arg;
        (void)arg2;
        return 0.0;
    }
    virtual double get_Vneg(unsigned int arg = 0, unsigned int arg2 = 0)
    {
        (void)arg;
        (void)arg2;
        return 0.0;
    }
    virtual void setPinStimulus(PinModule *, int);
    virtual void set_INpinNeg(PinModule *pin_cm0, PinModule *pin_cm1,
                              PinModule *pin_cm2 = nullptr,  PinModule *pin_cm3 = nullptr,
                              PinModule *pin_cm4 = nullptr);
    virtual void set_OUTpin(PinModule *pin_cm0, PinModule *pin_cm1 = nullptr);
    virtual void set_INpinPos(PinModule *pin_cm0, PinModule *pin_cm1 = nullptr);
    void setIOpin(PinModule *pin_cm0, int arg) override;
    virtual bool hyst_active(unsigned int /* cm */ ) { return false; }
    virtual void set_vrcon(VRCON * ) {}
    virtual void set_vrcon(VRCON_2 * ) {}
    virtual void tmr_gate(unsigned int, bool) {}

protected:
    unsigned int cm;	// comparator number
    CM_stimulus 	    *cm_stimulus[4];     // stimuli to monitor input pin
    PinModule	    *stimulus_pin[4];    // monitor stimulus loaded on this pin
    PinModule	    *ctmu_stimulus_pin;  // ctmu stimulus pin
    ComparatorModule2 *m_cmModule;
    PinModule 	    *cm_inputNeg[5];
    PinModule 	    *cm_inputPos[2];
    PinModule 	    *cm_output[2];
};


// CMxCON1 only uses 1 or 2 of Negative select bits and 2 Positive select bits
class CMxCON1 : public CMxCON1_base
{
public:
    CMxCON1(Processor *pCpu, const char *pName, const char *pDesc, unsigned int _x, ComparatorModule2 *);
    ~CMxCON1();

    enum
    {
        CxNCH0 	= 1 << 0, //  Negative Input Channel Select bits
        CxNCH1 	= 1 << 1, //  Negative Input Channel Select bits
        CxNCH2 	= 1 << 2, //  Negative Input Channel Select bits
        CxPCH0	= 1 << 3, //  Positive Input Channel Select bits
        CxPCH1	= 1 << 4, //  Positive Input Channel Select bits
        CxPCH2	= 1 << 5, //  Positive Input Channel Select bits
        CxINTN 	= 1 << 6, //  Interrupt on Negative Going Edge Enable bits
        CxINTP 	= 1 << 7, //  Interrupt on Positive Going Edge Enable bits
        CxNMASK = (CxNCH0 | CxNCH1 | CxNCH2),
        CxPMASK = (CxPCH0 | CxPCH1 | CxPCH2)
    };

    enum
    {
	CM_AGND = 0,
	CM_VDD,
	CM_FVR,
	CM_DAC1,
	CM_DAC2,
	CM_DAC3,
	CM_DAC4,
	CM_DAC5,
	CM_PIN,
	CM_UNUSED
    };

    void put(unsigned int new_value) override;
    double get_Vpos(unsigned int arg = 0, unsigned int arg2 = 0) override;
    double get_Vneg(unsigned int arg = 0, unsigned int arg2 = 0) override;
};

/*  two comparators with common CM2CON1 and no COUT register, hyteresis,
    C1, C2 possible T1,3,5  gate, FVR or  DAC for voltage reference,
    used by 18f26k22.
*/
class CM2CON1_V2 : public CMxCON1_base
{
public:
    CM2CON1_V2(Processor *pCpu, const char *pName, const char *pDesc,
               ComparatorModule2 * cmModule);
    ~CM2CON1_V2();

    enum
    {
        C2SYNC = 1 << 0,
        C1SYNC = 1 << 1,
        C2HYS  = 1 << 2,
        C1HYS  = 1 << 3,
        C2RSEL = 1 << 4,
        C1RSEL = 1 << 5,
        MC2OUT = 1 << 6,
        MC1OUT = 1 << 7
    };

    void put(unsigned int new_value) override;
    double get_Vpos(unsigned int cm, unsigned int cmxcon0) override;
    double get_Vneg(unsigned int cm, unsigned int cmxcon0) override;
    bool hyst_active(unsigned int cm) override;
    void tmr_gate(unsigned int cm, bool output) override;
    void set_ctmu_stim(stimulus *_ctmu_stim, CTMU *_ctmu_module);
    void attach_ctmu_stim();
    void detach_ctmu_stim();

private:
    stimulus          *ctmu_stim = nullptr;
    stimulus          *comp_input_cap = nullptr;
    bool	       ctmu_attached = false;
};


/*  two comparators, no hyteresis, cm2con1 controls t1 gate,
    C2 possible T1 gate, vrcon for voltage reference
    used by 16f882.
*/

class CM2CON1_V3 : public CMxCON1_base
{
public:
    CM2CON1_V3(Processor *pCpu, const char *pName, const char *pDesc,
               ComparatorModule2 * cmModule);
    ~CM2CON1_V3();

    enum
    {
        C2SYNC = 1 << 0,
        T1GSS  = 1 << 1,
        C2RSEL = 1 << 4,
        C1RSEL = 1 << 5,
        MC2OUT = 1 << 6,
        MC1OUT = 1 << 7
    };

    void put(unsigned int new_value) override;
    double get_Vpos(unsigned int cm, unsigned int cmxcon0) override;
    double get_Vneg(unsigned int cm, unsigned int cmxcon0) override;
    bool hyst_active(unsigned int /* cm */ ) override { return false; }
    void set_vrcon(VRCON * _vrcon) override { m_vrcon = _vrcon;}
    void set_vrcon(VRCON_2 * _vrcon) override { m_vrcon = (VRCON *)_vrcon;}
    void tmr_gate(unsigned int cm, bool output) override;

protected:
    VRCON 	*m_vrcon = nullptr;
};


/*  two comparators, possible  hyteresis, cm2con1 controls t1 gate,
    C2 possible T1 gate, VRCON for voltage reference
    Like CM2CON1_V3 without C1RSEL, C2RSEL
    used by 16f690, 16f610, 16f616.
*/

class CM2CON1_V4 : public CM2CON1_V3
{
public:
    CM2CON1_V4(Processor *pCpu, const char *pName, const char *pDesc,
               unsigned int _cm, ComparatorModule2 * cmModule);
    CM2CON1_V4(const CM2CON1_V4 &) = delete;
    CM2CON1_V4& operator = (const CM2CON1_V4 &) = delete;
    ~CM2CON1_V4();

    void put(unsigned int new_value) override;
    double get_Vpos(unsigned int cm, unsigned int cmxcon0) override;
    void set_vrcon(VRCON_2 * _vrcon) { m_vrcon = _vrcon; }
    bool hyst_active(unsigned int cm) override;

    enum
    {
        C2SYNC = 1 << 0,
        T1GSS  = 1 << 1,
        C2HYS  = 1 << 2,
        C1HYS  = 1 << 3,
        T1ACS  = 1 << 4,
        MC2OUT = 1 << 6,
        MC1OUT = 1 << 7
    };
protected:
    VRCON_2 	*m_vrcon = nullptr;
    CM_stimulus 	*cm1_cvref;
    CM_stimulus 	*cm1_v06ref;
    CM_stimulus 	*cm2_cvref;
    CM_stimulus 	*cm2_v06ref;
};


class CMOUT : public sfr_register
{
public:
    CMOUT(Processor *pCpu, const char *pName, const char *pDesc)
        : sfr_register(pCpu, pName, pDesc) {}

    void put(unsigned int /* val */ ) override
    {
        // Read only by user
    }
};


// uses CMxCON0, CMxCON1, CMOUT
class ComparatorModule2 : public FVR_ATTACH, public DAC_ATTACH
{
public:
    explicit ComparatorModule2(Processor *);
    ~ComparatorModule2();

    void run_get(unsigned int comp) { cmxcon0[comp]->get(); }

    DATA_SERVER  *get_CM_data_server() { return data_server;}
    void set_DAC_volt(double, unsigned int) override;
    void set_FVR_volt(double, unsigned int) override;
    void set_cmout(unsigned int bit, bool value);
    void set_if(unsigned int);
    void assign_pir_set(PIR_SET *new_pir_set) { pir_set = new_pir_set; }
    void assign_tmr1l(TMRL *t1, TMRL *t3 = nullptr, TMRL *t5 = nullptr)
    {
        tmr1l[0] = t1;
        tmr1l[1] = t3;
        tmr1l[2] = t5;
    }
    void assign_t1gcon(T1GCON *t1g, T1GCON *t3g = nullptr, T1GCON *t5g = nullptr)
    {
        t1gcon[0] = t1g;
        t1gcon[1] = t3g;
        t1gcon[2] = t5g;
    }
    void assign_sr_module(SR_MODULE *_sr_module) { sr_module = _sr_module; }

    void assign_eccpsas(ECCPAS *a1, ECCPAS *a2 = nullptr, ECCPAS *a3 = nullptr)
    {
        eccpas[0] = a1;
        eccpas[1] = a2;
        eccpas[2] = a3;
    }

    CMxCON0_base 	*cmxcon0[4];
    CMxCON1_base 	*cmxcon1[4];
    CMOUT		*cmout = nullptr;

    //protected:
    int 	Pmask[8];
    int 	Nmask[8];
    double 	IN_voltage[8];
    double 	DAC_voltage = 0.0;
    double 	FVR_voltage = 0.0;
    PIR_SET 	*pir_set = nullptr;
    TMRL	*tmr1l[3];
    T1GCON	*t1gcon[3];
    SR_MODULE	*sr_module = nullptr;
    CTMU	*ctmu_module = nullptr;
    ECCPAS 	*eccpas[3];
    ADCON2_TRIG *m_adcon2 = nullptr;
    CLC_BASE	*m_clc[4];
    COG		*p_cog = nullptr;
    Stimulus_Node *node_cvref = nullptr;
    VREF_stimulus *vref_stimulus = nullptr;
    FVRCON	*fvrcon = nullptr;
    DATA_SERVER *data_server = nullptr;

};


#endif // SRC_COMPARATOR_H_
