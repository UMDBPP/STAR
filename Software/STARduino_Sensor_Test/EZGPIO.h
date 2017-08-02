#define GPIO_BASE_ADDR 0x41004400
#define PA 0x00
#define PB 0x80
#define IN 0x1
#define OUT 0x0
#define HI 0x1
#define LO 0x0

#define DIR_A *(unsigned int volatile *)(GPIO_BASE_ADDR)
#define DIRCLR_A *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x04)
#define DIRSET_A *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x08)
#define DIRTGL_A *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x0C)
#define OUT_A *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x10)
#define OUTCLR_A *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x14)
#define OUTSET_A *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x18)
#define OUTTGL_A *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x1C)
#define IN_A *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x20)
#define PINCFG_A(pinnum) *(unsigned char volatile *)(GPIO_BASE_ADDR + 0x40 + pinnum)
#define PMUX_A(pinnum) *(unsigned char volatile *)(GPIO_BASE_ADDR + 0x30 + (pinnum/2))

#define DIR_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x80)
#define DIRCLR_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x84)
#define DIRSET_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x88)
#define DIRTGL_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x8C)
#define OUT_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x90)
#define OUTCLR_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x94)
#define OUTSET_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x98)
#define OUTTGL_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x9C)
#define IN_B *(unsigned int volatile *)(GPIO_BASE_ADDR + 0xA0)
#define PINCFG_B(pinnum) *(unsigned char volatile *)(GPIO_BASE_ADDR + 0xC0 + pinnum)
#define PMUX_B(pinnum) *(unsigned char volatile *)(GPIO_BASE_ADDR + 0x30 + 0x80 + (pinnum/2))

// Pinmux black magic
// By Camden Miller
inline void set_PMUX_A(unsigned char pinnum, unsigned char value) {
    value &= 0x08;
    unsigned char pmux_n = PMUX_A(pinnum);
    unsigned char offset = pinnum % 2;

    unsigned char buf = pmux_n;
    buf &= (0xF0 >> (offset * 4));
    buf |= (value << (offset * 4));
    pmux_n = buf;
}

inline void set_PMUX_B(unsigned char pinnum, unsigned char value) {
    value &= 0x08;
    unsigned char pmux_n = PMUX_B(pinnum);
    unsigned char offset = pinnum % 2;

    unsigned char buf = pmux_n;
    buf &= (0xF0 >> (offset * 4));
    buf |= (value << (offset * 4));
    pmux_n = buf;
}

inline void set_DIR(int port, int pin, bool direction) {
    if((pin > 31) || (pin < 0)) { return; }

    if(port == PA) {
        if(direction == IN) {
            DIRCLR_A = (1 << pin);
        } else {
            DIRSET_A = (1 << pin);
        }
    } else if(port == PB) {
        if(direction == IN) {
            DIRCLR_B = (1 << pin);
        } else {
            DIRSET_B = (1 << pin);
        }
    } else {
        return;
    }
}

inline void toggle_DIR(int port, int pin) {
    if((pin > 31) || (pin < 0)) { return; }

    if(port == PA) {
        DIRTGL_A = (1 << pin);
    } else if(port == PB) {
        DIRTGL_B = (1 << pin);
    }
}

inline void set_OUTPUT(int port, int pin, bool setting) {
    if((pin > 31) || (pin < 0)) { return; }

    if(port == PA) {
        if(setting == HI) {
            DIRSET_A = (1 << pin);
        } else {
            DIRCLR_A = (1 << pin);
        }
    } else if(port == PB) {
        if(setting == HI) {
            DIRSET_B = (1 << pin);
        } else {
            DIRCLR_B = (1 << pin);
        }
    }
}

inline void toggle_OUTPUT(int port, int pin) {
    if((pin > 31) || (pin < 0)) { return; }

    if(port == PA) {
        OUTTGL_A = (1 << pin);
    } else if(port == PB) {
        OUTTGL_B = (1 << pin);
    }
}

inline volatile int read_INPUT(int port, int pin) {
    unsigned volatile int buf;

    if(port == PA) {
        buf = IN_A;
    } else if(port == PB) {
        buf = IN_B;
    }

    if(buf & (1 << pin)) {
        return 1;
    } else {
        return 0;
    }
}
