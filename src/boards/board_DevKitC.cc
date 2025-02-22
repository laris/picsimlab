/* ########################################################################

   PICSimLab - Programmable IC Simulator Laboratory

   ########################################################################

   Copyright (c) : 2021-2023  Luis Claudio Gambôa Lopes <lcgamboa@yahoo.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

// include files
#include "board_DevKitC.h"
#include "../lib/oscilloscope.h"
#include "../lib/picsimlab.h"
#include "../lib/spareparts.h"

/* ids of inputs of input map*/
enum {
    I_ICSP,  // ICSP connector
    I_PWR,   // Power button
    I_RST,   // Reset button
    I_BOOT   // Boot button
};

/* ids of outputs of output map*/
enum {
    O_LON,   // Power LED
    O_RST,   // Reset button
    O_BOOT,  // Boot button
    O_LED    // User LED
};
// return the input ids numbers of names used in input map

enum {
    VDD = 1,
    EN,
    IO36,
    IO39,
    IO34,
    IO35,
    IO32,
    IO33,
    IO25,
    IO26,
    IO27,
    IO14,
    IO12,
    GND,
    IO13,
    IO9,
    IO10,
    IO11,
    VIN,
    IO6,
    IO7,
    IO8,
    IO15,
    IO2,
    IO0,
    IO4,
    IO16,
    IO17,
    IO5,
    IO18,
    IO19,
    GND2,
    IO21,
    IO3,
    IO1,
    IO22,
    IO23
};

static int io2pin(int io) {
    switch (io) {
        case 0:
            return IO0;
            break;
        case 1:
            return IO1;
            break;
        case 2:
            return IO2;
            break;
        case 3:
            return IO3;
            break;
        case 4:
            return IO4;
            break;
        case 5:
            return IO5;
            break;
        case 6:
            return IO6;
            break;
        case 7:
            return IO7;
            break;
        case 8:
            return IO8;
            break;
        case 9:
            return IO9;
            break;
        case 10:
            return IO10;
            break;
        case 11:
            return IO11;
            break;
        case 12:
            return IO12;
            break;
        case 13:
            return IO13;
            break;
        case 14:
            return IO14;
            break;
        case 15:
            return IO15;
            break;
        case 16:
            return IO16;
            break;
        case 17:
            return IO17;
            break;
        case 18:
            return IO18;
            break;
        case 19:
            return IO19;
            break;
        case 21:
            return IO21;
            break;
        case 22:
            return IO22;
            break;
        case 23:
            return IO23;
            break;
        case 25:
            return IO25;
            break;
        case 26:
            return IO26;
            break;
        case 27:
            return IO27;
            break;
        case 32:
            return IO32;
            break;
        case 33:
            return IO33;
            break;
        case 34:
            return IO34;
            break;
        case 35:
            return IO35;
            break;
        case 36:
            return IO36;
            break;
        case 39:
            return IO39;
            break;
    }
    return 0;
}

static const short int pinmap[39] = {
    38,  // number of pins
    -1,  // 1-3V3
    -1,  // 2-EN
    36,  // 3-GPIO36
    39,  // 4-GPIO39
    34,  // 5-GPIO34
    35,  // 6-GPIO35
    32,  // 7-GPIO32
    33,  // 8-GPIO33
    25,  // 9-GPIO25
    26,  // 10-GPIO26
    27,  // 11-GPIO27
    14,  // 12-GPIO14
    12,  // 13-GPIO12
    -1,  // 14-GND
    13,  // 15-GPIO13
    9,   // 16-GPIO9
    10,  // 17-GPIO10
    11,  // 18-GPIO11
    -1,  // 19-VIN
    6,   // 20-GPIO6
    7,   // 21-GPIO7
    8,   // 22-GPIO8
    15,  // 23-GPIO15
    2,   // 24-GPIO2
    0,   // 25-GPIO0
    4,   // 26-GPIO4
    16,  // 27-GPIO16
    17,  // 28-GPIO17
    5,   // 29-GPIO5
    18,  // 30-GPIO18
    19,  // 31-GPIO19
    -1,  // 32-GND
    21,  // 33-GPIO21
    3,   // 34-GPIO3
    1,   // 35-GPIO1
    22,  // 36-GPIO22
    23,  // 37-GPIO23
    -1,  // 38-GND
};

const short int* cboard_DevKitC::GetPinMap(void) {
    return pinmap;
}

unsigned short cboard_DevKitC::GetInputId(char* name) {
    if (strcmp(name, "PG_ICSP") == 0)
        return I_ICSP;
    if (strcmp(name, "SW_PWR") == 0)
        return I_PWR;
    if (strcmp(name, "PB_RST") == 0)
        return I_RST;
    if (strcmp(name, "PB_BOOT") == 0)
        return I_BOOT;

    printf("Error input '%s' don't have a valid id! \n", name);
    return INVALID_ID;
}

// return the output ids numbers of names used in output map

