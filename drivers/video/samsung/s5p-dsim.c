/* linux/drivers/video/samsung/s5p-dsim.c
 *
 * Samsung MIPI-DSIM driver.
 *
 * InKi Dae, <inki.dae@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modified by Samsung Electronics (UK) on May 2010
 *
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/clk.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/fb.h>
#include <linux/ctype.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/memory.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>
#include <linux/regulator/consumer.h>
#include <linux/gpio.h>
#include <linux/pm_runtime.h>

#ifdef CONFIG_HAS_WAKELOCK
#include <linux/wakelock.h>
#include <linux/earlysuspend.h>
#include <linux/suspend.h>
#endif

#include <plat/clock.h>
#include <plat/regs-dsim.h>
#include <plat/gpio-cfg.h>

#include <mach/map.h>
#include <mach/dsim.h>
#include <mach/mipi_ddi.h>
#include <mach/irqs.h>
#include <mach/regs-clock.h>

#include "s5p-dsim.h"
#include "s5p_dsim_lowlevel.h"
#include "s3cfb.h"

static DEFINE_MUTEX(dsim_rd_wr_mutex);
static DECLARE_COMPLETION(dsim_rd_comp);
static DECLARE_COMPLETION(dsim_wr_comp);

static bool probe_fly = true;
struct delayed_work backlight_on;//fly
struct mutex mutex_dsim;
#define CONFIG_BUSFREQ_OPP_DSIM_LOCK
#ifdef CONFIG_BUSFREQ_OPP_DSIM_LOCK//ly
#include <mach/dev.h>
#define BUSFREQ_160MHZ  133133
#endif
/* Indicates the state of the device */
struct dsim_global {
	struct device *dev;
	struct clk *clock;
	/* VMIPI_1.1V */
	struct regulator *r_mipi_1_1v;
	/* VMIPI_1.8V */
	struct regulator *r_mipi_1_8v;

#ifdef CONFIG_BUSFREQ_OPP_DSIM_LOCK
	struct device *bus_dev;
#endif
	struct s5p_platform_dsim *pd;
	struct dsim_config *dsim_info;
	struct dsim_lcd_config *dsim_lcd_info;
	/* lcd panel data. */
	struct s3cfb_lcd *lcd_panel_info;
	/* platform and machine specific data for lcd panel driver. */
	struct mipi_ddi_platform_data *mipi_ddi_pd;
	/* lcd panel driver based on MIPI-DSI. */
	struct mipi_lcd_driver *mipi_drv;

	unsigned int irq;
	unsigned int te_irq;
	unsigned int reg_base;

	unsigned char state;
	unsigned int data_lane;
	enum dsim_byte_clk_src e_clk_src;
	unsigned long hs_clk;
	unsigned long byte_clk;
	unsigned long escape_clk;
	unsigned char freq_band;

	char header_fifo_index[DSIM_HEADER_FIFO_SZ];
	#ifdef CONFIG_HAS_WAKELOCK
	struct early_suspend	early_suspend;
	struct wake_lock	idle_lock;
	#endif
	struct timer_list lp_timer; 	//f ly
	struct timer_list lp_timer2; 	//f ly
	struct delayed_work lp_work_low;
	struct delayed_work lp_work_high;

	struct dsim_ops		*ops;
};

struct mipi_lcd_info {
	struct list_head	list;
	struct mipi_lcd_driver	*mipi_drv;
};
extern void lcd_on(void);
extern void lcd_off(void);

#define MIPI_RESP_ERR				0x02
#define MIPI_RESP_EOTP			0x08
#define MIPI_RESP_GENERIC_RD_1		0x11
#define MIPI_RESP_GENERIC_RD_2		0x12
#define MIPI_RESP_GENERIC_RD_LONG		0x1A
#define MIPI_RESP_DCS_RD_LONG		0x1C
#define MIPI_RESP_DCS_RD_1			0x21
#define MIPI_RESP_DCS_RD_2			0x22

#define MIPI_CMD_GENERIC_WR_0		0x03
#define MIPI_CMD_GENERIC_WR_1		0x13
#define MIPI_CMD_GENERIC_WR_2		0x23
#define MIPI_CMD_GENERIC_WR_LONG		0x29

#define MIPI_CMD_DSI_WR_0			0x05
#define MIPI_CMD_DSI_WR_1			0x15
#define MIPI_CMD_DSI_WR_LONG			0x39

#define MIPI_CMD_GENERIC_RD_0		0x04
#define MIPI_CMD_GENERIC_RD_1		0x14
#define MIPI_CMD_GENERIC_RD_2		0x24

#define MIPI_CMD_DSI_RD_0			0x06

#define MIPI_CMD_DSI_SET_PKT_SZ		0x37

#define DSIM_TIMEOUT				msecs_to_jiffies(250)
#define DSIM_RX_FIFO_READ_DONE		0x30800002
#define DSIM_MAX_RX_FIFO			20

#define S5P_DSIM_INT_SFR_FIFO_EMPTY		29
#define S5P_DSIM_INT_BTA			25
#define S5P_DSIM_INT_MSK_FRAME_DONE		24
#define S5P_DSIM_INT_RX_TIMEOUT		21
#define S5P_DSIM_INT_BTA_TIMEOUT		20
#define S5P_DSIM_INT_RX_DONE			18
#define S5P_DSIM_INT_RX_TE			17
#define S5P_DSIM_INT_RX_ACK			16
#define S5P_DSIM_INT_RX_ECC_ERR		15
#define S5P_DSIM_IMT_RX_CRC_ERR		14

static LIST_HEAD(lcd_info_list);
static DEFINE_MUTEX(mipi_lock);
static struct dsim_global *g_dsim;

static void s5p_dsim_set_clock(unsigned int dsim_base,
	unsigned char byte_clk_sel, unsigned char enable);
unsigned char s5p_dsim_set_hs_enable(unsigned int dsim_base);
static int s5p_dsim_init_dsim(unsigned int dsim_base);
static void s5p_dsim_set_display_mode(unsigned int dsim_base,
	struct dsim_lcd_config *main_lcd, struct dsim_lcd_config *sub_lcd);
static int s5p_dsim_init_link(unsigned int dsim_base);
static unsigned char s5p_dsim_set_data_transfer_mode(unsigned int dsim_base,
	unsigned char data_path, unsigned char hs_enable);


static struct dsim_global dsim;

static unsigned int dsim_toggle_per_frame_count=25;


//fly
void delayed_work_lcd_on(void)
{
	lcd_on();
}

void s5p_dsim_frame_done_interrupt_enable(u8 enable)
{
	u32 intmsk;
	u8 state = !enable;

	intmsk = readl(dsim.reg_base + S5P_DSIM_INTMSK);

	if (state == 0)
		intmsk &= ~(0x01 << S5P_DSIM_INT_MSK_FRAME_DONE);
	else
		intmsk |= (0x01 << S5P_DSIM_INT_MSK_FRAME_DONE);

	writel(intmsk, dsim.reg_base + S5P_DSIM_INTMSK);

}
EXPORT_SYMBOL_GPL(s5p_dsim_frame_done_interrupt_enable);

struct s5p_platform_dsim *to_dsim_plat(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);

	return (struct s5p_platform_dsim *)pdev->dev.platform_data;
}

