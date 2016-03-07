#ifndef JOS_KBDREG_H
#define JOS_KBDREG_H


//press key
#define KEY_ESC 0x1
#define KEY_1   0x2
#define KEY_2	0x3
#define KEY_3	0x4
#define KEY_4	0x5
#define KEY_5	0x6
#define KEY_6	0x7
#define KEY_7	0x8
#define KEY_8	0x9
#define KEY_9	0xa
#define KEY_0	0xb
#define KEY_BAR	0xc
#define KEY_EQU	0xd
#define KEY_BACKSPACE	0xe
#define KEY_TAB	0xf
#define KEY_Q	0x10
#define KEY_W	0x11
#define KEY_E	0x12
#define KEY_R	0x13
#define KEY_T	0x14
#define KEY_Y	0x15
#define KEY_U	0x16
#define KEY_I	0x17
#define KEY_O	0x18
#define KEY_P	0x19
#define KEY_LEFTBRACE	0x1a
#define KEY_RIGHTBRACE	0x1b	
#define KEY_ENTER	0x1c
#define KEY_LEFTCTRL	0x1d
#define KEY_A	0x1e
#define KEY_S	0x1f
#define KEY_D	0x20
#define KEY_F	0x21
#define KEY_G	0x22
#define KEY_H	0x23
#define KEY_J	0x24
#define KEY_K	0x25
#define KEY_L	0x26
#define KEY_SEMICOLON	0x27
#define KEY_QUOTE	0x28
#define KEY_BACKTICK	0x29
#define KEY_LEFTSHIFT	0x2a
#define KEY_BACKSLASH	0x2b
#define KEY_Z	0x2c
#define KEY_X	0x2d
#define KEY_C	0x2e
#define KEY_V	0x2f
#define KEY_B	0x30
#define KEY_N	0x31
#define KEY_M	0x32
#define KEY_COMMA	0x33
#define KEY_DOT	0x34
#define KEY_SLASH	0x35
#define KEY_RIGHTSHIFT	0x36
#define KEY_LEFTALT	0x38
#define KEY_SPACE	0x39
#define KEY_CAPSLOCK	0x3a
#define KEY_F1	0x3b
#define KEY_F2	0x3c
#define KEY_F3	0x3d
#define KEY_F4	0x3e
#define KEY_F5	0x3f
#define KEY_F6	0x40
#define KEY_F7	0x41
#define KEY_F8	0x42
#define KEY_F9	0x43
#define KEY_F10	0x44
#define KEY_F11	0x57
#define KEY_F12	0x58

//release key
#define RELEASE_ESC 0x81
#define RELEASE_1	0x82
#define RELEASE_2	0x83
#define RELEASE_3	0x84
#define RELEASE_4	0x85
#define RELEASE_5	0x86
#define RELEASE_6	0x87
#define RELEASE_7	0x88
#define RELEASE_8	0x89
#define RELEASE_9	0x8a
#define RELEASE_0	0x8b
#define RELEASE_BAR	0x8c
#define RELEASE_EQU	0x8d
#define RELEASE_BACKSPACE	0x8e
#define RELEASE_TAB	0x8f
#define RELEASE_Q	0x90
#define RELEASE_W	0x91
#define RELEASE_E	0x92
#define RELEASE_R	0x93
#define RELEASE_T	0x94
#define RELEASE_Y	0x95
#define RELEASE_U	0x96
#define RELEASE_I	0x97
#define RELEASE_O	0x98
#define RELEASE_P	0x99
#define RELEASE_LEFTBRACE	0x9a
#define RELEASE_RIGHTBRACE	0x9b
#define RELEASE_ENTER	0x9c
#define RELEASE_LEFTCTRL	0x9d
#define RELEASE_A	0x9e
#define RELEASE_S	0x9f
#define RELEASE_D	0xa0
#define RELEASE_F	0xa1
#define RELEASE_G	0xa2
#define RELEASE_H	0xa3
#define RELEASE_J	0xa4
#define RELEASE_K	0xa5
#define RELEASE_L	0xa6
#define RELEASE_SEMICOLON	0xa7
#define RELEASE_QUOTE	0xa8
#define RELEASE_BACKTICK	0xa9
#define RELEASE_LEFTSHIFT	0xaa
#define RELEASE_BASKSLASH	0xab
#define RELEASE_Z	0xac
#define RELEASE_X	0xad
#define RELEASE_C	0xae
#define RELEASE_V	0xaf
#define RELEASE_B	0xb0
#define RELEASE_N	0xb1
#define RELEASE_M	0xb2
#define RELEASE_COMMA	0xb3
#define RELEASE_DOT	0xb4
#define RELEASE_SLASH	0xb5
#define RELEASE_RIGHTSHIFT	0xb6
#define RELEASE_SPACE 0xb9
#define RELEASE_LEFTALT 0xb8
#define RELEASE_CAPSLOCK	0xba
#define RELEASE_F1	0xbb
#define RELEASE_F2	0xbc
#define RELEASE_F3	0xbd
#define RELEASE_F4	0xbe
#define RELEASE_F5	0xbf
#define RELEASE_F6	0xc0
#define RELEASE_F7	0xc1
#define RELEASE_F8	0xc2
#define RELEASE_F9	0xc3
#define RELEASE_F10	0xc4
#define RELEASE_F11	0xd7
#define RELEASE_F12	0xd8