unsigned short cboard_DevKitC::GetOutputId(char* name) {
    if (strcmp(name, "LD_ON") == 0)
        return O_LON;
    if (strcmp(name, "PB_RST") == 0)
        return O_RST;
    if (strcmp(name, "PB_BOOT") == 0)
        return O_BOOT;
    if (strcmp(name, "LD_USER") == 0)
        return O_LED;

    printf("Error output '%s' don't have a valid id! \n", name);
    return INVALID_ID;
}

// Constructor called once on board creation

cboard_DevKitC::cboard_DevKitC(void) {
    char buffer[1024];

    SimType = QEMU_SIM_ESP32;
    p_BOOT = 1;

    Proc = "ESP32";  // default microcontroller if none defined in preferences
    ReadMaps();      // Read input and output board maps

    ConfEnableWifi = 1;
    ConfDisableWdt = 1;
    ConfEnableEthernet = 0;

    application_offset = 0x10000;

    master_i2c[0].scl_pin = 0;
    master_i2c[0].sda_pin = 0;
    master_i2c[1].scl_pin = 0;
    master_i2c[1].sda_pin = 0;

    // HSPI
    master_spi[0].sck_pin = 0;
    master_spi[0].copi_pin = 0;
    master_spi[0].cipo_pin = 0;
    master_spi[0].cs_pin[0] = 0;
    master_spi[0].cs_pin[1] = 0;
    master_spi[0].cs_pin[2] = 0;

    // VSPI
    master_spi[1].sck_pin = 0;
    master_spi[1].copi_pin = 0;
    master_spi[1].cipo_pin = 0;
    master_spi[1].cs_pin[0] = 0;
    master_spi[1].cs_pin[1] = 0;
    master_spi[1].cs_pin[2] = 0;

    master_uart[0].tx_pin = 0;
    master_uart[0].rx_pin = IO3;  // FIXME without this Micropython don´t work
    master_uart[1].tx_pin = 0;
    master_uart[1].rx_pin = 0;
    master_uart[2].tx_pin = 0;
    master_uart[2].rx_pin = 0;

    bitbang_pwm_init(&ledc, this, 16);

    if (PICSimLab.GetWindow()) {
        // label1
        label1 = new CLabel();
        label1->SetFOwner(PICSimLab.GetWindow());
        label1->SetName(lxT("label1_"));
        label1->SetX(13);
        label1->SetY(54 + 20);
        label1->SetWidth(120);
        label1->SetHeight(24);
        label1->SetEnable(1);
        label1->SetVisible(1);
        label1->SetText(lxT("Qemu CPU MIPS"));
        label1->SetAlign(1);
        PICSimLab.GetWindow()->CreateChild(label1);
        // combo1
        combo1 = new CCombo();
        combo1->SetFOwner(PICSimLab.GetWindow());
        combo1->SetName(lxT("combo1_"));
        combo1->SetX(13);
        combo1->SetY(78 + 20);
        combo1->SetWidth(130);
        combo1->SetHeight(24);
        combo1->SetEnable(1);
        combo1->SetVisible(1);
        combo1->SetText(IcountToMipsStr(icount));
        combo1->SetItems(IcountToMipsItens(buffer));
        combo1->SetTag(3);
        combo1->EvOnComboChange = PICSimLab.board_Event;
        PICSimLab.GetWindow()->CreateChild(combo1);
        // button1
        button1 = new CButton();
        button1->SetFOwner(PICSimLab.GetWindow());
        button1->SetName(lxT("button1_"));
        button1->SetX(13);
        button1->SetY(78 + 60);
        button1->SetWidth(130);
        button1->SetHeight(24);
        button1->SetEnable(1);
        button1->SetVisible(1);
        button1->SetText("Config Qemu");
        button1->SetTag(4);
        button1->EvMouseButtonRelease = PICSimLab.board_ButtonEvent;
        PICSimLab.GetWindow()->CreateChild(button1);

        wconfig = new CPWindow();
        wconfig->SetCanDestroy(false);
        wconfig->SetVisible(false);
        wconfig->SetName("window1");  // must be the same as in xml
        Application->ACreateWindow(wconfig);
    } else {
        label1 = NULL;
        combo1 = NULL;
        button1 = NULL;
    }
}

// Destructor called once on board destruction

cboard_DevKitC::~cboard_DevKitC(void) {
    if (PICSimLab.GetWindow()) {
        PICSimLab.GetWindow()->DestroyChild(label1);
        PICSimLab.GetWindow()->DestroyChild(combo1);
        PICSimLab.GetWindow()->DestroyChild(button1);
        wconfig->SetCanDestroy(true);
        wconfig->WDestroy();
    }
    bitbang_pwm_end(&ledc);
}

// Reset board status

void cboard_DevKitC::Reset(void) {
    if (qemu_started != 1) {
        return;
    }
    uint32_t* strap_mode = qemu_picsimlab_get_internals(QEMU_INTERNAL_STRAP);

    if (p_BOOT) {
        *strap_mode = 0x12;  // SPI_FAST_FLASH_BOOT
    } else {
        *strap_mode = 0x0f;  // UART_BOOT(UART0)
    }

    MReset(1);

    PICSimLab.GetStatusBar()->SetField(2, lxT("Serial: ") + lxString::FromAscii(SERIALDEVICE));

    if (use_spare)
        SpareParts.Reset();

    RegisterRemoteControl();
}