unsigned char s5p_dsim_wr_data(unsigned int dsim_base,
	unsigned int data_id, unsigned int data0, unsigned int data1)
{
	unsigned char check_rx_ack = 0;

	if (dsim.state == DSIM_STATE_ULPS) {
		dev_err(dsim.dev, "state is ULPS.\n");
		return DSIM_FALSE;
	}
	switch (data_id) {
	/* short packet types of packet types for command. */
	case GEN_SHORT_WR_NO_PARA:
	case GEN_SHORT_WR_1_PARA:
	case GEN_SHORT_WR_2_PARA:
	case DCS_WR_NO_PARA:
	case DCS_WR_1_PARA:
	case SET_MAX_RTN_PKT_SIZE:
		s5p_dsim_wr_tx_header(dsim_base, (unsigned char) data_id,
			(unsigned char) data0, (unsigned char) data1);
		if (check_rx_ack)
			/* process response func  should be implemented */
			return DSIM_TRUE;
		else
			return DSIM_TRUE;

	/* general command */
	case CMD_OFF:
	case CMD_ON:
	case SHUT_DOWN:
	case TURN_ON:
		s5p_dsim_wr_tx_header(dsim_base, (unsigned char) data_id,
			(unsigned char) data0, (unsigned char) data1);
		if (check_rx_ack)
			/* process response func should be implemented. */
			return DSIM_TRUE;
		else
			return DSIM_TRUE;

	/* packet types for video data */
	case VSYNC_START:
	case VSYNC_END:
	case HSYNC_START:
	case HSYNC_END:
	case EOT_PKT:
		return DSIM_TRUE;

	/* short and response packet types for command */
	case GEN_RD_1_PARA:
	case GEN_RD_2_PARA:
	case GEN_RD_NO_PARA:
	case DCS_RD_NO_PARA:
		s5p_dsim_clear_interrupt(dsim_base, 0xffffffff);
		s5p_dsim_wr_tx_header(dsim_base, (unsigned char) data_id,
			(unsigned char) data0, (unsigned char) data1);
		/* process response func should be implemented. */
		return DSIM_FALSE;

	/* long packet type and null packet */
	case NULL_PKT:
	case BLANKING_PKT:
		return DSIM_TRUE;
	case GEN_LONG_WR:
	case DCS_LONG_WR:
		{
			u32 uCnt = 0;
			u32* pWordPtr = (u32 *)data0;
			do {
				s5p_dsim_wr_tx_data(dsim_base, pWordPtr[uCnt]);
			} while (((data1-1) / 4) > uCnt++);
		}

		/* put data into header fifo */
		s5p_dsim_wr_tx_header(dsim_base, (unsigned char)data_id,
			(unsigned char)(((unsigned short)data1) & 0xff),
			(unsigned char)((((unsigned short)data1) & 0xff00) >> 8));

		if (check_rx_ack)
			/* process response func should be implemented. */
			return DSIM_TRUE;
		else
			return DSIM_TRUE;

	/* packet typo for video data */
	case RGB565_PACKED:
	case RGB666_PACKED:
	case RGB666_LOOSLY:
	case RGB888_PACKED:
		if (check_rx_ack)
		{
			/* process response func should be implemented. */
			return DSIM_TRUE;
		}
		else
			return DSIM_TRUE;
	default:
		dev_warn(dsim.dev, "data id %x is not supported current DSI spec.\n", data_id);
		return DSIM_FALSE;
	}

	return DSIM_TRUE;
}

int s5p_dsim_rd_data(void *ptr, u8 addr, u16 count, u8 *buf)
{
	u32 i, temp;
	u8 response = 0;
	u16 rxsize;
	u32 txhd;
	u32 rxhd;
	int j;
	struct dsim_global *dsim = ptr;
	unsigned int reg_base = dsim->reg_base;

	if (dsim->mipi_ddi_pd->resume_complete == 0) {
		dev_err(dsim->dev, "DSIM Status: SUSPEND\n");
		return DSIM_FALSE;
	}

	mutex_lock(&dsim_rd_wr_mutex);
	INIT_COMPLETION(dsim_rd_comp);

	switch (count) {
	case 1:
		response = MIPI_RESP_GENERIC_RD_1;
		break;
	case 2:
		response = MIPI_RESP_GENERIC_RD_2;
		break;
	default:
		response = MIPI_RESP_GENERIC_RD_LONG;
		break;
	}

	/* set return packet size */
	txhd = MIPI_CMD_DSI_SET_PKT_SZ | count << 8;

	writel(txhd, reg_base + S5P_DSIM_PKTHDR);

	/* set address to read */
	txhd = MIPI_CMD_GENERIC_RD_1 | addr << 8;

	writel(txhd, reg_base + S5P_DSIM_PKTHDR);

	if (!wait_for_completion_interruptible_timeout(&dsim_rd_comp, DSIM_TIMEOUT)) {
		dev_err(dsim->dev, "ERROR:%s timout\n", __func__);
		mutex_unlock(&dsim_rd_wr_mutex);
		return 0;
	}

	rxhd = readl(reg_base + S5P_DSIM_RXFIFO);
	dev_info(dsim->dev, "rxhd : %x\n", rxhd);
	if ((u8)(rxhd & 0xff) != response) {
		dev_err(dsim->dev, "[DSIM:ERROR]:%s wrong response rxhd : %x, response:%x\n"
		    , __func__, rxhd, response);
		goto clear_rx_fifo;
	}
	/* for short packet */
	if (count <= 2) {
		for (i = 0; i < count; i++)
			buf[i] = (rxhd >> (8+(i*8))) & 0xff;
		rxsize = count;
	} else {
		/* for long packet */
		rxsize = (u16)((rxhd & 0x00ffff00) >> 8);
		dev_info(dsim->dev, "rcv size : %d\n", rxsize);
		if (rxsize != count) {
			dev_err(dsim->dev, "[DSIM:ERROR]:%s received data size mismatch received : %d, requested : %d\n",
				__func__, rxsize, count);
			goto clear_rx_fifo;
		}

		for (i = 0; i < rxsize>>2; i++) {
			temp = readl(reg_base + S5P_DSIM_RXFIFO);
			dev_info(dsim->dev, "pkt : %08x\n", temp);
			for (j = 0; j < 4; j++) {
				buf[(i*4)+j] = (u8)(temp>>(j*8))&0xff;
				/* printk("Value : %02x\n",(temp>>(j*8))&0xff); */
			}
		}
		if (rxsize % 4) {
			temp = readl(reg_base + S5P_DSIM_RXFIFO);
			dev_info(dsim->dev, "pkt-l : %08x\n", temp);
			for (j = 0; j < rxsize%4; j++) {
				buf[(i*4)+j] = (u8)(temp>>(j*8))&0xff;
				/* printk("Value : %02x\n",(temp>>(j*8))&0xff); */
			}
		}
	}

	temp = readl(reg_base + S5P_DSIM_RXFIFO);

	if (temp != DSIM_RX_FIFO_READ_DONE) {
		dev_warn(dsim->dev, "[DSIM:WARN]:%s Can't found RX FIFO READ DONE FLAG : %x\n", __func__, temp);
		goto clear_rx_fifo;
	}

	mutex_unlock(&dsim_rd_wr_mutex);
	return rxsize;

clear_rx_fifo:
	i = 0;
	while (1) {
		temp = readl(reg_base+S5P_DSIM_RXFIFO);
		if ((temp == DSIM_RX_FIFO_READ_DONE) || (i > DSIM_MAX_RX_FIFO))
			break;
		dev_info(dsim->dev, "[DSIM:INFO] : %s clear rx fifo : %08x\n", __func__, temp);
		i++;
	}
	dev_info(dsim->dev, "[DSIM:INFO] : %s done count : %d, temp : %08x\n", __func__, i, temp);

	mutex_unlock(&dsim_rd_wr_mutex);
	return 0;

}

