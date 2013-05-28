#include <config.h>
#include <echo-reg.h>
#include <hci_tx_ucode.h>
#include <sp_setup.h>

#define HCI_READ(a) 		readl((a))
#define HCI_WRITE(v,a) 		writel(v, (a))

#define HCI_TX_RAM_BASE		(0x30380000UL)
#define HCI_CFG_REG_BASE	(0x303B0000UL)
#define HCI_8051_REG_BASE	(0x3039F000UL)

enum {
	CFG_AUTO = 0x08,
	CFG_DONE = 0x0C,
}HCI_CFG_REG;

enum {
	RESET 	=    0xD10,
	PATCH0_VLD = 0xC1C,
}HCI_8051_REG;

void sdp_hci_tx_init(void)
{
	int idx = 0;

	HCI_WRITE(0x00000001, 0x30910004);		// Main Sync On
	HCI_WRITE(0x00000000, 0x30910084);		// Disp Mute Off
	HCI_WRITE(0x0000000B, 0x30220D48);		// Post Pattern Select
	HCI_WRITE(0x00000104, 0x30220D70);		// Post PTP TPGN Select
//	HCI_WRITE(0x00000008, 0x30090860);		// SSC Off, 110708
	HCI_WRITE(0x00028000, 0x30090914);		// lvds0_sel_main[14]=0, lvds0_sel_sub[15]=1, dp_clk_div_sel[16]=0, svdl_sel_main[17]=1

	HCI_WRITE(1, (HCI_8051_REG_BASE + RESET));	// Reset 8051

	for(idx = 0; idx < (sizeof(u16NormalTV) >> 1); idx+=2){
		HCI_WRITE(u16NormalTV[idx+1], (HCI_TX_RAM_BASE + u16NormalTV[idx]));
	}

	HCI_WRITE(0x01, (HCI_8051_REG_BASE + PATCH0_VLD));	
	HCI_WRITE(0xE1, (HCI_CFG_REG_BASE + CFG_AUTO));	
	HCI_WRITE(0x01, (HCI_CFG_REG_BASE + CFG_DONE));	
//	HCI_WRITE(0x02, (HCI_CFG_REG_BASE + 0x0044));	//110708, chulju
	HCI_WRITE(0x00, (HCI_8051_REG_BASE + RESET));	// un-reset 8051
}