void cboard_DevKitC::RegisterRemoteControl(void) {
    output_ids[O_LED]->status = &pins[23].oavalue;
}

// Called ever 1s to refresh status

void cboard_DevKitC::RefreshStatus(void) {
    if (serial_open) {
        PICSimLab.GetStatusBar()->SetField(2, lxT("Serial: ") + lxString::FromAscii(SERIALDEVICE));
    } else {
        PICSimLab.GetStatusBar()->SetField(2, lxT("Serial: Error"));
    }
}

// Called to save board preferences in configuration file

void cboard_DevKitC::WritePreferences(void) {
    // write selected microcontroller of board_x to preferences
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_proc"), Proc);
    // write microcontroller clock to preferences
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_clock"), lxString().Format("%2.1f", PICSimLab.GetClock()));
    // write microcontroller icount to preferences
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_icount"), itoa(icount));

    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_cfgeserial"), itoa(ConfEnableSerial));
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_cfgewifi"), itoa(ConfEnableWifi));
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_cfgdwdt"), itoa(ConfDisableWdt));
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_cfgeeth"), itoa(ConfEnableEthernet));
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_cfguextra"), itoa(use_cmdline_extra));
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_cmdextra"), cmdline_extra);
    PICSimLab.SavePrefs(lxT("ESP32_DevKitC_app_off"), itoa(application_offset));
}

// Called whe configuration file load  preferences

void cboard_DevKitC::ReadPreferences(char* name, char* value) {
    // read microcontroller of preferences
    if (!strcmp(name, "ESP32_DevKitC_proc")) {
        Proc = value;
    }
    // read microcontroller clock
    if (!strcmp(name, "ESP32_DevKitC_clock")) {
        PICSimLab.SetClock(atof(value));
    }
    // read microcontroller icount
    if (!strcmp(name, "ESP32_DevKitC_icount")) {
        icount = atoi(value);
        if (combo1) {
            combo1->SetText(IcountToMipsStr(icount));
        }
    }

    if (!strcmp(name, "ESP32_DevKitC_cfgeserial")) {
        ConfEnableSerial = atoi(value);
    }
    if (!strcmp(name, "ESP32_DevKitC_cfgewifi")) {
        ConfEnableWifi = atoi(value);
    }
    if (!strcmp(name, "ESP32_DevKitC_cfgdwdt")) {
        ConfDisableWdt = atoi(value);
    }
    if (!strcmp(name, "ESP32_DevKitC_cfgeeth")) {
        ConfEnableEthernet = atoi(value);
    }
    if (!strcmp(name, "ESP32_DevKitC_cfguextra")) {
        use_cmdline_extra = atoi(value);
    }
    if (!strcmp(name, "ESP32_DevKitC_cmdextra")) {
        cmdline_extra = value;
    }
    if (!strcmp(name, "ESP32_DevKitC_app_off")) {
        application_offset = atoi(value);
    }
}

// Event on the board

void cboard_DevKitC::EvKeyPress(uint key, uint mask) {}

// Event on the board

void cboard_DevKitC::EvKeyRelease(uint key, uint mask) {}

// Event on the board

void cboard_DevKitC::EvMouseButtonPress(uint button, uint x, uint y, uint state) {
    int i;

    // search for the input area which owner the event
    for (i = 0; i < inputc; i++) {
        if (((input[i].x1 <= x) && (input[i].x2 >= x)) && ((input[i].y1 <= y) && (input[i].y2 >= y))) {
            switch (input[i].id) {
                    // if event is over I_ISCP area then load hex file
                case I_ICSP:
                    PICSimLab.OpenLoadHexFileDialog();
                    break;
                    // if event is over I_PWR area then toggle board on/off
                case I_PWR:
                    if (PICSimLab.GetMcuPwr())  // if on turn off
                    {
                        PICSimLab.SetMcuPwr(0);
                        Reset();
                    } else  // if off turn on
                    {
                        PICSimLab.SetMcuPwr(1);
                        Reset();
                    }
                    break;
                    // if event is over I_RST area then turn off and reset
                case I_RST:
                    /*
                    if (PICSimLab.GetWindow()->Get_mcupwr () && reset (-1))//if powered
                     {
                      PICSimLab.Set_mcupwr (0);
                      PICSimLab.Set_mcurst (1);
                     }
                     */
                    Reset();
                    p_RST = 0;
                    break;
                case I_BOOT:
                    p_BOOT = 0;
                    break;
            }
        }
    }
}

// Event on the board