int s5p_dsim_dcs_rd_data(void *ptr, u8 addr, u16 count, u8 *buf)
{
	u32 i, temp;
	u8 response = 0;
	u16 rxsize;
	u32 txhd;
	u32 rxhd;
	int j;
	struct dsim_global *dsim = ptr;
	unsigned int reg_base = dsim->reg_base;

	if (dsim->mipi_ddi_pd->resume_complete == 0) {
		dev_err(dsim->dev, "DSIM Status: SUSPEND\n");
		return DSIM_FALSE;
	}

	mutex_lock(&dsim_rd_wr_mutex);
	INIT_COMPLETION(dsim_rd_comp);

	switch (count) {
	case 1:
		response = MIPI_RESP_DCS_RD_1;
		break;
	case 2:
		response = MIPI_RESP_DCS_RD_2;
		break;
	default:
		response = MIPI_RESP_DCS_RD_LONG;
		break;
	}

	/* set return packet size */
	txhd = MIPI_CMD_DSI_SET_PKT_SZ | count << 8;

	writel(txhd, reg_base + S5P_DSIM_PKTHDR);

	/* set address to read */
	txhd = MIPI_CMD_DSI_RD_0 | addr << 8;

	writel(txhd, reg_base + S5P_DSIM_PKTHDR);

	if (!wait_for_completion_interruptible_timeout(&dsim_rd_comp, DSIM_TIMEOUT)) {
		dev_err(dsim->dev, "ERROR:%s timout\n", __func__);
		mutex_unlock(&dsim_rd_wr_mutex);
		return 0;
	}

	rxhd = readl(reg_base + S5P_DSIM_RXFIFO);
	dev_info(dsim->dev, "rxhd : %x\n", rxhd);
	if ((u8)(rxhd & 0xff) != response) {
		dev_err(dsim->dev, "[DSIM:ERROR]:%s wrong response rxhd : %x, response:%x\n"
		    , __func__, rxhd, response);
		goto error_read;
	}
	/* for short packet */
	if (count <= 2) {
		for (i = 0; i < count; i++)
			buf[i] = (rxhd >> (8+(i*8))) & 0xff;
		rxsize = count;
	} else {
		/* for long packet */
		rxsize = (u16)((rxhd & 0x00ffff00) >> 8);
		dev_info(dsim->dev, "rcv size : %d\n", rxsize);
		if (rxsize != count) {
			dev_err(dsim->dev, "[DSIM:ERROR]:%s received data size mismatch received : %d, requested : %d\n",
				__func__, rxsize, count);
			goto error_read;
		}

		for (i = 0; i < rxsize>>2; i++) {
			temp = readl(reg_base + S5P_DSIM_RXFIFO);
			dev_info(dsim->dev, "pkt : %08x\n", temp);
			for (j = 0; j < 4; j++) {
				buf[(i*4)+j] = (u8)(temp>>(j*8))&0xff;
				/* printk("Value : %02x\n",(temp>>(j*8))&0xff); */
			}
		}
		if (rxsize % 4) {
			temp = readl(reg_base + S5P_DSIM_RXFIFO);
			dev_info(dsim->dev, "pkt-l : %08x\n", temp);
			for (j = 0; j < rxsize%4; j++) {
				buf[(i*4)+j] = (u8)(temp>>(j*8))&0xff;
				/* printk("Value : %02x\n",(temp>>(j*8))&0xff); */
			}
		}
	}

	mutex_unlock(&dsim_rd_wr_mutex);
	return rxsize;

error_read:
	mutex_unlock(&dsim_rd_wr_mutex);
	return 0;

}

static int count_flag = 1;

void delayed_work_high(void)
{
	if(count_flag == 1)
	{
		s5p_dsim_enable_hs_clock(dsim.reg_base, 0); // 0 is set to high
		schedule_delayed_work(&(dsim.lp_work_low), usecs_to_jiffies(200));
		count_flag = 0;
	}
}

void delayed_work_low(void)
{
	if(count_flag == 0)
	{
		s5p_dsim_enable_hs_clock(dsim.reg_base, 1); // 1 is set to low
		schedule_delayed_work(&(dsim.lp_work_high), msecs_to_jiffies(300));
		count_flag = 1;
	}
}

void s3p_dsim_clock_lp2(int i_interrupts)
{

}

void s3p_dsim_clock_lp(void)
{

}
static irqreturn_t s5p_dsim_isr(int irq, void *dev_id)
{
	int i;
	unsigned int intsrc = 0;
	unsigned int intmsk = 0;
	struct dsim_global *pdsim = NULL;

	//printk("s5p_dsim_isr+++++++++++++++++++++++++fly\n"); 
	pdsim = (struct dsim_global *)dev_id;
	if (!pdsim) {
		printk(KERN_ERR "%s:error:wrong parameter\n", __func__);
		return IRQ_HANDLED;
	}

	intsrc = readl(pdsim->reg_base + S5P_DSIM_INTSRC);
	intmsk = readl(pdsim->reg_base + S5P_DSIM_INTMSK);

	intmsk = ~(intmsk) & intsrc;

	for (i = 0; i < 32; i++) {
		if (intmsk & (0x01<<i)) {
			switch (i) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:	
			case 10:	
			case 11:	
			case 12:	
			case 13:		
				break;
			case S5P_DSIM_INT_BTA:
				break;
			case S5P_DSIM_INT_RX_TIMEOUT:
				break;
			case S5P_DSIM_INT_BTA_TIMEOUT:
				break;
			case S5P_DSIM_INT_RX_DONE:
				break;
			case S5P_DSIM_INT_RX_TE:
				break;
			case S5P_DSIM_INT_RX_ACK:
				break;
			case S5P_DSIM_INT_RX_ECC_ERR:
				break;
			case S5P_DSIM_IMT_RX_CRC_ERR:
				break;
			case S5P_DSIM_INT_SFR_FIFO_EMPTY:
				break;
			case S5P_DSIM_INT_MSK_FRAME_DONE:
				
				if (!dsim_toggle_per_frame_count && s3cfb_vsync_status_check() ) 
				{
					s5p_dsim_enable_hs_clock(dsim.reg_base, 0);
					udelay(1);
					s5p_dsim_enable_hs_clock(dsim.reg_base, 1);
 
					dsim_toggle_per_frame_count = 25;
				}
				if (dsim_toggle_per_frame_count)
					dsim_toggle_per_frame_count--;
				break;
			}
		}
	}
	/* clear irq */
	writel(intsrc, pdsim->reg_base + S5P_DSIM_INTSRC);
	return IRQ_HANDLED;
}

static void s5p_dsim_init_header_fifo(void)
{
	unsigned int cnt;

	for (cnt = 0; cnt < DSIM_HEADER_FIFO_SZ; cnt++)
		dsim.header_fifo_index[cnt] = -1;
	return;
}

unsigned char s5p_dsim_pll_on(unsigned int dsim_base, unsigned char enable)
{
	if (enable) {
		int sw_timeout = 1000;
		s5p_dsim_clear_interrupt(dsim_base, DSIM_PLL_STABLE);
		s5p_dsim_enable_pll(dsim_base, 1);
		while (1) {
			sw_timeout--;
			if (s5p_dsim_is_pll_stable(dsim_base))
				return DSIM_TRUE;
			if (sw_timeout == 0)
				return DSIM_FALSE;
		}
	} else
		s5p_dsim_enable_pll(dsim_base, 0);

	return DSIM_TRUE;
}