// Special keycodes
#define KEY_HOME	0xE0
#define KEY_END		0xE1
#define KEY_UP		0xE2
#define KEY_DN		0xE3
#define KEY_LF		0xE4
#define KEY_RT		0xE5
#define KEY_PGUP	0xE6
#define KEY_PGDN	0xE7
#define KEY_INS		0xE8
#define KEY_DEL		0xE9



/* This is i8042reg.h + kbdreg.h from NetBSD. */

#define	KBSTATP		0x64	/* kbd controller status port(I) */
#define	 KBS_DIB	0x01	/* kbd data in buffer */
#define	 KBS_IBF	0x02	/* kbd input buffer low */
#define	 KBS_WARM	0x04	/* kbd input buffer low */
#define	 KBS_OCMD	0x08	/* kbd output buffer has command */
#define	 KBS_NOSEC	0x10	/* kbd security lock not engaged */
#define	 KBS_TERR	0x20	/* kbd transmission error */
#define	 KBS_RERR	0x40	/* kbd receive error */
#define	 KBS_PERR	0x80	/* kbd parity error */

#define	KBCMDP		0x64	/* kbd controller port(O) */
#define	 KBC_RAMREAD	0x20	/* read from RAM */
#define	 KBC_RAMWRITE	0x60	/* write to RAM */
#define	 KBC_AUXDISABLE	0xa7	/* disable auxiliary port */
#define	 KBC_AUXENABLE	0xa8	/* enable auxiliary port */
#define	 KBC_AUXTEST	0xa9	/* test auxiliary port */
#define	 KBC_KBDECHO	0xd2	/* echo to keyboard port */
#define	 KBC_AUXECHO	0xd3	/* echo to auxiliary port */
#define	 KBC_AUXWRITE	0xd4	/* write to auxiliary port */
#define	 KBC_SELFTEST	0xaa	/* start self-test */
#define	 KBC_KBDTEST	0xab	/* test keyboard port */
#define	 KBC_KBDDISABLE	0xad	/* disable keyboard port */
#define	 KBC_KBDENABLE	0xae	/* enable keyboard port */
#define	 KBC_PULSE0	0xfe	/* pulse output bit 0 */
#define	 KBC_PULSE1	0xfd	/* pulse output bit 1 */
#define	 KBC_PULSE2	0xfb	/* pulse output bit 2 */
#define	 KBC_PULSE3	0xf7	/* pulse output bit 3 */

#define	KBDATAP		0x60	/* kbd data port(I) */
#define	KBOUTP		0x60	/* kbd data port(O) */

#define	K_RDCMDBYTE	0x20
#define	K_LDCMDBYTE	0x60

#define	KC8_TRANS	0x40	/* convert to old scan codes */
#define	KC8_MDISABLE	0x20	/* disable mouse */
#define	KC8_KDISABLE	0x10	/* disable keyboard */
#define	KC8_IGNSEC	0x08	/* ignore security lock */
#define	KC8_CPU		0x04	/* exit from protected mode reset */
#define	KC8_MENABLE	0x02	/* enable mouse interrupt */
#define	KC8_KENABLE	0x01	/* enable keyboard interrupt */
#define	CMDBYTE		(KC8_TRANS|KC8_CPU|KC8_MENABLE|KC8_KENABLE)

/* keyboard commands */
#define	KBC_RESET	0xFF	/* reset the keyboard */
#define	KBC_RESEND	0xFE	/* request the keyboard resend the last byte */
#define	KBC_SETDEFAULT	0xF6	/* resets keyboard to its power-on defaults */
#define	KBC_DISABLE	0xF5	/* as per KBC_SETDEFAULT, but also disable key scanning */
#define	KBC_ENABLE	0xF4	/* enable key scanning */
#define	KBC_TYPEMATIC	0xF3	/* set typematic rate and delay */
#define	KBC_SETTABLE	0xF0	/* set scancode translation table */
#define	KBC_MODEIND	0xED	/* set mode indicators(i.e. LEDs) */
#define	KBC_ECHO	0xEE	/* request an echo from the keyboard */

/* keyboard responses */
#define	KBR_EXTENDED	0xE0	/* extended key sequence */
#define	KBR_RESEND	0xFE	/* needs resend of command */
#define	KBR_ACK		0xFA	/* received a valid command */
#define	KBR_OVERRUN	0x00	/* flooded */
#define	KBR_FAILURE	0xFD	/* diagnosic failure */
#define	KBR_BREAK	0xF0	/* break code prefix - sent on key release */
#define	KBR_RSTDONE	0xAA	/* reset complete */
#define	KBR_ECHO	0xEE	/* echo response */

#endif /* !JOS_KBDREG_H */