void cboard_DevKitC::EvMouseButtonRelease(uint button, uint x, uint y, uint state) {
    int i;

    // search for the input area which owner the event
    for (i = 0; i < inputc; i++) {
        if (((input[i].x1 <= x) && (input[i].x2 >= x)) && ((input[i].y1 <= y) && (input[i].y2 >= y))) {
            switch (input[i].id) {
                    // if event is over I_RST area then turn on
                case I_RST:
                    if (PICSimLab.GetMcuRst())  // if powered
                    {
                        PICSimLab.SetMcuPwr(1);
                        PICSimLab.SetMcuRst(0);
                        /*
                                 if (reset (-1))
                                  {
                                   Reset ();
                                  }
                         */
                    }
                    p_RST = 1;
                    break;
                case I_BOOT:
                    p_BOOT = 1;
                    break;
            }
        }
    }
}

// Called ever 100ms to draw board
// This is the critical code for simulator running speed

void cboard_DevKitC::Draw(CDraw* draw) {
    int i;

    draw->Canvas.Init(Scale, Scale);  // initialize draw context

    // board_x draw
    for (i = 0; i < outputc; i++)  // run over all outputs
    {
        if (!output[i].r)  // if output shape is a rectangle
        {
            draw->Canvas.SetFgColor(0, 0, 0);  // black

            switch (output[i].id)  // search for color of output
            {
                case O_LON:  // Blue using mcupwr value
                    draw->Canvas.SetColor(200 * PICSimLab.GetMcuPwr() + 55, 0, 0);
                    break;
                case O_LED:  // Blue using mcupwr value
                    draw->Canvas.SetColor(0, 0, pins[23].oavalue);
                    break;
                case O_RST:
                case O_BOOT:
                    draw->Canvas.SetColor(100, 100, 100);
                    break;
            }

            if (output[i].id == O_RST) {
                draw->Canvas.Circle(1, output[i].cx, output[i].cy, 13);
                if (p_RST) {
                    draw->Canvas.SetColor(15, 15, 15);
                } else {
                    draw->Canvas.SetColor(55, 55, 55);
                }
                draw->Canvas.Circle(1, output[i].cx, output[i].cy, 11);
            } else if (output[i].id == O_BOOT) {
                draw->Canvas.Circle(1, output[i].cx, output[i].cy, 13);
                if (p_BOOT) {
                    draw->Canvas.SetColor(15, 15, 15);
                } else {
                    draw->Canvas.SetColor(55, 55, 55);
                }
                draw->Canvas.Circle(1, output[i].cx, output[i].cy, 11);
            } else {
                draw->Canvas.Rectangle(1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1,
                                       output[i].y2 - output[i].y1);
            }
        }
    }

    // end draw
    draw->Canvas.End();
    draw->Update();
}

void cboard_DevKitC::Run_CPU_ns(uint64_t time) {
    static unsigned char pi = 0;
    static unsigned int alm[64];
    static const int pinc = MGetPinCount();

    const float RNSTEP = 200.0 * pinc * inc_ns / TTIMEOUT;

    MSetPin(IO0, p_BOOT);

    for (uint64_t c = 0; c < time; c += inc_ns) {
        if (ns_count < inc_ns) {
            // reset pins mean value
            memset(alm, 0, 64 * sizeof(unsigned int));

            // Spare parts window pre process
            if (use_spare)
                SpareParts.PreProcess();

            // j = JUMPSTEPS; //step counter
            pi = 0;
        }

        if (PICSimLab.GetMcuPwr())  // if powered
                                    // for (i = 0; i < NSTEP; i++)  // repeat for number of steps in 100ms
        {
            /*
            if (j >= JUMPSTEPS)//if number of step is bigger than steps to skip
             {
             }
             */
            // verify if a breakpoint is reached if not run one instruction
            MStep();
            InstCounterInc();
            // Oscilloscope window process
            if (use_oscope)
                Oscilloscope.SetSample();
            // Spare parts window process
            if (use_spare)
                SpareParts.Process();

            //  increment mean value counter if pin is high
            alm[pi] += pins[pi].value;
            pi++;
            if (pi == pinc)
                pi = 0;
            /*
                if (j >= JUMPSTEPS)//if number of step is bigger than steps to skip
                 {
                  j = -1; //reset counter
                 }

                j++; //counter increment
             */
        }

        ns_count += inc_ns;
        if (ns_count >= TTIMEOUT) {  // every 100ms
            ns_count -= TTIMEOUT;
            //  calculate mean value
            for (pi = 0; pi < MGetPinCount(); pi++) {
                pins[pi].oavalue = (int)((alm[pi] * RNSTEP) + 55);
            }
            // Spare parts window pre post process
            if (use_spare)
                SpareParts.PostProcess();
        }
    }
}