static unsigned long s5p_dsim_change_pll(unsigned int dsim_base, unsigned char pre_divider,
	unsigned short main_divider, unsigned char scaler)
{
	unsigned long dfin_pll, dfvco, dpll_out;
	unsigned char freq_band;
	unsigned char temp0, temp1;

	dfin_pll = (MIPI_FIN / pre_divider);

	if (dfin_pll < 6 * 1000 * 1000 || dfin_pll > 12 * 1000 * 1000) {
		dev_warn(dsim.dev, "warning!!\n");
		dev_warn(dsim.dev, "fin_pll range is 6MHz ~ 12MHz\n");
		dev_warn(dsim.dev, "fin_pll of mipi dphy pll is %luMHz\n",
				(dfin_pll / 1000000));

		s5p_dsim_enable_afc(dsim_base, 0, 0);
	} else {
		if (dfin_pll < 7 * 1000000)
			s5p_dsim_enable_afc(dsim_base, 1, 0x1);
		else if (dfin_pll < 8 * 1000000)
			s5p_dsim_enable_afc(dsim_base, 1, 0x0);
		else if (dfin_pll < 9 * 1000000)
			s5p_dsim_enable_afc(dsim_base, 1, 0x3);
		else if (dfin_pll < 10 * 1000000)
			s5p_dsim_enable_afc(dsim_base, 1, 0x2);
		else if (dfin_pll < 11 * 1000000)
			s5p_dsim_enable_afc(dsim_base, 1, 0x5);
		else
			s5p_dsim_enable_afc(dsim_base, 1, 0x4);
	}

	dfvco = dfin_pll * main_divider;
	dev_dbg(dsim.dev, "dfvco = %lu, dfin_pll = %lu, main_divider = %d\n",
		dfvco, dfin_pll, main_divider);
	if (dfvco < 500000000 || dfvco > 1000000000) {
		dev_warn(dsim.dev, "Caution!!\n");
		dev_warn(dsim.dev, "fvco range is 500MHz ~ 1000MHz\n");
		dev_warn(dsim.dev, "fvco of mipi dphy pll is %luMHz\n",
				(dfvco / 1000000));
	}

	dpll_out = dfvco / (1 << scaler);
	dev_dbg(dsim.dev, "dpll_out = %lu, dfvco = %lu, scaler = %d\n",
		dpll_out, dfvco, scaler);
	if (dpll_out < 100 * 1000000)
		freq_band = 0x0;
	else if (dpll_out < 120 * 1000000)
		freq_band = 0x1;
	else if (dpll_out < 170 * 1000000)
		freq_band = 0x2;
	else if (dpll_out < 220 * 1000000)
		freq_band = 0x3;
	else if (dpll_out < 270 * 1000000)
		freq_band = 0x4;
	else if (dpll_out < 320 * 1000000)
		freq_band = 0x5;
	else if (dpll_out < 390 * 1000000)
		freq_band = 0x6;
	else if (dpll_out < 450 * 1000000)
		freq_band = 0x7;
	else if (dpll_out < 510 * 1000000)
		freq_band = 0x8;
	else if (dpll_out < 560 * 1000000)
		freq_band = 0x9;
	else if (dpll_out < 640 * 1000000)
		freq_band = 0xa;
	else if (dpll_out < 690 * 1000000)
		freq_band = 0xb;
	else if (dpll_out < 770 * 1000000)
		freq_band = 0xc;
	else if (dpll_out < 870 * 1000000)
		freq_band = 0xd;
	else if (dpll_out < 950 * 1000000)
		freq_band = 0xe;
	else
		freq_band = 0xf;

	dev_dbg(dsim.dev, "freq_band = %d\n", freq_band);

	s5p_dsim_pll_freq(dsim_base, pre_divider, main_divider, scaler);
	temp0 = 0;
	s5p_dsim_hs_zero_ctrl(dsim_base, temp0);
	temp1 = 0;
	s5p_dsim_prep_ctrl(dsim_base, temp1);

	/* Freq Band */
	s5p_dsim_pll_freq_band(dsim_base, freq_band);

	/* Stable time */
	s5p_dsim_pll_stable_time(dsim_base, dsim.dsim_info->pll_stable_time);

	/* Enable PLL */
	dev_dbg(dsim.dev, "FOUT of mipi dphy pll is %luMHz\n",
			(dpll_out / 1000000));

	return dpll_out;
}

static void s5p_dsim_set_clock(unsigned int dsim_base,
	unsigned char byte_clk_sel, unsigned char enable)
{
	unsigned int esc_div;
	unsigned long esc_clk_error_rate;

	if (enable) {
		dsim.e_clk_src = byte_clk_sel;

		/* Escape mode clock and byte clock source */
		s5p_dsim_set_byte_clock_src(dsim_base, byte_clk_sel);

		/* DPHY, DSIM Link : D-PHY clock out */
		if (byte_clk_sel == DSIM_PLL_OUT_DIV8) {
			dsim.hs_clk = s5p_dsim_change_pll(dsim_base,
					dsim.dsim_info->p, dsim.dsim_info->m,
					dsim.dsim_info->s);
			dsim.byte_clk = dsim.hs_clk / 8;
			s5p_dsim_enable_pll_bypass(dsim_base, 0);
			s5p_dsim_pll_on(dsim_base, 1);
		/* DPHY : D-PHY clock out, DSIM link : external clock out */
		} else if (byte_clk_sel == DSIM_EXT_CLK_DIV8)
			dev_warn(dsim.dev, "this project is not supported "
				"external clock source for MIPI DSIM\n");
		else if (byte_clk_sel == DSIM_EXT_CLK_BYPASS)
			dev_warn(dsim.dev, "this project is not supported "
				"external clock source for MIPI DSIM\n");

		/* escape clock divider */
		esc_div = dsim.byte_clk / (dsim.dsim_info->esc_clk);
		dev_dbg(dsim.dev, "esc_div = %d, byte_clk = %lu, "
				"esc_clk = %lu\n",
			esc_div, dsim.byte_clk, dsim.dsim_info->esc_clk);
		if ((dsim.byte_clk / esc_div) >= 20000000 ||
			(dsim.byte_clk / esc_div) > dsim.dsim_info->esc_clk)
			esc_div += 1;

		dsim.escape_clk = dsim.byte_clk / esc_div;
		dev_dbg(dsim.dev, "escape_clk = %lu, byte_clk = %lu, "
				"esc_div = %d\n",
			dsim.escape_clk, dsim.byte_clk, esc_div);

		/*
		 * enable escclk on lane
		 */
		s5p_dsim_enable_byte_clock(dsim_base, DSIM_TRUE);

		/* enable byte clk and escape clock */
		s5p_dsim_set_esc_clk_prs(dsim_base, 1, esc_div);
		/* escape clock on lane */
		s5p_dsim_enable_esc_clk_on_lane(dsim_base, (DSIM_LANE_CLOCK | dsim.data_lane), 1);

		dev_dbg(dsim.dev, "byte clock is %luMHz\n",
				(dsim.byte_clk / 1000000));
		dev_dbg(dsim.dev, "escape clock that user's need is %lu\n",
				(dsim.dsim_info->esc_clk / 1000000));
		dev_dbg(dsim.dev, "escape clock divider is %x\n", esc_div);
		dev_dbg(dsim.dev, "escape clock is %luMHz\n",
				((dsim.byte_clk / esc_div) / 1000000));

		if ((dsim.byte_clk / esc_div) > dsim.escape_clk) {
			esc_clk_error_rate =
				dsim.escape_clk / (dsim.byte_clk / esc_div);
			dev_warn(dsim.dev, "error rate is %lu over.\n",
					(esc_clk_error_rate / 100));
		} else if ((dsim.byte_clk / esc_div) < (dsim.escape_clk)) {
			esc_clk_error_rate =
				(dsim.byte_clk / esc_div) / dsim.escape_clk;
			dev_warn(dsim.dev, "error rate is %lu under.\n",
					(esc_clk_error_rate / 100));
		}
	} else {
		s5p_dsim_enable_esc_clk_on_lane(dsim_base,
				(DSIM_LANE_CLOCK | dsim.data_lane), 0);
		s5p_dsim_set_esc_clk_prs(dsim_base, 0, 0);

		s5p_dsim_enable_byte_clock(dsim_base, DSIM_FALSE);

		if (byte_clk_sel == DSIM_PLL_OUT_DIV8)
			s5p_dsim_pll_on(dsim_base, 0);
	}
}

static int s5p_dsim_late_resume_init_dsim(unsigned int dsim_base)
{
	if (dsim.pd->init_d_phy)
		dsim.pd->init_d_phy(dsim.reg_base);

	if (dsim.pd->cfg_gpio)
		dsim.pd->cfg_gpio();

	dsim.state = DSIM_STATE_RESET;

	switch (dsim.dsim_info->e_no_data_lane) {
	case DSIM_DATA_LANE_1:
		dsim.data_lane = DSIM_LANE_DATA0;
		break;
	case DSIM_DATA_LANE_2:
		dsim.data_lane = DSIM_LANE_DATA0 | DSIM_LANE_DATA1;
		break;
	case DSIM_DATA_LANE_3:
		dsim.data_lane = DSIM_LANE_DATA0 | DSIM_LANE_DATA1 |
			DSIM_LANE_DATA2;
		break;
	case DSIM_DATA_LANE_4:
		dsim.data_lane = DSIM_LANE_DATA0 | DSIM_LANE_DATA1 |
			DSIM_LANE_DATA2 | DSIM_LANE_DATA3;
		break;
	default:
		dev_info(dsim.dev, "data lane is invalid.\n");
		return -1;
	};

	s5p_dsim_init_header_fifo();
	s5p_dsim_sw_reset(dsim_base);
	s5p_dsim_dp_dn_swap(dsim_base, dsim.dsim_info->e_lane_swap);

	/* enable only frame done interrupt */
	/* s5p_dsim_clear_interrupt(dsim_base, AllDsimIntr); */
	s5p_dsim_set_interrupt_mask(dsim.reg_base, AllDsimIntr, 1);

	return 0;
}

