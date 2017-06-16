#define GPIO_BASE_ADDR *(unsigned int volatile *)0x41004400

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
#define DIRCLR *(unsigned int volatile *)(GPIO_BASE_ADDR + 0x84)
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
    unsigned char volatile * pmux_n = PMUX_A(pinnum);
    unsigned char offset = pinnum % 2;

    unsigned char buf = *pmux_n;
    buf &= (0xF0 >> (offset * 4));
    buf |= (value << (offset * 4));
    pmux_n = buf;
}

inline void set_PMUX_B(unsigned char pinnum, unsigned char value) {
    value &= 0x08;
    unsigned char volatile * pmux_n = PMUX_B(pinnum);
    unsigned char offset = pinnum % 2;

    unsigned char buf = *pmux_n;
    buf &= (0xF0 >> (offset * 4));
    buf |= (value << (offset * 4));
    pmux_n = buf;
}