void cboard_DevKitC::Run_CPU(void) {
#define CHR_TIOCM_CTS 0x020
#define CHR_TIOCM_CAR 0x040
#define CHR_TIOCM_DSR 0x100
#define CHR_TIOCM_RI 0x080
#define CHR_TIOCM_DTR 0x002
#define CHR_TIOCM_RTS 0x004

    if (qemu_started != 1) {
        return;
    }

    if (serial_open && PICSimLab.GetUseDSRReset()) {
        static unsigned int status_ = 0;
        unsigned int status = qemu_picsimlab_get_TIOCM();

        if (status_ != status) {
            status_ = status;

            if ((status & CHR_TIOCM_CTS) && !(status & CHR_TIOCM_DSR)) {
                Reset();
            }

            if (!(status & CHR_TIOCM_CTS) && (status & CHR_TIOCM_DSR)) {
                uint32_t* strap_mode = qemu_picsimlab_get_internals(QEMU_INTERNAL_STRAP);
                *strap_mode = 0x0f;  // UART_BOOT(UART0)
                MReset(1);
            }
        }
    }
}

void cboard_DevKitC::board_Event(CControl* control) {
    icount = MipsStrToIcount(combo1->GetText().c_str());
    PICSimLab.EndSimulation();
}

void cboard_DevKitC::BoardOptions(int* argc, char** argv) {
    if (ConfEnableWifi) {
        strcpy(argv[(*argc)++], "-nic");
        strcpy(argv[(*argc)++], "user,model=esp32_wifi,id=u1,net=192.168.4.0/24");
    }
    if (ConfDisableWdt) {
        strcpy(argv[(*argc)++], "-global");
        strcpy(argv[(*argc)++], "driver=timer.esp32.timg,property=wdt_disable,value=true");
    }
    if (ConfEnableEthernet) {
        strcpy(argv[(*argc)++], "-nic");
        strcpy(argv[(*argc)++], "user,model=open_eth,id=u2,net=192.168.3.0/24");
    }
}

void cboard_DevKitC::board_ButtonEvent(CControl* control, uint button, uint x, uint y, uint state) {
    switch (control->GetTag()) {
        case 4: {
            lxString fname = PICSimLab.GetSharePath() + "boards/" BOARD_DevKitC_Name + "/config.lxrad";

            if (lxFileExists(fname)) {
                wconfig->DestroyChilds();
                if (wconfig->LoadXMLContextAndCreateChilds(fname)) {
                    CText* Text1 = (CText*)wconfig->GetChildByName("text1");
                    char buff[2048];
                    char line[1024];
                    strncpy(buff, (const char*)cmdline.c_str(), 2047);

                    char* arg = strtok(buff, " \n");
                    line[0] = 0;

                    while (arg) {
                        if (!line[0]) {
                            strcpy(line, arg);
                            if (line[0] != '-') {
                                Text1->AddLine(line);
                                line[0] = 0;
                            }
                        } else {
                            if (arg[0] == '-') {
                                Text1->AddLine(line);
                                strcpy(line, arg);
                            } else {
                                strcat(line, " ");
                                strcat(line, arg);
                                Text1->AddLine(line);
                                line[0] = 0;
                            }
                        }
                        arg = strtok(NULL, " \n");
                    }

                    CText* Text2 = (CText*)wconfig->GetChildByName("text2");
                    if (cmdline_extra.length()) {
                        strncpy(buff, (const char*)cmdline_extra.c_str(), 2047);

                        arg = strtok(buff, " \n");
                        line[0] = 0;

                        while (arg) {
                            if (!line[0]) {
                                strcpy(line, arg);
                                if (line[0] != '-') {
                                    Text2->AddLine(line);
                                    line[0] = 0;
                                }
                            } else {
                                if (arg[0] == '-') {
                                    Text2->AddLine(line);
                                    strcpy(line, arg);
                                } else {
                                    strcat(line, " ");
                                    strcat(line, arg);
                                    Text2->AddLine(line);
                                    line[0] = 0;
                                }
                            }
                            arg = strtok(NULL, " \n");
                        }
                    } else {
                        Text2->Clear();
                    }

                    ((CCheckBox*)wconfig->GetChildByName("checkbox1"))->SetCheck(ConfEnableWifi);
                    ((CCheckBox*)wconfig->GetChildByName("checkbox2"))->SetCheck(ConfDisableWdt);
                    ((CCheckBox*)wconfig->GetChildByName("checkbox4"))->SetCheck(ConfEnableEthernet);
                    ((CCheckBox*)wconfig->GetChildByName("checkbox3"))->SetCheck(use_cmdline_extra);
                    ((CCheckBox*)wconfig->GetChildByName("checkbox5"))->SetCheck(ConfEnableSerial);

                    ((CButton*)wconfig->GetChildByName("button1"))->EvMouseButtonRelease = PICSimLab.board_ButtonEvent;

                    ((CButton*)wconfig->GetChildByName("button2"))->EvMouseButtonRelease = PICSimLab.board_ButtonEvent;

                    wconfig->SetX(PICSimLab.GetWindow()->GetX() + 50);
                    wconfig->SetY(PICSimLab.GetWindow()->GetY() + 50);

                    wconfig->Draw();
                    wconfig->ShowExclusive();
                }
            } else {
                PICSimLab.RegisterError("File " + fname + " not found!");
            }
        } break;
        case 5: {
            ConfEnableSerial = ((CCheckBox*)wconfig->GetChildByName("checkbox5"))->GetCheck();
            ConfEnableWifi = ((CCheckBox*)wconfig->GetChildByName("checkbox1"))->GetCheck();
            ConfDisableWdt = ((CCheckBox*)wconfig->GetChildByName("checkbox2"))->GetCheck();
            ConfEnableEthernet = ((CCheckBox*)wconfig->GetChildByName("checkbox4"))->GetCheck();
            use_cmdline_extra = ((CCheckBox*)wconfig->GetChildByName("checkbox3"))->GetCheck();
            CText* Text2 = (CText*)wconfig->GetChildByName("text2");
            cmdline_extra = "";
            for (unsigned int i = 0; i < Text2->GetCountLines(); i++) {
                cmdline_extra += Text2->GetLine(i);
                cmdline_extra += " ";
            }
            wconfig->HideExclusive();
            PICSimLab.EndSimulation();
        } break;
        case 6:
            wconfig->HideExclusive();
            break;
    }
}