static int s5p_dsim_init_dsim(unsigned int dsim_base)
{
	if (dsim.pd->init_d_phy)
		dsim.pd->init_d_phy(dsim.reg_base);

	if (dsim.pd->cfg_gpio)
		dsim.pd->cfg_gpio();

	dsim.state = DSIM_STATE_RESET;

	switch (dsim.dsim_info->e_no_data_lane) {
	case DSIM_DATA_LANE_1:
		dsim.data_lane = DSIM_LANE_DATA0;
		break;
	case DSIM_DATA_LANE_2:
		dsim.data_lane = DSIM_LANE_DATA0 | DSIM_LANE_DATA1;
		break;
	case DSIM_DATA_LANE_3:
		dsim.data_lane = DSIM_LANE_DATA0 | DSIM_LANE_DATA1 |
			DSIM_LANE_DATA2;
		break;
	case DSIM_DATA_LANE_4:
		dsim.data_lane = DSIM_LANE_DATA0 | DSIM_LANE_DATA1 |
			DSIM_LANE_DATA2 | DSIM_LANE_DATA3;
		break;
	default:
		dev_info(dsim.dev, "data lane is invalid.\n");
		return -1;
	};

	s5p_dsim_init_header_fifo();
	s5p_dsim_dp_dn_swap(dsim_base, dsim.dsim_info->e_lane_swap);

	/* enable only frame done interrupt */
	return 0;
}

static void s5p_dsim_set_display_mode(unsigned int dsim_base,
	struct dsim_lcd_config *main_lcd, struct dsim_lcd_config *sub_lcd)
{
	struct s3cfb_lcd *main_lcd_panel_info = NULL, *sub_lcd_panel_info = NULL;
	struct s3cfb_lcd_timing *main_timing = NULL;

	if (main_lcd != NULL) {
		if (main_lcd->lcd_panel_info != NULL) {
			main_lcd_panel_info =
				(struct s3cfb_lcd *) main_lcd->lcd_panel_info;

			s5p_dsim_set_main_disp_resol(dsim_base,
				main_lcd_panel_info->height,
				main_lcd_panel_info->width);
		} else
			dev_warn(dsim.dev, "lcd panel info of main lcd is NULL.\n");
	} else {
		dev_err(dsim.dev, "main lcd is NULL.\n");
		return;
	}

	/* in case of VIDEO MODE (RGB INTERFACE) */
	if (dsim.dsim_lcd_info->e_interface == (u32) DSIM_VIDEO) {

		main_timing = &main_lcd_panel_info->timing;
		if (main_timing == NULL) {
			dev_err(dsim.dev, "main_timing is NULL.\n");
			return;
		}

		/* if (dsim.dsim_info->auto_vertical_cnt == DSIM_FALSE) */
		{
			s5p_dsim_set_main_disp_vporch(dsim_base,
				main_timing->cmd_allow_len,
				main_timing->v_fp, (u16) main_timing->v_bp);
			s5p_dsim_set_main_disp_hporch(dsim_base,
				main_timing->h_fp, (u16) main_timing->h_bp);
			s5p_dsim_set_main_disp_sync_area(dsim_base,
				main_timing->v_sw, (u16) main_timing->h_sw);
		}
	/* in case of COMMAND MODE (CPU or I80 INTERFACE) */
	} else {
		if (sub_lcd != NULL) {
			if (sub_lcd->lcd_panel_info != NULL) {
				sub_lcd_panel_info =
					(struct s3cfb_lcd *)
						sub_lcd->lcd_panel_info;

				s5p_dsim_set_sub_disp_resol(dsim_base,
					sub_lcd_panel_info->height,
					sub_lcd_panel_info->width);
			} else
				dev_warn(dsim.dev, "lcd panel info of sub lcd is NULL.\n");
		}
	}

	s5p_dsim_display_config(dsim_base, dsim.dsim_lcd_info, NULL);
}

static int s5p_dsim_init_link(unsigned int dsim_base)
{
	unsigned int time_out = 100;

	switch (dsim.state) {
	case DSIM_STATE_RESET:
		s5p_dsim_sw_reset(dsim_base);
	case DSIM_STATE_INIT:
		if (!probe_fly){
		s5p_dsim_init_fifo_pointer(dsim_base, 0x0);
		mdelay(10);
		s5p_dsim_init_fifo_pointer(dsim_base, 0x1f);
			}
		/* dsi configuration */
		if (!probe_fly)	{
		s5p_dsim_init_config(dsim_base, dsim.dsim_lcd_info,
				NULL, dsim.dsim_info);
		s5p_dsim_enable_lane(dsim_base, DSIM_LANE_CLOCK, 1);
		s5p_dsim_enable_lane(dsim_base, dsim.data_lane, 1);

		/* set clock configuration */
	 	s5p_dsim_set_clock(dsim_base, dsim.dsim_info->e_byte_clk, 1);
			}
		if (!probe_fly) 
		/* check clock and data lane state is stop state */
		while (!(s5p_dsim_is_lane_state(dsim_base, DSIM_LANE_CLOCK)
				== DSIM_LANE_STATE_STOP) &&
			!(s5p_dsim_is_lane_state(dsim_base, dsim.data_lane)
				== DSIM_LANE_STATE_STOP)) {
			time_out--;
			if (time_out == 0) {
				dev_info(dsim.dev, "DSI Master state is not "
						"stop state!!!\n");
				dev_info(dsim.dev, "Please check "
						"initialization process\n");

				return DSIM_FALSE;
			}
		}

		dsim.state = DSIM_STATE_STOP;

		/* BTA sequence counters */
		s5p_dsim_set_stop_state_counter(dsim_base,
				dsim.dsim_info->stop_holding_cnt);
		s5p_dsim_set_bta_timeout(dsim_base,
				dsim.dsim_info->bta_timeout);
		s5p_dsim_set_lpdr_timeout(dsim_base,
				dsim.dsim_info->rx_timeout);

		/* default LPDT by both cpu and lcd controller */
		if (!probe_fly)
		s5p_dsim_set_data_mode(dsim_base, DSIM_TRANSFER_BOTH,
				DSIM_STATE_STOP);

		return DSIM_TRUE;
	default:
		dev_info(dsim.dev, "DSI Master is already init.\n");

		return DSIM_FALSE;
	}
}

unsigned char s5p_dsim_set_hs_enable(unsigned int dsim_base)
{
	u8 ret =  DSIM_FALSE;

	if (dsim.state == DSIM_STATE_STOP) {
		if (dsim.e_clk_src != DSIM_EXT_CLK_BYPASS) {
			dsim.state = DSIM_STATE_HSCLKEN;
			s5p_dsim_set_data_mode(dsim_base, DSIM_TRANSFER_BOTH,
				DSIM_STATE_HSCLKEN);
			s5p_dsim_enable_hs_clock(dsim_base, 1);

			ret = DSIM_TRUE;
		} else
			dev_warn(dsim.dev, "clock source is external bypass.\n");
	} else
		dev_warn(dsim.dev, "DSIM is not stop state.\n");

	return ret;
}

static unsigned char s5p_dsim_set_data_transfer_mode(unsigned int dsim_base,
	unsigned char data_path, unsigned char hs_enable)
{
	u8 ret = DSIM_FALSE;

	if (hs_enable) {
		if (dsim.state == DSIM_STATE_HSCLKEN) {
			s5p_dsim_set_data_mode(dsim_base, data_path, DSIM_STATE_HSCLKEN);
			ret = DSIM_TRUE;
		} else {
			dev_err(dsim.dev, "HS Clock lane is not enabled.\n");
			ret = DSIM_FALSE;
		}
	} else {
		if (dsim.state == DSIM_STATE_INIT || dsim.state == DSIM_STATE_ULPS) {
			dev_err(dsim.dev, "DSI Master is not STOP or HSDT state.\n");
			ret = DSIM_FALSE;
		} else {
			s5p_dsim_set_data_mode(dsim_base, data_path, DSIM_STATE_STOP);
			ret = DSIM_TRUE;
		}
	}

	return ret;
}

int s5p_dsim_register_lcd_driver(struct mipi_lcd_driver *lcd_drv)
{
	struct mipi_lcd_info	*lcd_info = NULL;

	lcd_info = kmalloc(sizeof(struct mipi_lcd_info), GFP_KERNEL);
	if (lcd_info == NULL)
		return -ENOMEM;

	lcd_info->mipi_drv = kmalloc(sizeof(struct mipi_lcd_driver), GFP_KERNEL);
	if (lcd_info->mipi_drv == NULL)
		return -ENOMEM;


	memcpy(lcd_info->mipi_drv, lcd_drv, sizeof(struct mipi_lcd_driver));

	mutex_lock(&mipi_lock);
	list_add_tail(&lcd_info->list, &lcd_info_list);
	mutex_unlock(&mipi_lock);

	return 0;
}

struct mipi_lcd_driver *scan_mipi_driver(const char *name)
{
	struct mipi_lcd_info *lcd_info;
	struct mipi_lcd_driver *mipi_drv = NULL;

	mutex_lock(&mipi_lock);

	dev_dbg(dsim.dev, "find lcd panel driver(%s).\n",
		name);

	list_for_each_entry(lcd_info, &lcd_info_list, list) {
		mipi_drv = lcd_info->mipi_drv;

		if ((strcmp(mipi_drv->name, name)) == 0) {
			mutex_unlock(&mipi_lock);
			dev_dbg(dsim.dev, "found!!!(%s).\n", mipi_drv->name);
			return mipi_drv;
		}
	}

	dev_warn(dsim.dev, "failed to find lcd panel driver(%s).\n",
		name);

	mutex_unlock(&mipi_lock);

	return NULL;
}

void s5p_dsim_interrupt_mask_set(void)
{
	u32 int_stat;

	int_stat = readl(dsim.reg_base + S5P_DSIM_INTMSK);

	int_stat &= ~((0x01<<S5P_DSIM_INT_BTA) | (0x01<<S5P_DSIM_INT_RX_TIMEOUT) |
		(0x01<<S5P_DSIM_INT_BTA_TIMEOUT) | (0x01 << S5P_DSIM_INT_RX_DONE) |
		(0x01<<S5P_DSIM_INT_RX_TE) | (0x01<<S5P_DSIM_INT_RX_ACK) |
		(0x01<<S5P_DSIM_INT_RX_ECC_ERR) | (0x01<<S5P_DSIM_IMT_RX_CRC_ERR) |
		(0x01<<S5P_DSIM_INT_SFR_FIFO_EMPTY));

	writel(int_stat, dsim.reg_base + S5P_DSIM_INTMSK);
}

#ifdef CONFIG_HAS_EARLYSUSPEND
void s5p_dsim_early_suspend(struct early_suspend *h)
{
	struct regulator *vdd10_mipi_regulator = NULL;//ldo8
	struct regulator *vdd18_mipi_regulator = NULL;//ldo10

	mutex_lock(&mutex_dsim);
	pr_err("[DSIM][%s] \n", __func__);

	dsim.mipi_ddi_pd->resume_complete = 0;

	if (dsim.mipi_drv->suspend)
		dsim.mipi_drv->suspend();

	clk_disable(dsim.clock);

	if (dsim.pd->mipi_power)
		dsim.pd->mipi_power(0);

//	msleep(100);

#ifdef CONFIG_EXYNOS_DEV_PD

	/* disable the power domain */

	pm_runtime_put(dsim.dev);
#endif
#ifdef CONFIG_BUSFREQ_OPP_DSIM_LOCK
	dev_unlock(dsim.bus_dev, dsim.dev);
#endif

	//Cellon add start, Jacob.wu, 2012/11/08, for mipi power

	vdd10_mipi_regulator= regulator_get(NULL, "vdd10_mipi");
	if (IS_ERR(vdd10_mipi_regulator)) {
		pr_err("%s:MIPI failed to get %s\n", __func__, "vdd10_mipi");
		goto out;
	}	

	if (regulator_is_enabled(vdd10_mipi_regulator))
		regulator_disable(vdd10_mipi_regulator);
	
	regulator_put(vdd10_mipi_regulator);

	vdd18_mipi_regulator= regulator_get(NULL, "vdd18_mipi");
	if (IS_ERR(vdd18_mipi_regulator)) {
		pr_err("%s:MIPI failed to get %s\n", __func__, "vdd18_mipi");
		goto out;
	}	

	if (regulator_is_enabled(vdd18_mipi_regulator))
		regulator_disable(vdd18_mipi_regulator);
	
	regulator_put(vdd18_mipi_regulator);

	vdd10_mipi_regulator = NULL;
	vdd18_mipi_regulator = NULL;
	
	mutex_unlock(&mutex_dsim);
	return;
out:
	vdd10_mipi_regulator = NULL;
	vdd18_mipi_regulator = NULL;
	mutex_unlock(&mutex_dsim);
	return;
//Cellon add end, Jacob.wu, 2012/11/08, for mipi power

}

irqreturn_t s5p_dsim_intr(struct platform_device *pdev)
{
	u32 intr;
	u32 dsim_clkcon;
	disable_irq(IRQ_MIPIDSI0);
	intr = readl(dsim.reg_base + S5P_DSIM_INTSRC) & ~(readl(dsim.reg_base + S5P_DSIM_INTMSK));

	/* frame done */
	if (intr & (FrameDone)) {
		writel(FrameDone, dsim.reg_base + S5P_DSIM_INTSRC);
		dsim_clkcon = readl(dsim.reg_base + S5P_DSIM_CLKCTRL) & ~(1 << 31);
		writel(dsim_clkcon, dsim.reg_base + S5P_DSIM_CLKCTRL);
	} else if (intr & ErrDsimIntr) { /* all of error interttup of dsim */
		writel((1 << 16), dsim.reg_base + S5P_DSIM_SWRST);
	} else if (intr & SwRstRelease) {
		writel(SwRstRelease, dsim.reg_base + S5P_DSIM_INTSRC);
		dsim_clkcon = readl(dsim.reg_base + S5P_DSIM_CLKCTRL) | (1 << 31);
		writel(dsim_clkcon, dsim.reg_base + S5P_DSIM_CLKCTRL);
	} else if (intr & (SFRFifoEmpty | PllStable)) {
		writel((SFRFifoEmpty | PllStable), dsim.reg_base + S5P_DSIM_INTSRC);
	} else {
		/* others */
	}
	enable_irq(IRQ_MIPIDSI0);
	return IRQ_HANDLED;
}