void cboard_DevKitC::PinsExtraConfig(int cfg) {
    switch ((cfg & 0xf000) >> 12) {
        case QEMU_EXTRA_PIN_IN_CFG: {
            uint32_t* gpio_in_sel = qemu_picsimlab_get_internals(QEMU_INTERNAL_GPIO_IN_SEL);
            int function = cfg & 0x1ff;
            int gpio = gpio_in_sel[cfg & 0x1ff] & 0x3F;
            // printf("sel in    %3i = %3i\n", gpio, function);

            switch (function) {
                case 9:  // HSPIQ
                    master_spi[0].ctrl_on = 1;
                    master_spi[0].cipo_pin = io2pin(gpio);
                    break;

                case 14:  // U0RXD
                    master_uart[0].ctrl_on = 1;
                    master_uart[0].rx_pin = io2pin(gpio);
                    break;
                case 17:  // U1RXD
                    master_uart[1].ctrl_on = 1;
                    master_uart[1].rx_pin = io2pin(gpio);
                    break;

                case 29:  // I2CEXT0_SCL
                    master_i2c[0].ctrl_on = 1;
                    master_i2c[0].scl_pin = io2pin(gpio);
                    break;
                case 30:  // I2CEXT0_SDA
                    master_i2c[0].ctrl_on = 1;
                    master_i2c[0].sda_pin = io2pin(gpio);
                    break;

                case 64:  // VSPIQ
                    master_spi[1].ctrl_on = 1;
                    master_spi[1].cipo_pin = io2pin(gpio);
                    break;

                case 95:  // I2CEXT1_SCL
                    master_i2c[1].ctrl_on = 1;
                    master_i2c[1].scl_pin = io2pin(gpio);
                    break;
                case 96:  // I2CEXT1_SDA
                    master_i2c[1].ctrl_on = 1;
                    master_i2c[1].sda_pin = io2pin(gpio);
                    break;

                case 198:  // U2RXD
                    master_uart[2].ctrl_on = 1;
                    master_uart[2].rx_pin = io2pin(gpio);
                    break;
            }

        } break;
        case QEMU_EXTRA_PIN_OUT_CFG: {
            uint32_t* gpio_out_sel = qemu_picsimlab_get_internals(QEMU_INTERNAL_GPIO_OUT_SEL);

            int function = gpio_out_sel[cfg & 0xff] & 0x1FF;
            int gpio = cfg & 0xff;
            // printf("sel out   %3i = %3i\n", gpio, function);

            switch (function) {
                case 8:  // HSPICLK
                    master_spi[0].ctrl_on = 1;
                    master_spi[0].sck_pin = io2pin(gpio);
                    break;
                case 9:  // HSPIQ
                    master_spi[0].ctrl_on = 1;
                    master_spi[0].cipo_pin = io2pin(gpio);
                    break;
                case 10:  // HSPID
                    master_spi[0].ctrl_on = 1;
                    master_spi[0].copi_pin = io2pin(gpio);
                    break;
                case 11:  // HSPICS0
                    master_spi[0].ctrl_on = 1;
                    master_spi[0].cs_pin[0] = io2pin(gpio);
                    break;

                case 14:  // U0TXD
                    master_uart[0].ctrl_on = 1;
                    master_uart[0].tx_pin = io2pin(gpio);
                    break;
                case 17:  // U1TXD
                    master_uart[1].ctrl_on = 1;
                    master_uart[1].tx_pin = io2pin(gpio);
                    break;

                case 29:  // I2CEXT0_SCL
                    master_i2c[0].ctrl_on = 1;
                    master_i2c[0].scl_pin = io2pin(gpio);
                    break;
                case 30:  // I2CEXT0_SDA
                    master_i2c[0].ctrl_on = 1;
                    master_i2c[0].sda_pin = io2pin(gpio);
                    break;

                case 61:  // HSPICS1
                    master_spi[0].ctrl_on = 1;
                    master_spi[0].cs_pin[1] = io2pin(gpio);
                    break;
                case 62:  // HSPICS2
                    master_spi[0].ctrl_on = 1;
                    master_spi[0].cs_pin[2] = io2pin(gpio);
                    break;

                case 63:  // VSPICLK
                    master_spi[1].ctrl_on = 1;
                    master_spi[1].sck_pin = io2pin(gpio);
                    break;
                case 64:  // VSPIQ
                    master_spi[1].ctrl_on = 1;
                    master_spi[1].cipo_pin = io2pin(gpio);
                    break;
                case 65:  // VSPID
                    master_spi[1].ctrl_on = 1;
                    master_spi[1].copi_pin = io2pin(gpio);
                    break;
                case 68:  // VSPICS0
                    master_spi[1].ctrl_on = 1;
                    master_spi[1].cs_pin[0] = io2pin(gpio);
                    break;
                case 69:  // VSPICS1
                    master_spi[1].ctrl_on = 1;
                    master_spi[1].cs_pin[1] = io2pin(gpio);
                    break;
                case 70:  // VSPICS2
                    master_spi[1].ctrl_on = 1;
                    master_spi[1].cs_pin[2] = io2pin(gpio);
                    break;
                case 71:  // ledc_hs_sig_out0
                case 72:  // ledc_hs_sig_out1
                case 73:  // ledc_hs_sig_out2
                case 74:  // ledc_hs_sig_out3
                case 75:  // ledc_hs_sig_out4
                case 76:  // ledc_hs_sig_out5
                case 77:  // ledc_hs_sig_out6
                case 78:  // ledc_hs_sig_out7
                case 79:  // ledc_ls_sig_out0
                case 80:  // ledc_ls_sig_out1
                case 81:  // ledc_ls_sig_out2
                case 82:  // ledc_ls_sig_out3
                case 83:  // ledc_ls_sig_out4
                case 84:  // ledc_ls_sig_out5
                case 85:  // ledc_ls_sig_out6
                case 86:  // ledc_ls_sig_out7
                    // printf("LEDC channel %i in GPIO %i\n", function - 71, gpio);
                    ledc.pins[function - 71] = io2pin(gpio);
                    break;
                case 87:  // rmt_sig_out0
                case 88:  // rmt_sig_out1
                case 89:  // rmt_sig_out2
                case 90:  // rmt_sig_out3
                case 91:  // rmt_sig_out4
                case 92:  // rmt_sig_out5
                case 93:  // rmt_sig_out6
                case 94:  // rmt_sig_out7
                    // printf("RMT channel %i in GPIO %i\n", function - 71, gpio);
                    break;
                case 95:  // I2CEXT1_SCL
                    master_i2c[1].ctrl_on = 1;
                    master_i2c[1].scl_pin = io2pin(gpio);
                    break;
                case 96:  // I2CEXT1_SDA
                    master_i2c[1].ctrl_on = 1;
                    master_i2c[1].sda_pin = io2pin(gpio);
                    break;

                case 198:  // U2RTXD
                    master_uart[2].ctrl_on = 1;
                    master_uart[2].tx_pin = io2pin(gpio);
                    break;
            }

        } break;
        case QEMU_EXTRA_PIN_IOMUX_CFG: {
            uint32_t* muxgpios = qemu_picsimlab_get_internals(QEMU_INTERNAL_IOMUX_GPIOS);

            int function = (muxgpios[cfg & 0xff] & 0x7000) >> 12;
            int gpio = cfg & 0xff;
            // printf("iomux fun 0x%02x = 0x%08x\n", gpio, function);

            switch (function) {
                case 0:
                    switch (gpio) {
                        case 1:
                            master_uart[0].ctrl_on = 1;
                            master_uart[0].tx_pin = IO1;
                            break;
                        case 3:
                            master_uart[0].ctrl_on = 1;
                            master_uart[0].rx_pin = IO3;
                            break;
                    }
                    break;
                case 1:
                    switch (gpio) {
                        case 12:
                            master_spi[0].ctrl_on = 1;
                            master_spi[0].cipo_pin = IO12;
                            break;
                        case 13:
                            master_spi[0].ctrl_on = 1;
                            master_spi[0].copi_pin = IO13;
                            break;
                        case 14:
                            master_spi[0].ctrl_on = 1;
                            master_spi[0].sck_pin = IO14;
                            break;
                        case 15:
                            master_spi[0].ctrl_on = 1;
                            master_spi[0].cs_pin[0] = IO15;
                            break;
                        case 18:
                            master_spi[1].ctrl_on = 1;
                            master_spi[1].sck_pin = IO18;
                            break;
                        case 19:
                            master_spi[1].ctrl_on = 1;
                            master_spi[1].cipo_pin = IO19;
                            break;
                        case 23:
                            master_spi[1].ctrl_on = 1;
                            master_spi[1].copi_pin = IO23;
                            break;
                        case 5:
                            master_spi[1].ctrl_on = 1;
                            master_spi[1].cs_pin[0] = IO5;
                            break;
                    }
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    switch (gpio) {
                        case 9:
                            master_uart[1].ctrl_on = 1;
                            master_uart[1].rx_pin = IO9;
                            break;
                        case 10:
                            master_uart[1].ctrl_on = 1;
                            master_uart[1].tx_pin = IO10;
                            break;
                        case 16:
                            master_uart[2].ctrl_on = 1;
                            master_uart[2].rx_pin = IO16;
                            break;
                        case 17:
                            master_uart[2].ctrl_on = 1;
                            master_uart[2].tx_pin = IO17;
                            break;
                    }
                    break;
                case 5:
                    break;
            }

        } break;
        case QEMU_EXTRA_PIN_LEDC_CFG:
            bitbang_pwm_set_duty(&ledc, (cfg & 0x0F00) >> 8, cfg & 0xFF);
            break;
    }
}