//void s5p_dsim_late_resume(struct early_suspend *h)
void s5p_dsim_do_late_resume(void)
{
	 struct regulator *vdd10_mipi_regulator = NULL;//ldo8
	 struct regulator *vdd18_mipi_regulator = NULL;//ldo10
	mutex_lock(&mutex_dsim);	
	if(dsim.mipi_ddi_pd->resume_complete)//ly add for by pass
		return ;
	//Cellon add start, Jacob.wu, 2012/11/08, for mipi power

	vdd10_mipi_regulator= regulator_get(NULL, "vdd10_mipi");
	if (IS_ERR(vdd10_mipi_regulator)) {
		pr_err("%s:MIPI failed to get %s\n", __func__, "vdd10_mipi");
		goto out;
	}	

	if (!regulator_is_enabled(vdd10_mipi_regulator))
		regulator_enable(vdd10_mipi_regulator);
	
	regulator_put(vdd10_mipi_regulator);

	 vdd18_mipi_regulator= regulator_get(NULL, "vdd18_mipi");
	if (IS_ERR(vdd18_mipi_regulator)) {
		pr_err("%s:MIPI failed to get %s\n", __func__, "vdd18_mipi");
		goto out;
	}	

	if (!regulator_is_enabled(vdd18_mipi_regulator))
		regulator_enable(vdd18_mipi_regulator);
	
	regulator_put(vdd18_mipi_regulator);
#ifdef CONFIG_BUSFREQ_OPP_DSIM_LOCK
	dev_lock(dsim.bus_dev, dsim.dev, BUSFREQ_160MHZ);
#endif

#ifdef CONFIG_EXYNOS_DEV_PD
	/* enable the power domain */
	pm_runtime_get_sync(dsim.dev);
#endif
	probe_fly = false; 	// fly
	/* MIPI SIGNAL ON */
	if (dsim.pd->mipi_power)
		dsim.pd->mipi_power(1);

	/* reset lcd */
	dsim.mipi_ddi_pd->lcd_reset();

	clk_enable(dsim.clock);
	
	dsim.mipi_drv->set_link((void *) dsim.mipi_ddi_pd, dsim.reg_base,
		s5p_dsim_wr_data, NULL);

	dsim.state = DSIM_STATE_INIT;
	
	s5p_dsim_late_resume_init_dsim(dsim.reg_base);
	s5p_dsim_init_link(dsim.reg_base);

	s5p_dsim_set_hs_enable(dsim.reg_base);
	s5p_dsim_set_data_transfer_mode(dsim.reg_base, DSIM_TRANSFER_BYCPU, 1);

	/* initialize lcd panel */
	if (dsim.mipi_drv->init)
		dsim.mipi_drv->init();
	else
		dev_warn(dsim.dev, "init func is null.\n");

	s5p_dsim_set_display_mode(dsim.reg_base, dsim.dsim_lcd_info, NULL);

	s5p_dsim_set_data_transfer_mode(dsim.reg_base, DSIM_TRANSFER_BYLCDC, 1);

	s5p_dsim_set_interrupt_mask(dsim.reg_base, AllDsimIntr, 0);

	dsim.mipi_ddi_pd->resume_complete = 1;

	s5p_dsim_frame_done_interrupt_enable(1);

	printk("[DSIM][%s]   done\n", __func__);

	vdd10_mipi_regulator = NULL;
	vdd18_mipi_regulator = NULL;

	mutex_unlock(&mutex_dsim);
	return;
	out:
		vdd10_mipi_regulator = NULL;
		vdd18_mipi_regulator = NULL;
		mutex_unlock(&mutex_dsim);
		return;
}

void s5p_dsim_late_resume(struct early_suspend *h)
{
	s5p_dsim_do_late_resume();
}

int  s5p_dsim_do_suspend(struct platform_device *pdev, pm_message_t state)
{
	return 0;
}
int s5p_dsim_do_resume(struct platform_device *pdev)
{
	return 0;
}
#else
#ifdef CONFIG_PM
int s5p_dsim_suspend(struct platform_device *pdev, pm_message_t state)
{
	dsim.mipi_ddi_pd->resume_complete = 0;

	if (dsim.mipi_drv->suspend)
		dsim.mipi_drv->suspend(&pdev->dev, state);

	clk_disable(dsim.clock);

	if (dsim.pd->mipi_power)
		dsim.pd->mipi_power(0);
	return 0;

}

int s5p_dsim_resume(struct platform_device *pdev)
{
	u32 int_stat;
	if (dsim.pd->mipi_power)
		dsim.pd->mipi_power(1);

	/* reset lcd */
	dsim.mipi_ddi_pd->lcd_reset();

	mdelay(10);

	clk_enable(dsim.clock);

	if (dsim.mipi_drv->resume)
		dsim.mipi_drv->resume(&pdev->dev);

	s5p_dsim_init_dsim(dsim.reg_base);
	s5p_dsim_init_link(dsim.reg_base);

	s5p_dsim_set_hs_enable(dsim.reg_base);
	s5p_dsim_set_data_transfer_mode(dsim.reg_base, DSIM_TRANSFER_BYCPU, 1);

	mdelay(10);

	/* initialize lcd panel */
	if (dsim.mipi_drv->init)
		dsim.mipi_drv->init(&pdev->dev);
	else
		dev_warn(&pdev->dev, "init func is null.\n");

	s5p_dsim_set_display_mode(dsim.reg_base, dsim.dsim_lcd_info, NULL);

	s5p_dsim_set_data_transfer_mode(dsim.reg_base, DSIM_TRANSFER_BYLCDC, 1);

	int_stat = readl(dsim.reg_base + S5P_DSIM_INTMSK);

	int_stat &= ~((0x01<<S5P_DSIM_INT_BTA) | (0x01<<S5P_DSIM_INT_RX_TIMEOUT) |
		(0x01<<S5P_DSIM_INT_BTA_TIMEOUT) | (0x01 << S5P_DSIM_INT_RX_DONE) |
		(0x01<<S5P_DSIM_INT_RX_TE) | (0x01<<S5P_DSIM_INT_RX_ACK) |
		(0x01<<S5P_DSIM_INT_RX_ECC_ERR) | (0x01<<S5P_DSIM_IMT_RX_CRC_ERR) |
		(0x01<<S5P_DSIM_INT_SFR_FIFO_EMPTY));

	writel(int_stat, dsim.reg_base + S5P_DSIM_INTMSK);

	dsim.mipi_ddi_pd->resume_complete = 1;

	return 0;
}
#else
#define s5p_dsim_suspend NULL
#define s5p_dsim_resume NULL
#endif
#endif

u32 read_dsim_register(u32 num)
{
	struct dsim_global *dsim = g_dsim;

	return readl(dsim->reg_base + (num*4));
}

static struct dsim_ops s5p_dsim_ops = {
	.cmd_write	= s5p_dsim_wr_data,
	.cmd_read	= s5p_dsim_rd_data,
	.cmd_dcs_read	= s5p_dsim_dcs_rd_data,
	.suspend	= s5p_dsim_early_suspend,
	.resume		= s5p_dsim_late_resume,
};

static int s5p_dsim_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret = -1;
	u32 int_stat;
	struct regulator *vdd10_mipi_regulator = NULL;//ldo8
	struct regulator *vdd18_mipi_regulator = NULL;//ldo10

	mutex_init(&mutex_dsim);
	
	vdd10_mipi_regulator= regulator_get(NULL, "vdd10_mipi");
	if (IS_ERR(vdd10_mipi_regulator)) {
		pr_err("%s:MIPI failed to get %s\n", __func__, "vdd10_mipi");
		goto out;
	}	
		regulator_enable(vdd10_mipi_regulator);
	
	regulator_put(vdd10_mipi_regulator);
	
  	vdd18_mipi_regulator= regulator_get(NULL, "vdd18_mipi");
	if (IS_ERR(vdd18_mipi_regulator)) {
		pr_err("%s:MIPI failed to get %s\n", __func__, "vdd18_mipi");
		goto out;
	}	
		regulator_enable(vdd18_mipi_regulator);
	
	regulator_put(vdd18_mipi_regulator);

	dsim.pd = to_dsim_plat(&pdev->dev);
	dsim.dev = &pdev->dev;
#ifdef CONFIG_EXYNOS_DEV_PD
	/* to use the runtime PM helper functions */
	pm_runtime_enable(&pdev->dev);
	/* enable the power domain */
	pm_runtime_get_sync(&pdev->dev);
#endif
#ifdef CONFIG_BUSFREQ_OPP_DSIM_LOCK
	dsim.bus_dev = dev_get("exynos-busfreq");
	dev_lock(dsim.bus_dev, dsim.dev, BUSFREQ_160MHZ);