lxString cboard_DevKitC::MGetPinName(int pin) {
    lxString pinname = "error";

    switch (pin) {
        case 1:
            pinname = "VDD";
            break;
        case 2:
            pinname = "EN";
            break;
        case 3:
            pinname = "IO36";
            break;
        case 4:
            pinname = "IO39";
            break;
        case 5:
            pinname = "IO34";
            break;
        case 6:
            pinname = "IO35";
            break;
        case 7:
            pinname = "IO32";
            break;
        case 8:
            pinname = "IO33";
            break;
        case 9:
            pinname = "IO25";
            break;
        case 10:
            pinname = "IO26";
            break;
        case 11:
            pinname = "IO27";
            break;
        case 12:
            pinname = "IO14";
            break;
        case 13:
            pinname = "IO12";
            break;
        case 14:
            pinname = "GND";
            break;
        case 15:
            pinname = "IO13";
            break;
        case 16:
            pinname = "IO9";
            break;
        case 17:
            pinname = "IO10";
            break;
        case 18:
            pinname = "IO11";
            break;
        case 19:
            pinname = "VIN";
            break;
        case 20:
            pinname = "IO6";
            break;
        case 21:
            pinname = "IO7";
            break;
        case 22:
            pinname = "IO8";
            break;
        case 23:
            pinname = "IO15";
            break;
        case 24:
            pinname = "IO2";
            break;
        case 25:
            pinname = "IO0";
            break;
        case 26:
            pinname = "IO4";
            break;
        case 27:
            pinname = "IO16";
            break;
        case 28:
            pinname = "IO17";
            break;
        case 29:
            pinname = "IO5";
            break;
        case 30:
            pinname = "IO18";
            break;
        case 31:
            pinname = "IO19";
            break;
        case 32:
            pinname = "GND";
            break;
        case 33:
            pinname = "IO21";
            break;
        case 34:
            pinname = "IO3";
            break;
        case 35:
            pinname = "IO1";
            break;
        case 36:
            pinname = "IO22";
            break;
        case 37:
            pinname = "IO23";
            break;
        case 38:
            pinname = "GND";
            break;
    }

    return pinname;
}