#endif

	/* set dsim config data, dsim lcd config data and lcd panel data. */
	dsim.dsim_info = dsim.pd->dsim_info;
	dsim.dsim_lcd_info = dsim.pd->dsim_lcd_info;
	dsim.lcd_panel_info =
		(struct s3cfb_lcd *)dsim.dsim_lcd_info->lcd_panel_info;
	dsim.mipi_ddi_pd =
		(struct mipi_ddi_platform_data *)dsim.dsim_lcd_info->mipi_ddi_pd;
	dsim.mipi_ddi_pd->te_irq = dsim.pd->te_irq;

	dsim.mipi_ddi_pd->resume_complete = 0;

	/* reset lcd */
	/* clock */
	dsim.clock = clk_get(&pdev->dev, dsim.pd->clk_name);
	if (IS_ERR(dsim.clock)) {
		dev_err(&pdev->dev, "failed to get dsim clock source\n");
		vdd10_mipi_regulator = NULL;
		vdd18_mipi_regulator = NULL;
		return -EINVAL;
	}

	clk_enable(dsim.clock);

	/* io memory */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "failed to get io memory region\n");
		ret = -EINVAL;
		goto err_clk_disable;
	}

	/* request mem region */
	res = request_mem_region(res->start,
				 res->end - res->start + 1, pdev->name);
	if (!res) {
		dev_err(&pdev->dev, "failed to request io memory region\n");
		ret = -EINVAL;
		goto err_clk_disable;
	}

	/* ioremap for register block */
	dsim.reg_base = (unsigned int) ioremap(res->start,
			res->end - res->start + 1);
	if (!dsim.reg_base) {
		dev_err(&pdev->dev, "failed to remap io region\n");
		ret = -EINVAL;
		goto err_clk_disable;
	}

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev, "failed to request dsim irq resource\n");
		ret = -EINVAL;
		goto err_clk_disable;
	}

	/* clear interrupt */
	int_stat = 0xffffffff;
	writel(int_stat, dsim.reg_base + S5P_DSIM_INTSRC);

	/* enable interrupts */
	int_stat = readl(dsim.reg_base + S5P_DSIM_INTMSK);

	int_stat &= ~((0x01<<S5P_DSIM_INT_BTA) | (0x01<<S5P_DSIM_INT_RX_TIMEOUT) |
		(0x01<<S5P_DSIM_INT_BTA_TIMEOUT) | (0x01 << S5P_DSIM_INT_RX_DONE) |
		(0x01<<S5P_DSIM_INT_RX_TE) | (0x01<<S5P_DSIM_INT_RX_ACK) |
		(0x01<<S5P_DSIM_INT_RX_ECC_ERR) | (0x01<<S5P_DSIM_IMT_RX_CRC_ERR) |
		(0x01<<S5P_DSIM_INT_SFR_FIFO_EMPTY));

	writel(int_stat, dsim.reg_base + S5P_DSIM_INTMSK);
	ret = request_irq(res->start, (void *)s5p_dsim_isr, IRQF_DISABLED, pdev->name, &dsim);
	if (ret != 0) {
		dev_err(&pdev->dev, "failed to request dsim irq\n");
		ret = -EINVAL;
		goto err_clk_disable;
	}

	/* find lcd panel driver registered to mipi-dsi driver. */
	dsim.mipi_drv = scan_mipi_driver(dsim.pd->lcd_panel_name);
	if (dsim.mipi_drv == NULL) {
		dev_err(&pdev->dev, "mipi_drv is NULL.\n");
		goto mipi_drv_err;
	}
//Specific LCD driver function execute start 
	/* set lcd panel driver link */
	dsim.mipi_drv->set_link((void *) dsim.mipi_ddi_pd, dsim.reg_base,
		s5p_dsim_wr_data, NULL);

	dsim.mipi_drv->probe(&pdev->dev);
	s5p_dsim_init_dsim(dsim.reg_base);
	dsim.state = DSIM_STATE_INIT;
	s5p_dsim_init_link(dsim.reg_base);

	s5p_dsim_set_hs_enable(dsim.reg_base);
	s5p_dsim_set_data_transfer_mode(dsim.reg_base, DSIM_TRANSFER_BYCPU, 1);

	/* initialize lcd panel */

	s5p_dsim_set_data_transfer_mode(dsim.reg_base, DSIM_TRANSFER_BYLCDC, 1);
	
	s5p_dsim_frame_done_interrupt_enable(1);
	
	dev_info(&pdev->dev, "mipi-dsi driver has been probed.\n");
//Specific LCD driver function execute end 

#ifdef CONFIG_HAS_WAKELOCK
#ifdef CONFIG_HAS_EARLYSUSPEND
	dsim.early_suspend.suspend = s5p_dsim_early_suspend;
	dsim.early_suspend.resume = s5p_dsim_late_resume;
	dsim.early_suspend.level = EARLY_SUSPEND_LEVEL_DISABLE_FB;
	register_early_suspend(&dsim.early_suspend);
#endif
#endif
	// fly
	#if 1

	#else
	INIT_DELAYED_WORK(&(dsim.lp_work_low), delayed_work_low);
	INIT_DELAYED_WORK(&(dsim.lp_work_high), delayed_work_high);
	schedule_delayed_work(&(dsim.lp_work_high), msecs_to_jiffies(0));
	#endif

	
	vdd10_mipi_regulator = NULL;
	vdd18_mipi_regulator = NULL;
	
	return 0;

mipi_drv_err:
	dsim.pd->mipi_power(0);
	iounmap((void __iomem *) dsim.reg_base);
err_clk_disable:
	clk_disable(dsim.clock);
out:	
	ret = -EINVAL;
	vdd10_mipi_regulator = NULL;
	vdd18_mipi_regulator = NULL;
	return ret;
}

static int s5p_dsim_remove(struct platform_device *pdev)
{
#ifdef CONFIG_EXYNOS_DEV_PD
	/* disable the power domain */
	pm_runtime_put(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
#endif
	return 0;
}

static void s5p_dsim_shutdown(struct platform_device *pdev)
{
  	struct regulator *vdd10_mipi_regulator = NULL;
  	struct regulator *vdd18_mipi_regulator = NULL;

	lcd_off();

	vdd10_mipi_regulator= regulator_get(NULL, "vdd10_mipi");
	if (IS_ERR(vdd10_mipi_regulator)) {
		pr_err("%s:MIPI failed to get %s\n", __func__, "vdd10_mipi");
	}	

	if (!regulator_is_enabled(vdd10_mipi_regulator))
		regulator_enable(vdd10_mipi_regulator);
	
	regulator_put(vdd10_mipi_regulator);
	
  	vdd18_mipi_regulator= regulator_get(NULL, "vdd18_mipi");
	if (IS_ERR(vdd18_mipi_regulator)) {
		pr_err("%s:MIPI failed to get %s\n", __func__, "vdd18_mipi");
	}	

	if (!regulator_is_enabled(vdd18_mipi_regulator))
		regulator_enable(vdd18_mipi_regulator);
	
	regulator_put(vdd18_mipi_regulator);
	usleep_range(20000,20000);//wait for mipi power stable
//Cellon add end, Jacob.wu, 2012/11/08, for mipi power
	pr_err("[DSIM][%s] \n", __func__);
//	printk("s5p_dsim_late_resume is called\n");
#ifdef CONFIG_EXYNOS_DEV_PD
	/* enable the power domain */
	pm_runtime_get_sync(dsim.dev);
#endif

	clk_enable(dsim.clock);

	vdd10_mipi_regulator = NULL;
	vdd18_mipi_regulator = NULL;
		
	return;
	
}
#ifdef CONFIG_EXYNOS_DEV_PD
static int s5p_dsim_runtime_suspend(struct device *dev)
{
	return 0;
}

static int s5p_dsim_runtime_resume(struct device *dev)
{
	return 0;
}

static const struct dev_pm_ops s5p_dsim_pm_ops = {
	.runtime_suspend = s5p_dsim_runtime_suspend,
	.runtime_resume = s5p_dsim_runtime_resume,
};
#endif

static struct platform_driver s5p_dsim_driver = {
	.probe = s5p_dsim_probe,
	.remove = s5p_dsim_remove,
	.shutdown = s5p_dsim_shutdown,
	.suspend = s5p_dsim_do_suspend,
	.resume = s5p_dsim_do_resume,
	.driver = {
		   .name = "s5p-dsim",
		   .owner = THIS_MODULE,
#ifdef CONFIG_EXYNOS_DEV_PD
		   .pm	= &s5p_dsim_pm_ops,
#endif
		   },

};

static int s5p_dsim_register(void)
{
	platform_driver_register(&s5p_dsim_driver);
	return 0;
}

static void s5p_dsim_unregister(void)
{
	platform_driver_unregister(&s5p_dsim_driver);
}

module_init(s5p_dsim_register);
module_exit(s5p_dsim_unregister);

MODULE_AUTHOR("InKi Dae <inki.dae@samsung.com>");
MODULE_DESCRIPTION("Samusung MIPI-DSIM driver");
MODULE_LICENSE("GPL");