int cboard_DevKitC::MGetPinCount(void) {
    return 38;
}

void cboard_DevKitC::MSetAPin(int pin, float value) {
    if (!pin)
        return;
    if ((pins[pin - 1].avalue != value)) {
        unsigned char channel = 0xFF;

        pins[pin - 1].avalue = value;

        switch (pin) {
            case IO36:
                channel = 0;
                break;
                /*
                //GPIO37 and GPIO38 are not exposed in devkitc
                            case x:  // GPIO37
                                channel = 1;
                                break;
                            case x:  // GPIO38
                                channel = 2;
                                break;
                */
            case IO39:
                channel = 3;
                break;
            case IO32:
                channel = 4;
                break;
            case IO33:
                channel = 5;
                break;
            case IO34:
                channel = 6;
                break;
            case IO35:
                channel = 7;
                break;
            case IO4:
                channel = 0x8 + 0;
                break;
            case IO0:
                channel = 0x8 + 1;
                break;
            case IO2:
                channel = 0x8 + 2;
                break;
            case IO15:
                channel = 0x8 + 3;
                break;
            case IO13:
                channel = 0x8 + 4;
                break;
            case IO12:
                channel = 0x8 + 5;
                break;
            case IO14:
                channel = 0x8 + 6;
                break;
            case IO27:
                channel = 0x8 + 7;
                break;
            case IO25:
                channel = 0x8 + 8;
                break;
            case IO26:
                channel = 0x8 + 9;
                break;
        }

        if (channel != 0xFF) {
            if (value > 3.3)
                value = 3.3;
            if (value < 0)
                value = 0;

            unsigned short svalue = (unsigned short)(4096 * value / 3.3);

            pins[pin - 1].ptype = PT_ANALOG;

            if (ADCvalues[channel] != svalue) {
                qemu_picsimlab_set_apin(channel, svalue);
                ADCvalues[channel] = svalue;
                // printf("Analog channel %02X = %i\n", channel, svalue);
            }
        }
    }
}

// Register the board in PICSimLab
board_init(BOARD_DevKitC_Name, cboard_DevKitC);
