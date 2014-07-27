/*
 * Linux cfg80211 driver
 *
<<<<<<< HEAD
 * Copyright (C) 1999-2012, Broadcom Corporation
=======
 * Copyright (C) 1999-2014, Broadcom Corporation
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
 * 
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
<<<<<<< HEAD
 * $Id: wl_cfg80211.c 374275 2012-12-12 11:44:18Z $
 */

=======
 * $Id: wl_cfg80211.c 464537 2014-03-25 06:28:32Z $
 */
/* */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#include <typedefs.h>
#include <linuxver.h>
#include <osl.h>
#include <linux/kernel.h>

#include <bcmutils.h>
#include <bcmwifi_channels.h>
#include <bcmendian.h>
#include <proto/ethernet.h>
#include <proto/802.11.h>
#include <linux/if_arp.h>
#include <asm/uaccess.h>

#include <dngl_stats.h>
#include <dhd.h>
<<<<<<< HEAD
#include <dhdioctl.h>
#include <wlioctl.h>
#include <dhd_cfg80211.h>
=======
#include <dhd_linux.h>
#include <dhdioctl.h>
#include <wlioctl.h>
#include <dhd_cfg80211.h>
#ifdef PNO_SUPPORT
#include <dhd_pno.h>
#endif /* PNO_SUPPORT */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#include <proto/ethernet.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/netdevice.h>
#include <linux/sched.h>
#include <linux/etherdevice.h>
#include <linux/wireless.h>
#include <linux/ieee80211.h>
#include <linux/wait.h>
#include <net/cfg80211.h>
#include <net/rtnetlink.h>
<<<<<<< HEAD
=======

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#include <wlioctl.h>
#include <wldev_common.h>
#include <wl_cfg80211.h>
#include <wl_cfgp2p.h>
#include <wl_android.h>

#ifdef PROP_TXSTATUS
#include <dhd_wlfc.h>
#endif

<<<<<<< HEAD
#ifdef WL11U
#ifndef WL_ENABLE_P2P_IF
#error "You should enable WL_ENABLE_P2P_IF and Only supported in JB"
#endif
#endif /* WL11U */


#define IW_WSEC_ENABLED(wsec)   ((wsec) & (WEP_ENABLED | TKIP_ENABLED | AES_ENABLED))

static struct device *cfg80211_parent_dev = NULL;
struct wl_priv *wlcfg_drv_priv = NULL;
u32 wl_dbg_level = WL_DBG_ERR;
=======
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 13, 0)) || defined(WL_VENDOR_EXT_SUPPORT)
#include <wl_cfgvendor.h>
#endif /* (LINUX_VERSION_CODE > KERNEL_VERSION(3, 13, 0)) || defined(WL_VENDOR_EXT_SUPPORT) */
#ifdef WL11U
#if !defined(WL_ENABLE_P2P_IF) && !defined(WL_CFG80211_P2P_DEV_IF)
#error You should enable 'WL_ENABLE_P2P_IF' or 'WL_CFG80211_P2P_DEV_IF' \
	according to Kernel version and is supported only in Android-JB
#endif /* !WL_ENABLE_P2P_IF && !WL_CFG80211_P2P_DEV_IF */
#endif /* WL11U */

#ifdef BCMWAPI_WPI
/* these items should evetually go into wireless.h of the linux system headfile dir */
#ifndef IW_ENCODE_ALG_SM4
#define IW_ENCODE_ALG_SM4 0x20
#endif

#ifndef IW_AUTH_WAPI_ENABLED
#define IW_AUTH_WAPI_ENABLED 0x20
#endif

#ifndef IW_AUTH_WAPI_VERSION_1
#define IW_AUTH_WAPI_VERSION_1  0x00000008
#endif

#ifndef IW_AUTH_CIPHER_SMS4
#define IW_AUTH_CIPHER_SMS4     0x00000020
#endif

#ifndef IW_AUTH_KEY_MGMT_WAPI_PSK
#define IW_AUTH_KEY_MGMT_WAPI_PSK 4
#endif

#ifndef IW_AUTH_KEY_MGMT_WAPI_CERT
#define IW_AUTH_KEY_MGMT_WAPI_CERT 8
#endif
#endif /* BCMWAPI_WPI */

#ifdef BCMWAPI_WPI
#define IW_WSEC_ENABLED(wsec)   ((wsec) & (WEP_ENABLED | TKIP_ENABLED | AES_ENABLED | SMS4_ENABLED))
#else /* BCMWAPI_WPI */
#define IW_WSEC_ENABLED(wsec)   ((wsec) & (WEP_ENABLED | TKIP_ENABLED | AES_ENABLED))
#endif /* BCMWAPI_WPI */

static struct device *cfg80211_parent_dev = NULL;
/* g_bcm_cfg should be static. Do not change */
static struct bcm_cfg80211 *g_bcm_cfg = NULL;
#ifdef CUSTOMER_HW4
u32 wl_dbg_level = WL_DBG_ERR | WL_DBG_P2P_ACTION;
#else
u32 wl_dbg_level = WL_DBG_ERR;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#define MAX_WAIT_TIME 1500

#ifdef VSDB
/* sleep time to keep STA's connecting or connection for continuous af tx or finding a peer */
<<<<<<< HEAD
#define DEFAULT_SLEEP_TIME_VSDB 	200
#define OFF_CHAN_TIME_THRESHOLD_MS	200

/* if sta is connected or connecting, sleep for a while before retry af tx or finding a peer */
#define WL_AF_TX_KEEP_PRI_CONNECTION_VSDB(wl)	\
	do {	\
		if (wl_get_drv_status(wl, CONNECTED, wl_to_prmry_ndev(wl)) ||	\
			wl_get_drv_status(wl, CONNECTING, wl_to_prmry_ndev(wl))) {	\
			msleep(DEFAULT_SLEEP_TIME_VSDB);	\
=======
#define DEFAULT_SLEEP_TIME_VSDB		120
#define OFF_CHAN_TIME_THRESHOLD_MS	200
#define AF_RETRY_DELAY_TIME			40

/* if sta is connected or connecting, sleep for a while before retry af tx or finding a peer */
#define WL_AF_TX_KEEP_PRI_CONNECTION_VSDB(cfg)	\
	do {	\
		if (wl_get_drv_status(cfg, CONNECTED, bcmcfg_to_prmry_ndev(cfg)) ||	\
			wl_get_drv_status(cfg, CONNECTING, bcmcfg_to_prmry_ndev(cfg))) {	\
			OSL_SLEEP(DEFAULT_SLEEP_TIME_VSDB);			\
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}	\
	} while (0)
#else /* VSDB */
/* if not VSDB, do nothing */
<<<<<<< HEAD
#define WL_AF_TX_KEEP_PRI_CONNECTION_VSDB(wl)
#endif /* VSDB */

#ifdef WL_CFG80211_SYNC_GON
#define WL_DRV_STATUS_SENDING_AF_FRM_EXT(wl) \
	(wl_get_drv_status_all(wl, SENDING_ACT_FRM) || \
		wl_get_drv_status_all(wl, WAITING_NEXT_ACT_FRM_LISTEN))
#else
#define WL_DRV_STATUS_SENDING_AF_FRM_EXT(wl) wl_get_drv_status_all(wl, SENDING_ACT_FRM)
#endif /* WL_CFG80211_SYNC_GON */

#define WL_CHANSPEC_CTL_SB_NONE WL_CHANSPEC_CTL_SB_LLL


#define DNGL_FUNC(func, parameters) func parameters;
=======
#define WL_AF_TX_KEEP_PRI_CONNECTION_VSDB(cfg)
#endif /* VSDB */

#ifdef WL_CFG80211_SYNC_GON
#define WL_DRV_STATUS_SENDING_AF_FRM_EXT(cfg) \
	(wl_get_drv_status_all(cfg, SENDING_ACT_FRM) || \
		wl_get_drv_status_all(cfg, WAITING_NEXT_ACT_FRM_LISTEN))
#else
#define WL_DRV_STATUS_SENDING_AF_FRM_EXT(cfg) wl_get_drv_status_all(cfg, SENDING_ACT_FRM)
#endif /* WL_CFG80211_SYNC_GON */

#define DNGL_FUNC(func, parameters) func parameters
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#define COEX_DHCP

#define WLAN_EID_SSID	0
#define CH_MIN_5G_CHANNEL 34
#define CH_MIN_2G_CHANNEL 1

/* This is to override regulatory domains defined in cfg80211 module (reg.c)
 * By default world regulatory domain defined in reg.c puts the flags NL80211_RRF_PASSIVE_SCAN
 * and NL80211_RRF_NO_IBSS for 5GHz channels (for 36..48 and 149..165).
 * With respect to these flags, wpa_supplicant doesn't start p2p operations on 5GHz channels.
 * All the chnages in world regulatory domain are to be done here.
 */
static const struct ieee80211_regdomain brcm_regdom = {
	.n_reg_rules = 4,
	.alpha2 =  "99",
	.reg_rules = {
		/* IEEE 802.11b/g, channels 1..11 */
		REG_RULE(2412-10, 2472+10, 40, 6, 20, 0),
		/* If any */
		/* IEEE 802.11 channel 14 - Only JP enables
		 * this and for 802.11b only
		 */
		REG_RULE(2484-10, 2484+10, 20, 6, 20, 0),
		/* IEEE 802.11a, channel 36..64 */
		REG_RULE(5150-10, 5350+10, 40, 6, 20, 0),
		/* IEEE 802.11a, channel 100..165 */
		REG_RULE(5470-10, 5850+10, 40, 6, 20, 0), }
};

<<<<<<< HEAD
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)) && \
	(defined(WL_IFACE_COMB_NUM_CHANNELS) || defined(WL_CFG80211_P2P_DEV_IF))
/*
 * Possible interface combinations supported by driver
 *
 * ADHOC Mode     - #ADHOC <= 1 on channels = 1
 * SoftAP Mode    - #AP <= 1 on channels = 1
 * STA + P2P Mode - #STA <= 2, #{P2P-GO, P2P-client} <= 1, #P2P-device <= 1
 *                  on channels = 2
 */
static const struct ieee80211_iface_limit common_if_limits[] = {
	{
	.max = 1,
	.types = BIT(NL80211_IFTYPE_AP),
	},
	{
	/*
	 * During P2P-GO removal, P2P-GO is first changed to STA and later only
	 * removed. So setting maximum possible number of STA interfaces according
	 * to kernel version.
	 *
	 * less than linux-3.8 - max:3 (wlan0 + p2p0 + group removal of p2p-p2p0-x)
	 * linux-3.8 and above - max:2 (wlan0 + group removal of p2p-wlan0-x)
	 */
#ifdef WL_ENABLE_P2P_IF
	.max = 3,
#else
	.max = 2,
#endif /* WL_ENABLE_P2P_IF */
	.types = BIT(NL80211_IFTYPE_STATION),
	},
	{
	.max = 2,
	.types = BIT(NL80211_IFTYPE_P2P_GO) | BIT(NL80211_IFTYPE_P2P_CLIENT),
	},
#if defined(WL_CFG80211_P2P_DEV_IF)
	{
	.max = 1,
	.types = BIT(NL80211_IFTYPE_P2P_DEVICE),
	},
#endif /* WL_CFG80211_P2P_DEV_IF */
	{
	.max = 1,
	.types = BIT(NL80211_IFTYPE_ADHOC),
	},
};
#ifdef BCM4330_CHIP
#define NUM_DIFF_CHANNELS 1
#else
#define NUM_DIFF_CHANNELS 2
#endif
static const struct ieee80211_iface_combination
common_iface_combinations[] = {
	{
	.num_different_channels = NUM_DIFF_CHANNELS,
	.max_interfaces = 4,
	.limits = common_if_limits,
	.n_limits = ARRAY_SIZE(common_if_limits),
	},
};
#endif /* LINUX_VER >= 3.0 && (WL_IFACE_COMB_NUM_CHANNELS || WL_CFG80211_P2P_DEV_IF) */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

/* Data Element Definitions */
#define WPS_ID_CONFIG_METHODS     0x1008
#define WPS_ID_REQ_TYPE           0x103A
#define WPS_ID_DEVICE_NAME        0x1011
#define WPS_ID_VERSION            0x104A
#define WPS_ID_DEVICE_PWD_ID      0x1012
#define WPS_ID_REQ_DEV_TYPE       0x106A
#define WPS_ID_SELECTED_REGISTRAR_CONFIG_METHODS 0x1053
#define WPS_ID_PRIM_DEV_TYPE      0x1054

/* Device Password ID */
#define DEV_PW_DEFAULT 0x0000
#define DEV_PW_USER_SPECIFIED 0x0001,
#define DEV_PW_MACHINE_SPECIFIED 0x0002
#define DEV_PW_REKEY 0x0003
#define DEV_PW_PUSHBUTTON 0x0004
#define DEV_PW_REGISTRAR_SPECIFIED 0x0005

/* Config Methods */
#define WPS_CONFIG_USBA 0x0001
#define WPS_CONFIG_ETHERNET 0x0002
#define WPS_CONFIG_LABEL 0x0004
#define WPS_CONFIG_DISPLAY 0x0008
#define WPS_CONFIG_EXT_NFC_TOKEN 0x0010
#define WPS_CONFIG_INT_NFC_TOKEN 0x0020
#define WPS_CONFIG_NFC_INTERFACE 0x0040
#define WPS_CONFIG_PUSHBUTTON 0x0080
#define WPS_CONFIG_KEYPAD 0x0100
#define WPS_CONFIG_VIRT_PUSHBUTTON 0x0280
#define WPS_CONFIG_PHY_PUSHBUTTON 0x0480
#define WPS_CONFIG_VIRT_DISPLAY 0x2008
#define WPS_CONFIG_PHY_DISPLAY 0x4008

#define PM_BLOCK 1
#define PM_ENABLE 0

<<<<<<< HEAD

#ifndef RSSI_OFFSET
#define RSSI_OFFSET	0
=======
#ifdef BCMCCX
#ifndef WLAN_AKM_SUITE_CCKM
#define WLAN_AKM_SUITE_CCKM 0x00409600
#endif
#define DOT11_LEAP_AUTH	0x80 /* LEAP auth frame paylod constants */
#endif /* BCMCCX */

#ifdef MFP
#define WL_AKM_SUITE_MFP_1X  0x000FAC05
#define WL_AKM_SUITE_MFP_PSK 0x000FAC06
#endif /* MFP */

#ifndef IBSS_COALESCE_ALLOWED
#define IBSS_COALESCE_ALLOWED 0
#endif

#ifndef IBSS_INITIAL_SCAN_ALLOWED
#define IBSS_INITIAL_SCAN_ALLOWED 0
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#endif
/*
 * cfg80211_ops api/callback list
 */
static s32 wl_frame_get_mgmt(u16 fc, const struct ether_addr *da,
	const struct ether_addr *sa, const struct ether_addr *bssid,
	u8 **pheader, u32 *body_len, u8 *pbody);
static s32 __wl_cfg80211_scan(struct wiphy *wiphy, struct net_device *ndev,
	struct cfg80211_scan_request *request,
	struct cfg80211_ssid *this_ssid);
<<<<<<< HEAD
static s32 wl_cfg80211_scan(struct wiphy *wiphy, struct net_device *ndev,
	struct cfg80211_scan_request *request);
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
static s32
wl_cfg80211_scan(struct wiphy *wiphy, struct cfg80211_scan_request *request);
#else
static s32
wl_cfg80211_scan(struct wiphy *wiphy, struct net_device *ndev,
	struct cfg80211_scan_request *request);
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32 wl_cfg80211_set_wiphy_params(struct wiphy *wiphy, u32 changed);
static s32 wl_cfg80211_join_ibss(struct wiphy *wiphy, struct net_device *dev,
	struct cfg80211_ibss_params *params);
static s32 wl_cfg80211_leave_ibss(struct wiphy *wiphy,
	struct net_device *dev);
static s32 wl_cfg80211_get_station(struct wiphy *wiphy,
	struct net_device *dev, u8 *mac,
	struct station_info *sinfo);
static s32 wl_cfg80211_set_power_mgmt(struct wiphy *wiphy,
	struct net_device *dev, bool enabled,
	s32 timeout);
static int wl_cfg80211_connect(struct wiphy *wiphy, struct net_device *dev,
	struct cfg80211_connect_params *sme);
static s32 wl_cfg80211_disconnect(struct wiphy *wiphy, struct net_device *dev,
	u16 reason_code);
<<<<<<< HEAD
static s32 wl_cfg80211_set_tx_power(struct wiphy *wiphy,
	enum nl80211_tx_power_setting type,
	s32 dbm);
static s32 wl_cfg80211_get_tx_power(struct wiphy *wiphy, s32 *dbm);
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
static s32
wl_cfg80211_set_tx_power(struct wiphy *wiphy, struct wireless_dev *wdev,
	enum nl80211_tx_power_setting type, s32 mbm);
#else
static s32
wl_cfg80211_set_tx_power(struct wiphy *wiphy,
	enum nl80211_tx_power_setting type, s32 dbm);
#endif /* WL_CFG80211_P2P_DEV_IF */
#if defined(WL_CFG80211_P2P_DEV_IF)
static s32 wl_cfg80211_get_tx_power(struct wiphy *wiphy,
	struct wireless_dev *wdev, s32 *dbm);
#else
static s32 wl_cfg80211_get_tx_power(struct wiphy *wiphy, s32 *dbm);
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32 wl_cfg80211_config_default_key(struct wiphy *wiphy,
	struct net_device *dev,
	u8 key_idx, bool unicast, bool multicast);
static s32 wl_cfg80211_add_key(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, bool pairwise, const u8 *mac_addr,
	struct key_params *params);
static s32 wl_cfg80211_del_key(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, bool pairwise, const u8 *mac_addr);
static s32 wl_cfg80211_get_key(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, bool pairwise, const u8 *mac_addr,
	void *cookie, void (*callback) (void *cookie,
	struct key_params *params));
static s32 wl_cfg80211_config_default_mgmt_key(struct wiphy *wiphy,
	struct net_device *dev,	u8 key_idx);
static s32 wl_cfg80211_resume(struct wiphy *wiphy);
#if defined(WL_SUPPORT_BACKPORTED_KPATCHES) || (LINUX_VERSION_CODE >= KERNEL_VERSION(3, \
	2, 0))
static s32 wl_cfg80211_mgmt_tx_cancel_wait(struct wiphy *wiphy,
<<<<<<< HEAD
	struct net_device *dev, u64 cookie);
static s32 wl_cfg80211_del_station(struct wiphy *wiphy,
	struct net_device *ndev, u8* mac_addr);
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)
=======
	bcm_struct_cfgdev *cfgdev, u64 cookie);
static s32 wl_cfg80211_del_station(struct wiphy *wiphy,
	struct net_device *ndev, u8* mac_addr);
static s32 wl_cfg80211_change_station(struct wiphy *wiphy,
	struct net_device *dev, u8 *mac, struct station_parameters *params);
#endif /* WL_SUPPORT_BACKPORTED_KPATCHES || KERNEL_VER >= KERNEL_VERSION(3, 2, 0)) */
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) || defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32 wl_cfg80211_suspend(struct wiphy *wiphy, struct cfg80211_wowlan *wow);
#else
static s32 wl_cfg80211_suspend(struct wiphy *wiphy);
#endif
static s32 wl_cfg80211_set_pmksa(struct wiphy *wiphy, struct net_device *dev,
	struct cfg80211_pmksa *pmksa);
static s32 wl_cfg80211_del_pmksa(struct wiphy *wiphy, struct net_device *dev,
	struct cfg80211_pmksa *pmksa);
static s32 wl_cfg80211_flush_pmksa(struct wiphy *wiphy,
	struct net_device *dev);
<<<<<<< HEAD
static s32 wl_notify_escan_complete(struct wl_priv *wl,
	struct net_device *ndev, bool aborted, bool fw_abort);
/*
 * event & event Q handlers for cfg80211 interfaces
 */
static s32 wl_create_event_handler(struct wl_priv *wl);
static void wl_destroy_event_handler(struct wl_priv *wl);
static s32 wl_event_handler(void *data);
static void wl_init_eq(struct wl_priv *wl);
static void wl_flush_eq(struct wl_priv *wl);
static unsigned long wl_lock_eq(struct wl_priv *wl);
static void wl_unlock_eq(struct wl_priv *wl, unsigned long flags);
static void wl_init_eq_lock(struct wl_priv *wl);
static void wl_init_event_handler(struct wl_priv *wl);
static struct wl_event_q *wl_deq_event(struct wl_priv *wl);
static s32 wl_enq_event(struct wl_priv *wl, struct net_device *ndev, u32 type,
	const wl_event_msg_t *msg, void *data);
static void wl_put_event(struct wl_event_q *e);
static void wl_wakeup_event(struct wl_priv *wl);
static s32 wl_notify_connect_status_ap(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
static s32 wl_notify_connect_status(struct wl_priv *wl,
	struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
static s32 wl_notify_roaming_status(struct wl_priv *wl,
	struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
static s32 wl_notify_scan_status(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
static s32 wl_bss_connect_done(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data, bool completed);
static s32 wl_bss_roaming_done(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
static s32 wl_notify_mic_status(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
#ifdef WL_SCHED_SCAN
static s32
wl_notify_sched_scan_results(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
#endif /* WL_SCHED_SCAN */
#ifdef PNO_SUPPORT
static s32 wl_notify_pfn_status(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
#endif /* PNO_SUPPORT */
static s32 wl_notifier_change_state(struct wl_priv *wl, struct net_info *_net_info,
	enum wl_status state, bool set);
=======
static void wl_cfg80211_scan_abort(struct bcm_cfg80211 *cfg);
static s32 wl_notify_escan_complete(struct bcm_cfg80211 *cfg,
	struct net_device *ndev, bool aborted, bool fw_abort);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 2, 0)) || defined(WL_COMPAT_WIRELESS)
static s32 wl_cfg80211_tdls_oper(struct wiphy *wiphy, struct net_device *dev,
	u8 *peer, enum nl80211_tdls_operation oper);
#endif /* LINUX_VERSION > KERNEL_VERSION(3,2,0) || WL_COMPAT_WIRELESS */
#ifdef WL_SCHED_SCAN
static int wl_cfg80211_sched_scan_stop(struct wiphy *wiphy, struct net_device *dev);
#endif

/*
 * event & event Q handlers for cfg80211 interfaces
 */
static s32 wl_create_event_handler(struct bcm_cfg80211 *cfg);
static void wl_destroy_event_handler(struct bcm_cfg80211 *cfg);
static s32 wl_event_handler(void *data);
static void wl_init_eq(struct bcm_cfg80211 *cfg);
static void wl_flush_eq(struct bcm_cfg80211 *cfg);
static unsigned long wl_lock_eq(struct bcm_cfg80211 *cfg);
static void wl_unlock_eq(struct bcm_cfg80211 *cfg, unsigned long flags);
static void wl_init_eq_lock(struct bcm_cfg80211 *cfg);
static void wl_init_event_handler(struct bcm_cfg80211 *cfg);
static struct wl_event_q *wl_deq_event(struct bcm_cfg80211 *cfg);
static s32 wl_enq_event(struct bcm_cfg80211 *cfg, struct net_device *ndev, u32 type,
	const wl_event_msg_t *msg, void *data);
static void wl_put_event(struct wl_event_q *e);
static void wl_wakeup_event(struct bcm_cfg80211 *cfg);
static s32 wl_notify_connect_status_ap(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
static s32 wl_notify_connect_status(struct bcm_cfg80211 *cfg,
	bcm_struct_cfgdev *cfgdev, const wl_event_msg_t *e, void *data);
static s32 wl_notify_roaming_status(struct bcm_cfg80211 *cfg,
	bcm_struct_cfgdev *cfgdev, const wl_event_msg_t *e, void *data);
static s32 wl_notify_scan_status(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data);
static s32 wl_bss_connect_done(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data, bool completed);
static s32 wl_bss_roaming_done(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
static s32 wl_notify_mic_status(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data);
#ifdef WL_SCHED_SCAN
static s32
wl_notify_sched_scan_results(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data);
#endif /* WL_SCHED_SCAN */
#ifdef PNO_SUPPORT
static s32 wl_notify_pfn_status(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data);
#endif /* PNO_SUPPORT */
static s32 wl_notifier_change_state(struct bcm_cfg80211 *cfg, struct net_info *_net_info,
	enum wl_status state, bool set);

#ifdef WLTDLS
static s32 wl_tdls_event_handler(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data);
#endif /* WLTDLS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
/*
 * register/deregister parent device
 */
static void wl_cfg80211_clear_parent_dev(void);

/*
 * ioctl utilites
 */

/*
 * cfg80211 set_wiphy_params utilities
 */
static s32 wl_set_frag(struct net_device *dev, u32 frag_threshold);
static s32 wl_set_rts(struct net_device *dev, u32 frag_threshold);
static s32 wl_set_retry(struct net_device *dev, u32 retry, bool l);

/*
<<<<<<< HEAD
 * wl profile utilities
 */
static s32 wl_update_prof(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data, s32 item);
static void *wl_read_prof(struct wl_priv *wl, struct net_device *ndev, s32 item);
static void wl_init_prof(struct wl_priv *wl, struct net_device *ndev);
=======
 * cfg profile utilities
 */
static s32 wl_update_prof(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data, s32 item);
static void *wl_read_prof(struct bcm_cfg80211 *cfg, struct net_device *ndev, s32 item);
static void wl_init_prof(struct bcm_cfg80211 *cfg, struct net_device *ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

/*
 * cfg80211 connect utilites
 */
static s32 wl_set_wpa_version(struct net_device *dev,
	struct cfg80211_connect_params *sme);
static s32 wl_set_auth_type(struct net_device *dev,
	struct cfg80211_connect_params *sme);
static s32 wl_set_set_cipher(struct net_device *dev,
	struct cfg80211_connect_params *sme);
static s32 wl_set_key_mgmt(struct net_device *dev,
	struct cfg80211_connect_params *sme);
static s32 wl_set_set_sharedkey(struct net_device *dev,
	struct cfg80211_connect_params *sme);
<<<<<<< HEAD
static s32 wl_get_assoc_ies(struct wl_priv *wl, struct net_device *ndev);
=======
#ifdef BCMWAPI_WPI
static s32 wl_set_set_wapi_ie(struct net_device *dev,
        struct cfg80211_connect_params *sme);
#endif
static s32 wl_get_assoc_ies(struct bcm_cfg80211 *cfg, struct net_device *ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static void wl_ch_to_chanspec(int ch,
	struct wl_join_params *join_params, size_t *join_params_size);

/*
 * information element utilities
 */
<<<<<<< HEAD
static void wl_rst_ie(struct wl_priv *wl);
static __used s32 wl_add_ie(struct wl_priv *wl, u8 t, u8 l, u8 *v);
static s32 wl_mrg_ie(struct wl_priv *wl, u8 *ie_stream, u16 ie_size);
static s32 wl_cp_ie(struct wl_priv *wl, u8 *dst, u16 dst_size);
static u32 wl_get_ielen(struct wl_priv *wl);
=======
static void wl_rst_ie(struct bcm_cfg80211 *cfg);
static __used s32 wl_add_ie(struct bcm_cfg80211 *cfg, u8 t, u8 l, u8 *v);
static void wl_update_hidden_ap_ie(struct wl_bss_info *bi, u8 *ie_stream, u32 *ie_size);
static s32 wl_mrg_ie(struct bcm_cfg80211 *cfg, u8 *ie_stream, u16 ie_size);
static s32 wl_cp_ie(struct bcm_cfg80211 *cfg, u8 *dst, u16 dst_size);
static u32 wl_get_ielen(struct bcm_cfg80211 *cfg);
#ifdef MFP
static int wl_cfg80211_get_rsn_capa(bcm_tlv_t *wpa2ie, u8* capa);
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#ifdef WL11U
bcm_tlv_t *
wl_cfg80211_find_interworking_ie(u8 *parse, u32 len);
static s32
<<<<<<< HEAD
wl_cfg80211_add_iw_ie(struct wl_priv *wl, struct net_device *ndev, s32 bssidx, s32 pktflag,
            uint8 ie_id, uint8 *data, uint8 data_len);
#endif /* WL11U */

static s32 wl_setup_wiphy(struct wireless_dev *wdev, struct device *dev);
static void wl_free_wdev(struct wl_priv *wl);
static int
wl_cfg80211_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);


static s32 wl_inform_bss(struct wl_priv *wl);
static s32 wl_inform_single_bss(struct wl_priv *wl, struct wl_bss_info *bi, u8 is_roam_done);
static s32 wl_update_bss_info(struct wl_priv *wl, struct net_device *ndev, u8 is_roam_done);
static chanspec_t wl_cfg80211_get_shared_freq(struct wiphy *wiphy);
s32 wl_cfg80211_channel_to_freq(u32 channel);

#if defined(DHCP_SCAN_SUPPRESS)
static void wl_cfg80211_work_handler(struct work_struct *work);
static void wl_cfg80211_scan_supp_timerfunc(ulong data);
#endif /* DHCP_SCAN_SUPPRESS */

=======
wl_cfg80211_add_iw_ie(struct bcm_cfg80211 *cfg, struct net_device *ndev, s32 bssidx, s32 pktflag,
            uint8 ie_id, uint8 *data, uint8 data_len);
#endif /* WL11U */

static s32 wl_setup_wiphy(struct wireless_dev *wdev, struct device *dev, void *data);
static void wl_free_wdev(struct bcm_cfg80211 *cfg);

static s32 wl_inform_bss(struct bcm_cfg80211 *cfg);
static s32 wl_inform_single_bss(struct bcm_cfg80211 *cfg, struct wl_bss_info *bi);
static s32 wl_update_bss_info(struct bcm_cfg80211 *cfg, struct net_device *ndev);
static chanspec_t wl_cfg80211_get_shared_freq(struct wiphy *wiphy);
s32 wl_cfg80211_channel_to_freq(u32 channel);


static void wl_cfg80211_work_handler(struct work_struct *work);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32 wl_add_keyext(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, const u8 *mac_addr,
	struct key_params *params);
/*
 * key indianess swap utilities
 */
static void swap_key_from_BE(struct wl_wsec_key *key);
static void swap_key_to_BE(struct wl_wsec_key *key);

/*
<<<<<<< HEAD
 * wl_priv memory init/deinit utilities
 */
static s32 wl_init_priv_mem(struct wl_priv *wl);
static void wl_deinit_priv_mem(struct wl_priv *wl);
=======
 * bcm_cfg80211 memory init/deinit utilities
 */
static s32 wl_init_priv_mem(struct bcm_cfg80211 *cfg);
static void wl_deinit_priv_mem(struct bcm_cfg80211 *cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

static void wl_delay(u32 ms);

/*
 * ibss mode utilities
 */
<<<<<<< HEAD
static bool wl_is_ibssmode(struct wl_priv *wl, struct net_device *ndev);
static __used bool wl_is_ibssstarter(struct wl_priv *wl);
=======
static bool wl_is_ibssmode(struct bcm_cfg80211 *cfg, struct net_device *ndev);
static __used bool wl_is_ibssstarter(struct bcm_cfg80211 *cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

/*
 * link up/down , default configuration utilities
 */
<<<<<<< HEAD
static s32 __wl_cfg80211_up(struct wl_priv *wl);
static s32 __wl_cfg80211_down(struct wl_priv *wl);
static bool wl_is_linkdown(struct wl_priv *wl, const wl_event_msg_t *e);
static bool wl_is_linkup(struct wl_priv *wl, const wl_event_msg_t *e, struct net_device *ndev);
static bool wl_is_nonetwork(struct wl_priv *wl, const wl_event_msg_t *e);
static void wl_link_up(struct wl_priv *wl);
static void wl_link_down(struct wl_priv *wl);
static s32 wl_config_ifmode(struct wl_priv *wl, struct net_device *ndev, s32 iftype);
static void wl_init_conf(struct wl_conf *conf);

/*
 * iscan handler
 */
static void wl_iscan_timer(unsigned long data);
static void wl_term_iscan(struct wl_priv *wl);
static s32 wl_init_scan(struct wl_priv *wl);
static s32 wl_iscan_thread(void *data);
static s32 wl_run_iscan(struct wl_iscan_ctrl *iscan, struct cfg80211_scan_request *request,
	u16 action);
static s32 wl_do_iscan(struct wl_priv *wl,  struct cfg80211_scan_request *request);
static s32 wl_wakeup_iscan(struct wl_iscan_ctrl *iscan);
static s32 wl_invoke_iscan(struct wl_priv *wl);
static s32 wl_get_iscan_results(struct wl_iscan_ctrl *iscan, u32 *status,
	struct wl_scan_results **bss_list);
static void wl_notify_iscan_complete(struct wl_iscan_ctrl *iscan, bool aborted);
static void wl_init_iscan_handler(struct wl_iscan_ctrl *iscan);
static s32 wl_iscan_done(struct wl_priv *wl);
static s32 wl_iscan_pending(struct wl_priv *wl);
static s32 wl_iscan_inprogress(struct wl_priv *wl);
static s32 wl_iscan_aborted(struct wl_priv *wl);
=======
static s32 __wl_cfg80211_up(struct bcm_cfg80211 *cfg);
static s32 __wl_cfg80211_down(struct bcm_cfg80211 *cfg);
static bool wl_is_linkdown(struct bcm_cfg80211 *cfg, const wl_event_msg_t *e);
static bool wl_is_linkup(struct bcm_cfg80211 *cfg, const wl_event_msg_t *e,
	struct net_device *ndev);
static bool wl_is_nonetwork(struct bcm_cfg80211 *cfg, const wl_event_msg_t *e);
static void wl_link_up(struct bcm_cfg80211 *cfg);
static void wl_link_down(struct bcm_cfg80211 *cfg);
static s32 wl_config_ifmode(struct bcm_cfg80211 *cfg, struct net_device *ndev, s32 iftype);
static void wl_init_conf(struct wl_conf *conf);
static s32 wl_cfg80211_handle_ifdel(struct bcm_cfg80211 *cfg, wl_if_event_info *if_event_info,
	struct net_device* ndev);

int wl_cfg80211_get_ioctl_version(void);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

/*
 * find most significant bit set
 */
static __used u32 wl_find_msb(u16 bit16);

/*
 * rfkill support
 */
<<<<<<< HEAD
static int wl_setup_rfkill(struct wl_priv *wl, bool setup);
static int wl_rfkill_set(void *data, bool blocked);

static wl_scan_params_t *wl_cfg80211_scan_alloc_params(int channel,
	int nprobes, int *out_params_size);
static void get_primary_mac(struct wl_priv *wl, struct ether_addr *mac);
=======
static int wl_setup_rfkill(struct bcm_cfg80211 *cfg, bool setup);
static int wl_rfkill_set(void *data, bool blocked);
#ifdef DEBUGFS_CFG80211
static s32 wl_setup_debugfs(struct bcm_cfg80211 *cfg);
static s32 wl_free_debugfs(struct bcm_cfg80211 *cfg);
#endif

static wl_scan_params_t *wl_cfg80211_scan_alloc_params(int channel,
	int nprobes, int *out_params_size);
static bool check_dev_role_integrity(struct bcm_cfg80211 *cfg, u32 dev_role);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

/*
 * Some external functions, TODO: move them to dhd_linux.h
 */
int dhd_add_monitor(char *name, struct net_device **new_ndev);
int dhd_del_monitor(struct net_device *ndev);
int dhd_monitor_init(void *dhd_pub);
int dhd_monitor_uninit(void);
int dhd_start_xmit(struct sk_buff *skb, struct net_device *net);

<<<<<<< HEAD


#define CHECK_SYS_UP(wlpriv)						\
do {									\
	struct net_device *ndev = wl_to_prmry_ndev(wlpriv);       	\
	if (unlikely(!wl_get_drv_status(wlpriv, READY, ndev))) {	\
=======
#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
void init_roam(int ioctl_ver);
void reset_roam_cache(void);
void add_roam_cache(wl_bss_info_t *bi);
int  get_roam_channel_list(int target_chan,
	chanspec_t *channels, const wlc_ssid_t *ssid, int ioctl_ver);
void print_roam_cache(void);
void set_roam_band(int band);
void update_roam_cache(struct bcm_cfg80211 *cfg, int ioctl_ver);
#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */

static int wl_cfg80211_delayed_roam(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const struct ether_addr *bssid);


#define RETURN_EIO_IF_NOT_UP(wlpriv)						\
do {									\
	struct net_device *checkSysUpNDev = bcmcfg_to_prmry_ndev(wlpriv);       	\
	if (unlikely(!wl_get_drv_status(wlpriv, READY, checkSysUpNDev))) {	\
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_INFO(("device is not ready\n"));			\
		return -EIO;						\
	}								\
} while (0)

<<<<<<< HEAD
=======
#ifdef RSSI_OFFSET
static s32 wl_rssi_offset(s32 rssi)
{
	rssi += RSSI_OFFSET;
	if (rssi > 0)
		rssi = 0;
	return rssi;
}
#else
#define wl_rssi_offset(x)	x
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#define IS_WPA_AKM(akm) ((akm) == RSN_AKM_NONE || 			\
				 (akm) == RSN_AKM_UNSPECIFIED || 	\
				 (akm) == RSN_AKM_PSK)


extern int dhd_wait_pend8021x(struct net_device *dev);
#ifdef PROP_TXSTATUS_VSDB
extern int disable_proptx;
<<<<<<< HEAD
extern int dhd_wlfc_init(dhd_pub_t *dhd);
extern void dhd_wlfc_deinit(dhd_pub_t *dhd);
#endif /* PROP_TXSTATUS_VSDB */


=======
#endif /* PROP_TXSTATUS_VSDB */

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#if (WL_DBG_LEVEL > 0)
#define WL_DBG_ESTR_MAX	50
static s8 wl_dbg_estr[][WL_DBG_ESTR_MAX] = {
	"SET_SSID", "JOIN", "START", "AUTH", "AUTH_IND",
	"DEAUTH", "DEAUTH_IND", "ASSOC", "ASSOC_IND", "REASSOC",
	"REASSOC_IND", "DISASSOC", "DISASSOC_IND", "QUIET_START", "QUIET_END",
	"BEACON_RX", "LINK", "MIC_ERROR", "NDIS_LINK", "ROAM",
	"TXFAIL", "PMKID_CACHE", "RETROGRADE_TSF", "PRUNE", "AUTOAUTH",
	"EAPOL_MSG", "SCAN_COMPLETE", "ADDTS_IND", "DELTS_IND", "BCNSENT_IND",
	"BCNRX_MSG", "BCNLOST_MSG", "ROAM_PREP", "PFN_NET_FOUND",
	"PFN_NET_LOST",
	"RESET_COMPLETE", "JOIN_START", "ROAM_START", "ASSOC_START",
	"IBSS_ASSOC",
	"RADIO", "PSM_WATCHDOG", "WLC_E_CCX_ASSOC_START", "WLC_E_CCX_ASSOC_ABORT",
	"PROBREQ_MSG",
	"SCAN_CONFIRM_IND", "PSK_SUP", "COUNTRY_CODE_CHANGED",
	"EXCEEDED_MEDIUM_TIME", "ICV_ERROR",
	"UNICAST_DECODE_ERROR", "MULTICAST_DECODE_ERROR", "TRACE",
	"WLC_E_BTA_HCI_EVENT", "IF", "WLC_E_P2P_DISC_LISTEN_COMPLETE",
	"RSSI", "PFN_SCAN_COMPLETE", "WLC_E_EXTLOG_MSG",
	"ACTION_FRAME", "ACTION_FRAME_COMPLETE", "WLC_E_PRE_ASSOC_IND",
	"WLC_E_PRE_REASSOC_IND", "WLC_E_CHANNEL_ADOPTED", "WLC_E_AP_STARTED",
	"WLC_E_DFS_AP_STOP", "WLC_E_DFS_AP_RESUME", "WLC_E_WAI_STA_EVENT",
	"WLC_E_WAI_MSG", "WLC_E_ESCAN_RESULT", "WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE",
	"WLC_E_PROBRESP_MSG", "WLC_E_P2P_PROBREQ_MSG", "WLC_E_DCS_REQUEST", "WLC_E_FIFO_CREDIT_MAP",
	"WLC_E_ACTION_FRAME_RX", "WLC_E_WAKE_EVENT", "WLC_E_RM_COMPLETE"
};
#endif				/* WL_DBG_LEVEL */

#define CHAN2G(_channel, _freq, _flags) {			\
	.band			= IEEE80211_BAND_2GHZ,		\
	.center_freq		= (_freq),			\
	.hw_value		= (_channel),			\
	.flags			= (_flags),			\
	.max_antenna_gain	= 0,				\
	.max_power		= 30,				\
}

#define CHAN5G(_channel, _flags) {				\
	.band			= IEEE80211_BAND_5GHZ,		\
	.center_freq		= 5000 + (5 * (_channel)),	\
	.hw_value		= (_channel),			\
	.flags			= (_flags),			\
	.max_antenna_gain	= 0,				\
	.max_power		= 30,				\
}

#define RATE_TO_BASE100KBPS(rate)   (((rate) * 10) / 2)
#define RATETAB_ENT(_rateid, _flags) \
	{								\
		.bitrate	= RATE_TO_BASE100KBPS(_rateid),     \
		.hw_value	= (_rateid),			    \
		.flags	  = (_flags),			     \
	}

static struct ieee80211_rate __wl_rates[] = {
<<<<<<< HEAD
	RATETAB_ENT(WLC_RATE_1M, 0),
	RATETAB_ENT(WLC_RATE_2M, IEEE80211_RATE_SHORT_PREAMBLE),
	RATETAB_ENT(WLC_RATE_5M5, IEEE80211_RATE_SHORT_PREAMBLE),
	RATETAB_ENT(WLC_RATE_11M, IEEE80211_RATE_SHORT_PREAMBLE),
	RATETAB_ENT(WLC_RATE_6M, 0),
	RATETAB_ENT(WLC_RATE_9M, 0),
	RATETAB_ENT(WLC_RATE_12M, 0),
	RATETAB_ENT(WLC_RATE_18M, 0),
	RATETAB_ENT(WLC_RATE_24M, 0),
	RATETAB_ENT(WLC_RATE_36M, 0),
	RATETAB_ENT(WLC_RATE_48M, 0),
	RATETAB_ENT(WLC_RATE_54M, 0)
=======
	RATETAB_ENT(DOT11_RATE_1M, 0),
	RATETAB_ENT(DOT11_RATE_2M, IEEE80211_RATE_SHORT_PREAMBLE),
	RATETAB_ENT(DOT11_RATE_5M5, IEEE80211_RATE_SHORT_PREAMBLE),
	RATETAB_ENT(DOT11_RATE_11M, IEEE80211_RATE_SHORT_PREAMBLE),
	RATETAB_ENT(DOT11_RATE_6M, 0),
	RATETAB_ENT(DOT11_RATE_9M, 0),
	RATETAB_ENT(DOT11_RATE_12M, 0),
	RATETAB_ENT(DOT11_RATE_18M, 0),
	RATETAB_ENT(DOT11_RATE_24M, 0),
	RATETAB_ENT(DOT11_RATE_36M, 0),
	RATETAB_ENT(DOT11_RATE_48M, 0),
	RATETAB_ENT(DOT11_RATE_54M, 0)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
};

#define wl_a_rates		(__wl_rates + 4)
#define wl_a_rates_size	8
#define wl_g_rates		(__wl_rates + 0)
#define wl_g_rates_size	12

static struct ieee80211_channel __wl_2ghz_channels[] = {
	CHAN2G(1, 2412, 0),
	CHAN2G(2, 2417, 0),
	CHAN2G(3, 2422, 0),
	CHAN2G(4, 2427, 0),
	CHAN2G(5, 2432, 0),
	CHAN2G(6, 2437, 0),
	CHAN2G(7, 2442, 0),
	CHAN2G(8, 2447, 0),
	CHAN2G(9, 2452, 0),
	CHAN2G(10, 2457, 0),
	CHAN2G(11, 2462, 0),
	CHAN2G(12, 2467, 0),
	CHAN2G(13, 2472, 0),
	CHAN2G(14, 2484, 0)
};

static struct ieee80211_channel __wl_5ghz_a_channels[] = {
	CHAN5G(34, 0), CHAN5G(36, 0),
	CHAN5G(38, 0), CHAN5G(40, 0),
	CHAN5G(42, 0), CHAN5G(44, 0),
	CHAN5G(46, 0), CHAN5G(48, 0),
	CHAN5G(52, 0), CHAN5G(56, 0),
	CHAN5G(60, 0), CHAN5G(64, 0),
	CHAN5G(100, 0), CHAN5G(104, 0),
	CHAN5G(108, 0), CHAN5G(112, 0),
	CHAN5G(116, 0), CHAN5G(120, 0),
	CHAN5G(124, 0), CHAN5G(128, 0),
	CHAN5G(132, 0), CHAN5G(136, 0),
	CHAN5G(140, 0), CHAN5G(149, 0),
	CHAN5G(153, 0), CHAN5G(157, 0),
	CHAN5G(161, 0), CHAN5G(165, 0)
};

static struct ieee80211_supported_band __wl_band_2ghz = {
	.band = IEEE80211_BAND_2GHZ,
	.channels = __wl_2ghz_channels,
	.n_channels = ARRAY_SIZE(__wl_2ghz_channels),
	.bitrates = wl_g_rates,
	.n_bitrates = wl_g_rates_size
};

static struct ieee80211_supported_band __wl_band_5ghz_a = {
	.band = IEEE80211_BAND_5GHZ,
	.channels = __wl_5ghz_a_channels,
	.n_channels = ARRAY_SIZE(__wl_5ghz_a_channels),
	.bitrates = wl_a_rates,
	.n_bitrates = wl_a_rates_size
};

static const u32 __wl_cipher_suites[] = {
	WLAN_CIPHER_SUITE_WEP40,
	WLAN_CIPHER_SUITE_WEP104,
	WLAN_CIPHER_SUITE_TKIP,
	WLAN_CIPHER_SUITE_CCMP,
	WLAN_CIPHER_SUITE_AES_CMAC,
<<<<<<< HEAD
};


/* IOCtl version read from targeted driver */
static int ioctl_version;
=======
#ifdef BCMWAPI_WPI
	WLAN_CIPHER_SUITE_SMS4,
#endif
#if defined(WLFBT) && defined(WLAN_CIPHER_SUITE_PMK)
	WLAN_CIPHER_SUITE_PMK,
#endif
};

#ifdef WL_CFG80211_GON_COLLISION
#define BLOCK_GON_REQ_MAX_NUM 5
#endif /* WL_CFG80211_GON_COLLISION */

#if defined(USE_DYNAMIC_MAXPKT_RXGLOM)
static int maxrxpktglom = 0;
#endif

/* IOCtl version read from targeted driver */
static int ioctl_version;
#ifdef DEBUGFS_CFG80211
#define S_SUBLOGLEVEL 20
static const struct {
	u32 log_level;
	char *sublogname;
} sublogname_map[] = {
	{WL_DBG_ERR, "ERR"},
	{WL_DBG_INFO, "INFO"},
	{WL_DBG_DBG, "DBG"},
	{WL_DBG_SCAN, "SCAN"},
	{WL_DBG_TRACE, "TRACE"},
	{WL_DBG_P2P_ACTION, "P2PACTION"}
};
#endif

#if defined(CUSTOMER_HW4) && defined(DHD_DEBUG)
uint prev_dhd_console_ms = 0;
u32 prev_wl_dbg_level = 0;
bool wl_scan_timeout_dbg_enabled = 0;
static void wl_scan_timeout_dbg_set(void);
static void wl_scan_timeout_dbg_clear(void);

static void wl_scan_timeout_dbg_set(void)
{
	WL_ERR(("Enter \n"));
	prev_dhd_console_ms = dhd_console_ms;
	prev_wl_dbg_level = wl_dbg_level;

	dhd_console_ms = 1;
	wl_dbg_level |= (WL_DBG_ERR | WL_DBG_P2P_ACTION | WL_DBG_SCAN);

	wl_scan_timeout_dbg_enabled = 1;
}
static void wl_scan_timeout_dbg_clear(void)
{
	WL_ERR(("Enter \n"));
	dhd_console_ms = prev_dhd_console_ms;
	wl_dbg_level = prev_wl_dbg_level;

	wl_scan_timeout_dbg_enabled = 0;
}
#endif /* CUSTOMER_HW4 && DHD_DEBUG */

static void wl_add_remove_pm_enable_work(struct bcm_cfg80211 *cfg, bool add_remove,
	enum wl_handler_del_type type)
{
	if (cfg == NULL)
		return;

	if (cfg->pm_enable_work_on) {
		if (add_remove) {
#ifdef CUSTOMER_HW4
			DHD_OS_WAKE_LOCK(cfg->pub);
#endif /* CUSTOMER_HW4 */
			schedule_delayed_work(&cfg->pm_enable_work,
				msecs_to_jiffies(WL_PM_ENABLE_TIMEOUT));
		} else {
			cancel_delayed_work_sync(&cfg->pm_enable_work);
			switch (type) {
				case WL_HANDLER_MAINTAIN:
					schedule_delayed_work(&cfg->pm_enable_work,
						msecs_to_jiffies(WL_PM_ENABLE_TIMEOUT));
					break;
				case WL_HANDLER_PEND:
					schedule_delayed_work(&cfg->pm_enable_work,
						msecs_to_jiffies(WL_PM_ENABLE_TIMEOUT*2));
					break;
				case WL_HANDLER_DEL:
				default:
					cfg->pm_enable_work_on = false;
#ifdef CUSTOMER_HW4
					DHD_OS_WAKE_UNLOCK(cfg->pub);
#endif /* CUSTOMER_HW4 */
					break;
			}
		}
	}
}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

/* Return a new chanspec given a legacy chanspec
 * Returns INVCHANSPEC on error
 */
static chanspec_t
wl_chspec_from_legacy(chanspec_t legacy_chspec)
{
	chanspec_t chspec;

	/* get the channel number */
	chspec = LCHSPEC_CHANNEL(legacy_chspec);

	/* convert the band */
	if (LCHSPEC_IS2G(legacy_chspec)) {
		chspec |= WL_CHANSPEC_BAND_2G;
	} else {
		chspec |= WL_CHANSPEC_BAND_5G;
	}

	/* convert the bw and sideband */
	if (LCHSPEC_IS20(legacy_chspec)) {
		chspec |= WL_CHANSPEC_BW_20;
	} else {
		chspec |= WL_CHANSPEC_BW_40;
		if (LCHSPEC_CTL_SB(legacy_chspec) == WL_LCHANSPEC_CTL_SB_LOWER) {
			chspec |= WL_CHANSPEC_CTL_SB_L;
		} else {
			chspec |= WL_CHANSPEC_CTL_SB_U;
		}
	}

	if (wf_chspec_malformed(chspec)) {
		WL_ERR(("wl_chspec_from_legacy: output chanspec (0x%04X) malformed\n",
		        chspec));
		return INVCHANSPEC;
	}

	return chspec;
}

/* Return a legacy chanspec given a new chanspec
 * Returns INVCHANSPEC on error
 */
static chanspec_t
wl_chspec_to_legacy(chanspec_t chspec)
{
	chanspec_t lchspec;

	if (wf_chspec_malformed(chspec)) {
		WL_ERR(("wl_chspec_to_legacy: input chanspec (0x%04X) malformed\n",
		        chspec));
		return INVCHANSPEC;
	}

	/* get the channel number */
	lchspec = CHSPEC_CHANNEL(chspec);

	/* convert the band */
	if (CHSPEC_IS2G(chspec)) {
		lchspec |= WL_LCHANSPEC_BAND_2G;
	} else {
		lchspec |= WL_LCHANSPEC_BAND_5G;
	}

	/* convert the bw and sideband */
	if (CHSPEC_IS20(chspec)) {
		lchspec |= WL_LCHANSPEC_BW_20;
		lchspec |= WL_LCHANSPEC_CTL_SB_NONE;
	} else if (CHSPEC_IS40(chspec)) {
		lchspec |= WL_LCHANSPEC_BW_40;
		if (CHSPEC_CTL_SB(chspec) == WL_CHANSPEC_CTL_SB_L) {
			lchspec |= WL_LCHANSPEC_CTL_SB_LOWER;
		} else {
			lchspec |= WL_LCHANSPEC_CTL_SB_UPPER;
		}
	} else {
		/* cannot express the bandwidth */
		char chanbuf[CHANSPEC_STR_LEN];
		WL_ERR((
		        "wl_chspec_to_legacy: unable to convert chanspec %s (0x%04X) "
		        "to pre-11ac format\n",
		        wf_chspec_ntoa(chspec, chanbuf), chspec));
		return INVCHANSPEC;
	}

	return lchspec;
}

/* given a chanspec value, do the endian and chanspec version conversion to
 * a chanspec_t value
 * Returns INVCHANSPEC on error
 */
static chanspec_t
wl_chspec_host_to_driver(chanspec_t chanspec)
{
	if (ioctl_version == 1) {
		chanspec = wl_chspec_to_legacy(chanspec);
		if (chanspec == INVCHANSPEC) {
			return chanspec;
		}
	}
	chanspec = htodchanspec(chanspec);

	return chanspec;
}

/* given a channel value, do the endian and chanspec version conversion to
 * a chanspec_t value
 * Returns INVCHANSPEC on error
 */
chanspec_t
wl_ch_host_to_driver(u16 channel)
{

	chanspec_t chanspec;

	chanspec = channel & WL_CHANSPEC_CHAN_MASK;

	if (channel <= CH_MAX_2G_CHANNEL)
		chanspec |= WL_CHANSPEC_BAND_2G;
	else
		chanspec |= WL_CHANSPEC_BAND_5G;

	chanspec |= WL_CHANSPEC_BW_20;
	chanspec |= WL_CHANSPEC_CTL_SB_NONE;

	return wl_chspec_host_to_driver(chanspec);
}

/* given a chanspec value from the driver, do the endian and chanspec version conversion to
 * a chanspec_t value
 * Returns INVCHANSPEC on error
 */
static chanspec_t
wl_chspec_driver_to_host(chanspec_t chanspec)
{
	chanspec = dtohchanspec(chanspec);
	if (ioctl_version == 1) {
		chanspec = wl_chspec_from_legacy(chanspec);
	}

	return chanspec;
}

/* There isn't a lot of sense in it, but you can transmit anything you like */
static const struct ieee80211_txrx_stypes
wl_cfg80211_default_mgmt_stypes[NUM_NL80211_IFTYPES] = {
	[NL80211_IFTYPE_ADHOC] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4)
	},
	[NL80211_IFTYPE_STATION] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
		BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
	},
	[NL80211_IFTYPE_AP] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
		BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
		BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
		BIT(IEEE80211_STYPE_DISASSOC >> 4) |
		BIT(IEEE80211_STYPE_AUTH >> 4) |
		BIT(IEEE80211_STYPE_DEAUTH >> 4) |
		BIT(IEEE80211_STYPE_ACTION >> 4)
	},
	[NL80211_IFTYPE_AP_VLAN] = {
		/* copy AP */
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
		BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
		BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
		BIT(IEEE80211_STYPE_DISASSOC >> 4) |
		BIT(IEEE80211_STYPE_AUTH >> 4) |
		BIT(IEEE80211_STYPE_DEAUTH >> 4) |
		BIT(IEEE80211_STYPE_ACTION >> 4)
	},
	[NL80211_IFTYPE_P2P_CLIENT] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
		BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
	},
	[NL80211_IFTYPE_P2P_GO] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
		BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
		BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
		BIT(IEEE80211_STYPE_DISASSOC >> 4) |
		BIT(IEEE80211_STYPE_AUTH >> 4) |
		BIT(IEEE80211_STYPE_DEAUTH >> 4) |
		BIT(IEEE80211_STYPE_ACTION >> 4)
<<<<<<< HEAD
	}
=======
	},
#if defined(WL_CFG80211_P2P_DEV_IF)
	[NL80211_IFTYPE_P2P_DEVICE] = {
		.tx = 0xffff,
		.rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
		BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
	},
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
};

static void swap_key_from_BE(struct wl_wsec_key *key)
{
	key->index = htod32(key->index);
	key->len = htod32(key->len);
	key->algo = htod32(key->algo);
	key->flags = htod32(key->flags);
	key->rxiv.hi = htod32(key->rxiv.hi);
	key->rxiv.lo = htod16(key->rxiv.lo);
	key->iv_initialized = htod32(key->iv_initialized);
}

static void swap_key_to_BE(struct wl_wsec_key *key)
{
	key->index = dtoh32(key->index);
	key->len = dtoh32(key->len);
	key->algo = dtoh32(key->algo);
	key->flags = dtoh32(key->flags);
	key->rxiv.hi = dtoh32(key->rxiv.hi);
	key->rxiv.lo = dtoh16(key->rxiv.lo);
	key->iv_initialized = dtoh32(key->iv_initialized);
}

<<<<<<< HEAD
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
=======
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)) && !defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
/* For debug: Dump the contents of the encoded wps ie buffe */
static void
wl_validate_wps_ie(char *wps_ie, s32 wps_ie_len, bool *pbc)
{
	#define WPS_IE_FIXED_LEN 6
	u16 len;
	u8 *subel = NULL;
	u16 subelt_id;
	u16 subelt_len;
	u16 val;
	u8 *valptr = (uint8*) &val;
	if (wps_ie == NULL || wps_ie_len < WPS_IE_FIXED_LEN) {
		WL_ERR(("invalid argument : NULL\n"));
		return;
	}
	len = (u16)wps_ie[TLV_LEN_OFF];

	if (len > wps_ie_len) {
		WL_ERR(("invalid length len %d, wps ie len %d\n", len, wps_ie_len));
		return;
	}
	WL_DBG(("wps_ie len=%d\n", len));
	len -= 4;	/* for the WPS IE's OUI, oui_type fields */
	subel = wps_ie + WPS_IE_FIXED_LEN;
	while (len >= 4) {		/* must have attr id, attr len fields */
		valptr[0] = *subel++;
		valptr[1] = *subel++;
		subelt_id = HTON16(val);

		valptr[0] = *subel++;
		valptr[1] = *subel++;
		subelt_len = HTON16(val);

		len -= 4;			/* for the attr id, attr len fields */
		len -= subelt_len;	/* for the remaining fields in this attribute */
		WL_DBG((" subel=%p, subelt_id=0x%x subelt_len=%u\n",
			subel, subelt_id, subelt_len));

		if (subelt_id == WPS_ID_VERSION) {
			WL_DBG(("  attr WPS_ID_VERSION: %u\n", *subel));
		} else if (subelt_id == WPS_ID_REQ_TYPE) {
			WL_DBG(("  attr WPS_ID_REQ_TYPE: %u\n", *subel));
		} else if (subelt_id == WPS_ID_CONFIG_METHODS) {
			valptr[0] = *subel;
			valptr[1] = *(subel + 1);
			WL_DBG(("  attr WPS_ID_CONFIG_METHODS: %x\n", HTON16(val)));
		} else if (subelt_id == WPS_ID_DEVICE_NAME) {
			char devname[100];
			memcpy(devname, subel, subelt_len);
			devname[subelt_len] = '\0';
			WL_DBG(("  attr WPS_ID_DEVICE_NAME: %s (len %u)\n",
				devname, subelt_len));
		} else if (subelt_id == WPS_ID_DEVICE_PWD_ID) {
			valptr[0] = *subel;
			valptr[1] = *(subel + 1);
			WL_DBG(("  attr WPS_ID_DEVICE_PWD_ID: %u\n", HTON16(val)));
			*pbc = (HTON16(val) == DEV_PW_PUSHBUTTON) ? true : false;
		} else if (subelt_id == WPS_ID_PRIM_DEV_TYPE) {
			valptr[0] = *subel;
			valptr[1] = *(subel + 1);
			WL_DBG(("  attr WPS_ID_PRIM_DEV_TYPE: cat=%u \n", HTON16(val)));
			valptr[0] = *(subel + 6);
			valptr[1] = *(subel + 7);
			WL_DBG(("  attr WPS_ID_PRIM_DEV_TYPE: subcat=%u\n", HTON16(val)));
		} else if (subelt_id == WPS_ID_REQ_DEV_TYPE) {
			valptr[0] = *subel;
			valptr[1] = *(subel + 1);
			WL_DBG(("  attr WPS_ID_REQ_DEV_TYPE: cat=%u\n", HTON16(val)));
			valptr[0] = *(subel + 6);
			valptr[1] = *(subel + 7);
			WL_DBG(("  attr WPS_ID_REQ_DEV_TYPE: subcat=%u\n", HTON16(val)));
		} else if (subelt_id == WPS_ID_SELECTED_REGISTRAR_CONFIG_METHODS) {
			valptr[0] = *subel;
			valptr[1] = *(subel + 1);
			WL_DBG(("  attr WPS_ID_SELECTED_REGISTRAR_CONFIG_METHODS"
				": cat=%u\n", HTON16(val)));
		} else {
			WL_DBG(("  unknown attr 0x%x\n", subelt_id));
		}

		subel += subelt_len;
	}
}
<<<<<<< HEAD
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0) */
=======
#endif /* LINUX_VERSION < VERSION(3, 4, 0) && !WL_COMPAT_WIRELESS */

s32 wl_set_tx_power(struct net_device *dev,
	enum nl80211_tx_power_setting type, s32 dbm)
{
	s32 err = 0;
	s32 disable = 0;
	s32 txpwrqdbm;
#ifdef TEST_TX_POWER_CONTROL
	char *tmppwr_str = NULL;
#endif /* TEST_TX_POWER_CONTROL */

	/* Make sure radio is off or on as far as software is concerned */
	disable = WL_RADIO_SW_DISABLE << 16;
	disable = htod32(disable);
	err = wldev_ioctl(dev, WLC_SET_RADIO, &disable, sizeof(disable), true);
	if (unlikely(err)) {
		WL_ERR(("WLC_SET_RADIO error (%d)\n", err));
		return err;
	}

	if (dbm > 0xffff)
		dbm = 0xffff;
	txpwrqdbm = dbm * 4;
#ifdef SUPPORT_WL_TXPOWER
	if (type == NL80211_TX_POWER_AUTOMATIC) {
		txpwrqdbm = 127;
#ifdef TEST_TX_POWER_CONTROL
		err = wldev_iovar_setint(dev, "qtxpower", txpwrqdbm);
		if (unlikely(err))
			WL_ERR(("qtxpower error (%d)\n", err));
		else
			WL_ERR(("mW=%d, txpwrqdbm=0x%x\n", dbm, txpwrqdbm));

		tmppwr_str = kzalloc(0x71a, GFP_KERNEL);
		if (!tmppwr_str) {
			WL_ERR(("tmppwr memory alloc failed\n"));
		} else {
			err = wldev_iovar_getbuf(dev, "curpower", NULL, 0, tmppwr_str, 0x71a, NULL);
			if (unlikely(err)) {
				WL_ERR(("curpower error (%d)\n", err));
			}
			kfree(tmppwr_str);
		}
#endif /* TEST_TX_POWER_CONTROL */
	} else {
		txpwrqdbm |= WL_TXPWR_OVERRIDE;
	}
#endif /* SUPPORT_WL_TXPOWER */

	err = wldev_iovar_setint(dev, "qtxpower", txpwrqdbm);
	if (unlikely(err))
		WL_ERR(("qtxpower error (%d)\n", err));
	else
		WL_ERR(("dBm=%d, txpwrqdbm=0x%x\n", dbm, txpwrqdbm));

	return err;
}

s32 wl_get_tx_power(struct net_device *dev, s32 *dbm)
{
	s32 err = 0;
	s32 txpwrdbm;

	err = wldev_iovar_getint(dev, "qtxpower", &txpwrdbm);
	if (unlikely(err)) {
		WL_ERR(("error (%d)\n", err));
		return err;
	}

	*dbm = (txpwrdbm & ~WL_TXPWR_OVERRIDE) / 4;

	return err;
}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

static chanspec_t wl_cfg80211_get_shared_freq(struct wiphy *wiphy)
{
	chanspec_t chspec;
	int err = 0;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct net_device *dev = wl_to_prmry_ndev(wl);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct net_device *dev = bcmcfg_to_prmry_ndev(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct ether_addr bssid;
	struct wl_bss_info *bss = NULL;

	if ((err = wldev_ioctl(dev, WLC_GET_BSSID, &bssid, sizeof(bssid), false))) {
		/* STA interface is not associated. So start the new interface on a temp
		 * channel . Later proper channel will be applied by the above framework
		 * via set_channel (cfg80211 API).
		 */
		WL_DBG(("Not associated. Return a temp channel. \n"));
		return wl_ch_host_to_driver(WL_P2P_TEMP_CHAN);
	}


<<<<<<< HEAD
	*(u32 *) wl->extra_buf = htod32(WL_EXTRA_BUF_MAX);
	if ((err = wldev_ioctl(dev, WLC_GET_BSS_INFO, wl->extra_buf,
=======
	*(u32 *) cfg->extra_buf = htod32(WL_EXTRA_BUF_MAX);
	if ((err = wldev_ioctl(dev, WLC_GET_BSS_INFO, cfg->extra_buf,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_EXTRA_BUF_MAX, false))) {
			WL_ERR(("Failed to get associated bss info, use temp channel \n"));
			chspec = wl_ch_host_to_driver(WL_P2P_TEMP_CHAN);
	}
	else {
<<<<<<< HEAD
			bss = (struct wl_bss_info *) (wl->extra_buf + 4);
			chspec =  bss->chanspec;
=======
			bss = (struct wl_bss_info *) (cfg->extra_buf + 4);
			chspec =  bss->chanspec;

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_DBG(("Valid BSS Found. chanspec:%d \n", chspec));
	}
	return chspec;
}

<<<<<<< HEAD
static struct net_device* wl_cfg80211_add_monitor_if(char *name)
{
#if defined(WLP2P) && defined(WL_ENABLE_P2P_IF)
=======
static bcm_struct_cfgdev *
wl_cfg80211_add_monitor_if(char *name)
{
#if defined(WL_ENABLE_P2P_IF) || defined(WL_CFG80211_P2P_DEV_IF)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	WL_INFO(("wl_cfg80211_add_monitor_if: No more support monitor interface\n"));
	return ERR_PTR(-EOPNOTSUPP);
#else
	struct net_device* ndev = NULL;

	dhd_add_monitor(name, &ndev);
	WL_INFO(("wl_cfg80211_add_monitor_if net device returned: 0x%p\n", ndev));
<<<<<<< HEAD
	return ndev;
#endif /* defined(WLP2P) && defined(WL_ENABLE_P2P_IF) */
}

static struct net_device *
wl_cfg80211_add_virtual_iface(struct wiphy *wiphy, char *name,
=======
	return ndev_to_cfgdev(ndev);
#endif /* WL_ENABLE_P2P_IF || WL_CFG80211_P2P_DEV_IF */
}

static bcm_struct_cfgdev *
wl_cfg80211_add_virtual_iface(struct wiphy *wiphy,
#if defined(WL_CFG80211_P2P_DEV_IF)
	const char *name,
#else
	char *name,
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	enum nl80211_iftype type, u32 *flags,
	struct vif_params *params)
{
	s32 err;
	s32 timeout = -1;
	s32 wlif_type = -1;
	s32 mode = 0;
	s32 val = 0;
	s32 dhd_mode = 0;
	chanspec_t chspec;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct net_device *_ndev;
	struct ether_addr primary_mac;
	int (*net_attach)(void *dhdp, int ifidx);
	bool rollback_lock = false;
#ifdef PROP_TXSTATUS_VSDB
	s32 up = 1;
	dhd_pub_t *dhd;
#endif /* PROP_TXSTATUS_VSDB */

	if (!wl)
		return ERR_PTR(-EINVAL);

#ifdef PROP_TXSTATUS_VSDB
	dhd = (dhd_pub_t *)(wl->pub);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct net_device *primary_ndev;
	struct net_device *new_ndev;
	struct ether_addr primary_mac;
#ifdef PROP_TXSTATUS_VSDB
	s32 up = 1;
	dhd_pub_t *dhd;
	bool enabled;
#endif /* PROP_TXSTATUS_VSDB */

	if (!cfg)
		return ERR_PTR(-EINVAL);

#ifdef PROP_TXSTATUS_VSDB
	dhd = (dhd_pub_t *)(cfg->pub);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#endif /* PROP_TXSTATUS_VSDB */


	/* Use primary I/F for sending cmds down to firmware */
<<<<<<< HEAD
	_ndev = wl_to_prmry_ndev(wl);
=======
	primary_ndev = bcmcfg_to_prmry_ndev(cfg);

	if (unlikely(!wl_get_drv_status(cfg, READY, primary_ndev))) {
		WL_ERR(("device is not ready\n"));
		return ERR_PTR(-ENODEV);
	}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	WL_DBG(("if name: %s, type: %d\n", name, type));
	switch (type) {
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_WDS:
	case NL80211_IFTYPE_MESH_POINT:
		WL_ERR(("Unsupported interface type\n"));
		mode = WL_MODE_IBSS;
		return NULL;
	case NL80211_IFTYPE_MONITOR:
<<<<<<< HEAD
		return wl_cfg80211_add_monitor_if(name);
=======
		return wl_cfg80211_add_monitor_if((char *)name);
#if defined(WL_CFG80211_P2P_DEV_IF)
	case NL80211_IFTYPE_P2P_DEVICE:
		return wl_cfgp2p_add_p2p_disc_if(cfg);
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_STATION:
		wlif_type = WL_P2P_IF_CLIENT;
		mode = WL_MODE_BSS;
		break;
	case NL80211_IFTYPE_P2P_GO:
	case NL80211_IFTYPE_AP:
		wlif_type = WL_P2P_IF_GO;
		mode = WL_MODE_AP;
		break;
	default:
		WL_ERR(("Unsupported interface type\n"));
		return NULL;
		break;
	}

	if (!name) {
		WL_ERR(("name is NULL\n"));
		return NULL;
	}
<<<<<<< HEAD
	if (wl->p2p_supported && (wlif_type != -1)) {
		if (wl_get_p2p_status(wl, IF_DELETING)) {
			/* wait till IF_DEL is complete
			 * release the lock for the unregister to proceed
			 */
			if (rtnl_is_locked()) {
				rtnl_unlock();
				rollback_lock = true;
			}
			WL_INFO(("%s: Released the lock and wait till IF_DEL is complete\n",
				__func__));
			timeout = wait_event_interruptible_timeout(wl->netif_change_event,
				(wl_get_p2p_status(wl, IF_DELETING) == false),
				msecs_to_jiffies(MAX_WAIT_TIME));

			/* put back the rtnl_lock again */
			if (rollback_lock) {
				rtnl_lock();
				rollback_lock = false;
			}
			if (timeout > 0) {
				WL_ERR(("IF DEL is Success\n"));

			} else {
				WL_ERR(("timeount < 0, return -EAGAIN\n"));
				return ERR_PTR(-EAGAIN);
			}
			/* It should be now be safe to put this check here since we are sure
			 * by now netdev_notifier (unregister) would have been called
			 */
			if (wl->iface_cnt == IFACE_MAX_CNT)
				return ERR_PTR(-ENOMEM);
		}
=======
	if (cfg->p2p_supported && (wlif_type != -1)) {
		ASSERT(cfg->p2p); /* ensure expectation of p2p initialization */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#ifdef PROP_TXSTATUS_VSDB
		if (!dhd)
			return ERR_PTR(-ENODEV);
#endif /* PROP_TXSTATUS_VSDB */
<<<<<<< HEAD
		if (!wl->p2p)
			return ERR_PTR(-ENODEV);

		if (wl->p2p && !wl->p2p->on && strstr(name, WL_P2P_INTERFACE_PREFIX)) {
			p2p_on(wl) = true;
			wl_cfgp2p_set_firm_p2p(wl);
			wl_cfgp2p_init_discovery(wl);
			get_primary_mac(wl, &primary_mac);
			wl_cfgp2p_generate_bss_mac(&primary_mac,
				&wl->p2p->dev_addr, &wl->p2p->int_addr);
		}

		memset(wl->p2p->vir_ifname, 0, IFNAMSIZ);
		strncpy(wl->p2p->vir_ifname, name, IFNAMSIZ - 1);

		wl_notify_escan_complete(wl, _ndev, true, true);
#ifdef PROP_TXSTATUS_VSDB
		if (!wl->wlfc_on && !disable_proptx) {
			dhd->wlfc_enabled = true;
			dhd_wlfc_init(dhd);
			err = wldev_ioctl(_ndev, WLC_UP, &up, sizeof(s32), true);
			if (err < 0)
				WL_ERR(("WLC_UP return err:%d\n", err));
			wl->wlfc_on = true;
=======
		if (!cfg->p2p)
			return ERR_PTR(-ENODEV);

		if (cfg->p2p && !cfg->p2p->on && strstr(name, WL_P2P_INTERFACE_PREFIX)) {
			p2p_on(cfg) = true;
			wl_cfgp2p_set_firm_p2p(cfg);
			wl_cfgp2p_init_discovery(cfg);
			get_primary_mac(cfg, &primary_mac);
			wl_cfgp2p_generate_bss_mac(&primary_mac,
				&cfg->p2p->dev_addr, &cfg->p2p->int_addr);
		}

		memset(cfg->p2p->vir_ifname, 0, IFNAMSIZ);
		strncpy(cfg->p2p->vir_ifname, name, IFNAMSIZ - 1);

		wl_cfg80211_scan_abort(cfg);
#ifdef PROP_TXSTATUS_VSDB
		if (!cfg->wlfc_on && !disable_proptx) {
			dhd_wlfc_get_enable(dhd, &enabled);
			if (!enabled && dhd->op_mode != DHD_FLAG_HOSTAP_MODE &&
				dhd->op_mode != DHD_FLAG_IBSS_MODE) {
				dhd_wlfc_init(dhd);
				err = wldev_ioctl(primary_ndev, WLC_UP, &up, sizeof(s32), true);
				if (err < 0)
					WL_ERR(("WLC_UP return err:%d\n", err));
			}
			cfg->wlfc_on = true;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
#endif /* PROP_TXSTATUS_VSDB */

		/* In concurrency case, STA may be already associated in a particular channel.
		 * so retrieve the current channel of primary interface and then start the virtual
		 * interface on that.
		 */
		 chspec = wl_cfg80211_get_shared_freq(wiphy);

		/* For P2P mode, use P2P-specific driver features to create the
<<<<<<< HEAD
		 * bss: "wl p2p_ifadd"
		 */
		wl_set_p2p_status(wl, IF_ADD);
		if (wlif_type == WL_P2P_IF_GO)
			wldev_iovar_setint(_ndev, "mpc", 0);
		err = wl_cfgp2p_ifadd(wl, &wl->p2p->int_addr, htod32(wlif_type), chspec);

		if (unlikely(err)) {
=======
		 * bss: "cfg p2p_ifadd"
		 */
		wl_set_p2p_status(cfg, IF_ADDING);
		memset(&cfg->if_event_info, 0, sizeof(cfg->if_event_info));
		if (wlif_type == WL_P2P_IF_GO)
			wldev_iovar_setint(primary_ndev, "mpc", 0);
		err = wl_cfgp2p_ifadd(cfg, &cfg->p2p->int_addr, htod32(wlif_type), chspec);
		if (unlikely(err)) {
			wl_clr_p2p_status(cfg, IF_ADDING);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_ERR((" virtual iface add failed (%d) \n", err));
			return ERR_PTR(-ENOMEM);
		}

<<<<<<< HEAD
		timeout = wait_event_interruptible_timeout(wl->netif_change_event,
			(wl_get_p2p_status(wl, IF_ADD) == false),
			msecs_to_jiffies(MAX_WAIT_TIME));
		if (timeout > 0 && (!wl_get_p2p_status(wl, IF_ADD))) {

			struct wireless_dev *vwdev;
			vwdev = kzalloc(sizeof(*vwdev), GFP_KERNEL);
			if (unlikely(!vwdev)) {
				WL_ERR(("Could not allocate wireless device\n"));
				return ERR_PTR(-ENOMEM);
			}
			vwdev->wiphy = wl->wdev->wiphy;
			WL_INFO((" virtual interface(%s) is created memalloc done \n",
				wl->p2p->vir_ifname));
			vwdev->iftype = type;
			_ndev =  wl_to_p2p_bss_ndev(wl, P2PAPI_BSSCFG_CONNECTION);
			_ndev->ieee80211_ptr = vwdev;
			SET_NETDEV_DEV(_ndev, wiphy_dev(vwdev->wiphy));
			vwdev->netdev = _ndev;
			wl_set_drv_status(wl, READY, _ndev);
			wl->p2p->vif_created = true;
			wl_set_mode_by_netdev(wl, _ndev, mode);
			net_attach =  wl_to_p2p_bss_private(wl, P2PAPI_BSSCFG_CONNECTION);
			if (rtnl_is_locked()) {
				rtnl_unlock();
				rollback_lock = true;
			}
			if (net_attach && !net_attach(wl->pub, _ndev->ifindex)) {
				wl_alloc_netinfo(wl, _ndev, vwdev, mode, PM_ENABLE);
				val = 1;
				/* Disable firmware roaming for P2P interface  */
				wldev_iovar_setint(_ndev, "roam_off", val);
				WL_ERR((" virtual interface(%s) is "
					"created net attach done\n", wl->p2p->vir_ifname));
				if (mode == WL_MODE_AP)
					wl_set_drv_status(wl, CONNECTED, _ndev);
				if (type == NL80211_IFTYPE_P2P_CLIENT)
					dhd_mode = DHD_FLAG_P2P_GC_MODE;
				else if (type == NL80211_IFTYPE_P2P_GO)
					dhd_mode = DHD_FLAG_P2P_GO_MODE;
				DNGL_FUNC(dhd_cfg80211_set_p2p_info, (wl, dhd_mode));
			} else {
				/* put back the rtnl_lock again */
				if (rollback_lock)
					rtnl_lock();
				goto fail;
			}
			/* put back the rtnl_lock again */
			if (rollback_lock)
				rtnl_lock();
			return _ndev;

		} else {
			wl_clr_p2p_status(wl, IF_ADD);
			WL_ERR((" virtual interface(%s) is not created \n", wl->p2p->vir_ifname));
			memset(wl->p2p->vir_ifname, '\0', IFNAMSIZ);
			wl->p2p->vif_created = false;
#ifdef PROP_TXSTATUS_VSDB
		if (dhd->wlfc_enabled && wl->wlfc_on) {
			dhd->wlfc_enabled = false;
			dhd_wlfc_deinit(dhd);
			wl->wlfc_on = false;
=======
		timeout = wait_event_interruptible_timeout(cfg->netif_change_event,
			(wl_get_p2p_status(cfg, IF_ADDING) == false),
			msecs_to_jiffies(MAX_WAIT_TIME));

		if (timeout > 0 && !wl_get_p2p_status(cfg, IF_ADDING) && cfg->if_event_info.valid) {
			struct wireless_dev *vwdev;
			int pm_mode = PM_ENABLE;
			wl_if_event_info *event = &cfg->if_event_info;

			/* IF_ADD event has come back, we can proceed to to register
			 * the new interface now, use the interface name provided by caller (thus
			 * ignore the one from wlc)
			 */
			strncpy(cfg->if_event_info.name, name, IFNAMSIZ - 1);
			new_ndev = wl_cfg80211_allocate_if(cfg, event->ifidx, cfg->p2p->vir_ifname,
				event->mac, event->bssidx);
			if (new_ndev == NULL)
				goto fail;

			wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_CONNECTION) = new_ndev;
			wl_to_p2p_bss_bssidx(cfg, P2PAPI_BSSCFG_CONNECTION) = event->bssidx;
			vwdev = kzalloc(sizeof(*vwdev), GFP_KERNEL);
			if (unlikely(!vwdev)) {
				WL_ERR(("Could not allocate wireless device\n"));
				goto fail;
			}
			vwdev->wiphy = cfg->wdev->wiphy;
			WL_INFO(("virtual interface(%s) is created\n", cfg->p2p->vir_ifname));
			vwdev->iftype = type;
			vwdev->netdev = new_ndev;
			new_ndev->ieee80211_ptr = vwdev;
			SET_NETDEV_DEV(new_ndev, wiphy_dev(vwdev->wiphy));
			wl_set_drv_status(cfg, READY, new_ndev);
			cfg->p2p->vif_created = true;
			wl_set_mode_by_netdev(cfg, new_ndev, mode);

			if (wl_cfg80211_register_if(cfg, event->ifidx, new_ndev) != BCME_OK) {
				wl_cfg80211_remove_if(cfg, event->ifidx, new_ndev);
				goto fail;
			}
			wl_alloc_netinfo(cfg, new_ndev, vwdev, mode, pm_mode);
			val = 1;
			/* Disable firmware roaming for P2P interface  */
			wldev_iovar_setint(new_ndev, "roam_off", val);

			if (mode != WL_MODE_AP)
				wldev_iovar_setint(new_ndev, "buf_key_b4_m4", 1);

			WL_ERR((" virtual interface(%s) is "
				"created net attach done\n", cfg->p2p->vir_ifname));
			if (mode == WL_MODE_AP)
				wl_set_drv_status(cfg, CONNECTED, new_ndev);
			if (type == NL80211_IFTYPE_P2P_CLIENT)
				dhd_mode = DHD_FLAG_P2P_GC_MODE;
			else if (type == NL80211_IFTYPE_P2P_GO)
				dhd_mode = DHD_FLAG_P2P_GO_MODE;
			DNGL_FUNC(dhd_cfg80211_set_p2p_info, (cfg, dhd_mode));
			/* reinitialize completion to clear previous count */
			INIT_COMPLETION(cfg->iface_disable);

			return ndev_to_cfgdev(new_ndev);
		} else {
			wl_clr_p2p_status(cfg, IF_ADDING);
			WL_ERR((" virtual interface(%s) is not created \n", cfg->p2p->vir_ifname));
			memset(cfg->p2p->vir_ifname, '\0', IFNAMSIZ);
			cfg->p2p->vif_created = false;
#ifdef PROP_TXSTATUS_VSDB
			dhd_wlfc_get_enable(dhd, &enabled);
		if (enabled && cfg->wlfc_on && dhd->op_mode != DHD_FLAG_HOSTAP_MODE &&
			dhd->op_mode != DHD_FLAG_IBSS_MODE) {
			dhd_wlfc_deinit(dhd);
			cfg->wlfc_on = false;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
#endif /* PROP_TXSTATUS_VSDB */
		}
	}
<<<<<<< HEAD
fail:
	if (wlif_type == WL_P2P_IF_GO)
		wldev_iovar_setint(_ndev, "mpc", 1);
=======

fail:
	if (wlif_type == WL_P2P_IF_GO)
		wldev_iovar_setint(primary_ndev, "mpc", 1);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return ERR_PTR(-ENODEV);
}

static s32
<<<<<<< HEAD
wl_cfg80211_del_virtual_iface(struct wiphy *wiphy, struct net_device *dev)
{
	struct ether_addr p2p_mac;
	struct wl_priv *wl = wiphy_priv(wiphy);
	s32 timeout = -1;
	s32 ret = 0;
	WL_DBG(("Enter\n"));

	if (wl->p2p_net == dev) {
		/* Since there is no ifidx corresponding to p2p0, cmds to
		 * firmware should be routed through primary I/F
		 */
		dev = wl_to_prmry_ndev(wl);
	}

	if (wl->p2p_supported) {
		memcpy(p2p_mac.octet, wl->p2p->int_addr.octet, ETHER_ADDR_LEN);
=======
wl_cfg80211_del_virtual_iface(struct wiphy *wiphy, bcm_struct_cfgdev *cfgdev)
{
	struct net_device *dev = NULL;
	struct ether_addr p2p_mac;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	s32 timeout = -1;
	s32 ret = 0;
	s32 index = -1;
#ifdef CUSTOM_SET_CPUCORE
	dhd_pub_t *dhd = (dhd_pub_t *)(cfg->pub);
#endif /* CUSTOM_SET_CPUCORE */
	WL_DBG(("Enter\n"));

#ifdef CUSTOM_SET_CPUCORE
	dhd->chan_isvht80 &= ~DHD_FLAG_P2P_MODE;
	if (!(dhd->chan_isvht80))
		dhd_set_cpucore(dhd, FALSE);
#endif /* CUSTOM_SET_CPUCORE */
#if defined(WL_CFG80211_P2P_DEV_IF)
	if (cfgdev->iftype == NL80211_IFTYPE_P2P_DEVICE) {
		return wl_cfgp2p_del_p2p_disc_if(cfgdev, cfg);
	}
#endif /* WL_CFG80211_P2P_DEV_IF */
	dev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	if (wl_cfgp2p_find_idx(cfg, dev, &index) != BCME_OK) {
		WL_ERR(("Find p2p index from ndev(%p) failed\n", dev));
		return BCME_ERROR;
	}
	if (cfg->p2p_supported) {
		memcpy(p2p_mac.octet, cfg->p2p->int_addr.octet, ETHER_ADDR_LEN);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		/* Clear GO_NEG_PHASE bit to take care of GO-NEG-FAIL cases
		 */
		WL_DBG(("P2P: GO_NEG_PHASE status cleared "));
<<<<<<< HEAD
		wl_clr_p2p_status(wl, GO_NEG_PHASE);
		if (wl->p2p->vif_created) {
			if (wl_get_drv_status(wl, SCANNING, dev)) {
				wl_notify_escan_complete(wl, dev, true, true);
			}
			wldev_iovar_setint(dev, "mpc", 1);

			/* for GC */
			if (wl_get_drv_status(wl, DISCONNECTING, dev) &&
				(wl_get_mode_by_netdev(wl, dev) != WL_MODE_AP)) {
				WL_ERR(("Wait for Link Down event for GC !\n"));
				wait_for_completion_timeout
					(&wl->iface_disable, msecs_to_jiffies(500));
			}
			wl_set_p2p_status(wl, IF_DELETING);
			DNGL_FUNC(dhd_cfg80211_clean_p2p_info, (wl));

			/* for GO */
			if (wl_get_mode_by_netdev(wl, dev) == WL_MODE_AP) {
				wl_add_remove_eventmsg(dev, WLC_E_PROBREQ_MSG, false);
				/* disable interface before bsscfg free */
				ret = wl_cfgp2p_ifdisable(wl, &p2p_mac);
=======
		wl_clr_p2p_status(cfg, GO_NEG_PHASE);
		if (cfg->p2p->vif_created) {
			if (wl_get_drv_status(cfg, SCANNING, dev)) {
				wl_notify_escan_complete(cfg, dev, true, true);
			}
			wldev_iovar_setint(dev, "mpc", 1);
			/* Delete pm_enable_work */
			wl_add_remove_pm_enable_work(cfg, FALSE, WL_HANDLER_DEL);

			/* for GC */
			if (wl_get_drv_status(cfg, DISCONNECTING, dev) &&
				(wl_get_mode_by_netdev(cfg, dev) != WL_MODE_AP)) {
				WL_ERR(("Wait for Link Down event for GC !\n"));
				wait_for_completion_timeout
					(&cfg->iface_disable, msecs_to_jiffies(500));
			}

			memset(&cfg->if_event_info, 0, sizeof(cfg->if_event_info));
			wl_set_p2p_status(cfg, IF_DELETING);
			DNGL_FUNC(dhd_cfg80211_clean_p2p_info, (cfg));

			/* for GO */
			if (wl_get_mode_by_netdev(cfg, dev) == WL_MODE_AP) {
				wl_add_remove_eventmsg(dev, WLC_E_PROBREQ_MSG, false);
				/* disable interface before bsscfg free */
				ret = wl_cfgp2p_ifdisable(cfg, &p2p_mac);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				/* if fw doesn't support "ifdis",
				   do not wait for link down of ap mode
				 */
				if (ret == 0) {
					WL_ERR(("Wait for Link Down event for GO !!!\n"));
<<<<<<< HEAD
					wait_for_completion_timeout(&wl->iface_disable,
						msecs_to_jiffies(500));
				} else {
					msleep(300);
				}
			}
			wl_cfgp2p_clear_management_ie(wl, wl_cfgp2p_find_idx(wl, dev));
			/* delete interface after link down */
			ret = wl_cfgp2p_ifdel(wl, &p2p_mac);
			/* Firmware could not delete the interface so we will not get WLC_E_IF
			* event for cleaning the dhd virtual nw interace
			* So lets do it here. Failures from fw will ensure the application to do
			* ifconfig <inter> down and up sequnce, which will reload the fw
			* however we should cleanup the linux network virtual interfaces
			*/
			/* Request framework to RESET and clean up */
			if (ret) {
				struct net_device *ndev = wl_to_prmry_ndev(wl);
				WL_ERR(("Firmware returned an error (%d) from p2p_ifdel"
					"HANG Notification sent to %s\n", ret, ndev->name));
				net_os_send_hang_message(ndev);
			}
			/* Wait for IF_DEL operation to be finished in firmware */
			timeout = wait_event_interruptible_timeout(wl->netif_change_event,
				(wl->p2p->vif_created == false),
				msecs_to_jiffies(MAX_WAIT_TIME));
			if (timeout > 0 && (wl->p2p->vif_created == false)) {
				WL_DBG(("IFDEL operation done\n"));
			} else {
				WL_ERR(("IFDEL didn't complete properly\n"));
			}
			ret = dhd_del_monitor(dev);
=======
					wait_for_completion_timeout(&cfg->iface_disable,
						msecs_to_jiffies(500));
				} else if (ret != BCME_UNSUPPORTED) {
					msleep(300);
				}
			}
			wl_cfgp2p_clear_management_ie(cfg, index);

			if (wl_get_mode_by_netdev(cfg, dev) != WL_MODE_AP)
				wldev_iovar_setint(dev, "buf_key_b4_m4", 0);

			/* delete interface after link down */
			ret = wl_cfgp2p_ifdel(cfg, &p2p_mac);

			if (ret != BCME_OK) {
				struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);

				WL_ERR(("p2p_ifdel failed, error %d, sent HANG event to %s\n",
					ret, ndev->name));
				#if defined(BCMDONGLEHOST) && defined(OEM_ANDROID)
				net_os_send_hang_message(ndev);
				#endif 
			} else {
				/* Wait for IF_DEL operation to be finished */
				timeout = wait_event_interruptible_timeout(cfg->netif_change_event,
					(wl_get_p2p_status(cfg, IF_DELETING) == false),
					msecs_to_jiffies(MAX_WAIT_TIME));
				if (timeout > 0 && !wl_get_p2p_status(cfg, IF_DELETING) &&
					cfg->if_event_info.valid) {

					WL_DBG(("IFDEL operation done\n"));
					wl_cfg80211_handle_ifdel(cfg, &cfg->if_event_info, dev);
				} else {
					WL_ERR(("IFDEL didn't complete properly\n"));
				}
			}

			ret = dhd_del_monitor(dev);
			if (wl_get_mode_by_netdev(cfg, dev) == WL_MODE_AP) {
				DHD_OS_WAKE_LOCK_CTRL_TIMEOUT_CANCEL((dhd_pub_t *)(cfg->pub));
			}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	}
	return ret;
}

static s32
wl_cfg80211_change_virtual_iface(struct wiphy *wiphy, struct net_device *ndev,
	enum nl80211_iftype type, u32 *flags,
	struct vif_params *params)
{
	s32 ap = 0;
	s32 infra = 0;
<<<<<<< HEAD
	s32 wlif_type;
	s32 mode = 0;
	chanspec_t chspec;
	struct wl_priv *wl = wiphy_priv(wiphy);
	dhd_pub_t *dhd = (dhd_pub_t *)(wl->pub);
=======
	s32 ibss = 0;
	s32 wlif_type;
	s32 mode = 0;
	s32 err = BCME_OK;
	chanspec_t chspec;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	dhd_pub_t *dhd = (dhd_pub_t *)(cfg->pub);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	WL_DBG(("Enter type %d\n", type));
	switch (type) {
	case NL80211_IFTYPE_MONITOR:
	case NL80211_IFTYPE_WDS:
	case NL80211_IFTYPE_MESH_POINT:
		ap = 1;
		WL_ERR(("type (%d) : currently we do not support this type\n",
			type));
		break;
	case NL80211_IFTYPE_ADHOC:
		mode = WL_MODE_IBSS;
<<<<<<< HEAD
=======
		ibss = 1;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		break;
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_P2P_CLIENT:
		mode = WL_MODE_BSS;
		infra = 1;
		break;
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_P2P_GO:
		mode = WL_MODE_AP;
		ap = 1;
		break;
	default:
		return -EINVAL;
	}
	if (!dhd)
		return -EINVAL;
	if (ap) {
<<<<<<< HEAD
		wl_set_mode_by_netdev(wl, ndev, mode);
		if (wl->p2p_supported && wl->p2p->vif_created) {
			WL_DBG(("p2p_vif_created (%d) p2p_on (%d)\n", wl->p2p->vif_created,
			p2p_on(wl)));
			wldev_iovar_setint(ndev, "mpc", 0);
			wl_notify_escan_complete(wl, ndev, true, true);
=======
		wl_set_mode_by_netdev(cfg, ndev, mode);
		if (cfg->p2p_supported && cfg->p2p->vif_created) {
			WL_DBG(("p2p_vif_created (%d) p2p_on (%d)\n", cfg->p2p->vif_created,
			p2p_on(cfg)));
			wldev_iovar_setint(ndev, "mpc", 0);
			wl_notify_escan_complete(cfg, ndev, true, true);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

			/* In concurrency case, STA may be already associated in a particular
			 * channel. so retrieve the current channel of primary interface and
			 * then start the virtual interface on that.
			 */
			chspec = wl_cfg80211_get_shared_freq(wiphy);

			wlif_type = WL_P2P_IF_GO;
			WL_ERR(("%s : ap (%d), infra (%d), iftype: (%d)\n",
				ndev->name, ap, infra, type));
<<<<<<< HEAD
			wl_set_p2p_status(wl, IF_CHANGING);
			wl_clr_p2p_status(wl, IF_CHANGED);
			wl_cfgp2p_ifchange(wl, &wl->p2p->int_addr, htod32(wlif_type), chspec);
			wait_event_interruptible_timeout(wl->netif_change_event,
				(wl_get_p2p_status(wl, IF_CHANGED) == true),
				msecs_to_jiffies(MAX_WAIT_TIME));
			wl_set_mode_by_netdev(wl, ndev, mode);
			dhd->op_mode &= ~DHD_FLAG_P2P_GC_MODE;
			dhd->op_mode |= DHD_FLAG_P2P_GO_MODE;
			wl_clr_p2p_status(wl, IF_CHANGING);
			wl_clr_p2p_status(wl, IF_CHANGED);
			if (mode == WL_MODE_AP)
				wl_set_drv_status(wl, CONNECTED, ndev);
		} else if (ndev == wl_to_prmry_ndev(wl) &&
			!wl_get_drv_status(wl, AP_CREATED, ndev)) {
			wl_set_drv_status(wl, AP_CREATING, ndev);
			if (!wl->ap_info &&
				!(wl->ap_info = kzalloc(sizeof(struct ap_info), GFP_KERNEL))) {
=======
			wl_set_p2p_status(cfg, IF_CHANGING);
			wl_clr_p2p_status(cfg, IF_CHANGED);
			wl_cfgp2p_ifchange(cfg, &cfg->p2p->int_addr, htod32(wlif_type), chspec);
			wait_event_interruptible_timeout(cfg->netif_change_event,
				(wl_get_p2p_status(cfg, IF_CHANGED) == true),
				msecs_to_jiffies(MAX_WAIT_TIME));
			wl_set_mode_by_netdev(cfg, ndev, mode);
			dhd->op_mode &= ~DHD_FLAG_P2P_GC_MODE;
			dhd->op_mode |= DHD_FLAG_P2P_GO_MODE;
			wl_clr_p2p_status(cfg, IF_CHANGING);
			wl_clr_p2p_status(cfg, IF_CHANGED);
			if (mode == WL_MODE_AP)
				wl_set_drv_status(cfg, CONNECTED, ndev);
		} else if (ndev == bcmcfg_to_prmry_ndev(cfg) &&
			!wl_get_drv_status(cfg, AP_CREATED, ndev)) {
			wl_set_drv_status(cfg, AP_CREATING, ndev);
			if (!cfg->ap_info &&
				!(cfg->ap_info = kzalloc(sizeof(struct ap_info), GFP_KERNEL))) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				WL_ERR(("struct ap_saved_ie allocation failed\n"));
				return -ENOMEM;
			}
		} else {
			WL_ERR(("Cannot change the interface for GO or SOFTAP\n"));
			return -EINVAL;
		}
	} else {
		WL_DBG(("Change_virtual_iface for transition from GO/AP to client/STA"));
	}

<<<<<<< HEAD
=======
	if (ibss) {
		infra = 0;
		wl_set_mode_by_netdev(cfg, ndev, mode);
		err = wldev_ioctl(ndev, WLC_SET_INFRA, &infra, sizeof(s32), true);
		if (err < 0) {
			WL_ERR(("SET Adhoc error %d\n", err));
			return -EINVAL;
		}
	}

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	ndev->ieee80211_ptr->iftype = type;
	return 0;
}

s32
<<<<<<< HEAD
wl_cfg80211_notify_ifadd(struct net_device *ndev, s32 idx, s32 bssidx,
	void* _net_attach)
{
	struct wl_priv *wl = wlcfg_drv_priv;
	s32 ret = BCME_OK;
	WL_DBG(("Enter"));
	if (!ndev) {
		WL_ERR(("net is NULL\n"));
		return 0;
	}
	if (wl->p2p_supported && wl_get_p2p_status(wl, IF_ADD)) {
		WL_DBG(("IF_ADD event called from dongle, old interface name: %s,"
			"new name: %s\n", ndev->name, wl->p2p->vir_ifname));
		/* Assign the net device to CONNECT BSSCFG */
		strncpy(ndev->name, wl->p2p->vir_ifname, IFNAMSIZ - 1);
		wl_to_p2p_bss_ndev(wl, P2PAPI_BSSCFG_CONNECTION) = ndev;
		wl_to_p2p_bss_bssidx(wl, P2PAPI_BSSCFG_CONNECTION) = bssidx;
		wl_to_p2p_bss_private(wl, P2PAPI_BSSCFG_CONNECTION) = _net_attach;
		ndev->ifindex = idx;
		wl_clr_p2p_status(wl, IF_ADD);

		wake_up_interruptible(&wl->netif_change_event);
	} else {
		ret = BCME_NOTREADY;
	}
	return ret;
}

s32
wl_cfg80211_notify_ifdel(void)
{
	struct wl_priv *wl = wlcfg_drv_priv;

	WL_DBG(("Enter \n"));
	wl_clr_p2p_status(wl, IF_DELETING);
	wake_up_interruptible(&wl->netif_change_event);
	return 0;
}

s32
wl_cfg80211_ifdel_ops(struct net_device *ndev)
{
	struct wl_priv *wl = wlcfg_drv_priv;
	bool rollback_lock = false;
	s32 index = 0;
#ifdef PROP_TXSTATUS_VSDB
	dhd_pub_t *dhd =  (dhd_pub_t *)(wl->pub);
#endif /* PROP_TXSTATUS_VSDB */
	if (!ndev || (strlen(ndev->name) == 0)) {
		WL_ERR(("net is NULL\n"));
		return 0;
	}

	if (p2p_is_on(wl) && wl->p2p->vif_created &&
		wl_get_p2p_status(wl, IF_DELETING)) {
		if (wl->scan_request &&
			(wl->escan_info.ndev == ndev)) {
			/* Abort any pending scan requests */
			wl->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
			if (!rtnl_is_locked()) {
				rtnl_lock();
				rollback_lock = true;
			}
			WL_DBG(("ESCAN COMPLETED\n"));
			wl_notify_escan_complete(wl, ndev, true, false);
			if (rollback_lock)
				rtnl_unlock();
		}
		WL_ERR(("IF_DEL event called from dongle, net %x, vif name: %s\n",
			(unsigned int)ndev, wl->p2p->vir_ifname));

		memset(wl->p2p->vir_ifname, '\0', IFNAMSIZ);
		index = wl_cfgp2p_find_idx(wl, ndev);
		wl_to_p2p_bss_ndev(wl, index) = NULL;
		wl_to_p2p_bss_bssidx(wl, index) = WL_INVALID;
		wl->p2p->vif_created = false;

		WL_DBG(("index : %d\n", index));
#ifdef PROP_TXSTATUS_VSDB
		if (dhd->wlfc_enabled && wl->wlfc_on) {
			dhd->wlfc_enabled = false;
			dhd_wlfc_deinit(dhd);
			wl->wlfc_on = false;
		}
#endif /* PROP_TXSTATUS_VSDB */
		wl_clr_drv_status(wl, CONNECTED, ndev);
	}
	/* Wake up any waiting thread */
	wake_up_interruptible(&wl->netif_change_event);

	return 0;
}

s32
wl_cfg80211_is_progress_ifadd(void)
{
	s32 is_progress = 0;
	struct wl_priv *wl = wlcfg_drv_priv;
	if (wl_get_p2p_status(wl, IF_ADD))
		is_progress = 1;
	return is_progress;
}

s32
wl_cfg80211_is_progress_ifchange(void)
{
	s32 is_progress = 0;
	struct wl_priv *wl = wlcfg_drv_priv;
	if (wl_get_p2p_status(wl, IF_CHANGING))
		is_progress = 1;
	return is_progress;
}


s32
wl_cfg80211_notify_ifchange(void)
{
	struct wl_priv *wl = wlcfg_drv_priv;
	if (wl_get_p2p_status(wl, IF_CHANGING)) {
		wl_set_p2p_status(wl, IF_CHANGED);
		wake_up_interruptible(&wl->netif_change_event);
	}
	return 0;
}

/* Find listen channel */
static s32 wl_find_listen_channel(struct wl_priv *wl,
	u8 *ie, u32 ie_len)
{
	wifi_p2p_ie_t *p2p_ie;
	u8 *end, *pos;
	s32 listen_channel;

	p2p_ie = wl_cfgp2p_find_p2pie(ie, ie_len);

	if (p2p_ie == NULL)
		return 0;
=======
wl_cfg80211_notify_ifadd(int ifidx, char *name, uint8 *mac, uint8 bssidx)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;

	/* P2P may send WLC_E_IF_ADD and/or WLC_E_IF_CHANGE during IF updating ("p2p_ifupd")
	 * redirect the IF_ADD event to ifchange as it is not a real "new" interface
	 */
	if (wl_get_p2p_status(cfg, IF_CHANGING))
		return wl_cfg80211_notify_ifchange(ifidx, name, mac, bssidx);

	/* Okay, we are expecting IF_ADD (as IF_ADDING is true) */
	if (wl_get_p2p_status(cfg, IF_ADDING)) {
		wl_if_event_info *if_event_info = &cfg->if_event_info;

		if_event_info->valid = TRUE;
		if_event_info->ifidx = ifidx;
		if_event_info->bssidx = bssidx;
		strncpy(if_event_info->name, name, IFNAMSIZ);
		if_event_info->name[IFNAMSIZ] = '\0';
		if (mac)
			memcpy(if_event_info->mac, mac, ETHER_ADDR_LEN);

		wl_clr_p2p_status(cfg, IF_ADDING);
		wake_up_interruptible(&cfg->netif_change_event);
		return BCME_OK;
	}

	return BCME_ERROR;
}

s32
wl_cfg80211_notify_ifdel(int ifidx, char *name, uint8 *mac, uint8 bssidx)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	wl_if_event_info *if_event_info = &cfg->if_event_info;

	if (wl_get_p2p_status(cfg, IF_DELETING)) {
		if_event_info->valid = TRUE;
		if_event_info->ifidx = ifidx;
		if_event_info->bssidx = bssidx;
		wl_clr_p2p_status(cfg, IF_DELETING);
		wake_up_interruptible(&cfg->netif_change_event);
		return BCME_OK;
	}

	return BCME_ERROR;
}

s32
wl_cfg80211_notify_ifchange(int ifidx, char *name, uint8 *mac, uint8 bssidx)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;

	if (wl_get_p2p_status(cfg, IF_CHANGING)) {
		wl_set_p2p_status(cfg, IF_CHANGED);
		wake_up_interruptible(&cfg->netif_change_event);
		return BCME_OK;
	}

	return BCME_ERROR;
}

static s32 wl_cfg80211_handle_ifdel(struct bcm_cfg80211 *cfg, wl_if_event_info *if_event_info,
	struct net_device* ndev)
{
	s32 type = -1;
	s32 bssidx = -1;
#ifdef PROP_TXSTATUS_VSDB
	dhd_pub_t *dhd =  (dhd_pub_t *)(cfg->pub);
	bool enabled;
#endif /* PROP_TXSTATUS_VSDB */

	bssidx = if_event_info->bssidx;
	if (bssidx != wl_to_p2p_bss_bssidx(cfg, P2PAPI_BSSCFG_CONNECTION)) {
		WL_ERR(("got IF_DEL for if %d, not owned by cfg driver\n", bssidx));
		return BCME_ERROR;
	}

	if (p2p_is_on(cfg) && cfg->p2p->vif_created) {

		if (cfg->scan_request && (cfg->escan_info.ndev == ndev)) {
			/* Abort any pending scan requests */
			cfg->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
			WL_DBG(("ESCAN COMPLETED\n"));
			wl_notify_escan_complete(cfg, cfg->escan_info.ndev, true, false);
		}

		memset(cfg->p2p->vir_ifname, '\0', IFNAMSIZ);
		if (wl_cfgp2p_find_type(cfg, bssidx, &type) != BCME_OK) {
			WL_ERR(("Find p2p type from bssidx(%d) failed\n", bssidx));
			return BCME_ERROR;
		}
		wl_clr_drv_status(cfg, CONNECTED, wl_to_p2p_bss_ndev(cfg, type));
		wl_to_p2p_bss_ndev(cfg, type) = NULL;
		wl_to_p2p_bss_bssidx(cfg, type) = WL_INVALID;
		cfg->p2p->vif_created = false;

#ifdef PROP_TXSTATUS_VSDB
		dhd_wlfc_get_enable(dhd, &enabled);
		if (enabled && cfg->wlfc_on && dhd->op_mode != DHD_FLAG_HOSTAP_MODE &&
			dhd->op_mode != DHD_FLAG_IBSS_MODE) {
			dhd_wlfc_deinit(dhd);
			cfg->wlfc_on = false;
		}
#endif /* PROP_TXSTATUS_VSDB */
	}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0))
	wl_cfg80211_remove_if(cfg, if_event_info->ifidx, ndev);
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)) */
	return BCME_OK;
}

/* Find listen channel */
static s32 wl_find_listen_channel(struct bcm_cfg80211 *cfg,
	const u8 *ie, u32 ie_len)
{
	wifi_p2p_ie_t *p2p_ie;
	u8 *end, *pos;
	s32 listen_channel;

	pos = (u8 *)ie;
	p2p_ie = wl_cfgp2p_find_p2pie(pos, ie_len);

	if (p2p_ie == NULL)
		return 0;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	pos = p2p_ie->subelts;
	end = p2p_ie->subelts + (p2p_ie->len - 4);

	CFGP2P_DBG((" found p2p ie ! lenth %d \n",
		p2p_ie->len));

	while (pos < end) {
		uint16 attr_len;
		if (pos + 2 >= end) {
			CFGP2P_DBG((" -- Invalid P2P attribute"));
			return 0;
		}
		attr_len = ((uint16) (((pos + 1)[1] << 8) | (pos + 1)[0]));

		if (pos + 3 + attr_len > end) {
			CFGP2P_DBG(("P2P: Attribute underflow "
				   "(len=%u left=%d)",
				   attr_len, (int) (end - pos - 3)));
			return 0;
		}

		/* if Listen Channel att id is 6 and the vailue is valid,
		 * return the listen channel
		 */
		if (pos[0] == 6) {
			/* listen channel subel length format
			 * 1(id) + 2(len) + 3(country) + 1(op. class) + 1(chan num)
			 */
			listen_channel = pos[1 + 2 + 3 + 1];

			if (listen_channel == SOCIAL_CHAN_1 ||
				listen_channel == SOCIAL_CHAN_2 ||
				listen_channel == SOCIAL_CHAN_3) {
				CFGP2P_DBG((" Found my Listen Channel %d \n", listen_channel));
				return listen_channel;
			}
		}
		pos += 3 + attr_len;
	}
	return 0;
}

static void wl_scan_prep(struct wl_scan_params *params, struct cfg80211_scan_request *request)
{
	u32 n_ssids;
	u32 n_channels;
	u16 channel;
	chanspec_t chanspec;
	s32 i = 0, j = 0, offset;
	char *ptr;
	wlc_ssid_t ssid;
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	memcpy(&params->bssid, &ether_bcast, ETHER_ADDR_LEN);
	params->bss_type = DOT11_BSSTYPE_ANY;
	params->scan_type = 0;
	params->nprobes = -1;
	params->active_time = -1;
	params->passive_time = -1;
	params->home_time = -1;
	params->channel_num = 0;
	memset(&params->ssid, 0, sizeof(wlc_ssid_t));

	WL_SCAN(("Preparing Scan request\n"));
	WL_SCAN(("nprobes=%d\n", params->nprobes));
	WL_SCAN(("active_time=%d\n", params->active_time));
	WL_SCAN(("passive_time=%d\n", params->passive_time));
	WL_SCAN(("home_time=%d\n", params->home_time));
	WL_SCAN(("scan_type=%d\n", params->scan_type));

	params->nprobes = htod32(params->nprobes);
	params->active_time = htod32(params->active_time);
	params->passive_time = htod32(params->passive_time);
	params->home_time = htod32(params->home_time);

	/* if request is null just exit so it will be all channel broadcast scan */
	if (!request)
		return;

	n_ssids = request->n_ssids;
	n_channels = request->n_channels;

	/* Copy channel array if applicable */
	WL_SCAN(("### List of channelspecs to scan ###\n"));
	if (n_channels > 0) {
		for (i = 0; i < n_channels; i++) {
			chanspec = 0;
			channel = ieee80211_frequency_to_channel(request->channels[i]->center_freq);
			/* SKIP DFS channels for Secondary interface */
<<<<<<< HEAD
			if ((wl->escan_info.ndev != wl_to_prmry_ndev(wl)) &&
=======
			if ((cfg->escan_info.ndev != bcmcfg_to_prmry_ndev(cfg)) &&
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				(request->channels[i]->flags &
				(IEEE80211_CHAN_RADAR | IEEE80211_CHAN_PASSIVE_SCAN)))
				continue;

			if (request->channels[i]->band == IEEE80211_BAND_2GHZ) {
#ifdef WL_HOST_BAND_MGMT
<<<<<<< HEAD
				if (wl->curr_band == WLC_BAND_5G) {
=======
				if (cfg->curr_band == WLC_BAND_5G) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
					WL_DBG(("In 5G only mode, omit 2G channel:%d\n", channel));
					continue;
				}
#endif /* WL_HOST_BAND_MGMT */
				chanspec |= WL_CHANSPEC_BAND_2G;
			} else {
#ifdef WL_HOST_BAND_MGMT
<<<<<<< HEAD
				if (wl->curr_band == WLC_BAND_2G) {
=======
				if (cfg->curr_band == WLC_BAND_2G) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
					WL_DBG(("In 2G only mode, omit 5G channel:%d\n", channel));
					continue;
				}
#endif /* WL_HOST_BAND_MGMT */
				chanspec |= WL_CHANSPEC_BAND_5G;
			}

			chanspec |= WL_CHANSPEC_BW_20;
			chanspec |= WL_CHANSPEC_CTL_SB_NONE;

			params->channel_list[j] = channel;
			params->channel_list[j] &= WL_CHANSPEC_CHAN_MASK;
			params->channel_list[j] |= chanspec;
			WL_SCAN(("Chan : %d, Channel spec: %x \n",
				channel, params->channel_list[j]));
			params->channel_list[j] = wl_chspec_host_to_driver(params->channel_list[j]);
			j++;
		}
	} else {
		WL_SCAN(("Scanning all channels\n"));
	}
	n_channels = j;
	/* Copy ssid array if applicable */
	WL_SCAN(("### List of SSIDs to scan ###\n"));
	if (n_ssids > 0) {
		offset = offsetof(wl_scan_params_t, channel_list) + n_channels * sizeof(u16);
		offset = roundup(offset, sizeof(u32));
		ptr = (char*)params + offset;
		for (i = 0; i < n_ssids; i++) {
			memset(&ssid, 0, sizeof(wlc_ssid_t));
			ssid.SSID_len = request->ssids[i].ssid_len;
			memcpy(ssid.SSID, request->ssids[i].ssid, ssid.SSID_len);
			if (!ssid.SSID_len)
				WL_SCAN(("%d: Broadcast scan\n", i));
			else
				WL_SCAN(("%d: scan  for  %s size =%d\n", i,
				ssid.SSID, ssid.SSID_len));
			memcpy(ptr, &ssid, sizeof(wlc_ssid_t));
			ptr += sizeof(wlc_ssid_t);
		}
	} else {
		WL_SCAN(("Broadcast scan\n"));
	}
	/* Adding mask to channel numbers */
	params->channel_num =
	        htod32((n_ssids << WL_SCAN_PARAMS_NSSID_SHIFT) |
	               (n_channels & WL_SCAN_PARAMS_COUNT_MASK));

<<<<<<< HEAD
	if (n_channels == 1 && wl_get_drv_status_all(wl, CONNECTED)) {
		params->active_time = WL_SCAN_CONNECT_DWELL_TIME_MS;
	}
}

static s32
wl_run_iscan(struct wl_iscan_ctrl *iscan, struct cfg80211_scan_request *request, u16 action)
{
	u32 n_channels;
	u32 n_ssids;
	s32 params_size =
	    (WL_SCAN_PARAMS_FIXED_SIZE + offsetof(wl_iscan_params_t, params));
	struct wl_iscan_params *params = NULL;
	s32 err = 0;

	if (request != NULL) {
		n_channels = request->n_channels;
		n_ssids = request->n_ssids;
		/* Allocate space for populating ssids in wl_iscan_params struct */
		if (n_channels % 2)
			/* If n_channels is odd, add a padd of u16 */
			params_size += sizeof(u16) * (n_channels + 1);
		else
			params_size += sizeof(u16) * n_channels;

		/* Allocate space for populating ssids in wl_iscan_params struct */
		params_size += sizeof(struct wlc_ssid) * n_ssids;
	}
	params = (struct wl_iscan_params *)kzalloc(params_size, GFP_KERNEL);
	if (!params) {
		err = -ENOMEM;
		goto done;
	}
	wl_scan_prep(&params->params, request);

	params->version = htod32(ISCAN_REQ_VERSION);
	params->action = htod16(action);
	params->scan_duration = htod16(0);

	if (params_size + sizeof("iscan") >= WLC_IOCTL_MEDLEN) {
		WL_ERR(("ioctl buffer length is not sufficient\n"));
		err = -ENOMEM;
		goto done;
	}
	err = wldev_iovar_setbuf(iscan->dev, "iscan", params, params_size,
		iscan->ioctl_buf, WLC_IOCTL_MEDLEN, NULL);
	if (unlikely(err)) {
		if (err == -EBUSY) {
			WL_ERR(("system busy : iscan canceled\n"));
		} else {
			WL_ERR(("error (%d)\n", err));
		}
	}

done:
	if (params)
		kfree(params);
	return err;
}

static s32 wl_do_iscan(struct wl_priv *wl, struct cfg80211_scan_request *request)
{
	struct wl_iscan_ctrl *iscan = wl_to_iscan(wl);
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	s32 passive_scan;
	s32 err = 0;

	iscan->state = WL_ISCAN_STATE_SCANING;

	passive_scan = wl->active_scan ? 0 : 1;
	err = wldev_ioctl(ndev, WLC_SET_PASSIVE_SCAN,
		&passive_scan, sizeof(passive_scan), true);
	if (unlikely(err)) {
		WL_DBG(("error (%d)\n", err));
		return err;
	}
	wl->iscan_kickstart = true;
	wl_run_iscan(iscan, request, WL_SCAN_ACTION_START);
	mod_timer(&iscan->timer, jiffies + msecs_to_jiffies(iscan->timer_ms));
	iscan->timer_on = 1;

	return err;
}
=======
	if (n_channels == 1) {
		params->active_time = htod32(WL_SCAN_CONNECT_DWELL_TIME_MS);
		params->nprobes = htod32(params->active_time / WL_SCAN_JOIN_PROBE_INTERVAL_MS);
	}
}

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32
wl_get_valid_channels(struct net_device *ndev, u8 *valid_chan_list, s32 size)
{
	wl_uint32_list_t *list;
	s32 err = BCME_OK;
	if (valid_chan_list == NULL || size <= 0)
		return -ENOMEM;

	memset(valid_chan_list, 0, size);
	list = (wl_uint32_list_t *)(void *) valid_chan_list;
	list->count = htod32(WL_NUMCHANNELS);
	err = wldev_ioctl(ndev, WLC_GET_VALID_CHANNELS, valid_chan_list, size, false);
	if (err != 0) {
		WL_ERR(("get channels failed with %d\n", err));
	}

	return err;
}

<<<<<<< HEAD

static s32
wl_run_escan(struct wl_priv *wl, struct net_device *ndev,
=======
#if defined(USE_INITIAL_2G_SCAN) || defined(USE_INITIAL_SHORT_DWELL_TIME)
#define FIRST_SCAN_ACTIVE_DWELL_TIME_MS 40
bool g_first_broadcast_scan = TRUE;
#endif /* USE_INITIAL_2G_SCAN || USE_INITIAL_SHORT_DWELL_TIME */

static s32
wl_run_escan(struct bcm_cfg80211 *cfg, struct net_device *ndev,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct cfg80211_scan_request *request, uint16 action)
{
	s32 err = BCME_OK;
	u32 n_channels;
	u32 n_ssids;
	s32 params_size = (WL_SCAN_PARAMS_FIXED_SIZE + OFFSETOF(wl_escan_params_t, params));
	wl_escan_params_t *params = NULL;
	u8 chan_buf[sizeof(u32)*(WL_NUMCHANNELS + 1)];
	u32 num_chans = 0;
	s32 channel;
	s32 n_valid_chan;
	s32 search_state = WL_P2P_DISC_ST_SCAN;
	u32 i, j, n_nodfs = 0;
	u16 *default_chan_list = NULL;
	wl_uint32_list_t *list;
	struct net_device *dev = NULL;
<<<<<<< HEAD

	WL_DBG(("Enter \n"));

	if (!wl) {
		err = -EINVAL;
		goto exit;
	}
	if (!wl->p2p_supported || !p2p_scan(wl)) {
		/* LEGACY SCAN TRIGGER */
		WL_SCAN((" LEGACY E-SCAN START\n"));

		/* if scan request is not empty parse scan request paramters */
		if (request != NULL) {


			n_channels = request->n_channels;
			n_ssids = request->n_ssids;
			/* Allocate space for populating ssids in wl_iscan_params struct */
=======
#if defined(USE_INITIAL_2G_SCAN) || defined(USE_INITIAL_SHORT_DWELL_TIME)
	bool is_first_init_2g_scan = false;
#endif /* USE_INITIAL_2G_SCAN || USE_INITIAL_SHORT_DWELL_TIME */
	p2p_scan_purpose_t	p2p_scan_purpose = P2P_SCAN_PURPOSE_MIN;

	WL_DBG(("Enter \n"));

	/* scan request can come with empty request : perform all default scan */
	if (!cfg) {
		err = -EINVAL;
		goto exit;
	}
	if (!cfg->p2p_supported || !p2p_scan(cfg)) {
		/* LEGACY SCAN TRIGGER */
		WL_SCAN((" LEGACY E-SCAN START\n"));

#if defined(USE_INITIAL_2G_SCAN) || defined(USE_INITIAL_SHORT_DWELL_TIME)
		if (!request) {
			err = -EINVAL;
			goto exit;
		}
		if (ndev == bcmcfg_to_prmry_ndev(cfg) && g_first_broadcast_scan == true) {
#ifdef USE_INITIAL_2G_SCAN
			struct ieee80211_channel tmp_channel_list[CH_MAX_2G_CHANNEL];
			/* allow one 5G channel to add previous connected channel in 5G */
			bool allow_one_5g_channel = TRUE;
			j = 0;
			for (i = 0; i < request->n_channels; i++) {
				int tmp_chan = ieee80211_frequency_to_channel
					(request->channels[i]->center_freq);
				if (tmp_chan > CH_MAX_2G_CHANNEL) {
					if (allow_one_5g_channel)
						allow_one_5g_channel = FALSE;
					else
						continue;
				}
				if (j > CH_MAX_2G_CHANNEL) {
					WL_ERR(("Index %d exceeds max 2.4GHz channels %d"
						" and previous 5G connected channel\n",
						j, CH_MAX_2G_CHANNEL));
					break;
				}
#if defined(BCM4334_CHIP)
				request->channels[i]->flags |=
					IEEE80211_CHAN_NO_HT40;
#endif
				bcopy(request->channels[i], &tmp_channel_list[j],
					sizeof(struct ieee80211_channel));
				WL_SCAN(("channel of request->channels[%d]=%d\n", i, tmp_chan));
				j++;
			}
			if ((j > 0) && (j <= CH_MAX_2G_CHANNEL)) {
				for (i = 0; i < j; i++)
					bcopy(&tmp_channel_list[i], request->channels[i],
						sizeof(struct ieee80211_channel));

				request->n_channels = j;
				is_first_init_2g_scan = true;
			}
			else
				WL_ERR(("Invalid number of 2.4GHz channels %d\n", j));

			WL_SCAN(("request->n_channels=%d\n", request->n_channels));
#else /* USE_INITIAL_SHORT_DWELL_TIME */
			is_first_init_2g_scan = true;
#endif /* USE_INITIAL_2G_SCAN */
			g_first_broadcast_scan = false;
		}
#endif /* USE_INITIAL_2G_SCAN || USE_INITIAL_SHORT_DWELL_TIME */

		/* if scan request is not empty parse scan request paramters */
		if (request != NULL) {
			n_channels = request->n_channels;
			n_ssids = request->n_ssids;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			if (n_channels % 2)
				/* If n_channels is odd, add a padd of u16 */
				params_size += sizeof(u16) * (n_channels + 1);
			else
				params_size += sizeof(u16) * n_channels;

<<<<<<< HEAD
			/* Allocate space for populating ssids in wl_iscan_params struct */
=======
			/* Allocate space for populating ssids in wl_escan_params_t struct */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			params_size += sizeof(struct wlc_ssid) * n_ssids;
		}
		params = (wl_escan_params_t *) kzalloc(params_size, GFP_KERNEL);
		if (params == NULL) {
			err = -ENOMEM;
			goto exit;
		}
		wl_scan_prep(&params->params, request);

<<<<<<< HEAD

		params->version = htod32(ESCAN_REQ_VERSION);
		params->action =  htod16(action);
		params->sync_id = htod16(0x1234);
=======
#if defined(USE_INITIAL_2G_SCAN) || defined(USE_INITIAL_SHORT_DWELL_TIME)
		/* Override active_time to reduce scan time if it's first bradcast scan. */
		if (is_first_init_2g_scan)
			params->params.active_time = FIRST_SCAN_ACTIVE_DWELL_TIME_MS;
#endif /* USE_INITIAL_2G_SCAN || USE_INITIAL_SHORT_DWELL_TIME */

		params->version = htod32(ESCAN_REQ_VERSION);
		params->action =  htod16(action);
		wl_escan_set_sync_id(params->sync_id, cfg);
		wl_escan_set_type(cfg, WL_SCANTYPE_LEGACY);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (params_size + sizeof("escan") >= WLC_IOCTL_MEDLEN) {
			WL_ERR(("ioctl buffer length not sufficient\n"));
			kfree(params);
			err = -ENOMEM;
			goto exit;
		}
		err = wldev_iovar_setbuf(ndev, "escan", params, params_size,
<<<<<<< HEAD
			wl->escan_ioctl_buf, WLC_IOCTL_MEDLEN, NULL);
=======
			cfg->escan_ioctl_buf, WLC_IOCTL_MEDLEN, NULL);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (unlikely(err)) {
			if (err == BCME_EPERM)
				/* Scan Not permitted at this point of time */
				WL_DBG((" Escan not permitted at this time (%d)\n", err));
			else
				WL_ERR((" Escan set error (%d)\n", err));
		}
		kfree(params);
	}
<<<<<<< HEAD
	else if (p2p_is_on(wl) && p2p_scan(wl)) {
=======
	else if (p2p_is_on(cfg) && p2p_scan(cfg)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* P2P SCAN TRIGGER */
		s32 _freq = 0;
		n_nodfs = 0;
		if (request && request->n_channels) {
			num_chans = request->n_channels;
			WL_SCAN((" chann number : %d\n", num_chans));
			default_chan_list = kzalloc(num_chans * sizeof(*default_chan_list),
				GFP_KERNEL);
			if (default_chan_list == NULL) {
				WL_ERR(("channel list allocation failed \n"));
				err = -ENOMEM;
				goto exit;
			}
			if (!wl_get_valid_channels(ndev, chan_buf, sizeof(chan_buf))) {
<<<<<<< HEAD
=======
#ifdef CUSTOMER_HW4
				int is_printed = false;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				list = (wl_uint32_list_t *) chan_buf;
				n_valid_chan = dtoh32(list->count);
				for (i = 0; i < num_chans; i++)
				{
#ifdef WL_HOST_BAND_MGMT
					int channel_band = 0;
#endif /* WL_HOST_BAND_MGMT */
					_freq = request->channels[i]->center_freq;
					channel = ieee80211_frequency_to_channel(_freq);
#ifdef WL_HOST_BAND_MGMT
					channel_band = (channel > CH_MAX_2G_CHANNEL) ?
						WLC_BAND_5G : WLC_BAND_2G;
<<<<<<< HEAD
					if ((wl->curr_band != WLC_BAND_AUTO) &&
						(wl->curr_band != channel_band) &&
=======
					if ((cfg->curr_band != WLC_BAND_AUTO) &&
						(cfg->curr_band != channel_band) &&
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
						!IS_P2P_SOCIAL_CHANNEL(channel))
							continue;
#endif /* WL_HOST_BAND_MGMT */

					/* ignore DFS channels */
					if (request->channels[i]->flags &
						(IEEE80211_CHAN_RADAR
						| IEEE80211_CHAN_PASSIVE_SCAN))
						continue;
<<<<<<< HEAD
=======
#ifdef CUSTOMER_HW4
					if (channel >= 52 && channel <= 140) {
						if (is_printed == false) {
							WL_ERR(("SKIP DFS CHANs(52~140)\n"));
							is_printed = true;
						}
						continue;
					}
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

					for (j = 0; j < n_valid_chan; j++) {
						/* allows only supported channel on
						*  current reguatory
						*/
						if (channel == (dtoh32(list->element[j])))
							default_chan_list[n_nodfs++] =
								channel;
					}

				}
			}
<<<<<<< HEAD
			if (num_chans == 3 && (
=======
			if (num_chans == SOCIAL_CHAN_CNT && (
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
						(default_chan_list[0] == SOCIAL_CHAN_1) &&
						(default_chan_list[1] == SOCIAL_CHAN_2) &&
						(default_chan_list[2] == SOCIAL_CHAN_3))) {
				/* SOCIAL CHANNELS 1, 6, 11 */
				search_state = WL_P2P_DISC_ST_SEARCH;
<<<<<<< HEAD
				WL_INFO(("P2P SEARCH PHASE START \n"));
			} else if ((dev = wl_to_p2p_bss_ndev(wl, P2PAPI_BSSCFG_CONNECTION)) &&
				(wl_get_mode_by_netdev(wl, dev) == WL_MODE_AP)) {
=======
				p2p_scan_purpose = P2P_SCAN_SOCIAL_CHANNEL;
				WL_INFO(("P2P SEARCH PHASE START \n"));
			} else if ((dev = wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_CONNECTION)) &&
				(wl_get_mode_by_netdev(cfg, dev) == WL_MODE_AP)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				/* If you are already a GO, then do SEARCH only */
				WL_INFO(("Already a GO. Do SEARCH Only"));
				search_state = WL_P2P_DISC_ST_SEARCH;
				num_chans = n_nodfs;
<<<<<<< HEAD

			} else {
				WL_INFO(("P2P SCAN STATE START \n"));
				num_chans = n_nodfs;
			}

		}
		err = wl_cfgp2p_escan(wl, ndev, wl->active_scan, num_chans, default_chan_list,
			search_state, action,
			wl_to_p2p_bss_bssidx(wl, P2PAPI_BSSCFG_DEVICE));
=======
				p2p_scan_purpose = P2P_SCAN_NORMAL;

			} else if (num_chans == 1) {
				p2p_scan_purpose = P2P_SCAN_CONNECT_TRY;
			} else if (num_chans == SOCIAL_CHAN_CNT + 1) {
			/* SOCIAL_CHAN_CNT + 1 takes care of the Progressive scan supported by
			 * the supplicant
			 */
				p2p_scan_purpose = P2P_SCAN_SOCIAL_CHANNEL;
			} else {
				WL_INFO(("P2P SCAN STATE START \n"));
				num_chans = n_nodfs;
				p2p_scan_purpose = P2P_SCAN_NORMAL;
			}
		} else {
			err = -EINVAL;
			goto exit;
		}
		err = wl_cfgp2p_escan(cfg, ndev, cfg->active_scan, num_chans, default_chan_list,
			search_state, action,
			wl_to_p2p_bss_bssidx(cfg, P2PAPI_BSSCFG_DEVICE), NULL,
			p2p_scan_purpose);

		if (!err)
			cfg->p2p->search_state = search_state;

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		kfree(default_chan_list);
	}
exit:
	if (unlikely(err)) {
		/* Don't print Error incase of Scan suppress */
<<<<<<< HEAD
		if ((err == BCME_EPERM) && wl->scan_suppressed)
=======
		if ((err == BCME_EPERM) && cfg->scan_suppressed)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_DBG(("Escan failed: Scan Suppressed \n"));
		else
			WL_ERR(("error (%d)\n", err));
	}
	return err;
}


static s32
<<<<<<< HEAD
wl_do_escan(struct wl_priv *wl, struct wiphy *wiphy, struct net_device *ndev,
=======
wl_do_escan(struct bcm_cfg80211 *cfg, struct wiphy *wiphy, struct net_device *ndev,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct cfg80211_scan_request *request)
{
	s32 err = BCME_OK;
	s32 passive_scan;
	wl_scan_results_t *results;
	WL_SCAN(("Enter \n"));
<<<<<<< HEAD
	mutex_lock(&wl->usr_sync);
	results = (wl_scan_results_t *) wl->escan_info.escan_buf;
=======
	mutex_lock(&cfg->usr_sync);

	results = wl_escan_get_buf(cfg, FALSE);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	results->version = 0;
	results->count = 0;
	results->buflen = WL_SCAN_RESULTS_FIXED_SIZE;

<<<<<<< HEAD
	wl->escan_info.ndev = ndev;
	wl->escan_info.wiphy = wiphy;
	wl->escan_info.escan_state = WL_ESCAN_STATE_SCANING;
	passive_scan = wl->active_scan ? 0 : 1;
=======
	cfg->escan_info.ndev = ndev;
	cfg->escan_info.wiphy = wiphy;
	cfg->escan_info.escan_state = WL_ESCAN_STATE_SCANING;
	passive_scan = cfg->active_scan ? 0 : 1;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	err = wldev_ioctl(ndev, WLC_SET_PASSIVE_SCAN,
		&passive_scan, sizeof(passive_scan), true);
	if (unlikely(err)) {
		WL_ERR(("error (%d)\n", err));
		goto exit;
	}

<<<<<<< HEAD
	err = wl_run_escan(wl, ndev, request, WL_SCAN_ACTION_START);
exit:
	mutex_unlock(&wl->usr_sync);
=======
	err = wl_run_escan(cfg, ndev, request, WL_SCAN_ACTION_START);
exit:
	mutex_unlock(&cfg->usr_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}

static s32
__wl_cfg80211_scan(struct wiphy *wiphy, struct net_device *ndev,
	struct cfg80211_scan_request *request,
	struct cfg80211_ssid *this_ssid)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct cfg80211_ssid *ssids;
	struct wl_scan_req *sr = wl_to_sr(wl);
	struct ether_addr primary_mac;
	s32 passive_scan;
	bool iscan_req;
	bool escan_req = false;
	bool p2p_ssid;
#ifdef WL11U
	bcm_tlv_t *interworking_ie;
	u32 ie_len;
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct cfg80211_ssid *ssids;
	struct ether_addr primary_mac;
	bool p2p_ssid;
#ifdef WL11U
	bcm_tlv_t *interworking_ie;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#endif
	s32 err = 0;
	s32 bssidx = -1;
	s32 i;

	unsigned long flags;
	static s32 busy_count = 0;
<<<<<<< HEAD

	/* If scan req comes for p2p0, send it over primary I/F
	 * Scan results will be delivered corresponding to cfg80211_scan_request
	 */
	if (ndev == wl->p2p_net) {
		ndev = wl_to_prmry_ndev(wl);
	}

	if (WL_DRV_STATUS_SENDING_AF_FRM_EXT(wl)) {
=======
#ifdef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
	struct net_device *remain_on_channel_ndev = NULL;
#endif

	dhd_pub_t *dhd;

	dhd = (dhd_pub_t *)(cfg->pub);
	if (dhd->op_mode & DHD_FLAG_HOSTAP_MODE) {
		WL_ERR(("Invalid Scan Command at SoftAP mode\n"));
		return -EINVAL;
	}

	ndev = ndev_to_wlc_ndev(ndev, cfg);

	if (WL_DRV_STATUS_SENDING_AF_FRM_EXT(cfg)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("Sending Action Frames. Try it again.\n"));
		return -EAGAIN;
	}

	WL_DBG(("Enter wiphy (%p)\n", wiphy));
<<<<<<< HEAD
	if (wl_get_drv_status_all(wl, SCANNING)) {
		if (wl->scan_request == NULL) {
			wl_clr_drv_status_all(wl, SCANNING);
=======
	if (wl_get_drv_status_all(cfg, SCANNING)) {
		if (cfg->scan_request == NULL) {
			wl_clr_drv_status_all(cfg, SCANNING);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_DBG(("<<<<<<<<<<<Force Clear Scanning Status>>>>>>>>>>>\n"));
		} else {
			WL_ERR(("Scanning already\n"));
			return -EAGAIN;
		}
	}
<<<<<<< HEAD
	if (wl_get_drv_status(wl, SCAN_ABORTING, ndev)) {
=======
	if (wl_get_drv_status(cfg, SCAN_ABORTING, ndev)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("Scanning being aborted\n"));
		return -EAGAIN;
	}
	if (request && request->n_ssids > WL_SCAN_PARAMS_SSID_MAX) {
		WL_ERR(("request null or n_ssids > WL_SCAN_PARAMS_SSID_MAX\n"));
		return -EOPNOTSUPP;
	}
#ifdef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
<<<<<<< HEAD
	if (wl_get_drv_status_all(wl, REMAINING_ON_CHANNEL)) {
		WL_DBG(("Remain_on_channel bit is set, somehow it didn't get cleared\n"));
		wl_notify_escan_complete(wl, ndev, true, true);
=======
	remain_on_channel_ndev = wl_cfg80211_get_remain_on_channel_ndev(cfg);
	if (remain_on_channel_ndev) {
		WL_DBG(("Remain_on_channel bit is set, somehow it didn't get cleared\n"));
		wl_notify_escan_complete(cfg, remain_on_channel_ndev, true, true);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
#endif /* WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */


	/* Arm scan timeout timer */
<<<<<<< HEAD
	mod_timer(&wl->scan_timeout, jiffies + msecs_to_jiffies(WL_SCAN_TIMER_INTERVAL_MS));
	iscan_req = false;
	if (request) {		/* scan bss */
		ssids = request->ssids;
		if (wl->iscan_on && (!ssids || !ssids->ssid_len || request->n_ssids != 1)) {
			iscan_req = true;
		} else if (wl->escan_on) {
			escan_req = true;
			p2p_ssid = false;
			for (i = 0; i < request->n_ssids; i++) {
				if (ssids[i].ssid_len &&
					IS_P2P_SSID(ssids[i].ssid, ssids[i].ssid_len)) {
					p2p_ssid = true;
					break;
				}
			}
			if (p2p_ssid) {
				if (wl->p2p_supported) {
					/* p2p scan trigger */
					if (p2p_on(wl) == false) {
						/* p2p on at the first time */
						p2p_on(wl) = true;
						wl_cfgp2p_set_firm_p2p(wl);
						get_primary_mac(wl, &primary_mac);
						wl_cfgp2p_generate_bss_mac(&primary_mac,
							&wl->p2p->dev_addr, &wl->p2p->int_addr);
					}
					wl_clr_p2p_status(wl, GO_NEG_PHASE);
					WL_DBG(("P2P: GO_NEG_PHASE status cleared \n"));
					p2p_scan(wl) = true;
				}
			} else {
				/* legacy scan trigger
				 * So, we have to disable p2p discovery if p2p discovery is on
				 */
				if (wl->p2p_supported) {
					p2p_scan(wl) = false;
					/* If Netdevice is not equals to primary and p2p is on
					*  , we will do p2p scan using P2PAPI_BSSCFG_DEVICE.
					*/

					if (p2p_scan(wl) == false) {
						if (wl_get_p2p_status(wl, DISCOVERY_ON)) {
							err = wl_cfgp2p_discover_enable_search(wl,
							false);
							if (unlikely(err)) {
								goto scan_out;
							}

						}
					}
				}
				if (!wl->p2p_supported || !p2p_scan(wl)) {
					bssidx = wl_cfgp2p_find_idx(wl, ndev);

#ifdef WL11U
					if ((interworking_ie = wl_cfg80211_find_interworking_ie(
						(u8 *)request->ie, request->ie_len)) != NULL) {
						ie_len = interworking_ie->len;

						err = wl_cfg80211_add_iw_ie(wl, ndev, bssidx,
						       VNDR_IE_CUSTOM_FLAG, interworking_ie->id,
						       interworking_ie->data, interworking_ie->len);

						if (unlikely(err)) {
							goto scan_out;
						}
					} else if (wl->iw_ie_len != 0) {
					/* we have to clear IW IE and disable gratuitous APR */
						wl_cfg80211_add_iw_ie(wl, ndev, bssidx,
							VNDR_IE_CUSTOM_FLAG,
							DOT11_MNG_INTERWORKING_ID,
							0, 0);

						wldev_iovar_setint_bsscfg(ndev, "grat_arp", 0,
							bssidx);
						/* we don't care about error */
					}
#endif /* WL11U */
					err = wl_cfgp2p_set_management_ie(wl, ndev, bssidx,
						VNDR_IE_PRBREQ_FLAG, (u8 *)request->ie,
						request->ie_len);
=======
	mod_timer(&cfg->scan_timeout, jiffies + msecs_to_jiffies(WL_SCAN_TIMER_INTERVAL_MS));
	if (request) {		/* scan bss */
		ssids = request->ssids;
		p2p_ssid = false;
		for (i = 0; i < request->n_ssids; i++) {
			if (ssids[i].ssid_len &&
				IS_P2P_SSID(ssids[i].ssid, ssids[i].ssid_len)) {
				p2p_ssid = true;
				break;
			}
		}
		if (p2p_ssid) {
			if (cfg->p2p_supported) {
				/* p2p scan trigger */
				if (p2p_on(cfg) == false) {
					/* p2p on at the first time */
					p2p_on(cfg) = true;
					wl_cfgp2p_set_firm_p2p(cfg);
					get_primary_mac(cfg, &primary_mac);
					wl_cfgp2p_generate_bss_mac(&primary_mac,
						&cfg->p2p->dev_addr, &cfg->p2p->int_addr);
				}
				wl_clr_p2p_status(cfg, GO_NEG_PHASE);
				WL_DBG(("P2P: GO_NEG_PHASE status cleared \n"));
				p2p_scan(cfg) = true;
			}
		} else {
			/* legacy scan trigger
			 * So, we have to disable p2p discovery if p2p discovery is on
			 */
			if (cfg->p2p_supported) {
				p2p_scan(cfg) = false;
				/* If Netdevice is not equals to primary and p2p is on
				*  , we will do p2p scan using P2PAPI_BSSCFG_DEVICE.
				*/

				if (p2p_scan(cfg) == false) {
					if (wl_get_p2p_status(cfg, DISCOVERY_ON)) {
						err = wl_cfgp2p_discover_enable_search(cfg,
						false);
						if (unlikely(err)) {
							goto scan_out;
						}

					}
				}
			}
			if (!cfg->p2p_supported || !p2p_scan(cfg)) {

				if (wl_cfgp2p_find_idx(cfg, ndev, &bssidx) != BCME_OK) {
					WL_ERR(("Find p2p index from ndev(%p) failed\n",
						ndev));
					err = BCME_ERROR;
					goto scan_out;
				}
#ifdef WL11U
				if ((interworking_ie = wl_cfg80211_find_interworking_ie(
					(u8 *)request->ie, request->ie_len)) != NULL) {
					err = wl_cfg80211_add_iw_ie(cfg, ndev, bssidx,
					       VNDR_IE_CUSTOM_FLAG, interworking_ie->id,
					       interworking_ie->data, interworking_ie->len);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

					if (unlikely(err)) {
						goto scan_out;
					}
<<<<<<< HEAD

				}
			}
		}
	} else {		/* scan in ibss */
		/* we don't do iscan in ibss */
		ssids = this_ssid;
	}
	wl->scan_request = request;
	wl_set_drv_status(wl, SCANNING, ndev);
	if (iscan_req) {
		err = wl_do_iscan(wl, request);
		if (likely(!err))
			goto scan_success;
		else
			goto scan_out;
	} else if (escan_req) {
		if (wl->p2p_supported) {
			if (p2p_on(wl) && p2p_scan(wl)) {

				/* find my listen channel */
				wl->afx_hdl->my_listen_chan =
					wl_find_listen_channel(wl, (u8 *)request->ie,
					request->ie_len);
				err = wl_cfgp2p_enable_discovery(wl, ndev,
					request->ie, request->ie_len);
=======
				} else if (cfg->iw_ie_len != 0) {
				/* we have to clear IW IE and disable gratuitous APR */
					wl_cfg80211_add_iw_ie(cfg, ndev, bssidx,
						VNDR_IE_CUSTOM_FLAG,
						DOT11_MNG_INTERWORKING_ID,
						0, 0);

					wldev_iovar_setint_bsscfg(ndev, "grat_arp", 0,
						bssidx);
					cfg->wl11u = FALSE;
					/* we don't care about error */
				}
#endif /* WL11U */
				err = wl_cfgp2p_set_management_ie(cfg, ndev, bssidx,
					VNDR_IE_PRBREQ_FLAG, (u8 *)request->ie,
					request->ie_len);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

				if (unlikely(err)) {
					goto scan_out;
				}
<<<<<<< HEAD
			}
		}
		err = wl_do_escan(wl, wiphy, ndev, request);
		if (likely(!err))
			goto scan_success;
		else
			goto scan_out;


	} else {
		memset(&sr->ssid, 0, sizeof(sr->ssid));
		sr->ssid.SSID_len =
			min_t(u8, sizeof(sr->ssid.SSID), ssids->ssid_len);
		if (sr->ssid.SSID_len) {
			memcpy(sr->ssid.SSID, ssids->ssid, sr->ssid.SSID_len);
			sr->ssid.SSID_len = htod32(sr->ssid.SSID_len);
			WL_SCAN(("Specific scan ssid=\"%s\" len=%d\n",
				sr->ssid.SSID, sr->ssid.SSID_len));
		} else {
			WL_SCAN(("Broadcast scan\n"));
		}
		WL_SCAN(("sr->ssid.SSID_len (%d)\n", sr->ssid.SSID_len));
		passive_scan = wl->active_scan ? 0 : 1;
		err = wldev_ioctl(ndev, WLC_SET_PASSIVE_SCAN,
			&passive_scan, sizeof(passive_scan), true);
		if (unlikely(err)) {
			WL_SCAN(("WLC_SET_PASSIVE_SCAN error (%d)\n", err));
			goto scan_out;
		}
		err = wldev_ioctl(ndev, WLC_SCAN, &sr->ssid,
			sizeof(sr->ssid), false);
		if (err) {
			if (err == -EBUSY) {
				WL_ERR(("system busy : scan for \"%s\" "
					"canceled\n", sr->ssid.SSID));
			} else {
				WL_ERR(("WLC_SCAN error (%d)\n", err));
			}
			goto scan_out;
		}
	}

scan_success:

=======

			}
		}
	} else {		/* scan in ibss */
		ssids = this_ssid;
	}

	if (request && !p2p_scan(cfg)) {
		WL_TRACE_HW4(("START SCAN\n"));
	}

	cfg->scan_request = request;
	wl_set_drv_status(cfg, SCANNING, ndev);

	if (cfg->p2p_supported) {
		if (p2p_on(cfg) && p2p_scan(cfg)) {

			/* find my listen channel */
			cfg->afx_hdl->my_listen_chan =
				wl_find_listen_channel(cfg, request->ie,
				request->ie_len);
			err = wl_cfgp2p_enable_discovery(cfg, ndev,
			request->ie, request->ie_len);

			if (unlikely(err)) {
				goto scan_out;
			}
		}
	}
	err = wl_do_escan(cfg, wiphy, ndev, request);
	if (likely(!err))
		goto scan_success;
	else
		goto scan_out;

scan_success:
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	busy_count = 0;

	return 0;

scan_out:
<<<<<<< HEAD

=======
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (err == BCME_BUSY || err == BCME_NOTREADY) {
		WL_ERR(("Scan err = (%d), busy?%d", err, -EBUSY));
		err = -EBUSY;
	}

#define SCAN_EBUSY_RETRY_LIMIT 10
	if (err == -EBUSY) {
		if (busy_count++ > SCAN_EBUSY_RETRY_LIMIT) {
			struct ether_addr bssid;
			s32 ret = 0;
			busy_count = 0;
			WL_ERR(("Unusual continuous EBUSY error, %d %d %d %d %d %d %d %d %d\n",
<<<<<<< HEAD
				wl_get_drv_status(wl, SCANNING, ndev),
				wl_get_drv_status(wl, SCAN_ABORTING, ndev),
				wl_get_drv_status(wl, CONNECTING, ndev),
				wl_get_drv_status(wl, CONNECTED, ndev),
				wl_get_drv_status(wl, DISCONNECTING, ndev),
				wl_get_drv_status(wl, AP_CREATING, ndev),
				wl_get_drv_status(wl, AP_CREATED, ndev),
				wl_get_drv_status(wl, SENDING_ACT_FRM, ndev),
				wl_get_drv_status(wl, SENDING_ACT_FRM, ndev)));
=======
				wl_get_drv_status(cfg, SCANNING, ndev),
				wl_get_drv_status(cfg, SCAN_ABORTING, ndev),
				wl_get_drv_status(cfg, CONNECTING, ndev),
				wl_get_drv_status(cfg, CONNECTED, ndev),
				wl_get_drv_status(cfg, DISCONNECTING, ndev),
				wl_get_drv_status(cfg, AP_CREATING, ndev),
				wl_get_drv_status(cfg, AP_CREATED, ndev),
				wl_get_drv_status(cfg, SENDING_ACT_FRM, ndev),
				wl_get_drv_status(cfg, SENDING_ACT_FRM, ndev)));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

			bzero(&bssid, sizeof(bssid));
			if ((ret = wldev_ioctl(ndev, WLC_GET_BSSID,
				&bssid, ETHER_ADDR_LEN, false)) == 0)
				WL_ERR(("FW is connected with " MACDBG "/n",
<<<<<<< HEAD
				MAC2STRDBG(bssid.octet)));
			else
				WL_ERR(("GET BSSID failed with %d\n", ret));

			wl_cfg80211_disconnect(wiphy, ndev, DOT11_RC_DISASSOC_LEAVING);
=======
					MAC2STRDBG(bssid.octet)));
			else
				WL_ERR(("GET BSSID failed with %d\n", ret));

			wl_cfg80211_scan_abort(cfg);

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	} else {
		busy_count = 0;
	}
<<<<<<< HEAD
	wl_clr_drv_status(wl, SCANNING, ndev);
	if (timer_pending(&wl->scan_timeout))
		del_timer_sync(&wl->scan_timeout);
	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
	wl->scan_request = NULL;
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);
=======

	wl_clr_drv_status(cfg, SCANNING, ndev);
	if (timer_pending(&cfg->scan_timeout))
		del_timer_sync(&cfg->scan_timeout);
	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
	cfg->scan_request = NULL;
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
static s32
wl_cfg80211_scan(struct wiphy *wiphy, struct net_device *ndev,
	struct cfg80211_scan_request *request)
{
	s32 err = 0;
	struct wl_priv *wl = wiphy_priv(wiphy);

	WL_DBG(("Enter \n"));
	CHECK_SYS_UP(wl);

	err = __wl_cfg80211_scan(wiphy, ndev, request, NULL);
	if (unlikely(err)) {
		if ((err == BCME_EPERM) && wl->scan_suppressed)
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
static s32
wl_cfg80211_scan(struct wiphy *wiphy, struct cfg80211_scan_request *request)
#else
static s32
wl_cfg80211_scan(struct wiphy *wiphy, struct net_device *ndev,
	struct cfg80211_scan_request *request)
#endif /* WL_CFG80211_P2P_DEV_IF */
{
	s32 err = 0;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
#if defined(WL_CFG80211_P2P_DEV_IF)
	struct net_device *ndev = wdev_to_wlc_ndev(request->wdev, cfg);
#endif /* WL_CFG80211_P2P_DEV_IF */

	WL_DBG(("Enter \n"));
	RETURN_EIO_IF_NOT_UP(cfg);

	err = __wl_cfg80211_scan(wiphy, ndev, request, NULL);
	if (unlikely(err)) {
		if ((err == BCME_EPERM) && cfg->scan_suppressed)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_DBG(("scan not permitted at this time (%d)\n", err));
		else
			WL_ERR(("scan error (%d)\n", err));
		return err;
	}

	return err;
}

static s32 wl_set_rts(struct net_device *dev, u32 rts_threshold)
{
	s32 err = 0;

	err = wldev_iovar_setint(dev, "rtsthresh", rts_threshold);
	if (unlikely(err)) {
		WL_ERR(("Error (%d)\n", err));
		return err;
	}
	return err;
}

static s32 wl_set_frag(struct net_device *dev, u32 frag_threshold)
{
	s32 err = 0;

	err = wldev_iovar_setint_bsscfg(dev, "fragthresh", frag_threshold, 0);
	if (unlikely(err)) {
		WL_ERR(("Error (%d)\n", err));
		return err;
	}
	return err;
}

static s32 wl_set_retry(struct net_device *dev, u32 retry, bool l)
{
	s32 err = 0;
	u32 cmd = (l ? WLC_SET_LRL : WLC_SET_SRL);

	retry = htod32(retry);
	err = wldev_ioctl(dev, cmd, &retry, sizeof(retry), true);
	if (unlikely(err)) {
		WL_ERR(("cmd (%d) , error (%d)\n", cmd, err));
		return err;
	}
	return err;
}

static s32 wl_cfg80211_set_wiphy_params(struct wiphy *wiphy, u32 changed)
{
<<<<<<< HEAD
	struct wl_priv *wl = (struct wl_priv *)wiphy_priv(wiphy);
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	s32 err = 0;

	CHECK_SYS_UP(wl);
	WL_DBG(("Enter\n"));
	if (changed & WIPHY_PARAM_RTS_THRESHOLD &&
		(wl->conf->rts_threshold != wiphy->rts_threshold)) {
		wl->conf->rts_threshold = wiphy->rts_threshold;
		err = wl_set_rts(ndev, wl->conf->rts_threshold);
=======
	struct bcm_cfg80211 *cfg = (struct bcm_cfg80211 *)wiphy_priv(wiphy);
	struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);
	s32 err = 0;

	RETURN_EIO_IF_NOT_UP(cfg);
	WL_DBG(("Enter\n"));
	if (changed & WIPHY_PARAM_RTS_THRESHOLD &&
		(cfg->conf->rts_threshold != wiphy->rts_threshold)) {
		cfg->conf->rts_threshold = wiphy->rts_threshold;
		err = wl_set_rts(ndev, cfg->conf->rts_threshold);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (!err)
			return err;
	}
	if (changed & WIPHY_PARAM_FRAG_THRESHOLD &&
<<<<<<< HEAD
		(wl->conf->frag_threshold != wiphy->frag_threshold)) {
		wl->conf->frag_threshold = wiphy->frag_threshold;
		err = wl_set_frag(ndev, wl->conf->frag_threshold);
=======
		(cfg->conf->frag_threshold != wiphy->frag_threshold)) {
		cfg->conf->frag_threshold = wiphy->frag_threshold;
		err = wl_set_frag(ndev, cfg->conf->frag_threshold);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (!err)
			return err;
	}
	if (changed & WIPHY_PARAM_RETRY_LONG &&
<<<<<<< HEAD
		(wl->conf->retry_long != wiphy->retry_long)) {
		wl->conf->retry_long = wiphy->retry_long;
		err = wl_set_retry(ndev, wl->conf->retry_long, true);
=======
		(cfg->conf->retry_long != wiphy->retry_long)) {
		cfg->conf->retry_long = wiphy->retry_long;
		err = wl_set_retry(ndev, cfg->conf->retry_long, true);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (!err)
			return err;
	}
	if (changed & WIPHY_PARAM_RETRY_SHORT &&
<<<<<<< HEAD
		(wl->conf->retry_short != wiphy->retry_short)) {
		wl->conf->retry_short = wiphy->retry_short;
		err = wl_set_retry(ndev, wl->conf->retry_short, false);
=======
		(cfg->conf->retry_short != wiphy->retry_short)) {
		cfg->conf->retry_short = wiphy->retry_short;
		err = wl_set_retry(ndev, cfg->conf->retry_short, false);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (!err) {
			return err;
		}
	}

	return err;
}
<<<<<<< HEAD
=======
static chanspec_t
channel_to_chanspec(struct wiphy *wiphy, struct net_device *dev, u32 channel, u32 bw_cap)
{
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	u8 *buf = NULL;
	wl_uint32_list_t *list;
	int err = BCME_OK;
	chanspec_t c = 0, ret_c = 0;
	int bw = 0, tmp_bw = 0;
	int i;
	u32 tmp_c, sb;
	u16 kflags = in_atomic() ? GFP_ATOMIC : GFP_KERNEL;
#define LOCAL_BUF_SIZE	1024
	buf = (u8 *) kzalloc(LOCAL_BUF_SIZE, kflags);
	if (!buf) {
		WL_ERR(("buf memory alloc failed\n"));
		goto exit;
	}
	list = (wl_uint32_list_t *)(void *)buf;
	list->count = htod32(WL_NUMCHANSPECS);
	err = wldev_iovar_getbuf_bsscfg(dev, "chanspecs", NULL,
		0, buf, LOCAL_BUF_SIZE, 0, &cfg->ioctl_buf_sync);
	if (err != BCME_OK) {
		WL_ERR(("get chanspecs failed with %d\n", err));
		goto exit;
	}
	for (i = 0; i < dtoh32(list->count); i++) {
		c = dtoh32(list->element[i]);
		if (channel <= CH_MAX_2G_CHANNEL) {
			if (!CHSPEC_IS20(c))
				continue;
			if (channel == CHSPEC_CHANNEL(c)) {
				ret_c = c;
				bw = 20;
				goto exit;
			}
		}
		if (CHSPEC_IS20(c)) {
			tmp_c = CHSPEC_CHANNEL(c);
			tmp_bw = WLC_BW_CAP_20MHZ;
		}
		else if (CHSPEC_IS40(c)) {
			tmp_c = CHSPEC_CHANNEL(c);
			if (CHSPEC_SB_UPPER(c)) {
				tmp_c += CH_10MHZ_APART;
			} else {
				tmp_c -= CH_10MHZ_APART;
			}
			tmp_bw = WLC_BW_CAP_40MHZ;
		}
		else {
			tmp_c = CHSPEC_CHANNEL(c);
			sb = c & WL_CHANSPEC_CTL_SB_MASK;
			if (sb == WL_CHANSPEC_CTL_SB_LL) {
				tmp_c -= (CH_10MHZ_APART + CH_20MHZ_APART);
			} else if (sb == WL_CHANSPEC_CTL_SB_LU) {
				tmp_c -= CH_10MHZ_APART;
			} else if (sb == WL_CHANSPEC_CTL_SB_UL) {
				tmp_c += CH_10MHZ_APART;
			} else {
				/* WL_CHANSPEC_CTL_SB_UU */
				tmp_c += (CH_10MHZ_APART + CH_20MHZ_APART);
			}
			tmp_bw = WLC_BW_CAP_80MHZ;
		}
		if (tmp_c != channel)
			continue;

		if ((tmp_bw > bw) && (tmp_bw <= bw_cap)) {
			bw = tmp_bw;
			ret_c = c;
			if (bw == bw_cap)
				goto exit;
		}
	}
exit:
	if (buf)
		kfree(buf);
#undef LOCAL_BUF_SIZE
	WL_INFO(("return chanspec %x %d\n", ret_c, bw));
	return ret_c;
}

void
wl_cfg80211_ibss_vsie_set_buffer(vndr_ie_setbuf_t *ibss_vsie, int ibss_vsie_len)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;

	if (cfg != NULL && ibss_vsie != NULL) {
		if (cfg->ibss_vsie != NULL) {
			kfree(cfg->ibss_vsie);
		}
		cfg->ibss_vsie = ibss_vsie;
		cfg->ibss_vsie_len = ibss_vsie_len;
	}
}

static void
wl_cfg80211_ibss_vsie_free(struct bcm_cfg80211 *cfg)
{
	/* free & initiralize VSIE (Vendor Specific IE) */
	if (cfg->ibss_vsie != NULL) {
		kfree(cfg->ibss_vsie);
		cfg->ibss_vsie = NULL;
		cfg->ibss_vsie_len = 0;
	}
}

s32
wl_cfg80211_ibss_vsie_delete(struct net_device *dev)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	char *ioctl_buf = NULL;
	s32 ret = BCME_OK;

	if (cfg != NULL && cfg->ibss_vsie != NULL) {
		ioctl_buf = kmalloc(WLC_IOCTL_MEDLEN, GFP_KERNEL);
		if (!ioctl_buf) {
			WL_ERR(("ioctl memory alloc failed\n"));
			return -ENOMEM;
		}

		/* change the command from "add" to "del" */
		strncpy(cfg->ibss_vsie->cmd, "del", VNDR_IE_CMD_LEN - 1);
		cfg->ibss_vsie->cmd[VNDR_IE_CMD_LEN - 1] = '\0';

		ret = wldev_iovar_setbuf(dev, "ie",
			cfg->ibss_vsie, cfg->ibss_vsie_len,
			ioctl_buf, WLC_IOCTL_MEDLEN, NULL);
		WL_ERR(("ret=%d\n", ret));

		if (ret == BCME_OK) {
			/* free & initiralize VSIE */
			kfree(cfg->ibss_vsie);
			cfg->ibss_vsie = NULL;
			cfg->ibss_vsie_len = 0;
		}

		if (ioctl_buf) {
			kfree(ioctl_buf);
		}
	}

	return ret;
}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

static s32
wl_cfg80211_join_ibss(struct wiphy *wiphy, struct net_device *dev,
	struct cfg80211_ibss_params *params)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct cfg80211_bss *bss;
	struct ieee80211_channel *chan;
	struct wl_join_params join_params;
	struct cfg80211_ssid ssid;
	s32 scan_retry = 0;
	s32 err = 0;
	bool rollback_lock = false;

	WL_TRACE(("In\n"));
	CHECK_SYS_UP(wl);
	if (params->bssid) {
		WL_ERR(("Invalid bssid\n"));
		return -EOPNOTSUPP;
	}
	bss = cfg80211_get_ibss(wiphy, NULL, params->ssid, params->ssid_len);
	if (!bss) {
		memcpy(ssid.ssid, params->ssid, params->ssid_len);
		ssid.ssid_len = params->ssid_len;
		do {
			if (unlikely
				(__wl_cfg80211_scan(wiphy, dev, NULL, &ssid) ==
				 -EBUSY)) {
				wl_delay(150);
			} else {
				break;
			}
		} while (++scan_retry < WL_SCAN_RETRY_MAX);
		/* to allow scan_inform to propagate to cfg80211 plane */
		if (rtnl_is_locked()) {
			rtnl_unlock();
			rollback_lock = true;
		}

		/* wait 4 secons till scan done.... */
		schedule_timeout_interruptible(msecs_to_jiffies(4000));
		if (rollback_lock)
			rtnl_lock();
		bss = cfg80211_get_ibss(wiphy, NULL,
			params->ssid, params->ssid_len);
	}
	if (bss) {
		wl->ibss_starter = false;
		WL_DBG(("Found IBSS\n"));
	} else {
		wl->ibss_starter = true;
	}
	chan = params->channel;
	if (chan)
		wl->channel = ieee80211_frequency_to_channel(chan->center_freq);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct cfg80211_bss *bss;
	struct ieee80211_channel *chan;
	struct wl_join_params join_params;
	int scan_suppress;
	struct cfg80211_ssid ssid;
	s32 scan_retry = 0;
	s32 err = 0;
	size_t join_params_size;
	chanspec_t chanspec = 0;
	u32 param[2] = {0, 0};
	u32 bw_cap = 0;
#ifdef WLAIBSS
	aibss_bcn_force_config_t bcn_config;
#endif /* WLAIBSS */

	WL_TRACE(("In\n"));
	RETURN_EIO_IF_NOT_UP(cfg);
	WL_INFO(("JOIN BSSID:" MACDBG "\n", MAC2STRDBG(params->bssid)));
	if (!params->ssid || params->ssid_len <= 0) {
		WL_ERR(("Invalid parameter\n"));
		return -EINVAL;
	}
#if defined(WL_CFG80211_P2P_DEV_IF)
	chan = params->chandef.chan;
#else
	chan = params->channel;
#endif /* WL_CFG80211_P2P_DEV_IF */
	if (chan)
		cfg->channel = ieee80211_frequency_to_channel(chan->center_freq);
	if (wl_get_drv_status(cfg, CONNECTED, dev)) {
		struct wlc_ssid *ssid = (struct wlc_ssid *)wl_read_prof(cfg, dev, WL_PROF_SSID);
		u8 *bssid = (u8 *)wl_read_prof(cfg, dev, WL_PROF_BSSID);
		u32 *channel = (u32 *)wl_read_prof(cfg, dev, WL_PROF_CHAN);
		if (!params->bssid || ((memcmp(params->bssid, bssid, ETHER_ADDR_LEN) == 0) &&
			(memcmp(params->ssid, ssid->SSID, ssid->SSID_len) == 0) &&
			(*channel == cfg->channel))) {
			WL_ERR(("Connection already existed to " MACDBG "\n",
				MAC2STRDBG((u8 *)wl_read_prof(cfg, dev, WL_PROF_BSSID))));
			return -EISCONN;
		}
		WL_ERR(("Ignore Previous connecton to %s (" MACDBG ")\n",
			ssid->SSID, MAC2STRDBG(bssid)));
	}

	/* remove the VSIE */
	wl_cfg80211_ibss_vsie_delete(dev);

	bss = cfg80211_get_ibss(wiphy, NULL, params->ssid, params->ssid_len);
	if (!bss) {
		if (IBSS_INITIAL_SCAN_ALLOWED == TRUE) {
			memcpy(ssid.ssid, params->ssid, params->ssid_len);
			ssid.ssid_len = params->ssid_len;
			do {
				if (unlikely
					(__wl_cfg80211_scan(wiphy, dev, NULL, &ssid) ==
					 -EBUSY)) {
					wl_delay(150);
				} else {
					break;
				}
			} while (++scan_retry < WL_SCAN_RETRY_MAX);

			/* rtnl lock code is removed here. don't see why rtnl lock
			 * needs to be released.
			 */

			/* wait 4 secons till scan done.... */
			schedule_timeout_interruptible(msecs_to_jiffies(4000));

			bss = cfg80211_get_ibss(wiphy, NULL,
				params->ssid, params->ssid_len);
		}
	}
	if (bss && ((IBSS_COALESCE_ALLOWED == TRUE) ||
		((IBSS_COALESCE_ALLOWED == FALSE) && params->bssid &&
		!memcmp(bss->bssid, params->bssid, ETHER_ADDR_LEN)))) {
		cfg->ibss_starter = false;
		WL_DBG(("Found IBSS\n"));
	} else {
		cfg->ibss_starter = true;
	}
	if (chan) {
		if (chan->band == IEEE80211_BAND_5GHZ)
			param[0] = WLC_BAND_5G;
		else if (chan->band == IEEE80211_BAND_2GHZ)
			param[0] = WLC_BAND_2G;
		err = wldev_iovar_getint(dev, "bw_cap", param);
		if (unlikely(err)) {
			WL_ERR(("Get bw_cap Failed (%d)\n", err));
			return err;
		}
		bw_cap = param[0];
		chanspec = channel_to_chanspec(wiphy, dev, cfg->channel, bw_cap);
	}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	/*
	 * Join with specific BSSID and cached SSID
	 * If SSID is zero join based on BSSID only
	 */
	memset(&join_params, 0, sizeof(join_params));
	memcpy((void *)join_params.ssid.SSID, (void *)params->ssid,
		params->ssid_len);
	join_params.ssid.SSID_len = htod32(params->ssid_len);
<<<<<<< HEAD
	if (params->bssid)
		memcpy(&join_params.params.bssid, params->bssid,
			ETHER_ADDR_LEN);
	else
		memset(&join_params.params.bssid, 0, ETHER_ADDR_LEN);

	err = wldev_ioctl(dev, WLC_SET_SSID, &join_params,
		sizeof(join_params), true);
	if (unlikely(err)) {
		WL_ERR(("Error (%d)\n", err));
		return err;
	}
=======
	if (params->bssid) {
		memcpy(&join_params.params.bssid, params->bssid, ETHER_ADDR_LEN);
		err = wldev_ioctl(dev, WLC_SET_DESIRED_BSSID, &join_params.params.bssid,
			ETHER_ADDR_LEN, true);
		if (unlikely(err)) {
			WL_ERR(("Error (%d)\n", err));
			return err;
		}
	} else
		memset(&join_params.params.bssid, 0, ETHER_ADDR_LEN);
	wldev_iovar_setint(dev, "ibss_coalesce_allowed", IBSS_COALESCE_ALLOWED);

	if (IBSS_INITIAL_SCAN_ALLOWED == FALSE) {
		scan_suppress = TRUE;
		/* Set the SCAN SUPPRESS Flag in the firmware to skip join scan */
		err = wldev_ioctl(dev, WLC_SET_SCANSUPPRESS,
			&scan_suppress, sizeof(int), true);
		if (unlikely(err)) {
			WL_ERR(("Scan Suppress Setting Failed (%d)\n", err));
			return err;
		}
	}

	join_params.params.chanspec_list[0] = chanspec;
	join_params.params.chanspec_num = 1;
	wldev_iovar_setint(dev, "chanspec", chanspec);
	join_params_size = sizeof(join_params);

	/* Disable Authentication, IBSS will add key if it required */
	wldev_iovar_setint(dev, "wpa_auth", WPA_AUTH_DISABLED);
	wldev_iovar_setint(dev, "wsec", 0);

#ifdef WLAIBSS
	/* Enable custom ibss features */
	err = wldev_iovar_setint(dev, "aibss", TRUE);

	if (unlikely(err)) {
		WL_ERR(("Enable custom IBSS mode failed (%d)\n", err));
		return err;
	}

	/* Configure custom IBSS beacon transmission */
	memset(&bcn_config, 0, sizeof(bcn_config));
	bcn_config.initial_min_bcn_dur = AIBSS_INITIAL_MIN_BCN_DUR;
	bcn_config.min_bcn_dur = AIBSS_MIN_BCN_DUR;
	bcn_config.bcn_flood_dur = AIBSS_BCN_FLOOD_DUR;
	bcn_config.version = AIBSS_BCN_FORCE_CONFIG_VER_0;
	bcn_config.len = sizeof(bcn_config);

	err = wldev_iovar_setbuf(dev, "aibss_bcn_force_config", &bcn_config,
		sizeof(bcn_config), cfg->ioctl_buf, WLC_IOCTL_MAXLEN, NULL);

	if (unlikely(err)) {
		WL_ERR(("Setting IBSS beacon configuration failed (%d)\n", err));
		return err;
	}
#endif /* WLAIBSS */

	err = wldev_ioctl(dev, WLC_SET_SSID, &join_params,
		join_params_size, true);
	if (unlikely(err)) {
		WL_ERR(("Error (%d)\n", err));
		return err;
	}

	if (IBSS_INITIAL_SCAN_ALLOWED == FALSE) {
		scan_suppress = FALSE;
		/* Reset the SCAN SUPPRESS Flag */
		err = wldev_ioctl(dev, WLC_SET_SCANSUPPRESS,
			&scan_suppress, sizeof(int), true);
		if (unlikely(err)) {
			WL_ERR(("Reset Scan Suppress Flag Failed (%d)\n", err));
			return err;
		}
	}
	wl_update_prof(cfg, dev, NULL, &join_params.ssid, WL_PROF_SSID);
	wl_update_prof(cfg, dev, NULL, &cfg->channel, WL_PROF_CHAN);
#ifdef WLAIBSS
	cfg->aibss_txfail_seq = 0;	/* initialize the sequence */
#endif /* WLAIBSS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}

static s32 wl_cfg80211_leave_ibss(struct wiphy *wiphy, struct net_device *dev)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	s32 err = 0;

	CHECK_SYS_UP(wl);
	wl_link_down(wl);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	s32 err = 0;
	scb_val_t scbval;
	u8 *curbssid;

	RETURN_EIO_IF_NOT_UP(cfg);
	wl_link_down(cfg);

	WL_ERR(("Leave IBSS\n"));
	curbssid = wl_read_prof(cfg, dev, WL_PROF_BSSID);
	wl_set_drv_status(cfg, DISCONNECTING, dev);
	scbval.val = 0;
	memcpy(&scbval.ea, curbssid, ETHER_ADDR_LEN);
	err = wldev_ioctl(dev, WLC_DISASSOC, &scbval,
		sizeof(scb_val_t), true);
	if (unlikely(err)) {
		wl_clr_drv_status(cfg, DISCONNECTING, dev);
		WL_ERR(("error(%d)\n", err));
		return err;
	}

	/* remove the VSIE */
	wl_cfg80211_ibss_vsie_delete(dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
static s32
wl_set_wpa_version(struct net_device *dev, struct cfg80211_connect_params *sme)
{
	struct wl_priv *wl = wlcfg_drv_priv;
	struct wl_security *sec;
	s32 val = 0;
	s32 err = 0;
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);

	if (sme->crypto.wpa_versions & NL80211_WPA_VERSION_1)
		val = WPA_AUTH_PSK |
			WPA_AUTH_UNSPECIFIED;
	else if (sme->crypto.wpa_versions & NL80211_WPA_VERSION_2)
		val = WPA2_AUTH_PSK|
=======
#ifdef MFP
static int wl_cfg80211_get_rsn_capa(bcm_tlv_t *wpa2ie, u8* capa)
{
	u16 suite_count;
	wpa_suite_mcast_t *mcast;
	wpa_suite_ucast_t *ucast;
	u16 len;
	wpa_suite_auth_key_mgmt_t *mgmt;

	if (!wpa2ie)
		return -1;

	len = wpa2ie->len;
	mcast = (wpa_suite_mcast_t *)&wpa2ie->data[WPA2_VERSION_LEN];
	if ((len -= WPA_SUITE_LEN) <= 0)
		return BCME_BADLEN;
	ucast = (wpa_suite_ucast_t *)&mcast[1];
	suite_count = ltoh16_ua(&ucast->count);
	if ((suite_count > NL80211_MAX_NR_CIPHER_SUITES) ||
		(len -= (WPA_IE_SUITE_COUNT_LEN +
		(WPA_SUITE_LEN * suite_count))) <= 0)
		return BCME_BADLEN;

	mgmt = (wpa_suite_auth_key_mgmt_t *)&ucast->list[suite_count];
	suite_count = ltoh16_ua(&mgmt->count);

	if ((suite_count > NL80211_MAX_NR_CIPHER_SUITES) ||
		(len -= (WPA_IE_SUITE_COUNT_LEN +
		(WPA_SUITE_LEN * suite_count))) >= RSN_CAP_LEN) {
		capa[0] = *(u8 *)&mgmt->list[suite_count];
		capa[1] = *((u8 *)&mgmt->list[suite_count] + 1);
	} else
		return BCME_BADLEN;

	return 0;
}
#endif /* MFP */

static s32
wl_set_wpa_version(struct net_device *dev, struct cfg80211_connect_params *sme)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	struct wl_security *sec;
	s32 val = 0;
	s32 err = 0;
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}

	if (sme->crypto.wpa_versions & NL80211_WPA_VERSION_1)
		val = WPA_AUTH_PSK |
#ifdef BCMCCX
			WPA_AUTH_CCKM |
#endif
			WPA_AUTH_UNSPECIFIED;
	else if (sme->crypto.wpa_versions & NL80211_WPA_VERSION_2)
		val = WPA2_AUTH_PSK|
#ifdef BCMCCX
			WPA2_AUTH_CCKM |
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WPA2_AUTH_UNSPECIFIED;
	else
		val = WPA_AUTH_DISABLED;

	if (is_wps_conn(sme))
		val = WPA_AUTH_DISABLED;

<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
	if (sme->crypto.wpa_versions & NL80211_WAPI_VERSION_1) {
		WL_DBG((" * wl_set_wpa_version, set wpa_auth"
			" to WPA_AUTH_WAPI 0x400"));
		val = WAPI_AUTH_PSK | WAPI_AUTH_UNSPECIFIED;
	}
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	WL_DBG(("setting wpa_auth to 0x%0x\n", val));
	err = wldev_iovar_setint_bsscfg(dev, "wpa_auth", val, bssidx);
	if (unlikely(err)) {
		WL_ERR(("set wpa_auth failed (%d)\n", err));
		return err;
	}
<<<<<<< HEAD
	sec = wl_read_prof(wl, dev, WL_PROF_SEC);
=======
	sec = wl_read_prof(cfg, dev, WL_PROF_SEC);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	sec->wpa_versions = sme->crypto.wpa_versions;
	return err;
}

<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
static s32
wl_set_set_wapi_ie(struct net_device *dev, struct cfg80211_connect_params *sme)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	s32 err = 0;
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}

	WL_DBG((" %s \n", __FUNCTION__));

	if (sme->crypto.wpa_versions & NL80211_WAPI_VERSION_1) {
		err = wldev_iovar_setbuf_bsscfg(dev, "wapiie", sme->ie, sme->ie_len,
			cfg->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);

		if (unlikely(err)) {
			WL_ERR(("===> set_wapi_ie Error (%d)\n", err));
			return err;
		}
	} else
		WL_DBG((" * skip \n"));
	return err;
}
#endif /* BCMWAPI_WPI */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

static s32
wl_set_auth_type(struct net_device *dev, struct cfg80211_connect_params *sme)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
	struct wl_security *sec;
	s32 val = 0;
	s32 err = 0;
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	struct wl_security *sec;
	s32 val = 0;
	s32 err = 0;
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	switch (sme->auth_type) {
	case NL80211_AUTHTYPE_OPEN_SYSTEM:
		val = WL_AUTH_OPEN_SYSTEM;
		WL_DBG(("open system\n"));
		break;
	case NL80211_AUTHTYPE_SHARED_KEY:
		val = WL_AUTH_SHARED_KEY;
		WL_DBG(("shared key\n"));
		break;
	case NL80211_AUTHTYPE_AUTOMATIC:
		val = WL_AUTH_OPEN_SHARED;
		WL_DBG(("automatic\n"));
		break;
<<<<<<< HEAD
	default:
		val = WL_AUTH_OPEN_SHARED;
=======
#ifdef BCMCCX
	case NL80211_AUTHTYPE_NETWORK_EAP:
		WL_DBG(("network eap\n"));
		val = DOT11_LEAP_AUTH;
		break;
#endif
	default:
		val = 2;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("invalid auth type (%d)\n", sme->auth_type));
		break;
	}

	err = wldev_iovar_setint_bsscfg(dev, "auth", val, bssidx);
	if (unlikely(err)) {
		WL_ERR(("set auth failed (%d)\n", err));
		return err;
	}
<<<<<<< HEAD
	sec = wl_read_prof(wl, dev, WL_PROF_SEC);
=======
	sec = wl_read_prof(cfg, dev, WL_PROF_SEC);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	sec->auth_type = sme->auth_type;
	return err;
}

static s32
wl_set_set_cipher(struct net_device *dev, struct cfg80211_connect_params *sme)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct wl_security *sec;
	s32 pval = 0;
	s32 gval = 0;
	s32 err = 0;
<<<<<<< HEAD
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);
=======
	s32 wsec_val = 0;
#ifdef MFP
	s32 mfp = 0;
	bcm_tlv_t *wpa2_ie;
	u8 rsn_cap[2];
#endif /* MFP */

#ifdef BCMWAPI_WPI
	s32 val = 0;
#endif
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (sme->crypto.n_ciphers_pairwise) {
		switch (sme->crypto.ciphers_pairwise[0]) {
		case WLAN_CIPHER_SUITE_WEP40:
		case WLAN_CIPHER_SUITE_WEP104:
			pval = WEP_ENABLED;
			break;
		case WLAN_CIPHER_SUITE_TKIP:
			pval = TKIP_ENABLED;
			break;
		case WLAN_CIPHER_SUITE_CCMP:
<<<<<<< HEAD
			pval = AES_ENABLED;
			break;
		case WLAN_CIPHER_SUITE_AES_CMAC:
			pval = AES_ENABLED;
			break;
=======
		case WLAN_CIPHER_SUITE_AES_CMAC:
			pval = AES_ENABLED;
			break;
#ifdef BCMWAPI_WPI
		case WLAN_CIPHER_SUITE_SMS4:
			val = SMS4_ENABLED;
			pval = SMS4_ENABLED;
			break;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		default:
			WL_ERR(("invalid cipher pairwise (%d)\n",
				sme->crypto.ciphers_pairwise[0]));
			return -EINVAL;
		}
	}
<<<<<<< HEAD
=======
#if defined(BCMSUP_4WAY_HANDSHAKE) && defined(WLAN_AKM_SUITE_FT_8021X)
	/* Ensure in-dongle supplicant is turned on when FBT wants to do the 4-way
	 * handshake.
	 * Note that the FW feature flag only exists on kernels that support the
	 * FT-EAP AKM suite.
	 */
	if (cfg->wdev->wiphy->features & NL80211_FEATURE_FW_4WAY_HANDSHAKE) {
		if (pval == AES_ENABLED)
			err = wldev_iovar_setint_bsscfg(dev, "sup_wpa", 1, bssidx);
		else
			err = wldev_iovar_setint_bsscfg(dev, "sup_wpa", 0, bssidx);

		if (err) {
			WL_ERR(("FBT: Error setting sup_wpa (%d)\n", err));
			return err;
		}
	}
#endif /* BCMSUP_4WAY_HANDSHAKE && WLAN_AKM_SUITE_FT_8021X */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (sme->crypto.cipher_group) {
		switch (sme->crypto.cipher_group) {
		case WLAN_CIPHER_SUITE_WEP40:
		case WLAN_CIPHER_SUITE_WEP104:
			gval = WEP_ENABLED;
			break;
		case WLAN_CIPHER_SUITE_TKIP:
			gval = TKIP_ENABLED;
			break;
		case WLAN_CIPHER_SUITE_CCMP:
			gval = AES_ENABLED;
			break;
		case WLAN_CIPHER_SUITE_AES_CMAC:
			gval = AES_ENABLED;
			break;
<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
		case WLAN_CIPHER_SUITE_SMS4:
			val = SMS4_ENABLED;
			gval = SMS4_ENABLED;
			break;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		default:
			WL_ERR(("invalid cipher group (%d)\n",
				sme->crypto.cipher_group));
			return -EINVAL;
		}
	}

	WL_DBG(("pval (%d) gval (%d)\n", pval, gval));

	if (is_wps_conn(sme)) {
		if (sme->privacy)
			err = wldev_iovar_setint_bsscfg(dev, "wsec", 4, bssidx);
		else
			/* WPS-2.0 allows no security */
			err = wldev_iovar_setint_bsscfg(dev, "wsec", 0, bssidx);
	} else {
<<<<<<< HEAD
			WL_DBG((" NO, is_wps_conn, Set pval | gval to WSEC"));
			err = wldev_iovar_setint_bsscfg(dev, "wsec",
				pval | gval, bssidx);
=======
#ifdef BCMWAPI_WPI
		if (sme->crypto.cipher_group == WLAN_CIPHER_SUITE_SMS4) {
			WL_DBG((" NO, is_wps_conn, WAPI set to SMS4_ENABLED"));
			err = wldev_iovar_setint_bsscfg(dev, "wsec", val, bssidx);
		} else {
#endif
			WL_DBG((" NO, is_wps_conn, Set pval | gval to WSEC"));
			wsec_val = pval | gval;

#ifdef MFP
			if (pval == AES_ENABLED) {
				if (((wpa2_ie = bcm_parse_tlvs((u8 *)sme->ie, sme->ie_len,
					DOT11_MNG_RSN_ID)) != NULL) &&
					(wl_cfg80211_get_rsn_capa(wpa2_ie, rsn_cap) == 0)) {

					if (rsn_cap[0] & RSN_CAP_MFPC) {
						/* MFP Capability advertised by supplicant. Check
						 * whether MFP is supported in the firmware
						 */
						if ((err = wldev_iovar_getint_bsscfg(dev,
								"mfp", &mfp, bssidx)) < 0) {
							WL_ERR(("Get MFP failed! "
								"Check MFP support in FW \n"));
							return -1;
						}

						if ((sme->crypto.n_akm_suites == 1) &&
							((sme->crypto.akm_suites[0] ==
							WL_AKM_SUITE_MFP_PSK) ||
							(sme->crypto.akm_suites[0] ==
							WL_AKM_SUITE_MFP_1X))) {
							wsec_val |= MFP_SHA256;
						} else if (sme->crypto.n_akm_suites > 1) {
							WL_ERR(("Multiple AKM Specified \n"));
							return -EINVAL;
						}

						wsec_val |= MFP_CAPABLE;
						if (rsn_cap[0] & RSN_CAP_MFPR)
							wsec_val |= MFP_REQUIRED;
					}
				}
			}
#endif /* MFP */
			WL_DBG((" Set WSEC to fW 0x%x \n", wsec_val));
			err = wldev_iovar_setint_bsscfg(dev, "wsec",
				wsec_val, bssidx);
#ifdef BCMWAPI_WPI
		}
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
	if (unlikely(err)) {
		WL_ERR(("error (%d)\n", err));
		return err;
	}

<<<<<<< HEAD
	sec = wl_read_prof(wl, dev, WL_PROF_SEC);
=======
	sec = wl_read_prof(cfg, dev, WL_PROF_SEC);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	sec->cipher_pairwise = sme->crypto.ciphers_pairwise[0];
	sec->cipher_group = sme->crypto.cipher_group;

	return err;
}

static s32
wl_set_key_mgmt(struct net_device *dev, struct cfg80211_connect_params *sme)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
	struct wl_security *sec;
	s32 val = 0;
	s32 err = 0;
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	struct wl_security *sec;
	s32 val = 0;
	s32 err = 0;
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (sme->crypto.n_akm_suites) {
		err = wldev_iovar_getint(dev, "wpa_auth", &val);
		if (unlikely(err)) {
			WL_ERR(("could not get wpa_auth (%d)\n", err));
			return err;
		}
		if (val & (WPA_AUTH_PSK |
<<<<<<< HEAD
=======
#ifdef BCMCCX
			WPA_AUTH_CCKM |
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WPA_AUTH_UNSPECIFIED)) {
			switch (sme->crypto.akm_suites[0]) {
			case WLAN_AKM_SUITE_8021X:
				val = WPA_AUTH_UNSPECIFIED;
				break;
			case WLAN_AKM_SUITE_PSK:
				val = WPA_AUTH_PSK;
				break;
<<<<<<< HEAD
=======
#ifdef BCMCCX
			case WLAN_AKM_SUITE_CCKM:
				val = WPA_AUTH_CCKM;
				break;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			default:
				WL_ERR(("invalid cipher group (%d)\n",
					sme->crypto.cipher_group));
				return -EINVAL;
			}
		} else if (val & (WPA2_AUTH_PSK |
<<<<<<< HEAD
=======
#ifdef BCMCCX
			WPA2_AUTH_CCKM |
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WPA2_AUTH_UNSPECIFIED)) {
			switch (sme->crypto.akm_suites[0]) {
			case WLAN_AKM_SUITE_8021X:
				val = WPA2_AUTH_UNSPECIFIED;
				break;
<<<<<<< HEAD
			case WLAN_AKM_SUITE_PSK:
				val = WPA2_AUTH_PSK;
				break;
=======
#ifdef MFP
			case WL_AKM_SUITE_MFP_1X:
				val = WPA2_AUTH_UNSPECIFIED;
				break;
			case WL_AKM_SUITE_MFP_PSK:
				val = WPA2_AUTH_PSK;
				break;
#endif
			case WLAN_AKM_SUITE_PSK:
				val = WPA2_AUTH_PSK;
				break;
#if defined(WLFBT) && defined(WLAN_AKM_SUITE_FT_8021X)
			case WLAN_AKM_SUITE_FT_8021X:
				val = WPA2_AUTH_UNSPECIFIED | WPA2_AUTH_FT;
				break;
#endif
#if defined(WLFBT) && defined(WLAN_AKM_SUITE_FT_PSK)
			case WLAN_AKM_SUITE_FT_PSK:
				val = WPA2_AUTH_PSK | WPA2_AUTH_FT;
				break;
#endif
#ifdef BCMCCX
			case WLAN_AKM_SUITE_CCKM:
				val = WPA2_AUTH_CCKM;
				break;
#endif
			default:
				WL_ERR(("invalid cipher group (%d)\n",
					sme->crypto.cipher_group));
				return -EINVAL;
			}
		}
#ifdef BCMWAPI_WPI
		else if (val & (WAPI_AUTH_PSK | WAPI_AUTH_UNSPECIFIED)) {
			switch (sme->crypto.akm_suites[0]) {
			case WLAN_AKM_SUITE_WAPI_CERT:
				val = WAPI_AUTH_UNSPECIFIED;
				break;
			case WLAN_AKM_SUITE_WAPI_PSK:
				val = WAPI_AUTH_PSK;
				break;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			default:
				WL_ERR(("invalid cipher group (%d)\n",
					sme->crypto.cipher_group));
				return -EINVAL;
			}
		}
<<<<<<< HEAD
=======
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_DBG(("setting wpa_auth to %d\n", val));

		err = wldev_iovar_setint_bsscfg(dev, "wpa_auth", val, bssidx);
		if (unlikely(err)) {
			WL_ERR(("could not set wpa_auth (%d)\n", err));
			return err;
		}
	}
<<<<<<< HEAD
	sec = wl_read_prof(wl, dev, WL_PROF_SEC);
=======
	sec = wl_read_prof(cfg, dev, WL_PROF_SEC);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	sec->wpa_auth = sme->crypto.akm_suites[0];

	return err;
}

static s32
wl_set_set_sharedkey(struct net_device *dev,
	struct cfg80211_connect_params *sme)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct wl_security *sec;
	struct wl_wsec_key key;
	s32 val;
	s32 err = 0;
<<<<<<< HEAD
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);

	WL_DBG(("key len (%d)\n", sme->key_len));
	if (sme->key_len) {
		sec = wl_read_prof(wl, dev, WL_PROF_SEC);
		WL_DBG(("wpa_versions 0x%x cipher_pairwise 0x%x\n",
			sec->wpa_versions, sec->cipher_pairwise));
		if (!(sec->wpa_versions & (NL80211_WPA_VERSION_1 |
			NL80211_WPA_VERSION_2)) &&
			(sec->cipher_pairwise & (WLAN_CIPHER_SUITE_WEP40 |
		WLAN_CIPHER_SUITE_WEP104)))
=======
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}

	WL_DBG(("key len (%d)\n", sme->key_len));
	if (sme->key_len) {
		sec = wl_read_prof(cfg, dev, WL_PROF_SEC);
		WL_DBG(("wpa_versions 0x%x cipher_pairwise 0x%x\n",
			sec->wpa_versions, sec->cipher_pairwise));
		if (!(sec->wpa_versions & (NL80211_WPA_VERSION_1 |
#ifdef BCMWAPI_WPI
			NL80211_WPA_VERSION_2 | NL80211_WAPI_VERSION_1)) &&
#else
			NL80211_WPA_VERSION_2)) &&
#endif
			(sec->cipher_pairwise & (WLAN_CIPHER_SUITE_WEP40 |
#ifdef BCMWAPI_WPI
		WLAN_CIPHER_SUITE_WEP104 | WLAN_CIPHER_SUITE_SMS4)))
#else
		WLAN_CIPHER_SUITE_WEP104)))
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		{
			memset(&key, 0, sizeof(key));
			key.len = (u32) sme->key_len;
			key.index = (u32) sme->key_idx;
			if (unlikely(key.len > sizeof(key.data))) {
				WL_ERR(("Too long key length (%u)\n", key.len));
				return -EINVAL;
			}
			memcpy(key.data, sme->key, key.len);
			key.flags = WL_PRIMARY_KEY;
			switch (sec->cipher_pairwise) {
			case WLAN_CIPHER_SUITE_WEP40:
				key.algo = CRYPTO_ALGO_WEP1;
				break;
			case WLAN_CIPHER_SUITE_WEP104:
				key.algo = CRYPTO_ALGO_WEP128;
				break;
<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
			case WLAN_CIPHER_SUITE_SMS4:
				key.algo = CRYPTO_ALGO_SMS4;
				break;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			default:
				WL_ERR(("Invalid algorithm (%d)\n",
					sme->crypto.ciphers_pairwise[0]));
				return -EINVAL;
			}
			/* Set the new key/index */
			WL_DBG(("key length (%d) key index (%d) algo (%d)\n",
				key.len, key.index, key.algo));
			WL_DBG(("key \"%s\"\n", key.data));
			swap_key_from_BE(&key);
			err = wldev_iovar_setbuf_bsscfg(dev, "wsec_key", &key, sizeof(key),
<<<<<<< HEAD
				wl->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &wl->ioctl_buf_sync);
=======
				cfg->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			if (unlikely(err)) {
				WL_ERR(("WLC_SET_KEY error (%d)\n", err));
				return err;
			}
			if (sec->auth_type == NL80211_AUTHTYPE_SHARED_KEY) {
				WL_DBG(("set auth_type to shared key\n"));
				val = WL_AUTH_SHARED_KEY;	/* shared key */
				err = wldev_iovar_setint_bsscfg(dev, "auth", val, bssidx);
				if (unlikely(err)) {
					WL_ERR(("set auth failed (%d)\n", err));
					return err;
				}
			}
		}
	}
	return err;
}

<<<<<<< HEAD
#ifdef ESCAN_RESULT_PATCH
static u8 connect_req_bssid[6];
static u8 broad_bssid[6];
#endif

=======
#if defined(ESCAN_RESULT_PATCH)
static u8 connect_req_bssid[6];
static u8 broad_bssid[6];
#endif /* ESCAN_RESULT_PATCH */


#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
#define MAX_ROAM_CACHE_NUM 100
#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */

#if defined(CUSTOM_SET_CPUCORE) || defined(CONFIG_TCPACK_FASTTX)
static bool wl_get_chan_isvht80(struct net_device *net, dhd_pub_t *dhd)
{
	u32 chanspec = 0;
	bool isvht80 = 0;

	if (wldev_iovar_getint(net, "chanspec", (s32 *)&chanspec) == BCME_OK)
		chanspec = wl_chspec_driver_to_host(chanspec);

	isvht80 = chanspec & WL_CHANSPEC_BW_80;
	WL_INFO(("%s: chanspec(%x:%d)\n", __FUNCTION__, chanspec, isvht80));

	return isvht80;
}
#endif /* CUSTOM_SET_CPUCORE || CONFIG_TCPACK_FASTTX */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

static s32
wl_cfg80211_connect(struct wiphy *wiphy, struct net_device *dev,
	struct cfg80211_connect_params *sme)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct ieee80211_channel *chan = sme->channel;
	wl_extjoin_params_t *ext_join_params;
	struct wl_join_params join_params;
	size_t join_params_size;
<<<<<<< HEAD
=======
#if defined(ROAM_ENABLE) && defined(ROAM_AP_ENV_DETECTION)
	dhd_pub_t *dhd =  (dhd_pub_t *)(cfg->pub);
	s32 roam_trigger[2] = {0, 0};
#endif /* ROAM_AP_ENV_DETECTION */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = 0;
	wpa_ie_fixed_t *wpa_ie;
	bcm_tlv_t *wpa2_ie;
	u8* wpaie  = 0;
	u32 wpaie_len = 0;
	u32 chan_cnt = 0;
	struct ether_addr bssid;
<<<<<<< HEAD
	int ret;
=======
	s32 bssidx;
#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
	chanspec_t chanspec_list[MAX_ROAM_CACHE_NUM];
#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */
	int ret;
	int wait_cnt;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	WL_DBG(("In\n"));

	if (unlikely(!sme->ssid)) {
		WL_ERR(("Invalid ssid\n"));
		return -EOPNOTSUPP;
	}

<<<<<<< HEAD
	CHECK_SYS_UP(wl);
=======
	if (unlikely(sme->ssid_len > DOT11_MAX_SSID_LEN)) {
		WL_ERR(("Invalid SSID info: SSID=%s, length=%zd\n",
			sme->ssid, sme->ssid_len));
		return -EINVAL;
	}

	RETURN_EIO_IF_NOT_UP(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	/*
	 * Cancel ongoing scan to sync up with sme state machine of cfg80211.
	 */
<<<<<<< HEAD
#if !defined(ESCAN_RESULT_PATCH)
	if (wl->scan_request) {
		wl_notify_escan_complete(wl, dev, true, true);
	}
#endif
#ifdef ESCAN_RESULT_PATCH
	if (sme->bssid) {
		memcpy(connect_req_bssid, sme->bssid, ETHER_ADDR_LEN);
	}
	else {
		bzero(connect_req_bssid, ETHER_ADDR_LEN);
	}
	bzero(broad_bssid, ETHER_ADDR_LEN);
#endif

	bzero(&bssid, sizeof(bssid));
	if (!wl_get_drv_status(wl, CONNECTED, dev)&&
		(ret = wldev_ioctl(dev, WLC_GET_BSSID, &bssid, ETHER_ADDR_LEN, false)) == 0) {
		if (!ETHER_ISNULLADDR(&bssid)) {
			scb_val_t scbval;
			wl_set_drv_status(wl, DISCONNECTING, dev);
=======
#if (defined(BCM4334_CHIP) || !defined(ESCAN_RESULT_PATCH))
	if (cfg->scan_request) {
		wl_notify_escan_complete(cfg, dev, true, true);
	}
#endif
#ifdef WL_SCHED_SCAN
	if (cfg->sched_scan_req) {
		wl_cfg80211_sched_scan_stop(wiphy, bcmcfg_to_prmry_ndev(cfg));
	}
#endif
#ifdef WL_CFG80211_GON_COLLISION
	/* init block gon req count  */
	cfg->block_gon_req_tx_count = 0;
	cfg->block_gon_req_rx_count = 0;
#endif /* WL_CFG80211_GON_COLLISION */
#if defined(ESCAN_RESULT_PATCH)
	if (sme->bssid)
		memcpy(connect_req_bssid, sme->bssid, ETHER_ADDR_LEN);
	else
		bzero(connect_req_bssid, ETHER_ADDR_LEN);
	bzero(broad_bssid, ETHER_ADDR_LEN);
#endif
#if defined(USE_DYNAMIC_MAXPKT_RXGLOM)
	maxrxpktglom = 0;
#endif
	bzero(&bssid, sizeof(bssid));
	if (!wl_get_drv_status(cfg, CONNECTED, dev)&&
		(ret = wldev_ioctl(dev, WLC_GET_BSSID, &bssid, ETHER_ADDR_LEN, false)) == 0) {
		if (!ETHER_ISNULLADDR(&bssid)) {
			scb_val_t scbval;
			wl_set_drv_status(cfg, DISCONNECTING, dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			scbval.val = DOT11_RC_DISASSOC_LEAVING;
			memcpy(&scbval.ea, &bssid, ETHER_ADDR_LEN);
			scbval.val = htod32(scbval.val);

			WL_DBG(("drv status CONNECTED is not set, but connected in FW!" MACDBG "/n",
				MAC2STRDBG(bssid.octet)));
			err = wldev_ioctl(dev, WLC_DISASSOC, &scbval,
				sizeof(scb_val_t), true);
			if (unlikely(err)) {
<<<<<<< HEAD
				wl_clr_drv_status(wl, DISCONNECTING, dev);
				WL_ERR(("error (%d)\n", err));
				return err;
			}
			while (wl_get_drv_status(wl, DISCONNECTING, dev)) {
				WL_ERR(("Waiting for disconnection terminated.\n"));
				msleep(20);
			}
		} else
			WL_DBG(("Currently not associated!\n"));
=======
				wl_clr_drv_status(cfg, DISCONNECTING, dev);
				WL_ERR(("error (%d)\n", err));
				return err;
			}
			wait_cnt = 500/10;
			while (wl_get_drv_status(cfg, DISCONNECTING, dev) && wait_cnt) {
				WL_DBG(("Waiting for disconnection terminated, wait_cnt: %d\n",
					wait_cnt));
				wait_cnt--;
				OSL_SLEEP(10);
			}
		} else
			WL_DBG(("Currently not associated!\n"));
	} else {
		/* if status is DISCONNECTING, wait for disconnection terminated max 500 ms */
		wait_cnt = 500/10;
		while (wl_get_drv_status(cfg, DISCONNECTING, dev) && wait_cnt) {
			WL_DBG(("Waiting for disconnection terminated, wait_cnt: %d\n", wait_cnt));
			wait_cnt--;
			OSL_SLEEP(10);
		}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

	/* Clean BSSID */
	bzero(&bssid, sizeof(bssid));
<<<<<<< HEAD
	if (!wl_get_drv_status(wl, DISCONNECTING, dev))
		wl_update_prof(wl, dev, NULL, (void *)&bssid, WL_PROF_BSSID);

	if (p2p_is_on(wl) && (dev != wl_to_prmry_ndev(wl))) {
		/* we only allow to connect using virtual interface in case of P2P */
			wl_cfgp2p_set_management_ie(wl, dev, wl_cfgp2p_find_idx(wl, dev),
				VNDR_IE_ASSOCREQ_FLAG, sme->ie, sme->ie_len);
	} else if (dev == wl_to_prmry_ndev(wl)) {
=======
	if (!wl_get_drv_status(cfg, DISCONNECTING, dev))
		wl_update_prof(cfg, dev, NULL, (void *)&bssid, WL_PROF_BSSID);

	if (p2p_is_on(cfg) && (dev != bcmcfg_to_prmry_ndev(cfg))) {
		/* we only allow to connect using virtual interface in case of P2P */
			if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
				WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
				return BCME_ERROR;
			}
			wl_cfgp2p_set_management_ie(cfg, dev, bssidx,
				VNDR_IE_ASSOCREQ_FLAG, sme->ie, sme->ie_len);
	} else if (dev == bcmcfg_to_prmry_ndev(cfg)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* find the RSN_IE */
		if ((wpa2_ie = bcm_parse_tlvs((u8 *)sme->ie, sme->ie_len,
			DOT11_MNG_RSN_ID)) != NULL) {
			WL_DBG((" WPA2 IE is found\n"));
		}
		/* find the WPA_IE */
		if ((wpa_ie = wl_cfgp2p_find_wpaie((u8 *)sme->ie,
			sme->ie_len)) != NULL) {
			WL_DBG((" WPA IE is found\n"));
		}
		if (wpa_ie != NULL || wpa2_ie != NULL) {
			wpaie = (wpa_ie != NULL) ? (u8 *)wpa_ie : (u8 *)wpa2_ie;
			wpaie_len = (wpa_ie != NULL) ? wpa_ie->length : wpa2_ie->len;
			wpaie_len += WPA_RSN_IE_TAG_FIXED_LEN;
			wldev_iovar_setbuf(dev, "wpaie", wpaie, wpaie_len,
<<<<<<< HEAD
				wl->ioctl_buf, WLC_IOCTL_MAXLEN, &wl->ioctl_buf_sync);
		} else {
			wldev_iovar_setbuf(dev, "wpaie", NULL, 0,
				wl->ioctl_buf, WLC_IOCTL_MAXLEN, &wl->ioctl_buf_sync);
		}

		err = wl_cfgp2p_set_management_ie(wl, dev, wl_cfgp2p_find_idx(wl, dev),
=======
				cfg->ioctl_buf, WLC_IOCTL_MAXLEN, &cfg->ioctl_buf_sync);
		} else {
			wldev_iovar_setbuf(dev, "wpaie", NULL, 0,
				cfg->ioctl_buf, WLC_IOCTL_MAXLEN, &cfg->ioctl_buf_sync);
		}

		if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
			WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
			return BCME_ERROR;
		}
		err = wl_cfgp2p_set_management_ie(cfg, dev, bssidx,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			VNDR_IE_ASSOCREQ_FLAG, (u8 *)sme->ie, sme->ie_len);
		if (unlikely(err)) {
			return err;
		}
	}
<<<<<<< HEAD
	if (chan) {
		wl->channel = ieee80211_frequency_to_channel(chan->center_freq);
		chan_cnt = 1;
		WL_DBG(("channel (%d), center_req (%d), %d channels\n", wl->channel,
			chan->center_freq, chan_cnt));
	} else
		wl->channel = 0;
=======
#if defined(ROAM_ENABLE) && defined(ROAM_AP_ENV_DETECTION)
	if (dhd->roam_env_detection) {
		bool is_roamtrig_reset = TRUE;
		bool is_roam_env_ok = (wldev_iovar_setint(dev, "roam_env_detection",
			AP_ENV_DETECT_NOT_USED) == BCME_OK);
#ifdef CUSTOMER_HW4
		roam_trigger[1] = WLC_BAND_2G;
		is_roamtrig_reset =
			(wldev_ioctl(dev, WLC_GET_ROAM_TRIGGER, roam_trigger,
			sizeof(roam_trigger), 0) == BCME_OK) &&
			(roam_trigger[0] == WL_AUTO_ROAM_TRIGGER-10);
#endif /* CUSTOMER_HW4 */
		if (is_roamtrig_reset && is_roam_env_ok) {
			roam_trigger[0] = WL_AUTO_ROAM_TRIGGER;
			roam_trigger[1] = WLC_BAND_ALL;
		err = wldev_ioctl(dev, WLC_SET_ROAM_TRIGGER, roam_trigger,
			sizeof(roam_trigger), true);
		if (unlikely(err)) {
				WL_ERR((" failed to restore roam_trigger for auto env"
					" detection\n"));
		}
	}
	}
#endif /* ROAM_ENABLE && ROAM_AP_ENV_DETECTION */
	if (chan) {
#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
		wlc_ssid_t ssid;
		int band;

		err = wldev_get_band(dev, &band);
		if (!err) {
			set_roam_band(band);
		}

		cfg->channel = ieee80211_frequency_to_channel(chan->center_freq);
		memcpy(ssid.SSID, sme->ssid, sme->ssid_len);
		ssid.SSID_len = sme->ssid_len;
		chan_cnt = get_roam_channel_list(cfg->channel, chanspec_list, &ssid, ioctl_version);
#else
		cfg->channel = ieee80211_frequency_to_channel(chan->center_freq);
		chan_cnt = 1;
#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */
		WL_DBG(("channel (%d), center_req (%d), %d channels\n", cfg->channel,
			chan->center_freq, chan_cnt));
	} else
		cfg->channel = 0;
#ifdef BCMWAPI_WPI
	WL_DBG(("1. enable wapi auth\n"));
	if (sme->crypto.wpa_versions & NL80211_WAPI_VERSION_1) {
		WL_DBG(("2. set wapi ie  \n"));
		err = wl_set_set_wapi_ie(dev, sme);
		if (unlikely(err))
			return err;
	} else
		WL_DBG(("2. Not wapi ie  \n"));
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	WL_DBG(("ie (%p), ie_len (%zd)\n", sme->ie, sme->ie_len));
	WL_DBG(("3. set wapi version \n"));
	err = wl_set_wpa_version(dev, sme);
	if (unlikely(err)) {
		WL_ERR(("Invalid wpa_version\n"));
		return err;
	}
<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
	if (sme->crypto.wpa_versions & NL80211_WAPI_VERSION_1)
		WL_DBG(("4. WAPI Dont Set wl_set_auth_type\n"));
	else {
		WL_DBG(("4. wl_set_auth_type\n"));
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		err = wl_set_auth_type(dev, sme);
		if (unlikely(err)) {
			WL_ERR(("Invalid auth type\n"));
			return err;
		}
<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
	}
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	err = wl_set_set_cipher(dev, sme);
	if (unlikely(err)) {
		WL_ERR(("Invalid ciper\n"));
		return err;
	}

	err = wl_set_key_mgmt(dev, sme);
	if (unlikely(err)) {
		WL_ERR(("Invalid key mgmt\n"));
		return err;
	}

	err = wl_set_set_sharedkey(dev, sme);
	if (unlikely(err)) {
		WL_ERR(("Invalid shared key\n"));
		return err;
	}

	/*
	 *  Join with specific BSSID and cached SSID
	 *  If SSID is zero join based on BSSID only
	 */
	join_params_size = WL_EXTJOIN_PARAMS_FIXED_SIZE +
		chan_cnt * sizeof(chanspec_t);
	ext_join_params =  (wl_extjoin_params_t*)kzalloc(join_params_size, GFP_KERNEL);
	if (ext_join_params == NULL) {
		err = -ENOMEM;
<<<<<<< HEAD
		wl_clr_drv_status(wl, CONNECTING, dev);
=======
		wl_clr_drv_status(cfg, CONNECTING, dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		goto exit;
	}
	ext_join_params->ssid.SSID_len = min(sizeof(ext_join_params->ssid.SSID), sme->ssid_len);
	memcpy(&ext_join_params->ssid.SSID, sme->ssid, ext_join_params->ssid.SSID_len);
<<<<<<< HEAD
	wl_update_prof(wl, dev, NULL, &ext_join_params->ssid, WL_PROF_SSID);
=======
	wl_update_prof(cfg, dev, NULL, &ext_join_params->ssid, WL_PROF_SSID);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	ext_join_params->ssid.SSID_len = htod32(ext_join_params->ssid.SSID_len);
	/* increate dwell time to receive probe response or detect Beacon
	* from target AP at a noisy air only during connect command
	*/
<<<<<<< HEAD
	ext_join_params->scan.active_time = WL_SCAN_JOIN_ACTIVE_DWELL_TIME_MS;
	ext_join_params->scan.passive_time = WL_SCAN_JOIN_PASSIVE_DWELL_TIME_MS;
	/* Set up join scan parameters */
	ext_join_params->scan.scan_type = -1;
	ext_join_params->scan.nprobes
		= (ext_join_params->scan.active_time/WL_SCAN_JOIN_PROBE_INTERVAL_MS);
=======
	ext_join_params->scan.active_time = chan_cnt ? WL_SCAN_JOIN_ACTIVE_DWELL_TIME_MS : -1;
	ext_join_params->scan.passive_time = chan_cnt ? WL_SCAN_JOIN_PASSIVE_DWELL_TIME_MS : -1;
	/* Set up join scan parameters */
	ext_join_params->scan.scan_type = -1;
	ext_join_params->scan.nprobes = chan_cnt ?
		(ext_join_params->scan.active_time/WL_SCAN_JOIN_PROBE_INTERVAL_MS) : -1;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	ext_join_params->scan.home_time = -1;

	if (sme->bssid)
		memcpy(&ext_join_params->assoc.bssid, sme->bssid, ETH_ALEN);
	else
		memcpy(&ext_join_params->assoc.bssid, &ether_bcast, ETH_ALEN);
	ext_join_params->assoc.chanspec_num = chan_cnt;
	if (chan_cnt) {
<<<<<<< HEAD
		u16 channel, band, bw, ctl_sb;
		chanspec_t chspec;
		channel = wl->channel;
=======
#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
		memcpy(ext_join_params->assoc.chanspec_list, chanspec_list,
			sizeof(chanspec_t) * chan_cnt);
#else
		u16 channel, band, bw, ctl_sb;
		chanspec_t chspec;
		channel = cfg->channel;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		band = (channel <= CH_MAX_2G_CHANNEL) ? WL_CHANSPEC_BAND_2G
			: WL_CHANSPEC_BAND_5G;
		bw = WL_CHANSPEC_BW_20;
		ctl_sb = WL_CHANSPEC_CTL_SB_NONE;
		chspec = (channel | band | bw | ctl_sb);
		ext_join_params->assoc.chanspec_list[0]  &= WL_CHANSPEC_CHAN_MASK;
		ext_join_params->assoc.chanspec_list[0] |= chspec;
		ext_join_params->assoc.chanspec_list[0] =
			wl_chspec_host_to_driver(ext_join_params->assoc.chanspec_list[0]);
<<<<<<< HEAD
=======
#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
	ext_join_params->assoc.chanspec_num = htod32(ext_join_params->assoc.chanspec_num);
	if (ext_join_params->ssid.SSID_len < IEEE80211_MAX_SSID_LEN) {
		WL_INFO(("ssid \"%s\", len (%d)\n", ext_join_params->ssid.SSID,
			ext_join_params->ssid.SSID_len));
	}
<<<<<<< HEAD
	wl_set_drv_status(wl, CONNECTING, dev);
	err = wldev_iovar_setbuf_bsscfg(dev, "join", ext_join_params, join_params_size,
		wl->ioctl_buf, WLC_IOCTL_MAXLEN, wl_cfgp2p_find_idx(wl, dev), &wl->ioctl_buf_sync);
	kfree(ext_join_params);
	if (err) {
		wl_clr_drv_status(wl, CONNECTING, dev);
		if (err == BCME_UNSUPPORTED) {
			WL_DBG(("join iovar is not supported\n"));
			goto set_ssid;
		} else
			WL_ERR(("error (%d)\n", err));
=======
	wl_set_drv_status(cfg, CONNECTING, dev);

	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
	err = wldev_iovar_setbuf_bsscfg(dev, "join", ext_join_params, join_params_size,
		cfg->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);

	WL_ERR(("Connectting with" MACDBG " channel (%d) ssid \"%s\", len (%d)\n\n",
		MAC2STRDBG((u8*)(&ext_join_params->assoc.bssid)), cfg->channel,
		ext_join_params->ssid.SSID, ext_join_params->ssid.SSID_len));

	kfree(ext_join_params);
	if (err) {
		wl_clr_drv_status(cfg, CONNECTING, dev);
		if (err == BCME_UNSUPPORTED) {
			WL_DBG(("join iovar is not supported\n"));
			goto set_ssid;
		} else {
			WL_ERR(("error (%d)\n", err));
			goto exit;
		}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	} else
		goto exit;

set_ssid:
	memset(&join_params, 0, sizeof(join_params));
	join_params_size = sizeof(join_params.ssid);

	join_params.ssid.SSID_len = min(sizeof(join_params.ssid.SSID), sme->ssid_len);
	memcpy(&join_params.ssid.SSID, sme->ssid, join_params.ssid.SSID_len);
	join_params.ssid.SSID_len = htod32(join_params.ssid.SSID_len);
<<<<<<< HEAD
	wl_update_prof(wl, dev, NULL, &join_params.ssid, WL_PROF_SSID);
=======
	wl_update_prof(cfg, dev, NULL, &join_params.ssid, WL_PROF_SSID);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (sme->bssid)
		memcpy(&join_params.params.bssid, sme->bssid, ETH_ALEN);
	else
		memcpy(&join_params.params.bssid, &ether_bcast, ETH_ALEN);

<<<<<<< HEAD
	wl_ch_to_chanspec(wl->channel, &join_params, &join_params_size);
	WL_DBG(("join_param_size %d\n", join_params_size));
=======
	wl_ch_to_chanspec(cfg->channel, &join_params, &join_params_size);
	WL_DBG(("join_param_size %zu\n", join_params_size));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (join_params.ssid.SSID_len < IEEE80211_MAX_SSID_LEN) {
		WL_INFO(("ssid \"%s\", len (%d)\n", join_params.ssid.SSID,
			join_params.ssid.SSID_len));
	}
<<<<<<< HEAD
	wl_set_drv_status(wl, CONNECTING, dev);
	err = wldev_ioctl(dev, WLC_SET_SSID, &join_params, join_params_size, true);
	if (err) {
		WL_ERR(("error (%d)\n", err));
		wl_clr_drv_status(wl, CONNECTING, dev);
=======
	wl_set_drv_status(cfg, CONNECTING, dev);
	err = wldev_ioctl(dev, WLC_SET_SSID, &join_params, join_params_size, true);
	if (err) {
		WL_ERR(("error (%d)\n", err));
		wl_clr_drv_status(cfg, CONNECTING, dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
exit:
	return err;
}

static s32
wl_cfg80211_disconnect(struct wiphy *wiphy, struct net_device *dev,
	u16 reason_code)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	scb_val_t scbval;
	bool act = false;
	s32 err = 0;
	u8 *curbssid;
<<<<<<< HEAD
	WL_ERR(("Reason %d\n", reason_code));
	CHECK_SYS_UP(wl);
	act = *(bool *) wl_read_prof(wl, dev, WL_PROF_ACT);
	curbssid = wl_read_prof(wl, dev, WL_PROF_BSSID);
=======
#ifdef CUSTOM_SET_CPUCORE
	dhd_pub_t *dhd = (dhd_pub_t *)(cfg->pub);
#endif /* CUSTOM_SET_CPUCORE */
	WL_ERR(("Reason %d\n", reason_code));
	RETURN_EIO_IF_NOT_UP(cfg);
	act = *(bool *) wl_read_prof(cfg, dev, WL_PROF_ACT);
	curbssid = wl_read_prof(cfg, dev, WL_PROF_BSSID);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (act) {
		/*
		* Cancel ongoing scan to sync up with sme state machine of cfg80211.
		*/
<<<<<<< HEAD
#if !defined(ESCAN_RESULT_PATCH)
		/* Let scan aborted by F/W */
		if (wl->scan_request) {
			wl_notify_escan_complete(wl, dev, true, true);
		}
#endif /* ESCAN_RESULT_PATCH */
		wl_set_drv_status(wl, DISCONNECTING, dev);
=======
#if (defined(BCM4334_CHIP) || !defined(ESCAN_RESULT_PATCH))
		/* Let scan aborted by F/W */
		if (cfg->scan_request) {
			wl_notify_escan_complete(cfg, dev, true, true);
		}
#endif /* ESCAN_RESULT_PATCH */
		wl_set_drv_status(cfg, DISCONNECTING, dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		scbval.val = reason_code;
		memcpy(&scbval.ea, curbssid, ETHER_ADDR_LEN);
		scbval.val = htod32(scbval.val);
		err = wldev_ioctl(dev, WLC_DISASSOC, &scbval,
			sizeof(scb_val_t), true);
		if (unlikely(err)) {
<<<<<<< HEAD
			wl_clr_drv_status(wl, DISCONNECTING, dev);
=======
			wl_clr_drv_status(cfg, DISCONNECTING, dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_ERR(("error (%d)\n", err));
			return err;
		}
	}
<<<<<<< HEAD
=======
#ifdef CUSTOM_SET_CPUCORE
	/* set default cpucore */
	if (dev == bcmcfg_to_prmry_ndev(cfg)) {
		dhd->chan_isvht80 &= ~DHD_FLAG_STA_MODE;
		if (!(dhd->chan_isvht80))
			dhd_set_cpucore(dhd, FALSE);
	}
#endif /* CUSTOM_SET_CPUCORE */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
static s32
wl_cfg80211_set_tx_power(struct wiphy *wiphy,
	enum nl80211_tx_power_setting type, s32 dbm)
{

	struct wl_priv *wl = wiphy_priv(wiphy);
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	u16 txpwrmw;
	s32 err = 0;
	s32 disable = 0;

	CHECK_SYS_UP(wl);
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
static s32
wl_cfg80211_set_tx_power(struct wiphy *wiphy, struct wireless_dev *wdev,
	enum nl80211_tx_power_setting type, s32 mbm)
#else
static s32
wl_cfg80211_set_tx_power(struct wiphy *wiphy,
	enum nl80211_tx_power_setting type, s32 dbm)
#endif /* WL_CFG80211_P2P_DEV_IF */
{

	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);
	s32 err = 0;
#if defined(WL_CFG80211_P2P_DEV_IF)
	s32 dbm = MBM_TO_DBM(mbm);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)) || \
	defined(WL_COMPAT_WIRELESS) || defined(WL_SUPPORT_BACKPORTED_KPATCHES)
	dbm = MBM_TO_DBM(dbm);
#endif /* WL_CFG80211_P2P_DEV_IF */

	RETURN_EIO_IF_NOT_UP(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	switch (type) {
	case NL80211_TX_POWER_AUTOMATIC:
		break;
	case NL80211_TX_POWER_LIMITED:
		if (dbm < 0) {
			WL_ERR(("TX_POWER_LIMITTED - dbm is negative\n"));
			return -EINVAL;
		}
		break;
	case NL80211_TX_POWER_FIXED:
		if (dbm < 0) {
			WL_ERR(("TX_POWER_FIXED - dbm is negative..\n"));
			return -EINVAL;
		}
		break;
	}
<<<<<<< HEAD
	/* Make sure radio is off or on as far as software is concerned */
	disable = WL_RADIO_SW_DISABLE << 16;
	disable = htod32(disable);
	err = wldev_ioctl(ndev, WLC_SET_RADIO, &disable, sizeof(disable), true);
	if (unlikely(err)) {
		WL_ERR(("WLC_SET_RADIO error (%d)\n", err));
		return err;
	}

	if (dbm > 0xffff)
		txpwrmw = 0xffff;
	else
		txpwrmw = (u16) dbm;
	err = wldev_iovar_setint(ndev, "qtxpower",
		(s32) (bcm_mw_to_qdbm(txpwrmw)));
	if (unlikely(err)) {
		WL_ERR(("qtxpower error (%d)\n", err));
		return err;
	}
	wl->conf->tx_power = dbm;
=======

	err = wl_set_tx_power(ndev, type, dbm);
	if (unlikely(err)) {
		WL_ERR(("error (%d)\n", err));
		return err;
	}

	cfg->conf->tx_power = dbm;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
static s32 wl_cfg80211_get_tx_power(struct wiphy *wiphy, s32 *dbm)
{
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	s32 txpwrdbm;
	u8 result;
	s32 err = 0;

	CHECK_SYS_UP(wl);
	err = wldev_iovar_getint(ndev, "qtxpower", &txpwrdbm);
	if (unlikely(err)) {
		WL_ERR(("error (%d)\n", err));
		return err;
	}
	result = (u8) (txpwrdbm & ~WL_TXPWR_OVERRIDE);
	*dbm = (s32) bcm_qdbm_to_mw(result);
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
static s32 wl_cfg80211_get_tx_power(struct wiphy *wiphy,
	struct wireless_dev *wdev, s32 *dbm)
#else
static s32 wl_cfg80211_get_tx_power(struct wiphy *wiphy, s32 *dbm)
#endif /* WL_CFG80211_P2P_DEV_IF */
{
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);
	s32 err = 0;

	RETURN_EIO_IF_NOT_UP(cfg);
	err = wl_get_tx_power(ndev, dbm);
	if (unlikely(err))
		WL_ERR(("error (%d)\n", err));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

static s32
wl_cfg80211_config_default_key(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, bool unicast, bool multicast)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	u32 index;
	s32 wsec;
	s32 err = 0;
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);

	WL_DBG(("key index (%d)\n", key_idx));
	CHECK_SYS_UP(wl);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	u32 index;
	s32 wsec;
	s32 err = 0;
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}

	WL_DBG(("key index (%d)\n", key_idx));
	RETURN_EIO_IF_NOT_UP(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	err = wldev_iovar_getint_bsscfg(dev, "wsec", &wsec, bssidx);
	if (unlikely(err)) {
		WL_ERR(("WLC_GET_WSEC error (%d)\n", err));
		return err;
	}
<<<<<<< HEAD
	if (wsec & WEP_ENABLED) {
=======
	if (wsec == WEP_ENABLED) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* Just select a new current key */
		index = (u32) key_idx;
		index = htod32(index);
		err = wldev_ioctl(dev, WLC_SET_KEY_PRIMARY, &index,
			sizeof(index), true);
		if (unlikely(err)) {
			WL_ERR(("error (%d)\n", err));
		}
	}
	return err;
}

static s32
wl_add_keyext(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, const u8 *mac_addr, struct key_params *params)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct wl_wsec_key key;
	s32 err = 0;
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);
	s32 mode = wl_get_mode_by_netdev(wl, dev);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct wl_wsec_key key;
	s32 err = 0;
	s32 bssidx;
	s32 mode = wl_get_mode_by_netdev(cfg, dev);
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	memset(&key, 0, sizeof(key));
	key.index = (u32) key_idx;

	if (!ETHER_ISMULTI(mac_addr))
		memcpy((char *)&key.ea, (void *)mac_addr, ETHER_ADDR_LEN);
	key.len = (u32) params->key_len;

	/* check for key index change */
	if (key.len == 0) {
		/* key delete */
		swap_key_from_BE(&key);
		err = wldev_iovar_setbuf_bsscfg(dev, "wsec_key", &key, sizeof(key),
<<<<<<< HEAD
			wl->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &wl->ioctl_buf_sync);
=======
			cfg->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (unlikely(err)) {
			WL_ERR(("key delete error (%d)\n", err));
			return err;
		}
	} else {
		if (key.len > sizeof(key.data)) {
			WL_ERR(("Invalid key length (%d)\n", key.len));
			return -EINVAL;
		}
		WL_DBG(("Setting the key index %d\n", key.index));
		memcpy(key.data, params->key, key.len);

		if ((mode == WL_MODE_BSS) &&
			(params->cipher == WLAN_CIPHER_SUITE_TKIP)) {
			u8 keybuf[8];
			memcpy(keybuf, &key.data[24], sizeof(keybuf));
			memcpy(&key.data[24], &key.data[16], sizeof(keybuf));
			memcpy(&key.data[16], keybuf, sizeof(keybuf));
		}

		/* if IW_ENCODE_EXT_RX_SEQ_VALID set */
		if (params->seq && params->seq_len == 6) {
			/* rx iv */
			u8 *ivptr;
			ivptr = (u8 *) params->seq;
			key.rxiv.hi = (ivptr[5] << 24) | (ivptr[4] << 16) |
				(ivptr[3] << 8) | ivptr[2];
			key.rxiv.lo = (ivptr[1] << 8) | ivptr[0];
			key.iv_initialized = true;
		}

		switch (params->cipher) {
		case WLAN_CIPHER_SUITE_WEP40:
			key.algo = CRYPTO_ALGO_WEP1;
			WL_DBG(("WLAN_CIPHER_SUITE_WEP40\n"));
			break;
		case WLAN_CIPHER_SUITE_WEP104:
			key.algo = CRYPTO_ALGO_WEP128;
			WL_DBG(("WLAN_CIPHER_SUITE_WEP104\n"));
			break;
		case WLAN_CIPHER_SUITE_TKIP:
			key.algo = CRYPTO_ALGO_TKIP;
			WL_DBG(("WLAN_CIPHER_SUITE_TKIP\n"));
			break;
		case WLAN_CIPHER_SUITE_AES_CMAC:
			key.algo = CRYPTO_ALGO_AES_CCM;
			WL_DBG(("WLAN_CIPHER_SUITE_AES_CMAC\n"));
			break;
		case WLAN_CIPHER_SUITE_CCMP:
			key.algo = CRYPTO_ALGO_AES_CCM;
			WL_DBG(("WLAN_CIPHER_SUITE_CCMP\n"));
			break;
<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
		case WLAN_CIPHER_SUITE_SMS4:
			key.algo = CRYPTO_ALGO_SMS4;
			WL_DBG(("WLAN_CIPHER_SUITE_SMS4\n"));
			break;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		default:
			WL_ERR(("Invalid cipher (0x%x)\n", params->cipher));
			return -EINVAL;
		}
		swap_key_from_BE(&key);
		/* need to guarantee EAPOL 4/4 send out before set key */
		dhd_wait_pend8021x(dev);
		err = wldev_iovar_setbuf_bsscfg(dev, "wsec_key", &key, sizeof(key),
<<<<<<< HEAD
			wl->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &wl->ioctl_buf_sync);
=======
			cfg->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (unlikely(err)) {
			WL_ERR(("WLC_SET_KEY error (%d)\n", err));
			return err;
		}
	}
	return err;
}

<<<<<<< HEAD
=======
int
wl_cfg80211_enable_roam_offload(struct net_device *dev, bool enable)
{
	int err;
	wl_eventmsg_buf_t ev_buf;

	if (dev != bcmcfg_to_prmry_ndev(g_bcm_cfg)) {
		/* roam offload is only for the primary device */
		return -1;
	}
	err = wldev_iovar_setint(dev, "roam_offload", (int)enable);
	if (err)
		return err;

	bzero(&ev_buf, sizeof(wl_eventmsg_buf_t));
	wl_cfg80211_add_to_eventbuffer(&ev_buf, WLC_E_PSK_SUP, !enable);
	wl_cfg80211_add_to_eventbuffer(&ev_buf, WLC_E_ASSOC_REQ_IE, !enable);
	wl_cfg80211_add_to_eventbuffer(&ev_buf, WLC_E_ASSOC_RESP_IE, !enable);
	wl_cfg80211_add_to_eventbuffer(&ev_buf, WLC_E_REASSOC, !enable);
	wl_cfg80211_add_to_eventbuffer(&ev_buf, WLC_E_JOIN, !enable);
	wl_cfg80211_add_to_eventbuffer(&ev_buf, WLC_E_ROAM, !enable);
	err = wl_cfg80211_apply_eventbuffer(dev, g_bcm_cfg, &ev_buf);
	if (!err) {
		g_bcm_cfg->roam_offload = enable;
	}
	return err;
}

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32
wl_cfg80211_add_key(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, bool pairwise, const u8 *mac_addr,
	struct key_params *params)
{
	struct wl_wsec_key key;
	s32 val = 0;
	s32 wsec = 0;
	s32 err = 0;
	u8 keybuf[8];
	s32 bssidx = 0;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	s32 mode = wl_get_mode_by_netdev(wl, dev);
	WL_DBG(("key index (%d)\n", key_idx));
	CHECK_SYS_UP(wl);

	bssidx = wl_cfgp2p_find_idx(wl, dev);

	if (mac_addr) {
		wl_add_keyext(wiphy, dev, key_idx, mac_addr, params);
		goto exit;
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	s32 mode = wl_get_mode_by_netdev(cfg, dev);
	WL_DBG(("key index (%d)\n", key_idx));
	RETURN_EIO_IF_NOT_UP(cfg);

	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}

	if (mac_addr &&
		((params->cipher != WLAN_CIPHER_SUITE_WEP40) &&
		(params->cipher != WLAN_CIPHER_SUITE_WEP104))) {
			wl_add_keyext(wiphy, dev, key_idx, mac_addr, params);
			goto exit;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
	memset(&key, 0, sizeof(key));

	key.len = (u32) params->key_len;
	key.index = (u32) key_idx;

	if (unlikely(key.len > sizeof(key.data))) {
		WL_ERR(("Too long key length (%u)\n", key.len));
		return -EINVAL;
	}
	memcpy(key.data, params->key, key.len);

	key.flags = WL_PRIMARY_KEY;
	switch (params->cipher) {
	case WLAN_CIPHER_SUITE_WEP40:
		key.algo = CRYPTO_ALGO_WEP1;
		val = WEP_ENABLED;
		WL_DBG(("WLAN_CIPHER_SUITE_WEP40\n"));
		break;
	case WLAN_CIPHER_SUITE_WEP104:
		key.algo = CRYPTO_ALGO_WEP128;
		val = WEP_ENABLED;
		WL_DBG(("WLAN_CIPHER_SUITE_WEP104\n"));
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		key.algo = CRYPTO_ALGO_TKIP;
		val = TKIP_ENABLED;
		/* wpa_supplicant switches the third and fourth quarters of the TKIP key */
		if (mode == WL_MODE_BSS) {
			bcopy(&key.data[24], keybuf, sizeof(keybuf));
			bcopy(&key.data[16], &key.data[24], sizeof(keybuf));
			bcopy(keybuf, &key.data[16], sizeof(keybuf));
		}
		WL_DBG(("WLAN_CIPHER_SUITE_TKIP\n"));
		break;
	case WLAN_CIPHER_SUITE_AES_CMAC:
		key.algo = CRYPTO_ALGO_AES_CCM;
		val = AES_ENABLED;
		WL_DBG(("WLAN_CIPHER_SUITE_AES_CMAC\n"));
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		key.algo = CRYPTO_ALGO_AES_CCM;
		val = AES_ENABLED;
		WL_DBG(("WLAN_CIPHER_SUITE_CCMP\n"));
		break;
<<<<<<< HEAD
	default:
		WL_ERR(("Invalid cipher (0x%x)\n", params->cipher));
		return -EINVAL;
	}

	/* Set the new key/index */
	swap_key_from_BE(&key);
	err = wldev_iovar_setbuf_bsscfg(dev, "wsec_key", &key, sizeof(key), wl->ioctl_buf,
		WLC_IOCTL_MAXLEN, bssidx, &wl->ioctl_buf_sync);
=======
#ifdef BCMWAPI_WPI
	case WLAN_CIPHER_SUITE_SMS4:
		key.algo = CRYPTO_ALGO_SMS4;
		WL_DBG(("WLAN_CIPHER_SUITE_SMS4\n"));
		val = SMS4_ENABLED;
		break;
#endif /* BCMWAPI_WPI */
#if defined(WLFBT) && defined(WLAN_CIPHER_SUITE_PMK)
	case WLAN_CIPHER_SUITE_PMK: {
		int j;
		wsec_pmk_t pmk;
		char keystring[WSEC_MAX_PSK_LEN + 1];
		char* charptr = keystring;
		uint len;
		struct wl_security *sec;

		sec = wl_read_prof(cfg, dev, WL_PROF_SEC);
		if (sec->wpa_auth == WLAN_AKM_SUITE_8021X) {
			err = wldev_iovar_setbuf(dev, "okc_info_pmk", params->key,
				WSEC_MAX_PSK_LEN / 2, keystring, sizeof(keystring), NULL);
			if (err) {
				/* could fail in case that 'okc' is not supported */
				WL_INFO(("Setting 'okc_info_pmk' failed, err=%d\n", err));
			}
		}
		/* copy the raw hex key to the appropriate format */
		for (j = 0; j < (WSEC_MAX_PSK_LEN / 2); j++) {
			sprintf(charptr, "%02x", params->key[j]);
			charptr += 2;
		}
		len = strlen(keystring);
		pmk.key_len = htod16(len);
		bcopy(keystring, pmk.key, len);
		pmk.flags = htod16(WSEC_PASSPHRASE);

		err = wldev_ioctl(dev, WLC_SET_WSEC_PMK, &pmk, sizeof(pmk), true);
		if (err)
			return err;
	} break;
#endif /* WLFBT && WLAN_CIPHER_SUITE_PMK */
	default:
		WL_ERR(("Invalid cipher (0x%x)\n", params->cipher));
		return -EINVAL;
	}

	/* Set the new key/index */
	if ((mode == WL_MODE_IBSS) && (val & (TKIP_ENABLED | AES_ENABLED))) {
		WL_ERR(("IBSS KEY setted\n"));
		wldev_iovar_setint(dev, "wpa_auth", WPA_AUTH_NONE);
	}
	swap_key_from_BE(&key);
	err = wldev_iovar_setbuf_bsscfg(dev, "wsec_key", &key, sizeof(key), cfg->ioctl_buf,
		WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (unlikely(err)) {
		WL_ERR(("WLC_SET_KEY error (%d)\n", err));
		return err;
	}

exit:
	err = wldev_iovar_getint_bsscfg(dev, "wsec", &wsec, bssidx);
	if (unlikely(err)) {
		WL_ERR(("get wsec error (%d)\n", err));
		return err;
	}

	wsec |= val;
	err = wldev_iovar_setint_bsscfg(dev, "wsec", wsec, bssidx);
	if (unlikely(err)) {
		WL_ERR(("set wsec error (%d)\n", err));
		return err;
	}

	return err;
}

static s32
wl_cfg80211_del_key(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, bool pairwise, const u8 *mac_addr)
{
	struct wl_wsec_key key;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	s32 err = 0;
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);

	WL_DBG(("Enter\n"));
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	s32 err = 0;
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
	WL_DBG(("Enter\n"));

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#ifndef IEEE80211W
	if ((key_idx >= DOT11_MAX_DEFAULT_KEYS) && (key_idx < DOT11_MAX_DEFAULT_KEYS+2))
		return -EINVAL;
#endif
<<<<<<< HEAD
	CHECK_SYS_UP(wl);
=======

	RETURN_EIO_IF_NOT_UP(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	memset(&key, 0, sizeof(key));

	key.flags = WL_PRIMARY_KEY;
	key.algo = CRYPTO_ALGO_OFF;
	key.index = (u32) key_idx;

	WL_DBG(("key index (%d)\n", key_idx));
	/* Set the new key/index */
	swap_key_from_BE(&key);
<<<<<<< HEAD
	err = wldev_iovar_setbuf_bsscfg(dev, "wsec_key", &key, sizeof(key), wl->ioctl_buf,
		WLC_IOCTL_MAXLEN, bssidx, &wl->ioctl_buf_sync);
=======
	err = wldev_iovar_setbuf_bsscfg(dev, "wsec_key", &key, sizeof(key), cfg->ioctl_buf,
		WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (unlikely(err)) {
		if (err == -EINVAL) {
			if (key.index >= DOT11_MAX_DEFAULT_KEYS) {
				/* we ignore this key index in this case */
				WL_DBG(("invalid key index (%d)\n", key_idx));
			}
		} else {
			WL_ERR(("WLC_SET_KEY error (%d)\n", err));
		}
		return err;
	}
	return err;
}

static s32
wl_cfg80211_get_key(struct wiphy *wiphy, struct net_device *dev,
	u8 key_idx, bool pairwise, const u8 *mac_addr, void *cookie,
	void (*callback) (void *cookie, struct key_params * params))
{
	struct key_params params;
	struct wl_wsec_key key;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct wl_security *sec;
	s32 wsec;
	s32 err = 0;
	s32 bssidx = wl_cfgp2p_find_idx(wl, dev);

	WL_DBG(("key index (%d)\n", key_idx));
	CHECK_SYS_UP(wl);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct wl_security *sec;
	s32 wsec;
	s32 err = 0;
	s32 bssidx;
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
	WL_DBG(("key index (%d)\n", key_idx));
	RETURN_EIO_IF_NOT_UP(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	memset(&key, 0, sizeof(key));
	key.index = key_idx;
	swap_key_to_BE(&key);
	memset(&params, 0, sizeof(params));
	params.key_len = (u8) min_t(u8, DOT11_MAX_KEY_SIZE, key.len);
	memcpy(params.key, key.data, params.key_len);

<<<<<<< HEAD
	wldev_iovar_getint_bsscfg(dev, "wsec", &wsec, bssidx);
=======
	err = wldev_iovar_getint_bsscfg(dev, "wsec", &wsec, bssidx);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (unlikely(err)) {
		WL_ERR(("WLC_GET_WSEC error (%d)\n", err));
		return err;
	}
	switch (wsec & ~SES_OW_ENABLED) {
		case WEP_ENABLED:
<<<<<<< HEAD
			sec = wl_read_prof(wl, dev, WL_PROF_SEC);
=======
			sec = wl_read_prof(cfg, dev, WL_PROF_SEC);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			if (sec->cipher_pairwise & WLAN_CIPHER_SUITE_WEP40) {
				params.cipher = WLAN_CIPHER_SUITE_WEP40;
				WL_DBG(("WLAN_CIPHER_SUITE_WEP40\n"));
			} else if (sec->cipher_pairwise & WLAN_CIPHER_SUITE_WEP104) {
				params.cipher = WLAN_CIPHER_SUITE_WEP104;
				WL_DBG(("WLAN_CIPHER_SUITE_WEP104\n"));
			}
			break;
		case TKIP_ENABLED:
			params.cipher = WLAN_CIPHER_SUITE_TKIP;
			WL_DBG(("WLAN_CIPHER_SUITE_TKIP\n"));
			break;
		case AES_ENABLED:
			params.cipher = WLAN_CIPHER_SUITE_AES_CMAC;
			WL_DBG(("WLAN_CIPHER_SUITE_AES_CMAC\n"));
			break;
<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
		case WLAN_CIPHER_SUITE_SMS4:
			key.algo = CRYPTO_ALGO_SMS4;
			WL_DBG(("WLAN_CIPHER_SUITE_SMS4\n"));
			break;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		default:
			WL_ERR(("Invalid algo (0x%x)\n", wsec));
			return -EINVAL;
	}

	callback(cookie, &params);
	return err;
}

static s32
wl_cfg80211_config_default_mgmt_key(struct wiphy *wiphy,
	struct net_device *dev, u8 key_idx)
{
	WL_INFO(("Not supported\n"));
	return -EOPNOTSUPP;
}

static s32
wl_cfg80211_get_station(struct wiphy *wiphy, struct net_device *dev,
	u8 *mac, struct station_info *sinfo)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	scb_val_t scb_val;
	s32 rssi;
	s32 rate;
	s32 err = 0;
	sta_info_t *sta;
<<<<<<< HEAD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)
	s8 eabuf[ETHER_ADDR_STR_LEN];
#endif
	dhd_pub_t *dhd =  (dhd_pub_t *)(wl->pub);
	CHECK_SYS_UP(wl);
	if (wl_get_mode_by_netdev(wl, dev) == WL_MODE_AP) {
		err = wldev_iovar_getbuf(dev, "sta_info", (struct ether_addr *)mac,
			ETHER_ADDR_LEN, wl->ioctl_buf, WLC_IOCTL_SMLEN, &wl->ioctl_buf_sync);
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)) || defined(WL_COMPAT_WIRELESS)
	s8 eabuf[ETHER_ADDR_STR_LEN];
#endif
	dhd_pub_t *dhd =  (dhd_pub_t *)(cfg->pub);
	RETURN_EIO_IF_NOT_UP(cfg);
	if (wl_get_mode_by_netdev(cfg, dev) == WL_MODE_AP) {
		err = wldev_iovar_getbuf(dev, "sta_info", (struct ether_addr *)mac,
			ETHER_ADDR_LEN, cfg->ioctl_buf, WLC_IOCTL_SMLEN, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (err < 0) {
			WL_ERR(("GET STA INFO failed, %d\n", err));
			return err;
		}
		sinfo->filled = STATION_INFO_INACTIVE_TIME;
<<<<<<< HEAD
		sta = (sta_info_t *)wl->ioctl_buf;
=======
		sta = (sta_info_t *)cfg->ioctl_buf;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		sta->len = dtoh16(sta->len);
		sta->cap = dtoh16(sta->cap);
		sta->flags = dtoh32(sta->flags);
		sta->idle = dtoh32(sta->idle);
		sta->in = dtoh32(sta->in);
		sinfo->inactive_time = sta->idle * 1000;
<<<<<<< HEAD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)) || defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (sta->flags & WL_STA_ASSOC) {
			sinfo->filled |= STATION_INFO_CONNECTED_TIME;
			sinfo->connected_time = sta->in;
		}
		WL_INFO(("STA %s : idle time : %d sec, connected time :%d ms\n",
			bcm_ether_ntoa((const struct ether_addr *)mac, eabuf), sinfo->inactive_time,
			sta->idle * 1000));
#endif
<<<<<<< HEAD
	} else if (wl_get_mode_by_netdev(wl, dev) == WL_MODE_BSS) {
		get_pktcnt_t pktcnt;
		u8 *curmacp = wl_read_prof(wl, dev, WL_PROF_BSSID);
		if (!wl_get_drv_status(wl, CONNECTED, dev) ||
=======
	} else if (wl_get_mode_by_netdev(cfg, dev) == WL_MODE_BSS ||
		wl_get_mode_by_netdev(cfg, dev) == WL_MODE_IBSS) {
		get_pktcnt_t pktcnt;
		u8 *curmacp;

		if (cfg->roam_offload) {
			struct ether_addr bssid;
			err = wldev_ioctl(dev, WLC_GET_BSSID, &bssid, ETHER_ADDR_LEN, false);
			if (err) {
				WL_ERR(("Failed to get current BSSID\n"));
			} else {
				if (memcmp(mac, &bssid.octet, ETHER_ADDR_LEN) != 0) {
					/* roaming is detected */
					err = wl_cfg80211_delayed_roam(cfg, dev, &bssid);
					if (err)
						WL_ERR(("Failed to handle the delayed roam, "
							"err=%d", err));
					mac = (u8 *)bssid.octet;
				}
			}
		}
		if (!wl_get_drv_status(cfg, CONNECTED, dev) ||
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			(dhd_is_associated(dhd, NULL, &err) == FALSE)) {
			WL_ERR(("NOT assoc\n"));
			if (err == -ERESTARTSYS)
				return err;
			err = -ENODEV;
			return err;
		}
<<<<<<< HEAD
=======
		curmacp = wl_read_prof(cfg, dev, WL_PROF_BSSID);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (memcmp(mac, curmacp, ETHER_ADDR_LEN)) {
			WL_ERR(("Wrong Mac address: "MACDBG" != "MACDBG"\n",
				MAC2STRDBG(mac), MAC2STRDBG(curmacp)));
		}

		/* Report the current tx rate */
		err = wldev_ioctl(dev, WLC_GET_RATE, &rate, sizeof(rate), false);
		if (err) {
			WL_ERR(("Could not get rate (%d)\n", err));
		} else {
<<<<<<< HEAD
=======
#if defined(USE_DYNAMIC_MAXPKT_RXGLOM)
			int rxpktglom;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			rate = dtoh32(rate);
			sinfo->filled |= STATION_INFO_TX_BITRATE;
			sinfo->txrate.legacy = rate * 5;
			WL_DBG(("Rate %d Mbps\n", (rate / 2)));
<<<<<<< HEAD
=======
#if defined(USE_DYNAMIC_MAXPKT_RXGLOM)
			rxpktglom = ((rate/2) > 150) ? 20 : 10;

			if (maxrxpktglom != rxpktglom) {
				maxrxpktglom = rxpktglom;
				WL_DBG(("Rate %d Mbps, update bus:maxtxpktglom=%d\n", (rate/2),
					maxrxpktglom));
				err = wldev_iovar_setbuf(dev, "bus:maxtxpktglom",
					(char*)&maxrxpktglom, 4, cfg->ioctl_buf,
					WLC_IOCTL_MAXLEN, NULL);
				if (err < 0) {
					WL_ERR(("set bus:maxtxpktglom failed, %d\n", err));
				}
			}
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}

		memset(&scb_val, 0, sizeof(scb_val));
		scb_val.val = 0;
		err = wldev_ioctl(dev, WLC_GET_RSSI, &scb_val,
			sizeof(scb_val_t), false);
		if (err) {
			WL_ERR(("Could not get rssi (%d)\n", err));
			goto get_station_err;
		}
<<<<<<< HEAD
		rssi = dtoh32(scb_val.val) + RSSI_OFFSET;
=======
		rssi = wl_rssi_offset(dtoh32(scb_val.val));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		sinfo->filled |= STATION_INFO_SIGNAL;
		sinfo->signal = rssi;
		WL_DBG(("RSSI %d dBm\n", rssi));
		err = wldev_ioctl(dev, WLC_GET_PKTCNTS, &pktcnt,
			sizeof(pktcnt), false);
		if (!err) {
			sinfo->filled |= (STATION_INFO_RX_PACKETS |
				STATION_INFO_RX_DROP_MISC |
				STATION_INFO_TX_PACKETS |
				STATION_INFO_TX_FAILED);
			sinfo->rx_packets = pktcnt.rx_good_pkt;
			sinfo->rx_dropped_misc = pktcnt.rx_bad_pkt;
			sinfo->tx_packets = pktcnt.tx_good_pkt;
			sinfo->tx_failed  = pktcnt.tx_bad_pkt;
		}
get_station_err:
		if (err && (err != -ERESTARTSYS)) {
			/* Disconnect due to zero BSSID or error to get RSSI */
<<<<<<< HEAD
			WL_ERR(("force cfg80211_disconnected\n"));
			wl_clr_drv_status(wl, CONNECTED, dev);
			cfg80211_disconnected(dev, 0, NULL, 0, GFP_KERNEL);
			wl_link_down(wl);
		}
	}
=======
			WL_ERR(("force cfg80211_disconnected: %d\n", err));
			wl_clr_drv_status(cfg, CONNECTED, dev);
			cfg80211_disconnected(dev, 0, NULL, 0, GFP_KERNEL);
			wl_link_down(cfg);
		}
	}
	else {
		WL_ERR(("Invalid device mode %d\n", wl_get_mode_by_netdev(cfg, dev)));
	}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
/* Function to update sta power save mode for Kernel wifi stack */
int wl_cfg80211_update_power_mode(struct net_device *dev)
{
	int pm = -1;
	int err;

	err = wldev_ioctl(dev, WLC_GET_PM, &pm, sizeof(pm), false);
	if (err || (pm == -1)) {
		WL_ERR(("error (%d)\n", err));
	} else {
		pm = (pm == PM_OFF) ? false : true;
		WL_DBG(("%s: %d\n", __func__, pm));
		if (dev->ieee80211_ptr)
			dev->ieee80211_ptr->ps = pm;
	}
	return err;
}
=======
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32
wl_cfg80211_set_power_mgmt(struct wiphy *wiphy, struct net_device *dev,
	bool enabled, s32 timeout)
{
	s32 pm;
	s32 err = 0;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct net_info *_net_info = wl_get_netinfo_by_netdev(wl, dev);
#if !defined(SUPPORT_PM2_ONLY)
	dhd_pub_t *dhd = (dhd_pub_t *)(wl->pub);
#endif /* (OEM_ANDROID) */
	CHECK_SYS_UP(wl);

	if (wl->p2p_net == dev || _net_info == NULL) {
		return err;
	}
	WL_DBG(("%s: Enter power save enabled %d\n", dev->name, enabled));

#if !defined(SUPPORT_PM2_ONLY)
	/* android has special hooks to change pm when kernel suspended */
	pm = enabled ? ((dhd->in_suspend) ? PM_MAX : PM_FAST) : PM_OFF;
#else
	pm = enabled ? PM_FAST : PM_OFF;
#endif /* SUPPORT_PM2_ONLY */

	if (_net_info->pm_block || wl->vsdb_mode) {
		/* Do not enable the power save if it is p2p interface or vsdb mode is set */
		WL_DBG(("%s:Do not enable the power save for pm_block %d or vsdb_mode %d\n",
			dev->name, _net_info->pm_block, wl->vsdb_mode));
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct net_info *_net_info = wl_get_netinfo_by_netdev(cfg, dev);

	RETURN_EIO_IF_NOT_UP(cfg);
	WL_DBG(("Enter\n"));
	if (cfg->p2p_net == dev || _net_info == NULL || cfg->vsdb_mode ||
		!wl_get_drv_status(cfg, CONNECTED, dev)) {
		return err;
	}

	/* Delete pm_enable_work */
	wl_add_remove_pm_enable_work(cfg, FALSE, WL_HANDLER_PEND);

	pm = enabled ? PM_FAST : PM_OFF;
	if (_net_info->pm_block) {
		WL_ERR(("%s:Do not enable the power save for pm_block %d\n",
			dev->name, _net_info->pm_block));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		pm = PM_OFF;
	}
	pm = htod32(pm);
	WL_DBG(("%s:power save %s\n", dev->name, (pm ? "enabled" : "disabled")));
	err = wldev_ioctl(dev, WLC_SET_PM, &pm, sizeof(pm), true);
	if (unlikely(err)) {
		if (err == -ENODEV)
			WL_DBG(("net_device is not ready yet\n"));
		else
			WL_ERR(("error (%d)\n", err));
		return err;
	}
<<<<<<< HEAD
	return err;
}

=======
	wl_cfg80211_update_power_mode(dev);
	return err;
}

void wl_cfg80211_update_power_mode(struct net_device *dev)
{
	int err, pm = -1;

	err = wldev_ioctl(dev, WLC_GET_PM, &pm, sizeof(pm), true);
	if (err)
		WL_ERR(("%s:error (%d)\n", __FUNCTION__, err));
	else if (pm != -1 && dev->ieee80211_ptr)
		dev->ieee80211_ptr->ps = (pm == PM_OFF) ? false : true;
}

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static __used u32 wl_find_msb(u16 bit16)
{
	u32 ret = 0;

	if (bit16 & 0xff00) {
		ret += 8;
		bit16 >>= 8;
	}

	if (bit16 & 0xf0) {
		ret += 4;
		bit16 >>= 4;
	}

	if (bit16 & 0xc) {
		ret += 2;
		bit16 >>= 2;
	}

	if (bit16 & 2)
		ret += bit16 & 2;
	else if (bit16)
		ret += bit16;

	return ret;
}

static s32 wl_cfg80211_resume(struct wiphy *wiphy)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	s32 err = 0;

	if (unlikely(!wl_get_drv_status(wl, READY, ndev))) {
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);
	s32 err = 0;

	if (unlikely(!wl_get_drv_status(cfg, READY, ndev))) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_INFO(("device is not ready\n"));
		return 0;
	}

<<<<<<< HEAD
	wl_invoke_iscan(wl);

	return err;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)
=======
	return err;
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) || defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32 wl_cfg80211_suspend(struct wiphy *wiphy, struct cfg80211_wowlan *wow)
#else
static s32 wl_cfg80211_suspend(struct wiphy *wiphy)
#endif
{
#ifdef DHD_CLEAR_ON_SUSPEND
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct net_info *iter, *next;
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	unsigned long flags;
	if (unlikely(!wl_get_drv_status(wl, READY, ndev))) {
		WL_INFO(("device is not ready : status (%d)\n",
			(int)wl->status));
		return 0;
	}
	for_each_ndev(wl, iter, next)
		wl_set_drv_status(wl, SCAN_ABORTING, iter->ndev);
	wl_term_iscan(wl);
	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
	if (wl->scan_request) {
		cfg80211_scan_done(wl->scan_request, true);
		wl->scan_request = NULL;
	}
	for_each_ndev(wl, iter, next) {
		wl_clr_drv_status(wl, SCANNING, iter->ndev);
		wl_clr_drv_status(wl, SCAN_ABORTING, iter->ndev);
	}
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);
	for_each_ndev(wl, iter, next) {
		if (wl_get_drv_status(wl, CONNECTING, iter->ndev)) {
			wl_bss_connect_done(wl, iter->ndev, NULL, NULL, false);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct net_info *iter, *next;
	struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);
	unsigned long flags;
	if (unlikely(!wl_get_drv_status(cfg, READY, ndev))) {
		WL_INFO(("device is not ready : status (%d)\n",
			(int)cfg->status));
		return 0;
	}
	for_each_ndev(cfg, iter, next)
		wl_set_drv_status(cfg, SCAN_ABORTING, iter->ndev);
	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
	if (cfg->scan_request) {
		cfg80211_scan_done(cfg->scan_request, true);
		cfg->scan_request = NULL;
	}
	for_each_ndev(cfg, iter, next) {
		wl_clr_drv_status(cfg, SCANNING, iter->ndev);
		wl_clr_drv_status(cfg, SCAN_ABORTING, iter->ndev);
	}
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);
	for_each_ndev(cfg, iter, next) {
		if (wl_get_drv_status(cfg, CONNECTING, iter->ndev)) {
			wl_bss_connect_done(cfg, iter->ndev, NULL, NULL, false);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	}
#endif /* DHD_CLEAR_ON_SUSPEND */
	return 0;
}

static s32
wl_update_pmklist(struct net_device *dev, struct wl_pmk_list *pmk_list,
	s32 err)
{
	int i, j;
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
	struct net_device *primary_dev = wl_to_prmry_ndev(wl);
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	struct net_device *primary_dev = bcmcfg_to_prmry_ndev(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (!pmk_list) {
		printk("pmk_list is NULL\n");
		return -EINVAL;
	}
	/* pmk list is supported only for STA interface i.e. primary interface
	 * Refer code wlc_bsscfg.c->wlc_bsscfg_sta_init
	 */
	if (primary_dev != dev) {
		WL_INFO(("Not supporting Flushing pmklist on virtual"
			" interfaces than primary interface\n"));
		return err;
	}

	WL_DBG(("No of elements %d\n", pmk_list->pmkids.npmkid));
	for (i = 0; i < pmk_list->pmkids.npmkid; i++) {
		WL_DBG(("PMKID[%d]: %pM =\n", i,
			&pmk_list->pmkids.pmkid[i].BSSID));
		for (j = 0; j < WPA2_PMKID_LEN; j++) {
			WL_DBG(("%02x\n", pmk_list->pmkids.pmkid[i].PMKID[j]));
		}
	}
	if (likely(!err)) {
		err = wldev_iovar_setbuf(dev, "pmkid_info", (char *)pmk_list,
<<<<<<< HEAD
			sizeof(*pmk_list), wl->ioctl_buf, WLC_IOCTL_MAXLEN, NULL);
=======
			sizeof(*pmk_list), cfg->ioctl_buf, WLC_IOCTL_MAXLEN, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

	return err;
}

static s32
wl_cfg80211_set_pmksa(struct wiphy *wiphy, struct net_device *dev,
	struct cfg80211_pmksa *pmksa)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	s32 err = 0;
	int i;

	CHECK_SYS_UP(wl);
	for (i = 0; i < wl->pmk_list->pmkids.npmkid; i++)
		if (!memcmp(pmksa->bssid, &wl->pmk_list->pmkids.pmkid[i].BSSID,
			ETHER_ADDR_LEN))
			break;
	if (i < WL_NUM_PMKIDS_MAX) {
		memcpy(&wl->pmk_list->pmkids.pmkid[i].BSSID, pmksa->bssid,
			ETHER_ADDR_LEN);
		memcpy(&wl->pmk_list->pmkids.pmkid[i].PMKID, pmksa->pmkid,
			WPA2_PMKID_LEN);
		if (i == wl->pmk_list->pmkids.npmkid)
			wl->pmk_list->pmkids.npmkid++;
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	s32 err = 0;
	int i;

	RETURN_EIO_IF_NOT_UP(cfg);
	for (i = 0; i < cfg->pmk_list->pmkids.npmkid; i++)
		if (!memcmp(pmksa->bssid, &cfg->pmk_list->pmkids.pmkid[i].BSSID,
			ETHER_ADDR_LEN))
			break;
	if (i < WL_NUM_PMKIDS_MAX) {
		memcpy(&cfg->pmk_list->pmkids.pmkid[i].BSSID, pmksa->bssid,
			ETHER_ADDR_LEN);
		memcpy(&cfg->pmk_list->pmkids.pmkid[i].PMKID, pmksa->pmkid,
			WPA2_PMKID_LEN);
		if (i == cfg->pmk_list->pmkids.npmkid)
			cfg->pmk_list->pmkids.npmkid++;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	} else {
		err = -EINVAL;
	}
	WL_DBG(("set_pmksa,IW_PMKSA_ADD - PMKID: %pM =\n",
<<<<<<< HEAD
		&wl->pmk_list->pmkids.pmkid[wl->pmk_list->pmkids.npmkid - 1].BSSID));
	for (i = 0; i < WPA2_PMKID_LEN; i++) {
		WL_DBG(("%02x\n",
			wl->pmk_list->pmkids.pmkid[wl->pmk_list->pmkids.npmkid - 1].
			PMKID[i]));
	}

	err = wl_update_pmklist(dev, wl->pmk_list, err);
=======
		&cfg->pmk_list->pmkids.pmkid[cfg->pmk_list->pmkids.npmkid - 1].BSSID));
	for (i = 0; i < WPA2_PMKID_LEN; i++) {
		WL_DBG(("%02x\n",
			cfg->pmk_list->pmkids.pmkid[cfg->pmk_list->pmkids.npmkid - 1].
			PMKID[i]));
	}

	err = wl_update_pmklist(dev, cfg->pmk_list, err);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

static s32
wl_cfg80211_del_pmksa(struct wiphy *wiphy, struct net_device *dev,
	struct cfg80211_pmksa *pmksa)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	struct _pmkid_list pmkid;
	s32 err = 0;
	int i;

	CHECK_SYS_UP(wl);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct _pmkid_list pmkid = {0};
	s32 err = 0;
	int i;

	RETURN_EIO_IF_NOT_UP(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	memcpy(&pmkid.pmkid[0].BSSID, pmksa->bssid, ETHER_ADDR_LEN);
	memcpy(pmkid.pmkid[0].PMKID, pmksa->pmkid, WPA2_PMKID_LEN);

	WL_DBG(("del_pmksa,IW_PMKSA_REMOVE - PMKID: %pM =\n",
		&pmkid.pmkid[0].BSSID));
	for (i = 0; i < WPA2_PMKID_LEN; i++) {
		WL_DBG(("%02x\n", pmkid.pmkid[0].PMKID[i]));
	}

<<<<<<< HEAD
	for (i = 0; i < wl->pmk_list->pmkids.npmkid; i++)
		if (!memcmp
		    (pmksa->bssid, &wl->pmk_list->pmkids.pmkid[i].BSSID,
		     ETHER_ADDR_LEN))
			break;

	if ((wl->pmk_list->pmkids.npmkid > 0) &&
		(i < wl->pmk_list->pmkids.npmkid)) {
		memset(&wl->pmk_list->pmkids.pmkid[i], 0, sizeof(pmkid_t));
		for (; i < (wl->pmk_list->pmkids.npmkid - 1); i++) {
			memcpy(&wl->pmk_list->pmkids.pmkid[i].BSSID,
				&wl->pmk_list->pmkids.pmkid[i + 1].BSSID,
				ETHER_ADDR_LEN);
			memcpy(&wl->pmk_list->pmkids.pmkid[i].PMKID,
				&wl->pmk_list->pmkids.pmkid[i + 1].PMKID,
				WPA2_PMKID_LEN);
		}
		wl->pmk_list->pmkids.npmkid--;
=======
	for (i = 0; i < cfg->pmk_list->pmkids.npmkid; i++)
		if (!memcmp
		    (pmksa->bssid, &cfg->pmk_list->pmkids.pmkid[i].BSSID,
		     ETHER_ADDR_LEN))
			break;

	if ((cfg->pmk_list->pmkids.npmkid > 0) &&
		(i < cfg->pmk_list->pmkids.npmkid)) {
		memset(&cfg->pmk_list->pmkids.pmkid[i], 0, sizeof(pmkid_t));
		for (; i < (cfg->pmk_list->pmkids.npmkid - 1); i++) {
			memcpy(&cfg->pmk_list->pmkids.pmkid[i].BSSID,
				&cfg->pmk_list->pmkids.pmkid[i + 1].BSSID,
				ETHER_ADDR_LEN);
			memcpy(&cfg->pmk_list->pmkids.pmkid[i].PMKID,
				&cfg->pmk_list->pmkids.pmkid[i + 1].PMKID,
				WPA2_PMKID_LEN);
		}
		cfg->pmk_list->pmkids.npmkid--;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	} else {
		err = -EINVAL;
	}

<<<<<<< HEAD
	err = wl_update_pmklist(dev, wl->pmk_list, err);
=======
	err = wl_update_pmklist(dev, cfg->pmk_list, err);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;

}

static s32
wl_cfg80211_flush_pmksa(struct wiphy *wiphy, struct net_device *dev)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	s32 err = 0;
	CHECK_SYS_UP(wl);
	memset(wl->pmk_list, 0, sizeof(*wl->pmk_list));
	err = wl_update_pmklist(dev, wl->pmk_list, err);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	s32 err = 0;
	RETURN_EIO_IF_NOT_UP(cfg);
	memset(cfg->pmk_list, 0, sizeof(*cfg->pmk_list));
	err = wl_update_pmklist(dev, cfg->pmk_list, err);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;

}

static wl_scan_params_t *
wl_cfg80211_scan_alloc_params(int channel, int nprobes, int *out_params_size)
{
	wl_scan_params_t *params;
	int params_size;
	int num_chans;

	*out_params_size = 0;

	/* Our scan params only need space for 1 channel and 0 ssids */
	params_size = WL_SCAN_PARAMS_FIXED_SIZE + 1 * sizeof(uint16);
	params = (wl_scan_params_t*) kzalloc(params_size, GFP_KERNEL);
	if (params == NULL) {
<<<<<<< HEAD
		WL_ERR(("%s: mem alloc failed (%d bytes)\n", __func__, params_size));
=======
		WL_ERR(("mem alloc failed (%d bytes)\n", params_size));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		return params;
	}
	memset(params, 0, params_size);
	params->nprobes = nprobes;

	num_chans = (channel == 0) ? 0 : 1;

	memcpy(&params->bssid, &ether_bcast, ETHER_ADDR_LEN);
	params->bss_type = DOT11_BSSTYPE_ANY;
	params->scan_type = DOT11_SCANTYPE_ACTIVE;
	params->nprobes = htod32(1);
	params->active_time = htod32(-1);
	params->passive_time = htod32(-1);
	params->home_time = htod32(10);
	if (channel == -1)
		params->channel_list[0] = htodchanspec(channel);
	else
		params->channel_list[0] = wl_ch_host_to_driver(channel);

	/* Our scan params have 1 channel and 0 ssids */
	params->channel_num = htod32((0 << WL_SCAN_PARAMS_NSSID_SHIFT) |
		(num_chans & WL_SCAN_PARAMS_COUNT_MASK));

	*out_params_size = params_size;	/* rtn size to the caller */
	return params;
}

<<<<<<< HEAD
static s32
wl_cfg80211_remain_on_channel(struct wiphy *wiphy, struct net_device *dev,
	struct ieee80211_channel * channel,
	enum nl80211_channel_type channel_type,
	unsigned int duration, u64 *cookie)
{
	s32 target_channel;
	u32 id;
	struct ether_addr primary_mac;
	struct net_device *ndev = NULL;

	s32 err = BCME_OK;
	struct wl_priv *wl = wiphy_priv(wiphy);

	WL_DBG(("Enter, ifindex: %d, channel: %d, duration ms (%d) SCANNING ?? %s \n",
		dev->ifindex, ieee80211_frequency_to_channel(channel->center_freq),
		duration, (wl_get_drv_status(wl, SCANNING, ndev)) ? "YES":"NO"));

	if (wl->p2p_net == dev) {
		ndev = wl_to_prmry_ndev(wl);
	} else {
		ndev = dev;
	}

	if (!wl->p2p) {
		WL_ERR(("wl->p2p is not initialized\n"));
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
static s32
wl_cfg80211_remain_on_channel(struct wiphy *wiphy, bcm_struct_cfgdev *cfgdev,
	struct ieee80211_channel *channel, unsigned int duration, u64 *cookie)
#else
static s32
wl_cfg80211_remain_on_channel(struct wiphy *wiphy, bcm_struct_cfgdev *cfgdev,
	struct ieee80211_channel * channel,
	enum nl80211_channel_type channel_type,
	unsigned int duration, u64 *cookie)
#endif /* WL_CFG80211_P2P_DEV_IF */
{
	s32 target_channel;
	u32 id;
	s32 err = BCME_OK;
	struct ether_addr primary_mac;
	struct net_device *ndev = NULL;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);

	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	WL_DBG(("Enter, channel: %d, duration ms (%d) SCANNING ?? %s \n",
		ieee80211_frequency_to_channel(channel->center_freq),
		duration, (wl_get_drv_status(cfg, SCANNING, ndev)) ? "YES":"NO"));

	if (!cfg->p2p) {
		WL_ERR(("cfg->p2p is not initialized\n"));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		err = BCME_ERROR;
		goto exit;
	}

#ifndef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
<<<<<<< HEAD
	if (wl_get_drv_status(wl, SCANNING, ndev)) {
		wl_notify_escan_complete(wl, ndev, true, true);
=======
	if (wl_get_drv_status_all(cfg, SCANNING)) {
		wl_notify_escan_complete(cfg, cfg->escan_info.ndev, true, true);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
#endif /* not WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */

	target_channel = ieee80211_frequency_to_channel(channel->center_freq);
<<<<<<< HEAD
	memcpy(&wl->remain_on_chan, channel, sizeof(struct ieee80211_channel));
	wl->remain_on_chan_type = channel_type;
	id = ++wl->last_roc_id;
	if (id == 0)
		id = ++wl->last_roc_id;
	*cookie = id;

#ifdef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
	if (wl_get_drv_status(wl, SCANNING, ndev)) {
		struct timer_list *_timer;
		WL_DBG(("scan is running. go to fake listen state\n"));

		wl_set_drv_status(wl, FAKE_REMAINING_ON_CHANNEL, ndev);

		if (timer_pending(&wl->p2p->listen_timer)) {
			WL_DBG(("cancel current listen timer \n"));
			del_timer_sync(&wl->p2p->listen_timer);
		}

		_timer = &wl->p2p->listen_timer;
		wl_clr_p2p_status(wl, LISTEN_EXPIRED);
=======
	memcpy(&cfg->remain_on_chan, channel, sizeof(struct ieee80211_channel));
#if defined(WL_ENABLE_P2P_IF)
	cfg->remain_on_chan_type = channel_type;
#endif /* WL_ENABLE_P2P_IF */
	id = ++cfg->last_roc_id;
	if (id == 0)
		id = ++cfg->last_roc_id;
	*cookie = id;

#ifdef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
	if (wl_get_drv_status(cfg, SCANNING, ndev)) {
		struct timer_list *_timer;
		WL_DBG(("scan is running. go to fake listen state\n"));

		wl_set_drv_status(cfg, FAKE_REMAINING_ON_CHANNEL, ndev);

		if (timer_pending(&cfg->p2p->listen_timer)) {
			WL_DBG(("cancel current listen timer \n"));
			del_timer_sync(&cfg->p2p->listen_timer);
		}

		_timer = &cfg->p2p->listen_timer;
		wl_clr_p2p_status(cfg, LISTEN_EXPIRED);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		INIT_TIMER(_timer, wl_cfgp2p_listen_expired, duration, 0);

		err = BCME_OK;
		goto exit;
	}
#endif /* WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */

#ifdef WL_CFG80211_SYNC_GON
<<<<<<< HEAD
	if (wl_get_drv_status_all(wl, WAITING_NEXT_ACT_FRM_LISTEN)) {
=======
	if (wl_get_drv_status_all(cfg, WAITING_NEXT_ACT_FRM_LISTEN)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* do not enter listen mode again if we are in listen mode already for next af.
		 * remain on channel completion will be returned by waiting next af completion.
		 */
#ifdef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
<<<<<<< HEAD
		wl_set_drv_status(wl, FAKE_REMAINING_ON_CHANNEL, ndev);
#else
		wl_set_drv_status(wl, REMAINING_ON_CHANNEL, ndev);
=======
		wl_set_drv_status(cfg, FAKE_REMAINING_ON_CHANNEL, ndev);
#else
		wl_set_drv_status(cfg, REMAINING_ON_CHANNEL, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#endif /* WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */
		goto exit;
	}
#endif /* WL_CFG80211_SYNC_GON */
<<<<<<< HEAD
	if (wl->p2p && !wl->p2p->on) {
		/* In case of p2p_listen command, supplicant send remain_on_channel
		 * without turning on P2P
		 */
		get_primary_mac(wl, &primary_mac);
		wl_cfgp2p_generate_bss_mac(&primary_mac, &wl->p2p->dev_addr, &wl->p2p->int_addr);
		p2p_on(wl) = true;
	}

	if (p2p_is_on(wl)) {
		err = wl_cfgp2p_enable_discovery(wl, ndev, NULL, 0);
=======
	if (cfg->p2p && !cfg->p2p->on) {
		/* In case of p2p_listen command, supplicant send remain_on_channel
		 * without turning on P2P
		 */
		get_primary_mac(cfg, &primary_mac);
		wl_cfgp2p_generate_bss_mac(&primary_mac, &cfg->p2p->dev_addr, &cfg->p2p->int_addr);
		p2p_on(cfg) = true;
	}

	if (p2p_is_on(cfg)) {
		err = wl_cfgp2p_enable_discovery(cfg, ndev, NULL, 0);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (unlikely(err)) {
			goto exit;
		}
#ifndef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
<<<<<<< HEAD
		wl_set_drv_status(wl, REMAINING_ON_CHANNEL, ndev);
#endif /* not WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */
		err = wl_cfgp2p_discover_listen(wl, target_channel, duration);

#ifdef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
		if (err == BCME_OK) {
			wl_set_drv_status(wl, REMAINING_ON_CHANNEL, ndev);
=======
		wl_set_drv_status(cfg, REMAINING_ON_CHANNEL, ndev);
#endif /* not WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */
		err = wl_cfgp2p_discover_listen(cfg, target_channel, duration);

#ifdef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
		if (err == BCME_OK) {
			wl_set_drv_status(cfg, REMAINING_ON_CHANNEL, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		} else {
			/* if failed, firmware may be internal scanning state.
			 * so other scan request shall not abort it
			 */
<<<<<<< HEAD
			wl_set_drv_status(wl, FAKE_REMAINING_ON_CHANNEL, ndev);
=======
			wl_set_drv_status(cfg, FAKE_REMAINING_ON_CHANNEL, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
#endif /* WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */
		/* WAR: set err = ok to prevent cookie mismatch in wpa_supplicant
		 * and expire timer will send a completion to the upper layer
		 */
		err = BCME_OK;
	}

exit:
	if (err == BCME_OK) {
		WL_INFO(("Success\n"));
<<<<<<< HEAD
		cfg80211_ready_on_channel(dev, *cookie, channel,
			channel_type, duration, GFP_KERNEL);
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
		cfg80211_ready_on_channel(cfgdev, *cookie, channel,
			duration, GFP_KERNEL);
#else
		cfg80211_ready_on_channel(cfgdev, *cookie, channel,
			channel_type, duration, GFP_KERNEL);
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	} else {
		WL_ERR(("Fail to Set (err=%d cookie:%llu)\n", err, *cookie));
	}
	return err;
}

static s32
<<<<<<< HEAD
wl_cfg80211_cancel_remain_on_channel(struct wiphy *wiphy, struct net_device *dev,
	u64 cookie)
{
	s32 err = 0;
	WL_DBG((" enter ) netdev_ifidx: %d \n", dev->ifindex));
=======
wl_cfg80211_cancel_remain_on_channel(struct wiphy *wiphy,
	bcm_struct_cfgdev *cfgdev, u64 cookie)
{
	s32 err = 0;

#if defined(WL_CFG80211_P2P_DEV_IF)
	if (cfgdev->iftype == NL80211_IFTYPE_P2P_DEVICE) {
		WL_DBG((" enter ) on P2P dedicated discover interface\n"));
	}
#else
	WL_DBG((" enter ) netdev_ifidx: %d \n", cfgdev->ifindex));
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}

static void
wl_cfg80211_afx_handler(struct work_struct *work)
{
	struct afx_hdl *afx_instance;
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
	s32 ret = BCME_OK;

	afx_instance = container_of(work, struct afx_hdl, work);
	if (afx_instance != NULL && wl->afx_hdl->is_active) {
		if (wl->afx_hdl->is_listen && wl->afx_hdl->my_listen_chan) {
			ret = wl_cfgp2p_discover_listen(wl, wl->afx_hdl->my_listen_chan,
				(100 * (1 + (random32() % 3)))); /* 100ms ~ 300ms */
		} else {
			ret = wl_cfgp2p_act_frm_search(wl, wl->afx_hdl->dev,
				wl->afx_hdl->bssidx, wl->afx_hdl->peer_listen_chan);
		}
		if (unlikely(ret != BCME_OK)) {
			WL_ERR(("ERROR occurred! returned value is (%d)\n", ret));
			if (wl_get_drv_status_all(wl, FINDING_COMMON_CHANNEL))
				complete(&wl->act_frm_scan);
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	s32 ret = BCME_OK;

	afx_instance = container_of(work, struct afx_hdl, work);
	if (afx_instance != NULL && cfg->afx_hdl->is_active) {
		if (cfg->afx_hdl->is_listen && cfg->afx_hdl->my_listen_chan) {
			ret = wl_cfgp2p_discover_listen(cfg, cfg->afx_hdl->my_listen_chan,
				(100 * (1 + (RANDOM32() % 3)))); /* 100ms ~ 300ms */
		} else {
			ret = wl_cfgp2p_act_frm_search(cfg, cfg->afx_hdl->dev,
				cfg->afx_hdl->bssidx, cfg->afx_hdl->peer_listen_chan,
				NULL);
		}
		if (unlikely(ret != BCME_OK)) {
			WL_ERR(("ERROR occurred! returned value is (%d)\n", ret));
			if (wl_get_drv_status_all(cfg, FINDING_COMMON_CHANNEL))
				complete(&cfg->act_frm_scan);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	}
}

static s32
<<<<<<< HEAD
wl_cfg80211_af_searching_channel(struct wl_priv *wl, struct net_device *dev)
{
	u32 max_retry = WL_CHANNEL_SYNC_RETRY;
=======
wl_cfg80211_af_searching_channel(struct bcm_cfg80211 *cfg, struct net_device *dev)
{
	u32 max_retry = WL_CHANNEL_SYNC_RETRY;
#ifdef CUSTOMER_HW4
	bool is_p2p_gas = false;
#endif /* CUSTOMER_HW4 */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (dev == NULL)
		return -1;

	WL_DBG((" enter ) \n"));

<<<<<<< HEAD
	wl_set_drv_status(wl, FINDING_COMMON_CHANNEL, dev);
	wl->afx_hdl->is_active = TRUE;
=======
	wl_set_drv_status(cfg, FINDING_COMMON_CHANNEL, dev);
	cfg->afx_hdl->is_active = TRUE;
#ifdef CUSTOMER_HW4
	if (cfg->afx_hdl->pending_tx_act_frm) {
		wl_action_frame_t *action_frame;
		action_frame = &(cfg->afx_hdl->pending_tx_act_frm->action_frame);
		if (wl_cfgp2p_is_p2p_gas_action(action_frame->data, action_frame->len))
			is_p2p_gas = true;
	}
#endif /* CUSTOMER_HW4 */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	/* Loop to wait until we find a peer's channel or the
	 * pending action frame tx is cancelled.
	 */
<<<<<<< HEAD
	while ((wl->afx_hdl->retry < max_retry) &&
		(wl->afx_hdl->peer_chan == WL_INVALID)) {
		wl->afx_hdl->is_listen = FALSE;
		wl_set_drv_status(wl, SCANNING, dev);
		WL_DBG(("Scheduling the action frame for sending.. retry %d\n",
			wl->afx_hdl->retry));
		/* search peer on peer's listen channel */
		schedule_work(&wl->afx_hdl->work);
		wait_for_completion_timeout(&wl->act_frm_scan,
			msecs_to_jiffies(MAX_WAIT_TIME));

		if ((wl->afx_hdl->peer_chan != WL_INVALID) ||
			!(wl_get_drv_status(wl, FINDING_COMMON_CHANNEL, dev)))
			break;

		if (wl->afx_hdl->my_listen_chan) {
			WL_DBG(("Scheduling Listen peer in my listen channel = %d\n",
				wl->afx_hdl->my_listen_chan));
			/* listen on my listen channel */
			wl->afx_hdl->is_listen = TRUE;
			schedule_work(&wl->afx_hdl->work);
			wait_for_completion_timeout(&wl->act_frm_scan,
				msecs_to_jiffies(MAX_WAIT_TIME));
		}
		if (!wl_get_drv_status(wl, FINDING_COMMON_CHANNEL, dev))
			break;
		wl->afx_hdl->retry++;

		WL_AF_TX_KEEP_PRI_CONNECTION_VSDB(wl);
	}

	wl->afx_hdl->is_active = FALSE;

	wl_clr_drv_status(wl, SCANNING, dev);
	wl_clr_drv_status(wl, FINDING_COMMON_CHANNEL, dev);

	return (wl->afx_hdl->peer_chan);
=======
	while ((cfg->afx_hdl->retry < max_retry) &&
		(cfg->afx_hdl->peer_chan == WL_INVALID)) {
		cfg->afx_hdl->is_listen = FALSE;
		wl_set_drv_status(cfg, SCANNING, dev);
		WL_DBG(("Scheduling the action frame for sending.. retry %d\n",
			cfg->afx_hdl->retry));
		/* search peer on peer's listen channel */
		schedule_work(&cfg->afx_hdl->work);
		wait_for_completion_timeout(&cfg->act_frm_scan,
			msecs_to_jiffies(WL_AF_SEARCH_TIME_MAX));

		if ((cfg->afx_hdl->peer_chan != WL_INVALID) ||
			!(wl_get_drv_status(cfg, FINDING_COMMON_CHANNEL, dev)))
			break;
#ifdef CUSTOMER_HW4
		if (is_p2p_gas)
			break;
#endif /* CUSTOMER_HW4 */

		if (cfg->afx_hdl->my_listen_chan) {
			WL_DBG(("Scheduling Listen peer in my listen channel = %d\n",
				cfg->afx_hdl->my_listen_chan));
			/* listen on my listen channel */
			cfg->afx_hdl->is_listen = TRUE;
			schedule_work(&cfg->afx_hdl->work);
			wait_for_completion_timeout(&cfg->act_frm_scan,
				msecs_to_jiffies(WL_AF_SEARCH_TIME_MAX));
		}
		if ((cfg->afx_hdl->peer_chan != WL_INVALID) ||
			!(wl_get_drv_status(cfg, FINDING_COMMON_CHANNEL, dev)))
			break;

		cfg->afx_hdl->retry++;

		WL_AF_TX_KEEP_PRI_CONNECTION_VSDB(cfg);
	}

	cfg->afx_hdl->is_active = FALSE;

	wl_clr_drv_status(cfg, SCANNING, dev);
	wl_clr_drv_status(cfg, FINDING_COMMON_CHANNEL, dev);

	return (cfg->afx_hdl->peer_chan);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

struct p2p_config_af_params {
	s32 max_tx_retry;	/* max tx retry count if tx no ack */
	/* To make sure to send successfully action frame, we have to turn off mpc
	 * 0: off, 1: on,  (-1): do nothing
	 */
	s32 mpc_onoff;
<<<<<<< HEAD
=======
#ifdef WL_CFG80211_GON_COLLISION
	/* drop tx go nego request if go nego collision occurs */
	bool drop_tx_req;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#ifdef WL_CFG80211_SYNC_GON
	bool extra_listen;
#endif
	bool search_channel;	/* 1: search peer's channel to send af */
};

static s32
wl_cfg80211_config_p2p_pub_af_tx(struct wiphy *wiphy,
	wl_action_frame_t *action_frame, wl_af_params_t *af_params,
	struct p2p_config_af_params *config_af_params)
{
	s32 err = BCME_OK;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	wifi_p2p_pub_act_frame_t *act_frm =
		(wifi_p2p_pub_act_frame_t *) (action_frame->data);

	/* initialize default value */
<<<<<<< HEAD
=======
#ifdef WL_CFG80211_GON_COLLISION
	config_af_params->drop_tx_req = false;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#ifdef WL_CFG80211_SYNC_GON
	config_af_params->extra_listen = true;
#endif
	config_af_params->search_channel = false;
	config_af_params->max_tx_retry = WL_AF_TX_MAX_RETRY;
	config_af_params->mpc_onoff = -1;
<<<<<<< HEAD
=======
	cfg->next_af_subtype = P2P_PAF_SUBTYPE_INVALID;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	switch (act_frm->subtype) {
	case P2P_PAF_GON_REQ: {
		WL_DBG(("P2P: GO_NEG_PHASE status set \n"));
<<<<<<< HEAD
		wl_set_p2p_status(wl, GO_NEG_PHASE);

		config_af_params->mpc_onoff = 0;
		config_af_params->search_channel = true;
		wl->next_af_subtype = act_frm->subtype + 1;
=======
		wl_set_p2p_status(cfg, GO_NEG_PHASE);

		config_af_params->mpc_onoff = 0;
		config_af_params->search_channel = true;
		cfg->next_af_subtype = act_frm->subtype + 1;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		/* increase dwell time to wait for RESP frame */
		af_params->dwell_time = WL_MED_DWELL_TIME;

<<<<<<< HEAD
		break;
	}
	case P2P_PAF_GON_RSP: {
		wl->next_af_subtype = act_frm->subtype + 1;
		/* increase dwell time to wait for CONF frame */
		af_params->dwell_time = WL_MED_DWELL_TIME;
=======
#ifdef WL_CFG80211_GON_COLLISION
		config_af_params->drop_tx_req = true;
#endif /* WL_CFG80211_GON_COLLISION */
		break;
	}
	case P2P_PAF_GON_RSP: {
		cfg->next_af_subtype = act_frm->subtype + 1;
		/* increase dwell time to wait for CONF frame */
		af_params->dwell_time = WL_MED_DWELL_TIME + 100;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		break;
	}
	case P2P_PAF_GON_CONF: {
		/* If we reached till GO Neg confirmation reset the filter */
		WL_DBG(("P2P: GO_NEG_PHASE status cleared \n"));
<<<<<<< HEAD
		wl_clr_p2p_status(wl, GO_NEG_PHASE);
=======
		wl_clr_p2p_status(cfg, GO_NEG_PHASE);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		/* turn on mpc again if go nego is done */
		config_af_params->mpc_onoff = 1;

		/* minimize dwell time */
		af_params->dwell_time = WL_MIN_DWELL_TIME;

<<<<<<< HEAD
=======
#ifdef WL_CFG80211_GON_COLLISION
		/* if go nego formation done, clear it */
		cfg->block_gon_req_tx_count = 0;
		cfg->block_gon_req_rx_count = 0;
#endif /* WL_CFG80211_GON_COLLISION */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#ifdef WL_CFG80211_SYNC_GON
		config_af_params->extra_listen = false;
#endif /* WL_CFG80211_SYNC_GON */
		break;
	}
	case P2P_PAF_INVITE_REQ: {
		config_af_params->search_channel = true;
<<<<<<< HEAD
		wl->next_af_subtype = act_frm->subtype + 1;
=======
		cfg->next_af_subtype = act_frm->subtype + 1;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		/* increase dwell time */
		af_params->dwell_time = WL_MED_DWELL_TIME;
		break;
	}
	case P2P_PAF_INVITE_RSP:
		/* minimize dwell time */
		af_params->dwell_time = WL_MIN_DWELL_TIME;
#ifdef WL_CFG80211_SYNC_GON
		config_af_params->extra_listen = false;
#endif /* WL_CFG80211_SYNC_GON */
		break;
	case P2P_PAF_DEVDIS_REQ: {
<<<<<<< HEAD
		config_af_params->search_channel = true;

		wl->next_af_subtype = act_frm->subtype + 1;
=======
		if (IS_ACTPUB_WITHOUT_GROUP_ID(&act_frm->elts[0],
			action_frame->len)) {
			config_af_params->search_channel = true;
		}

		cfg->next_af_subtype = act_frm->subtype + 1;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* maximize dwell time to wait for RESP frame */
		af_params->dwell_time = WL_LONG_DWELL_TIME;
		break;
	}
	case P2P_PAF_DEVDIS_RSP:
		/* minimize dwell time */
		af_params->dwell_time = WL_MIN_DWELL_TIME;
#ifdef WL_CFG80211_SYNC_GON
		config_af_params->extra_listen = false;
#endif /* WL_CFG80211_SYNC_GON */
		break;
	case P2P_PAF_PROVDIS_REQ: {
<<<<<<< HEAD
		if (IS_PROV_DISC_WITHOUT_GROUP_ID(&act_frm->elts[0],
=======
		if (IS_ACTPUB_WITHOUT_GROUP_ID(&act_frm->elts[0],
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			action_frame->len)) {
			config_af_params->search_channel = true;
		}

		config_af_params->mpc_onoff = 0;
<<<<<<< HEAD
		wl->next_af_subtype = act_frm->subtype + 1;
=======
		cfg->next_af_subtype = act_frm->subtype + 1;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* increase dwell time to wait for RESP frame */
		af_params->dwell_time = WL_MED_DWELL_TIME;
		break;
	}
	case P2P_PAF_PROVDIS_RSP: {
<<<<<<< HEAD
		wl->next_af_subtype = P2P_PAF_GON_REQ;
		/* increase dwell time to MED level */
		af_params->dwell_time = WL_MED_DWELL_TIME;
=======
		cfg->next_af_subtype = P2P_PAF_GON_REQ;
		af_params->dwell_time = WL_MIN_DWELL_TIME;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#ifdef WL_CFG80211_SYNC_GON
		config_af_params->extra_listen = false;
#endif /* WL_CFG80211_SYNC_GON */
		break;
	}
	default:
		WL_DBG(("Unknown p2p pub act frame subtype: %d\n",
			act_frm->subtype));
		err = BCME_BADARG;
	}
	return err;
}

<<<<<<< HEAD

static bool
wl_cfg80211_send_action_frame(struct wiphy *wiphy, struct net_device *dev,
	struct net_device *ndev, wl_af_params_t *af_params,
	wl_action_frame_t *action_frame, u16 action_frame_len, s32 bssidx)
{
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
#ifdef WL11U
static bool
wl_cfg80211_check_DFS_channel(struct bcm_cfg80211 *cfg, wl_af_params_t *af_params,
	void *frame, u16 frame_len)
{
	struct wl_scan_results *bss_list;
	struct wl_bss_info *bi = NULL;
	bool result = false;
	s32 i;

	/* If DFS channel is 52~148, check to block it or not */
	if (af_params &&
		(af_params->channel >= 52 && af_params->channel <= 148)) {
		if (!wl_cfgp2p_is_p2p_action(frame, frame_len)) {
			bss_list = cfg->bss_list;
			bi = next_bss(bss_list, bi);
			for_each_bss(bss_list, bi, i) {
				if (CHSPEC_IS5G(bi->chanspec) &&
					((bi->ctl_ch ? bi->ctl_ch : CHSPEC_CHANNEL(bi->chanspec))
					== af_params->channel)) {
					result = true;	/* do not block the action frame */
					break;
				}
			}
		}
	}
	else {
		result = true;
	}

	WL_DBG(("result=%s", result?"true":"false"));
	return result;
}
#endif /* WL11U */


static bool
wl_cfg80211_send_action_frame(struct wiphy *wiphy, struct net_device *dev,
	bcm_struct_cfgdev *cfgdev, wl_af_params_t *af_params,
	wl_action_frame_t *action_frame, u16 action_frame_len, s32 bssidx)
{
#ifdef WL11U
	struct net_device *ndev = NULL;
#endif /* WL11U */
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	bool ack = false;
	u8 category, action;
	s32 tx_retry;
	struct p2p_config_af_params config_af_params;
#ifdef VSDB
	ulong off_chan_started_jiffies = 0;
#endif
<<<<<<< HEAD
	dhd_pub_t *dhd = (dhd_pub_t *)(wl->pub);

	wl_cfgp2p_print_actframe(true, action_frame->data, action_frame->len);
=======
	dhd_pub_t *dhd = (dhd_pub_t *)(cfg->pub);
#ifdef WL11U
#if defined(WL_CFG80211_P2P_DEV_IF)
	ndev = dev;
#else
	ndev = ndev_to_cfgdev(cfgdev);
#endif /* WL_CFG80211_P2P_DEV_IF */
#endif /* WL11U */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	category = action_frame->data[DOT11_ACTION_CAT_OFF];
	action = action_frame->data[DOT11_ACTION_ACT_OFF];

	/* initialize variables */
	tx_retry = 0;
<<<<<<< HEAD
	wl->next_af_subtype = P2P_PAF_SUBTYPE_INVALID;
	config_af_params.max_tx_retry = WL_AF_TX_MAX_RETRY;
	config_af_params.mpc_onoff = -1;
	config_af_params.search_channel = false;
=======
	cfg->next_af_subtype = P2P_PAF_SUBTYPE_INVALID;
	config_af_params.max_tx_retry = WL_AF_TX_MAX_RETRY;
	config_af_params.mpc_onoff = -1;
	config_af_params.search_channel = false;
#ifdef WL_CFG80211_GON_COLLISION
	config_af_params.drop_tx_req = false;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#ifdef WL_CFG80211_SYNC_GON
	config_af_params.extra_listen = false;
#endif

	/* config parameters */
	/* Public Action Frame Process - DOT11_ACTION_CAT_PUBLIC */
	if (category == DOT11_ACTION_CAT_PUBLIC) {
		if ((action == P2P_PUB_AF_ACTION) &&
			(action_frame_len >= sizeof(wifi_p2p_pub_act_frame_t))) {
			/* p2p public action frame process */
			if (BCME_OK != wl_cfg80211_config_p2p_pub_af_tx(wiphy,
				action_frame, af_params, &config_af_params)) {
				WL_DBG(("Unknown subtype.\n"));
			}

<<<<<<< HEAD
=======
#ifdef WL_CFG80211_GON_COLLISION
			if (config_af_params.drop_tx_req) {
				if (cfg->block_gon_req_tx_count) {
					/* drop gon req tx action frame */
					WL_DBG(("Drop gon req tx action frame: count %d\n",
						cfg->block_gon_req_tx_count));
					goto exit;
				}
			}
#endif /* WL_CFG80211_GON_COLLISION */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		} else if (action_frame_len >= sizeof(wifi_p2psd_gas_pub_act_frame_t)) {
			/* service discovery process */
			if (action == P2PSD_ACTION_ID_GAS_IREQ ||
				action == P2PSD_ACTION_ID_GAS_CREQ) {
				/* configure service discovery query frame */

				config_af_params.search_channel = true;

				/* save next af suptype to cancel remained dwell time */
<<<<<<< HEAD
				wl->next_af_subtype = action + 1;
=======
				cfg->next_af_subtype = action + 1;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

				af_params->dwell_time = WL_MED_DWELL_TIME;
			} else if (action == P2PSD_ACTION_ID_GAS_IRESP ||
				action == P2PSD_ACTION_ID_GAS_CRESP) {
				/* configure service discovery response frame */
				af_params->dwell_time = WL_MIN_DWELL_TIME;
			} else {
				WL_DBG(("Unknown action type: %d\n", action));
			}
		} else {
			WL_DBG(("Unknown Frame: category 0x%x, action 0x%x, length %d\n",
				category, action, action_frame_len));
<<<<<<< HEAD
		}
=======
	}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	} else if (category == P2P_AF_CATEGORY) {
		/* do not configure anything. it will be sent with a default configuration */
	} else {
		WL_DBG(("Unknown Frame: category 0x%x, action 0x%x\n",
			category, action));
		if (dhd->op_mode & DHD_FLAG_HOSTAP_MODE) {
<<<<<<< HEAD
			wl_clr_drv_status(wl, SENDING_ACT_FRM, dev);
=======
			wl_clr_drv_status(cfg, SENDING_ACT_FRM, dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			return false;
		}
	}

	/* To make sure to send successfully action frame, we have to turn off mpc */
	if (config_af_params.mpc_onoff == 0) {
		wldev_iovar_setint(dev, "mpc", 0);
	}

	/* validate channel and p2p ies */
	if (config_af_params.search_channel && IS_P2P_SOCIAL(af_params->channel) &&
<<<<<<< HEAD
		wl_to_p2p_bss_saved_ie(wl, P2PAPI_BSSCFG_DEVICE).p2p_probe_req_ie_len) {
=======
		wl_to_p2p_bss_saved_ie(cfg, P2PAPI_BSSCFG_DEVICE).p2p_probe_req_ie_len) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		config_af_params.search_channel = true;
	} else {
		config_af_params.search_channel = false;
	}
<<<<<<< HEAD

#ifdef WL11U
	if (ndev == wl_to_prmry_ndev(wl))
=======
#ifdef WL11U
	if (ndev == bcmcfg_to_prmry_ndev(cfg))
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		config_af_params.search_channel = false;
#endif /* WL11U */

#ifdef VSDB
	/* if connecting on primary iface, sleep for a while before sending af tx for VSDB */
<<<<<<< HEAD
	if (wl_get_drv_status(wl, CONNECTING, wl_to_prmry_ndev(wl))) {
		msleep(50);
=======
	if (wl_get_drv_status(cfg, CONNECTING, bcmcfg_to_prmry_ndev(cfg))) {
		OSL_SLEEP(50);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
#endif

	/* if scan is ongoing, abort current scan. */
<<<<<<< HEAD
	if (wl_get_drv_status_all(wl, SCANNING)) {
		wl_notify_escan_complete(wl, ndev, true, true);
	}

	/* set status and destination address before sending af */
	if (wl->next_af_subtype != P2P_PAF_SUBTYPE_INVALID) {
		/* set this status to cancel the remained dwell time in rx process */
		wl_set_drv_status(wl, WAITING_NEXT_ACT_FRM, dev);
	}
	wl_set_drv_status(wl, SENDING_ACT_FRM, dev);
	memcpy(wl->afx_hdl->tx_dst_addr.octet,
		af_params->action_frame.da.octet,
		sizeof(wl->afx_hdl->tx_dst_addr.octet));

	/* save af_params for rx process */
	wl->afx_hdl->pending_tx_act_frm = af_params;
=======
	if (wl_get_drv_status_all(cfg, SCANNING)) {
		wl_notify_escan_complete(cfg, cfg->escan_info.ndev, true, true);
	}

#ifdef WL11U
	/* handling DFS channel exceptions */
	if (!wl_cfg80211_check_DFS_channel(cfg, af_params, action_frame->data, action_frame->len)) {
		return false;	/* the action frame was blocked */
	}
#endif /* WL11U */

	/* set status and destination address before sending af */
	if (cfg->next_af_subtype != P2P_PAF_SUBTYPE_INVALID) {
		/* set this status to cancel the remained dwell time in rx process */
		wl_set_drv_status(cfg, WAITING_NEXT_ACT_FRM, dev);
	}
	wl_set_drv_status(cfg, SENDING_ACT_FRM, dev);
	memcpy(cfg->afx_hdl->tx_dst_addr.octet,
		af_params->action_frame.da.octet,
		sizeof(cfg->afx_hdl->tx_dst_addr.octet));

	/* save af_params for rx process */
	cfg->afx_hdl->pending_tx_act_frm = af_params;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	/* search peer's channel */
	if (config_af_params.search_channel) {
		/* initialize afx_hdl */
<<<<<<< HEAD
		wl->afx_hdl->bssidx = wl_cfgp2p_find_idx(wl, dev);
		wl->afx_hdl->dev = dev;
		wl->afx_hdl->retry = 0;
		wl->afx_hdl->peer_chan = WL_INVALID;

		if (wl_cfg80211_af_searching_channel(wl, dev) == WL_INVALID) {
			WL_ERR(("couldn't find peer's channel.\n"));
			goto exit;
		}

		/* Suspend P2P discovery's search-listen to prevent it from
		 * starting a scan or changing the channel.
		 */
		wl_clr_drv_status(wl, SCANNING, wl->afx_hdl->dev);
/* Do not abort scan for VSDB. Scan will be aborted in firmware if necessary */
#ifndef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
		wl_notify_escan_complete(wl, dev, true, true);
#endif /* not WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */
		wl_cfgp2p_discover_enable_search(wl, false);

		/* update channel */
		af_params->channel = wl->afx_hdl->peer_chan;
=======
		if (wl_cfgp2p_find_idx(cfg, dev, &cfg->afx_hdl->bssidx) != BCME_OK) {
			WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
			goto exit;
		}
		cfg->afx_hdl->dev = dev;
		cfg->afx_hdl->retry = 0;
		cfg->afx_hdl->peer_chan = WL_INVALID;

		if (wl_cfg80211_af_searching_channel(cfg, dev) == WL_INVALID) {
			WL_ERR(("couldn't find peer's channel.\n"));
			wl_cfgp2p_print_actframe(true, action_frame->data, action_frame->len,
				af_params->channel);
			goto exit;
		}

		wl_clr_drv_status(cfg, SCANNING, cfg->afx_hdl->dev);
		/*
		 * Abort scan even for VSDB scenarios. Scan gets aborted in firmware
		 * but after the check of piggyback algorithm.
		 * To take care of current piggback algo, lets abort the scan here itself.
		 */
		wl_notify_escan_complete(cfg, dev, true, true);
		/* Suspend P2P discovery's search-listen to prevent it from
		 * starting a scan or changing the channel.
		 */
		wl_cfgp2p_discover_enable_search(cfg, false);

		/* update channel */
		af_params->channel = cfg->afx_hdl->peer_chan;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

#ifdef VSDB
	off_chan_started_jiffies = jiffies;
#endif /* VSDB */

<<<<<<< HEAD
	/* Now send a tx action frame */
	ack = wl_cfgp2p_tx_action_frame(wl, dev, af_params, bssidx) ? false : true;
=======
	wl_cfgp2p_print_actframe(true, action_frame->data, action_frame->len, af_params->channel);

	/* Now send a tx action frame */
	ack = wl_cfgp2p_tx_action_frame(cfg, dev, af_params, bssidx) ? false : true;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	/* if failed, retry it. tx_retry_max value is configure by .... */
	while ((ack == false) && (tx_retry++ < config_af_params.max_tx_retry)) {
#ifdef VSDB
		if (af_params->channel) {
			if (jiffies_to_msecs(jiffies - off_chan_started_jiffies) >
				OFF_CHAN_TIME_THRESHOLD_MS) {
<<<<<<< HEAD
				WL_AF_TX_KEEP_PRI_CONNECTION_VSDB(wl);
				off_chan_started_jiffies = jiffies;
			}
		}
#endif /* VSDB */
		ack = wl_cfgp2p_tx_action_frame(wl, dev, af_params, bssidx) ?
			false : true;
	}
	if (ack == false) {
		WL_ERR(("Failed to send Action Frame(retry %d)\n", tx_retry));
	}
exit:
	/* Clear SENDING_ACT_FRM after all sending af is done */
	wl_clr_drv_status(wl, SENDING_ACT_FRM, dev);
=======
				WL_AF_TX_KEEP_PRI_CONNECTION_VSDB(cfg);
				off_chan_started_jiffies = jiffies;
			} else
				OSL_SLEEP(AF_RETRY_DELAY_TIME);
		}
#endif /* VSDB */
		ack = wl_cfgp2p_tx_action_frame(cfg, dev, af_params, bssidx) ?
			false : true;
	}

	if (ack == false) {
		WL_ERR(("Failed to send Action Frame(retry %d)\n", tx_retry));
	}
	WL_DBG(("Complete to send action frame\n"));
exit:
	/* Clear SENDING_ACT_FRM after all sending af is done */
	wl_clr_drv_status(cfg, SENDING_ACT_FRM, dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#ifdef WL_CFG80211_SYNC_GON
	/* WAR: sometimes dongle does not keep the dwell time of 'actframe'.
	 * if we coundn't get the next action response frame and dongle does not keep
	 * the dwell time, go to listen state again to get next action response frame.
	 */
	if (ack && config_af_params.extra_listen &&
<<<<<<< HEAD
		wl_get_drv_status_all(wl, WAITING_NEXT_ACT_FRM) &&
		wl->af_sent_channel == wl->afx_hdl->my_listen_chan) {
		s32 extar_listen_time;

		extar_listen_time = af_params->dwell_time -
			jiffies_to_msecs(jiffies - wl->af_tx_sent_jiffies);

		if (extar_listen_time > 50) {
			wl_set_drv_status(wl, WAITING_NEXT_ACT_FRM_LISTEN, dev);
			WL_DBG(("Wait more time! actual af time:%d,"
				"calculated extar listen:%d\n",
				af_params->dwell_time, extar_listen_time));
			if (wl_cfgp2p_discover_listen(wl, wl->af_sent_channel,
				extar_listen_time + 100) == BCME_OK) {
				wait_for_completion_timeout(&wl->wait_next_af,
					msecs_to_jiffies(extar_listen_time + 100 + 300));
			}
			wl_clr_drv_status(wl, WAITING_NEXT_ACT_FRM_LISTEN, dev);
		}
	}
#endif /* WL_CFG80211_SYNC_GON */
	wl_clr_drv_status(wl, WAITING_NEXT_ACT_FRM, dev);

	if (wl->afx_hdl->pending_tx_act_frm)
		wl->afx_hdl->pending_tx_act_frm = NULL;

	WL_INFO(("-- sending Action Frame is %s, listen chan: %d\n",
		(ack) ? "Succeeded!!":"Failed!!", wl->afx_hdl->my_listen_chan));

=======
#ifdef WL_CFG80211_GON_COLLISION
		!cfg->block_gon_req_tx_count &&
#endif /* WL_CFG80211_GON_COLLISION */
		wl_get_drv_status_all(cfg, WAITING_NEXT_ACT_FRM) &&
		cfg->af_sent_channel == cfg->afx_hdl->my_listen_chan) {
		s32 extar_listen_time;

		extar_listen_time = af_params->dwell_time -
			jiffies_to_msecs(jiffies - cfg->af_tx_sent_jiffies);

		if (extar_listen_time > 50) {
			wl_set_drv_status(cfg, WAITING_NEXT_ACT_FRM_LISTEN, dev);
			WL_DBG(("Wait more time! actual af time:%d,"
				"calculated extar listen:%d\n",
				af_params->dwell_time, extar_listen_time));
			if (wl_cfgp2p_discover_listen(cfg, cfg->af_sent_channel,
				extar_listen_time + 100) == BCME_OK) {
				wait_for_completion_timeout(&cfg->wait_next_af,
					msecs_to_jiffies(extar_listen_time + 100 + 300));
			}
			wl_clr_drv_status(cfg, WAITING_NEXT_ACT_FRM_LISTEN, dev);
		}
	}
#endif /* WL_CFG80211_SYNC_GON */
	wl_clr_drv_status(cfg, WAITING_NEXT_ACT_FRM, dev);

	if (cfg->afx_hdl->pending_tx_act_frm)
		cfg->afx_hdl->pending_tx_act_frm = NULL;

	WL_INFO(("-- sending Action Frame is %s, listen chan: %d\n",
		(ack) ? "Succeeded!!":"Failed!!", cfg->afx_hdl->my_listen_chan));

#ifdef WL_CFG80211_GON_COLLISION
	if (cfg->block_gon_req_tx_count) {
		cfg->block_gon_req_tx_count--;
		/* if ack is ture, supplicant will wait more time(100ms).
		 * so we will return it as a success to get more time .
		 */
		ack = true;
	}
#endif /* WL_CFG80211_GON_COLLISION */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	/* if all done, turn mpc on again */
	if (config_af_params.mpc_onoff == 1) {
		wldev_iovar_setint(dev, "mpc", 1);
	}

	return ack;
}

<<<<<<< HEAD
static s32
wl_cfg80211_mgmt_tx(struct wiphy *wiphy, struct net_device *ndev,
=======
#define MAX_NUM_OF_ASSOCIATED_DEV       64
#if defined(WL_CFG80211_P2P_DEV_IF)
static s32
wl_cfg80211_mgmt_tx(struct wiphy *wiphy, bcm_struct_cfgdev *cfgdev,
	struct ieee80211_channel *channel, bool offchan,
	unsigned int wait, const u8* buf, size_t len, bool no_cck,
	bool dont_wait_for_ack, u64 *cookie)
#else
static s32
wl_cfg80211_mgmt_tx(struct wiphy *wiphy, bcm_struct_cfgdev *cfgdev,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct ieee80211_channel *channel, bool offchan,
	enum nl80211_channel_type channel_type,
	bool channel_type_valid, unsigned int wait,
	const u8* buf, size_t len,
<<<<<<< HEAD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
	bool no_cck,
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)
	bool dont_wait_for_ack,
#endif
	u64 *cookie)
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)) || defined(WL_COMPAT_WIRELESS)
	bool no_cck,
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)) || defined(WL_COMPAT_WIRELESS)
	bool dont_wait_for_ack,
#endif
	u64 *cookie)
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	wl_action_frame_t *action_frame;
	wl_af_params_t *af_params;
	scb_val_t scb_val;
	const struct ieee80211_mgmt *mgmt;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct net_device *dev = NULL;
	s32 err = BCME_OK;
	s32 bssidx = 0;
	u32 id;
	bool ack = false;
	s8 eabuf[ETHER_ADDR_STR_LEN];

	WL_DBG(("Enter \n"));

<<<<<<< HEAD
	if (ndev == wl->p2p_net) {
		dev = wl_to_prmry_ndev(wl);
	} else {
		/* If TX req is for any valid ifidx. Use as is */
		dev = ndev;
	}

	/* find bssidx based on ndev */
	bssidx = wl_cfgp2p_find_idx(wl, dev);
	if (bssidx == -1) {

		WL_ERR(("Can not find the bssidx for dev( %p )\n", dev));
		return -ENODEV;
	}
	if (p2p_is_on(wl)) {
		/* Suspend P2P discovery search-listen to prevent it from changing the
		 * channel.
		 */
		if ((err = wl_cfgp2p_discover_enable_search(wl, false)) < 0) {
=======
	dev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	/* set bsscfg idx for iovar (wlan0: P2PAPI_BSSCFG_PRIMARY, p2p: P2PAPI_BSSCFG_DEVICE)	*/
	if (discover_cfgdev(cfgdev, cfg)) {
		bssidx = wl_to_p2p_bss_bssidx(cfg, P2PAPI_BSSCFG_DEVICE);
	}
	else {
		if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
			WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
			return BCME_ERROR;
		}
	}

	WL_DBG(("TX target bssidx=%d\n", bssidx));

	if (p2p_is_on(cfg)) {
		/* Suspend P2P discovery search-listen to prevent it from changing the
		 * channel.
		 */
		if ((err = wl_cfgp2p_discover_enable_search(cfg, false)) < 0) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_ERR(("Can not disable discovery mode\n"));
			return -EFAULT;
		}
	}
	*cookie = 0;
<<<<<<< HEAD
	id = wl->send_action_id++;
	if (id == 0)
		id = wl->send_action_id++;
=======
	id = cfg->send_action_id++;
	if (id == 0)
		id = cfg->send_action_id++;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	*cookie = id;
	mgmt = (const struct ieee80211_mgmt *)buf;
	if (ieee80211_is_mgmt(mgmt->frame_control)) {
		if (ieee80211_is_probe_resp(mgmt->frame_control)) {
			s32 ie_offset =  DOT11_MGMT_HDR_LEN + DOT11_BCN_PRB_FIXED_LEN;
			s32 ie_len = len - ie_offset;
<<<<<<< HEAD
			if (dev == wl_to_prmry_ndev(wl))
				bssidx = wl_to_p2p_bss_bssidx(wl, P2PAPI_BSSCFG_DEVICE);
			wl_cfgp2p_set_management_ie(wl, dev, bssidx,
				VNDR_IE_PRBRSP_FLAG, (u8 *)(buf + ie_offset), ie_len);
			cfg80211_mgmt_tx_status(ndev, *cookie, buf, len, true, GFP_KERNEL);
			goto exit;
		} else if (ieee80211_is_disassoc(mgmt->frame_control) ||
			ieee80211_is_deauth(mgmt->frame_control)) {
=======
			if (dev == bcmcfg_to_prmry_ndev(cfg))
				bssidx = wl_to_p2p_bss_bssidx(cfg, P2PAPI_BSSCFG_DEVICE);
				wl_cfgp2p_set_management_ie(cfg, dev, bssidx,
				VNDR_IE_PRBRSP_FLAG, (u8 *)(buf + ie_offset), ie_len);
			cfg80211_mgmt_tx_status(cfgdev, *cookie, buf, len, true, GFP_KERNEL);
			goto exit;
		} else if (ieee80211_is_disassoc(mgmt->frame_control) ||
			ieee80211_is_deauth(mgmt->frame_control)) {
			char mac_buf[MAX_NUM_OF_ASSOCIATED_DEV *
				sizeof(struct ether_addr) + sizeof(uint)] = {0};
			int num_associated = 0;
			struct maclist *assoc_maclist = (struct maclist *)mac_buf;
			if (!bcmp((const uint8 *)BSSID_BROADCAST,
				(const struct ether_addr *)mgmt->da, ETHER_ADDR_LEN)) {
				assoc_maclist->count = MAX_NUM_OF_ASSOCIATED_DEV;
				err = wldev_ioctl(dev, WLC_GET_ASSOCLIST,
					assoc_maclist, sizeof(mac_buf), false);
				if (err < 0)
					WL_ERR(("WLC_GET_ASSOCLIST error %d\n", err));
				else
					num_associated = assoc_maclist->count;
			}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			memcpy(scb_val.ea.octet, mgmt->da, ETH_ALEN);
			scb_val.val = mgmt->u.disassoc.reason_code;
			err = wldev_ioctl(dev, WLC_SCB_DEAUTHENTICATE_FOR_REASON, &scb_val,
				sizeof(scb_val_t), true);
			if (err < 0)
				WL_ERR(("WLC_SCB_DEAUTHENTICATE_FOR_REASON error %d\n", err));
<<<<<<< HEAD
			WL_DBG(("Disconnect STA : %s scb_val.val %d\n",
				bcm_ether_ntoa((const struct ether_addr *)mgmt->da, eabuf),
				scb_val.val));
			wl_delay(400);
			cfg80211_mgmt_tx_status(ndev, *cookie, buf, len, true, GFP_KERNEL);
=======
			WL_ERR(("Disconnect STA : %s scb_val.val %d\n",
				bcm_ether_ntoa((const struct ether_addr *)mgmt->da, eabuf),
				scb_val.val));

			if (num_associated > 0 && ETHER_ISBCAST(mgmt->da))
				wl_delay(400);

			cfg80211_mgmt_tx_status(cfgdev, *cookie, buf, len, true, GFP_KERNEL);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			goto exit;

		} else if (ieee80211_is_action(mgmt->frame_control)) {
			/* Abort the dwell time of any previous off-channel
			* action frame that may be still in effect.  Sending
			* off-channel action frames relies on the driver's
			* scan engine.  If a previous off-channel action frame
			* tx is still in progress (including the dwell time),
			* then this new action frame will not be sent out.
			*/
/* Do not abort scan for VSDB. Scan will be aborted in firmware if necessary.
 * And previous off-channel action frame must be ended before new af tx.
 */
#ifndef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
<<<<<<< HEAD
			wl_notify_escan_complete(wl, dev, true, true);
=======
			wl_notify_escan_complete(cfg, dev, true, true);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#endif /* not WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */
		}

	} else {
		WL_ERR(("Driver only allows MGMT packet type\n"));
		goto exit;
	}

	af_params = (wl_af_params_t *) kzalloc(WL_WIFI_AF_PARAMS_SIZE, GFP_KERNEL);

	if (af_params == NULL)
	{
		WL_ERR(("unable to allocate frame\n"));
		return -ENOMEM;
	}

	action_frame = &af_params->action_frame;

	/* Add the packet Id */
	action_frame->packetId = *cookie;
	WL_DBG(("action frame %d\n", action_frame->packetId));
	/* Add BSSID */
	memcpy(&action_frame->da, &mgmt->da[0], ETHER_ADDR_LEN);
	memcpy(&af_params->BSSID, &mgmt->bssid[0], ETHER_ADDR_LEN);

	/* Add the length exepted for 802.11 header  */
	action_frame->len = len - DOT11_MGMT_HDR_LEN;
	WL_DBG(("action_frame->len: %d\n", action_frame->len));

	/* Add the channel */
	af_params->channel =
		ieee80211_frequency_to_channel(channel->center_freq);

	/* Save listen_chan for searching common channel */
<<<<<<< HEAD
	wl->afx_hdl->peer_listen_chan = af_params->channel;
	WL_DBG(("channel from upper layer %d\n", wl->afx_hdl->peer_listen_chan));
=======
	cfg->afx_hdl->peer_listen_chan = af_params->channel;
	WL_DBG(("channel from upper layer %d\n", cfg->afx_hdl->peer_listen_chan));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	/* Add the default dwell time
	 * Dwell time to stay off-channel to wait for a response action frame
	 * after transmitting an GO Negotiation action frame
	 */
	af_params->dwell_time = WL_DWELL_TIME;

	memcpy(action_frame->data, &buf[DOT11_MGMT_HDR_LEN], action_frame->len);

<<<<<<< HEAD
	ack = wl_cfg80211_send_action_frame(wiphy, dev, ndev, af_params,
		action_frame, action_frame->len, bssidx);

	cfg80211_mgmt_tx_status(ndev, *cookie, buf, len, ack, GFP_KERNEL);
=======
	ack = wl_cfg80211_send_action_frame(wiphy, dev, cfgdev, af_params,
		action_frame, action_frame->len, bssidx);
	cfg80211_mgmt_tx_status(cfgdev, *cookie, buf, len, ack, GFP_KERNEL);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	kfree(af_params);
exit:
	return err;
}


static void
<<<<<<< HEAD
wl_cfg80211_mgmt_frame_register(struct wiphy *wiphy, struct net_device *dev,
	u16 frame_type, bool reg)
{

	WL_DBG(("%s: frame_type: %x, reg: %d\n", __func__, frame_type, reg));
=======
wl_cfg80211_mgmt_frame_register(struct wiphy *wiphy, bcm_struct_cfgdev *cfgdev,
	u16 frame_type, bool reg)
{

	WL_DBG(("frame_type: %x, reg: %d\n", frame_type, reg));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (frame_type != (IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_PROBE_REQ))
		return;

	return;
}


static s32
wl_cfg80211_change_bss(struct wiphy *wiphy,
	struct net_device *dev,
	struct bss_parameters *params)
{
<<<<<<< HEAD
=======
	s32 err = 0;
	s32 ap_isolate = 0;

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (params->use_cts_prot >= 0) {
	}

	if (params->use_short_preamble >= 0) {
	}

	if (params->use_short_slot_time >= 0) {
	}

	if (params->basic_rates) {
	}

	if (params->ap_isolate >= 0) {
<<<<<<< HEAD
=======
		ap_isolate = params->ap_isolate;
		err = wldev_iovar_setint(dev, "ap_isolate", ap_isolate);
		if (unlikely(err))
		{
			WL_ERR(("set ap_isolate Error (%d)\n", err));
		}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

	if (params->ht_opmode >= 0) {
	}

<<<<<<< HEAD
=======

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return 0;
}

static s32
wl_cfg80211_set_channel(struct wiphy *wiphy, struct net_device *dev,
	struct ieee80211_channel *chan,
	enum nl80211_channel_type channel_type)
{
	s32 _chan;
	chanspec_t chspec = 0;
	chanspec_t fw_chspec = 0;
	u32 bw = WL_CHANSPEC_BW_20;

	s32 err = BCME_OK;
	s32 bw_cap = 0;
	struct {
		u32 band;
		u32 bw_cap;
	} param = {0, 0};
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);

	if (wl->p2p_net == dev) {
		dev = wl_to_prmry_ndev(wl);
	}
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
#ifdef CUSTOM_SET_CPUCORE
	dhd_pub_t *dhd =  (dhd_pub_t *)(cfg->pub);
#endif /* CUSTOM_SET_CPUCORE */

	dev = ndev_to_wlc_ndev(dev, cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	_chan = ieee80211_frequency_to_channel(chan->center_freq);
	WL_ERR(("netdev_ifidx(%d), chan_type(%d) target channel(%d) \n",
		dev->ifindex, channel_type, _chan));

<<<<<<< HEAD
=======
#ifdef NOT_YET
	switch (channel_type) {
		case NL80211_CHAN_HT40MINUS:
			/* secondary channel is below the control channel */
			chspec = CH40MHZ_CHSPEC(channel, WL_CHANSPEC_CTL_SB_UPPER);
			break;
		case NL80211_CHAN_HT40PLUS:
			/* secondary channel is above the control channel */
			chspec = CH40MHZ_CHSPEC(channel, WL_CHANSPEC_CTL_SB_LOWER);
			break;
		default:
			chspec = CH20MHZ_CHSPEC(channel);

	}
#endif /* NOT_YET */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (chan->band == IEEE80211_BAND_5GHZ) {
		param.band = WLC_BAND_5G;
		err = wldev_iovar_getbuf(dev, "bw_cap", &param, sizeof(param),
<<<<<<< HEAD
			wl->ioctl_buf, WLC_IOCTL_SMLEN, &wl->ioctl_buf_sync);
=======
			cfg->ioctl_buf, WLC_IOCTL_SMLEN, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (err) {
			if (err != BCME_UNSUPPORTED) {
				WL_ERR(("bw_cap failed, %d\n", err));
				return err;
			} else {
				err = wldev_iovar_getint(dev, "mimo_bw_cap", &bw_cap);
				if (err) {
					WL_ERR(("error get mimo_bw_cap (%d)\n", err));
				}
				if (bw_cap != WLC_N_BW_20ALL)
					bw = WL_CHANSPEC_BW_40;
			}
		} else {
<<<<<<< HEAD
			if (WL_BW_CAP_80MHZ(wl->ioctl_buf[0]))
				bw = WL_CHANSPEC_BW_80;
			else if (WL_BW_CAP_40MHZ(wl->ioctl_buf[0]))
=======
			if (WL_BW_CAP_80MHZ(cfg->ioctl_buf[0]))
				bw = WL_CHANSPEC_BW_80;
			else if (WL_BW_CAP_40MHZ(cfg->ioctl_buf[0]))
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				bw = WL_CHANSPEC_BW_40;
			else
				bw = WL_CHANSPEC_BW_20;

		}

	} else if (chan->band == IEEE80211_BAND_2GHZ)
		bw = WL_CHANSPEC_BW_20;
set_channel:
	chspec = wf_channel2chspec(_chan, bw);
	if (wf_chspec_valid(chspec)) {
		fw_chspec = wl_chspec_host_to_driver(chspec);
		if (fw_chspec != INVCHANSPEC) {
			if ((err = wldev_iovar_setint(dev, "chanspec",
				fw_chspec)) == BCME_BADCHAN) {
				if (bw == WL_CHANSPEC_BW_80)
					goto change_bw;
				err = wldev_ioctl(dev, WLC_SET_CHANNEL,
					&_chan, sizeof(_chan), true);
				if (err < 0) {
					WL_ERR(("WLC_SET_CHANNEL error %d"
					"chip may not be supporting this channel\n", err));
				}
			} else if (err) {
				WL_ERR(("failed to set chanspec error %d\n", err));
			}
		} else {
			WL_ERR(("failed to convert host chanspec to fw chanspec\n"));
			err = BCME_ERROR;
		}
	} else {
change_bw:
		if (bw == WL_CHANSPEC_BW_80)
			bw = WL_CHANSPEC_BW_40;
		else if (bw == WL_CHANSPEC_BW_40)
			bw = WL_CHANSPEC_BW_20;
		else
			bw = 0;
		if (bw)
			goto set_channel;
		WL_ERR(("Invalid chanspec 0x%x\n", chspec));
		err = BCME_ERROR;
	}
<<<<<<< HEAD
	return err;
}

=======
#ifdef CUSTOM_SET_CPUCORE
	if (dhd->op_mode == DHD_FLAG_HOSTAP_MODE) {
		WL_DBG(("SoftAP mode do not need to set cpucore\n"));
	} else if ((dev == wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_CONNECTION)) &&
		(chspec & WL_CHANSPEC_BW_80)) {
		/* If GO is vht80 */
		dhd->chan_isvht80 |= DHD_FLAG_P2P_MODE;
		dhd_set_cpucore(dhd, TRUE);
	}
#endif
	return err;
}

#ifdef WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST
struct net_device *
wl_cfg80211_get_remain_on_channel_ndev(struct bcm_cfg80211 *cfg)
{
	struct net_info *_net_info, *next;
	list_for_each_entry_safe(_net_info, next, &cfg->net_list, list) {
		if (_net_info->ndev &&
			test_bit(WL_STATUS_REMAINING_ON_CHANNEL, &_net_info->sme_state))
			return _net_info->ndev;
	}
	return NULL;
}
#endif /* WL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST */

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32
wl_validate_opensecurity(struct net_device *dev, s32 bssidx)
{
	s32 err = BCME_OK;

	/* set auth */
	err = wldev_iovar_setint_bsscfg(dev, "auth", 0, bssidx);
	if (err < 0) {
		WL_ERR(("auth error %d\n", err));
		return BCME_ERROR;
	}
	/* set wsec */
	err = wldev_iovar_setint_bsscfg(dev, "wsec", 0, bssidx);
	if (err < 0) {
		WL_ERR(("wsec error %d\n", err));
		return BCME_ERROR;
	}
<<<<<<< HEAD
=======

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	/* set upper-layer auth */
	err = wldev_iovar_setint_bsscfg(dev, "wpa_auth", WPA_AUTH_NONE, bssidx);
	if (err < 0) {
		WL_ERR(("wpa_auth error %d\n", err));
		return BCME_ERROR;
	}

	return 0;
}

static s32
wl_validate_wpa2ie(struct net_device *dev, bcm_tlv_t *wpa2ie, s32 bssidx)
{
	s32 len = 0;
	s32 err = BCME_OK;
	u16 auth = 0; /* d11 open authentication */
	u32 wsec;
	u32 pval = 0;
	u32 gval = 0;
	u32 wpa_auth = 0;
	wpa_suite_mcast_t *mcast;
	wpa_suite_ucast_t *ucast;
	wpa_suite_auth_key_mgmt_t *mgmt;

	u16 suite_count;
	u8 rsn_cap[2];
	u32 wme_bss_disable;

	if (wpa2ie == NULL)
		goto exit;

	WL_DBG(("Enter \n"));
	len =  wpa2ie->len;
	/* check the mcast cipher */
	mcast = (wpa_suite_mcast_t *)&wpa2ie->data[WPA2_VERSION_LEN];
	switch (mcast->type) {
		case WPA_CIPHER_NONE:
			gval = 0;
			break;
		case WPA_CIPHER_WEP_40:
		case WPA_CIPHER_WEP_104:
			gval = WEP_ENABLED;
			break;
		case WPA_CIPHER_TKIP:
			gval = TKIP_ENABLED;
			break;
		case WPA_CIPHER_AES_CCM:
			gval = AES_ENABLED;
			break;
<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
		case WAPI_CIPHER_SMS4:
			gval = SMS4_ENABLED;
			break;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		default:
			WL_ERR(("No Security Info\n"));
			break;
	}
	if ((len -= WPA_SUITE_LEN) <= 0)
		return BCME_BADLEN;

	/* check the unicast cipher */
	ucast = (wpa_suite_ucast_t *)&mcast[1];
	suite_count = ltoh16_ua(&ucast->count);
	switch (ucast->list[0].type) {
		case WPA_CIPHER_NONE:
			pval = 0;
			break;
		case WPA_CIPHER_WEP_40:
		case WPA_CIPHER_WEP_104:
			pval = WEP_ENABLED;
			break;
		case WPA_CIPHER_TKIP:
			pval = TKIP_ENABLED;
			break;
		case WPA_CIPHER_AES_CCM:
			pval = AES_ENABLED;
			break;
<<<<<<< HEAD
=======
#ifdef BCMWAPI_WPI
		case WAPI_CIPHER_SMS4:
			pval = SMS4_ENABLED;
			break;
#endif
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		default:
			WL_ERR(("No Security Info\n"));
	}
	if ((len -= (WPA_IE_SUITE_COUNT_LEN + (WPA_SUITE_LEN * suite_count))) <= 0)
		return BCME_BADLEN;

	/* FOR WPS , set SEC_OW_ENABLED */
	wsec = (pval | gval | SES_OW_ENABLED);
	/* check the AKM */
	mgmt = (wpa_suite_auth_key_mgmt_t *)&ucast->list[suite_count];
	suite_count = ltoh16_ua(&mgmt->count);
	switch (mgmt->list[0].type) {
		case RSN_AKM_NONE:
			wpa_auth = WPA_AUTH_NONE;
			break;
		case RSN_AKM_UNSPECIFIED:
			wpa_auth = WPA2_AUTH_UNSPECIFIED;
			break;
		case RSN_AKM_PSK:
			wpa_auth = WPA2_AUTH_PSK;
			break;
		default:
			WL_ERR(("No Key Mgmt Info\n"));
	}

	if ((len -= (WPA_IE_SUITE_COUNT_LEN + (WPA_SUITE_LEN * suite_count))) >= RSN_CAP_LEN) {
		rsn_cap[0] = *(u8 *)&mgmt->list[suite_count];
		rsn_cap[1] = *((u8 *)&mgmt->list[suite_count] + 1);

		if (rsn_cap[0] & (RSN_CAP_16_REPLAY_CNTRS << RSN_CAP_PTK_REPLAY_CNTR_SHIFT)) {
			wme_bss_disable = 0;
		} else {
			wme_bss_disable = 1;
		}

		/* set wme_bss_disable to sync RSN Capabilities */
		err = wldev_iovar_setint_bsscfg(dev, "wme_bss_disable", wme_bss_disable, bssidx);
		if (err < 0) {
			WL_ERR(("wme_bss_disable error %d\n", err));
			return BCME_ERROR;
		}
	} else {
		WL_DBG(("There is no RSN Capabilities. remained len %d\n", len));
	}

	/* set auth */
	err = wldev_iovar_setint_bsscfg(dev, "auth", auth, bssidx);
	if (err < 0) {
		WL_ERR(("auth error %d\n", err));
		return BCME_ERROR;
	}
	/* set wsec */
	err = wldev_iovar_setint_bsscfg(dev, "wsec", wsec, bssidx);
	if (err < 0) {
		WL_ERR(("wsec error %d\n", err));
		return BCME_ERROR;
	}
	/* set upper-layer auth */
	err = wldev_iovar_setint_bsscfg(dev, "wpa_auth", wpa_auth, bssidx);
	if (err < 0) {
		WL_ERR(("wpa_auth error %d\n", err));
		return BCME_ERROR;
	}
exit:
	return 0;
}

static s32
wl_validate_wpaie(struct net_device *dev, wpa_ie_fixed_t *wpaie, s32 bssidx)
{
	wpa_suite_mcast_t *mcast;
	wpa_suite_ucast_t *ucast;
	wpa_suite_auth_key_mgmt_t *mgmt;
	u16 auth = 0; /* d11 open authentication */
	u16 count;
	s32 err = BCME_OK;
	s32 len = 0;
	u32 i;
	u32 wsec;
	u32 pval = 0;
	u32 gval = 0;
	u32 wpa_auth = 0;
	u32 tmp = 0;

	if (wpaie == NULL)
		goto exit;
	WL_DBG(("Enter \n"));
	len = wpaie->length;    /* value length */
	len -= WPA_IE_TAG_FIXED_LEN;
	/* check for multicast cipher suite */
	if (len < WPA_SUITE_LEN) {
		WL_INFO(("no multicast cipher suite\n"));
		goto exit;
	}

	/* pick up multicast cipher */
	mcast = (wpa_suite_mcast_t *)&wpaie[1];
	len -= WPA_SUITE_LEN;
	if (!bcmp(mcast->oui, WPA_OUI, WPA_OUI_LEN)) {
		if (IS_WPA_CIPHER(mcast->type)) {
			tmp = 0;
			switch (mcast->type) {
				case WPA_CIPHER_NONE:
					tmp = 0;
					break;
				case WPA_CIPHER_WEP_40:
				case WPA_CIPHER_WEP_104:
					tmp = WEP_ENABLED;
					break;
				case WPA_CIPHER_TKIP:
					tmp = TKIP_ENABLED;
					break;
				case WPA_CIPHER_AES_CCM:
					tmp = AES_ENABLED;
					break;
				default:
					WL_ERR(("No Security Info\n"));
			}
			gval |= tmp;
		}
	}
	/* Check for unicast suite(s) */
	if (len < WPA_IE_SUITE_COUNT_LEN) {
		WL_INFO(("no unicast suite\n"));
		goto exit;
	}
	/* walk thru unicast cipher list and pick up what we recognize */
	ucast = (wpa_suite_ucast_t *)&mcast[1];
	count = ltoh16_ua(&ucast->count);
	len -= WPA_IE_SUITE_COUNT_LEN;
	for (i = 0; i < count && len >= WPA_SUITE_LEN;
		i++, len -= WPA_SUITE_LEN) {
		if (!bcmp(ucast->list[i].oui, WPA_OUI, WPA_OUI_LEN)) {
			if (IS_WPA_CIPHER(ucast->list[i].type)) {
				tmp = 0;
				switch (ucast->list[i].type) {
					case WPA_CIPHER_NONE:
						tmp = 0;
						break;
					case WPA_CIPHER_WEP_40:
					case WPA_CIPHER_WEP_104:
						tmp = WEP_ENABLED;
						break;
					case WPA_CIPHER_TKIP:
						tmp = TKIP_ENABLED;
						break;
					case WPA_CIPHER_AES_CCM:
						tmp = AES_ENABLED;
						break;
					default:
						WL_ERR(("No Security Info\n"));
				}
				pval |= tmp;
			}
		}
	}
	len -= (count - i) * WPA_SUITE_LEN;
	/* Check for auth key management suite(s) */
	if (len < WPA_IE_SUITE_COUNT_LEN) {
		WL_INFO((" no auth key mgmt suite\n"));
		goto exit;
	}
	/* walk thru auth management suite list and pick up what we recognize */
	mgmt = (wpa_suite_auth_key_mgmt_t *)&ucast->list[count];
	count = ltoh16_ua(&mgmt->count);
	len -= WPA_IE_SUITE_COUNT_LEN;
	for (i = 0; i < count && len >= WPA_SUITE_LEN;
		i++, len -= WPA_SUITE_LEN) {
		if (!bcmp(mgmt->list[i].oui, WPA_OUI, WPA_OUI_LEN)) {
			if (IS_WPA_AKM(mgmt->list[i].type)) {
				tmp = 0;
				switch (mgmt->list[i].type) {
					case RSN_AKM_NONE:
						tmp = WPA_AUTH_NONE;
						break;
					case RSN_AKM_UNSPECIFIED:
						tmp = WPA_AUTH_UNSPECIFIED;
						break;
					case RSN_AKM_PSK:
						tmp = WPA_AUTH_PSK;
						break;
					default:
						WL_ERR(("No Key Mgmt Info\n"));
				}
				wpa_auth |= tmp;
			}
		}

	}
	/* FOR WPS , set SEC_OW_ENABLED */
	wsec = (pval | gval | SES_OW_ENABLED);
	/* set auth */
	err = wldev_iovar_setint_bsscfg(dev, "auth", auth, bssidx);
	if (err < 0) {
		WL_ERR(("auth error %d\n", err));
		return BCME_ERROR;
	}
	/* set wsec */
	err = wldev_iovar_setint_bsscfg(dev, "wsec", wsec, bssidx);
	if (err < 0) {
		WL_ERR(("wsec error %d\n", err));
		return BCME_ERROR;
	}
	/* set upper-layer auth */
	err = wldev_iovar_setint_bsscfg(dev, "wpa_auth", wpa_auth, bssidx);
	if (err < 0) {
		WL_ERR(("wpa_auth error %d\n", err));
		return BCME_ERROR;
	}
exit:
	return 0;
}

<<<<<<< HEAD
=======

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32
wl_cfg80211_bcn_validate_sec(
	struct net_device *dev,
	struct parsed_ies *ies,
	u32 dev_role,
	s32 bssidx)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (dev_role == NL80211_IFTYPE_P2P_GO && (ies->wpa2_ie)) {
		/* For P2P GO, the sec type is WPA2-PSK */
		WL_DBG(("P2P GO: validating wpa2_ie"));
		if (wl_validate_wpa2ie(dev, ies->wpa2_ie, bssidx)  < 0)
			return BCME_ERROR;

	} else if (dev_role == NL80211_IFTYPE_AP) {

		WL_DBG(("SoftAP: validating security"));
		/* If wpa2_ie or wpa_ie is present validate it */
<<<<<<< HEAD
		if ((ies->wpa2_ie || ies->wpa_ie) &&
			((wl_validate_wpa2ie(dev, ies->wpa2_ie, bssidx)  < 0 ||
			wl_validate_wpaie(dev, ies->wpa_ie, bssidx) < 0))) {
			wl->ap_info->security_mode = false;
			return BCME_ERROR;
		}

		wl->ap_info->security_mode = true;
		if (wl->ap_info->rsn_ie) {
			kfree(wl->ap_info->rsn_ie);
			wl->ap_info->rsn_ie = NULL;
		}
		if (wl->ap_info->wpa_ie) {
			kfree(wl->ap_info->wpa_ie);
			wl->ap_info->wpa_ie = NULL;
		}
		if (wl->ap_info->wps_ie) {
			kfree(wl->ap_info->wps_ie);
			wl->ap_info->wps_ie = NULL;
		}
		if (ies->wpa_ie != NULL) {
			/* WPAIE */
			wl->ap_info->rsn_ie = NULL;
			wl->ap_info->wpa_ie = kmemdup(ies->wpa_ie,
=======

		if ((ies->wpa2_ie || ies->wpa_ie) &&
			((wl_validate_wpa2ie(dev, ies->wpa2_ie, bssidx)  < 0 ||
			wl_validate_wpaie(dev, ies->wpa_ie, bssidx) < 0))) {
			cfg->ap_info->security_mode = false;
			return BCME_ERROR;
		}

		cfg->ap_info->security_mode = true;
		if (cfg->ap_info->rsn_ie) {
			kfree(cfg->ap_info->rsn_ie);
			cfg->ap_info->rsn_ie = NULL;
		}
		if (cfg->ap_info->wpa_ie) {
			kfree(cfg->ap_info->wpa_ie);
			cfg->ap_info->wpa_ie = NULL;
		}
		if (cfg->ap_info->wps_ie) {
			kfree(cfg->ap_info->wps_ie);
			cfg->ap_info->wps_ie = NULL;
		}
		if (ies->wpa_ie != NULL) {
			/* WPAIE */
			cfg->ap_info->rsn_ie = NULL;
			cfg->ap_info->wpa_ie = kmemdup(ies->wpa_ie,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				ies->wpa_ie->length + WPA_RSN_IE_TAG_FIXED_LEN,
				GFP_KERNEL);
		} else if (ies->wpa2_ie != NULL) {
			/* RSNIE */
<<<<<<< HEAD
			wl->ap_info->wpa_ie = NULL;
			wl->ap_info->rsn_ie = kmemdup(ies->wpa2_ie,
=======
			cfg->ap_info->wpa_ie = NULL;
			cfg->ap_info->rsn_ie = kmemdup(ies->wpa2_ie,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				ies->wpa2_ie->len + WPA_RSN_IE_TAG_FIXED_LEN,
				GFP_KERNEL);
		}

		if (!ies->wpa2_ie && !ies->wpa_ie) {
			wl_validate_opensecurity(dev, bssidx);
<<<<<<< HEAD
			wl->ap_info->security_mode = false;
		}

		if (ies->wps_ie) {
			wl->ap_info->wps_ie = kmemdup(ies->wps_ie, ies->wps_ie_len, GFP_KERNEL);
=======
			cfg->ap_info->security_mode = false;
		}

		if (ies->wps_ie) {
			cfg->ap_info->wps_ie = kmemdup(ies->wps_ie, ies->wps_ie_len, GFP_KERNEL);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	}

	return 0;

}

<<<<<<< HEAD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) || defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32 wl_cfg80211_bcn_set_params(
	struct cfg80211_ap_settings *info,
	struct net_device *dev,
	u32 dev_role, s32 bssidx)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = BCME_OK;

	WL_DBG(("interval (%d) \ndtim_period (%d) \n",
		info->beacon_interval, info->dtim_period));

	if (info->beacon_interval) {
		if ((err = wldev_ioctl(dev, WLC_SET_BCNPRD,
			&info->beacon_interval, sizeof(s32), true)) < 0) {
			WL_ERR(("Beacon Interval Set Error, %d\n", err));
			return err;
		}
	}

	if (info->dtim_period) {
		if ((err = wldev_ioctl(dev, WLC_SET_DTIMPRD,
			&info->dtim_period, sizeof(s32), true)) < 0) {
			WL_ERR(("DTIM Interval Set Error, %d\n", err));
			return err;
		}
	}

	if ((info->ssid) && (info->ssid_len > 0) &&
		(info->ssid_len <= 32)) {
<<<<<<< HEAD
		WL_DBG(("SSID (%s) len:%d \n", info->ssid, info->ssid_len));
		if (dev_role == NL80211_IFTYPE_AP) {
			/* Store the hostapd SSID */
			memset(wl->hostapd_ssid.SSID, 0x00, 32);
			memcpy(wl->hostapd_ssid.SSID, info->ssid, info->ssid_len);
			wl->hostapd_ssid.SSID_len = info->ssid_len;
		} else {
				/* P2P GO */
			memset(wl->p2p->ssid.SSID, 0x00, 32);
			memcpy(wl->p2p->ssid.SSID, info->ssid, info->ssid_len);
			wl->p2p->ssid.SSID_len = info->ssid_len;
=======
		WL_DBG(("SSID (%s) len:%zd \n", info->ssid, info->ssid_len));
		if (dev_role == NL80211_IFTYPE_AP) {
			/* Store the hostapd SSID */
			memset(cfg->hostapd_ssid.SSID, 0x00, 32);
			memcpy(cfg->hostapd_ssid.SSID, info->ssid, info->ssid_len);
			cfg->hostapd_ssid.SSID_len = info->ssid_len;
		} else {
				/* P2P GO */
			memset(cfg->p2p->ssid.SSID, 0x00, 32);
			memcpy(cfg->p2p->ssid.SSID, info->ssid, info->ssid_len);
			cfg->p2p->ssid.SSID_len = info->ssid_len;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	}

	if (info->hidden_ssid) {
		if ((err = wldev_iovar_setint(dev, "closednet", 1)) < 0)
			WL_ERR(("failed to set hidden : %d\n", err));
		WL_DBG(("hidden_ssid_enum_val: %d \n", info->hidden_ssid));
	}

	return err;
}
<<<<<<< HEAD
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0) */
=======
#endif /* LINUX_VERSION >= VERSION(3,4,0) || WL_COMPAT_WIRELESS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

static s32
wl_cfg80211_parse_ies(u8 *ptr, u32 len, struct parsed_ies *ies)
{
	s32 err = BCME_OK;

	memset(ies, 0, sizeof(struct parsed_ies));

	/* find the WPSIE */
	if ((ies->wps_ie = wl_cfgp2p_find_wpsie(ptr, len)) != NULL) {
		WL_DBG(("WPSIE in beacon \n"));
		ies->wps_ie_len = ies->wps_ie->length + WPA_RSN_IE_TAG_FIXED_LEN;
	} else {
		WL_ERR(("No WPSIE in beacon \n"));
	}

	/* find the RSN_IE */
	if ((ies->wpa2_ie = bcm_parse_tlvs(ptr, len,
		DOT11_MNG_RSN_ID)) != NULL) {
		WL_DBG((" WPA2 IE found\n"));
		ies->wpa2_ie_len = ies->wpa2_ie->len;
	}

	/* find the WPA_IE */
	if ((ies->wpa_ie = wl_cfgp2p_find_wpaie(ptr, len)) != NULL) {
		WL_DBG((" WPA found\n"));
		ies->wpa_ie_len = ies->wpa_ie->length;
	}

	return err;

}

static s32
wl_cfg80211_bcn_bringup_ap(
	struct net_device *dev,
	struct parsed_ies *ies,
	u32 dev_role, s32 bssidx)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct wl_join_params join_params;
	bool is_bssup = false;
	s32 infra = 1;
	s32 join_params_size = 0;
	s32 ap = 1;
<<<<<<< HEAD
=======
#ifdef DISABLE_11H_SOFTAP
	s32 spect = 0;
#endif /* DISABLE_11H_SOFTAP */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = BCME_OK;

	WL_DBG(("Enter dev_role: %d\n", dev_role));

	/* Common code for SoftAP and P2P GO */
	wldev_iovar_setint(dev, "mpc", 0);

	if (dev_role == NL80211_IFTYPE_P2P_GO) {
		is_bssup = wl_cfgp2p_bss_isup(dev, bssidx);
		if (!is_bssup && (ies->wpa2_ie != NULL)) {

			err = wldev_ioctl(dev, WLC_SET_INFRA, &infra, sizeof(s32), true);
			if (err < 0) {
				WL_ERR(("SET INFRA error %d\n", err));
				goto exit;
			}

<<<<<<< HEAD
			err = wldev_iovar_setbuf_bsscfg(dev, "ssid", &wl->p2p->ssid,
				sizeof(wl->p2p->ssid), wl->ioctl_buf, WLC_IOCTL_MAXLEN,
				bssidx, &wl->ioctl_buf_sync);
=======
			err = wldev_iovar_setbuf_bsscfg(dev, "ssid", &cfg->p2p->ssid,
				sizeof(cfg->p2p->ssid), cfg->ioctl_buf, WLC_IOCTL_MAXLEN,
				bssidx, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			if (err < 0) {
				WL_ERR(("GO SSID setting error %d\n", err));
				goto exit;
			}

<<<<<<< HEAD
			if ((err = wl_cfgp2p_bss(wl, dev, bssidx, 1)) < 0) {
=======
			/* Do abort scan before creating GO */
			wl_cfg80211_scan_abort(cfg);

			if ((err = wl_cfgp2p_bss(cfg, dev, bssidx, 1)) < 0) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				WL_ERR(("GO Bring up error %d\n", err));
				goto exit;
			}
		} else
			WL_DBG(("Bss is already up\n"));
	} else if ((dev_role == NL80211_IFTYPE_AP) &&
<<<<<<< HEAD
		(wl_get_drv_status(wl, AP_CREATING, dev))) {
=======
		(wl_get_drv_status(cfg, AP_CREATING, dev))) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* Device role SoftAP */
		err = wldev_ioctl(dev, WLC_DOWN, &ap, sizeof(s32), true);
		if (err < 0) {
			WL_ERR(("WLC_DOWN error %d\n", err));
			goto exit;
		}
		err = wldev_ioctl(dev, WLC_SET_INFRA, &infra, sizeof(s32), true);
		if (err < 0) {
			WL_ERR(("SET INFRA error %d\n", err));
			goto exit;
		}
		if ((err = wldev_ioctl(dev, WLC_SET_AP, &ap, sizeof(s32), true)) < 0) {
			WL_ERR(("setting AP mode failed %d \n", err));
			goto exit;
		}
<<<<<<< HEAD
=======
#ifdef DISABLE_11H_SOFTAP
		err = wldev_ioctl(dev, WLC_SET_SPECT_MANAGMENT,
			&spect, sizeof(s32), true);
		if (err < 0) {
			WL_ERR(("SET SPECT_MANAGMENT error %d\n", err));
			goto exit;
		}
#endif /* DISABLE_11H_SOFTAP */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		err = wldev_ioctl(dev, WLC_UP, &ap, sizeof(s32), true);
		if (unlikely(err)) {
			WL_ERR(("WLC_UP error (%d)\n", err));
			goto exit;
		}

		memset(&join_params, 0, sizeof(join_params));
		/* join parameters starts with ssid */
		join_params_size = sizeof(join_params.ssid);
<<<<<<< HEAD
		memcpy(join_params.ssid.SSID, wl->hostapd_ssid.SSID,
			wl->hostapd_ssid.SSID_len);
		join_params.ssid.SSID_len = htod32(wl->hostapd_ssid.SSID_len);
=======
		memcpy(join_params.ssid.SSID, cfg->hostapd_ssid.SSID,
			cfg->hostapd_ssid.SSID_len);
		join_params.ssid.SSID_len = htod32(cfg->hostapd_ssid.SSID_len);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		/* create softap */
		if ((err = wldev_ioctl(dev, WLC_SET_SSID, &join_params,
			join_params_size, true)) == 0) {
			WL_DBG(("SoftAP set SSID (%s) success\n", join_params.ssid.SSID));
<<<<<<< HEAD
			wl_clr_drv_status(wl, AP_CREATING, dev);
			wl_set_drv_status(wl, AP_CREATED, dev);
=======
			wl_clr_drv_status(cfg, AP_CREATING, dev);
			wl_set_drv_status(cfg, AP_CREATED, dev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	}


exit:
	return err;
}

<<<<<<< HEAD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
s32
wl_cfg80211_parse_set_ies(
	struct net_device *dev,
	struct cfg80211_beacon_data *info,
	struct parsed_ies *ies,
	u32 dev_role,
	s32 bssidx)
{
	struct wl_priv *wl = wlcfg_drv_priv;
	struct parsed_ies prb_ies;
	s32 err = BCME_OK;

	memset(ies, 0, sizeof(struct parsed_ies));
	memset(&prb_ies, 0, sizeof(struct parsed_ies));

=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) || defined(WL_COMPAT_WIRELESS)
s32
wl_cfg80211_parse_ap_ies(
	struct net_device *dev,
	struct cfg80211_beacon_data *info,
	struct parsed_ies *ies)
{
	struct parsed_ies prb_ies;
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	dhd_pub_t *dhd = (dhd_pub_t *)(cfg->pub);
	u8 *vndr = NULL;
	u32 vndr_ie_len = 0;
	s32 err = BCME_OK;

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	/* Parse Beacon IEs */
	if (wl_cfg80211_parse_ies((u8 *)info->tail,
		info->tail_len, ies) < 0) {
		WL_ERR(("Beacon get IEs failed \n"));
		err = -EINVAL;
		goto fail;
	}

<<<<<<< HEAD
	/* Set Beacon IEs to FW */
	if ((err = wl_cfgp2p_set_management_ie(wl, dev, bssidx,
=======
	vndr = (u8 *)info->proberesp_ies;
	vndr_ie_len = info->proberesp_ies_len;

	if (dhd->op_mode & DHD_FLAG_HOSTAP_MODE) {
		/* SoftAP mode */
		struct ieee80211_mgmt *mgmt;
		mgmt = (struct ieee80211_mgmt *)info->probe_resp;
		if (mgmt != NULL) {
			vndr = (u8 *)&mgmt->u.probe_resp.variable;
			vndr_ie_len = info->probe_resp_len -
				offsetof(struct ieee80211_mgmt, u.probe_resp.variable);
		}
	}

	/* Parse Probe Response IEs */
	if (wl_cfg80211_parse_ies(vndr, vndr_ie_len, &prb_ies) < 0) {
		WL_ERR(("PROBE RESP get IEs failed \n"));
		err = -EINVAL;
	}

fail:

	return err;
}

s32
wl_cfg80211_set_ies(
	struct net_device *dev,
	struct cfg80211_beacon_data *info,
	s32 bssidx)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	dhd_pub_t *dhd = (dhd_pub_t *)(cfg->pub);
	u8 *vndr = NULL;
	u32 vndr_ie_len = 0;
	s32 err = BCME_OK;

	/* Set Beacon IEs to FW */
	if ((err = wl_cfgp2p_set_management_ie(cfg, dev, bssidx,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		VNDR_IE_BEACON_FLAG, (u8 *)info->tail,
		info->tail_len)) < 0) {
		WL_ERR(("Set Beacon IE Failed \n"));
	} else {
		WL_DBG(("Applied Vndr IEs for Beacon \n"));
	}

<<<<<<< HEAD
	/* Parse Probe Response IEs */
	if (wl_cfg80211_parse_ies((u8 *)info->proberesp_ies,
		info->proberesp_ies_len, &prb_ies) < 0) {
		WL_ERR(("PRB RESP get IEs failed \n"));
		err = -EINVAL;
		goto fail;
	}

	/* Set Probe Response IEs to FW */
	if ((err = wl_cfgp2p_set_management_ie(wl, dev, bssidx,
		VNDR_IE_PRBRSP_FLAG, (u8 *)info->proberesp_ies,
		info->proberesp_ies_len)) < 0) {
=======
	vndr = (u8 *)info->proberesp_ies;
	vndr_ie_len = info->proberesp_ies_len;

	if (dhd->op_mode & DHD_FLAG_HOSTAP_MODE) {
		/* SoftAP mode */
		struct ieee80211_mgmt *mgmt;
		mgmt = (struct ieee80211_mgmt *)info->probe_resp;
		if (mgmt != NULL) {
			vndr = (u8 *)&mgmt->u.probe_resp.variable;
			vndr_ie_len = info->probe_resp_len -
				offsetof(struct ieee80211_mgmt, u.probe_resp.variable);
		}
	}

	/* Set Probe Response IEs to FW */
	if ((err = wl_cfgp2p_set_management_ie(cfg, dev, bssidx,
		VNDR_IE_PRBRSP_FLAG, vndr, vndr_ie_len)) < 0) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("Set Probe Resp IE Failed \n"));
	} else {
		WL_DBG(("Applied Vndr IEs for Probe Resp \n"));
	}

<<<<<<< HEAD
fail:

	return err;
}
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0) */
=======
	return err;
}
#endif /* LINUX_VERSION >= VERSION(3,4,0) || WL_COMPAT_WIRELESS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

static s32 wl_cfg80211_hostapd_sec(
	struct net_device *dev,
	struct parsed_ies *ies,
	s32 bssidx)
{
	bool update_bss = 0;
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;


	if (ies->wps_ie) {
		if (wl->ap_info->wps_ie &&
			memcmp(wl->ap_info->wps_ie, ies->wps_ie, ies->wps_ie_len)) {
			WL_DBG((" WPS IE is changed\n"));
			kfree(wl->ap_info->wps_ie);
			wl->ap_info->wps_ie = kmemdup(ies->wps_ie, ies->wps_ie_len, GFP_KERNEL);
		} else if (wl->ap_info->wps_ie == NULL) {
			WL_DBG((" WPS IE is added\n"));
			wl->ap_info->wps_ie = kmemdup(ies->wps_ie, ies->wps_ie_len, GFP_KERNEL);
		}
		if ((ies->wpa_ie != NULL || ies->wpa2_ie != NULL)) {
			if (!wl->ap_info->security_mode) {
				/* change from open mode to security mode */
				update_bss = true;
				if (ies->wpa_ie != NULL) {
					wl->ap_info->wpa_ie = kmemdup(ies->wpa_ie,
					ies->wpa_ie->length + WPA_RSN_IE_TAG_FIXED_LEN,
					GFP_KERNEL);
				} else {
					wl->ap_info->rsn_ie = kmemdup(ies->wpa2_ie,
					ies->wpa2_ie->len + WPA_RSN_IE_TAG_FIXED_LEN,
					GFP_KERNEL);
				}
			} else if (wl->ap_info->wpa_ie) {
				/* change from WPA2 mode to WPA mode */
				if (ies->wpa_ie != NULL) {
					update_bss = true;
					kfree(wl->ap_info->rsn_ie);
					wl->ap_info->rsn_ie = NULL;
					wl->ap_info->wpa_ie = kmemdup(ies->wpa_ie,
					ies->wpa_ie->length + WPA_RSN_IE_TAG_FIXED_LEN,
					GFP_KERNEL);
				} else if (memcmp(wl->ap_info->rsn_ie,
					ies->wpa2_ie, ies->wpa2_ie->len
					+ WPA_RSN_IE_TAG_FIXED_LEN)) {
					update_bss = true;
					kfree(wl->ap_info->rsn_ie);
					wl->ap_info->rsn_ie = kmemdup(ies->wpa2_ie,
					ies->wpa2_ie->len + WPA_RSN_IE_TAG_FIXED_LEN,
					GFP_KERNEL);
					wl->ap_info->wpa_ie = NULL;
				}
			}
			if (update_bss) {
				wl->ap_info->security_mode = true;
				wl_cfgp2p_bss(wl, dev, bssidx, 0);
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;


	if (ies->wps_ie) {
		if (cfg->ap_info->wps_ie &&
			memcmp(cfg->ap_info->wps_ie, ies->wps_ie, ies->wps_ie_len)) {
			WL_DBG((" WPS IE is changed\n"));
			kfree(cfg->ap_info->wps_ie);
			cfg->ap_info->wps_ie = kmemdup(ies->wps_ie, ies->wps_ie_len, GFP_KERNEL);
		} else if (cfg->ap_info->wps_ie == NULL) {
			WL_DBG((" WPS IE is added\n"));
			cfg->ap_info->wps_ie = kmemdup(ies->wps_ie, ies->wps_ie_len, GFP_KERNEL);
		}

		if ((ies->wpa_ie != NULL || ies->wpa2_ie != NULL)) {
			if (!cfg->ap_info->security_mode) {
				/* change from open mode to security mode */
				update_bss = true;
				if (ies->wpa_ie != NULL) {
					cfg->ap_info->wpa_ie = kmemdup(ies->wpa_ie,
					ies->wpa_ie->length + WPA_RSN_IE_TAG_FIXED_LEN,
					GFP_KERNEL);
				} else {
					cfg->ap_info->rsn_ie = kmemdup(ies->wpa2_ie,
					ies->wpa2_ie->len + WPA_RSN_IE_TAG_FIXED_LEN,
					GFP_KERNEL);
				}
			} else if (cfg->ap_info->wpa_ie) {
				/* change from WPA2 mode to WPA mode */
				if (ies->wpa_ie != NULL) {
					update_bss = true;
					kfree(cfg->ap_info->rsn_ie);
					cfg->ap_info->rsn_ie = NULL;
					cfg->ap_info->wpa_ie = kmemdup(ies->wpa_ie,
					ies->wpa_ie->length + WPA_RSN_IE_TAG_FIXED_LEN,
					GFP_KERNEL);
				} else if (memcmp(cfg->ap_info->rsn_ie,
					ies->wpa2_ie, ies->wpa2_ie->len
					+ WPA_RSN_IE_TAG_FIXED_LEN)) {
					update_bss = true;
					kfree(cfg->ap_info->rsn_ie);
					cfg->ap_info->rsn_ie = kmemdup(ies->wpa2_ie,
					ies->wpa2_ie->len + WPA_RSN_IE_TAG_FIXED_LEN,
					GFP_KERNEL);
					cfg->ap_info->wpa_ie = NULL;
				}
			}
			if (update_bss) {
				cfg->ap_info->security_mode = true;
				wl_cfgp2p_bss(cfg, dev, bssidx, 0);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				if (wl_validate_wpa2ie(dev, ies->wpa2_ie, bssidx)  < 0 ||
					wl_validate_wpaie(dev, ies->wpa_ie, bssidx) < 0) {
					return BCME_ERROR;
				}
<<<<<<< HEAD
				wl_cfgp2p_bss(wl, dev, bssidx, 1);
=======
				wl_cfgp2p_bss(cfg, dev, bssidx, 1);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			}
		}
	} else {
		WL_ERR(("No WPSIE in beacon \n"));
	}
	return 0;
}

#if defined(WL_SUPPORT_BACKPORTED_KPATCHES) || (LINUX_VERSION_CODE >= KERNEL_VERSION(3, \
	2, 0))
static s32
wl_cfg80211_del_station(
	struct wiphy *wiphy,
	struct net_device *ndev,
	u8* mac_addr)
{
	struct net_device *dev;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
	scb_val_t scb_val;
	s8 eabuf[ETHER_ADDR_STR_LEN];
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	scb_val_t scb_val;
	s8 eabuf[ETHER_ADDR_STR_LEN];
	int err;
	char mac_buf[MAX_NUM_OF_ASSOCIATED_DEV *
		sizeof(struct ether_addr) + sizeof(uint)] = {0};
	struct maclist *assoc_maclist = (struct maclist *)mac_buf;
	int num_associated = 0;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	WL_DBG(("Entry\n"));
	if (mac_addr == NULL) {
		WL_DBG(("mac_addr is NULL ignore it\n"));
		return 0;
	}

<<<<<<< HEAD
	if (ndev == wl->p2p_net) {
		dev = wl_to_prmry_ndev(wl);
	} else {
		dev = ndev;
	}

	if (p2p_is_on(wl)) {
		/* Suspend P2P discovery search-listen to prevent it from changing the
		 * channel.
		 */
		if ((wl_cfgp2p_discover_enable_search(wl, false)) < 0) {
=======
	dev = ndev_to_wlc_ndev(ndev, cfg);

	if (p2p_is_on(cfg)) {
		/* Suspend P2P discovery search-listen to prevent it from changing the
		 * channel.
		 */
		if ((wl_cfgp2p_discover_enable_search(cfg, false)) < 0) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_ERR(("Can not disable discovery mode\n"));
			return -EFAULT;
		}
	}

<<<<<<< HEAD
	memcpy(scb_val.ea.octet, mac_addr, ETHER_ADDR_LEN);
	scb_val.val = DOT11_RC_DEAUTH_LEAVING;
	if (wldev_ioctl(dev, WLC_SCB_DEAUTHENTICATE_FOR_REASON, &scb_val,
		sizeof(scb_val_t), true))
		WL_ERR(("WLC_SCB_DEAUTHENTICATE_FOR_REASON failed\n"));
	WL_DBG(("Disconnect STA : %s scb_val.val %d\n",
		bcm_ether_ntoa((const struct ether_addr *)mac_addr, eabuf),
		scb_val.val));
	wl_delay(400);
	return 0;
}
#endif /* WL_SUPPORT_BACKPORTED_KPATCHES || KERNEL_VER >= KERNEL_VERSION(3, 2, 0)) */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
=======
	assoc_maclist->count = MAX_NUM_OF_ASSOCIATED_DEV;
	err = wldev_ioctl(ndev, WLC_GET_ASSOCLIST,
		assoc_maclist, sizeof(mac_buf), false);
	if (err < 0)
		WL_ERR(("WLC_GET_ASSOCLIST error %d\n", err));
	else
		num_associated = assoc_maclist->count;

	memcpy(scb_val.ea.octet, mac_addr, ETHER_ADDR_LEN);
	scb_val.val = DOT11_RC_DEAUTH_LEAVING;
	err = wldev_ioctl(dev, WLC_SCB_DEAUTHENTICATE_FOR_REASON, &scb_val,
		sizeof(scb_val_t), true);
	if (err < 0)
		WL_ERR(("WLC_SCB_DEAUTHENTICATE_FOR_REASON err %d\n", err));
	WL_ERR(("Disconnect STA : %s scb_val.val %d\n",
		bcm_ether_ntoa((const struct ether_addr *)mac_addr, eabuf),
		scb_val.val));

	if (num_associated > 0 && ETHER_ISBCAST(mac_addr))
		wl_delay(400);

	return 0;
}

static s32
wl_cfg80211_change_station(
	struct wiphy *wiphy,
	struct net_device *dev,
	u8 *mac,
	struct station_parameters *params)
{
	int err;
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
	struct net_device *primary_ndev = bcmcfg_to_prmry_ndev(cfg);

	/* Processing only authorize/de-authorize flag for now */
	if (!(params->sta_flags_mask & BIT(NL80211_STA_FLAG_AUTHORIZED)))
		return -ENOTSUPP;

	if (!(params->sta_flags_set & BIT(NL80211_STA_FLAG_AUTHORIZED))) {
		err = wldev_ioctl(primary_ndev, WLC_SCB_DEAUTHORIZE, mac, ETH_ALEN, true);
		if (err)
			WL_ERR(("WLC_SCB_DEAUTHORIZE error (%d)\n", err));
		return err;
	}

	err = wldev_ioctl(primary_ndev, WLC_SCB_AUTHORIZE, mac, ETH_ALEN, true);
	if (err)
		WL_ERR(("WLC_SCB_AUTHORIZE error (%d)\n", err));
	return err;
}
#endif /* WL_SUPPORT_BACKPORTED_KPATCHES || KERNEL_VER >= KERNEL_VERSION(3, 2, 0)) */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) || defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32
wl_cfg80211_start_ap(
	struct wiphy *wiphy,
	struct net_device *dev,
	struct cfg80211_ap_settings *info)
{
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = BCME_OK;
	struct parsed_ies ies;
	s32 bssidx = 0;
	u32 dev_role = 0;

	WL_DBG(("Enter \n"));
<<<<<<< HEAD
	if (dev == wl_to_prmry_ndev(wl)) {
		WL_DBG(("Start AP req on primary iface: Softap\n"));
		dev_role = NL80211_IFTYPE_AP;
	} else if (dev == wl->p2p_net) {
		/* Group Add request on p2p0 */
		WL_DBG(("Start AP req on P2P iface: GO\n"));
		dev = wl_to_prmry_ndev(wl);
		dev_role = NL80211_IFTYPE_P2P_GO;
	}

	bssidx = wl_cfgp2p_find_idx(wl, dev);
	if (p2p_is_on(wl) &&
		(bssidx == wl_to_p2p_bss_bssidx(wl,
=======
	if (dev == bcmcfg_to_prmry_ndev(cfg)) {
		WL_DBG(("Start AP req on primary iface: Softap\n"));
		dev_role = NL80211_IFTYPE_AP;
	}
#if defined(WL_ENABLE_P2P_IF)
	else if (dev == cfg->p2p_net) {
		/* Group Add request on p2p0 */
		WL_DBG(("Start AP req on P2P iface: GO\n"));
		dev = bcmcfg_to_prmry_ndev(cfg);
		dev_role = NL80211_IFTYPE_P2P_GO;
	}
#endif /* WL_ENABLE_P2P_IF */
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
	if (p2p_is_on(cfg) &&
		(bssidx == wl_to_p2p_bss_bssidx(cfg,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		P2PAPI_BSSCFG_CONNECTION))) {
		dev_role = NL80211_IFTYPE_P2P_GO;
		WL_DBG(("Start AP req on P2P connection iface\n"));
	}

<<<<<<< HEAD
=======
	if (!check_dev_role_integrity(cfg, dev_role))
		goto fail;

#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)) && !defined(WL_COMPAT_WIRELESS))
	if ((err = wl_cfg80211_set_channel(wiphy, dev,
		dev->ieee80211_ptr->preset_chandef.chan,
		NL80211_CHAN_HT20) < 0)) {
		WL_ERR(("Set channel failed \n"));
		goto fail;
	}
#endif /* ((LINUX_VERSION >= VERSION(3, 6, 0) && !WL_COMPAT_WIRELESS) */

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if ((err = wl_cfg80211_bcn_set_params(info, dev,
		dev_role, bssidx)) < 0) {
		WL_ERR(("Beacon params set failed \n"));
		goto fail;
	}

<<<<<<< HEAD
	/* Set IEs to FW */
	if ((err = wl_cfg80211_parse_set_ies(dev, &info->beacon,
		&ies, dev_role, bssidx) < 0)) {
=======
	/* Parse IEs */
	if ((err = wl_cfg80211_parse_ap_ies(dev, &info->beacon, &ies)) < 0) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("Set IEs failed \n"));
		goto fail;
	}

	if ((wl_cfg80211_bcn_validate_sec(dev, &ies,
		dev_role, bssidx)) < 0)
	{
		WL_ERR(("Beacon set security failed \n"));
		goto fail;
	}

	if ((err = wl_cfg80211_bcn_bringup_ap(dev, &ies,
		dev_role, bssidx)) < 0) {
		WL_ERR(("Beacon bring up AP/GO failed \n"));
		goto fail;
	}

	WL_DBG(("** AP/GO Created **\n"));

<<<<<<< HEAD
=======
	/* Set IEs to FW */
	if ((err = wl_cfg80211_set_ies(dev, &info->beacon, bssidx)) < 0)
		WL_ERR(("Set IEs failed \n"));

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
fail:
	if (err) {
		WL_ERR(("ADD/SET beacon failed\n"));
		wldev_iovar_setint(dev, "mpc", 1);
	}

	return err;
}

static s32
wl_cfg80211_stop_ap(
	struct wiphy *wiphy,
	struct net_device *dev)
{
	int err = 0;
	u32 dev_role = 0;
	int infra = 0;
	int ap = 0;
	s32 bssidx = 0;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);

	WL_DBG(("Enter \n"));
	if (dev == wl_to_prmry_ndev(wl)) {
		dev_role = NL80211_IFTYPE_AP;
	} else if (dev == wl->p2p_net) {
		/* Group Add request on p2p0 */
		dev = wl_to_prmry_ndev(wl);
		dev_role = NL80211_IFTYPE_P2P_GO;
	}
	bssidx = wl_cfgp2p_find_idx(wl, dev);
	if (p2p_is_on(wl) &&
		(bssidx == wl_to_p2p_bss_bssidx(wl,
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);

	WL_DBG(("Enter \n"));
	if (dev == bcmcfg_to_prmry_ndev(cfg)) {
		dev_role = NL80211_IFTYPE_AP;
	}
#if defined(WL_ENABLE_P2P_IF)
	else if (dev == cfg->p2p_net) {
		/* Group Add request on p2p0 */
		dev = bcmcfg_to_prmry_ndev(cfg);
		dev_role = NL80211_IFTYPE_P2P_GO;
	}
#endif /* WL_ENABLE_P2P_IF */
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
	if (p2p_is_on(cfg) &&
		(bssidx == wl_to_p2p_bss_bssidx(cfg,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		P2PAPI_BSSCFG_CONNECTION))) {
		dev_role = NL80211_IFTYPE_P2P_GO;
	}

<<<<<<< HEAD
=======
	if (!check_dev_role_integrity(cfg, dev_role))
		goto exit;

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (dev_role == NL80211_IFTYPE_AP) {
		/* SoftAp on primary Interface.
		 * Shut down AP and turn on MPC
		 */
<<<<<<< HEAD
=======
		if ((err = wldev_ioctl(dev, WLC_SET_AP, &ap, sizeof(s32), true)) < 0) {
			WL_ERR(("setting AP mode failed %d \n", err));
			err = -ENOTSUPP;
			goto exit;
		}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		err = wldev_ioctl(dev, WLC_SET_INFRA, &infra, sizeof(s32), true);
		if (err < 0) {
			WL_ERR(("SET INFRA error %d\n", err));
			err = -ENOTSUPP;
			goto exit;
		}
<<<<<<< HEAD
		if ((err = wldev_ioctl(dev, WLC_SET_AP, &ap, sizeof(s32), true)) < 0) {
			WL_ERR(("setting AP mode failed %d \n", err));
			err = -ENOTSUPP;
			goto exit;
		}
=======
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		err = wldev_ioctl(dev, WLC_UP, &ap, sizeof(s32), true);
		if (unlikely(err)) {
			WL_ERR(("WLC_UP error (%d)\n", err));
			err = -EINVAL;
			goto exit;
		}

<<<<<<< HEAD
		wl_clr_drv_status(wl, AP_CREATED, dev);
		/* Turn on the MPC */
		wldev_iovar_setint(dev, "mpc", 1);
	} else {
		WL_DBG(("Stopping P2P GO \n"));
=======
		wl_clr_drv_status(cfg, AP_CREATED, dev);
		/* Turn on the MPC */
		wldev_iovar_setint(dev, "mpc", 1);
		if (cfg->ap_info) {
			kfree(cfg->ap_info->wpa_ie);
			kfree(cfg->ap_info->rsn_ie);
			kfree(cfg->ap_info->wps_ie);
			kfree(cfg->ap_info);
			cfg->ap_info = NULL;
		}
	} else {
		WL_DBG(("Stopping P2P GO \n"));
		DHD_OS_WAKE_LOCK_CTRL_TIMEOUT_ENABLE((dhd_pub_t *)(cfg->pub),
			DHD_EVENT_TIMEOUT_MS*3);
		DHD_OS_WAKE_LOCK_TIMEOUT((dhd_pub_t *)(cfg->pub));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

exit:
	return err;
}

static s32
wl_cfg80211_change_beacon(
	struct wiphy *wiphy,
	struct net_device *dev,
	struct cfg80211_beacon_data *info)
{
	s32 err = BCME_OK;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct parsed_ies ies;
	u32 dev_role = 0;
	s32 bssidx = 0;

	WL_DBG(("Enter \n"));

<<<<<<< HEAD
	if (dev == wl_to_prmry_ndev(wl)) {
		dev_role = NL80211_IFTYPE_AP;
	} else if (dev == wl->p2p_net) {
		/* Group Add request on p2p0 */
		dev = wl_to_prmry_ndev(wl);
		dev_role = NL80211_IFTYPE_P2P_GO;
	}

	bssidx = wl_cfgp2p_find_idx(wl, dev);
	if (p2p_is_on(wl) &&
		(bssidx == wl_to_p2p_bss_bssidx(wl,
=======
	if (dev == bcmcfg_to_prmry_ndev(cfg)) {
		dev_role = NL80211_IFTYPE_AP;
	}
#if defined(WL_ENABLE_P2P_IF)
	else if (dev == cfg->p2p_net) {
		/* Group Add request on p2p0 */
		dev = bcmcfg_to_prmry_ndev(cfg);
		dev_role = NL80211_IFTYPE_P2P_GO;
	}
#endif /* WL_ENABLE_P2P_IF */
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
	if (p2p_is_on(cfg) &&
		(bssidx == wl_to_p2p_bss_bssidx(cfg,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		P2PAPI_BSSCFG_CONNECTION))) {
		dev_role = NL80211_IFTYPE_P2P_GO;
	}

<<<<<<< HEAD
	/* Set IEs to FW */
	if ((err = wl_cfg80211_parse_set_ies(dev, info,
		&ies, dev_role, bssidx) < 0)) {
=======
	if (!check_dev_role_integrity(cfg, dev_role))
		goto fail;

	/* Parse IEs */
	if ((err = wl_cfg80211_parse_ap_ies(dev, info, &ies)) < 0) {
		WL_ERR(("Parse IEs failed \n"));
		goto fail;
	}

	/* Set IEs to FW */
	if ((err = wl_cfg80211_set_ies(dev, info, bssidx)) < 0) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("Set IEs failed \n"));
		goto fail;
	}

	if (dev_role == NL80211_IFTYPE_AP) {
		if (wl_cfg80211_hostapd_sec(dev, &ies, bssidx) < 0) {
			WL_ERR(("Hostapd update sec failed \n"));
			err = -EINVAL;
			goto fail;
		}
	}

fail:
	return err;
}
<<<<<<< HEAD
#else /* LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0) */
=======
#else
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32
wl_cfg80211_add_set_beacon(struct wiphy *wiphy, struct net_device *dev,
	struct beacon_parameters *info)
{
	s32 err = BCME_OK;
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 ie_offset = 0;
	s32 bssidx = 0;
	u32 dev_role = NL80211_IFTYPE_AP;
	struct parsed_ies ies;
	bcm_tlv_t *ssid_ie;
	bool pbc = 0;
	WL_DBG(("interval (%d) dtim_period (%d) head_len (%d) tail_len (%d)\n",
		info->interval, info->dtim_period, info->head_len, info->tail_len));

<<<<<<< HEAD
	if (dev == wl_to_prmry_ndev(wl)) {
		dev_role = NL80211_IFTYPE_AP;
	} else if (dev == wl->p2p_net) {
		/* Group Add request on p2p0 */
		dev = wl_to_prmry_ndev(wl);
		dev_role = NL80211_IFTYPE_P2P_GO;
	}

	bssidx = wl_cfgp2p_find_idx(wl, dev);
	if (p2p_is_on(wl) &&
		(bssidx == wl_to_p2p_bss_bssidx(wl,
=======
	if (dev == bcmcfg_to_prmry_ndev(cfg)) {
		dev_role = NL80211_IFTYPE_AP;
	}
#if defined(WL_ENABLE_P2P_IF)
	else if (dev == cfg->p2p_net) {
		/* Group Add request on p2p0 */
		dev = bcmcfg_to_prmry_ndev(cfg);
		dev_role = NL80211_IFTYPE_P2P_GO;
	}
#endif /* WL_ENABLE_P2P_IF */
	if (wl_cfgp2p_find_idx(cfg, dev, &bssidx) != BCME_OK) {
		WL_ERR(("Find p2p index from dev(%p) failed\n", dev));
		return BCME_ERROR;
	}
	if (p2p_is_on(cfg) &&
		(bssidx == wl_to_p2p_bss_bssidx(cfg,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		P2PAPI_BSSCFG_CONNECTION))) {
		dev_role = NL80211_IFTYPE_P2P_GO;
	}

<<<<<<< HEAD
=======
	if (!check_dev_role_integrity(cfg, dev_role))
		goto fail;

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	ie_offset = DOT11_MGMT_HDR_LEN + DOT11_BCN_PRB_FIXED_LEN;
	/* find the SSID */
	if ((ssid_ie = bcm_parse_tlvs((u8 *)&info->head[ie_offset],
		info->head_len - ie_offset,
		DOT11_MNG_SSID_ID)) != NULL) {
		if (dev_role == NL80211_IFTYPE_AP) {
			/* Store the hostapd SSID */
<<<<<<< HEAD
			memset(&wl->hostapd_ssid.SSID[0], 0x00, 32);
			memcpy(&wl->hostapd_ssid.SSID[0], ssid_ie->data, ssid_ie->len);
			wl->hostapd_ssid.SSID_len = ssid_ie->len;
		} else {
				/* P2P GO */
			memset(&wl->p2p->ssid.SSID[0], 0x00, 32);
			memcpy(wl->p2p->ssid.SSID, ssid_ie->data, ssid_ie->len);
			wl->p2p->ssid.SSID_len = ssid_ie->len;
=======
			memset(&cfg->hostapd_ssid.SSID[0], 0x00, 32);
			memcpy(&cfg->hostapd_ssid.SSID[0], ssid_ie->data, ssid_ie->len);
			cfg->hostapd_ssid.SSID_len = ssid_ie->len;
		} else {
				/* P2P GO */
			memset(&cfg->p2p->ssid.SSID[0], 0x00, 32);
			memcpy(cfg->p2p->ssid.SSID, ssid_ie->data, ssid_ie->len);
			cfg->p2p->ssid.SSID_len = ssid_ie->len;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	}

	if (wl_cfg80211_parse_ies((u8 *)info->tail,
		info->tail_len, &ies) < 0) {
		WL_ERR(("Beacon get IEs failed \n"));
		err = -EINVAL;
		goto fail;
	}

<<<<<<< HEAD
	if (wl_cfgp2p_set_management_ie(wl, dev, bssidx,
=======
	if (wl_cfgp2p_set_management_ie(cfg, dev, bssidx,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		VNDR_IE_BEACON_FLAG, (u8 *)info->tail,
		info->tail_len) < 0) {
		WL_ERR(("Beacon set IEs failed \n"));
		goto fail;
	} else {
		WL_DBG(("Applied Vndr IEs for Beacon \n"));
	}
	if (!wl_cfgp2p_bss_isup(dev, bssidx) &&
		(wl_cfg80211_bcn_validate_sec(dev, &ies, dev_role, bssidx) < 0))
	{
		WL_ERR(("Beacon set security failed \n"));
		goto fail;
	}

	/* Set BI and DTIM period */
	if (info->interval) {
		if ((err = wldev_ioctl(dev, WLC_SET_BCNPRD,
			&info->interval, sizeof(s32), true)) < 0) {
			WL_ERR(("Beacon Interval Set Error, %d\n", err));
			return err;
		}
	}
	if (info->dtim_period) {
		if ((err = wldev_ioctl(dev, WLC_SET_DTIMPRD,
			&info->dtim_period, sizeof(s32), true)) < 0) {
			WL_ERR(("DTIM Interval Set Error, %d\n", err));
			return err;
		}
	}

	if (wl_cfg80211_bcn_bringup_ap(dev, &ies, dev_role, bssidx) < 0) {
		WL_ERR(("Beacon bring up AP/GO failed \n"));
		goto fail;
	}

<<<<<<< HEAD
	if (wl_get_drv_status(wl, AP_CREATED, dev)) {
=======
	if (wl_get_drv_status(cfg, AP_CREATED, dev)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* Soft AP already running. Update changed params */
		if (wl_cfg80211_hostapd_sec(dev, &ies, bssidx) < 0) {
			WL_ERR(("Hostapd update sec failed \n"));
			err = -EINVAL;
			goto fail;
		}
	}

	/* Enable Probe Req filter */
	if (((dev_role == NL80211_IFTYPE_P2P_GO) ||
		(dev_role == NL80211_IFTYPE_AP)) && (ies.wps_ie != NULL)) {
		wl_validate_wps_ie((char *) ies.wps_ie, ies.wps_ie_len, &pbc);
		if (pbc)
			wl_add_remove_eventmsg(dev, WLC_E_PROBREQ_MSG, true);
	}

	WL_DBG(("** ADD/SET beacon done **\n"));

fail:
	if (err) {
		WL_ERR(("ADD/SET beacon failed\n"));
		wldev_iovar_setint(dev, "mpc", 1);
	}
	return err;

}
<<<<<<< HEAD
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0) */
=======
#endif /* LINUX_VERSION < VERSION(3,4,0) || WL_COMPAT_WIRELESS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#ifdef WL_SCHED_SCAN
#define PNO_TIME		30
#define PNO_REPEAT		4
#define PNO_FREQ_EXPO_MAX	2
<<<<<<< HEAD
int wl_cfg80211_sched_scan_start(struct wiphy *wiphy,
=======
static int
wl_cfg80211_sched_scan_start(struct wiphy *wiphy,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
                             struct net_device *dev,
                             struct cfg80211_sched_scan_request *request)
{
	ushort pno_time = PNO_TIME;
	int pno_repeat = PNO_REPEAT;
	int pno_freq_expo_max = PNO_FREQ_EXPO_MAX;
	wlc_ssid_t ssids_local[MAX_PFN_LIST_COUNT];
<<<<<<< HEAD
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct cfg80211_ssid *ssid = NULL;
	int ssid_count = 0;
	int i;
	int ret = 0;

	WL_DBG(("Enter \n"));
	WL_PNO((">>> SCHED SCAN START\n"));
	WL_PNO(("Enter n_match_sets:%d   n_ssids:%d \n",
		request->n_match_sets, request->n_ssids));
	WL_PNO(("ssids:%d pno_time:%d pno_repeat:%d pno_freq:%d \n",
		request->n_ssids, pno_time, pno_repeat, pno_freq_expo_max));


	if (!request || !request->n_ssids || !request->n_match_sets) {
		WL_ERR(("Invalid sched scan req!! n_ssids:%d \n", request->n_ssids));
		return -EINVAL;
	}

	memset(&ssids_local, 0, sizeof(ssids_local));

	if (request->n_match_sets > 0) {
		for (i = 0; i < request->n_match_sets; i++) {
			ssid = &request->match_sets[i].ssid;
			memcpy(ssids_local[i].SSID, ssid->ssid, ssid->ssid_len);
			ssids_local[i].SSID_len = ssid->ssid_len;
			WL_PNO((">>> PNO filter set for ssid (%s) \n", ssid->ssid));
			ssid_count++;
		}
	}

	if (request->n_ssids > 0) {
		for (i = 0; i < request->n_ssids; i++) {
			/* Active scan req for ssids */
			WL_PNO((">>> Active scan req for ssid (%s) \n", request->ssids[i].ssid));

			/* match_set ssids is a supert set of n_ssid list, so we need
			 * not add these set seperately
			 */
		}
	}

	if (ssid_count) {
<<<<<<< HEAD
		if ((ret = dhd_dev_pno_set(dev, ssids_local, request->n_match_sets,
			pno_time, pno_repeat, pno_freq_expo_max)) < 0) {
			WL_ERR(("PNO setup failed!! ret=%d \n", ret));
			return -EINVAL;
		}

		/* Enable the PNO */
		if (dhd_dev_pno_enable(dev, 1) < 0) {
			WL_ERR(("PNO enable failed!! ret=%d \n", ret));
			return -EINVAL;
		}
		wl->sched_scan_req = request;
=======
		if ((ret = dhd_dev_pno_set_for_ssid(dev, ssids_local, request->n_match_sets,
			pno_time, pno_repeat, pno_freq_expo_max, NULL, 0)) < 0) {
			WL_ERR(("PNO setup failed!! ret=%d \n", ret));
			return -EINVAL;
		}
		cfg->sched_scan_req = request;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	} else {
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
int wl_cfg80211_sched_scan_stop(struct wiphy *wiphy, struct net_device *dev)
{
	struct wl_priv *wl = wiphy_priv(wiphy);
=======
static int
wl_cfg80211_sched_scan_stop(struct wiphy *wiphy, struct net_device *dev)
{
	struct bcm_cfg80211 *cfg = wiphy_priv(wiphy);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	WL_DBG(("Enter \n"));
	WL_PNO((">>> SCHED SCAN STOP\n"));

<<<<<<< HEAD
	if (dhd_dev_pno_enable(dev, 0) < 0)
		WL_ERR(("PNO disable failed"));

	if (dhd_dev_pno_reset(dev) < 0)
		WL_ERR(("PNO reset failed"));

	if (wl->scan_request && wl->sched_scan_running) {
		WL_PNO((">>> Sched scan running. Aborting it..\n"));
		wl_notify_escan_complete(wl, dev, true, true);
	}

	 wl->sched_scan_req = NULL;
	 wl->sched_scan_running = FALSE;
=======
	if (dhd_dev_pno_stop_for_ssid(dev) < 0)
		WL_ERR(("PNO Stop for SSID failed"));

	if (cfg->scan_request && cfg->sched_scan_running) {
		WL_PNO((">>> Sched scan running. Aborting it..\n"));
		wl_notify_escan_complete(cfg, dev, true, true);
	}

	 cfg->sched_scan_req = NULL;
	 cfg->sched_scan_running = FALSE;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return 0;
}
#endif /* WL_SCHED_SCAN */

static struct cfg80211_ops wl_cfg80211_ops = {
	.add_virtual_intf = wl_cfg80211_add_virtual_iface,
	.del_virtual_intf = wl_cfg80211_del_virtual_iface,
	.change_virtual_intf = wl_cfg80211_change_virtual_iface,
<<<<<<< HEAD
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
	.start_p2p_device = wl_cfgp2p_start_p2p_device,
	.stop_p2p_device = wl_cfgp2p_stop_p2p_device,
#endif /* WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	.scan = wl_cfg80211_scan,
	.set_wiphy_params = wl_cfg80211_set_wiphy_params,
	.join_ibss = wl_cfg80211_join_ibss,
	.leave_ibss = wl_cfg80211_leave_ibss,
	.get_station = wl_cfg80211_get_station,
	.set_tx_power = wl_cfg80211_set_tx_power,
	.get_tx_power = wl_cfg80211_get_tx_power,
	.add_key = wl_cfg80211_add_key,
	.del_key = wl_cfg80211_del_key,
	.get_key = wl_cfg80211_get_key,
	.set_default_key = wl_cfg80211_config_default_key,
	.set_default_mgmt_key = wl_cfg80211_config_default_mgmt_key,
	.set_power_mgmt = wl_cfg80211_set_power_mgmt,
	.connect = wl_cfg80211_connect,
	.disconnect = wl_cfg80211_disconnect,
	.suspend = wl_cfg80211_suspend,
	.resume = wl_cfg80211_resume,
	.set_pmksa = wl_cfg80211_set_pmksa,
	.del_pmksa = wl_cfg80211_del_pmksa,
	.flush_pmksa = wl_cfg80211_flush_pmksa,
	.remain_on_channel = wl_cfg80211_remain_on_channel,
	.cancel_remain_on_channel = wl_cfg80211_cancel_remain_on_channel,
	.mgmt_tx = wl_cfg80211_mgmt_tx,
	.mgmt_frame_register = wl_cfg80211_mgmt_frame_register,
	.change_bss = wl_cfg80211_change_bss,
<<<<<<< HEAD
	.set_channel = wl_cfg80211_set_channel,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
=======
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)) || defined(WL_COMPAT_WIRELESS)
	.set_channel = wl_cfg80211_set_channel,
#endif /* ((LINUX_VERSION < VERSION(3, 6, 0)) || WL_COMPAT_WIRELESS */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)) && !defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	.set_beacon = wl_cfg80211_add_set_beacon,
	.add_beacon = wl_cfg80211_add_set_beacon,
#else
	.change_beacon = wl_cfg80211_change_beacon,
	.start_ap = wl_cfg80211_start_ap,
	.stop_ap = wl_cfg80211_stop_ap,
<<<<<<< HEAD
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0) */
#ifdef WL_SCHED_SCAN
	.sched_scan_start = wl_cfg80211_sched_scan_start,
	.sched_scan_stop = wl_cfg80211_sched_scan_stop,
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0) */
#if defined(WL_SUPPORT_BACKPORTED_KPATCHES) || (LINUX_VERSION_CODE >= KERNEL_VERSION(3, \
	2, 0))
	.del_station = wl_cfg80211_del_station,
	.mgmt_tx_cancel_wait = wl_cfg80211_mgmt_tx_cancel_wait,
#endif /* WL_SUPPORT_BACKPORTED_KPATCHES || KERNEL_VERSION >= (3,2,0) */
=======
#endif /* LINUX_VERSION < KERNEL_VERSION(3,4,0) && !WL_COMPAT_WIRELESS */
#ifdef WL_SCHED_SCAN
	.sched_scan_start = wl_cfg80211_sched_scan_start,
	.sched_scan_stop = wl_cfg80211_sched_scan_stop,
#endif /* WL_SCHED_SCAN */
#if defined(WL_SUPPORT_BACKPORTED_KPATCHES) || (LINUX_VERSION_CODE >= KERNEL_VERSION(3, \
	2, 0))
	.del_station = wl_cfg80211_del_station,
	.change_station = wl_cfg80211_change_station,
	.mgmt_tx_cancel_wait = wl_cfg80211_mgmt_tx_cancel_wait,
#endif /* WL_SUPPORT_BACKPORTED_KPATCHES || KERNEL_VERSION >= (3,2,0) */
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 2, 0)) || defined(WL_COMPAT_WIRELESS)
	.tdls_oper = wl_cfg80211_tdls_oper,
#endif /* LINUX_VERSION > VERSION(3, 2, 0) || WL_COMPAT_WIRELESS */
	CFG80211_TESTMODE_CMD(dhd_cfg80211_testmode_cmd)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
};

s32 wl_mode_to_nl80211_iftype(s32 mode)
{
	s32 err = 0;

	switch (mode) {
	case WL_MODE_BSS:
		return NL80211_IFTYPE_STATION;
	case WL_MODE_IBSS:
		return NL80211_IFTYPE_ADHOC;
	case WL_MODE_AP:
		return NL80211_IFTYPE_AP;
	default:
		return NL80211_IFTYPE_UNSPECIFIED;
	}

	return err;
}

<<<<<<< HEAD
static int
wl_cfg80211_reg_notifier(
	struct wiphy *wiphy,
	struct regulatory_request *request)
{
	struct wl_priv *wl = (struct wl_priv *)wiphy_priv(wiphy);
	wl_country_t cspec = {{0}, 0, {0} };
	int ret = 0;

	if (!request || !wl) {
		WL_ERR(("Invalid arg\n"));
		return -EINVAL;
	}

	WL_DBG(("ccode: %c%c Initiator: %d\n",
		request->alpha2[0], request->alpha2[1], request->initiator));

	/* We support only REGDOM_SET_BY_USER as of now */
	if (request->initiator != NL80211_REGDOM_SET_BY_USER) {
		WL_ERR(("reg_notifier for intiator:%d not supported \n",
			request->initiator));
		return -ENOTSUPP;
	}

	if (request->alpha2[0] == '0' && request->alpha2[1] == '0') {
		/* world domain */
		WL_ERR(("World domain. Setting XY/4 \n"));
		strncpy(cspec.country_abbrev, "XY", strlen("XY"));
		cspec.rev = 4;
	} else {
		memcpy(cspec.country_abbrev, request->alpha2, 2);
		cspec.country_abbrev[3] = '\0';
		cspec.rev = -1; /* Unspecified */
	}

	if ((ret = wldev_iovar_setbuf(wl_to_prmry_ndev(wl), "country", (char *)&cspec,
		sizeof(cspec), wl->ioctl_buf, WLC_IOCTL_SMLEN, NULL)) < 0) {
		WL_ERR(("set country Failed :%d\n", ret));
		goto exit;
	}

	if ((ret = wl_update_wiphybands(wl, false)) < 0) {
		WL_ERR(("wl_update_wiphybands failed\n"));
		goto exit;
	}

	WL_DBG(("%s: set country '%s/%d' done\n",
		__FUNCTION__, cspec.country_abbrev, cspec.rev));

exit:
	return ret;
}

static s32 wl_setup_wiphy(struct wireless_dev *wdev, struct device *sdiofunc_dev)
{
	s32 err = 0;
	wdev->wiphy =
	    wiphy_new(&wl_cfg80211_ops, sizeof(struct wl_priv));
=======

static s32 wl_setup_wiphy(struct wireless_dev *wdev, struct device *sdiofunc_dev, void *context)
{
	s32 err = 0;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0) || defined(WL_COMPAT_WIRELESS))
	dhd_pub_t *dhd = (dhd_pub_t *)context;
	BCM_REFERENCE(dhd);

	if (!dhd) {
		WL_ERR(("DHD is NULL!!"));
		err = -ENODEV;
		return err;
	}
#endif 

	wdev->wiphy =
	    wiphy_new(&wl_cfg80211_ops, sizeof(struct bcm_cfg80211));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (unlikely(!wdev->wiphy)) {
		WL_ERR(("Couldn not allocate wiphy device\n"));
		err = -ENOMEM;
		return err;
	}
	set_wiphy_dev(wdev->wiphy, sdiofunc_dev);
	wdev->wiphy->max_scan_ie_len = WL_SCAN_IE_LEN_MAX;
	/* Report  how many SSIDs Driver can support per Scan request */
	wdev->wiphy->max_scan_ssids = WL_SCAN_PARAMS_SSID_MAX;
	wdev->wiphy->max_num_pmkids = WL_NUM_PMKIDS_MAX;
#ifdef WL_SCHED_SCAN
	wdev->wiphy->max_sched_scan_ssids = MAX_PFN_LIST_COUNT;
	wdev->wiphy->max_match_sets = MAX_PFN_LIST_COUNT;
	wdev->wiphy->max_sched_scan_ie_len = WL_SCAN_IE_LEN_MAX;
	wdev->wiphy->flags |= WIPHY_FLAG_SUPPORTS_SCHED_SCAN;
#endif /* WL_SCHED_SCAN */
	wdev->wiphy->interface_modes =
		BIT(NL80211_IFTYPE_STATION)
<<<<<<< HEAD
#if !(defined(WLP2P) && defined(WL_ENABLE_P2P_IF))
		| BIT(NL80211_IFTYPE_MONITOR)
#endif
		| BIT(NL80211_IFTYPE_AP);

=======
		| BIT(NL80211_IFTYPE_ADHOC)
#if !defined(WL_ENABLE_P2P_IF)
		| BIT(NL80211_IFTYPE_MONITOR)
#endif /* !WL_ENABLE_P2P_IF */
#if defined(WL_IFACE_COMB_NUM_CHANNELS) || defined(WL_CFG80211_P2P_DEV_IF)
		| BIT(NL80211_IFTYPE_P2P_CLIENT)
		| BIT(NL80211_IFTYPE_P2P_GO)
#endif /* WL_IFACE_COMB_NUM_CHANNELS || WL_CFG80211_P2P_DEV_IF */
#if defined(WL_CFG80211_P2P_DEV_IF)
		| BIT(NL80211_IFTYPE_P2P_DEVICE)
#endif /* WL_CFG80211_P2P_DEV_IF */
		| BIT(NL80211_IFTYPE_AP);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)) && \
	(defined(WL_IFACE_COMB_NUM_CHANNELS) || defined(WL_CFG80211_P2P_DEV_IF))
	WL_DBG(("Setting interface combinations for common mode\n"));
	wdev->wiphy->iface_combinations = common_iface_combinations;
	wdev->wiphy->n_iface_combinations =
		ARRAY_SIZE(common_iface_combinations);
#endif /* LINUX_VER >= 3.0 && (WL_IFACE_COMB_NUM_CHANNELS || WL_CFG80211_P2P_DEV_IF) */

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	wdev->wiphy->bands[IEEE80211_BAND_2GHZ] = &__wl_band_2ghz;

	wdev->wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;
	wdev->wiphy->cipher_suites = __wl_cipher_suites;
	wdev->wiphy->n_cipher_suites = ARRAY_SIZE(__wl_cipher_suites);
	wdev->wiphy->max_remain_on_channel_duration = 5000;
	wdev->wiphy->mgmt_stypes = wl_cfg80211_default_mgmt_stypes;
#ifndef WL_POWERSAVE_DISABLED
	wdev->wiphy->flags |= WIPHY_FLAG_PS_ON_BY_DEFAULT;
#else
	wdev->wiphy->flags &= ~WIPHY_FLAG_PS_ON_BY_DEFAULT;
#endif				/* !WL_POWERSAVE_DISABLED */
	wdev->wiphy->flags |= WIPHY_FLAG_NETNS_OK |
		WIPHY_FLAG_4ADDR_AP |
<<<<<<< HEAD
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 39)
		WIPHY_FLAG_SUPPORTS_SEPARATE_DEFAULT_KEYS |
#endif
		WIPHY_FLAG_4ADDR_STATION;
	/*  If driver advertises FW_ROAM, the supplicant wouldn't
	 * send the BSSID & Freq in the connect command allowing the
	 * the driver to choose the AP to connect to. But unless we
	 * support ROAM_CACHE in firware this will delay the ASSOC as
	 * as the FW need to do a full scan before attempting to connect
	 * So that feature will just increase assoc. The better approach
	 * to let Supplicant to provide channel info and FW letter may roam
	 * if needed so DON'T advertise that featur eto Supplicant.
	 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
/*	wdev->wiphy->flags |= WIPHY_FLAG_SUPPORTS_FW_ROAM; */
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)
=======
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 39)) && !defined(WL_COMPAT_WIRELESS)
		WIPHY_FLAG_SUPPORTS_SEPARATE_DEFAULT_KEYS |
#endif
		WIPHY_FLAG_4ADDR_STATION;
#if (defined(ROAM_ENABLE) || defined(BCMFW_ROAM_ENABLE)) && ((LINUX_VERSION_CODE >= \
	KERNEL_VERSION(3, 2, 0)) || defined(WL_COMPAT_WIRELESS)) && !defined(CUSTOMER_HW4)
	/* Please use supplicant ver >= 76 if FW_ROAM is enabled
	 * If driver advertises FW_ROAM, older supplicant wouldn't
	 * send the BSSID & Freq in the connect req command. This
	 * will delay the ASSOC as the FW need to do a full scan
	 * before attempting to connect. Supplicant >=76 has patch
	 * to allow bssid & freq to be sent down to driver even if
	 * FW ROAM is advertised.
	 */
	wdev->wiphy->flags |= WIPHY_FLAG_SUPPORTS_FW_ROAM;
#endif /* ROAM_ENABLE && (LINUX_VERSION 3.2.0  || WL_COMPAT_WIRELESS) && !CUSTOMER_HW4 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)) || defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	wdev->wiphy->flags |= WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL |
		WIPHY_FLAG_OFFCHAN_TX;
#endif
#if defined(WL_SUPPORT_BACKPORTED_KPATCHES) || (LINUX_VERSION_CODE >= KERNEL_VERSION(3, \
	4, 0))
<<<<<<< HEAD
		/* From 3.4 kernel ownards AP_SME flag can be advertised
		  * to remove the patch from supplicant
		  */
		wdev->wiphy->flags |= WIPHY_FLAG_HAVE_AP_SME;
#endif

	wdev->wiphy->reg_notifier = wl_cfg80211_reg_notifier;
=======
	/* From 3.4 kernel ownards AP_SME flag can be advertised
	 * to remove the patch from supplicant
	 */
	wdev->wiphy->flags |= WIPHY_FLAG_HAVE_AP_SME;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0) || defined(WL_COMPAT_WIRELESS))
	/* Supplicant distinguish between the SoftAP mode and other
	 * modes (e.g. P2P, WPS, HS2.0) when it builds the probe
	 * response frame from Supplicant MR1 and Kernel 3.4.0 or
	 * later version. To add Vendor specific IE into the
	 * probe response frame in case of SoftAP mode,
	 * AP_PROBE_RESP_OFFLOAD flag is set to wiphy->flags variable.
	 */
	if (dhd_get_fw_mode(dhd->info) == DHD_FLAG_HOSTAP_MODE) {
		wdev->wiphy->flags |= WIPHY_FLAG_AP_PROBE_RESP_OFFLOAD;
		wdev->wiphy->probe_resp_offload = 0;
	}
#endif 
#endif /* WL_SUPPORT_BACKPORTED_KPATCHES) || (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) */


#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 2, 0)) || defined(WL_COMPAT_WIRELESS)
	wdev->wiphy->flags |= WIPHY_FLAG_SUPPORTS_TDLS;
#endif

#if defined(CONFIG_PM) && defined(WL_CFG80211_P2P_DEV_IF)
	/*
	 * From linux-3.10 kernel, wowlan packet filter is mandated to avoid the
	 * disconnection of connected network before suspend. So a dummy wowlan
	 * filter is configured for kernels linux-3.8 and above.
	 */
	wdev->wiphy->wowlan.flags = WIPHY_WOWLAN_ANY;
#endif /* CONFIG_PM && WL_CFG80211_P2P_DEV_IF */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	WL_DBG(("Registering custom regulatory)\n"));
	wdev->wiphy->flags |= WIPHY_FLAG_CUSTOM_REGULATORY;
	wiphy_apply_custom_regulatory(wdev->wiphy, &brcm_regdom);
<<<<<<< HEAD
=======

#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 13, 0)) || defined(WL_VENDOR_EXT_SUPPORT)
	WL_ERR(("Registering Vendor80211)\n"));
	err = wl_cfgvendor_attach(wdev->wiphy);
	if (unlikely(err < 0)) {
		WL_ERR(("Couldn not attach vendor commands (%d)\n", err));
	}
#endif /* (LINUX_VERSION_CODE > KERNEL_VERSION(3, 13, 0)) || defined(WL_VENDOR_EXT_SUPPORT) */


>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	/* Now we can register wiphy with cfg80211 module */
	err = wiphy_register(wdev->wiphy);
	if (unlikely(err < 0)) {
		WL_ERR(("Couldn not register wiphy device (%d)\n", err));
		wiphy_free(wdev->wiphy);
	}
<<<<<<< HEAD
	return err;
}

static void wl_free_wdev(struct wl_priv *wl)
{
	struct wireless_dev *wdev = wl->wdev;
=======

#if ((LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)) && (LINUX_VERSION_CODE <= \
	KERNEL_VERSION(3, 3, 0))) && defined(WL_IFACE_COMB_NUM_CHANNELS)
	wdev->wiphy->flags &= ~WIPHY_FLAG_ENFORCE_COMBINATIONS;
#endif

	return err;
}

static void wl_free_wdev(struct bcm_cfg80211 *cfg)
{
	struct wireless_dev *wdev = cfg->wdev;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct wiphy *wiphy;
	if (!wdev) {
		WL_ERR(("wdev is invalid\n"));
		return;
	}
	wiphy = wdev->wiphy;
<<<<<<< HEAD
	wiphy_unregister(wdev->wiphy);
	wdev->wiphy->dev.parent = NULL;

	wl_delete_all_netinfo(wl);
	wiphy_free(wiphy);
	/* PLEASE do NOT call any function after wiphy_free, the driver's private structure "wl",
=======

#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 13, 0)) || defined(WL_VENDOR_EXT_SUPPORT)
	wl_cfgvendor_detach(wdev->wiphy);
#endif /* if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 13, 0)) || defined(WL_VENDOR_EXT_SUPPORT) */

	wiphy_unregister(wdev->wiphy);
	wdev->wiphy->dev.parent = NULL;

	wl_delete_all_netinfo(cfg);
	wiphy_free(wiphy);
	/* PLEASE do NOT call any function after wiphy_free, the driver's private structure "cfg",
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	 * which is the private part of wiphy, has been freed in wiphy_free !!!!!!!!!!!
	 */
}

<<<<<<< HEAD
static s32 wl_inform_bss(struct wl_priv *wl)
=======
static s32 wl_inform_bss(struct bcm_cfg80211 *cfg)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	struct wl_scan_results *bss_list;
	struct wl_bss_info *bi = NULL;	/* must be initialized */
	s32 err = 0;
	s32 i;

<<<<<<< HEAD
	bss_list = wl->bss_list;
	WL_DBG(("scanned AP count (%d)\n", bss_list->count));
	bi = next_bss(bss_list, bi);
	for_each_bss(bss_list, bi, i) {
		err = wl_inform_single_bss(wl, bi, 0);
		if (unlikely(err))
			break;
	}
	return err;
}

static s32 wl_inform_single_bss(struct wl_priv *wl, struct wl_bss_info *bi, u8 is_roam_done)
{
	struct wiphy *wiphy = wl_to_wiphy(wl);
=======
	bss_list = cfg->bss_list;
	WL_DBG(("scanned AP count (%d)\n", bss_list->count));
#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
	reset_roam_cache();
#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */
	bi = next_bss(bss_list, bi);
	for_each_bss(bss_list, bi, i) {
#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
		add_roam_cache(bi);
#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */
		err = wl_inform_single_bss(cfg, bi);
		if (unlikely(err))
			break;
	}
#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
	/* print_roam_cache(); */
	update_roam_cache(cfg, ioctl_version);
#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */
	return err;
}

static s32 wl_inform_single_bss(struct bcm_cfg80211 *cfg, struct wl_bss_info *bi)
{
	struct wiphy *wiphy = bcmcfg_to_wiphy(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct ieee80211_mgmt *mgmt;
	struct ieee80211_channel *channel;
	struct ieee80211_supported_band *band;
	struct wl_cfg80211_bss_info *notif_bss_info;
<<<<<<< HEAD
	struct wl_scan_req *sr = wl_to_sr(wl);
=======
	struct wl_scan_req *sr = wl_to_sr(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct beacon_proberesp *beacon_proberesp;
	struct cfg80211_bss *cbss = NULL;
	s32 mgmt_type;
	s32 signal;
	u32 freq;
	s32 err = 0;
	gfp_t aflags;
<<<<<<< HEAD
	u8 *ie_offset = NULL;
=======
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (unlikely(dtoh32(bi->length) > WL_BSS_INFO_MAX)) {
		WL_DBG(("Beacon is larger than buffer. Discarding\n"));
		return err;
	}
	aflags = (in_atomic()) ? GFP_ATOMIC : GFP_KERNEL;
	notif_bss_info = kzalloc(sizeof(*notif_bss_info) + sizeof(*mgmt)
		- sizeof(u8) + WL_BSS_INFO_MAX, aflags);
	if (unlikely(!notif_bss_info)) {
		WL_ERR(("notif_bss_info alloc failed\n"));
		return -ENOMEM;
	}
	mgmt = (struct ieee80211_mgmt *)notif_bss_info->frame_buf;
	notif_bss_info->channel =
<<<<<<< HEAD
		bi->ctl_ch ? bi->ctl_ch : CHSPEC_CHANNEL(wl_chspec_driver_to_host(bi->chanspec));
=======
		bi->ctl_ch ? bi->ctl_ch : wf_chspec_ctlchan(wl_chspec_driver_to_host(bi->chanspec));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (notif_bss_info->channel <= CH_MAX_2G_CHANNEL)
		band = wiphy->bands[IEEE80211_BAND_2GHZ];
	else
		band = wiphy->bands[IEEE80211_BAND_5GHZ];
	if (!band) {
		WL_ERR(("No valid band"));
		kfree(notif_bss_info);
		return -EINVAL;
	}
<<<<<<< HEAD
	notif_bss_info->rssi = dtoh16(bi->RSSI) + RSSI_OFFSET;
	memcpy(mgmt->bssid, &bi->BSSID, ETHER_ADDR_LEN);
	mgmt_type = wl->active_scan ?
=======
	notif_bss_info->rssi = wl_rssi_offset(dtoh16(bi->RSSI));
	memcpy(mgmt->bssid, &bi->BSSID, ETHER_ADDR_LEN);
	mgmt_type = cfg->active_scan ?
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		IEEE80211_STYPE_PROBE_RESP : IEEE80211_STYPE_BEACON;
	if (!memcmp(bi->SSID, sr->ssid.SSID, bi->SSID_len)) {
	    mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT | mgmt_type);
	}
<<<<<<< HEAD
	beacon_proberesp = wl->active_scan ?
=======
	beacon_proberesp = cfg->active_scan ?
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		(struct beacon_proberesp *)&mgmt->u.probe_resp :
		(struct beacon_proberesp *)&mgmt->u.beacon;
	beacon_proberesp->timestamp = 0;
	beacon_proberesp->beacon_int = cpu_to_le16(bi->beacon_period);
	beacon_proberesp->capab_info = cpu_to_le16(bi->capability);
<<<<<<< HEAD
	wl_rst_ie(wl);

	ie_offset = ((u8 *) bi) + bi->ie_offset;

	if (is_roam_done && ((int)(*(ie_offset)) == WLAN_EID_SSID &&
		((int)(*(ie_offset+1)) == 0 || (int)(*(ie_offset+2)) == 0))) {
		u8 *ie_new_offset = NULL;
		uint8 ie_new_length;

		WL_ERR(("WAR trace: Changing the SSID Info, from beacon %d\n",
			bi->flags & WL_BSS_FLAGS_FROM_BEACON));

		ie_new_offset = (u8 *)kzalloc(WL_BSS_INFO_MAX, GFP_KERNEL);
		if (ie_new_offset) {
			*(ie_new_offset) = WLAN_EID_SSID;
			*(ie_new_offset+1) = bi->SSID_len;
			memcpy(ie_new_offset+2, bi->SSID, bi->SSID_len);
			ie_new_length = bi->ie_length - *(ie_offset+1) + bi->SSID_len;

			/* Copy the remaining IE apart from SSID IE from bi */
			memcpy(ie_new_offset+2 + bi->SSID_len,
				ie_offset+2 + *(ie_offset+1),
				bi->ie_length - 2 - *(ie_offset+1));
			wl_mrg_ie(wl, ie_new_offset, ie_new_length);
			kfree(ie_new_offset);
		} else {
			wl_mrg_ie(wl, ((u8 *) bi) + bi->ie_offset, bi->ie_length);
		}
	} else {
		wl_mrg_ie(wl, ((u8 *) bi) + bi->ie_offset, bi->ie_length);
	}

	wl_cp_ie(wl, beacon_proberesp->variable, WL_BSS_INFO_MAX -
		offsetof(struct wl_cfg80211_bss_info, frame_buf));
	notif_bss_info->frame_len = offsetof(struct ieee80211_mgmt,
		u.beacon.variable) + wl_get_ielen(wl);
=======
	wl_rst_ie(cfg);
	wl_update_hidden_ap_ie(bi, ((u8 *) bi) + bi->ie_offset, &bi->ie_length);
	wl_mrg_ie(cfg, ((u8 *) bi) + bi->ie_offset, bi->ie_length);
	wl_cp_ie(cfg, beacon_proberesp->variable, WL_BSS_INFO_MAX -
		offsetof(struct wl_cfg80211_bss_info, frame_buf));
	notif_bss_info->frame_len = offsetof(struct ieee80211_mgmt,
		u.beacon.variable) + wl_get_ielen(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 38) && !defined(WL_COMPAT_WIRELESS)
	freq = ieee80211_channel_to_frequency(notif_bss_info->channel);
	(void)band->band;
#else
	freq = ieee80211_channel_to_frequency(notif_bss_info->channel, band->band);
#endif
	if (freq == 0) {
		WL_ERR(("Invalid channel, fail to chcnage channel to freq\n"));
		kfree(notif_bss_info);
		return -EINVAL;
	}
	channel = ieee80211_get_channel(wiphy, freq);
	if (unlikely(!channel)) {
		WL_ERR(("ieee80211_get_channel error\n"));
		kfree(notif_bss_info);
		return -EINVAL;
	}
	WL_DBG(("SSID : \"%s\", rssi %d, channel %d, capability : 0x04%x, bssid %pM"
			"mgmt_type %d frame_len %d\n", bi->SSID,
			notif_bss_info->rssi, notif_bss_info->channel,
			mgmt->u.beacon.capab_info, &bi->BSSID, mgmt_type,
			notif_bss_info->frame_len));

	signal = notif_bss_info->rssi * 100;
	if (!mgmt->u.probe_resp.timestamp) {
<<<<<<< HEAD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39)
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		struct timespec ts;
		get_monotonic_boottime(&ts);
		mgmt->u.probe_resp.timestamp = ((u64)ts.tv_sec*1000000)
				+ ts.tv_nsec / 1000;
#else
		struct timeval tv;
		do_gettimeofday(&tv);
		mgmt->u.probe_resp.timestamp = ((u64)tv.tv_sec*1000000)
				+ tv.tv_usec;
#endif
	}


	cbss = cfg80211_inform_bss_frame(wiphy, channel, mgmt,
		le16_to_cpu(notif_bss_info->frame_len), signal, aflags);
	if (unlikely(!cbss)) {
		WL_ERR(("cfg80211_inform_bss_frame error\n"));
		kfree(notif_bss_info);
		return -EINVAL;
	}

<<<<<<< HEAD
	cfg80211_put_bss(cbss);
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
	cfg80211_put_bss(wiphy, cbss);
#else
	cfg80211_put_bss(cbss);
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0) */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	kfree(notif_bss_info);
	return err;
}

<<<<<<< HEAD
static bool wl_is_linkup(struct wl_priv *wl, const wl_event_msg_t *e, struct net_device *ndev)
=======
static bool wl_is_linkup(struct bcm_cfg80211 *cfg, const wl_event_msg_t *e, struct net_device *ndev)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	u32 event = ntoh32(e->event_type);
	u32 status =  ntoh32(e->status);
	u16 flags = ntoh16(e->flags);

	WL_DBG(("event %d, status %d flags %x\n", event, status, flags));
	if (event == WLC_E_SET_SSID) {
		if (status == WLC_E_STATUS_SUCCESS) {
<<<<<<< HEAD
			if (!wl_is_ibssmode(wl, ndev))
=======
			if (!wl_is_ibssmode(cfg, ndev))
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				return true;
		}
	} else if (event == WLC_E_LINK) {
		if (flags & WLC_EVENT_MSG_LINK)
			return true;
	}

	WL_DBG(("wl_is_linkup false\n"));
	return false;
}

<<<<<<< HEAD
static bool wl_is_linkdown(struct wl_priv *wl, const wl_event_msg_t *e)
=======
static bool wl_is_linkdown(struct bcm_cfg80211 *cfg, const wl_event_msg_t *e)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	u32 event = ntoh32(e->event_type);
	u16 flags = ntoh16(e->flags);

	if (event == WLC_E_DEAUTH_IND ||
	event == WLC_E_DISASSOC_IND ||
	event == WLC_E_DISASSOC ||
	event == WLC_E_DEAUTH) {
#if (WL_DBG_LEVEL > 0)
	WL_ERR(("Link down Reason : WLC_E_%s\n", wl_dbg_estr[event]));
#endif /* (WL_DBG_LEVEL > 0) */
		return true;
	} else if (event == WLC_E_LINK) {
		if (!(flags & WLC_EVENT_MSG_LINK)) {
#if (WL_DBG_LEVEL > 0)
	WL_ERR(("Link down Reason : WLC_E_%s\n", wl_dbg_estr[event]));
#endif /* (WL_DBG_LEVEL > 0) */
			return true;
		}
	}

	return false;
}

<<<<<<< HEAD
static bool wl_is_nonetwork(struct wl_priv *wl, const wl_event_msg_t *e)
=======
static bool wl_is_nonetwork(struct bcm_cfg80211 *cfg, const wl_event_msg_t *e)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	u32 event = ntoh32(e->event_type);
	u32 status = ntoh32(e->status);

	if (event == WLC_E_LINK && status == WLC_E_STATUS_NO_NETWORKS)
		return true;
	if (event == WLC_E_SET_SSID && status != WLC_E_STATUS_SUCCESS)
		return true;

	return false;
}

/* The mainline kernel >= 3.2.0 has support for indicating new/del station
 * to AP/P2P GO via events. If this change is backported to kernel for which
 * this driver is being built, then define WL_CFG80211_STA_EVENT. You
 * should use this new/del sta event mechanism for BRCM supplicant >= 22.
 */
static s32
<<<<<<< HEAD
wl_notify_connect_status_ap(struct wl_priv *wl, struct net_device *ndev,
=======
wl_notify_connect_status_ap(struct bcm_cfg80211 *cfg, struct net_device *ndev,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	const wl_event_msg_t *e, void *data)
{
	s32 err = 0;
	u32 event = ntoh32(e->event_type);
	u32 reason = ntoh32(e->reason);
	u32 len = ntoh32(e->datalen);

<<<<<<< HEAD
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0)) && !defined(WL_CFG80211_STA_EVENT)
=======
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0)) && !defined(WL_CFG80211_STA_EVENT) \
	&& !defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	bool isfree = false;
	u8 *mgmt_frame;
	u8 bsscfgidx = e->bsscfgidx;
	s32 freq;
	s32 channel;
	u8 *body = NULL;
	u16 fc = 0;

	struct ieee80211_supported_band *band;
	struct ether_addr da;
	struct ether_addr bssid;
<<<<<<< HEAD
	struct wiphy *wiphy = wl_to_wiphy(wl);
	channel_info_t ci;
#else
	struct station_info sinfo;
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0)) && !WL_CFG80211_STA_EVENT */
=======
	struct wiphy *wiphy = bcmcfg_to_wiphy(cfg);
	channel_info_t ci;
#else
	struct station_info sinfo;
#endif /* (LINUX_VERSION < VERSION(3,2,0)) && !WL_CFG80211_STA_EVENT && !WL_COMPAT_WIRELESS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	WL_DBG(("event %d status %d reason %d\n", event, ntoh32(e->status), reason));
	/* if link down, bsscfg is disabled. */
	if (event == WLC_E_LINK && reason == WLC_E_LINK_BSSCFG_DIS &&
<<<<<<< HEAD
		wl_get_p2p_status(wl, IF_DELETING) && (ndev != wl_to_prmry_ndev(wl))) {
		wl_add_remove_eventmsg(ndev, WLC_E_PROBREQ_MSG, false);
		WL_INFO(("AP mode link down !! \n"));
		complete(&wl->iface_disable);
		return 0;
	}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0)) && !defined(WL_CFG80211_STA_EVENT)
=======
		wl_get_p2p_status(cfg, IF_DELETING) && (ndev != bcmcfg_to_prmry_ndev(cfg))) {
		wl_add_remove_eventmsg(ndev, WLC_E_PROBREQ_MSG, false);
		WL_INFO(("AP mode link down !! \n"));
		complete(&cfg->iface_disable);
		return 0;
	}

	if (event == WLC_E_DISASSOC_IND || event == WLC_E_DEAUTH_IND || event == WLC_E_DEAUTH) {
		WL_ERR(("event %s(%d) status %d reason %d\n",
		bcmevent_names[event].name, event, ntoh32(e->status), reason));
	}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0)) && !defined(WL_CFG80211_STA_EVENT) \
	&& !defined(WL_COMPAT_WIRELESS)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	WL_DBG(("Enter \n"));
	if (!len && (event == WLC_E_DEAUTH)) {
		len = 2; /* reason code field */
		data = &reason;
	}
	if (len) {
		body = kzalloc(len, GFP_KERNEL);

		if (body == NULL) {
			WL_ERR(("wl_notify_connect_status: Failed to allocate body\n"));
			return WL_INVALID;
		}
	}
	memset(&bssid, 0, ETHER_ADDR_LEN);
	WL_DBG(("Enter event %d ndev %p\n", event, ndev));
<<<<<<< HEAD
	if (wl_get_mode_by_netdev(wl, ndev) == WL_INVALID) {
=======
	if (wl_get_mode_by_netdev(cfg, ndev) == WL_INVALID) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		kfree(body);
		return WL_INVALID;
	}
	if (len)
		memcpy(body, data, len);

	wldev_iovar_getbuf_bsscfg(ndev, "cur_etheraddr",
<<<<<<< HEAD
		NULL, 0, wl->ioctl_buf, WLC_IOCTL_SMLEN, bsscfgidx, &wl->ioctl_buf_sync);
	memcpy(da.octet, wl->ioctl_buf, ETHER_ADDR_LEN);
=======
		NULL, 0, cfg->ioctl_buf, WLC_IOCTL_SMLEN, bsscfgidx, &cfg->ioctl_buf_sync);
	memcpy(da.octet, cfg->ioctl_buf, ETHER_ADDR_LEN);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	err = wldev_ioctl(ndev, WLC_GET_BSSID, &bssid, ETHER_ADDR_LEN, false);
	switch (event) {
		case WLC_E_ASSOC_IND:
			fc = FC_ASSOC_REQ;
			break;
		case WLC_E_REASSOC_IND:
			fc = FC_REASSOC_REQ;
			break;
		case WLC_E_DISASSOC_IND:
			fc = FC_DISASSOC;
			break;
		case WLC_E_DEAUTH_IND:
			fc = FC_DISASSOC;
			break;
		case WLC_E_DEAUTH:
			fc = FC_DISASSOC;
			break;
		default:
			fc = 0;
			goto exit;
	}
	if ((err = wldev_ioctl(ndev, WLC_GET_CHANNEL, &ci, sizeof(ci), false))) {
		kfree(body);
		return err;
	}

	channel = dtoh32(ci.hw_channel);
	if (channel <= CH_MAX_2G_CHANNEL)
		band = wiphy->bands[IEEE80211_BAND_2GHZ];
	else
		band = wiphy->bands[IEEE80211_BAND_5GHZ];
	if (!band) {
		WL_ERR(("No valid band"));
		if (body)
			kfree(body);
		return -EINVAL;
	}
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 38) && !defined(WL_COMPAT_WIRELESS)
	freq = ieee80211_channel_to_frequency(channel);
	(void)band->band;
#else
	freq = ieee80211_channel_to_frequency(channel, band->band);
#endif

	err = wl_frame_get_mgmt(fc, &da, &e->addr, &bssid,
		&mgmt_frame, &len, body);
	if (err < 0)
		goto exit;
	isfree = true;

	if (event == WLC_E_ASSOC_IND && reason == DOT11_SC_SUCCESS) {
<<<<<<< HEAD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
		cfg80211_rx_mgmt(ndev, freq, 0, mgmt_frame, len, GFP_ATOMIC);
#else
		cfg80211_rx_mgmt(ndev, freq, mgmt_frame, len, GFP_ATOMIC);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0) */
	} else if (event == WLC_E_DISASSOC_IND) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
		cfg80211_rx_mgmt(ndev, freq, 0, mgmt_frame, len, GFP_ATOMIC);
#else
		cfg80211_rx_mgmt(ndev, freq, mgmt_frame, len, GFP_ATOMIC);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0) */
	} else if ((event == WLC_E_DEAUTH_IND) || (event == WLC_E_DEAUTH)) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
		cfg80211_rx_mgmt(ndev, freq, 0, mgmt_frame, len, GFP_ATOMIC);
#else
		cfg80211_rx_mgmt(ndev, freq, mgmt_frame, len, GFP_ATOMIC);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0) */
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) || defined(WL_COMPAT_WIRELESS)
		cfg80211_rx_mgmt(ndev, freq, 0, mgmt_frame, len, GFP_ATOMIC);
#else
		cfg80211_rx_mgmt(ndev, freq, mgmt_frame, len, GFP_ATOMIC);
#endif /* LINUX_VERSION >= VERSION(3,4,0) || WL_COMPAT_WIRELESS */
	} else if (event == WLC_E_DISASSOC_IND) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) || defined(WL_COMPAT_WIRELESS)
		cfg80211_rx_mgmt(ndev, freq, 0, mgmt_frame, len, GFP_ATOMIC);
#else
		cfg80211_rx_mgmt(ndev, freq, mgmt_frame, len, GFP_ATOMIC);
#endif /* LINUX_VERSION >= VERSION(3,4,0) || WL_COMPAT_WIRELESS */
	} else if ((event == WLC_E_DEAUTH_IND) || (event == WLC_E_DEAUTH)) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) || defined(WL_COMPAT_WIRELESS)
		cfg80211_rx_mgmt(ndev, freq, 0, mgmt_frame, len, GFP_ATOMIC);
#else
		cfg80211_rx_mgmt(ndev, freq, mgmt_frame, len, GFP_ATOMIC);
#endif /* LINUX_VERSION >= VERSION(3,4,0) || WL_COMPAT_WIRELESS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

exit:
	if (isfree)
		kfree(mgmt_frame);
	if (body)
		kfree(body);
<<<<<<< HEAD
	return err;
#else /* LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0) && !WL_CFG80211_STA_EVENT */
=======
#else /* LINUX_VERSION < VERSION(3,2,0) && !WL_CFG80211_STA_EVENT && !WL_COMPAT_WIRELESS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	sinfo.filled = 0;
	if (((event == WLC_E_ASSOC_IND) || (event == WLC_E_REASSOC_IND)) &&
		reason == DOT11_SC_SUCCESS) {
		sinfo.filled = STATION_INFO_ASSOC_REQ_IES;
		if (!data) {
			WL_ERR(("No IEs present in ASSOC/REASSOC_IND"));
			return -EINVAL;
		}
		sinfo.assoc_req_ies = data;
		sinfo.assoc_req_ies_len = len;
		cfg80211_new_sta(ndev, e->addr.octet, &sinfo, GFP_ATOMIC);
	} else if (event == WLC_E_DISASSOC_IND) {
		cfg80211_del_sta(ndev, e->addr.octet, GFP_ATOMIC);
	} else if ((event == WLC_E_DEAUTH_IND) || (event == WLC_E_DEAUTH)) {
		cfg80211_del_sta(ndev, e->addr.octet, GFP_ATOMIC);
	}
<<<<<<< HEAD
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0) && !WL_CFG80211_STA_EVENT */
=======
#endif /* LINUX_VERSION < VERSION(3,2,0) && !WL_CFG80211_STA_EVENT && !WL_COMPAT_WIRELESS */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}

static s32
<<<<<<< HEAD
wl_get_auth_assoc_status(struct wl_priv *wl, struct net_device *ndev,
=======
wl_get_auth_assoc_status(struct bcm_cfg80211 *cfg, struct net_device *ndev,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	const wl_event_msg_t *e)
{
	u32 reason = ntoh32(e->reason);
	u32 event = ntoh32(e->event_type);
<<<<<<< HEAD
	struct wl_security *sec = wl_read_prof(wl, ndev, WL_PROF_SEC);
=======
	struct wl_security *sec = wl_read_prof(cfg, ndev, WL_PROF_SEC);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	WL_DBG(("event type : %d, reason : %d\n", event, reason));
	if (sec) {
		switch (event) {
		case WLC_E_ASSOC:
		case WLC_E_AUTH:
				sec->auth_assoc_res_status = reason;
		default:
			break;
		}
	} else
		WL_ERR(("sec is NULL\n"));
	return 0;
}

static s32
<<<<<<< HEAD
wl_notify_connect_status(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	bool act;
	s32 err = 0;
	u32 event = ntoh32(e->event_type);

	if (wl_get_mode_by_netdev(wl, ndev) == WL_MODE_AP) {
		wl_notify_connect_status_ap(wl, ndev, e, data);
	} else {
		WL_DBG(("wl_notify_connect_status : event %d status : %d ndev %p\n",
			ntoh32(e->event_type), ntoh32(e->status), ndev));
		if (event == WLC_E_ASSOC || event == WLC_E_AUTH) {
			wl_get_auth_assoc_status(wl, ndev, e);
			return err;
		}
		if (wl_is_linkup(wl, e, ndev)) {
			wl_link_up(wl);
			act = true;
			if (wl_is_ibssmode(wl, ndev)) {
				printk("cfg80211_ibss_joined\n");
				cfg80211_ibss_joined(ndev, (s8 *)&e->addr,
					GFP_KERNEL);
				WL_DBG(("joined in IBSS network\n"));
			} else {
				if (!wl_get_drv_status(wl, DISCONNECTING, ndev)) {
					printk("wl_bss_connect_done succeeded with " MACDBG "\n",
						MAC2STRDBG((u8*)(&e->addr)));
					wl_bss_connect_done(wl, ndev, e, data, true);
					WL_DBG(("joined in BSS network \"%s\"\n",
					((struct wlc_ssid *)
					 wl_read_prof(wl, ndev, WL_PROF_SSID))->SSID));
				}
			}
			wl_update_prof(wl, ndev, e, &act, WL_PROF_ACT);
			wl_update_prof(wl, ndev, NULL, (void *)&e->addr, WL_PROF_BSSID);

		} else if (wl_is_linkdown(wl, e)) {
			if (wl->scan_request) {
				if (wl->escan_on) {
					wl_notify_escan_complete(wl, ndev, true, true);
				} else {
					del_timer_sync(&wl->scan_timeout);
					wl_iscan_aborted(wl);
				}
			}
			if (wl_get_drv_status(wl, CONNECTED, ndev)) {
				scb_val_t scbval;
				u8 *curbssid = wl_read_prof(wl, ndev, WL_PROF_BSSID);
=======
wl_notify_connect_status_ibss(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	s32 err = 0;
	u32 event = ntoh32(e->event_type);
	u16 flags = ntoh16(e->flags);
	u32 status =  ntoh32(e->status);
	bool active;

	if (event == WLC_E_JOIN) {
		WL_DBG(("joined in IBSS network\n"));
	}
	if (event == WLC_E_START) {
		WL_DBG(("started IBSS network\n"));
	}
	if (event == WLC_E_JOIN || event == WLC_E_START ||
		(event == WLC_E_LINK && (flags == WLC_EVENT_MSG_LINK))) {
		if (wl_get_drv_status(cfg, CONNECTED, ndev)) {
			/* ROAM or Redundant */
			u8 *cur_bssid = wl_read_prof(cfg, ndev, WL_PROF_BSSID);
			if (memcmp(cur_bssid, &e->addr, ETHER_ADDR_LEN) == 0) {
				WL_DBG(("IBSS connected event from same BSSID("
					MACDBG "), ignore it\n", MAC2STRDBG(cur_bssid)));
				return err;
			}
			WL_INFO(("IBSS BSSID is changed from " MACDBG " to " MACDBG "\n",
				MAC2STRDBG(cur_bssid), MAC2STRDBG((u8 *)&e->addr)));
			wl_get_assoc_ies(cfg, ndev);
			wl_update_prof(cfg, ndev, NULL, (void *)&e->addr, WL_PROF_BSSID);
			wl_update_bss_info(cfg, ndev);
			cfg80211_ibss_joined(ndev, (s8 *)&e->addr, GFP_KERNEL);
		}
		else {
			/* New connection */
			WL_INFO(("IBSS connected to " MACDBG "\n", MAC2STRDBG((u8 *)&e->addr)));
			wl_link_up(cfg);
			wl_get_assoc_ies(cfg, ndev);
			wl_update_prof(cfg, ndev, NULL, (void *)&e->addr, WL_PROF_BSSID);
			wl_update_bss_info(cfg, ndev);
			cfg80211_ibss_joined(ndev, (s8 *)&e->addr, GFP_KERNEL);
			wl_set_drv_status(cfg, CONNECTED, ndev);
			active = true;
			wl_update_prof(cfg, ndev, NULL, (void *)&active, WL_PROF_ACT);
		}
	} else if ((event == WLC_E_LINK && !(flags & WLC_EVENT_MSG_LINK)) ||
		event == WLC_E_DEAUTH_IND || event == WLC_E_DISASSOC_IND) {
		wl_clr_drv_status(cfg, CONNECTED, ndev);
		wl_link_down(cfg);
		wl_init_prof(cfg, ndev);
	}
	else if (event == WLC_E_SET_SSID && status == WLC_E_STATUS_NO_NETWORKS) {
		WL_DBG(("no action - join fail (IBSS mode)\n"));
	}
	else {
		WL_DBG(("no action (IBSS mode)\n"));
}
	return err;
}

static s32
wl_notify_connect_status(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data)
{
	bool act;
	struct net_device *ndev = NULL;
	s32 err = 0;
	u32 event = ntoh32(e->event_type);

	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	if (wl_get_mode_by_netdev(cfg, ndev) == WL_MODE_AP) {
		err = wl_notify_connect_status_ap(cfg, ndev, e, data);
	} else if (wl_get_mode_by_netdev(cfg, ndev) == WL_MODE_IBSS) {
		err = wl_notify_connect_status_ibss(cfg, ndev, e, data);
	} else if (wl_get_mode_by_netdev(cfg, ndev) == WL_MODE_BSS) {
		WL_DBG(("wl_notify_connect_status : event %d status : %d ndev %p\n",
			ntoh32(e->event_type), ntoh32(e->status), ndev));
		if (event == WLC_E_ASSOC || event == WLC_E_AUTH) {
			wl_get_auth_assoc_status(cfg, ndev, e);
			return 0;
		}
		if (wl_is_linkup(cfg, e, ndev)) {
			wl_link_up(cfg);
			act = true;
			if (!wl_get_drv_status(cfg, DISCONNECTING, ndev)) {
					printk("wl_bss_connect_done succeeded with " MACDBG "\n",
						MAC2STRDBG((u8*)(&e->addr)));
					wl_bss_connect_done(cfg, ndev, e, data, true);
					WL_DBG(("joined in BSS network \"%s\"\n",
					((struct wlc_ssid *)
					 wl_read_prof(cfg, ndev, WL_PROF_SSID))->SSID));
				}
			wl_update_prof(cfg, ndev, e, &act, WL_PROF_ACT);
			wl_update_prof(cfg, ndev, NULL, (void *)&e->addr, WL_PROF_BSSID);

		} else if (wl_is_linkdown(cfg, e)) {
			if (cfg->scan_request)
				wl_notify_escan_complete(cfg, ndev, true, true);
			if (wl_get_drv_status(cfg, CONNECTED, ndev)) {
				scb_val_t scbval;
				u8 *curbssid = wl_read_prof(cfg, ndev, WL_PROF_BSSID);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				s32 reason = 0;
				if (event == WLC_E_DEAUTH_IND || event == WLC_E_DISASSOC_IND)
					reason = ntoh32(e->reason);
				/* WLAN_REASON_UNSPECIFIED is used for hang up event in Android */
				reason = (reason == WLAN_REASON_UNSPECIFIED)? 0 : reason;

				printk("link down if %s may call cfg80211_disconnected. "
					"event : %d, reason=%d from " MACDBG "\n",
					ndev->name, event, ntoh32(e->reason),
					MAC2STRDBG((u8*)(&e->addr)));
<<<<<<< HEAD
				if (memcmp(curbssid, &e->addr, ETHER_ADDR_LEN) != 0) {
=======
				if (!cfg->roam_offload &&
					memcmp(curbssid, &e->addr, ETHER_ADDR_LEN) != 0) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
					WL_ERR(("BSSID of event is not the connected BSSID"
						"(ignore it) cur: " MACDBG " event: " MACDBG"\n",
						MAC2STRDBG(curbssid), MAC2STRDBG((u8*)(&e->addr))));
					return 0;
				}
<<<<<<< HEAD
				wl_clr_drv_status(wl, CONNECTED, ndev);
				if (! wl_get_drv_status(wl, DISCONNECTING, ndev)) {
=======
				wl_clr_drv_status(cfg, CONNECTED, ndev);
				if (! wl_get_drv_status(cfg, DISCONNECTING, ndev)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
					/* To make sure disconnect, explictly send dissassoc
					*  for BSSID 00:00:00:00:00:00 issue
					*/
					scbval.val = WLAN_REASON_DEAUTH_LEAVING;

					memcpy(&scbval.ea, curbssid, ETHER_ADDR_LEN);
					scbval.val = htod32(scbval.val);
					err = wldev_ioctl(ndev, WLC_DISASSOC, &scbval,
						sizeof(scb_val_t), true);
					if (err < 0) {
						WL_ERR(("WLC_DISASSOC error %d\n", err));
						err = 0;
					}
					cfg80211_disconnected(ndev, reason, NULL, 0, GFP_KERNEL);
<<<<<<< HEAD
					wl_link_down(wl);
					wl_init_prof(wl, ndev);
				}
			}
			else if (wl_get_drv_status(wl, CONNECTING, ndev)) {
=======
					wl_link_down(cfg);
					wl_init_prof(cfg, ndev);
				}
			}
			else if (wl_get_drv_status(cfg, CONNECTING, ndev)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				printk("link down, during connecting\n");
#ifdef ESCAN_RESULT_PATCH
				if ((memcmp(connect_req_bssid, broad_bssid, ETHER_ADDR_LEN) == 0) ||
					(memcmp(&e->addr, broad_bssid, ETHER_ADDR_LEN) == 0) ||
					(memcmp(&e->addr, connect_req_bssid, ETHER_ADDR_LEN) == 0))
					/* In case this event comes while associating another AP */
#endif /* ESCAN_RESULT_PATCH */
<<<<<<< HEAD
					wl_bss_connect_done(wl, ndev, e, data, false);
			}
			wl_clr_drv_status(wl, DISCONNECTING, ndev);

			/* if link down, bsscfg is diabled */
			if (ndev != wl_to_prmry_ndev(wl))
				complete(&wl->iface_disable);

		} else if (wl_is_nonetwork(wl, e)) {
			printk("connect failed event=%d e->status %d e->reason %d \n",
				event, (int)ntoh32(e->status), (int)ntoh32(e->reason));
			/* Clean up any pending scan request */
			if (wl->scan_request) {
				if (wl->escan_on) {
					wl_notify_escan_complete(wl, ndev, true, true);
				} else {
					del_timer_sync(&wl->scan_timeout);
					wl_iscan_aborted(wl);
				}
			}
			if (wl_get_drv_status(wl, CONNECTING, ndev))
				wl_bss_connect_done(wl, ndev, e, data, false);
		} else {
			printk("%s nothing\n", __FUNCTION__);
		}
	}
	return err;
}

static s32
wl_notify_roaming_status(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	bool act;
=======
					wl_bss_connect_done(cfg, ndev, e, data, false);
			}
			wl_clr_drv_status(cfg, DISCONNECTING, ndev);

			/* if link down, bsscfg is diabled */
			if (ndev != bcmcfg_to_prmry_ndev(cfg))
				complete(&cfg->iface_disable);

		} else if (wl_is_nonetwork(cfg, e)) {
			printk("connect failed event=%d e->status %d e->reason %d \n",
				event, (int)ntoh32(e->status), (int)ntoh32(e->reason));
			/* Clean up any pending scan request */
			if (cfg->scan_request)
				wl_notify_escan_complete(cfg, ndev, true, true);
			if (wl_get_drv_status(cfg, CONNECTING, ndev))
				wl_bss_connect_done(cfg, ndev, e, data, false);
		} else {
			WL_DBG(("%s nothing\n", __FUNCTION__));
		}
	}
		else {
		WL_ERR(("Invalid ndev status %d\n", wl_get_mode_by_netdev(cfg, ndev)));
	}
	return err;
}

#ifdef WLAIBSS
void wl_cfg80211_set_txfail_pid(int pid)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	if (pid > 0)
		cfg->aibss_txfail_pid = pid;
	WL_DBG(("txfail : pid=%d\n", pid));
}

static s32
wl_notify_aibss_txfail(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data)
{
	u32 evt = ntoh32(e->event_type);
	int ret = -1;

	if (cfg->aibss_txfail_pid != 0) {
		ret = wl_netlink_send_msg(cfg->aibss_txfail_pid, cfg->aibss_txfail_seq++,
			(void *)&e->addr, ETHER_ADDR_LEN);
	}

	WL_DBG(("txfail : evt=%d, pid=%d, ret=%d, mac=" MACF "\n",
		evt, cfg->aibss_txfail_pid, ret, ETHERP_TO_MACF(&e->addr)));
	return ret;
}
#endif /* WLAIBSS */

static s32
wl_notify_roaming_status(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data)
{
	bool act;
	struct net_device *ndev = NULL;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = 0;
	u32 event = be32_to_cpu(e->event_type);
	u32 status = be32_to_cpu(e->status);
	WL_DBG(("Enter \n"));
<<<<<<< HEAD
	if (event == WLC_E_ROAM && status == WLC_E_STATUS_SUCCESS) {
		if (wl_get_drv_status(wl, CONNECTED, ndev))
			wl_bss_roaming_done(wl, ndev, e, data);
		else
			wl_bss_connect_done(wl, ndev, e, data, true);
		act = true;
		wl_update_prof(wl, ndev, e, &act, WL_PROF_ACT);
		wl_update_prof(wl, ndev, NULL, (void *)&e->addr, WL_PROF_BSSID);
=======

	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	if ((!cfg->disable_roam_event) && (event == WLC_E_BSSID)) {
		wl_add_remove_eventmsg(ndev, WLC_E_ROAM, false);
		cfg->disable_roam_event = TRUE;
	}

	if ((cfg->disable_roam_event) && (event == WLC_E_ROAM))
		return err;

	if ((event == WLC_E_ROAM || event == WLC_E_BSSID) && status == WLC_E_STATUS_SUCCESS) {
		if (wl_get_drv_status(cfg, CONNECTED, ndev))
			wl_bss_roaming_done(cfg, ndev, e, data);
		else
			wl_bss_connect_done(cfg, ndev, e, data, true);
		act = true;
		wl_update_prof(cfg, ndev, e, &act, WL_PROF_ACT);
		wl_update_prof(cfg, ndev, NULL, (void *)&e->addr, WL_PROF_BSSID);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
	return err;
}

<<<<<<< HEAD
static s32 wl_get_assoc_ies(struct wl_priv *wl, struct net_device *ndev)
{
	wl_assoc_info_t assoc_info;
	struct wl_connect_info *conn_info = wl_to_conn(wl);
	s32 err = 0;

	WL_DBG(("Enter \n"));
	err = wldev_iovar_getbuf(ndev, "assoc_info", NULL, 0, wl->extra_buf,
=======
static s32 wl_get_assoc_ies(struct bcm_cfg80211 *cfg, struct net_device *ndev)
{
	wl_assoc_info_t assoc_info;
	struct wl_connect_info *conn_info = wl_to_conn(cfg);
	s32 err = 0;

	WL_DBG(("Enter \n"));
	err = wldev_iovar_getbuf(ndev, "assoc_info", NULL, 0, cfg->extra_buf,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ASSOC_INFO_MAX, NULL);
	if (unlikely(err)) {
		WL_ERR(("could not get assoc info (%d)\n", err));
		return err;
	}
<<<<<<< HEAD
	memcpy(&assoc_info, wl->extra_buf, sizeof(wl_assoc_info_t));
=======
	memcpy(&assoc_info, cfg->extra_buf, sizeof(wl_assoc_info_t));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	assoc_info.req_len = htod32(assoc_info.req_len);
	assoc_info.resp_len = htod32(assoc_info.resp_len);
	assoc_info.flags = htod32(assoc_info.flags);
	if (conn_info->req_ie_len) {
		conn_info->req_ie_len = 0;
		bzero(conn_info->req_ie, sizeof(conn_info->req_ie));
	}
	if (conn_info->resp_ie_len) {
		conn_info->resp_ie_len = 0;
		bzero(conn_info->resp_ie, sizeof(conn_info->resp_ie));
	}
	if (assoc_info.req_len) {
<<<<<<< HEAD
		err = wldev_iovar_getbuf(ndev, "assoc_req_ies", NULL, 0, wl->extra_buf,
=======
		err = wldev_iovar_getbuf(ndev, "assoc_req_ies", NULL, 0, cfg->extra_buf,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_ASSOC_INFO_MAX, NULL);
		if (unlikely(err)) {
			WL_ERR(("could not get assoc req (%d)\n", err));
			return err;
		}
		conn_info->req_ie_len = assoc_info.req_len - sizeof(struct dot11_assoc_req);
		if (assoc_info.flags & WLC_ASSOC_REQ_IS_REASSOC) {
			conn_info->req_ie_len -= ETHER_ADDR_LEN;
		}
		if (conn_info->req_ie_len <= MAX_REQ_LINE)
<<<<<<< HEAD
			memcpy(conn_info->req_ie, wl->extra_buf, conn_info->req_ie_len);
		else {
			WL_ERR(("%s IE size %d above max %d size \n",
				__FUNCTION__, conn_info->req_ie_len, MAX_REQ_LINE));
=======
			memcpy(conn_info->req_ie, cfg->extra_buf, conn_info->req_ie_len);
		else {
			WL_ERR(("IE size %d above max %d size \n",
				conn_info->req_ie_len, MAX_REQ_LINE));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			return err;
		}
	} else {
		conn_info->req_ie_len = 0;
	}
	if (assoc_info.resp_len) {
<<<<<<< HEAD
		err = wldev_iovar_getbuf(ndev, "assoc_resp_ies", NULL, 0, wl->extra_buf,
=======
		err = wldev_iovar_getbuf(ndev, "assoc_resp_ies", NULL, 0, cfg->extra_buf,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_ASSOC_INFO_MAX, NULL);
		if (unlikely(err)) {
			WL_ERR(("could not get assoc resp (%d)\n", err));
			return err;
		}
		conn_info->resp_ie_len = assoc_info.resp_len -sizeof(struct dot11_assoc_resp);
		if (conn_info->resp_ie_len <= MAX_REQ_LINE)
<<<<<<< HEAD
			memcpy(conn_info->resp_ie, wl->extra_buf, conn_info->resp_ie_len);
		else {
			WL_ERR(("%s IE size %d above max %d size \n",
				__FUNCTION__, conn_info->resp_ie_len, MAX_REQ_LINE));
=======
			memcpy(conn_info->resp_ie, cfg->extra_buf, conn_info->resp_ie_len);
		else {
			WL_ERR(("IE size %d above max %d size \n",
				conn_info->resp_ie_len, MAX_REQ_LINE));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			return err;
		}
	} else {
		conn_info->resp_ie_len = 0;
	}
	WL_DBG(("req len (%d) resp len (%d)\n", conn_info->req_ie_len,
		conn_info->resp_ie_len));

	return err;
}

static void wl_ch_to_chanspec(int ch, struct wl_join_params *join_params,
        size_t *join_params_size)
{
<<<<<<< HEAD
	chanspec_t chanspec = 0;
	if (ch != 0) {
=======
#ifndef ROAM_CHANNEL_CACHE
	chanspec_t chanspec = 0;
#endif
	if (ch != 0) {
#if defined(CUSTOMER_HW4) && defined(ROAM_CHANNEL_CACHE)
		int n_channels;

		n_channels = get_roam_channel_list(ch, join_params->params.chanspec_list,
			&join_params->ssid, ioctl_version);
		join_params->params.chanspec_num = htod32(n_channels);
		*join_params_size += WL_ASSOC_PARAMS_FIXED_SIZE +
			join_params->params.chanspec_num * sizeof(chanspec_t);
#else
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		join_params->params.chanspec_num = 1;
		join_params->params.chanspec_list[0] = ch;

		if (join_params->params.chanspec_list[0] <= CH_MAX_2G_CHANNEL)
			chanspec |= WL_CHANSPEC_BAND_2G;
		else
			chanspec |= WL_CHANSPEC_BAND_5G;

		chanspec |= WL_CHANSPEC_BW_20;
		chanspec |= WL_CHANSPEC_CTL_SB_NONE;

		*join_params_size += WL_ASSOC_PARAMS_FIXED_SIZE +
			join_params->params.chanspec_num * sizeof(chanspec_t);

		join_params->params.chanspec_list[0]  &= WL_CHANSPEC_CHAN_MASK;
		join_params->params.chanspec_list[0] |= chanspec;
		join_params->params.chanspec_list[0] =
			wl_chspec_host_to_driver(join_params->params.chanspec_list[0]);

		join_params->params.chanspec_num =
			htod32(join_params->params.chanspec_num);
<<<<<<< HEAD
=======

#endif /* CUSTOMER_HW4 && ROAM_CHANNEL_CACHE */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_DBG(("join_params->params.chanspec_list[0]= %X, %d channels\n",
			join_params->params.chanspec_list[0],
			join_params->params.chanspec_num));
	}
}

<<<<<<< HEAD
static s32 wl_update_bss_info(struct wl_priv *wl, struct net_device *ndev, u8 is_roam_done)
=======
static s32 wl_update_bss_info(struct bcm_cfg80211 *cfg, struct net_device *ndev)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	struct cfg80211_bss *bss;
	struct wl_bss_info *bi;
	struct wlc_ssid *ssid;
	struct bcm_tlv *tim;
	s32 beacon_interval;
	s32 dtim_period;
	size_t ie_len;
	u8 *ie;
<<<<<<< HEAD
	u8 *ssidie;
	u8 *curbssid;
	s32 err = 0;
	struct wiphy *wiphy;

	wiphy = wl_to_wiphy(wl);

	if (wl_is_ibssmode(wl, ndev))
		return err;

	ssid = (struct wlc_ssid *)wl_read_prof(wl, ndev, WL_PROF_SSID);
	curbssid = wl_read_prof(wl, ndev, WL_PROF_BSSID);
=======
	u8 *curbssid;
	s32 err = 0;
	struct wiphy *wiphy;
	u32 channel;
#ifdef	ROAM_CHANNEL_CACHE
	struct ieee80211_channel *cur_channel;
	u32 freq, band;
#endif /* ROAM_CHANNEL_CACHE */

	wiphy = bcmcfg_to_wiphy(cfg);

	ssid = (struct wlc_ssid *)wl_read_prof(cfg, ndev, WL_PROF_SSID);
	curbssid = wl_read_prof(cfg, ndev, WL_PROF_BSSID);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	bss = cfg80211_get_bss(wiphy, NULL, curbssid,
		ssid->SSID, ssid->SSID_len, WLAN_CAPABILITY_ESS,
		WLAN_CAPABILITY_ESS);

<<<<<<< HEAD
	mutex_lock(&wl->usr_sync);
	if (!bss) {
		WL_DBG(("Could not find the AP\n"));
		*(u32 *) wl->extra_buf = htod32(WL_EXTRA_BUF_MAX);
		err = wldev_ioctl(ndev, WLC_GET_BSS_INFO,
			wl->extra_buf, WL_EXTRA_BUF_MAX, false);
		if (unlikely(err)) {
			WL_ERR(("Could not get bss info %d\n", err));
			goto update_bss_info_out;
		}
		bi = (struct wl_bss_info *)(wl->extra_buf + 4);
		if (memcmp(bi->BSSID.octet, curbssid, ETHER_ADDR_LEN)) {
			err = -EIO;
			goto update_bss_info_out;
		}

		ie = ((u8 *)bi) + bi->ie_offset;
		ie_len = bi->ie_length;
		ssidie = (u8 *)cfg80211_find_ie(WLAN_EID_SSID, ie, ie_len);
		if (ssidie && ssidie[1] == bi->SSID_len && !ssidie[2] && bi->SSID[0])
			memcpy(ssidie + 2, bi->SSID, bi->SSID_len);

		err = wl_inform_single_bss(wl, bi, is_roam_done);
=======
	mutex_lock(&cfg->usr_sync);

	*(u32 *) cfg->extra_buf = htod32(WL_EXTRA_BUF_MAX);
	err = wldev_ioctl(ndev, WLC_GET_BSS_INFO,
		cfg->extra_buf, WL_EXTRA_BUF_MAX, false);
	if (unlikely(err)) {
		WL_ERR(("Could not get bss info %d\n", err));
		goto update_bss_info_out;
	}
	bi = (struct wl_bss_info *)(cfg->extra_buf + 4);
	channel = bi->ctl_ch ? bi->ctl_ch :
		CHSPEC_CHANNEL(wl_chspec_driver_to_host(bi->chanspec));
	wl_update_prof(cfg, ndev, NULL, &channel, WL_PROF_CHAN);

	if (!bss) {
		WL_DBG(("Could not find the AP\n"));
		if (memcmp(bi->BSSID.octet, curbssid, ETHER_ADDR_LEN)) {
			WL_ERR(("Bssid doesn't match\n"));
			err = -EIO;
			goto update_bss_info_out;
		}
		err = wl_inform_single_bss(cfg, bi);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (unlikely(err))
			goto update_bss_info_out;

		ie = ((u8 *)bi) + bi->ie_offset;
		ie_len = bi->ie_length;
		beacon_interval = cpu_to_le16(bi->beacon_period);
	} else {
		WL_DBG(("Found the AP in the list - BSSID %pM\n", bss->bssid));
<<<<<<< HEAD
		ie = bss->information_elements;
		ie_len = bss->len_information_elements;
		beacon_interval = bss->beacon_interval;
		cfg80211_put_bss(bss);
=======
#ifdef	ROAM_CHANNEL_CACHE
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 38) && !defined(WL_COMPAT_WIRELESS)
		freq = ieee80211_channel_to_frequency(channel);
#else
		band = (channel <= CH_MAX_2G_CHANNEL) ? IEEE80211_BAND_2GHZ : IEEE80211_BAND_5GHZ;
		freq = ieee80211_channel_to_frequency(channel, band);
#endif
		cur_channel = ieee80211_get_channel(wiphy, freq);
		bss->channel = cur_channel;
#endif /* ROAM_CHANNEL_CACHE */
#if defined(WL_CFG80211_P2P_DEV_IF)
		ie = (u8 *)bss->ies->data;
		ie_len = bss->ies->len;
#else
		ie = bss->information_elements;
		ie_len = bss->len_information_elements;
#endif /* WL_CFG80211_P2P_DEV_IF */
		beacon_interval = bss->beacon_interval;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
		cfg80211_put_bss(wiphy, bss);
#else
		cfg80211_put_bss(bss);
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0) */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

	tim = bcm_parse_tlvs(ie, ie_len, WLAN_EID_TIM);
	if (tim) {
		dtim_period = tim->data[1];
	} else {
		/*
		* active scan was done so we could not get dtim
		* information out of probe response.
		* so we speficially query dtim information.
		*/
		err = wldev_ioctl(ndev, WLC_GET_DTIMPRD,
			&dtim_period, sizeof(dtim_period), false);
		if (unlikely(err)) {
			WL_ERR(("WLC_GET_DTIMPRD error (%d)\n", err));
			goto update_bss_info_out;
		}
	}

<<<<<<< HEAD
	wl_update_prof(wl, ndev, NULL, &beacon_interval, WL_PROF_BEACONINT);
	wl_update_prof(wl, ndev, NULL, &dtim_period, WL_PROF_DTIMPERIOD);

update_bss_info_out:
	mutex_unlock(&wl->usr_sync);
=======
	wl_update_prof(cfg, ndev, NULL, &beacon_interval, WL_PROF_BEACONINT);
	wl_update_prof(cfg, ndev, NULL, &dtim_period, WL_PROF_DTIMPERIOD);

update_bss_info_out:
	if (unlikely(err)) {
		WL_ERR(("Failed with error %d\n", err));
	}
	mutex_unlock(&cfg->usr_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}

static s32
<<<<<<< HEAD
wl_bss_roaming_done(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	struct wl_connect_info *conn_info = wl_to_conn(wl);
	s32 err = 0;
	u8 *curbssid;

	wl_get_assoc_ies(wl, ndev);
	wl_update_prof(wl, ndev, NULL, (void *)(e->addr.octet), WL_PROF_BSSID);
	curbssid = wl_read_prof(wl, ndev, WL_PROF_BSSID);
	wl_update_bss_info(wl, ndev, 1);
	wl_update_pmklist(ndev, wl->pmk_list, err);
=======
wl_bss_roaming_done(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	struct wl_connect_info *conn_info = wl_to_conn(cfg);
	s32 err = 0;
	u8 *curbssid;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) || defined(WL_COMPAT_WIRELESS)
	struct wiphy *wiphy = bcmcfg_to_wiphy(cfg);
	struct ieee80211_supported_band *band;
	struct ieee80211_channel *notify_channel = NULL;
	u32 *channel;
	u32 freq;
#endif /* LINUX_VERSION > 2.6.39 || WL_COMPAT_WIRELESS */

	wl_get_assoc_ies(cfg, ndev);
	wl_update_prof(cfg, ndev, NULL, (void *)(e->addr.octet), WL_PROF_BSSID);
	curbssid = wl_read_prof(cfg, ndev, WL_PROF_BSSID);
	wl_update_bss_info(cfg, ndev);
	wl_update_pmklist(ndev, cfg->pmk_list, err);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) || defined(WL_COMPAT_WIRELESS)
	/* channel info for cfg80211_roamed introduced in 2.6.39-rc1 */
	channel = (u32 *)wl_read_prof(cfg, ndev, WL_PROF_CHAN);
	if (*channel <= CH_MAX_2G_CHANNEL)
		band = wiphy->bands[IEEE80211_BAND_2GHZ];
	else
		band = wiphy->bands[IEEE80211_BAND_5GHZ];
	freq = ieee80211_channel_to_frequency(*channel, band->band);
	notify_channel = ieee80211_get_channel(wiphy, freq);
#endif /* LINUX_VERSION > 2.6.39  || WL_COMPAT_WIRELESS */
#ifdef WLFBT
	/* back up the given FBT key for the further supplicant request,
	 * currently not checking the FBT is enabled for current BSS in DHD,
	 * because the supplicant decides to take it or not.
	 */
	if (data) {
		memcpy(cfg->fbt_key, data, FBT_KEYLEN);
	}
#endif /* WLFBT */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	printk("wl_bss_roaming_done succeeded to " MACDBG "\n",
		MAC2STRDBG((u8*)(&e->addr)));

	cfg80211_roamed(ndev,
<<<<<<< HEAD

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)
		NULL,	/* struct cfg80211_bss *bss */
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)
		NULL,
=======
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 39)) || defined(WL_COMPAT_WIRELESS)
		notify_channel,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#endif
		curbssid,
		conn_info->req_ie, conn_info->req_ie_len,
		conn_info->resp_ie, conn_info->resp_ie_len, GFP_KERNEL);
	WL_DBG(("Report roaming result\n"));

<<<<<<< HEAD
	wl_set_drv_status(wl, CONNECTED, ndev);
=======
	wl_set_drv_status(cfg, CONNECTED, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

static s32
<<<<<<< HEAD
wl_bss_connect_done(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data, bool completed)
{
	struct wl_connect_info *conn_info = wl_to_conn(wl);
	struct wl_security *sec = wl_read_prof(wl, ndev, WL_PROF_SEC);
	s32 err = 0;
	u8 *curbssid = wl_read_prof(wl, ndev, WL_PROF_BSSID);
=======
wl_bss_connect_done(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data, bool completed)
{
	struct wl_connect_info *conn_info = wl_to_conn(cfg);
	struct wl_security *sec = wl_read_prof(cfg, ndev, WL_PROF_SEC);
#if (defined(ROAM_ENABLE) && defined(ROAM_AP_ENV_DETECTION)) || \
	defined(CUSTOM_SET_CPUCORE)
	dhd_pub_t *dhd = (dhd_pub_t *)(cfg->pub);
#endif /* (ROAM_ENABLE && ROAM_AP_ENV_DETECTION) || CUSTOM_SET_CPUCORE */
	s32 err = 0;
	u8 *curbssid = wl_read_prof(cfg, ndev, WL_PROF_BSSID);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (!sec) {
		WL_ERR(("sec is NULL\n"));
		return -ENODEV;
	}
	WL_DBG((" enter\n"));
#ifdef ESCAN_RESULT_PATCH
<<<<<<< HEAD
	if (wl_get_drv_status(wl, CONNECTED, ndev)) {
=======
	if (wl_get_drv_status(cfg, CONNECTED, ndev)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (memcmp(curbssid, connect_req_bssid, ETHER_ADDR_LEN) == 0) {
			WL_DBG((" Connected event of connected device e=%d s=%d, ignore it\n",
				ntoh32(e->event_type), ntoh32(e->status)));
			return err;
		}
	}
	if (memcmp(curbssid, broad_bssid, ETHER_ADDR_LEN) == 0 &&
		memcmp(broad_bssid, connect_req_bssid, ETHER_ADDR_LEN) != 0) {
		WL_DBG(("copy bssid\n"));
		memcpy(curbssid, connect_req_bssid, ETHER_ADDR_LEN);
	}

<<<<<<< HEAD
#else
	if (wl->scan_request) {
		wl_notify_escan_complete(wl, ndev, true, true);
	}
#endif /* ESCAN_RESULT_PATCH */
	if (wl_get_drv_status(wl, CONNECTING, ndev)) {
		wl_clr_drv_status(wl, CONNECTING, ndev);
		if (completed) {
			wl_get_assoc_ies(wl, ndev);
			wl_update_prof(wl, ndev, NULL, (void *)(e->addr.octet), WL_PROF_BSSID);
			curbssid = wl_read_prof(wl, ndev, WL_PROF_BSSID);
			wl_update_bss_info(wl, ndev, 0);
			wl_update_pmklist(ndev, wl->pmk_list, err);
			wl_set_drv_status(wl, CONNECTED, ndev);
=======
#if defined(BCM4334_CHIP)
	if (cfg->scan_request) {
		wl_notify_escan_complete(cfg, ndev, true, true);
	}
#endif
#else
	if (cfg->scan_request) {
		wl_notify_escan_complete(cfg, ndev, true, true);
	}
#endif /* ESCAN_RESULT_PATCH */
	if (wl_get_drv_status(cfg, CONNECTING, ndev)) {
		wl_cfg80211_scan_abort(cfg);
		wl_clr_drv_status(cfg, CONNECTING, ndev);
		if (completed) {
			wl_get_assoc_ies(cfg, ndev);
			wl_update_prof(cfg, ndev, NULL, (void *)(e->addr.octet), WL_PROF_BSSID);
			curbssid = wl_read_prof(cfg, ndev, WL_PROF_BSSID);
			wl_update_bss_info(cfg, ndev);
			wl_update_pmklist(ndev, cfg->pmk_list, err);
			wl_set_drv_status(cfg, CONNECTED, ndev);
#if defined(ROAM_ENABLE) && defined(ROAM_AP_ENV_DETECTION)
			if (dhd->roam_env_detection)
				wldev_iovar_setint(ndev, "roam_env_detection",
					AP_ENV_INDETERMINATE);
#endif /* ROAM_AP_ENV_DETECTION */
			if (ndev != bcmcfg_to_prmry_ndev(cfg)) {
				/* reinitialize completion to clear previous count */
				INIT_COMPLETION(cfg->iface_disable);
			}
#ifdef CUSTOM_SET_CPUCORE
			if (wl_get_chan_isvht80(ndev, dhd)) {
				if (ndev == bcmcfg_to_prmry_ndev(cfg))
					dhd->chan_isvht80 |= DHD_FLAG_STA_MODE; /* STA mode */
				else if (ndev == wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_CONNECTION))
					dhd->chan_isvht80 |= DHD_FLAG_P2P_MODE; /* p2p mode */
				dhd_set_cpucore(dhd, TRUE);
			}
#endif /* CUSTOM_SET_CPUCORE */

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
		cfg80211_connect_result(ndev,
			curbssid,
			conn_info->req_ie,
			conn_info->req_ie_len,
			conn_info->resp_ie,
			conn_info->resp_ie_len,
			completed ? WLAN_STATUS_SUCCESS :
			(sec->auth_assoc_res_status) ?
			sec->auth_assoc_res_status :
			WLAN_STATUS_UNSPECIFIED_FAILURE,
			GFP_KERNEL);
		if (completed)
			WL_INFO(("Report connect result - connection succeeded\n"));
		else
			WL_ERR(("Report connect result - connection failed\n"));
	}
<<<<<<< HEAD
=======
#ifdef CONFIG_TCPACK_FASTTX
	if (wl_get_chan_isvht80(ndev, dhd))
		wldev_iovar_setint(ndev, "tcpack_fast_tx", 0);
	else
		wldev_iovar_setint(ndev, "tcpack_fast_tx", 1);
#endif /* CONFIG_TCPACK_FASTTX */

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}

static s32
<<<<<<< HEAD
wl_notify_mic_status(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	u16 flags = ntoh16(e->flags);
	enum nl80211_key_type key_type;

	mutex_lock(&wl->usr_sync);
=======
wl_notify_mic_status(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data)
{
	struct net_device *ndev = NULL;
	u16 flags = ntoh16(e->flags);
	enum nl80211_key_type key_type;

	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	mutex_lock(&cfg->usr_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (flags & WLC_EVENT_MSG_GROUP)
		key_type = NL80211_KEYTYPE_GROUP;
	else
		key_type = NL80211_KEYTYPE_PAIRWISE;

	cfg80211_michael_mic_failure(ndev, (u8 *)&e->addr, key_type, -1,
		NULL, GFP_KERNEL);
<<<<<<< HEAD
	mutex_unlock(&wl->usr_sync);
=======
	mutex_unlock(&cfg->usr_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return 0;
}

#ifdef PNO_SUPPORT
static s32
<<<<<<< HEAD
wl_notify_pfn_status(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	WL_ERR((">>> PNO Event\n"));

#ifndef WL_SCHED_SCAN
	mutex_lock(&wl->usr_sync);
	/* TODO: Use cfg80211_sched_scan_results(wiphy); */
	cfg80211_disconnected(ndev, 0, NULL, 0, GFP_KERNEL);
	mutex_unlock(&wl->usr_sync);
=======
wl_notify_pfn_status(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data)
{
	struct net_device *ndev = NULL;

	WL_ERR((">>> PNO Event\n"));

	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

#ifndef WL_SCHED_SCAN
#ifndef CUSTOMER_HW4
	mutex_lock(&cfg->usr_sync);
	/* TODO: Use cfg80211_sched_scan_results(wiphy); */
	cfg80211_disconnected(ndev, 0, NULL, 0, GFP_KERNEL);
	mutex_unlock(&cfg->usr_sync);
#endif /* !CUSTOMER_HW4 */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#else
	/* If cfg80211 scheduled scan is supported, report the pno results via sched
	 * scan results
	 */
<<<<<<< HEAD
	wl_notify_sched_scan_results(wl, ndev, e, data);
=======
	wl_notify_sched_scan_results(cfg, ndev, e, data);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#endif /* WL_SCHED_SCAN */
	return 0;
}
#endif /* PNO_SUPPORT */

static s32
<<<<<<< HEAD
wl_notify_scan_status(struct wl_priv *wl, struct net_device *ndev,
=======
wl_notify_scan_status(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	const wl_event_msg_t *e, void *data)
{
	struct channel_info channel_inform;
	struct wl_scan_results *bss_list;
<<<<<<< HEAD
=======
	struct net_device *ndev = NULL;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	u32 len = WL_SCAN_BUF_MAX;
	s32 err = 0;
	unsigned long flags;

	WL_DBG(("Enter \n"));
<<<<<<< HEAD
	if (!wl_get_drv_status(wl, SCANNING, ndev)) {
		WL_ERR(("scan is not ready \n"));
		return err;
	}
	if (wl->iscan_on && wl->iscan_kickstart)
		return wl_wakeup_iscan(wl_to_iscan(wl));

	mutex_lock(&wl->usr_sync);
	wl_clr_drv_status(wl, SCANNING, ndev);
=======
	if (!wl_get_drv_status(cfg, SCANNING, ndev)) {
		WL_ERR(("scan is not ready \n"));
		return err;
	}
	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	mutex_lock(&cfg->usr_sync);
	wl_clr_drv_status(cfg, SCANNING, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	err = wldev_ioctl(ndev, WLC_GET_CHANNEL, &channel_inform,
		sizeof(channel_inform), false);
	if (unlikely(err)) {
		WL_ERR(("scan busy (%d)\n", err));
		goto scan_done_out;
	}
	channel_inform.scan_channel = dtoh32(channel_inform.scan_channel);
	if (unlikely(channel_inform.scan_channel)) {

		WL_DBG(("channel_inform.scan_channel (%d)\n",
			channel_inform.scan_channel));
	}
<<<<<<< HEAD
	wl->bss_list = wl->scan_results;
	bss_list = wl->bss_list;
	memset(bss_list, 0, len);
	bss_list->buflen = htod32(len);
	err = wldev_ioctl(ndev, WLC_SCAN_RESULTS, bss_list, len, false);
	if (unlikely(err) && unlikely(!wl->scan_suppressed)) {
=======
	cfg->bss_list = cfg->scan_results;
	bss_list = cfg->bss_list;
	memset(bss_list, 0, len);
	bss_list->buflen = htod32(len);
	err = wldev_ioctl(ndev, WLC_SCAN_RESULTS, bss_list, len, false);
	if (unlikely(err) && unlikely(!cfg->scan_suppressed)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("%s Scan_results error (%d)\n", ndev->name, err));
		err = -EINVAL;
		goto scan_done_out;
	}
	bss_list->buflen = dtoh32(bss_list->buflen);
	bss_list->version = dtoh32(bss_list->version);
	bss_list->count = dtoh32(bss_list->count);

<<<<<<< HEAD
	err = wl_inform_bss(wl);

scan_done_out:
	del_timer_sync(&wl->scan_timeout);
	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
	if (wl->scan_request) {
		cfg80211_scan_done(wl->scan_request, false);
		wl->scan_request = NULL;
	}
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);
	WL_DBG(("cfg80211_scan_done\n"));
	mutex_unlock(&wl->usr_sync);
	return err;
}
=======
	err = wl_inform_bss(cfg);

scan_done_out:
	del_timer_sync(&cfg->scan_timeout);
	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
	if (cfg->scan_request) {
		cfg80211_scan_done(cfg->scan_request, false);
		cfg->scan_request = NULL;
	}
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);
	WL_DBG(("cfg80211_scan_done\n"));
	mutex_unlock(&cfg->usr_sync);
	return err;
}

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static s32
wl_frame_get_mgmt(u16 fc, const struct ether_addr *da,
	const struct ether_addr *sa, const struct ether_addr *bssid,
	u8 **pheader, u32 *body_len, u8 *pbody)
{
	struct dot11_management_header *hdr;
	u32 totlen = 0;
	s32 err = 0;
	u8 *offset;
	u32 prebody_len = *body_len;
	switch (fc) {
		case FC_ASSOC_REQ:
			/* capability , listen interval */
			totlen = DOT11_ASSOC_REQ_FIXED_LEN;
			*body_len += DOT11_ASSOC_REQ_FIXED_LEN;
			break;

		case FC_REASSOC_REQ:
			/* capability, listen inteval, ap address */
			totlen = DOT11_REASSOC_REQ_FIXED_LEN;
			*body_len += DOT11_REASSOC_REQ_FIXED_LEN;
			break;
	}
	totlen += DOT11_MGMT_HDR_LEN + prebody_len;
	*pheader = kzalloc(totlen, GFP_KERNEL);
	if (*pheader == NULL) {
		WL_ERR(("memory alloc failed \n"));
		return -ENOMEM;
	}
	hdr = (struct dot11_management_header *) (*pheader);
	hdr->fc = htol16(fc);
	hdr->durid = 0;
	hdr->seq = 0;
	offset = (u8*)(hdr + 1) + (totlen - DOT11_MGMT_HDR_LEN - prebody_len);
	bcopy((const char*)da, (u8*)&hdr->da, ETHER_ADDR_LEN);
	bcopy((const char*)sa, (u8*)&hdr->sa, ETHER_ADDR_LEN);
	bcopy((const char*)bssid, (u8*)&hdr->bssid, ETHER_ADDR_LEN);
	if ((pbody != NULL) && prebody_len)
		bcopy((const char*)pbody, offset, prebody_len);
	*body_len = totlen;
	return err;
}

<<<<<<< HEAD

void
wl_stop_wait_next_action_frame(struct wl_priv *wl, struct net_device *ndev)
{
	if (wl_get_drv_status_all(wl, SENDING_ACT_FRM) &&
		(wl_get_p2p_status(wl, ACTION_TX_COMPLETED) ||
		wl_get_p2p_status(wl, ACTION_TX_NOACK))) {
=======
#ifdef WL_CFG80211_GON_COLLISION
static void
wl_gon_req_collision(struct bcm_cfg80211 *cfg, wl_action_frame_t *tx_act_frm,
	wifi_p2p_pub_act_frame_t *rx_act_frm, struct net_device *ndev,
	struct ether_addr sa, struct ether_addr da)
{
	if (cfg->afx_hdl->pending_tx_act_frm == NULL)
		return;

	if (tx_act_frm &&
		wl_cfgp2p_is_pub_action(tx_act_frm->data, tx_act_frm->len)) {
		wifi_p2p_pub_act_frame_t *pact_frm;

		pact_frm = (wifi_p2p_pub_act_frame_t *)tx_act_frm->data;

		if (!(pact_frm->subtype == P2P_PAF_GON_REQ &&
			rx_act_frm->subtype == P2P_PAF_GON_REQ)) {
			return;
		}
	}

	WL_ERR((" GO NEGO Request COLLISION !!! \n"));

	/* if sa(peer) addr is less than da(my) addr,
	 * my device will process peer's gon request and block to send my gon req.
	 *
	 * if not (sa addr > da addr),
	 * my device will process gon request and drop gon req of peer.
	 */
	if (memcmp(sa.octet, da.octet, ETHER_ADDR_LEN) < 0) {
		/* block to send tx gon request */
		cfg->block_gon_req_tx_count = BLOCK_GON_REQ_MAX_NUM;
		WL_ERR((" block to send gon req tx !!!\n"));

		/* if we are finding a common channel for sending af,
		 * do not scan more to block to send current gon req
		 */
		if (wl_get_drv_status_all(cfg, FINDING_COMMON_CHANNEL)) {
			wl_clr_drv_status(cfg, FINDING_COMMON_CHANNEL, ndev);
			complete(&cfg->act_frm_scan);
		}
	} else {
		/* drop gon request of peer to process gon request by my device. */
		WL_ERR((" drop to receive gon req rx !!! \n"));
		cfg->block_gon_req_rx_count = BLOCK_GON_REQ_MAX_NUM;
	}

	return;
}
#endif /* WL_CFG80211_GON_COLLISION */

void
wl_stop_wait_next_action_frame(struct bcm_cfg80211 *cfg, struct net_device *ndev)
{
	if (wl_get_drv_status_all(cfg, FINDING_COMMON_CHANNEL)) {
		if (timer_pending(&cfg->p2p->listen_timer)) {
			del_timer_sync(&cfg->p2p->listen_timer);
		}
		if (cfg->afx_hdl != NULL) {
			if (cfg->afx_hdl->dev != NULL) {
				wl_clr_drv_status(cfg, SCANNING, cfg->afx_hdl->dev);
				wl_clr_drv_status(cfg, FINDING_COMMON_CHANNEL, cfg->afx_hdl->dev);
			}
			cfg->afx_hdl->peer_chan = WL_INVALID;
		}
		complete(&cfg->act_frm_scan);
		WL_DBG(("*** Wake UP ** Working afx searching is cleared\n"));
	} else if (wl_get_drv_status_all(cfg, SENDING_ACT_FRM)) {
		if (!(wl_get_p2p_status(cfg, ACTION_TX_COMPLETED) ||
			wl_get_p2p_status(cfg, ACTION_TX_NOACK)))
			wl_set_p2p_status(cfg, ACTION_TX_COMPLETED);

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_DBG(("*** Wake UP ** abort actframe iovar\n"));
		/* if channel is not zero, "actfame" uses off channel scan.
		 * So abort scan for off channel completion.
		 */
<<<<<<< HEAD
		if (wl->af_sent_channel)
			/* wl_cfg80211_scan_abort(wl, ndev); */
			wl_notify_escan_complete(wl,
				(ndev == wl->p2p_net) ? wl_to_prmry_ndev(wl) : ndev, true, true);
	}
#ifdef WL_CFG80211_SYNC_GON
	else if (wl_get_drv_status_all(wl, WAITING_NEXT_ACT_FRM_LISTEN)) {
		WL_DBG(("*** Wake UP ** abort listen for next af frame\n"));
		/* So abort scan to cancel listen */
		wl_notify_escan_complete(wl,
			(ndev == wl->p2p_net) ? wl_to_prmry_ndev(wl) : ndev, true, true);
=======
		if (cfg->af_sent_channel)
			wl_cfg80211_scan_abort(cfg);
	}
#ifdef WL_CFG80211_SYNC_GON
	else if (wl_get_drv_status_all(cfg, WAITING_NEXT_ACT_FRM_LISTEN)) {
		WL_DBG(("*** Wake UP ** abort listen for next af frame\n"));
		/* So abort scan to cancel listen */
		wl_cfg80211_scan_abort(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
#endif /* WL_CFG80211_SYNC_GON */
}

<<<<<<< HEAD

static s32
wl_notify_rx_mgmt_frame(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	struct ieee80211_supported_band *band;
	struct wiphy *wiphy = wl_to_wiphy(wl);
=======
#if defined(CUSTOMER_HW4) && defined(WES_SUPPORT)
static int wes_mode = 0;
int wl_cfg80211_set_wes_mode(int mode)
{
	wes_mode = mode;
	return 0;
}

int wl_cfg80211_get_wes_mode(void)
{
	return wes_mode;
}

bool wl_cfg80211_is_wes(void *frame, u32 frame_len)
{
	unsigned char *data;

	if (frame == NULL) {
		WL_ERR(("Invalid frame \n"));
		return false;
	}

	if (frame_len < 4) {
		WL_ERR(("Invalid frame length [%d] \n", frame_len));
		return false;
	}

	data = frame;

	if (memcmp(data, "\x7f\x00\x00\xf0", 4) == 0) {
		WL_DBG(("Receive WES VS Action Frame \n"));
		return true;
	}

	return false;
}
#endif /* WES_SUPPORT */

int wl_cfg80211_get_ioctl_version(void)
{
	return ioctl_version;
}

static s32
wl_notify_rx_mgmt_frame(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data)
{
	struct ieee80211_supported_band *band;
	struct wiphy *wiphy = bcmcfg_to_wiphy(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct ether_addr da;
	struct ether_addr bssid;
	bool isfree = false;
	s32 err = 0;
	s32 freq;
<<<<<<< HEAD
	struct net_device *dev = NULL;
=======
	struct net_device *ndev = NULL;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	wifi_p2p_pub_act_frame_t *act_frm = NULL;
	wifi_p2p_action_frame_t *p2p_act_frm = NULL;
	wifi_p2psd_gas_pub_act_frame_t *sd_act_frm = NULL;
	wl_event_rx_frame_data_t *rxframe =
		(wl_event_rx_frame_data_t*)data;
	u32 event = ntoh32(e->event_type);
	u8 *mgmt_frame;
	u8 bsscfgidx = e->bsscfgidx;
	u32 mgmt_frame_len = ntoh32(e->datalen) - sizeof(wl_event_rx_frame_data_t);
	u16 channel = ((ntoh16(rxframe->channel) & WL_CHANSPEC_CHAN_MASK));

	memset(&bssid, 0, ETHER_ADDR_LEN);

<<<<<<< HEAD
	if (wl->p2p_net == ndev) {
		dev = wl_to_prmry_ndev(wl);
	} else {
		dev = ndev;
	}
=======
	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (channel <= CH_MAX_2G_CHANNEL)
		band = wiphy->bands[IEEE80211_BAND_2GHZ];
	else
		band = wiphy->bands[IEEE80211_BAND_5GHZ];
	if (!band) {
		WL_ERR(("No valid band"));
		return -EINVAL;
	}
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 38) && !defined(WL_COMPAT_WIRELESS)
	freq = ieee80211_channel_to_frequency(channel);
	(void)band->band;
#else
	freq = ieee80211_channel_to_frequency(channel, band->band);
#endif
	if (event == WLC_E_ACTION_FRAME_RX) {
<<<<<<< HEAD
		wldev_iovar_getbuf_bsscfg(dev, "cur_etheraddr",
			NULL, 0, wl->ioctl_buf, WLC_IOCTL_SMLEN, bsscfgidx, &wl->ioctl_buf_sync);

		err = wldev_ioctl(dev, WLC_GET_BSSID, &bssid, ETHER_ADDR_LEN, false);
		if (err < 0)
			 WL_ERR(("WLC_GET_BSSID error %d\n", err));
		memcpy(da.octet, wl->ioctl_buf, ETHER_ADDR_LEN);
=======
		wldev_iovar_getbuf_bsscfg(ndev, "cur_etheraddr",
			NULL, 0, cfg->ioctl_buf, WLC_IOCTL_SMLEN, bsscfgidx, &cfg->ioctl_buf_sync);

		err = wldev_ioctl(ndev, WLC_GET_BSSID, &bssid, ETHER_ADDR_LEN, false);
		if (err < 0)
			 WL_ERR(("WLC_GET_BSSID error %d\n", err));
		memcpy(da.octet, cfg->ioctl_buf, ETHER_ADDR_LEN);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		err = wl_frame_get_mgmt(FC_ACTION, &da, &e->addr, &bssid,
			&mgmt_frame, &mgmt_frame_len,
			(u8 *)((wl_event_rx_frame_data_t *)rxframe + 1));
		if (err < 0) {
<<<<<<< HEAD
			WL_ERR(("%s: Error in receiving action frame len %d channel %d freq %d\n",
				__func__, mgmt_frame_len, channel, freq));
=======
			WL_ERR(("Error in receiving action frame len %d channel %d freq %d\n",
				mgmt_frame_len, channel, freq));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			goto exit;
		}
		isfree = true;
		if (wl_cfgp2p_is_pub_action(&mgmt_frame[DOT11_MGMT_HDR_LEN],
			mgmt_frame_len - DOT11_MGMT_HDR_LEN)) {
			act_frm = (wifi_p2p_pub_act_frame_t *)
					(&mgmt_frame[DOT11_MGMT_HDR_LEN]);
		} else if (wl_cfgp2p_is_p2p_action(&mgmt_frame[DOT11_MGMT_HDR_LEN],
			mgmt_frame_len - DOT11_MGMT_HDR_LEN)) {
			p2p_act_frm = (wifi_p2p_action_frame_t *)
					(&mgmt_frame[DOT11_MGMT_HDR_LEN]);
			(void) p2p_act_frm;
		} else if (wl_cfgp2p_is_gas_action(&mgmt_frame[DOT11_MGMT_HDR_LEN],
			mgmt_frame_len - DOT11_MGMT_HDR_LEN)) {

			sd_act_frm = (wifi_p2psd_gas_pub_act_frame_t *)
					(&mgmt_frame[DOT11_MGMT_HDR_LEN]);
<<<<<<< HEAD
			if (sd_act_frm && wl_get_drv_status_all(wl, WAITING_NEXT_ACT_FRM)) {
				if (wl->next_af_subtype == sd_act_frm->action) {
					WL_DBG(("We got a right next frame of SD!(%d)\n",
						sd_act_frm->action));
					wl_clr_drv_status(wl, WAITING_NEXT_ACT_FRM,
						(ndev == wl->p2p_net) ?
						wl_to_prmry_ndev(wl) : ndev);

					/* Stop waiting for next AF. */
					wl_stop_wait_next_action_frame(wl, ndev);
=======
			if (sd_act_frm && wl_get_drv_status_all(cfg, WAITING_NEXT_ACT_FRM)) {
				if (cfg->next_af_subtype == sd_act_frm->action) {
					WL_DBG(("We got a right next frame of SD!(%d)\n",
						sd_act_frm->action));
					wl_clr_drv_status(cfg, WAITING_NEXT_ACT_FRM, ndev);

					/* Stop waiting for next AF. */
					wl_stop_wait_next_action_frame(cfg, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				}
			}
			(void) sd_act_frm;
		} else {
			/*
<<<<<<< HEAD
			*  if we got normal action frame and ndev is p2p0,
			*  we have to change ndev from p2p0 to wlan0
			 */
			if (wl->p2p_net == ndev)
				ndev = wl_to_prmry_ndev(wl);
		}

		if (act_frm) {

			if (wl_get_drv_status_all(wl, WAITING_NEXT_ACT_FRM)) {
				if (wl->next_af_subtype == act_frm->subtype) {
					WL_DBG(("We got a right next frame!(%d)\n",
						act_frm->subtype));
					wl_clr_drv_status(wl, WAITING_NEXT_ACT_FRM,
						(ndev == wl->p2p_net) ?
						wl_to_prmry_ndev(wl) : ndev);

					/* Stop waiting for next AF. */
					wl_stop_wait_next_action_frame(wl, ndev);
=======
			 *  if we got normal action frame and ndev is p2p0,
			 *  we have to change ndev from p2p0 to wlan0
			 */
#if defined(CUSTOMER_HW4) && defined(WES_SUPPORT)
			if (wl_cfg80211_is_wes(&mgmt_frame[DOT11_MGMT_HDR_LEN],
			mgmt_frame_len - DOT11_MGMT_HDR_LEN) && wes_mode == 0) {
			/* Ignore WES VS Action frame */
			goto exit;
			}
#endif /* WES_SUPPORT */

			/* use primary device instead of p2p's */
			if (discover_cfgdev(cfgdev, cfg))
				cfgdev = bcmcfg_to_prmry_cfgdev(cfgdev, cfg);

			if (cfg->next_af_subtype != P2P_PAF_SUBTYPE_INVALID) {
				u8 action = 0;
				if (wl_get_public_action(&mgmt_frame[DOT11_MGMT_HDR_LEN],
					mgmt_frame_len - DOT11_MGMT_HDR_LEN, &action) != BCME_OK) {
					WL_DBG(("Recived action is not public action frame\n"));
				} else if (cfg->next_af_subtype == action) {
					WL_DBG(("Recived action is the waiting action(%d)\n",
						action));
					wl_clr_drv_status(cfg, WAITING_NEXT_ACT_FRM, ndev);

					/* Stop waiting for next AF. */
					wl_stop_wait_next_action_frame(cfg, ndev);
				}
			}
		}

		if (act_frm) {
#ifdef WL_CFG80211_GON_COLLISION
			if (act_frm->subtype == P2P_PAF_GON_REQ) {
				wl_gon_req_collision(cfg,
					&cfg->afx_hdl->pending_tx_act_frm->action_frame,
					act_frm, ndev, e->addr, da);

				if (cfg->block_gon_req_rx_count) {
					WL_ERR(("drop frame GON Req Rx : count (%d)\n",
						cfg->block_gon_req_rx_count));
					cfg->block_gon_req_rx_count--;
					goto exit;
				}
			} else if (act_frm->subtype == P2P_PAF_GON_CONF) {
				/* if go formation done, clear it */
				cfg->block_gon_req_tx_count = 0;
				cfg->block_gon_req_rx_count = 0;
			}
#endif /* WL_CFG80211_GON_COLLISION */

			if (wl_get_drv_status_all(cfg, WAITING_NEXT_ACT_FRM)) {
				if (cfg->next_af_subtype == act_frm->subtype) {
					WL_DBG(("We got a right next frame!(%d)\n",
						act_frm->subtype));
					wl_clr_drv_status(cfg, WAITING_NEXT_ACT_FRM, ndev);

					if (cfg->next_af_subtype == P2P_PAF_GON_CONF) {
						OSL_SLEEP(20);
					}

					/* Stop waiting for next AF. */
					wl_stop_wait_next_action_frame(cfg, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				}
			}
		}

		wl_cfgp2p_print_actframe(false, &mgmt_frame[DOT11_MGMT_HDR_LEN],
<<<<<<< HEAD
			mgmt_frame_len - DOT11_MGMT_HDR_LEN);
=======
			mgmt_frame_len - DOT11_MGMT_HDR_LEN, channel);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/*
		 * After complete GO Negotiation, roll back to mpc mode
		 */
		if (act_frm && ((act_frm->subtype == P2P_PAF_GON_CONF) ||
			(act_frm->subtype == P2P_PAF_PROVDIS_RSP))) {
<<<<<<< HEAD
			wldev_iovar_setint(dev, "mpc", 1);
		}
		if (act_frm && (act_frm->subtype == P2P_PAF_GON_CONF)) {
			WL_DBG(("P2P: GO_NEG_PHASE status cleared \n"));
			wl_clr_p2p_status(wl, GO_NEG_PHASE);
=======
			wldev_iovar_setint(ndev, "mpc", 1);
		}
		if (act_frm && (act_frm->subtype == P2P_PAF_GON_CONF)) {
			WL_DBG(("P2P: GO_NEG_PHASE status cleared \n"));
			wl_clr_p2p_status(cfg, GO_NEG_PHASE);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	} else {
		mgmt_frame = (u8 *)((wl_event_rx_frame_data_t *)rxframe + 1);

		/* wpa supplicant use probe request event for restarting another GON Req.
		 * but it makes GON Req repetition.
		 * so if src addr of prb req is same as my target device,
		 * do not send probe request event during sending action frame.
		 */
		if (event == WLC_E_P2P_PROBREQ_MSG) {
			WL_DBG((" Event %s\n", (event == WLC_E_P2P_PROBREQ_MSG) ?
				"WLC_E_P2P_PROBREQ_MSG":"WLC_E_PROBREQ_MSG"));

<<<<<<< HEAD
=======
#ifdef WL_CFG80211_USE_PRB_REQ_FOR_AF_TX
			if (WL_DRV_STATUS_SENDING_AF_FRM_EXT(cfg) &&
				!memcmp(cfg->afx_hdl->tx_dst_addr.octet, e->addr.octet,
				ETHER_ADDR_LEN)) {
				if (cfg->afx_hdl->pending_tx_act_frm &&
					wl_get_drv_status_all(cfg, FINDING_COMMON_CHANNEL)) {
					s32 channel = CHSPEC_CHANNEL(hton16(rxframe->channel));
					WL_DBG(("PROBE REQUEST : Peer found, channel : %d\n",
						channel));
					cfg->afx_hdl->peer_chan = channel;
					complete(&cfg->act_frm_scan);
				}
			}
#endif /* WL_CFG80211_USE_PRB_REQ_FOR_AF_TX */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

			/* Filter any P2P probe reqs arriving during the
			 * GO-NEG Phase
			 */
<<<<<<< HEAD
			if (wl->p2p &&
				wl_get_p2p_status(wl, GO_NEG_PHASE)) {
=======
			if (cfg->p2p &&
				wl_get_p2p_status(cfg, GO_NEG_PHASE)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				WL_DBG(("Filtering P2P probe_req while "
					"being in GO-Neg state\n"));
				return 0;
			}
		}
	}

<<<<<<< HEAD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
	cfg80211_rx_mgmt(ndev, freq, 0, mgmt_frame, mgmt_frame_len, GFP_ATOMIC);
#else
	cfg80211_rx_mgmt(ndev, freq, mgmt_frame, mgmt_frame_len, GFP_ATOMIC);
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0) */

	WL_DBG(("%s: mgmt_frame_len (%d) , e->datalen (%d), channel (%d), freq (%d)\n", __func__,
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)) || defined(WL_COMPAT_WIRELESS)
	cfg80211_rx_mgmt(cfgdev, freq, 0, mgmt_frame, mgmt_frame_len, GFP_ATOMIC);
#else
	cfg80211_rx_mgmt(cfgdev, freq, mgmt_frame, mgmt_frame_len, GFP_ATOMIC);
#endif /* LINUX_VERSION >= VERSION(3, 4, 0) || WL_COMPAT_WIRELESS */

	WL_DBG(("mgmt_frame_len (%d) , e->datalen (%d), channel (%d), freq (%d)\n",
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		mgmt_frame_len, ntoh32(e->datalen), channel, freq));
exit:
	if (isfree)
		kfree(mgmt_frame);
	return 0;
}

#ifdef WL_SCHED_SCAN
/* If target scan is not reliable, set the below define to "1" to do a
 * full escan
 */
#define FULL_ESCAN_ON_PFN_NET_FOUND		0
static s32
<<<<<<< HEAD
wl_notify_sched_scan_results(struct wl_priv *wl, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	wl_pfn_net_info_t *netinfo, *pnetinfo;
	struct cfg80211_scan_request request;
	struct wiphy *wiphy	= wl_to_wiphy(wl);
	int err = 0;
=======
wl_notify_sched_scan_results(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const wl_event_msg_t *e, void *data)
{
	wl_pfn_net_info_t *netinfo, *pnetinfo;
	struct wiphy *wiphy	= bcmcfg_to_wiphy(cfg);
	int err = 0;
	struct cfg80211_scan_request *request = NULL;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct cfg80211_ssid ssid[MAX_PFN_LIST_COUNT];
	struct ieee80211_channel *channel = NULL;
	int channel_req = 0;
	int band = 0;
	struct wl_pfn_scanresults *pfn_result = (struct wl_pfn_scanresults *)data;
<<<<<<< HEAD
=======
	int n_pfn_results = pfn_result->count;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	WL_DBG(("Enter\n"));

	if (e->event_type == WLC_E_PFN_NET_LOST) {
		WL_PNO(("PFN NET LOST event. Do Nothing \n"));
		return 0;
	}
<<<<<<< HEAD
	WL_PNO((">>> PFN NET FOUND event. count:%d \n", pfn_result->count));
	if (pfn_result->count > 0) {
		int i;

		memset(&request, 0x00, sizeof(struct cfg80211_scan_request));
		memset(&ssid, 0x00, sizeof(ssid));
		request.wiphy = wiphy;

		pnetinfo = (wl_pfn_net_info_t *)(data + sizeof(wl_pfn_scanresults_t)
				- sizeof(wl_pfn_net_info_t));
		channel = (struct ieee80211_channel *)kzalloc(
			(sizeof(struct ieee80211_channel) * MAX_PFN_LIST_COUNT),
			GFP_KERNEL);
		if (!channel) {
=======
	WL_PNO((">>> PFN NET FOUND event. count:%d \n", n_pfn_results));
	if (n_pfn_results > 0) {
		int i;

		if (n_pfn_results > MAX_PFN_LIST_COUNT)
			n_pfn_results = MAX_PFN_LIST_COUNT;
		pnetinfo = (wl_pfn_net_info_t *)(data + sizeof(wl_pfn_scanresults_t)
				- sizeof(wl_pfn_net_info_t));

		memset(&ssid, 0x00, sizeof(ssid));

		request = kzalloc(sizeof(*request)
			+ sizeof(*request->channels) * n_pfn_results,
			GFP_KERNEL);
		channel = (struct ieee80211_channel *)kzalloc(
			(sizeof(struct ieee80211_channel) * n_pfn_results),
			GFP_KERNEL);
		if (!request || !channel) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_ERR(("No memory"));
			err = -ENOMEM;
			goto out_err;
		}

<<<<<<< HEAD
		for (i = 0; i < pfn_result->count; i++) {
=======
		request->wiphy = wiphy;

		for (i = 0; i < n_pfn_results; i++) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			netinfo = &pnetinfo[i];
			if (!netinfo) {
				WL_ERR(("Invalid netinfo ptr. index:%d", i));
				err = -EINVAL;
				goto out_err;
			}
			WL_PNO((">>> SSID:%s Channel:%d \n",
				netinfo->pfnsubnet.SSID, netinfo->pfnsubnet.channel));
			/* PFN result doesn't have all the info which are required by the supplicant
			 * (For e.g IEs) Do a target Escan so that sched scan results are reported
			 * via wl_inform_single_bss in the required format. Escan does require the
			 * scan request in the form of cfg80211_scan_request. For timebeing, create
			 * cfg80211_scan_request one out of the received PNO event.
			 */
			memcpy(ssid[i].ssid, netinfo->pfnsubnet.SSID,
				netinfo->pfnsubnet.SSID_len);
			ssid[i].ssid_len = netinfo->pfnsubnet.SSID_len;
<<<<<<< HEAD
			request.n_ssids++;
=======
			request->n_ssids++;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

			channel_req = netinfo->pfnsubnet.channel;
			band = (channel_req <= CH_MAX_2G_CHANNEL) ? NL80211_BAND_2GHZ
				: NL80211_BAND_5GHZ;
			channel[i].center_freq = ieee80211_channel_to_frequency(channel_req, band);
			channel[i].band = band;
			channel[i].flags |= IEEE80211_CHAN_NO_HT40;
<<<<<<< HEAD
			request.channels[i] = &channel[i];
			request.n_channels++;
		}

		/* assign parsed ssid array */
		if (request.n_ssids)
			request.ssids = &ssid[0];

		if (wl_get_drv_status_all(wl, SCANNING)) {
			/* Abort any on-going scan */
			wl_notify_escan_complete(wl, ndev, true, true);
		}

		if (wl_get_p2p_status(wl, DISCOVERY_ON)) {
			WL_PNO((">>> P2P discovery was ON. Disabling it\n"));
			err = wl_cfgp2p_discover_enable_search(wl, false);
			if (unlikely(err)) {
				wl_clr_drv_status(wl, SCANNING, ndev);
				goto out_err;
			}
		}

		wl_set_drv_status(wl, SCANNING, ndev);
#if FULL_ESCAN_ON_PFN_NET_FOUND
		WL_PNO((">>> Doing Full ESCAN on PNO event\n"));
		err = wl_do_escan(wl, wiphy, ndev, NULL);
#else
		WL_PNO((">>> Doing targeted ESCAN on PNO event\n"));
		err = wl_do_escan(wl, wiphy, ndev, &request);
#endif
		if (err) {
			wl_clr_drv_status(wl, SCANNING, ndev);
			goto out_err;
		}
		wl->sched_scan_running = TRUE;
=======
			request->channels[i] = &channel[i];
			request->n_channels++;
		}

		/* assign parsed ssid array */
		if (request->n_ssids)
			request->ssids = &ssid[0];

		if (wl_get_drv_status_all(cfg, SCANNING)) {
			/* Abort any on-going scan */
			wl_notify_escan_complete(cfg, ndev, true, true);
		}

		if (wl_get_p2p_status(cfg, DISCOVERY_ON)) {
			WL_PNO((">>> P2P discovery was ON. Disabling it\n"));
			err = wl_cfgp2p_discover_enable_search(cfg, false);
			if (unlikely(err)) {
				wl_clr_drv_status(cfg, SCANNING, ndev);
				goto out_err;
			}
			p2p_scan(cfg) = false;
		}

		wl_set_drv_status(cfg, SCANNING, ndev);
#if FULL_ESCAN_ON_PFN_NET_FOUND
		WL_PNO((">>> Doing Full ESCAN on PNO event\n"));
		err = wl_do_escan(cfg, wiphy, ndev, NULL);
#else
		WL_PNO((">>> Doing targeted ESCAN on PNO event\n"));
		err = wl_do_escan(cfg, wiphy, ndev, request);
#endif
		if (err) {
			wl_clr_drv_status(cfg, SCANNING, ndev);
			goto out_err;
		}
		cfg->sched_scan_running = TRUE;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
	else {
		WL_ERR(("FALSE PNO Event. (pfn_count == 0) \n"));
	}
out_err:
<<<<<<< HEAD
=======
	if (request)
		kfree(request);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (channel)
		kfree(channel);
	return err;
}
#endif /* WL_SCHED_SCAN */

static void wl_init_conf(struct wl_conf *conf)
{
	WL_DBG(("Enter \n"));
	conf->frag_threshold = (u32)-1;
	conf->rts_threshold = (u32)-1;
	conf->retry_short = (u32)-1;
	conf->retry_long = (u32)-1;
	conf->tx_power = -1;
}

<<<<<<< HEAD
static void wl_init_prof(struct wl_priv *wl, struct net_device *ndev)
{
	unsigned long flags;
	struct wl_profile *profile = wl_get_profile_by_netdev(wl, ndev);

	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
	memset(profile, 0, sizeof(struct wl_profile));
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);
}

static void wl_init_event_handler(struct wl_priv *wl)
{
	memset(wl->evt_handler, 0, sizeof(wl->evt_handler));

	wl->evt_handler[WLC_E_SCAN_COMPLETE] = wl_notify_scan_status;
	wl->evt_handler[WLC_E_AUTH] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_ASSOC] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_LINK] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_DEAUTH_IND] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_DEAUTH] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_DISASSOC_IND] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_ASSOC_IND] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_REASSOC_IND] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_ROAM] = wl_notify_roaming_status;
	wl->evt_handler[WLC_E_MIC_ERROR] = wl_notify_mic_status;
	wl->evt_handler[WLC_E_SET_SSID] = wl_notify_connect_status;
	wl->evt_handler[WLC_E_ACTION_FRAME_RX] = wl_notify_rx_mgmt_frame;
	wl->evt_handler[WLC_E_PROBREQ_MSG] = wl_notify_rx_mgmt_frame;
	wl->evt_handler[WLC_E_P2P_PROBREQ_MSG] = wl_notify_rx_mgmt_frame;
	wl->evt_handler[WLC_E_P2P_DISC_LISTEN_COMPLETE] = wl_cfgp2p_listen_complete;
	wl->evt_handler[WLC_E_ACTION_FRAME_COMPLETE] = wl_cfgp2p_action_tx_complete;
	wl->evt_handler[WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE] = wl_cfgp2p_action_tx_complete;
#ifdef PNO_SUPPORT
	wl->evt_handler[WLC_E_PFN_NET_FOUND] = wl_notify_pfn_status;
#endif /* PNO_SUPPORT */
}

static s32 wl_init_priv_mem(struct wl_priv *wl)
{
	WL_DBG(("Enter \n"));
	wl->scan_results = (void *)kzalloc(WL_SCAN_BUF_MAX, GFP_KERNEL);
	if (unlikely(!wl->scan_results)) {
		WL_ERR(("Scan results alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->conf = (void *)kzalloc(sizeof(*wl->conf), GFP_KERNEL);
	if (unlikely(!wl->conf)) {
		WL_ERR(("wl_conf alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->scan_req_int =
	    (void *)kzalloc(sizeof(*wl->scan_req_int), GFP_KERNEL);
	if (unlikely(!wl->scan_req_int)) {
		WL_ERR(("Scan req alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->ioctl_buf = (void *)kzalloc(WLC_IOCTL_MAXLEN, GFP_KERNEL);
	if (unlikely(!wl->ioctl_buf)) {
		WL_ERR(("Ioctl buf alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->escan_ioctl_buf = (void *)kzalloc(WLC_IOCTL_MAXLEN, GFP_KERNEL);
	if (unlikely(!wl->escan_ioctl_buf)) {
		WL_ERR(("Ioctl buf alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->extra_buf = (void *)kzalloc(WL_EXTRA_BUF_MAX, GFP_KERNEL);
	if (unlikely(!wl->extra_buf)) {
		WL_ERR(("Extra buf alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->iscan = (void *)kzalloc(sizeof(*wl->iscan), GFP_KERNEL);
	if (unlikely(!wl->iscan)) {
		WL_ERR(("Iscan buf alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->pmk_list = (void *)kzalloc(sizeof(*wl->pmk_list), GFP_KERNEL);
	if (unlikely(!wl->pmk_list)) {
		WL_ERR(("pmk list alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->sta_info = (void *)kzalloc(sizeof(*wl->sta_info), GFP_KERNEL);
	if (unlikely(!wl->sta_info)) {
=======
static void wl_init_prof(struct bcm_cfg80211 *cfg, struct net_device *ndev)
{
	unsigned long flags;
	struct wl_profile *profile = wl_get_profile_by_netdev(cfg, ndev);

	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
	memset(profile, 0, sizeof(struct wl_profile));
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);
}

static void wl_init_event_handler(struct bcm_cfg80211 *cfg)
{
	memset(cfg->evt_handler, 0, sizeof(cfg->evt_handler));

	cfg->evt_handler[WLC_E_SCAN_COMPLETE] = wl_notify_scan_status;
	cfg->evt_handler[WLC_E_AUTH] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_ASSOC] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_LINK] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_DEAUTH_IND] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_DEAUTH] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_DISASSOC_IND] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_ASSOC_IND] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_REASSOC_IND] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_ROAM] = wl_notify_roaming_status;
	cfg->evt_handler[WLC_E_MIC_ERROR] = wl_notify_mic_status;
	cfg->evt_handler[WLC_E_SET_SSID] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_ACTION_FRAME_RX] = wl_notify_rx_mgmt_frame;
	cfg->evt_handler[WLC_E_PROBREQ_MSG] = wl_notify_rx_mgmt_frame;
	cfg->evt_handler[WLC_E_P2P_PROBREQ_MSG] = wl_notify_rx_mgmt_frame;
	cfg->evt_handler[WLC_E_P2P_DISC_LISTEN_COMPLETE] = wl_cfgp2p_listen_complete;
	cfg->evt_handler[WLC_E_ACTION_FRAME_COMPLETE] = wl_cfgp2p_action_tx_complete;
	cfg->evt_handler[WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE] = wl_cfgp2p_action_tx_complete;
	cfg->evt_handler[WLC_E_JOIN] = wl_notify_connect_status;
	cfg->evt_handler[WLC_E_START] = wl_notify_connect_status;
#ifdef PNO_SUPPORT
	cfg->evt_handler[WLC_E_PFN_NET_FOUND] = wl_notify_pfn_status;
#endif /* PNO_SUPPORT */
#ifdef WLTDLS
	cfg->evt_handler[WLC_E_TDLS_PEER_EVENT] = wl_tdls_event_handler;
#endif /* WLTDLS */
	cfg->evt_handler[WLC_E_BSSID] = wl_notify_roaming_status;
#ifdef WLAIBSS
	cfg->evt_handler[WLC_E_AIBSS_TXFAIL] = wl_notify_aibss_txfail;
#endif /* WLAIBSS */
}

#if defined(STATIC_WL_PRIV_STRUCT)
static void
wl_init_escan_result_buf(struct bcm_cfg80211 *cfg)
{
#if defined(DUAL_ESCAN_RESULT_BUFFER)
	cfg->escan_info.escan_buf[0] = DHD_OS_PREALLOC(cfg->pub, DHD_PREALLOC_WIPHY_ESCAN0, 0);
	bzero(cfg->escan_info.escan_buf[0], ESCAN_BUF_SIZE);
	cfg->escan_info.escan_buf[1] = DHD_OS_PREALLOC(cfg->pub, DHD_PREALLOC_WIPHY_ESCAN1, 0);
	bzero(cfg->escan_info.escan_buf[1], ESCAN_BUF_SIZE);
	cfg->escan_info.escan_type[0] = 0;
	cfg->escan_info.escan_type[1] = 0;
#else
	cfg->escan_info.escan_buf = DHD_OS_PREALLOC(cfg->pub, DHD_PREALLOC_WIPHY_ESCAN0, 0);
	bzero(cfg->escan_info.escan_buf, ESCAN_BUF_SIZE);
#endif /* DUAL_ESCAN_RESULT_BUFFER */
}

static void
wl_deinit_escan_result_buf(struct bcm_cfg80211 *cfg)
{
#if defined(DUAL_ESCAN_RESULT_BUFFER)
	cfg->escan_info.escan_buf[0] = NULL;
	cfg->escan_info.escan_buf[1] = NULL;
	cfg->escan_info.escan_type[0] = 0;
	cfg->escan_info.escan_type[1] = 0;
#else
	cfg->escan_info.escan_buf = NULL;
#endif

}
#endif /* STATIC_WL_PRIV_STRUCT */

static s32 wl_init_priv_mem(struct bcm_cfg80211 *cfg)
{
	WL_DBG(("Enter \n"));
	cfg->scan_results = (void *)kzalloc(WL_SCAN_BUF_MAX, GFP_KERNEL);
	if (unlikely(!cfg->scan_results)) {
		WL_ERR(("Scan results alloc failed\n"));
		goto init_priv_mem_out;
	}
	cfg->conf = (void *)kzalloc(sizeof(*cfg->conf), GFP_KERNEL);
	if (unlikely(!cfg->conf)) {
		WL_ERR(("wl_conf alloc failed\n"));
		goto init_priv_mem_out;
	}
	cfg->scan_req_int =
	    (void *)kzalloc(sizeof(*cfg->scan_req_int), GFP_KERNEL);
	if (unlikely(!cfg->scan_req_int)) {
		WL_ERR(("Scan req alloc failed\n"));
		goto init_priv_mem_out;
	}
	cfg->ioctl_buf = (void *)kzalloc(WLC_IOCTL_MAXLEN, GFP_KERNEL);
	if (unlikely(!cfg->ioctl_buf)) {
		WL_ERR(("Ioctl buf alloc failed\n"));
		goto init_priv_mem_out;
	}
	cfg->escan_ioctl_buf = (void *)kzalloc(WLC_IOCTL_MAXLEN, GFP_KERNEL);
	if (unlikely(!cfg->escan_ioctl_buf)) {
		WL_ERR(("Ioctl buf alloc failed\n"));
		goto init_priv_mem_out;
	}
	cfg->extra_buf = (void *)kzalloc(WL_EXTRA_BUF_MAX, GFP_KERNEL);
	if (unlikely(!cfg->extra_buf)) {
		WL_ERR(("Extra buf alloc failed\n"));
		goto init_priv_mem_out;
	}
	cfg->pmk_list = (void *)kzalloc(sizeof(*cfg->pmk_list), GFP_KERNEL);
	if (unlikely(!cfg->pmk_list)) {
		WL_ERR(("pmk list alloc failed\n"));
		goto init_priv_mem_out;
	}
	cfg->sta_info = (void *)kzalloc(sizeof(*cfg->sta_info), GFP_KERNEL);
	if (unlikely(!cfg->sta_info)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("sta info  alloc failed\n"));
		goto init_priv_mem_out;
	}

#if defined(STATIC_WL_PRIV_STRUCT)
<<<<<<< HEAD
	wl->conn_info = (void *)kzalloc(sizeof(*wl->conn_info), GFP_KERNEL);
	if (unlikely(!wl->conn_info)) {
		WL_ERR(("wl->conn_info  alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->ie = (void *)kzalloc(sizeof(*wl->ie), GFP_KERNEL);
	if (unlikely(!wl->ie)) {
		WL_ERR(("wl->ie  alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl->escan_info.escan_buf = dhd_os_prealloc(NULL, DHD_PREALLOC_WIPHY_ESCAN0, 0);
	bzero(wl->escan_info.escan_buf, ESCAN_BUF_SIZE);
#endif /* STATIC_WL_PRIV_STRUCT */
	wl->afx_hdl = (void *)kzalloc(sizeof(*wl->afx_hdl), GFP_KERNEL);
	if (unlikely(!wl->afx_hdl)) {
		WL_ERR(("afx hdl  alloc failed\n"));
		goto init_priv_mem_out;
	} else {
		init_completion(&wl->act_frm_scan);
		init_completion(&wl->wait_next_af);

		INIT_WORK(&wl->afx_hdl->work, wl_cfg80211_afx_handler);
=======
	cfg->conn_info = (void *)kzalloc(sizeof(*cfg->conn_info), GFP_KERNEL);
	if (unlikely(!cfg->conn_info)) {
		WL_ERR(("cfg->conn_info  alloc failed\n"));
		goto init_priv_mem_out;
	}
	cfg->ie = (void *)kzalloc(sizeof(*cfg->ie), GFP_KERNEL);
	if (unlikely(!cfg->ie)) {
		WL_ERR(("cfg->ie  alloc failed\n"));
		goto init_priv_mem_out;
	}
	wl_init_escan_result_buf(cfg);
#endif /* STATIC_WL_PRIV_STRUCT */
	cfg->afx_hdl = (void *)kzalloc(sizeof(*cfg->afx_hdl), GFP_KERNEL);
	if (unlikely(!cfg->afx_hdl)) {
		WL_ERR(("afx hdl  alloc failed\n"));
		goto init_priv_mem_out;
	} else {
		init_completion(&cfg->act_frm_scan);
		init_completion(&cfg->wait_next_af);

		INIT_WORK(&cfg->afx_hdl->work, wl_cfg80211_afx_handler);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
	return 0;

init_priv_mem_out:
<<<<<<< HEAD
	wl_deinit_priv_mem(wl);
=======
	wl_deinit_priv_mem(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return -ENOMEM;
}

<<<<<<< HEAD
static void wl_deinit_priv_mem(struct wl_priv *wl)
{
	kfree(wl->scan_results);
	wl->scan_results = NULL;
	kfree(wl->conf);
	wl->conf = NULL;
	kfree(wl->scan_req_int);
	wl->scan_req_int = NULL;
	kfree(wl->ioctl_buf);
	wl->ioctl_buf = NULL;
	kfree(wl->escan_ioctl_buf);
	wl->escan_ioctl_buf = NULL;
	kfree(wl->extra_buf);
	wl->extra_buf = NULL;
	kfree(wl->iscan);
	wl->iscan = NULL;
	kfree(wl->pmk_list);
	wl->pmk_list = NULL;
	kfree(wl->sta_info);
	wl->sta_info = NULL;
#if defined(STATIC_WL_PRIV_STRUCT)
	kfree(wl->conn_info);
	wl->conn_info = NULL;
	kfree(wl->ie);
	wl->ie = NULL;
	wl->escan_info.escan_buf = NULL;
#endif /* STATIC_WL_PRIV_STRUCT */
	if (wl->afx_hdl) {
		cancel_work_sync(&wl->afx_hdl->work);
		kfree(wl->afx_hdl);
		wl->afx_hdl = NULL;
	}

	if (wl->ap_info) {
		kfree(wl->ap_info->wpa_ie);
		kfree(wl->ap_info->rsn_ie);
		kfree(wl->ap_info->wps_ie);
		kfree(wl->ap_info);
		wl->ap_info = NULL;
	}
}

static s32 wl_create_event_handler(struct wl_priv *wl)
=======
static void wl_deinit_priv_mem(struct bcm_cfg80211 *cfg)
{
	kfree(cfg->scan_results);
	cfg->scan_results = NULL;
	kfree(cfg->conf);
	cfg->conf = NULL;
	kfree(cfg->scan_req_int);
	cfg->scan_req_int = NULL;
	kfree(cfg->ioctl_buf);
	cfg->ioctl_buf = NULL;
	kfree(cfg->escan_ioctl_buf);
	cfg->escan_ioctl_buf = NULL;
	kfree(cfg->extra_buf);
	cfg->extra_buf = NULL;
	kfree(cfg->pmk_list);
	cfg->pmk_list = NULL;
	kfree(cfg->sta_info);
	cfg->sta_info = NULL;
#if defined(STATIC_WL_PRIV_STRUCT)
	kfree(cfg->conn_info);
	cfg->conn_info = NULL;
	kfree(cfg->ie);
	cfg->ie = NULL;
	wl_deinit_escan_result_buf(cfg);
#endif /* STATIC_WL_PRIV_STRUCT */
	if (cfg->afx_hdl) {
		cancel_work_sync(&cfg->afx_hdl->work);
		kfree(cfg->afx_hdl);
		cfg->afx_hdl = NULL;
	}

	if (cfg->ap_info) {
		kfree(cfg->ap_info->wpa_ie);
		kfree(cfg->ap_info->rsn_ie);
		kfree(cfg->ap_info->wps_ie);
		kfree(cfg->ap_info);
		cfg->ap_info = NULL;
	}
}

static s32 wl_create_event_handler(struct bcm_cfg80211 *cfg)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	int ret = 0;
	WL_DBG(("Enter \n"));

	/* Do not use DHD in cfg driver */
<<<<<<< HEAD
	wl->event_tsk.thr_pid = -1;

#ifdef USE_KTHREAD_API
	PROC_START2(wl_event_handler, wl, &wl->event_tsk, 0, "wl_event_handler");
#else
	PROC_START(wl_event_handler, wl, &wl->event_tsk, 0);
#endif
	if (wl->event_tsk.thr_pid < 0)
=======
	cfg->event_tsk.thr_pid = -1;

	PROC_START(wl_event_handler, cfg, &cfg->event_tsk, 0, "wl_event_handler");
	if (cfg->event_tsk.thr_pid < 0)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		ret = -ENOMEM;
	return ret;
}

<<<<<<< HEAD
static void wl_destroy_event_handler(struct wl_priv *wl)
{
	if (wl->event_tsk.thr_pid >= 0)
		PROC_STOP(&wl->event_tsk);
}

static void wl_term_iscan(struct wl_priv *wl)
{
	struct wl_iscan_ctrl *iscan = wl_to_iscan(wl);
	WL_TRACE(("In\n"));
	if (wl->iscan_on && iscan->tsk) {
		iscan->state = WL_ISCAN_STATE_IDLE;
		WL_INFO(("SIGTERM\n"));
		send_sig(SIGTERM, iscan->tsk, 1);
		WL_DBG(("kthread_stop\n"));
		kthread_stop(iscan->tsk);
		iscan->tsk = NULL;
	}
}

static void wl_notify_iscan_complete(struct wl_iscan_ctrl *iscan, bool aborted)
{
	struct wl_priv *wl = iscan_to_wl(iscan);
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	unsigned long flags;

	WL_DBG(("Enter \n"));
	if (!wl_get_drv_status(wl, SCANNING, ndev)) {
		wl_clr_drv_status(wl, SCANNING, ndev);
		WL_ERR(("Scan complete while device not scanning\n"));
		return;
	}
	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
	wl_clr_drv_status(wl, SCANNING, ndev);
	if (likely(wl->scan_request)) {
		cfg80211_scan_done(wl->scan_request, aborted);
		wl->scan_request = NULL;
	}
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);
	wl->iscan_kickstart = false;
}

static s32 wl_wakeup_iscan(struct wl_iscan_ctrl *iscan)
{
	if (likely(iscan->state != WL_ISCAN_STATE_IDLE)) {
		WL_DBG(("wake up iscan\n"));
		up(&iscan->sync);
		return 0;
	}

	return -EIO;
}

static s32
wl_get_iscan_results(struct wl_iscan_ctrl *iscan, u32 *status,
	struct wl_scan_results **bss_list)
{
	struct wl_iscan_results list;
	struct wl_scan_results *results;
	struct wl_iscan_results *list_buf;
	s32 err = 0;

	WL_DBG(("Enter \n"));
	memset(iscan->scan_buf, 0, WL_ISCAN_BUF_MAX);
	list_buf = (struct wl_iscan_results *)iscan->scan_buf;
	results = &list_buf->results;
	results->buflen = WL_ISCAN_RESULTS_FIXED_SIZE;
	results->version = 0;
	results->count = 0;

	memset(&list, 0, sizeof(list));
	list.results.buflen = htod32(WL_ISCAN_BUF_MAX);
	err = wldev_iovar_getbuf(iscan->dev, "iscanresults", &list,
		WL_ISCAN_RESULTS_FIXED_SIZE, iscan->scan_buf,
		WL_ISCAN_BUF_MAX, NULL);
	if (unlikely(err)) {
		WL_ERR(("error (%d)\n", err));
		return err;
	}
	results->buflen = dtoh32(results->buflen);
	results->version = dtoh32(results->version);
	results->count = dtoh32(results->count);
	WL_DBG(("results->count = %d\n", results->count));
	WL_DBG(("results->buflen = %d\n", results->buflen));
	*status = dtoh32(list_buf->status);
	*bss_list = results;

	return err;
}

static s32 wl_iscan_done(struct wl_priv *wl)
{
	struct wl_iscan_ctrl *iscan = wl->iscan;
	s32 err = 0;

	iscan->state = WL_ISCAN_STATE_IDLE;
	mutex_lock(&wl->usr_sync);
	wl_inform_bss(wl);
	wl_notify_iscan_complete(iscan, false);
	mutex_unlock(&wl->usr_sync);

	return err;
}

static s32 wl_iscan_pending(struct wl_priv *wl)
{
	struct wl_iscan_ctrl *iscan = wl->iscan;
	s32 err = 0;

	/* Reschedule the timer */
	mod_timer(&iscan->timer, jiffies + msecs_to_jiffies(iscan->timer_ms));
	iscan->timer_on = 1;

	return err;
}

static s32 wl_iscan_inprogress(struct wl_priv *wl)
{
	struct wl_iscan_ctrl *iscan = wl->iscan;
	s32 err = 0;

	mutex_lock(&wl->usr_sync);
	wl_inform_bss(wl);
	wl_run_iscan(iscan, NULL, WL_SCAN_ACTION_CONTINUE);
	mutex_unlock(&wl->usr_sync);
	/* Reschedule the timer */
	mod_timer(&iscan->timer, jiffies +  msecs_to_jiffies(iscan->timer_ms));
	iscan->timer_on = 1;

	return err;
}

static s32 wl_iscan_aborted(struct wl_priv *wl)
{
	struct wl_iscan_ctrl *iscan = wl->iscan;
	s32 err = 0;

	iscan->state = WL_ISCAN_STATE_IDLE;
	mutex_lock(&wl->usr_sync);
	wl_notify_iscan_complete(iscan, true);
	mutex_unlock(&wl->usr_sync);

	return err;
}

static s32 wl_iscan_thread(void *data)
{
	struct wl_iscan_ctrl *iscan = (struct wl_iscan_ctrl *)data;
	struct wl_priv *wl = iscan_to_wl(iscan);
	u32 status;
	int err = 0;

	allow_signal(SIGTERM);
	status = WL_SCAN_RESULTS_PARTIAL;
	while (likely(!down_interruptible(&iscan->sync))) {
		if (kthread_should_stop())
			break;
		if (iscan->timer_on) {
			del_timer_sync(&iscan->timer);
			iscan->timer_on = 0;
		}
		mutex_lock(&wl->usr_sync);
		err = wl_get_iscan_results(iscan, &status, &wl->bss_list);
		if (unlikely(err)) {
			status = WL_SCAN_RESULTS_ABORTED;
			WL_ERR(("Abort iscan\n"));
		}
		mutex_unlock(&wl->usr_sync);
		iscan->iscan_handler[status] (wl);
	}
	if (iscan->timer_on) {
		del_timer_sync(&iscan->timer);
		iscan->timer_on = 0;
	}
	WL_DBG(("%s was terminated\n", __func__));

	return 0;
=======
static void wl_destroy_event_handler(struct bcm_cfg80211 *cfg)
{
	if (cfg->event_tsk.thr_pid >= 0)
		PROC_STOP(&cfg->event_tsk);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

static void wl_scan_timeout(unsigned long data)
{
<<<<<<< HEAD
	struct wl_priv *wl = (struct wl_priv *)data;

	if (wl->scan_request) {
		WL_ERR(("timer expired\n"));
		if (wl->escan_on)
			wl_notify_escan_complete(wl, wl->escan_info.ndev, true, true);
		else
			wl_notify_iscan_complete(wl_to_iscan(wl), true);
	}
}
static void wl_iscan_timer(unsigned long data)
{
	struct wl_iscan_ctrl *iscan = (struct wl_iscan_ctrl *)data;

	if (iscan) {
		iscan->timer_on = 0;
		WL_DBG(("timer expired\n"));
		wl_wakeup_iscan(iscan);
	}
}

static s32 wl_invoke_iscan(struct wl_priv *wl)
{
	struct wl_iscan_ctrl *iscan = wl_to_iscan(wl);
	int err = 0;

	if (wl->iscan_on && !iscan->tsk) {
		iscan->state = WL_ISCAN_STATE_IDLE;
		sema_init(&iscan->sync, 0);
		iscan->tsk = kthread_run(wl_iscan_thread, iscan, "wl_iscan");
		if (IS_ERR(iscan->tsk)) {
			WL_ERR(("Could not create iscan thread\n"));
			iscan->tsk = NULL;
			return -ENOMEM;
		}
	}

	return err;
}

static void wl_init_iscan_handler(struct wl_iscan_ctrl *iscan)
{
	memset(iscan->iscan_handler, 0, sizeof(iscan->iscan_handler));
	iscan->iscan_handler[WL_SCAN_RESULTS_SUCCESS] = wl_iscan_done;
	iscan->iscan_handler[WL_SCAN_RESULTS_PARTIAL] = wl_iscan_inprogress;
	iscan->iscan_handler[WL_SCAN_RESULTS_PENDING] = wl_iscan_pending;
	iscan->iscan_handler[WL_SCAN_RESULTS_ABORTED] = wl_iscan_aborted;
	iscan->iscan_handler[WL_SCAN_RESULTS_NO_MEM] = wl_iscan_aborted;
=======
	wl_event_msg_t msg;
	struct bcm_cfg80211 *cfg = (struct bcm_cfg80211 *)data;

	if (!(cfg->scan_request)) {
		WL_ERR(("timer expired but no scan request\n"));
		return;
	}
	bzero(&msg, sizeof(wl_event_msg_t));
	WL_ERR(("timer expired\n"));
	msg.event_type = hton32(WLC_E_ESCAN_RESULT);
	msg.status = hton32(WLC_E_STATUS_TIMEOUT);
	msg.reason = 0xFFFFFFFF;
	wl_cfg80211_event(bcmcfg_to_prmry_ndev(cfg), &msg, NULL);
#if defined(CUSTOMER_HW4) && defined(DHD_DEBUG)
	if (!wl_scan_timeout_dbg_enabled)
		wl_scan_timeout_dbg_set();
#endif /* CUSTOMER_HW4 && DHD_DEBUG */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

static s32
wl_cfg80211_netdev_notifier_call(struct notifier_block * nb,
	unsigned long state,
	void *ndev)
{
	struct net_device *dev = ndev;
	struct wireless_dev *wdev = dev->ieee80211_ptr;
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
	int refcnt = 0;

	WL_DBG(("Enter \n"));
	if (!wdev || !wl || dev == wl_to_prmry_ndev(wl))
		return NOTIFY_DONE;
	switch (state) {
		case NETDEV_DOWN:
			while (work_pending(&wdev->cleanup_work) && refcnt < 100) {
				if (refcnt%5 == 0)
					WL_ERR(("%s : [NETDEV_DOWN] work_pending (%d th)\n",
						__FUNCTION__, refcnt));
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	int refcnt = 0;

	WL_DBG(("Enter \n"));

	if (!wdev || !cfg || dev == bcmcfg_to_prmry_ndev(cfg))
		return NOTIFY_DONE;

	switch (state) {
		case NETDEV_DOWN:
		{
			int max_wait_timeout = 2;
			int max_wait_count = 100;
			unsigned long limit = jiffies + max_wait_timeout * HZ;
			while (work_pending(&wdev->cleanup_work)) {
				if (refcnt%5 == 0) {
					WL_ERR(("[NETDEV_DOWN] wait for "
						"complete of cleanup_work"
						" (%d th)\n", refcnt));
				}
				if (!time_before(jiffies, limit)) {
					WL_ERR(("[NETDEV_DOWN] cleanup_work"
						" of CFG80211 is not"
						" completed in %d sec\n",
						max_wait_timeout));
					break;
				}
				if (refcnt >= max_wait_count) {
					WL_ERR(("[NETDEV_DOWN] cleanup_work"
						" of CFG80211 is not"
						" completed in %d loop\n",
						max_wait_count));
					break;
				}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				set_current_state(TASK_INTERRUPTIBLE);
				schedule_timeout(100);
				set_current_state(TASK_RUNNING);
				refcnt++;
			}
			break;
<<<<<<< HEAD

		case NETDEV_UNREGISTER:
			/* after calling list_del_rcu(&wdev->list) */
			wl_dealloc_netinfo(wl, ndev);
=======
		}

		case NETDEV_UNREGISTER:
			/* after calling list_del_rcu(&wdev->list) */
			wl_dealloc_netinfo(cfg, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			break;
		case NETDEV_GOING_DOWN:
			/* At NETDEV_DOWN state, wdev_cleanup_work work will be called.
			*  In front of door, the function checks
			*  whether current scan is working or not.
			*  If the scanning is still working, wdev_cleanup_work call WARN_ON and
			*  make the scan done forcibly.
			*/
<<<<<<< HEAD
			if (wl_get_drv_status(wl, SCANNING, dev)) {
				if (wl->escan_on) {
					wl_notify_escan_complete(wl, dev, true, true);
				}
			}
=======
			if (wl_get_drv_status(cfg, SCANNING, dev))
				wl_notify_escan_complete(cfg, dev, true, true);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			break;
	}
	return NOTIFY_DONE;
}
static struct notifier_block wl_cfg80211_netdev_notifier = {
	.notifier_call = wl_cfg80211_netdev_notifier_call,
};
<<<<<<< HEAD

static s32 wl_notify_escan_complete(struct wl_priv *wl,
	struct net_device *ndev,
	bool aborted, bool fw_abort)
=======
/* to make sure we won't register the same notifier twice, otherwise a loop is likely to be
 * created in kernel notifier link list (with 'next' pointing to itself)
 */
static bool wl_cfg80211_netdev_notifier_registered = FALSE;

static void wl_cfg80211_scan_abort(struct bcm_cfg80211 *cfg)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	wl_scan_params_t *params = NULL;
	s32 params_size = 0;
	s32 err = BCME_OK;
<<<<<<< HEAD
	unsigned long flags;
	struct net_device *dev;

	WL_DBG(("Enter \n"));

	if (wl->escan_info.ndev != ndev)
	{
		WL_ERR(("ndev is different %p %p\n", wl->escan_info.ndev, ndev));
		return err;
	}

	if (wl->scan_request) {
		if (wl->scan_request->dev == wl->p2p_net)
			dev = wl_to_prmry_ndev(wl);
		else
			dev = wl->scan_request->dev;
	}
	else {
		WL_DBG(("wl->scan_request is NULL may be internal scan."
			"doing scan_abort for ndev %p primary %p p2p_net %p",
				ndev, wl_to_prmry_ndev(wl), wl->p2p_net));
		dev = ndev;
	}
	if (fw_abort && !in_atomic()) {
=======
	struct net_device *dev = bcmcfg_to_prmry_ndev(cfg);
	if (!in_atomic()) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		/* Our scan params only need space for 1 channel and 0 ssids */
		params = wl_cfg80211_scan_alloc_params(-1, 0, &params_size);
		if (params == NULL) {
			WL_ERR(("scan params allocation failed \n"));
			err = -ENOMEM;
		} else {
			/* Do a scan abort to stop the driver's scan engine */
			err = wldev_ioctl(dev, WLC_SCAN, params, params_size, true);
			if (err < 0) {
				WL_ERR(("scan abort  failed \n"));
			}
<<<<<<< HEAD
		}
	}
	if (timer_pending(&wl->scan_timeout))
		del_timer_sync(&wl->scan_timeout);
#if defined(ESCAN_RESULT_PATCH)
	if (likely(wl->scan_request)) {
		wl->bss_list = (wl_scan_results_t *)wl->escan_info.escan_buf;
		wl_inform_bss(wl);
	}
#endif /* ESCAN_RESULT_PATCH */
	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
#ifdef WL_SCHED_SCAN
	if (wl->sched_scan_req && !wl->scan_request) {
		WL_PNO((">>> REPORTING SCHED SCAN RESULTS \n"));
		if (aborted)
			cfg80211_sched_scan_stopped(wl->sched_scan_req->wiphy);
		else
			cfg80211_sched_scan_results(wl->sched_scan_req->wiphy);
		wl->sched_scan_running = FALSE;
		wl->sched_scan_req = NULL;
	}
#endif /* WL_SCHED_SCAN */
	if (likely(wl->scan_request)) {
		cfg80211_scan_done(wl->scan_request, aborted);
		wl->scan_request = NULL;
	}
	if (p2p_is_on(wl))
		wl_clr_p2p_status(wl, SCANNING);
	wl_clr_drv_status(wl, SCANNING, dev);
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);
	if (params)
		kfree(params);
=======
			kfree(params);
		}
	}
}

static s32 wl_notify_escan_complete(struct bcm_cfg80211 *cfg,
	struct net_device *ndev,
	bool aborted, bool fw_abort)
{
	s32 err = BCME_OK;
	unsigned long flags;
	struct net_device *dev;

	WL_DBG(("Enter \n"));
	if (!ndev) {
		WL_ERR(("ndev is null\n"));
		err = BCME_ERROR;
		return err;
	}

	if (cfg->escan_info.ndev != ndev) {
		WL_ERR(("ndev is different %p %p\n", cfg->escan_info.ndev, ndev));
		err = BCME_ERROR;
		return err;
	}

	if (cfg->scan_request) {
		dev = bcmcfg_to_prmry_ndev(cfg);
#if defined(WL_ENABLE_P2P_IF)
		if (cfg->scan_request->dev != cfg->p2p_net)
			dev = cfg->scan_request->dev;
#endif /* WL_ENABLE_P2P_IF */
	}
	else {
		WL_DBG(("cfg->scan_request is NULL may be internal scan."
			"doing scan_abort for ndev %p primary %p",
				ndev, bcmcfg_to_prmry_ndev(cfg)));
		dev = ndev;
	}
	if (fw_abort && !in_atomic())
		wl_cfg80211_scan_abort(cfg);
	if (timer_pending(&cfg->scan_timeout))
		del_timer_sync(&cfg->scan_timeout);
#if defined(ESCAN_RESULT_PATCH)
	if (likely(cfg->scan_request)) {
		cfg->bss_list = wl_escan_get_buf(cfg, aborted);
		wl_inform_bss(cfg);
	}
#endif /* ESCAN_RESULT_PATCH */
	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
#ifdef WL_SCHED_SCAN
	if (cfg->sched_scan_req && !cfg->scan_request) {
		WL_PNO((">>> REPORTING SCHED SCAN RESULTS \n"));
		if (!aborted)
			cfg80211_sched_scan_results(cfg->sched_scan_req->wiphy);
		cfg->sched_scan_running = FALSE;
		cfg->sched_scan_req = NULL;
	}
#endif /* WL_SCHED_SCAN */
	if (likely(cfg->scan_request)) {
		cfg80211_scan_done(cfg->scan_request, aborted);
		cfg->scan_request = NULL;
	}
	if (p2p_is_on(cfg))
		wl_clr_p2p_status(cfg, SCANNING);
	wl_clr_drv_status(cfg, SCANNING, dev);
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
static s32 wl_escan_handler(struct wl_priv *wl,
	struct net_device *ndev,
=======
static s32 wl_escan_handler(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	const wl_event_msg_t *e, void *data)
{
	s32 err = BCME_OK;
	s32 status = ntoh32(e->status);
	wl_bss_info_t *bi;
	wl_escan_result_t *escan_result;
	wl_bss_info_t *bss = NULL;
	wl_scan_results_t *list;
	wifi_p2p_ie_t * p2p_ie;
<<<<<<< HEAD
=======
	struct net_device *ndev = NULL;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	u32 bi_length;
	u32 i;
	u8 *p2p_dev_addr = NULL;

	WL_DBG((" enter event type : %d, status : %d \n",
		ntoh32(e->event_type), ntoh32(e->status)));

<<<<<<< HEAD
	mutex_lock(&wl->usr_sync);
	/* P2P SCAN is coming from primary interface */
	if (wl_get_p2p_status(wl, SCANNING)) {
		if (wl_get_drv_status_all(wl, SENDING_ACT_FRM))
			ndev = wl->afx_hdl->dev;
		else
			ndev = wl->escan_info.ndev;

	}
	if (!ndev || !wl->escan_on ||
		(!wl_get_drv_status(wl, SCANNING, ndev) &&
		!wl->sched_scan_running)) {
		WL_ERR(("escan is not ready ndev %p wl->escan_on %d drv_status 0x%x\n",
			ndev, wl->escan_on, wl_get_drv_status(wl, SCANNING, ndev)));
		goto exit;
	}
	if (status == WLC_E_STATUS_PARTIAL) {
		WL_INFO(("WLC_E_STATUS_PARTIAL \n"));
		escan_result = (wl_escan_result_t *) data;
=======
	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	mutex_lock(&cfg->usr_sync);
	/* P2P SCAN is coming from primary interface */
	if (wl_get_p2p_status(cfg, SCANNING)) {
		if (wl_get_drv_status_all(cfg, SENDING_ACT_FRM))
			ndev = cfg->afx_hdl->dev;
		else
			ndev = cfg->escan_info.ndev;

	}
	if (!ndev || (!wl_get_drv_status(cfg, SCANNING, ndev) && !cfg->sched_scan_running)) {
		WL_ERR(("escan is not ready ndev %p drv_status 0x%x e_type %d e_states %d\n",
			ndev, wl_get_drv_status(cfg, SCANNING, ndev),
			ntoh32(e->event_type), ntoh32(e->status)));
		goto exit;
	}
	escan_result = (wl_escan_result_t *)data;

	if (status == WLC_E_STATUS_PARTIAL) {
		WL_INFO(("WLC_E_STATUS_PARTIAL \n"));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		if (!escan_result) {
			WL_ERR(("Invalid escan result (NULL pointer)\n"));
			goto exit;
		}
		if (dtoh16(escan_result->bss_count) != 1) {
			WL_ERR(("Invalid bss_count %d: ignoring\n", escan_result->bss_count));
			goto exit;
		}
		bi = escan_result->bss_info;
		if (!bi) {
			WL_ERR(("Invalid escan bss info (NULL pointer)\n"));
			goto exit;
		}
		bi_length = dtoh32(bi->length);
		if (bi_length != (dtoh32(escan_result->buflen) - WL_ESCAN_RESULTS_FIXED_SIZE)) {
			WL_ERR(("Invalid bss_info length %d: ignoring\n", bi_length));
			goto exit;
		}
<<<<<<< HEAD

		if (!(wl_to_wiphy(wl)->interface_modes & BIT(NL80211_IFTYPE_ADHOC))) {
=======
		if (wl_escan_check_sync_id(status, escan_result->sync_id,
			cfg->escan_info.cur_sync_id) < 0)
			goto exit;

		if (!(bcmcfg_to_wiphy(cfg)->interface_modes & BIT(NL80211_IFTYPE_ADHOC))) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			if (dtoh16(bi->capability) & DOT11_CAP_IBSS) {
				WL_DBG(("Ignoring IBSS result\n"));
				goto exit;
			}
		}

<<<<<<< HEAD
		if (wl_get_drv_status_all(wl, FINDING_COMMON_CHANNEL)) {
			p2p_dev_addr = wl_cfgp2p_retreive_p2p_dev_addr(bi, bi_length);
			if (p2p_dev_addr && !memcmp(p2p_dev_addr,
				wl->afx_hdl->tx_dst_addr.octet, ETHER_ADDR_LEN)) {
				s32 channel = CHSPEC_CHANNEL(
					wl_chspec_driver_to_host(bi->chanspec));
				WL_DBG(("ACTION FRAME SCAN : Peer " MACDBG " found, channel : %d\n",
					MAC2STRDBG(wl->afx_hdl->tx_dst_addr.octet), channel));
				wl_clr_p2p_status(wl, SCANNING);
				wl->afx_hdl->peer_chan = channel;
				complete(&wl->act_frm_scan);
=======
		if (wl_get_drv_status_all(cfg, FINDING_COMMON_CHANNEL)) {
			p2p_dev_addr = wl_cfgp2p_retreive_p2p_dev_addr(bi, bi_length);
			if (p2p_dev_addr && !memcmp(p2p_dev_addr,
				cfg->afx_hdl->tx_dst_addr.octet, ETHER_ADDR_LEN)) {
				s32 channel = wf_chspec_ctlchan(
					wl_chspec_driver_to_host(bi->chanspec));

				if ((channel > MAXCHANNEL) || (channel <= 0))
					channel = WL_INVALID;
				else
					WL_ERR(("ACTION FRAME SCAN : Peer " MACDBG " found,"
						" channel : %d\n",
						MAC2STRDBG(cfg->afx_hdl->tx_dst_addr.octet),
						channel));

				wl_clr_p2p_status(cfg, SCANNING);
				cfg->afx_hdl->peer_chan = channel;
				complete(&cfg->act_frm_scan);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				goto exit;
			}

		} else {
<<<<<<< HEAD
			int cur_len = 0;
			list = (wl_scan_results_t *)wl->escan_info.escan_buf;
#if defined(WLP2P) && defined(WL_ENABLE_P2P_IF)
			if (wl->p2p_net && wl->scan_request &&
				wl->scan_request->dev == wl->p2p_net) {
#else
			if (p2p_is_on(wl) && p2p_scan(wl)) {
#endif
=======
			int cur_len = WL_SCAN_RESULTS_FIXED_SIZE;
			list = wl_escan_get_buf(cfg, FALSE);
			if (scan_req_match(cfg)) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
#ifdef WL_HOST_BAND_MGMT
				s32 channel = 0;
				s32 channel_band = 0;
#endif /* WL_HOST_BAND_MGMT */
				/* p2p scan && allow only probe response */
<<<<<<< HEAD
				if (bi->flags & WL_BSS_FLAGS_FROM_BEACON)
=======
				if ((cfg->p2p->search_state != WL_P2P_DISC_ST_SCAN) &&
					(bi->flags & WL_BSS_FLAGS_FROM_BEACON))
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
					goto exit;
				if ((p2p_ie = wl_cfgp2p_find_p2pie(((u8 *) bi) + bi->ie_offset,
					bi->ie_length)) == NULL) {
						WL_ERR(("Couldn't find P2PIE in probe"
							" response/beacon\n"));
						goto exit;
				}
#ifdef WL_HOST_BAND_MGMT
				channel = CHSPEC_CHANNEL(wl_chspec_driver_to_host(bi->chanspec));
				channel_band = (channel > CH_MAX_2G_CHANNEL) ?
				WLC_BAND_5G : WLC_BAND_2G;


<<<<<<< HEAD
				if ((wl->curr_band == WLC_BAND_5G) &&
=======
				if ((cfg->curr_band == WLC_BAND_5G) &&
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
					(channel_band == WLC_BAND_2G)) {
					/* Avoid sending the GO results in band conflict */
					if (wl_cfgp2p_retreive_p2pattrib(p2p_ie,
						P2P_SEID_GROUP_ID) != NULL)
						goto exit;
				}
#endif /* WL_HOST_BAND_MGMT */
			}
			for (i = 0; i < list->count; i++) {
				bss = bss ? (wl_bss_info_t *)((uintptr)bss + dtoh32(bss->length))
					: list->bss_info;

				if (!bcmp(&bi->BSSID, &bss->BSSID, ETHER_ADDR_LEN) &&
					(CHSPEC_BAND(wl_chspec_driver_to_host(bi->chanspec))
					== CHSPEC_BAND(wl_chspec_driver_to_host(bss->chanspec))) &&
					bi->SSID_len == bss->SSID_len &&
					!bcmp(bi->SSID, bss->SSID, bi->SSID_len)) {

					/* do not allow beacon data to update
					*the data recd from a probe response
					*/
					if (!(bss->flags & WL_BSS_FLAGS_FROM_BEACON) &&
						(bi->flags & WL_BSS_FLAGS_FROM_BEACON))
						goto exit;

					WL_DBG(("%s("MACDBG"), i=%d prev: RSSI %d"
						" flags 0x%x, new: RSSI %d flags 0x%x\n",
						bss->SSID, MAC2STRDBG(bi->BSSID.octet), i,
						bss->RSSI, bss->flags, bi->RSSI, bi->flags));

					if ((bss->flags & WL_BSS_FLAGS_RSSI_ONCHANNEL) ==
						(bi->flags & WL_BSS_FLAGS_RSSI_ONCHANNEL)) {
						/* preserve max RSSI if the measurements are
						* both on-channel or both off-channel
						*/
						WL_SCAN(("%s("MACDBG"), same onchan"
						", RSSI: prev %d new %d\n",
						bss->SSID, MAC2STRDBG(bi->BSSID.octet),
						bss->RSSI, bi->RSSI));
						bi->RSSI = MAX(bss->RSSI, bi->RSSI);
					} else if ((bss->flags & WL_BSS_FLAGS_RSSI_ONCHANNEL) &&
						(bi->flags & WL_BSS_FLAGS_RSSI_ONCHANNEL) == 0) {
						/* preserve the on-channel rssi measurement
						* if the new measurement is off channel
						*/
						WL_SCAN(("%s("MACDBG"), prev onchan"
						", RSSI: prev %d new %d\n",
						bss->SSID, MAC2STRDBG(bi->BSSID.octet),
						bss->RSSI, bi->RSSI));
						bi->RSSI = bss->RSSI;
						bi->flags |= WL_BSS_FLAGS_RSSI_ONCHANNEL;
					}
					if (dtoh32(bss->length) != bi_length) {
						u32 prev_len = dtoh32(bss->length);

						WL_SCAN(("bss info replacement"
							" is occured(bcast:%d->probresp%d)\n",
							bss->ie_length, bi->ie_length));
						WL_DBG(("%s("MACDBG"), replacement!(%d -> %d)\n",
						bss->SSID, MAC2STRDBG(bi->BSSID.octet),
						prev_len, bi_length));

						if (list->buflen - prev_len + bi_length
							> ESCAN_BUF_SIZE) {
							WL_ERR(("Buffer is too small: keep the"
								" previous result of this AP\n"));
							/* Only update RSSI */
							bss->RSSI = bi->RSSI;
							bss->flags |= (bi->flags
								& WL_BSS_FLAGS_RSSI_ONCHANNEL);
							goto exit;
						}

						if (i < list->count - 1) {
							/* memory copy required by this case only */
							memmove((u8 *)bss + bi_length,
								(u8 *)bss + prev_len,
								list->buflen - cur_len - prev_len);
						}
						list->buflen -= prev_len;
						list->buflen += bi_length;
					}
					list->version = dtoh32(bi->version);
					memcpy((u8 *)bss, (u8 *)bi, bi_length);
					goto exit;
				}
				cur_len += dtoh32(bss->length);
			}
			if (bi_length > ESCAN_BUF_SIZE - list->buflen) {
				WL_ERR(("Buffer is too small: ignoring\n"));
				goto exit;
			}
<<<<<<< HEAD
			memcpy(&(wl->escan_info.escan_buf[list->buflen]), bi, bi_length);
			list->version = dtoh32(bi->version);
			list->buflen += bi_length;
			list->count++;
=======

			memcpy(&(((char *)list)[list->buflen]), bi, bi_length);
			list->version = dtoh32(bi->version);
			list->buflen += bi_length;
			list->count++;

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}

	}
	else if (status == WLC_E_STATUS_SUCCESS) {
<<<<<<< HEAD
		wl->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
		if (wl_get_drv_status_all(wl, FINDING_COMMON_CHANNEL)) {
			WL_INFO(("ACTION FRAME SCAN DONE\n"));
			wl_clr_p2p_status(wl, SCANNING);
			wl_clr_drv_status(wl, SCANNING, wl->afx_hdl->dev);
			if (wl->afx_hdl->peer_chan == WL_INVALID)
				complete(&wl->act_frm_scan);
		} else if ((likely(wl->scan_request)) || (wl->sched_scan_running)) {
			WL_INFO(("ESCAN COMPLETED\n"));
			wl->bss_list = (wl_scan_results_t *)wl->escan_info.escan_buf;
			wl_inform_bss(wl);
			wl_notify_escan_complete(wl, ndev, false, false);
		}
	}
	else if (status == WLC_E_STATUS_ABORT) {
		wl->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
		if (wl_get_drv_status_all(wl, FINDING_COMMON_CHANNEL)) {
			WL_INFO(("ACTION FRAME SCAN DONE\n"));
			wl_clr_drv_status(wl, SCANNING, wl->afx_hdl->dev);
			wl_clr_p2p_status(wl, SCANNING);
			if (wl->afx_hdl->peer_chan == WL_INVALID)
				complete(&wl->act_frm_scan);
		} else if ((likely(wl->scan_request)) || (wl->sched_scan_running)) {
			WL_INFO(("ESCAN ABORTED\n"));
			wl->bss_list = (wl_scan_results_t *)wl->escan_info.escan_buf;
			wl_inform_bss(wl);
			wl_notify_escan_complete(wl, ndev, true, false);
		}
	}
	else if (status == WLC_E_STATUS_NEWSCAN)
	{
		escan_result = (wl_escan_result_t *) data;
		WL_ERR(("WLC_E_STATUS_NEWSCAN : scan_request[%p]\n", wl->scan_request));
		WL_ERR(("sync_id[%d], bss_count[%d]\n", escan_result->sync_id,
			escan_result->bss_count));
	} else {
		WL_ERR(("unexpected Escan Event %d : abort\n", status));
		wl->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
		if (wl_get_drv_status_all(wl, FINDING_COMMON_CHANNEL)) {
			WL_INFO(("ACTION FRAME SCAN DONE\n"));
			wl_clr_p2p_status(wl, SCANNING);
			wl_clr_drv_status(wl, SCANNING, wl->afx_hdl->dev);
			if (wl->afx_hdl->peer_chan == WL_INVALID)
				complete(&wl->act_frm_scan);
		} else if ((likely(wl->scan_request)) || (wl->sched_scan_running)) {
			wl->bss_list = (wl_scan_results_t *)wl->escan_info.escan_buf;
			wl_inform_bss(wl);
			wl_notify_escan_complete(wl, ndev, true, false);
		}
	}
exit:
	mutex_unlock(&wl->usr_sync);
	return err;
}
static void wl_cfg80211_concurrent_roam(struct wl_priv *wl, int enable)
{
	u32 connected_cnt  = wl_get_drv_status_all(wl, CONNECTED);
	struct net_info *iter, *next;
	int err;

	if (!wl->roamoff_on_concurrent)
		return;
	if (enable && connected_cnt > 1) {
		for_each_ndev(wl, iter, next) {
=======
		cfg->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
		wl_escan_print_sync_id(status, cfg->escan_info.cur_sync_id,
			escan_result->sync_id);

		if (wl_get_drv_status_all(cfg, FINDING_COMMON_CHANNEL)) {
			WL_INFO(("ACTION FRAME SCAN DONE\n"));
			wl_clr_p2p_status(cfg, SCANNING);
			wl_clr_drv_status(cfg, SCANNING, cfg->afx_hdl->dev);
			if (cfg->afx_hdl->peer_chan == WL_INVALID)
				complete(&cfg->act_frm_scan);
		} else if ((likely(cfg->scan_request)) || (cfg->sched_scan_running)) {
			WL_INFO(("ESCAN COMPLETED\n"));
			cfg->bss_list = wl_escan_get_buf(cfg, FALSE);
			if (!scan_req_match(cfg)) {
				WL_TRACE_HW4(("SCAN COMPLETED: scanned AP count=%d\n",
					cfg->bss_list->count));
			}
			wl_inform_bss(cfg);
			wl_notify_escan_complete(cfg, ndev, false, false);
		}
		wl_escan_increment_sync_id(cfg, SCAN_BUF_NEXT);
#if defined(CUSTOMER_HW4) && defined(DHD_DEBUG)
		if (wl_scan_timeout_dbg_enabled)
			wl_scan_timeout_dbg_clear();
#endif /* CUSTOMER_HW4 && DHD_DEBUG */
	}
	else if (status == WLC_E_STATUS_ABORT) {
		cfg->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
		wl_escan_print_sync_id(status, escan_result->sync_id,
			cfg->escan_info.cur_sync_id);
		if (wl_get_drv_status_all(cfg, FINDING_COMMON_CHANNEL)) {
			WL_INFO(("ACTION FRAME SCAN DONE\n"));
			wl_clr_drv_status(cfg, SCANNING, cfg->afx_hdl->dev);
			wl_clr_p2p_status(cfg, SCANNING);
			if (cfg->afx_hdl->peer_chan == WL_INVALID)
				complete(&cfg->act_frm_scan);
		} else if ((likely(cfg->scan_request)) || (cfg->sched_scan_running)) {
			WL_INFO(("ESCAN ABORTED\n"));
			cfg->bss_list = wl_escan_get_buf(cfg, TRUE);
			if (!scan_req_match(cfg)) {
				WL_TRACE_HW4(("SCAN ABORTED: scanned AP count=%d\n",
					cfg->bss_list->count));
			}
			wl_inform_bss(cfg);
			wl_notify_escan_complete(cfg, ndev, true, false);
		}
		wl_escan_increment_sync_id(cfg, SCAN_BUF_CNT);
	} else if (status == WLC_E_STATUS_NEWSCAN) {
		WL_ERR(("WLC_E_STATUS_NEWSCAN : scan_request[%p]\n", cfg->scan_request));
		WL_ERR(("sync_id[%d], bss_count[%d]\n", escan_result->sync_id,
			escan_result->bss_count));
	} else if (status == WLC_E_STATUS_TIMEOUT) {
		WL_ERR(("WLC_E_STATUS_TIMEOUT : scan_request[%p]\n", cfg->scan_request));
		WL_ERR(("reason[0x%x]\n", e->reason));
		if (e->reason == 0xFFFFFFFF) {
			wl_notify_escan_complete(cfg, cfg->escan_info.ndev, true, true);
		}
	} else {
		WL_ERR(("unexpected Escan Event %d : abort\n", status));
		cfg->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
		wl_escan_print_sync_id(status, escan_result->sync_id,
			cfg->escan_info.cur_sync_id);
		if (wl_get_drv_status_all(cfg, FINDING_COMMON_CHANNEL)) {
			WL_INFO(("ACTION FRAME SCAN DONE\n"));
			wl_clr_p2p_status(cfg, SCANNING);
			wl_clr_drv_status(cfg, SCANNING, cfg->afx_hdl->dev);
			if (cfg->afx_hdl->peer_chan == WL_INVALID)
				complete(&cfg->act_frm_scan);
		} else if ((likely(cfg->scan_request)) || (cfg->sched_scan_running)) {
			cfg->bss_list = wl_escan_get_buf(cfg, TRUE);
			if (!scan_req_match(cfg)) {
				WL_TRACE_HW4(("SCAN ABORTED(UNEXPECTED): "
					"scanned AP count=%d\n",
					cfg->bss_list->count));
			}
			wl_inform_bss(cfg);
			wl_notify_escan_complete(cfg, ndev, true, false);
		}
		wl_escan_increment_sync_id(cfg, 2);
	}
exit:
	mutex_unlock(&cfg->usr_sync);
	return err;
}

static void wl_cfg80211_concurrent_roam(struct bcm_cfg80211 *cfg, int enable)
{
	u32 connected_cnt  = wl_get_drv_status_all(cfg, CONNECTED);
	struct net_info *iter, *next;
	int err;

	if (!cfg->roamoff_on_concurrent)
		return;
	if (enable && connected_cnt > 1) {
		for_each_ndev(cfg, iter, next) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			/* Save the current roam setting */
			if ((err = wldev_iovar_getint(iter->ndev, "roam_off",
				(s32 *)&iter->roam_off)) != BCME_OK) {
				WL_ERR(("%s:Failed to get current roam setting err %d\n",
					iter->ndev->name, err));
				continue;
			}
			if ((err = wldev_iovar_setint(iter->ndev, "roam_off", 1)) != BCME_OK) {
				WL_ERR((" %s:failed to set roam_off : %d\n",
					iter->ndev->name, err));
			}
		}
	}
	else if (!enable) {
<<<<<<< HEAD
		for_each_ndev(wl, iter, next) {
=======
		for_each_ndev(cfg, iter, next) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			if (iter->roam_off != WL_INVALID) {
				if ((err = wldev_iovar_setint(iter->ndev, "roam_off",
					iter->roam_off)) == BCME_OK)
					iter->roam_off = WL_INVALID;
				else {
					WL_ERR((" %s:failed to set roam_off : %d\n",
						iter->ndev->name, err));
				}
			}
		}
	}
	return;
}

<<<<<<< HEAD
static void wl_cfg80211_determine_vsdb_mode(struct wl_priv *wl)
{
	struct net_info *iter, *next;
	u32 chan = 0;
	u32 chanspec = 0;
	u32 prev_chan = 0;
	u32 connected_cnt  = wl_get_drv_status_all(wl, CONNECTED);
	wl->vsdb_mode = false;
=======
static void wl_cfg80211_determine_vsdb_mode(struct bcm_cfg80211 *cfg)
{
#ifdef CUSTOMER_HW4
	u32 connected_cnt  = wl_get_drv_status_all(cfg, CONNECTED);
	if (connected_cnt > 1) {
		cfg->vsdb_mode = true;
	} else {
		cfg->vsdb_mode = false;
	}
	return;
#else
	struct net_info *iter, *next;
	u32 ctl_chan = 0;
	u32 chanspec = 0;
	u32 pre_ctl_chan = 0;
	u32 connected_cnt  = wl_get_drv_status_all(cfg, CONNECTED);
	cfg->vsdb_mode = false;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	if (connected_cnt <= 1)  {
		return;
	}
<<<<<<< HEAD
	for_each_ndev(wl, iter, next) {
		chanspec = 0;
		chan = 0;
		if (wl_get_drv_status(wl, CONNECTED, iter->ndev)) {
			if (wldev_iovar_getint(iter->ndev, "chanspec",
				(s32 *)&chanspec) == BCME_OK) {
				chan = CHSPEC_CHANNEL(chanspec);
				if (CHSPEC_IS40(chanspec)) {
					if (CHSPEC_SB_UPPER(chanspec))
						chan += CH_10MHZ_APART;
					else
						chan -= CH_10MHZ_APART;
				}
				wl_update_prof(wl, iter->ndev, NULL,
					&chan, WL_PROF_CHAN);
			}
			if (!prev_chan && chan)
				prev_chan = chan;
			else if (prev_chan && (prev_chan != chan))
				wl->vsdb_mode = true;
		}
	}
	return;
}
static s32 wl_notifier_change_state(struct wl_priv *wl, struct net_info *_net_info,
=======
	for_each_ndev(cfg, iter, next) {
		chanspec = 0;
		ctl_chan = 0;
		if (wl_get_drv_status(cfg, CONNECTED, iter->ndev)) {
			if (wldev_iovar_getint(iter->ndev, "chanspec",
				(s32 *)&chanspec) == BCME_OK) {
				chanspec = wl_chspec_driver_to_host(chanspec);
				ctl_chan = wf_chspec_ctlchan(chanspec);
				wl_update_prof(cfg, iter->ndev, NULL,
					&ctl_chan, WL_PROF_CHAN);
			}
			if (!cfg->vsdb_mode) {
				if (!pre_ctl_chan && ctl_chan)
					pre_ctl_chan = ctl_chan;
				else if (pre_ctl_chan && (pre_ctl_chan != ctl_chan)) {
					cfg->vsdb_mode = true;
				}
			}
		}
	}
	WL_ERR(("%s concurrency is enabled\n", cfg->vsdb_mode ? "Multi Channel" : "Same Channel"));
	return;
#endif /* CUSTOMER_HW4 */
}

static s32 wl_notifier_change_state(struct bcm_cfg80211 *cfg, struct net_info *_net_info,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	enum wl_status state, bool set)
{
	s32 pm = PM_FAST;
	s32 err = BCME_OK;
<<<<<<< HEAD
	u32 chan = 0;
	struct net_info *iter, *next;
	struct net_device *primary_dev = wl_to_prmry_ndev(wl);
=======
	u32 mode;
	u32 chan = 0;
	struct net_info *iter, *next;
	struct net_device *primary_dev = bcmcfg_to_prmry_ndev(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	WL_DBG(("Enter state %d set %d _net_info->pm_restore %d iface %s\n",
		state, set, _net_info->pm_restore, _net_info->ndev->name));

	if (state != WL_STATUS_CONNECTED)
		return 0;
<<<<<<< HEAD

	if (set) {
		wl_cfg80211_concurrent_roam(wl, 1);

		if (wl_get_mode_by_netdev(wl, _net_info->ndev) == WL_MODE_AP) {
			pm = PM_OFF;
			WL_DBG(("%s:AP power save %s\n", _net_info->ndev->name,
				pm ? "enabled" : "disabled"));
			if ((err = wldev_ioctl(_net_info->ndev, WLC_SET_PM,
				&pm, sizeof(pm), true)) != 0) {
				if (err == -ENODEV)
					WL_DBG(("%s:net_device is not ready\n",
						_net_info->ndev->name));
				else
					WL_ERR(("%s:error (%d)\n", _net_info->ndev->name, err));
			}
			if (wl_add_remove_eventmsg(primary_dev, WLC_E_P2P_PROBREQ_MSG, false))
				WL_ERR((" failed to unset WLC_E_P2P_PROPREQ_MSG\n"));
			return 0;
		}
		wl_cfg80211_determine_vsdb_mode(wl);
		pm = PM_OFF;
		for_each_ndev(wl, iter, next) {
			if ((!wl->vsdb_mode) && (iter->ndev != _net_info->ndev)) {
				/* Do not touch the other interfaces power save
				 * if we are not in vsdb mode
				 */
				continue;
			}
			/* Save the current power mode */
			iter->pm_restore = true;
			err = wldev_ioctl(iter->ndev, WLC_GET_PM, &iter->pm,
				sizeof(iter->pm), false);
			WL_DBG(("%s:power save %s\n", iter->ndev->name,
				iter->pm ? "enabled" : "disabled"));
			if ((err = wldev_ioctl(iter->ndev, WLC_SET_PM, &pm,
				sizeof(pm), true)) != 0) {
				if (err == -ENODEV)
					WL_DBG(("%s:netdev not ready\n", iter->ndev->name));
				else
					WL_ERR(("%s:error (%d)\n", iter->ndev->name, err));
				iter->ndev->ieee80211_ptr->ps = pm ? true: false;
			}
		}
=======
	mode = wl_get_mode_by_netdev(cfg, _net_info->ndev);
	if (set) {
		wl_cfg80211_concurrent_roam(cfg, 1);

		if (mode == WL_MODE_AP) {

			if (wl_add_remove_eventmsg(primary_dev, WLC_E_P2P_PROBREQ_MSG, false))
				WL_ERR((" failed to unset WLC_E_P2P_PROPREQ_MSG\n"));
		}
		wl_cfg80211_determine_vsdb_mode(cfg);
		if (cfg->vsdb_mode || _net_info->pm_block) {
			/* Delete pm_enable_work */
			wl_add_remove_pm_enable_work(cfg, FALSE, WL_HANDLER_MAINTAIN);
			/* save PM_FAST in _net_info to restore this
			 * if _net_info->pm_block is false
			 */
			if (!_net_info->pm_block && (mode == WL_MODE_BSS)) {
				_net_info->pm = PM_FAST;
				_net_info->pm_restore = true;
			}
			pm = PM_OFF;
			for_each_ndev(cfg, iter, next) {
				if (iter->pm_restore)
					continue;
				/* Save the current power mode */
				err = wldev_ioctl(iter->ndev, WLC_GET_PM, &iter->pm,
					sizeof(iter->pm), false);
				WL_DBG(("%s:power save %s\n", iter->ndev->name,
					iter->pm ? "enabled" : "disabled"));
				if (!err && iter->pm) {
					iter->pm_restore = true;
				}

			}
			for_each_ndev(cfg, iter, next) {
				if ((err = wldev_ioctl(iter->ndev, WLC_SET_PM, &pm,
					sizeof(pm), true)) != 0) {
					if (err == -ENODEV)
						WL_DBG(("%s:netdev not ready\n", iter->ndev->name));
					else
						WL_ERR(("%s:error (%d)\n", iter->ndev->name, err));
					wl_cfg80211_update_power_mode(iter->ndev);
				}
			}
		} else {
			/* add PM Enable timer to go to power save mode
			 * if supplicant control pm mode, it will be cleared or
			 * updated by wl_cfg80211_set_power_mgmt() if not - for static IP & HW4 P2P,
			 * PM will be configured when timer expired
			 */

			/*
			 * before calling pm_enable_timer, we need to set PM -1 for all ndev
			 */
			pm = PM_OFF;

			for_each_ndev(cfg, iter, next) {
				if ((err = wldev_ioctl(iter->ndev, WLC_SET_PM, &pm,
					sizeof(pm), true)) != 0) {
					if (err == -ENODEV)
						WL_DBG(("%s:netdev not ready\n", iter->ndev->name));
					else
						WL_ERR(("%s:error (%d)\n", iter->ndev->name, err));
				}
			}

			if (cfg->pm_enable_work_on) {
				wl_add_remove_pm_enable_work(cfg, FALSE, WL_HANDLER_DEL);
			}

			cfg->pm_enable_work_on = true;
			wl_add_remove_pm_enable_work(cfg, TRUE, WL_HANDLER_NOTUSE);
		}
#if defined(CUSTOMER_HW4) && defined(WLTDLS)
		if (cfg->vsdb_mode) {
			err = wldev_iovar_setint(primary_dev, "tdls_enable", 0);
		}
#endif /* defined(CUSTOMER_HW4) && defined(WLTDLS) */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
	 else { /* clear */
		chan = 0;
		/* clear chan information when the net device is disconnected */
<<<<<<< HEAD
		wl_update_prof(wl, _net_info->ndev, NULL, &chan, WL_PROF_CHAN);
		wl_cfg80211_determine_vsdb_mode(wl);
		for_each_ndev(wl, iter, next) {
			if (iter->pm_restore) {
=======
		wl_update_prof(cfg, _net_info->ndev, NULL, &chan, WL_PROF_CHAN);
		wl_cfg80211_determine_vsdb_mode(cfg);
		for_each_ndev(cfg, iter, next) {
			if (iter->pm_restore && iter->pm) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				WL_DBG(("%s:restoring power save %s\n",
					iter->ndev->name, (iter->pm ? "enabled" : "disabled")));
				err = wldev_ioctl(iter->ndev,
					WLC_SET_PM, &iter->pm, sizeof(iter->pm), true);
				if (unlikely(err)) {
					if (err == -ENODEV)
						WL_DBG(("%s:netdev not ready\n", iter->ndev->name));
					else
						WL_ERR(("%s:error(%d)\n", iter->ndev->name, err));
					break;
				}
				iter->pm_restore = 0;
<<<<<<< HEAD
			}
		}
		wl_cfg80211_concurrent_roam(wl, 0);
	}
	return err;
}
static s32 wl_init_scan(struct wl_priv *wl)
{
	struct wl_iscan_ctrl *iscan = wl_to_iscan(wl);
	int err = 0;

	if (wl->iscan_on) {
		iscan->dev = wl_to_prmry_ndev(wl);
		iscan->state = WL_ISCAN_STATE_IDLE;
		wl_init_iscan_handler(iscan);
		iscan->timer_ms = WL_ISCAN_TIMER_INTERVAL_MS;
		init_timer(&iscan->timer);
		iscan->timer.data = (unsigned long) iscan;
		iscan->timer.function = wl_iscan_timer;
		sema_init(&iscan->sync, 0);
		iscan->tsk = kthread_run(wl_iscan_thread, iscan, "wl_iscan");
		if (IS_ERR(iscan->tsk)) {
			WL_ERR(("Could not create iscan thread\n"));
			iscan->tsk = NULL;
			return -ENOMEM;
		}
		iscan->data = wl;
	} else if (wl->escan_on) {
		wl->evt_handler[WLC_E_ESCAN_RESULT] = wl_escan_handler;
		wl->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
	}
	/* Init scan_timeout timer */
	init_timer(&wl->scan_timeout);
	wl->scan_timeout.data = (unsigned long) wl;
	wl->scan_timeout.function = wl_scan_timeout;
=======
				wl_cfg80211_update_power_mode(iter->ndev);
			}
		}
		wl_cfg80211_concurrent_roam(cfg, 0);
#if defined(CUSTOMER_HW4) && defined(WLTDLS)
		if (!cfg->vsdb_mode) {
			err = wldev_iovar_setint(primary_dev, "tdls_enable", 1);
		}
#endif /* defined(CUSTOMER_HW4) && defined(WLTDLS) */
	}
	return err;
}
static s32 wl_init_scan(struct bcm_cfg80211 *cfg)
{
	int err = 0;

	cfg->evt_handler[WLC_E_ESCAN_RESULT] = wl_escan_handler;
	cfg->escan_info.escan_state = WL_ESCAN_STATE_IDLE;
	wl_escan_init_sync_id(cfg);

	/* Init scan_timeout timer */
	init_timer(&cfg->scan_timeout);
	cfg->scan_timeout.data = (unsigned long) cfg;
	cfg->scan_timeout.function = wl_scan_timeout;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
static s32 wl_init_priv(struct wl_priv *wl)
{
	struct wiphy *wiphy = wl_to_wiphy(wl);
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	s32 err = 0;

	wl->scan_request = NULL;
	wl->pwr_save = !!(wiphy->flags & WIPHY_FLAG_PS_ON_BY_DEFAULT);
	wl->iscan_on = false;
	wl->escan_on = true;
	wl->roam_on = false;
	wl->iscan_kickstart = false;
	wl->active_scan = true;
	wl->rf_blocked = false;
	wl->vsdb_mode = false;
	wl->wlfc_on = false;
	wl->roamoff_on_concurrent = true;
	/* register interested state */
	set_bit(WL_STATUS_CONNECTED, &wl->interrested_state);
	spin_lock_init(&wl->cfgdrv_lock);
	mutex_init(&wl->ioctl_buf_sync);
	init_waitqueue_head(&wl->netif_change_event);
	init_completion(&wl->send_af_done);
	init_completion(&wl->iface_disable);
	wl_init_eq(wl);
	err = wl_init_priv_mem(wl);
	if (err)
		return err;
	if (wl_create_event_handler(wl))
		return -ENOMEM;
	wl_init_event_handler(wl);
	mutex_init(&wl->usr_sync);
	mutex_init(&wl->event_sync);
	err = wl_init_scan(wl);
	if (err)
		return err;
	wl_init_conf(wl->conf);
	wl_init_prof(wl, ndev);
	wl_link_down(wl);
	DNGL_FUNC(dhd_cfg80211_init, (wl));
=======
static s32 wl_init_priv(struct bcm_cfg80211 *cfg)
{
	struct wiphy *wiphy = bcmcfg_to_wiphy(cfg);
	struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);
	s32 err = 0;

	cfg->scan_request = NULL;
	cfg->pwr_save = !!(wiphy->flags & WIPHY_FLAG_PS_ON_BY_DEFAULT);
	cfg->roam_on = false;
	cfg->active_scan = true;
	cfg->rf_blocked = false;
	cfg->vsdb_mode = false;
	cfg->wlfc_on = false;
	cfg->roamoff_on_concurrent = true;
	cfg->disable_roam_event = false;
	/* register interested state */
	set_bit(WL_STATUS_CONNECTED, &cfg->interrested_state);
	spin_lock_init(&cfg->cfgdrv_lock);
	mutex_init(&cfg->ioctl_buf_sync);
	init_waitqueue_head(&cfg->netif_change_event);
	init_completion(&cfg->send_af_done);
	init_completion(&cfg->iface_disable);
	wl_init_eq(cfg);
	err = wl_init_priv_mem(cfg);
	if (err)
		return err;
	if (wl_create_event_handler(cfg))
		return -ENOMEM;
	wl_init_event_handler(cfg);
	mutex_init(&cfg->usr_sync);
	mutex_init(&cfg->event_sync);
	err = wl_init_scan(cfg);
	if (err)
		return err;
	wl_init_conf(cfg->conf);
	wl_init_prof(cfg, ndev);
	wl_link_down(cfg);
	DNGL_FUNC(dhd_cfg80211_init, (cfg));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
static void wl_deinit_priv(struct wl_priv *wl)
{
	DNGL_FUNC(dhd_cfg80211_deinit, (wl));
	wl_destroy_event_handler(wl);
	wl_flush_eq(wl);
	wl_link_down(wl);
	del_timer_sync(&wl->scan_timeout);
	wl_term_iscan(wl);
	wl_deinit_priv_mem(wl);
	unregister_netdevice_notifier(&wl_cfg80211_netdev_notifier);
}

#if defined(WLP2P) && defined(WL_ENABLE_P2P_IF)
static s32 wl_cfg80211_attach_p2p(void)
{
	struct wl_priv *wl = wlcfg_drv_priv;

	WL_TRACE(("Enter \n"));

	if (wl_cfgp2p_register_ndev(wl) < 0) {
		WL_ERR(("%s: P2P attach failed. \n", __func__));
=======
static void wl_deinit_priv(struct bcm_cfg80211 *cfg)
{
	DNGL_FUNC(dhd_cfg80211_deinit, (cfg));
	wl_destroy_event_handler(cfg);
	wl_flush_eq(cfg);
	wl_link_down(cfg);
	del_timer_sync(&cfg->scan_timeout);
	wl_deinit_priv_mem(cfg);
	if (wl_cfg80211_netdev_notifier_registered) {
		wl_cfg80211_netdev_notifier_registered = FALSE;
		unregister_netdevice_notifier(&wl_cfg80211_netdev_notifier);
	}
}

#if defined(WL_ENABLE_P2P_IF) || defined(WL_NEWCFG_PRIVCMD_SUPPORT)
static s32 wl_cfg80211_attach_p2p(void)
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;

	WL_TRACE(("Enter \n"));

	if (wl_cfgp2p_register_ndev(cfg) < 0) {
		WL_ERR(("P2P attach failed. \n"));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		return -ENODEV;
	}

	return 0;
}

static s32  wl_cfg80211_detach_p2p(void)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
	struct wireless_dev *wdev = wl->p2p_wdev;

	WL_DBG(("Enter \n"));
	if (!wdev || !wl) {
		WL_ERR(("Invalid Ptr\n"));
		return -EINVAL;
	}

	wl_cfgp2p_unregister_ndev(wl);

	wl->p2p_wdev = NULL;
	wl->p2p_net = NULL;
	WL_DBG(("Freeing 0x%08x \n", (unsigned int)wdev));
	kfree(wdev);

	return 0;
}
#endif /* defined(WLP2P) && defined(WL_ENABLE_P2P_IF) */

s32 wl_cfg80211_attach_post(struct net_device *ndev)
{
	struct wl_priv * wl = NULL;
	s32 err = 0;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	struct wireless_dev *wdev;

	WL_DBG(("Enter \n"));
	if (!cfg) {
		WL_ERR(("Invalid Ptr\n"));
		return -EINVAL;
	} else
		wdev = cfg->p2p_wdev;

#ifndef WL_NEWCFG_PRIVCMD_SUPPORT
	if (!wdev) {
		WL_ERR(("Invalid Ptr\n"));
		return -EINVAL;
	}
#endif /* WL_NEWCFG_PRIVCMD_SUPPORT */

	wl_cfgp2p_unregister_ndev(cfg);

	cfg->p2p_wdev = NULL;
	cfg->p2p_net = NULL;
#ifndef WL_NEWCFG_PRIVCMD_SUPPORT
	WL_DBG(("Freeing 0x%08x \n", (unsigned int)wdev));
	kfree(wdev);
#endif /* WL_NEWCFG_PRIVCMD_SUPPORT */

	return 0;
}
#endif /* WL_ENABLE_P2P_IF || WL_NEWCFG_PRIVCMD_SUPPORT */

s32 wl_cfg80211_attach_post(struct net_device *ndev)
{
	struct bcm_cfg80211 * cfg = NULL;
	s32 err = 0;
	s32 ret = 0;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	WL_TRACE(("In\n"));
	if (unlikely(!ndev)) {
		WL_ERR(("ndev is invaild\n"));
		return -ENODEV;
	}
<<<<<<< HEAD
	wl = wlcfg_drv_priv;
	if (unlikely(!wl)) {
		WL_ERR(("wl is invaild\n"));
		return -EINVAL;
	}
	if (!wl_get_drv_status(wl, READY, ndev)) {
			if (wl->wdev &&
				wl_cfgp2p_supported(wl, ndev)) {
#if !defined(WL_ENABLE_P2P_IF)
				wl->wdev->wiphy->interface_modes |=
					(BIT(NL80211_IFTYPE_P2P_CLIENT)|
					BIT(NL80211_IFTYPE_P2P_GO));
#endif
				if ((err = wl_cfgp2p_init_priv(wl)) != 0)
					goto fail;

#if defined(WLP2P) && defined(WL_ENABLE_P2P_IF)
				if (wl->p2p_net) {
					/* Update MAC addr for p2p0 interface here. */
					memcpy(wl->p2p_net->dev_addr, ndev->dev_addr, ETH_ALEN);
					wl->p2p_net->dev_addr[0] |= 0x02;
					WL_ERR(("%s: p2p_dev_addr="MACDBG "\n",
						wl->p2p_net->name,
						MAC2STRDBG(wl->p2p_net->dev_addr)));
=======
	cfg = g_bcm_cfg;
	if (unlikely(!cfg)) {
		WL_ERR(("cfg is invaild\n"));
		return -EINVAL;
	}
	if (!wl_get_drv_status(cfg, READY, ndev)) {
		if (cfg->wdev) {
			ret = wl_cfgp2p_supported(cfg, ndev);
			if (ret > 0) {
#if !defined(WL_ENABLE_P2P_IF)
				cfg->wdev->wiphy->interface_modes |=
					(BIT(NL80211_IFTYPE_P2P_CLIENT)|
					BIT(NL80211_IFTYPE_P2P_GO));
#endif /* !WL_ENABLE_P2P_IF */
				if ((err = wl_cfgp2p_init_priv(cfg)) != 0)
					goto fail;

#if defined(WL_ENABLE_P2P_IF)
				if (cfg->p2p_net) {
					/* Update MAC addr for p2p0 interface here. */
					memcpy(cfg->p2p_net->dev_addr, ndev->dev_addr, ETH_ALEN);
					cfg->p2p_net->dev_addr[0] |= 0x02;
					WL_ERR(("%s: p2p_dev_addr="MACDBG "\n",
						cfg->p2p_net->name,
						MAC2STRDBG(cfg->p2p_net->dev_addr)));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				} else {
					WL_ERR(("p2p_net not yet populated."
					" Couldn't update the MAC Address for p2p0 \n"));
					return -ENODEV;
				}
<<<<<<< HEAD
#endif /* defined(WLP2P) && (WL_ENABLE_P2P_IF) */

				wl->p2p_supported = true;
			}
	}
	wl_set_drv_status(wl, READY, ndev);
=======
#endif /* WL_ENABLE_P2P_IF */
				cfg->p2p_supported = true;
			} else if (ret == 0) {
				if ((err = wl_cfgp2p_init_priv(cfg)) != 0)
					goto fail;
			} else {
				/* SDIO bus timeout */
				err = -ENODEV;
				goto fail;
			}
		}
	}
	wl_set_drv_status(cfg, READY, ndev);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
fail:
	return err;
}

<<<<<<< HEAD
s32 wl_cfg80211_attach(struct net_device *ndev, void *data)
{
	struct wireless_dev *wdev;
	struct wl_priv *wl;
=======
s32 wl_cfg80211_attach(struct net_device *ndev, void *context)
{
	struct wireless_dev *wdev;
	struct bcm_cfg80211 *cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = 0;
	struct device *dev;

	WL_TRACE(("In\n"));
	if (!ndev) {
		WL_ERR(("ndev is invaild\n"));
		return -ENODEV;
	}
	WL_DBG(("func %p\n", wl_cfg80211_get_parent_dev()));
	dev = wl_cfg80211_get_parent_dev();

	wdev = kzalloc(sizeof(*wdev), GFP_KERNEL);
	if (unlikely(!wdev)) {
		WL_ERR(("Could not allocate wireless device\n"));
		return -ENOMEM;
	}
<<<<<<< HEAD
	err = wl_setup_wiphy(wdev, dev);
=======
	err = wl_setup_wiphy(wdev, dev, context);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (unlikely(err)) {
		kfree(wdev);
		return -ENOMEM;
	}
	wdev->iftype = wl_mode_to_nl80211_iftype(WL_MODE_BSS);
<<<<<<< HEAD
	wl = (struct wl_priv *)wiphy_priv(wdev->wiphy);
	wl->wdev = wdev;
	wl->pub = data;
	INIT_LIST_HEAD(&wl->net_list);
	ndev->ieee80211_ptr = wdev;
	SET_NETDEV_DEV(ndev, wiphy_dev(wdev->wiphy));
	wdev->netdev = ndev;
	wl->state_notifier = wl_notifier_change_state;
	err = wl_alloc_netinfo(wl, ndev, wdev, WL_MODE_BSS, PM_ENABLE);
=======
	cfg = (struct bcm_cfg80211 *)wiphy_priv(wdev->wiphy);
	cfg->wdev = wdev;
	cfg->pub = context;
	INIT_LIST_HEAD(&cfg->net_list);
	ndev->ieee80211_ptr = wdev;
	SET_NETDEV_DEV(ndev, wiphy_dev(wdev->wiphy));
	wdev->netdev = ndev;
	cfg->state_notifier = wl_notifier_change_state;
	err = wl_alloc_netinfo(cfg, ndev, wdev, WL_MODE_BSS, PM_ENABLE);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (err) {
		WL_ERR(("Failed to alloc net_info (%d)\n", err));
		goto cfg80211_attach_out;
	}
<<<<<<< HEAD
	err = wl_init_priv(wl);
=======
	err = wl_init_priv(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (err) {
		WL_ERR(("Failed to init iwm_priv (%d)\n", err));
		goto cfg80211_attach_out;
	}

<<<<<<< HEAD
	err = wl_setup_rfkill(wl, TRUE);
=======
	err = wl_setup_rfkill(cfg, TRUE);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (err) {
		WL_ERR(("Failed to setup rfkill %d\n", err));
		goto cfg80211_attach_out;
	}
<<<<<<< HEAD
	err = register_netdevice_notifier(&wl_cfg80211_netdev_notifier);
	if (err) {
		WL_ERR(("Failed to register notifierl %d\n", err));
		goto cfg80211_attach_out;
	}
#if defined(COEX_DHCP)
	if (wl_cfg80211_btcoex_init(wl))
		goto cfg80211_attach_out;
#endif 

	wlcfg_drv_priv = wl;

#if defined(WLP2P) && defined(WL_ENABLE_P2P_IF)
	err = wl_cfg80211_attach_p2p();
	if (err)
		goto cfg80211_attach_out;
#endif
=======
#ifdef DEBUGFS_CFG80211
	err = wl_setup_debugfs(cfg);
	if (err) {
		WL_ERR(("Failed to setup debugfs %d\n", err));
		goto cfg80211_attach_out;
	}
#endif
	if (!wl_cfg80211_netdev_notifier_registered) {
		wl_cfg80211_netdev_notifier_registered = TRUE;
		err = register_netdevice_notifier(&wl_cfg80211_netdev_notifier);
		if (err) {
			wl_cfg80211_netdev_notifier_registered = FALSE;
			WL_ERR(("Failed to register notifierl %d\n", err));
			goto cfg80211_attach_out;
		}
	}
#if defined(COEX_DHCP)
	cfg->btcoex_info = wl_cfg80211_btcoex_init(cfg->wdev->netdev);
	if (!cfg->btcoex_info)
		goto cfg80211_attach_out;
#endif 

	g_bcm_cfg = cfg;

#if defined(WL_ENABLE_P2P_IF) || defined(WL_NEWCFG_PRIVCMD_SUPPORT)
	err = wl_cfg80211_attach_p2p();
	if (err)
		goto cfg80211_attach_out;
#endif /* WL_ENABLE_P2P_IF || WL_NEWCFG_PRIVCMD_SUPPORT */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;

cfg80211_attach_out:
<<<<<<< HEAD
	err = wl_setup_rfkill(wl, FALSE);
	wl_free_wdev(wl);
=======
	wl_setup_rfkill(cfg, FALSE);
	wl_free_wdev(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}

void wl_cfg80211_detach(void *para)
{
<<<<<<< HEAD
	struct wl_priv *wl;

	(void)para;
	wl = wlcfg_drv_priv;

	WL_TRACE(("In\n"));

#if defined(COEX_DHCP)
	wl_cfg80211_btcoex_deinit(wl);
#endif 

	wl_setup_rfkill(wl, FALSE);
	if (wl->p2p_supported) {
		if (timer_pending(&wl->p2p->listen_timer))
			del_timer_sync(&wl->p2p->listen_timer);
		wl_cfgp2p_deinit_priv(wl);
	}

#if defined(WLP2P) && defined(WL_ENABLE_P2P_IF)
	wl_cfg80211_detach_p2p();
#endif
	wl_deinit_priv(wl);
	wlcfg_drv_priv = NULL;
	wl_cfg80211_clear_parent_dev();
	wl_free_wdev(wl);
	 /* PLEASE do NOT call any function after wl_free_wdev, the driver's private structure "wl",
	  * which is the private part of wiphy, has been freed in wl_free_wdev !!!!!!!!!!!
	  */
}

static void wl_wakeup_event(struct wl_priv *wl)
{
	if (wl->event_tsk.thr_pid >= 0) {
		DHD_OS_WAKE_LOCK(wl->pub);
		up(&wl->event_tsk.sema);
	}
}

=======
	struct bcm_cfg80211 *cfg;

	(void)para;
	cfg = g_bcm_cfg;

	WL_TRACE(("In\n"));

	wl_add_remove_pm_enable_work(cfg, FALSE, WL_HANDLER_DEL);

#if defined(COEX_DHCP)
	wl_cfg80211_btcoex_deinit();
	cfg->btcoex_info = NULL;
#endif 

	wl_setup_rfkill(cfg, FALSE);
#ifdef DEBUGFS_CFG80211
	wl_free_debugfs(cfg);
#endif
	if (cfg->p2p_supported) {
		if (timer_pending(&cfg->p2p->listen_timer))
			del_timer_sync(&cfg->p2p->listen_timer);
		wl_cfgp2p_deinit_priv(cfg);
	}

	if (timer_pending(&cfg->scan_timeout))
		del_timer_sync(&cfg->scan_timeout);

#if defined(WL_CFG80211_P2P_DEV_IF)
	wl_cfgp2p_del_p2p_disc_if(cfg->p2p_wdev, cfg);
#endif /* WL_CFG80211_P2P_DEV_IF  */
#if defined(WL_ENABLE_P2P_IF) || defined(WL_NEWCFG_PRIVCMD_SUPPORT)
	wl_cfg80211_detach_p2p();
#endif /* WL_ENABLE_P2P_IF || WL_NEWCFG_PRIVCMD_SUPPORT */

	wl_cfg80211_ibss_vsie_free(cfg);
	wl_deinit_priv(cfg);
	g_bcm_cfg = NULL;
	wl_cfg80211_clear_parent_dev();
	wl_free_wdev(cfg);
	/* PLEASE do NOT call any function after wl_free_wdev, the driver's private
	 * structure "cfg", which is the private part of wiphy, has been freed in
	 * wl_free_wdev !!!!!!!!!!!
	 */
}

static void wl_wakeup_event(struct bcm_cfg80211 *cfg)
{
	if (cfg->event_tsk.thr_pid >= 0) {
		DHD_OS_WAKE_LOCK(cfg->pub);
		up(&cfg->event_tsk.sema);
	}
}

#if (defined(WL_CFG80211_P2P_DEV_IF) || defined(WL_ENABLE_P2P_IF))
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static int wl_is_p2p_event(struct wl_event_q *e)
{
	switch (e->etype) {
	/* We have to seperate out the P2P events received
	 * on primary interface so that it can be send up
	 * via p2p0 interface.
	*/
	case WLC_E_P2P_PROBREQ_MSG:
	case WLC_E_P2P_DISC_LISTEN_COMPLETE:
	case WLC_E_ACTION_FRAME_RX:
	case WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE:
	case WLC_E_ACTION_FRAME_COMPLETE:

		if (e->emsg.ifidx != 0) {
<<<<<<< HEAD
			WL_TRACE(("P2P Event on Virtual I/F (ifidx:%d) \n",
			e->emsg.ifidx));
=======
			WL_TRACE(("P2P event(%d) on virtual interface(ifidx:%d)\n",
				e->etype, e->emsg.ifidx));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			/* We are only bothered about the P2P events received
			 * on primary interface. For rest of them return false
			 * so that it is sent over the interface corresponding
			 * to the ifidx.
			 */
			return FALSE;
		} else {
<<<<<<< HEAD
			WL_TRACE(("P2P Event on Primary I/F (ifidx:%d)."
				" Sent it to p2p0 \n", e->emsg.ifidx));
=======
			WL_TRACE(("P2P event(%d) on interface(ifidx:%d)\n",
				e->etype, e->emsg.ifidx));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			return TRUE;
		}
		break;

	default:
<<<<<<< HEAD
		WL_TRACE(("NON-P2P Event %d on ifidx (ifidx:%d) \n",
=======
		WL_TRACE(("NON-P2P event(%d) on interface(ifidx:%d)\n",
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			e->etype, e->emsg.ifidx));
		return FALSE;
	}
}
<<<<<<< HEAD

static s32 wl_event_handler(void *data)
{
	struct net_device *netdev;
	struct wl_priv *wl = NULL;
	struct wl_event_q *e;
	tsk_ctl_t *tsk = (tsk_ctl_t *)data;

	wl = (struct wl_priv *)tsk->parent;
#ifndef USE_KTHREAD_API
	DAEMONIZE("dhd_cfg80211_event");
	complete(&tsk->completed);
#else
	WL_ERR(("tsk Enter, tsk = 0x%08x\n", (unsigned int)tsk));
#endif
=======
#endif /* BCMDONGLEHOST && (WL_CFG80211_P2P_DEV_IF || WL_ENABLE_P2P_IF) */

static s32 wl_event_handler(void *data)
{
	struct bcm_cfg80211 *cfg = NULL;
	struct wl_event_q *e;
	tsk_ctl_t *tsk = (tsk_ctl_t *)data;
	bcm_struct_cfgdev *cfgdev = NULL;

	cfg = (struct bcm_cfg80211 *)tsk->parent;

	WL_ERR(("tsk Enter, tsk = 0x%p\n", tsk));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	while (down_interruptible (&tsk->sema) == 0) {
		SMP_RD_BARRIER_DEPENDS();
		if (tsk->terminated)
			break;
<<<<<<< HEAD
		while ((e = wl_deq_event(wl))) {
=======
		while ((e = wl_deq_event(cfg))) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_DBG(("event type (%d), if idx: %d\n", e->etype, e->emsg.ifidx));
			/* All P2P device address related events comes on primary interface since
			 * there is no corresponding bsscfg for P2P interface. Map it to p2p0
			 * interface.
			 */
<<<<<<< HEAD
			if ((wl_is_p2p_event(e) == TRUE) && (wl->p2p_net)) {
				netdev = wl->p2p_net;
			} else {
				netdev = dhd_idx2net((struct dhd_pub *)(wl->pub), e->emsg.ifidx);
			}
			if (!netdev)
				netdev = wl_to_prmry_ndev(wl);
			if (e->etype < WLC_E_LAST && wl->evt_handler[e->etype]) {
				wl->evt_handler[e->etype] (wl, netdev, &e->emsg, e->edata);
=======
#if defined(WL_CFG80211_P2P_DEV_IF)
			if ((wl_is_p2p_event(e) == TRUE) && (cfg->p2p_wdev)) {
				cfgdev = bcmcfg_to_p2p_wdev(cfg);
			} else {
				struct net_device *ndev = NULL;

				ndev = dhd_idx2net((struct dhd_pub *)(cfg->pub), e->emsg.ifidx);
				if (ndev)
					cfgdev = ndev_to_wdev(ndev);
			}
#elif defined(WL_ENABLE_P2P_IF)
			if ((wl_is_p2p_event(e) == TRUE) && (cfg->p2p_net)) {
				cfgdev = cfg->p2p_net;
			} else {
				cfgdev = dhd_idx2net((struct dhd_pub *)(cfg->pub),
					e->emsg.ifidx);
			}
#endif /* WL_CFG80211_P2P_DEV_IF */

			if (!cfgdev) {
#if defined(WL_CFG80211_P2P_DEV_IF)
				cfgdev = bcmcfg_to_prmry_wdev(cfg);
#elif defined(WL_ENABLE_P2P_IF)
				cfgdev = bcmcfg_to_prmry_ndev(cfg);
#endif /* WL_CFG80211_P2P_DEV_IF */
			}
			if (e->etype < WLC_E_LAST && cfg->evt_handler[e->etype]) {
				cfg->evt_handler[e->etype] (cfg, cfgdev, &e->emsg, e->edata);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			} else {
				WL_DBG(("Unknown Event (%d): ignoring\n", e->etype));
			}
			wl_put_event(e);
		}
<<<<<<< HEAD
		DHD_OS_WAKE_UNLOCK(wl->pub);
	}
	WL_ERR(("%s was terminated\n", __func__));
=======
		DHD_OS_WAKE_UNLOCK(cfg->pub);
	}
	WL_ERR(("was terminated\n"));
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	complete_and_exit(&tsk->completed, 0);
	return 0;
}

void
wl_cfg80211_event(struct net_device *ndev, const wl_event_msg_t * e, void *data)
{
	u32 event_type = ntoh32(e->event_type);
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#if (WL_DBG_LEVEL > 0)
	s8 *estr = (event_type <= sizeof(wl_dbg_estr) / WL_DBG_ESTR_MAX - 1) ?
	    wl_dbg_estr[event_type] : (s8 *) "Unknown";
	WL_DBG(("event_type (%d):" "WLC_E_" "%s\n", event_type, estr));
#endif /* (WL_DBG_LEVEL > 0) */

<<<<<<< HEAD
=======
	if (wl_get_p2p_status(cfg, IF_CHANGING) || wl_get_p2p_status(cfg, IF_ADDING)) {
		WL_ERR(("during IF change, ignore event %d\n", event_type));
		return;
	}

	if (ndev != bcmcfg_to_prmry_ndev(cfg) && cfg->p2p_supported) {
		if (ndev != wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_CONNECTION) &&
#if defined(WL_ENABLE_P2P_IF)
			(ndev != (cfg->p2p_net ? cfg->p2p_net :
			wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_DEVICE))) &&
#else
			(ndev != wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_DEVICE)) &&
#endif /* WL_ENABLE_P2P_IF */
			TRUE) {
			WL_ERR(("ignore event %d, not interested\n", event_type));
			return;
		}
	}

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (event_type == WLC_E_PFN_NET_FOUND) {
		WL_DBG((" PNOEVENT: PNO_NET_FOUND\n"));
	}
	else if (event_type == WLC_E_PFN_NET_LOST) {
		WL_DBG((" PNOEVENT: PNO_NET_LOST\n"));
	}

<<<<<<< HEAD
	if (likely(!wl_enq_event(wl, ndev, event_type, e, data)))
		wl_wakeup_event(wl);
}

static void wl_init_eq(struct wl_priv *wl)
{
	wl_init_eq_lock(wl);
	INIT_LIST_HEAD(&wl->eq_list);
}

static void wl_flush_eq(struct wl_priv *wl)
=======
	if (likely(!wl_enq_event(cfg, ndev, event_type, e, data)))
		wl_wakeup_event(cfg);
}

static void wl_init_eq(struct bcm_cfg80211 *cfg)
{
	wl_init_eq_lock(cfg);
	INIT_LIST_HEAD(&cfg->eq_list);
}

static void wl_flush_eq(struct bcm_cfg80211 *cfg)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	struct wl_event_q *e;
	unsigned long flags;

<<<<<<< HEAD
	flags = wl_lock_eq(wl);
	while (!list_empty(&wl->eq_list)) {
		e = list_first_entry(&wl->eq_list, struct wl_event_q, eq_list);
		list_del(&e->eq_list);
		kfree(e);
	}
	wl_unlock_eq(wl, flags);
=======
	flags = wl_lock_eq(cfg);
	while (!list_empty(&cfg->eq_list)) {
		e = list_first_entry(&cfg->eq_list, struct wl_event_q, eq_list);
		list_del(&e->eq_list);
		kfree(e);
	}
	wl_unlock_eq(cfg, flags);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

/*
* retrieve first queued event from head
*/

<<<<<<< HEAD
static struct wl_event_q *wl_deq_event(struct wl_priv *wl)
=======
static struct wl_event_q *wl_deq_event(struct bcm_cfg80211 *cfg)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	struct wl_event_q *e = NULL;
	unsigned long flags;

<<<<<<< HEAD
	flags = wl_lock_eq(wl);
	if (likely(!list_empty(&wl->eq_list))) {
		e = list_first_entry(&wl->eq_list, struct wl_event_q, eq_list);
		list_del(&e->eq_list);
	}
	wl_unlock_eq(wl, flags);
=======
	flags = wl_lock_eq(cfg);
	if (likely(!list_empty(&cfg->eq_list))) {
		e = list_first_entry(&cfg->eq_list, struct wl_event_q, eq_list);
		list_del(&e->eq_list);
	}
	wl_unlock_eq(cfg, flags);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return e;
}

/*
 * push event to tail of the queue
 */

static s32
<<<<<<< HEAD
wl_enq_event(struct wl_priv *wl, struct net_device *ndev, u32 event, const wl_event_msg_t *msg,
	void *data)
=======
wl_enq_event(struct bcm_cfg80211 *cfg, struct net_device *ndev, u32 event,
	const wl_event_msg_t *msg, void *data)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	struct wl_event_q *e;
	s32 err = 0;
	uint32 evtq_size;
	uint32 data_len;
	unsigned long flags;
	gfp_t aflags;

	data_len = 0;
	if (data)
		data_len = ntoh32(msg->datalen);
	evtq_size = sizeof(struct wl_event_q) + data_len;
	aflags = (in_atomic()) ? GFP_ATOMIC : GFP_KERNEL;
	e = kzalloc(evtq_size, aflags);
	if (unlikely(!e)) {
		WL_ERR(("event alloc failed\n"));
		return -ENOMEM;
	}
	e->etype = event;
	memcpy(&e->emsg, msg, sizeof(wl_event_msg_t));
	if (data)
		memcpy(e->edata, data, data_len);
<<<<<<< HEAD
	flags = wl_lock_eq(wl);
	list_add_tail(&e->eq_list, &wl->eq_list);
	wl_unlock_eq(wl, flags);
=======
	flags = wl_lock_eq(cfg);
	list_add_tail(&e->eq_list, &cfg->eq_list);
	wl_unlock_eq(cfg, flags);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

static void wl_put_event(struct wl_event_q *e)
{
	kfree(e);
}

<<<<<<< HEAD
static s32 wl_config_ifmode(struct wl_priv *wl, struct net_device *ndev, s32 iftype)
=======
static s32 wl_config_ifmode(struct bcm_cfg80211 *cfg, struct net_device *ndev, s32 iftype)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	s32 infra = 0;
	s32 err = 0;
	s32 mode = 0;
	switch (iftype) {
	case NL80211_IFTYPE_MONITOR:
	case NL80211_IFTYPE_WDS:
		WL_ERR(("type (%d) : currently we do not support this mode\n",
			iftype));
		err = -EINVAL;
		return err;
	case NL80211_IFTYPE_ADHOC:
		mode = WL_MODE_IBSS;
		break;
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_P2P_CLIENT:
		mode = WL_MODE_BSS;
		infra = 1;
		break;
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_P2P_GO:
		mode = WL_MODE_AP;
		infra = 1;
		break;
	default:
		err = -EINVAL;
		WL_ERR(("invalid type (%d)\n", iftype));
		return err;
	}
	infra = htod32(infra);
	err = wldev_ioctl(ndev, WLC_SET_INFRA, &infra, sizeof(infra), true);
	if (unlikely(err)) {
		WL_ERR(("WLC_SET_INFRA error (%d)\n", err));
		return err;
	}

<<<<<<< HEAD
	wl_set_mode_by_netdev(wl, ndev, mode);
=======
	wl_set_mode_by_netdev(cfg, ndev, mode);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return 0;
}

void wl_cfg80211_add_to_eventbuffer(struct wl_eventmsg_buf *ev, u16 event, bool set)
{
	if (!ev || (event > WLC_E_LAST))
		return;

	if (ev->num < MAX_EVENT_BUF_NUM) {
		ev->event[ev->num].type = event;
		ev->event[ev->num].set = set;
		ev->num++;
	} else {
		WL_ERR(("evenbuffer doesn't support > %u events. Update"
			" the define MAX_EVENT_BUF_NUM \n", MAX_EVENT_BUF_NUM));
		ASSERT(0);
	}
}

s32 wl_cfg80211_apply_eventbuffer(
	struct net_device *ndev,
<<<<<<< HEAD
	struct wl_priv *wl,
=======
	struct bcm_cfg80211 *cfg,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	wl_eventmsg_buf_t *ev)
{
	char eventmask[WL_EVENTING_MASK_LEN];
	int i, ret = 0;
	s8 iovbuf[WL_EVENTING_MASK_LEN + 12];

	if (!ev || (!ev->num))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&wl->event_sync);
=======
	mutex_lock(&cfg->event_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	/* Read event_msgs mask */
	bcm_mkiovar("event_msgs", NULL, 0, iovbuf,
		sizeof(iovbuf));
	ret = wldev_ioctl(ndev, WLC_GET_VAR, iovbuf, sizeof(iovbuf), false);
	if (unlikely(ret)) {
		WL_ERR(("Get event_msgs error (%d)\n", ret));
		goto exit;
	}
	memcpy(eventmask, iovbuf, WL_EVENTING_MASK_LEN);

	/* apply the set bits */
	for (i = 0; i < ev->num; i++) {
		if (ev->event[i].set)
			setbit(eventmask, ev->event[i].type);
		else
			clrbit(eventmask, ev->event[i].type);
	}

	/* Write updated Event mask */
	bcm_mkiovar("event_msgs", eventmask, WL_EVENTING_MASK_LEN, iovbuf,
		sizeof(iovbuf));
	ret = wldev_ioctl(ndev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), true);
	if (unlikely(ret)) {
		WL_ERR(("Set event_msgs error (%d)\n", ret));
	}

exit:
<<<<<<< HEAD
	mutex_unlock(&wl->event_sync);
=======
	mutex_unlock(&cfg->event_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return ret;
}

s32 wl_add_remove_eventmsg(struct net_device *ndev, u16 event, bool add)
{
	s8 iovbuf[WL_EVENTING_MASK_LEN + 12];
	s8 eventmask[WL_EVENTING_MASK_LEN];
	s32 err = 0;
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;

	if (!ndev || !wl)
		return -ENODEV;

	mutex_lock(&wl->event_sync);
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;

	if (!ndev || !cfg)
		return -ENODEV;

	mutex_lock(&cfg->event_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	/* Setup event_msgs */
	bcm_mkiovar("event_msgs", NULL, 0, iovbuf,
		sizeof(iovbuf));
	err = wldev_ioctl(ndev, WLC_GET_VAR, iovbuf, sizeof(iovbuf), false);
	if (unlikely(err)) {
		WL_ERR(("Get event_msgs error (%d)\n", err));
		goto eventmsg_out;
	}
	memcpy(eventmask, iovbuf, WL_EVENTING_MASK_LEN);
	if (add) {
		setbit(eventmask, event);
	} else {
		clrbit(eventmask, event);
	}
	bcm_mkiovar("event_msgs", eventmask, WL_EVENTING_MASK_LEN, iovbuf,
		sizeof(iovbuf));
	err = wldev_ioctl(ndev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), true);
	if (unlikely(err)) {
		WL_ERR(("Set event_msgs error (%d)\n", err));
		goto eventmsg_out;
	}

eventmsg_out:
<<<<<<< HEAD
	mutex_unlock(&wl->event_sync);
	return err;
}

static int wl_construct_reginfo(struct wl_priv *wl, s32 bw_cap)
{
	struct net_device *dev = wl_to_prmry_ndev(wl);
=======
	mutex_unlock(&cfg->event_sync);
	return err;
}

static int wl_construct_reginfo(struct bcm_cfg80211 *cfg, s32 bw_cap)
{
	struct net_device *dev = bcmcfg_to_prmry_ndev(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct ieee80211_channel *band_chan_arr = NULL;
	wl_uint32_list_t *list;
	u32 i, j, index, n_2g, n_5g, band, channel, array_size;
	u32 *n_cnt = NULL;
	chanspec_t c = 0;
	s32 err = BCME_OK;
	bool update;
	bool ht40_allowed;
	u8 *pbuf = NULL;
<<<<<<< HEAD
=======
	bool dfs_radar_disabled = FALSE;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

#define LOCAL_BUF_LEN 1024
	pbuf = kzalloc(LOCAL_BUF_LEN, GFP_KERNEL);

	if (pbuf == NULL) {
		WL_ERR(("failed to allocate local buf\n"));
		return -ENOMEM;
	}
<<<<<<< HEAD
	list = (wl_uint32_list_t *)(void *) pbuf;
=======
	list = (wl_uint32_list_t *)(void *)pbuf;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	list->count = htod32(WL_NUMCHANSPECS);


	err = wldev_iovar_getbuf_bsscfg(dev, "chanspecs", NULL,
<<<<<<< HEAD
		0, pbuf, LOCAL_BUF_LEN, 0, &wl->ioctl_buf_sync);
=======
		0, pbuf, LOCAL_BUF_LEN, 0, &cfg->ioctl_buf_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (err != 0) {
		WL_ERR(("get chanspecs failed with %d\n", err));
		kfree(pbuf);
		return err;
	}
#undef LOCAL_BUF_LEN

	list = (wl_uint32_list_t *)(void *)pbuf;
	band = array_size = n_2g = n_5g = 0;
	for (i = 0; i < dtoh32(list->count); i++) {
		index = 0;
		update = false;
		ht40_allowed = false;
		c = (chanspec_t)dtoh32(list->element[i]);
		c = wl_chspec_driver_to_host(c);
		channel = CHSPEC_CHANNEL(c);
		if (CHSPEC_IS40(c)) {
			if (CHSPEC_SB_UPPER(c))
				channel += CH_10MHZ_APART;
			else
				channel -= CH_10MHZ_APART;
		} else if (CHSPEC_IS80(c)) {
			WL_DBG(("HT80 center channel : %d\n", channel));
			continue;
		}
		if (CHSPEC_IS2G(c) && (channel >= CH_MIN_2G_CHANNEL) &&
			(channel <= CH_MAX_2G_CHANNEL)) {
			band_chan_arr = __wl_2ghz_channels;
			array_size = ARRAYSIZE(__wl_2ghz_channels);
			n_cnt = &n_2g;
			band = IEEE80211_BAND_2GHZ;
			ht40_allowed = (bw_cap  == WLC_N_BW_40ALL)? true : false;
		} else if (CHSPEC_IS5G(c) && channel >= CH_MIN_5G_CHANNEL) {
			band_chan_arr = __wl_5ghz_a_channels;
			array_size = ARRAYSIZE(__wl_5ghz_a_channels);
			n_cnt = &n_5g;
			band = IEEE80211_BAND_5GHZ;
			ht40_allowed = (bw_cap  == WLC_N_BW_20ALL)? false : true;
		} else {
			WL_ERR(("Invalid channel Sepc. 0x%x.\n", c));
			continue;
		}
		if (!ht40_allowed && CHSPEC_IS40(c))
			continue;
		for (j = 0; (j < *n_cnt && (*n_cnt < array_size)); j++) {
			if (band_chan_arr[j].hw_value == channel) {
				update = true;
				break;
			}
		}
		if (update)
			index = j;
		else
			index = *n_cnt;
		if (index <  array_size) {
#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 38) && !defined(WL_COMPAT_WIRELESS)
			band_chan_arr[index].center_freq =
				ieee80211_channel_to_frequency(channel);
#else
			band_chan_arr[index].center_freq =
				ieee80211_channel_to_frequency(channel, band);
#endif
			band_chan_arr[index].hw_value = channel;

			if (CHSPEC_IS40(c) && ht40_allowed) {
				/* assuming the order is HT20, HT40 Upper,
<<<<<<< HEAD
				   HT40 lower from chanspecs
				*/
=======
				 *  HT40 lower from chanspecs
				 */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				u32 ht40_flag = band_chan_arr[index].flags & IEEE80211_CHAN_NO_HT40;
				if (CHSPEC_SB_UPPER(c)) {
					if (ht40_flag == IEEE80211_CHAN_NO_HT40)
						band_chan_arr[index].flags &=
							~IEEE80211_CHAN_NO_HT40;
					band_chan_arr[index].flags |= IEEE80211_CHAN_NO_HT40PLUS;
				} else {
					/* It should be one of
<<<<<<< HEAD
						IEEE80211_CHAN_NO_HT40 or IEEE80211_CHAN_NO_HT40PLUS
					*/
=======
					 * IEEE80211_CHAN_NO_HT40 or IEEE80211_CHAN_NO_HT40PLUS
					 */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
					band_chan_arr[index].flags &= ~IEEE80211_CHAN_NO_HT40;
					if (ht40_flag == IEEE80211_CHAN_NO_HT40)
						band_chan_arr[index].flags |=
							IEEE80211_CHAN_NO_HT40MINUS;
				}
			} else {
				band_chan_arr[index].flags = IEEE80211_CHAN_NO_HT40;
<<<<<<< HEAD
				if (band == IEEE80211_BAND_2GHZ)
					channel |= WL_CHANSPEC_BAND_2G;
				else
					channel |= WL_CHANSPEC_BAND_5G;
				channel |= WL_CHANSPEC_BW_20;
				channel = wl_chspec_host_to_driver(channel);
				err = wldev_iovar_getint(dev, "per_chan_info", &channel);
				if (!err) {
					if (channel & WL_CHAN_RADAR)
						band_chan_arr[index].flags |=
							(IEEE80211_CHAN_RADAR |
							IEEE80211_CHAN_NO_IBSS);
					if (channel & WL_CHAN_PASSIVE)
						band_chan_arr[index].flags |=
							IEEE80211_CHAN_PASSIVE_SCAN;
=======
				if (!dfs_radar_disabled) {
					if (band == IEEE80211_BAND_2GHZ)
						channel |= WL_CHANSPEC_BAND_2G;
					else
						channel |= WL_CHANSPEC_BAND_5G;
					channel |= WL_CHANSPEC_BW_20;
					channel = wl_chspec_host_to_driver(channel);
					err = wldev_iovar_getint(dev, "per_chan_info", &channel);
					if (!err) {
						if (channel & WL_CHAN_RADAR)
							band_chan_arr[index].flags |=
								(IEEE80211_CHAN_RADAR |
								IEEE80211_CHAN_NO_IBSS);
						if (channel & WL_CHAN_PASSIVE)
							band_chan_arr[index].flags |=
								IEEE80211_CHAN_PASSIVE_SCAN;
					} else if (err == BCME_UNSUPPORTED) {
						dfs_radar_disabled = TRUE;
						WL_ERR(("does not support per_chan_info\n"));
					}
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
				}
			}
			if (!update)
				(*n_cnt)++;
		}

	}
	__wl_band_2ghz.n_channels = n_2g;
	__wl_band_5ghz_a.n_channels = n_5g;
	kfree(pbuf);
	return err;
}

<<<<<<< HEAD
s32 wl_update_wiphybands(struct wl_priv *wl, bool notify)
=======
s32 wl_update_wiphybands(struct bcm_cfg80211 *cfg, bool notify)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	struct wiphy *wiphy;
	struct net_device *dev;
	u32 bandlist[3];
	u32 nband = 0;
	u32 i = 0;
	s32 err = 0;
	s32 index = 0;
	s32 nmode = 0;
	bool rollback_lock = false;
	s32 bw_cap = 0;
	s32 cur_band = -1;
	struct ieee80211_supported_band *bands[IEEE80211_NUM_BANDS] = {NULL, };

<<<<<<< HEAD
	if (wl == NULL) {
		wl = wlcfg_drv_priv;
		mutex_lock(&wl->usr_sync);
		rollback_lock = true;
	}
	dev = wl_to_prmry_ndev(wl);
=======
	if (cfg == NULL) {
		cfg = g_bcm_cfg;
		mutex_lock(&cfg->usr_sync);
		rollback_lock = true;
	}
	dev = bcmcfg_to_prmry_ndev(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	memset(bandlist, 0, sizeof(bandlist));
	err = wldev_ioctl(dev, WLC_GET_BANDLIST, bandlist,
		sizeof(bandlist), false);
	if (unlikely(err)) {
		WL_ERR(("error read bandlist (%d)\n", err));
		goto end_bands;
	}
<<<<<<< HEAD

	wiphy = wl_to_wiphy(wl);

=======
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	err = wldev_ioctl(dev, WLC_GET_BAND, &cur_band,
		sizeof(s32), false);
	if (unlikely(err)) {
		WL_ERR(("error (%d)\n", err));
		goto end_bands;
	}

	err = wldev_iovar_getint(dev, "nmode", &nmode);
	if (unlikely(err)) {
		WL_ERR(("error reading nmode (%d)\n", err));
	} else {
<<<<<<< HEAD
		/* For nmodeonly check bw cap */
		err = wldev_iovar_getint(dev, "mimo_bw_cap", &bw_cap);
		if (unlikely(err)) {
			 WL_ERR(("error get mimo_bw_cap (%d)\n", err));
		}
	}

	err = wl_construct_reginfo(wl, bw_cap);
=======
		/* For nmodeonly  check bw cap */
		err = wldev_iovar_getint(dev, "mimo_bw_cap", &bw_cap);
		if (unlikely(err)) {
			WL_ERR(("error get mimo_bw_cap (%d)\n", err));
		}
	}

	err = wl_construct_reginfo(cfg, bw_cap);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (err) {
		WL_ERR(("wl_construct_reginfo() fails err=%d\n", err));
		if (err != BCME_UNSUPPORTED)
			goto end_bands;
		err = 0;
	}
<<<<<<< HEAD

=======
	wiphy = bcmcfg_to_wiphy(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	nband = bandlist[0];

	for (i = 1; i <= nband && i < ARRAYSIZE(bandlist); i++) {
		index = -1;
		if (bandlist[i] == WLC_BAND_5G && __wl_band_5ghz_a.n_channels > 0) {
			bands[IEEE80211_BAND_5GHZ] =
				&__wl_band_5ghz_a;
			index = IEEE80211_BAND_5GHZ;
			if (bw_cap == WLC_N_BW_40ALL || bw_cap == WLC_N_BW_20IN2G_40IN5G)
				bands[index]->ht_cap.cap |= IEEE80211_HT_CAP_SGI_40;
		}
		else if (bandlist[i] == WLC_BAND_2G && __wl_band_2ghz.n_channels > 0) {
			bands[IEEE80211_BAND_2GHZ] =
				&__wl_band_2ghz;
			index = IEEE80211_BAND_2GHZ;
			if (bw_cap == WLC_N_BW_40ALL)
				bands[index]->ht_cap.cap |= IEEE80211_HT_CAP_SGI_40;
		}

		if ((index >= 0) && nmode) {
			bands[index]->ht_cap.cap |=
				(IEEE80211_HT_CAP_SGI_20 | IEEE80211_HT_CAP_DSSSCCK40);
			bands[index]->ht_cap.ht_supported = TRUE;
			bands[index]->ht_cap.ampdu_factor = IEEE80211_HT_MAX_AMPDU_64K;
			bands[index]->ht_cap.ampdu_density = IEEE80211_HT_MPDU_DENSITY_16;
			/* An HT shall support all EQM rates for one spatial stream */
			bands[index]->ht_cap.mcs.rx_mask[0] = 0xff;
		}

	}

	wiphy->bands[IEEE80211_BAND_2GHZ] = bands[IEEE80211_BAND_2GHZ];
	wiphy->bands[IEEE80211_BAND_5GHZ] = bands[IEEE80211_BAND_5GHZ];

<<<<<<< HEAD
=======
	/* check if any bands populated otherwise makes 2Ghz as default */
	if (wiphy->bands[IEEE80211_BAND_2GHZ] == NULL &&
		wiphy->bands[IEEE80211_BAND_5GHZ] == NULL) {
		/* Setup 2Ghz band as default */
		wiphy->bands[IEEE80211_BAND_2GHZ] = &__wl_band_2ghz;
	}

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (notify)
		wiphy_apply_custom_regulatory(wiphy, &brcm_regdom);

	end_bands:
		if (rollback_lock)
<<<<<<< HEAD
			mutex_unlock(&wl->usr_sync);
		return err;
}

static s32 __wl_cfg80211_up(struct wl_priv *wl)
{
	s32 err = 0;
	struct net_device *ndev = wl_to_prmry_ndev(wl);
=======
			mutex_unlock(&cfg->usr_sync);
	return err;
}

static s32 __wl_cfg80211_up(struct bcm_cfg80211 *cfg)
{
	s32 err = 0;
#ifdef WL_HOST_BAND_MGMT
	s32 ret = 0;
#endif /* WL_HOST_BAND_MGMT */
	struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct wireless_dev *wdev = ndev->ieee80211_ptr;

	WL_DBG(("In\n"));

<<<<<<< HEAD
	err = dhd_config_dongle(wl, false);
	if (unlikely(err))
		return err;

	err = wl_config_ifmode(wl, ndev, wdev->iftype);
	if (unlikely(err && err != -EINPROGRESS)) {
		WL_ERR(("wl_config_ifmode failed\n"));
	}
	err = wl_update_wiphybands(wl, true);
	if (unlikely(err)) {
		WL_ERR(("wl_update_wiphybands failed\n"));
	}

	err = dhd_monitor_init(wl->pub);
	err = wl_invoke_iscan(wl);

#ifdef WL_HOST_BAND_MGMT
	/* By default the curr_band is initialized to BAND_AUTO */
	if (wl_cfg80211_set_band(ndev, WLC_BAND_AUTO) < 0) {
		WL_ERR(("roam_band set failed\n"));
		err = -1;
	}
#endif /* WL_HOST_BAND_MGMT */

#if defined(DHCP_SCAN_SUPPRESS)
	/* wlan scan_supp timer and work thread info */
	init_timer(&wl->scan_supp_timer);
	wl->scan_supp_timer.data = (ulong)wl;
	wl->scan_supp_timer.function = wl_cfg80211_scan_supp_timerfunc;
	INIT_WORK(&wl->wlan_work, wl_cfg80211_work_handler);
#endif /* DHCP_SCAN_SUPPRESS */

	wl_set_drv_status(wl, READY, ndev);
	return err;
}

static s32 __wl_cfg80211_down(struct wl_priv *wl)
=======
	err = dhd_config_dongle(cfg);
	if (unlikely(err))
		return err;

	err = wl_config_ifmode(cfg, ndev, wdev->iftype);
	if (unlikely(err && err != -EINPROGRESS)) {
		WL_ERR(("wl_config_ifmode failed\n"));
		if (err == -1) {
			WL_ERR(("return error %d\n", err));
			return err;
		}
	}
	err = wl_update_wiphybands(cfg, true);
	if (unlikely(err)) {
		WL_ERR(("wl_update_wiphybands failed\n"));
		if (err == -1) {
			WL_ERR(("return error %d\n", err));
			return err;
		}
	}

	err = dhd_monitor_init(cfg->pub);

#ifdef WL_HOST_BAND_MGMT
	/* By default the curr_band is initialized to BAND_AUTO */
	if ((ret = wl_cfg80211_set_band(ndev, WLC_BAND_AUTO)) < 0) {
		if (ret == BCME_UNSUPPORTED) {
			/* Don't fail the initialization, lets just
			 * fall back to the original method
			 */
			WL_ERR(("WL_HOST_BAND_MGMT defined, "
				"but roam_band iovar not supported \n"));
		} else {
			WL_ERR(("roam_band failed. ret=%d", ret));
			err = -1;
		}
	}
#endif /* WL_HOST_BAND_MGMT */
#if defined(CUSTOMER_HW4) && defined(WES_SUPPORT)
	/* Reset WES mode to 0 */
	wes_mode = 0;
#endif
	INIT_DELAYED_WORK(&cfg->pm_enable_work, wl_cfg80211_work_handler);
	wl_set_drv_status(cfg, READY, ndev);
	return err;
}

static s32 __wl_cfg80211_down(struct bcm_cfg80211 *cfg)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	s32 err = 0;
	unsigned long flags;
	struct net_info *iter, *next;
<<<<<<< HEAD
	struct net_device *ndev = wl_to_prmry_ndev(wl);
	struct net_device *p2p_net = wl->p2p_net;
	u32 bssidx = wl_cfgp2p_find_idx(wl, ndev);
	WL_DBG(("In\n"));

#if defined(DHCP_SCAN_SUPPRESS)
	/* Force clear of scan_suppress */
	if (wl->scan_suppressed)
		wl_cfg80211_scan_suppress(ndev, 0);
	if (timer_pending(&wl->scan_supp_timer))
		del_timer_sync(&wl->scan_supp_timer);
	cancel_work_sync(&wl->wlan_work);
#endif /* DHCP_SCAN_SUPPRESS */

	/* If BSS is operational (e.g SoftAp), bring it down */
	if (wl_cfgp2p_bss_isup(ndev, bssidx)) {
		if (wl_cfgp2p_bss(wl, ndev, bssidx, 0) < 0)
=======
	struct net_device *ndev = bcmcfg_to_prmry_ndev(cfg);
#if defined(WL_CFG80211) && (defined(WL_ENABLE_P2P_IF) || \
	defined(WL_NEWCFG_PRIVCMD_SUPPORT))
	struct net_device *p2p_net = cfg->p2p_net;
#endif /* WL_CFG80211 && (WL_ENABLE_P2P_IF || WL_NEWCFG_PRIVCMD_SUPPORT) */
	u32 bssidx = 0;
#ifdef PROP_TXSTATUS_VSDB
	dhd_pub_t *dhd =  (dhd_pub_t *)(cfg->pub);
#endif /* PROP_TXSTATUS_VSDB */
	WL_DBG(("In\n"));
	/* Delete pm_enable_work */
	wl_add_remove_pm_enable_work(cfg, FALSE, WL_HANDLER_DEL);

	if (cfg->p2p_supported) {
		wl_clr_p2p_status(cfg, GO_NEG_PHASE);
#ifdef PROP_TXSTATUS_VSDB
		if (cfg->p2p->vif_created) {
			bool enabled = false;
			dhd_wlfc_get_enable(dhd, &enabled);
			if (enabled && cfg->wlfc_on && dhd->op_mode != DHD_FLAG_HOSTAP_MODE &&
				dhd->op_mode != DHD_FLAG_IBSS_MODE) {
				dhd_wlfc_deinit(dhd);
				cfg->wlfc_on = false;
			}
		}
#endif /* PROP_TXSTATUS_VSDB */
	}


	/* If primary BSS is operational (for e.g SoftAP), bring it down */
	if (!(wl_cfgp2p_find_idx(cfg, ndev, &bssidx)) &&
		wl_cfgp2p_bss_isup(ndev, bssidx)) {
		if (wl_cfgp2p_bss(cfg, ndev, bssidx, 0) < 0)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			WL_ERR(("BSS down failed \n"));
	}

	/* Check if cfg80211 interface is already down */
<<<<<<< HEAD
	if (!wl_get_drv_status(wl, READY, ndev))
		return err;	/* it is even not ready */

	for_each_ndev(wl, iter, next)
		wl_set_drv_status(wl, SCAN_ABORTING, iter->ndev);


	wl_term_iscan(wl);
	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
	if (wl->scan_request) {
		cfg80211_scan_done(wl->scan_request, true);
		wl->scan_request = NULL;
	}
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);

	for_each_ndev(wl, iter, next) {
		wl_clr_drv_status(wl, READY, iter->ndev);
		wl_clr_drv_status(wl, SCANNING, iter->ndev);
		wl_clr_drv_status(wl, SCAN_ABORTING, iter->ndev);
		wl_clr_drv_status(wl, CONNECTING, iter->ndev);
		wl_clr_drv_status(wl, CONNECTED, iter->ndev);
		wl_clr_drv_status(wl, DISCONNECTING, iter->ndev);
		wl_clr_drv_status(wl, AP_CREATED, iter->ndev);
		wl_clr_drv_status(wl, AP_CREATING, iter->ndev);
	}
	wl_to_prmry_ndev(wl)->ieee80211_ptr->iftype =
		NL80211_IFTYPE_STATION;
	if (p2p_net)
		dev_close(p2p_net);
	DNGL_FUNC(dhd_cfg80211_down, (wl));
	wl_flush_eq(wl);
	wl_link_down(wl);
	if (wl->p2p_supported)
		wl_cfgp2p_down(wl);
=======
	if (!wl_get_drv_status(cfg, READY, ndev))
		return err;	/* it is even not ready */
	for_each_ndev(cfg, iter, next)
		wl_set_drv_status(cfg, SCAN_ABORTING, iter->ndev);


	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
	if (cfg->scan_request) {
		cfg80211_scan_done(cfg->scan_request, true);
		cfg->scan_request = NULL;
	}
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);

	for_each_ndev(cfg, iter, next) {
		wl_clr_drv_status(cfg, READY, iter->ndev);
		wl_clr_drv_status(cfg, SCANNING, iter->ndev);
		wl_clr_drv_status(cfg, SCAN_ABORTING, iter->ndev);
		wl_clr_drv_status(cfg, CONNECTING, iter->ndev);
		wl_clr_drv_status(cfg, CONNECTED, iter->ndev);
		wl_clr_drv_status(cfg, DISCONNECTING, iter->ndev);
		wl_clr_drv_status(cfg, AP_CREATED, iter->ndev);
		wl_clr_drv_status(cfg, AP_CREATING, iter->ndev);
	}
	bcmcfg_to_prmry_ndev(cfg)->ieee80211_ptr->iftype =
		NL80211_IFTYPE_STATION;
#if defined(WL_CFG80211) && (defined(WL_ENABLE_P2P_IF) || \
	defined(WL_NEWCFG_PRIVCMD_SUPPORT))
#ifdef SUPPORT_DEEP_SLEEP
	if (!trigger_deep_sleep)
#endif /* SUPPORT_DEEP_SLEEP */
		if (p2p_net)
			dev_close(p2p_net);
#endif /* WL_CFG80211 && (WL_ENABLE_P2P_IF || WL_NEWCFG_PRIVCMD_SUPPORT) */
	DNGL_FUNC(dhd_cfg80211_down, (cfg));
	wl_flush_eq(cfg);
	wl_link_down(cfg);
	if (cfg->p2p_supported)
		wl_cfgp2p_down(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	dhd_monitor_uninit();

	return err;
}

s32 wl_cfg80211_up(void *para)
{
<<<<<<< HEAD
	struct wl_priv *wl;
	s32 err = 0;
	int val = 1;
	dhd_pub_t *dhd;
	(void)para;
	WL_DBG(("In\n"));
	wl = wlcfg_drv_priv;

	if ((err = wldev_ioctl(wl_to_prmry_ndev(wl), WLC_GET_VERSION, &val,
=======
	struct bcm_cfg80211 *cfg;
	s32 err = 0;
	int val = 1;
	dhd_pub_t *dhd;

	(void)para;
	WL_DBG(("In\n"));
	cfg = g_bcm_cfg;

	if ((err = wldev_ioctl(bcmcfg_to_prmry_ndev(cfg), WLC_GET_VERSION, &val,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		sizeof(int), false) < 0)) {
		WL_ERR(("WLC_GET_VERSION failed, err=%d\n", err));
		return err;
	}
	val = dtoh32(val);
	if (val != WLC_IOCTL_VERSION && val != 1) {
		WL_ERR(("Version mismatch, please upgrade. Got %d, expected %d or 1\n",
			val, WLC_IOCTL_VERSION));
		return BCME_VERSION;
	}
	ioctl_version = val;
	WL_TRACE(("WLC_GET_VERSION=%d\n", ioctl_version));

<<<<<<< HEAD
	mutex_lock(&wl->usr_sync);
	dhd = (dhd_pub_t *)(wl->pub);
	if (!(dhd->op_mode & DHD_FLAG_HOSTAP_MODE)) {
		err = wl_cfg80211_attach_post(wl_to_prmry_ndev(wl));
		if (unlikely(err))
			return err;
	}
	err = __wl_cfg80211_up(wl);
	if (unlikely(err))
		WL_ERR(("__wl_cfg80211_up failed\n"));
	mutex_unlock(&wl->usr_sync);
=======
	mutex_lock(&cfg->usr_sync);
	dhd = (dhd_pub_t *)(cfg->pub);
	if (!(dhd->op_mode & DHD_FLAG_HOSTAP_MODE)) {
		err = wl_cfg80211_attach_post(bcmcfg_to_prmry_ndev(cfg));
		if (unlikely(err))
			return err;
	}
#if defined(BCMSUP_4WAY_HANDSHAKE) && defined(WLAN_AKM_SUITE_FT_8021X)
	if (dhd->fw_4way_handshake)
		cfg->wdev->wiphy->features |= NL80211_FEATURE_FW_4WAY_HANDSHAKE;
#endif
	err = __wl_cfg80211_up(cfg);
	if (unlikely(err))
		WL_ERR(("__wl_cfg80211_up failed\n"));
#ifdef ROAM_CHANNEL_CACHE
	init_roam(ioctl_version);
#endif
	mutex_unlock(&cfg->usr_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}

/* Private Event to Supplicant with indication that chip hangs */
int wl_cfg80211_hang(struct net_device *dev, u16 reason)
{
<<<<<<< HEAD
	struct wl_priv *wl;
	wl = wlcfg_drv_priv;

	WL_ERR(("In : chip crash eventing\n"));
	cfg80211_disconnected(dev, reason, NULL, 0, GFP_KERNEL);
	if (wl != NULL) {
		wl_link_down(wl);
=======
	struct bcm_cfg80211 *cfg;
	cfg = g_bcm_cfg;

	WL_ERR(("In : chip crash eventing\n"));
	wl_add_remove_pm_enable_work(cfg, FALSE, WL_HANDLER_DEL);
	cfg80211_disconnected(dev, reason, NULL, 0, GFP_KERNEL);
	if (cfg != NULL) {
		wl_link_down(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
	return 0;
}

s32 wl_cfg80211_down(void *para)
{
<<<<<<< HEAD
	struct wl_priv *wl;
=======
	struct bcm_cfg80211 *cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = 0;

	(void)para;
	WL_DBG(("In\n"));
<<<<<<< HEAD
	wl = wlcfg_drv_priv;
	mutex_lock(&wl->usr_sync);
	err = __wl_cfg80211_down(wl);
	mutex_unlock(&wl->usr_sync);
=======
	cfg = g_bcm_cfg;
	mutex_lock(&cfg->usr_sync);
	err = __wl_cfg80211_down(cfg);
	mutex_unlock(&cfg->usr_sync);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return err;
}

<<<<<<< HEAD
static void *wl_read_prof(struct wl_priv *wl, struct net_device *ndev, s32 item)
{
	unsigned long flags;
	void *rptr = NULL;
	struct wl_profile *profile = wl_get_profile_by_netdev(wl, ndev);

	if (!profile)
		return NULL;
	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
=======
static void *wl_read_prof(struct bcm_cfg80211 *cfg, struct net_device *ndev, s32 item)
{
	unsigned long flags;
	void *rptr = NULL;
	struct wl_profile *profile = wl_get_profile_by_netdev(cfg, ndev);

	if (!profile)
		return NULL;
	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	switch (item) {
	case WL_PROF_SEC:
		rptr = &profile->sec;
		break;
	case WL_PROF_ACT:
		rptr = &profile->active;
		break;
	case WL_PROF_BSSID:
		rptr = profile->bssid;
		break;
	case WL_PROF_SSID:
		rptr = &profile->ssid;
		break;
	case WL_PROF_CHAN:
		rptr = &profile->channel;
		break;
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);
=======
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	if (!rptr)
		WL_ERR(("invalid item (%d)\n", item));
	return rptr;
}

static s32
<<<<<<< HEAD
wl_update_prof(struct wl_priv *wl, struct net_device *ndev,
=======
wl_update_prof(struct bcm_cfg80211 *cfg, struct net_device *ndev,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	const wl_event_msg_t *e, void *data, s32 item)
{
	s32 err = 0;
	struct wlc_ssid *ssid;
	unsigned long flags;
<<<<<<< HEAD
	struct wl_profile *profile = wl_get_profile_by_netdev(wl, ndev);

	if (!profile)
		return WL_INVALID;
	spin_lock_irqsave(&wl->cfgdrv_lock, flags);
=======
	struct wl_profile *profile = wl_get_profile_by_netdev(cfg, ndev);

	if (!profile)
		return WL_INVALID;
	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	switch (item) {
	case WL_PROF_SSID:
		ssid = (wlc_ssid_t *) data;
		memset(profile->ssid.SSID, 0,
			sizeof(profile->ssid.SSID));
		memcpy(profile->ssid.SSID, ssid->SSID, ssid->SSID_len);
		profile->ssid.SSID_len = ssid->SSID_len;
		break;
	case WL_PROF_BSSID:
		if (data)
			memcpy(profile->bssid, data, ETHER_ADDR_LEN);
		else
			memset(profile->bssid, 0, ETHER_ADDR_LEN);
		break;
	case WL_PROF_SEC:
		memcpy(&profile->sec, data, sizeof(profile->sec));
		break;
	case WL_PROF_ACT:
		profile->active = *(bool *)data;
		break;
	case WL_PROF_BEACONINT:
		profile->beacon_interval = *(u16 *)data;
		break;
	case WL_PROF_DTIMPERIOD:
		profile->dtim_period = *(u8 *)data;
		break;
	case WL_PROF_CHAN:
		profile->channel = *(u32*)data;
<<<<<<< HEAD
=======
		break;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	default:
		err = -EOPNOTSUPP;
		break;
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&wl->cfgdrv_lock, flags);

	if (err == EOPNOTSUPP)
=======
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);

	if (err == -EOPNOTSUPP)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_ERR(("unsupported item (%d)\n", item));

	return err;
}

void wl_cfg80211_dbg_level(u32 level)
{
	/*
	* prohibit to change debug level
	* by insmod parameter.
	* eventually debug level will be configured
	* in compile time by using CONFIG_XXX
	*/
	/* wl_dbg_level = level; */
}

<<<<<<< HEAD
static bool wl_is_ibssmode(struct wl_priv *wl, struct net_device *ndev)
{
	return wl_get_mode_by_netdev(wl, ndev) == WL_MODE_IBSS;
}

static __used bool wl_is_ibssstarter(struct wl_priv *wl)
{
	return wl->ibss_starter;
}

static void wl_rst_ie(struct wl_priv *wl)
{
	struct wl_ie *ie = wl_to_ie(wl);
=======
static bool wl_is_ibssmode(struct bcm_cfg80211 *cfg, struct net_device *ndev)
{
	return wl_get_mode_by_netdev(cfg, ndev) == WL_MODE_IBSS;
}

static __used bool wl_is_ibssstarter(struct bcm_cfg80211 *cfg)
{
	return cfg->ibss_starter;
}

static void wl_rst_ie(struct bcm_cfg80211 *cfg)
{
	struct wl_ie *ie = wl_to_ie(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	ie->offset = 0;
}

<<<<<<< HEAD
static __used s32 wl_add_ie(struct wl_priv *wl, u8 t, u8 l, u8 *v)
{
	struct wl_ie *ie = wl_to_ie(wl);
=======
static __used s32 wl_add_ie(struct bcm_cfg80211 *cfg, u8 t, u8 l, u8 *v)
{
	struct wl_ie *ie = wl_to_ie(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = 0;

	if (unlikely(ie->offset + l + 2 > WL_TLV_INFO_MAX)) {
		WL_ERR(("ei crosses buffer boundary\n"));
		return -ENOSPC;
	}
	ie->buf[ie->offset] = t;
	ie->buf[ie->offset + 1] = l;
	memcpy(&ie->buf[ie->offset + 2], v, l);
	ie->offset += l + 2;

	return err;
}

<<<<<<< HEAD
static s32 wl_mrg_ie(struct wl_priv *wl, u8 *ie_stream, u16 ie_size)
{
	struct wl_ie *ie = wl_to_ie(wl);
=======
static void wl_update_hidden_ap_ie(struct wl_bss_info *bi, u8 *ie_stream, u32 *ie_size)
{
	u8 *ssidie;
	ssidie = (u8 *)cfg80211_find_ie(WLAN_EID_SSID, ie_stream, *ie_size);
	if (!ssidie)
		return;
	if (ssidie[1] != bi->SSID_len) {
		if (ssidie[1]) {
			WL_ERR(("%s: Wrong SSID len: %d != %d\n",
				__FUNCTION__, ssidie[1], bi->SSID_len));
			return;
		}
		memmove(ssidie + bi->SSID_len + 2, ssidie + 2, *ie_size - (ssidie + 2 - ie_stream));
		memcpy(ssidie + 2, bi->SSID, bi->SSID_len);
		*ie_size = *ie_size + bi->SSID_len;
		ssidie[1] = bi->SSID_len;
		return;
	}
	if (*(ssidie + 2) == '\0')
		 memcpy(ssidie + 2, bi->SSID, bi->SSID_len);
	return;
}

static s32 wl_mrg_ie(struct bcm_cfg80211 *cfg, u8 *ie_stream, u16 ie_size)
{
	struct wl_ie *ie = wl_to_ie(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = 0;

	if (unlikely(ie->offset + ie_size > WL_TLV_INFO_MAX)) {
		WL_ERR(("ei_stream crosses buffer boundary\n"));
		return -ENOSPC;
	}
	memcpy(&ie->buf[ie->offset], ie_stream, ie_size);
	ie->offset += ie_size;

	return err;
}

<<<<<<< HEAD
static s32 wl_cp_ie(struct wl_priv *wl, u8 *dst, u16 dst_size)
{
	struct wl_ie *ie = wl_to_ie(wl);
=======
static s32 wl_cp_ie(struct bcm_cfg80211 *cfg, u8 *dst, u16 dst_size)
{
	struct wl_ie *ie = wl_to_ie(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	s32 err = 0;

	if (unlikely(ie->offset > dst_size)) {
		WL_ERR(("dst_size is not enough\n"));
		return -ENOSPC;
	}
	memcpy(dst, &ie->buf[0], ie->offset);

	return err;
}

<<<<<<< HEAD
static u32 wl_get_ielen(struct wl_priv *wl)
{
	struct wl_ie *ie = wl_to_ie(wl);
=======
static u32 wl_get_ielen(struct bcm_cfg80211 *cfg)
{
	struct wl_ie *ie = wl_to_ie(cfg);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return ie->offset;
}

<<<<<<< HEAD
static void wl_link_up(struct wl_priv *wl)
{
	wl->link_up = true;
}

static void wl_link_down(struct wl_priv *wl)
{
	struct wl_connect_info *conn_info = wl_to_conn(wl);

	WL_DBG(("In\n"));
	wl->link_up = false;
=======
static void wl_link_up(struct bcm_cfg80211 *cfg)
{
	cfg->link_up = true;
}

static void wl_link_down(struct bcm_cfg80211 *cfg)
{
	struct wl_connect_info *conn_info = wl_to_conn(cfg);

	WL_DBG(("In\n"));
	cfg->link_up = false;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	conn_info->req_ie_len = 0;
	conn_info->resp_ie_len = 0;
}

<<<<<<< HEAD
static unsigned long wl_lock_eq(struct wl_priv *wl)
{
	unsigned long flags;

	spin_lock_irqsave(&wl->eq_lock, flags);
	return flags;
}

static void wl_unlock_eq(struct wl_priv *wl, unsigned long flags)
{
	spin_unlock_irqrestore(&wl->eq_lock, flags);
}

static void wl_init_eq_lock(struct wl_priv *wl)
{
	spin_lock_init(&wl->eq_lock);
=======
static unsigned long wl_lock_eq(struct bcm_cfg80211 *cfg)
{
	unsigned long flags;

	spin_lock_irqsave(&cfg->eq_lock, flags);
	return flags;
}

static void wl_unlock_eq(struct bcm_cfg80211 *cfg, unsigned long flags)
{
	spin_unlock_irqrestore(&cfg->eq_lock, flags);
}

static void wl_init_eq_lock(struct bcm_cfg80211 *cfg)
{
	spin_lock_init(&cfg->eq_lock);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

static void wl_delay(u32 ms)
{
	if (in_atomic() || (ms < jiffies_to_msecs(1))) {
<<<<<<< HEAD
		mdelay(ms);
	} else {
		msleep(ms);
=======
		OSL_DELAY(ms*1000);
	} else {
		OSL_SLEEP(ms);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
}

s32 wl_cfg80211_get_p2p_dev_addr(struct net_device *net, struct ether_addr *p2pdev_addr)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
	struct ether_addr p2pif_addr;
	struct ether_addr primary_mac;
	if (!wl->p2p)
		return -1;
	if (!p2p_is_on(wl)) {
		get_primary_mac(wl, &primary_mac);
		wl_cfgp2p_generate_bss_mac(&primary_mac, p2pdev_addr, &p2pif_addr);
	} else {
		memcpy(p2pdev_addr->octet,
			wl->p2p->dev_addr.octet, ETHER_ADDR_LEN);
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
	struct ether_addr p2pif_addr;
	struct ether_addr primary_mac;
	if (!cfg->p2p)
		return -1;
	if (!p2p_is_on(cfg)) {
		get_primary_mac(cfg, &primary_mac);
		wl_cfgp2p_generate_bss_mac(&primary_mac, p2pdev_addr, &p2pif_addr);
	} else {
		memcpy(p2pdev_addr->octet,
			cfg->p2p->dev_addr.octet, ETHER_ADDR_LEN);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}


	return 0;
}
s32 wl_cfg80211_set_p2p_noa(struct net_device *net, char* buf, int len)
{
<<<<<<< HEAD
	struct wl_priv *wl;

	wl = wlcfg_drv_priv;

	return wl_cfgp2p_set_p2p_noa(wl, net, buf, len);
=======
	struct bcm_cfg80211 *cfg;

	cfg = g_bcm_cfg;

	return wl_cfgp2p_set_p2p_noa(cfg, net, buf, len);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

s32 wl_cfg80211_get_p2p_noa(struct net_device *net, char* buf, int len)
{
<<<<<<< HEAD
	struct wl_priv *wl;
	wl = wlcfg_drv_priv;

	return wl_cfgp2p_get_p2p_noa(wl, net, buf, len);
=======
	struct bcm_cfg80211 *cfg;
	cfg = g_bcm_cfg;

	return wl_cfgp2p_get_p2p_noa(cfg, net, buf, len);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

s32 wl_cfg80211_set_p2p_ps(struct net_device *net, char* buf, int len)
{
<<<<<<< HEAD
	struct wl_priv *wl;
	wl = wlcfg_drv_priv;

	return wl_cfgp2p_set_p2p_ps(wl, net, buf, len);
=======
	struct bcm_cfg80211 *cfg;
	cfg = g_bcm_cfg;

	return wl_cfgp2p_set_p2p_ps(cfg, net, buf, len);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

s32 wl_cfg80211_channel_to_freq(u32 channel)
{
	int freq = 0;

#if LINUX_VERSION_CODE == KERNEL_VERSION(2, 6, 38) && !defined(WL_COMPAT_WIRELESS)
	freq = ieee80211_channel_to_frequency(channel);
#else
	{
		u16 band = 0;
		if (channel <= CH_MAX_2G_CHANNEL)
			band = IEEE80211_BAND_2GHZ;
		else
			band = IEEE80211_BAND_5GHZ;
		freq = ieee80211_channel_to_frequency(channel, band);
	}
#endif
	return freq;
}

<<<<<<< HEAD
s32 wl_cfg80211_set_wps_p2p_ie(struct net_device *net, char *buf, int len,
	enum wl_management_type type)
{
	struct wl_priv *wl;
=======

#ifdef WLTDLS
static s32
wl_tdls_event_handler(struct bcm_cfg80211 *cfg, bcm_struct_cfgdev *cfgdev,
	const wl_event_msg_t *e, void *data) {

	struct net_device *ndev = NULL;
	u32 reason = ntoh32(e->reason);
	s8 *msg = NULL;

	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	switch (reason) {
	case WLC_E_TDLS_PEER_DISCOVERED :
		msg = " TDLS PEER DISCOVERD ";
		break;
	case WLC_E_TDLS_PEER_CONNECTED :
		msg = " TDLS PEER CONNECTED ";
		break;
	case WLC_E_TDLS_PEER_DISCONNECTED :
		msg = "TDLS PEER DISCONNECTED ";
		break;
	}
	if (msg) {
		WL_ERR(("%s: " MACDBG " on %s ndev\n", msg, MAC2STRDBG((u8*)(&e->addr)),
			(bcmcfg_to_prmry_ndev(cfg) == ndev) ? "primary" : "secondary"));
	}
	return 0;

}
#endif  /* WLTDLS */

#if (LINUX_VERSION_CODE > KERNEL_VERSION(3, 2, 0)) || defined(WL_COMPAT_WIRELESS)
static s32
wl_cfg80211_tdls_oper(struct wiphy *wiphy, struct net_device *dev,
	u8 *peer, enum nl80211_tdls_operation oper)
{
	s32 ret = 0;
#ifdef WLTDLS
	struct bcm_cfg80211 *cfg;
	tdls_iovar_t info;
	cfg = g_bcm_cfg;
	memset(&info, 0, sizeof(tdls_iovar_t));
	if (peer)
		memcpy(&info.ea, peer, ETHER_ADDR_LEN);
	switch (oper) {
	case NL80211_TDLS_DISCOVERY_REQ:
		/* turn on TDLS */
		ret = dhd_tdls_enable(dev, true, false, NULL);
		if (ret < 0)
			return ret;
		info.mode = TDLS_MANUAL_EP_DISCOVERY;
		break;
	case NL80211_TDLS_SETUP:
		/* auto mode on */
		ret = dhd_tdls_enable(dev, true, true, (struct ether_addr *)peer);
		if (ret < 0)
			return ret;
		break;
	case NL80211_TDLS_TEARDOWN:
		info.mode = TDLS_MANUAL_EP_DELETE;
		/* auto mode off */
		ret = dhd_tdls_enable(dev, true, false, (struct ether_addr *)peer);
		if (ret < 0)
			return ret;
		break;
	default:
		WL_ERR(("Unsupported operation : %d\n", oper));
		goto out;
	}
	if (info.mode) {
		ret = wldev_iovar_setbuf(dev, "tdls_endpoint", &info, sizeof(info),
			cfg->ioctl_buf, WLC_IOCTL_MAXLEN, &cfg->ioctl_buf_sync);
		if (ret) {
			WL_ERR(("tdls_endpoint error %d\n", ret));
		}
	}
out:
#endif /* WLTDLS */
	return ret;
}
#endif /* LINUX_VERSION > VERSION(3,2,0) || WL_COMPAT_WIRELESS */

s32 wl_cfg80211_set_wps_p2p_ie(struct net_device *net, char *buf, int len,
	enum wl_management_type type)
{
	struct bcm_cfg80211 *cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	struct net_device *ndev = NULL;
	struct ether_addr primary_mac;
	s32 ret = 0;
	s32 bssidx = 0;
	s32 pktflag = 0;
<<<<<<< HEAD
	wl = wlcfg_drv_priv;

	if (wl_get_drv_status(wl, AP_CREATING, net) ||
		wl_get_drv_status(wl, AP_CREATED, net)) {
		ndev = net;
		bssidx = 0;
	} else if (wl->p2p) {
		if (net == wl->p2p_net) {
			net = wl_to_prmry_ndev(wl);
		}
		if (!wl->p2p->on) {
			get_primary_mac(wl, &primary_mac);
			wl_cfgp2p_generate_bss_mac(&primary_mac, &wl->p2p->dev_addr,
				&wl->p2p->int_addr);
=======
	cfg = g_bcm_cfg;

	if (wl_get_drv_status(cfg, AP_CREATING, net)) {
		/* Vendor IEs should be set to FW
		 * after SoftAP interface is brought up
		 */
		goto exit;
	} else if (wl_get_drv_status(cfg, AP_CREATED, net)) {
		ndev = net;
		bssidx = 0;
	} else if (cfg->p2p) {
		net = ndev_to_wlc_ndev(net, cfg);
		if (!cfg->p2p->on) {
			get_primary_mac(cfg, &primary_mac);
			wl_cfgp2p_generate_bss_mac(&primary_mac, &cfg->p2p->dev_addr,
				&cfg->p2p->int_addr);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			/* In case of p2p_listen command, supplicant send remain_on_channel
			* without turning on P2P
			*/

<<<<<<< HEAD
			p2p_on(wl) = true;
			ret = wl_cfgp2p_enable_discovery(wl, net, NULL, 0);
=======
			p2p_on(cfg) = true;
			ret = wl_cfgp2p_enable_discovery(cfg, net, NULL, 0);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

			if (unlikely(ret)) {
				goto exit;
			}
		}
<<<<<<< HEAD
		if (net  != wl_to_prmry_ndev(wl)) {
			if (wl_get_mode_by_netdev(wl, net) == WL_MODE_AP) {
				ndev = wl_to_p2p_bss_ndev(wl, P2PAPI_BSSCFG_CONNECTION);
				bssidx = wl_to_p2p_bss_bssidx(wl, P2PAPI_BSSCFG_CONNECTION);
			}
		} else {
				ndev = wl_to_p2p_bss_ndev(wl, P2PAPI_BSSCFG_PRIMARY);
				bssidx = wl_to_p2p_bss_bssidx(wl, P2PAPI_BSSCFG_DEVICE);
=======
		if (net  != bcmcfg_to_prmry_ndev(cfg)) {
			if (wl_get_mode_by_netdev(cfg, net) == WL_MODE_AP) {
				ndev = wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_CONNECTION);
				bssidx = wl_to_p2p_bss_bssidx(cfg, P2PAPI_BSSCFG_CONNECTION);
			}
		} else {
				ndev = wl_to_p2p_bss_ndev(cfg, P2PAPI_BSSCFG_PRIMARY);
				bssidx = wl_to_p2p_bss_bssidx(cfg, P2PAPI_BSSCFG_DEVICE);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		}
	}
	if (ndev != NULL) {
		switch (type) {
			case WL_BEACON:
				pktflag = VNDR_IE_BEACON_FLAG;
				break;
			case WL_PROBE_RESP:
				pktflag = VNDR_IE_PRBRSP_FLAG;
				break;
			case WL_ASSOC_RESP:
				pktflag = VNDR_IE_ASSOCRSP_FLAG;
				break;
		}
		if (pktflag)
<<<<<<< HEAD
			ret = wl_cfgp2p_set_management_ie(wl, ndev, bssidx, pktflag, buf, len);
=======
			ret = wl_cfgp2p_set_management_ie(cfg, ndev, bssidx, pktflag, buf, len);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}
exit:
	return ret;
}

<<<<<<< HEAD
=======
#ifdef WL_SUPPORT_AUTO_CHANNEL
static s32
wl_cfg80211_set_auto_channel_scan_state(struct net_device *ndev)
{
	u32 val = 0;
	s32 ret = BCME_ERROR;
	struct bcm_cfg80211 *cfg = g_bcm_cfg;

	/* Disable mpc, to avoid automatic interface down. */
	val = 0;

	ret = wldev_iovar_setbuf_bsscfg(ndev, "mpc", (void *)&val,
		sizeof(val), cfg->ioctl_buf, WLC_IOCTL_SMLEN, 0,
		&cfg->ioctl_buf_sync);
	if (ret < 0) {
		WL_ERR(("set 'mpc' failed, error = %d\n", ret));
		goto done;
	}

	/* Set interface up, explicitly. */
	val = 1;

	ret = wldev_ioctl(ndev, WLC_UP, (void *)&val, sizeof(val), true);
	if (ret < 0) {
		WL_ERR(("set interface up failed, error = %d\n", ret));
		goto done;
	}

	/* Stop all scan explicitly, till auto channel selection complete. */
	wl_set_drv_status(cfg, SCANNING, ndev);
	if (cfg->escan_info.ndev == NULL) {
		ret = BCME_OK;
		goto done;
	}
	ret = wl_notify_escan_complete(cfg, ndev, true, true);
	if (ret < 0) {
		WL_ERR(("set scan abort failed, error = %d\n", ret));
		goto done;
	}

done:
	return ret;
}

static bool
wl_cfg80211_valid_chanspec_p2p(chanspec_t chanspec)
{
	bool valid = false;

	/* channel 1 to 14 */
	if ((chanspec >= 0x2b01) && (chanspec <= 0x2b0e)) {
		valid = true;
	}
	/* channel 36 to 48 */
	else if ((chanspec >= 0x1b24) && (chanspec <= 0x1b30)) {
		valid = true;
	}
	/* channel 149 to 161 */
	else if ((chanspec >= 0x1b95) && (chanspec <= 0x1ba1)) {
		valid = true;
	}
	else {
		valid = false;
		WL_INFO(("invalid P2P chanspec, channel = %d, chanspec = %04x\n",
			CHSPEC_CHANNEL(chanspec), chanspec));
	}

	return valid;
}

static s32
wl_cfg80211_get_chanspecs_2g(struct net_device *ndev, void *buf, s32 buflen)
{
	s32 ret = BCME_ERROR;
	struct bcm_cfg80211 *cfg = NULL;
	wl_uint32_list_t *list = NULL;
	chanspec_t chanspec = 0;

	memset(buf, 0, buflen);

	cfg = g_bcm_cfg;
	list = (wl_uint32_list_t *)buf;
	list->count = htod32(WL_NUMCHANSPECS);

	/* Restrict channels to 2.4GHz, 20MHz BW, no SB. */
	chanspec |= (WL_CHANSPEC_BAND_2G | WL_CHANSPEC_BW_20 |
		WL_CHANSPEC_CTL_SB_NONE);
	chanspec = wl_chspec_host_to_driver(chanspec);

	ret = wldev_iovar_getbuf_bsscfg(ndev, "chanspecs", (void *)&chanspec,
		sizeof(chanspec), buf, buflen, 0, &cfg->ioctl_buf_sync);
	if (ret < 0) {
		WL_ERR(("get 'chanspecs' failed, error = %d\n", ret));
	}

	return ret;
}

static s32
wl_cfg80211_get_chanspecs_5g(struct net_device *ndev, void *buf, s32 buflen)
{
	u32 channel = 0;
	s32 ret = BCME_ERROR;
	s32 i = 0;
	s32 j = 0;
	struct bcm_cfg80211 *cfg = NULL;
	wl_uint32_list_t *list = NULL;
	chanspec_t chanspec = 0;

	memset(buf, 0, buflen);

	cfg = g_bcm_cfg;
	list = (wl_uint32_list_t *)buf;
	list->count = htod32(WL_NUMCHANSPECS);

	/* Restrict channels to 5GHz, 20MHz BW, no SB. */
	chanspec |= (WL_CHANSPEC_BAND_5G | WL_CHANSPEC_BW_20 |
		WL_CHANSPEC_CTL_SB_NONE);
	chanspec = wl_chspec_host_to_driver(chanspec);

	ret = wldev_iovar_getbuf_bsscfg(ndev, "chanspecs", (void *)&chanspec,
		sizeof(chanspec), buf, buflen, 0, &cfg->ioctl_buf_sync);
	if (ret < 0) {
		WL_ERR(("get 'chanspecs' failed, error = %d\n", ret));
		goto done;
	}

	/* Skip DFS and inavlid P2P channel. */
	for (i = 0, j = 0; i < dtoh32(list->count); i++) {
		chanspec = (chanspec_t) dtoh32(list->element[i]);
		channel = CHSPEC_CHANNEL(chanspec);

		ret = wldev_iovar_getint(ndev, "per_chan_info", &channel);
		if (ret < 0) {
			WL_ERR(("get 'per_chan_info' failed, error = %d\n", ret));
			goto done;
		}

		if (CHANNEL_IS_RADAR(channel) ||
			!(wl_cfg80211_valid_chanspec_p2p(chanspec))) {
			continue;
		} else {
			list->element[j] = list->element[i];
		}

		j++;
	}

	list->count = j;

done:
	return ret;
}

static s32
wl_cfg80211_get_best_channel(struct net_device *ndev, void *buf, int buflen,
	int *channel)
{
	s32 ret = BCME_ERROR;
	int chosen = 0;
	int retry = 0;

	/* Start auto channel selection scan. */
	ret = wldev_ioctl(ndev, WLC_START_CHANNEL_SEL, buf, buflen, true);
	if (ret < 0) {
		WL_ERR(("can't start auto channel scan, error = %d\n", ret));
		*channel = 0;
		goto done;
	}

	/* Wait for auto channel selection, worst case possible delay is 5250ms. */
	retry = CHAN_SEL_RETRY_COUNT;

	while (retry--) {
		OSL_SLEEP(CHAN_SEL_IOCTL_DELAY);

		ret = wldev_ioctl(ndev, WLC_GET_CHANNEL_SEL, &chosen, sizeof(chosen),
			false);
		if ((ret == 0) && (dtoh32(chosen) != 0)) {
			*channel = (u16)(chosen & 0x00FF);
			WL_INFO(("selected channel = %d\n", *channel));
			break;
		}
		WL_INFO(("attempt = %d, ret = %d, chosen = %d\n",
			(CHAN_SEL_RETRY_COUNT - retry), ret, dtoh32(chosen)));
	}

	if (retry <= 0)	{
		WL_ERR(("failure, auto channel selection timed out\n"));
		*channel = 0;
		ret = BCME_ERROR;
	}

done:
	return ret;
}

static s32
wl_cfg80211_restore_auto_channel_scan_state(struct net_device *ndev)
{
	u32 val = 0;
	s32 ret = BCME_ERROR;
	struct bcm_cfg80211 *cfg = g_bcm_cfg;

	/* Clear scan stop driver status. */
	wl_clr_drv_status(cfg, SCANNING, ndev);

	/* Enable mpc back to 1, irrespective of initial state. */
	val = 1;

	ret = wldev_iovar_setbuf_bsscfg(ndev, "mpc", (void *)&val,
		sizeof(val), cfg->ioctl_buf, WLC_IOCTL_SMLEN, 0,
		&cfg->ioctl_buf_sync);
	if (ret < 0) {
		WL_ERR(("set 'mpc' failed, error = %d\n", ret));
	}

	return ret;
}

s32
wl_cfg80211_get_best_channels(struct net_device *dev, char* cmd, int total_len)
{
	int channel = 0;
	s32 ret = BCME_ERROR;
	u8 *buf = NULL;
	char *pos = cmd;
	struct bcm_cfg80211 *cfg = NULL;
	struct net_device *ndev = NULL;

	memset(cmd, 0, total_len);

	buf = kmalloc(CHANSPEC_BUF_SIZE, GFP_KERNEL);
	if (buf == NULL) {
		WL_ERR(("failed to allocate chanspec buffer\n"));
		return -ENOMEM;
	}

	/*
	 * Always use primary interface, irrespective of interface on which
	 * command came.
	 */
	cfg = g_bcm_cfg;
	ndev = bcmcfg_to_prmry_ndev(cfg);

	/*
	 * Make sure that FW and driver are in right state to do auto channel
	 * selection scan.
	 */
	ret = wl_cfg80211_set_auto_channel_scan_state(ndev);
	if (ret < 0) {
		WL_ERR(("can't set auto channel scan state, error = %d\n", ret));
		goto done;
	}

	/* Best channel selection in 2.4GHz band. */
	ret = wl_cfg80211_get_chanspecs_2g(ndev, (void *)buf, CHANSPEC_BUF_SIZE);
	if (ret < 0) {
		WL_ERR(("can't get chanspecs in 2.4GHz, error = %d\n", ret));
		goto done;
	}

	ret = wl_cfg80211_get_best_channel(ndev, (void *)buf, CHANSPEC_BUF_SIZE,
		&channel);
	if (ret < 0) {
		WL_ERR(("can't select best channel scan in 2.4GHz, error = %d\n", ret));
		goto done;
	}

	if (CHANNEL_IS_2G(channel)) {
		channel = ieee80211_channel_to_frequency(channel, IEEE80211_BAND_2GHZ);
	} else {
		WL_ERR(("invalid 2.4GHz channel, channel = %d\n", channel));
		channel = 0;
	}

	sprintf(pos, "%04d ", channel);
	pos += 5;

	/* Best channel selection in 5GHz band. */
	ret = wl_cfg80211_get_chanspecs_5g(ndev, (void *)buf, CHANSPEC_BUF_SIZE);
	if (ret < 0) {
		WL_ERR(("can't get chanspecs in 5GHz, error = %d\n", ret));
		goto done;
	}

	ret = wl_cfg80211_get_best_channel(ndev, (void *)buf, CHANSPEC_BUF_SIZE,
		&channel);
	if (ret < 0) {
		WL_ERR(("can't select best channel scan in 5GHz, error = %d\n", ret));
		goto done;
	}

	if (CHANNEL_IS_5G(channel)) {
		channel = ieee80211_channel_to_frequency(channel, IEEE80211_BAND_5GHZ);
	} else {
		WL_ERR(("invalid 5GHz channel, channel = %d\n", channel));
		channel = 0;
	}

	sprintf(pos, "%04d ", channel);
	pos += 5;

	/* Set overall best channel same as 5GHz best channel. */
	sprintf(pos, "%04d ", channel);
	pos += 5;

done:
	if (NULL != buf) {
		kfree(buf);
	}

	/* Restore FW and driver back to normal state. */
	ret = wl_cfg80211_restore_auto_channel_scan_state(ndev);
	if (ret < 0) {
		WL_ERR(("can't restore auto channel scan state, error = %d\n", ret));
	}

	return (pos - cmd);
}
#endif /* WL_SUPPORT_AUTO_CHANNEL */

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
static const struct rfkill_ops wl_rfkill_ops = {
	.set_block = wl_rfkill_set
};

static int wl_rfkill_set(void *data, bool blocked)
{
<<<<<<< HEAD
	struct wl_priv *wl = (struct wl_priv *)data;
=======
	struct bcm_cfg80211 *cfg = (struct bcm_cfg80211 *)data;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	WL_DBG(("Enter \n"));
	WL_DBG(("RF %s\n", blocked ? "blocked" : "unblocked"));

<<<<<<< HEAD
	if (!wl)
		return -EINVAL;

	wl->rf_blocked = blocked;
=======
	if (!cfg)
		return -EINVAL;

	cfg->rf_blocked = blocked;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return 0;
}

<<<<<<< HEAD
static int wl_setup_rfkill(struct wl_priv *wl, bool setup)
=======
static int wl_setup_rfkill(struct bcm_cfg80211 *cfg, bool setup)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	s32 err = 0;

	WL_DBG(("Enter \n"));
<<<<<<< HEAD
	if (!wl)
		return -EINVAL;
	if (setup) {
		wl->rfkill = rfkill_alloc("brcmfmac-wifi",
			wl_cfg80211_get_parent_dev(),
			RFKILL_TYPE_WLAN, &wl_rfkill_ops, (void *)wl);

		if (!wl->rfkill) {
=======
	if (!cfg)
		return -EINVAL;
	if (setup) {
		cfg->rfkill = rfkill_alloc("brcmfmac-wifi",
			wl_cfg80211_get_parent_dev(),
			RFKILL_TYPE_WLAN, &wl_rfkill_ops, (void *)cfg);

		if (!cfg->rfkill) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			err = -ENOMEM;
			goto err_out;
		}

<<<<<<< HEAD
		err = rfkill_register(wl->rfkill);

		if (err)
			rfkill_destroy(wl->rfkill);
	} else {
		if (!wl->rfkill) {
=======
		err = rfkill_register(cfg->rfkill);

		if (err)
			rfkill_destroy(cfg->rfkill);
	} else {
		if (!cfg->rfkill) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
			err = -ENOMEM;
			goto err_out;
		}

<<<<<<< HEAD
		rfkill_unregister(wl->rfkill);
		rfkill_destroy(wl->rfkill);
=======
		rfkill_unregister(cfg->rfkill);
		rfkill_destroy(cfg->rfkill);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

err_out:
	return err;
}

<<<<<<< HEAD
=======
#ifdef DEBUGFS_CFG80211
/**
* Format : echo "SCAN:1 DBG:1" > /sys/kernel/debug/dhd/debug_level
* to turn on SCAN and DBG log.
* To turn off SCAN partially, echo "SCAN:0" > /sys/kernel/debug/dhd/debug_level
* To see current setting of debug level,
* cat /sys/kernel/debug/dhd/debug_level
*/
static ssize_t
wl_debuglevel_write(struct file *file, const char __user *userbuf,
	size_t count, loff_t *ppos)
{
	char tbuf[S_SUBLOGLEVEL * ARRAYSIZE(sublogname_map)], sublog[S_SUBLOGLEVEL];
	char *params, *token, *colon;
	uint i, tokens, log_on = 0;
	memset(tbuf, 0, sizeof(tbuf));
	memset(sublog, 0, sizeof(sublog));
	if (copy_from_user(&tbuf, userbuf, min_t(size_t, sizeof(tbuf), count)))
		return -EFAULT;

	params = &tbuf[0];
	colon = strchr(params, '\n');
	if (colon != NULL)
		*colon = '\0';
	while ((token = strsep(&params, " ")) != NULL) {
		memset(sublog, 0, sizeof(sublog));
		if (token == NULL || !*token)
			break;
		if (*token == '\0')
			continue;
		colon = strchr(token, ':');
		if (colon != NULL) {
			*colon = ' ';
		}
		tokens = sscanf(token, "%s %u", sublog, &log_on);
		if (colon != NULL)
			*colon = ':';

		if (tokens == 2) {
				for (i = 0; i < ARRAYSIZE(sublogname_map); i++) {
					if (!strncmp(sublog, sublogname_map[i].sublogname,
						strlen(sublogname_map[i].sublogname))) {
						if (log_on)
							wl_dbg_level |=
							(sublogname_map[i].log_level);
						else
							wl_dbg_level &=
							~(sublogname_map[i].log_level);
					}
				}
		} else
			WL_ERR(("%s: can't parse '%s' as a "
			       "SUBMODULE:LEVEL (%d tokens)\n",
			       tbuf, token, tokens));


	}
	return count;
}

static ssize_t
wl_debuglevel_read(struct file *file, char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char *param;
	char tbuf[S_SUBLOGLEVEL * ARRAYSIZE(sublogname_map)];
	uint i;
	memset(tbuf, 0, sizeof(tbuf));
	param = &tbuf[0];
	for (i = 0; i < ARRAYSIZE(sublogname_map); i++) {
		param += snprintf(param, sizeof(tbuf) - 1, "%s:%d ",
			sublogname_map[i].sublogname,
			(wl_dbg_level & sublogname_map[i].log_level) ? 1 : 0);
	}
	*param = '\n';
	return simple_read_from_buffer(user_buf, count, ppos, tbuf, strlen(&tbuf[0]));

}
static const struct file_operations fops_debuglevel = {
	.open = NULL,
	.write = wl_debuglevel_write,
	.read = wl_debuglevel_read,
	.owner = THIS_MODULE,
	.llseek = NULL,
};

static s32 wl_setup_debugfs(struct bcm_cfg80211 *cfg)
{
	s32 err = 0;
	struct dentry *_dentry;
	if (!cfg)
		return -EINVAL;
	cfg->debugfs = debugfs_create_dir(KBUILD_MODNAME, NULL);
	if (!cfg->debugfs || IS_ERR(cfg->debugfs)) {
		if (cfg->debugfs == ERR_PTR(-ENODEV))
			WL_ERR(("Debugfs is not enabled on this kernel\n"));
		else
			WL_ERR(("Can not create debugfs directory\n"));
		cfg->debugfs = NULL;
		goto exit;

	}
	_dentry = debugfs_create_file("debug_level", S_IRUSR | S_IWUSR,
		cfg->debugfs, cfg, &fops_debuglevel);
	if (!_dentry || IS_ERR(_dentry)) {
		WL_ERR(("failed to create debug_level debug file\n"));
		wl_free_debugfs(cfg);
	}
exit:
	return err;
}
static s32 wl_free_debugfs(struct bcm_cfg80211 *cfg)
{
	if (!cfg)
		return -EINVAL;
	if (cfg->debugfs)
		debugfs_remove_recursive(cfg->debugfs);
	cfg->debugfs = NULL;
	return 0;
}
#endif /* DEBUGFS_CFG80211 */

>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
struct device *wl_cfg80211_get_parent_dev(void)
{
	return cfg80211_parent_dev;
}

void wl_cfg80211_set_parent_dev(void *dev)
{
	cfg80211_parent_dev = dev;
}

static void wl_cfg80211_clear_parent_dev(void)
{
	cfg80211_parent_dev = NULL;
}

<<<<<<< HEAD
static void get_primary_mac(struct wl_priv *wl, struct ether_addr *mac)
{
	wldev_iovar_getbuf_bsscfg(wl_to_prmry_ndev(wl), "cur_etheraddr", NULL,
		0, wl->ioctl_buf, WLC_IOCTL_SMLEN, 0, &wl->ioctl_buf_sync);
	memcpy(mac->octet, wl->ioctl_buf, ETHER_ADDR_LEN);
=======
void get_primary_mac(struct bcm_cfg80211 *cfg, struct ether_addr *mac)
{
	wldev_iovar_getbuf_bsscfg(bcmcfg_to_prmry_ndev(cfg), "cur_etheraddr", NULL,
		0, cfg->ioctl_buf, WLC_IOCTL_SMLEN, 0, &cfg->ioctl_buf_sync);
	memcpy(mac->octet, cfg->ioctl_buf, ETHER_ADDR_LEN);
}
static bool check_dev_role_integrity(struct bcm_cfg80211 *cfg, u32 dev_role)
{
	dhd_pub_t *dhd = (dhd_pub_t *)(cfg->pub);
	if (((dev_role == NL80211_IFTYPE_AP) &&
		!(dhd->op_mode & DHD_FLAG_HOSTAP_MODE)) ||
		((dev_role == NL80211_IFTYPE_P2P_GO) &&
		!(dhd->op_mode & DHD_FLAG_P2P_GO_MODE)))
	{
		WL_ERR(("device role select failed\n"));
		return false;
	}
	return true;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
}

int wl_cfg80211_do_driver_init(struct net_device *net)
{
<<<<<<< HEAD
	struct wl_priv *wl = *(struct wl_priv **)netdev_priv(net);

	if (!wl || !wl->wdev)
		return -EINVAL;

	if (dhd_do_driver_init(wl->wdev->netdev) < 0)
=======
	struct bcm_cfg80211 *cfg = *(struct bcm_cfg80211 **)netdev_priv(net);

	if (!cfg || !cfg->wdev)
		return -EINVAL;

	if (dhd_do_driver_init(cfg->wdev->netdev) < 0)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		return -1;

	return 0;
}

void wl_cfg80211_enable_trace(bool set, u32 level)
{
	if (set)
		wl_dbg_level = level & WL_DBG_LEVEL;
	else
		wl_dbg_level |= (WL_DBG_LEVEL & level);
}
#if defined(WL_SUPPORT_BACKPORTED_KPATCHES) || (LINUX_VERSION_CODE >= KERNEL_VERSION(3, \
	2, 0))
static s32
wl_cfg80211_mgmt_tx_cancel_wait(struct wiphy *wiphy,
<<<<<<< HEAD
	struct net_device *dev, u64 cookie)
=======
	bcm_struct_cfgdev *cfgdev, u64 cookie)
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
{
	/* CFG80211 checks for tx_cancel_wait callback when ATTR_DURATION
	 * is passed with CMD_FRAME. This callback is supposed to cancel
	 * the OFFCHANNEL Wait. Since we are already taking care of that
	 *  with the tx_mgmt logic, do nothing here.
	 */

	return 0;
}
#endif /* WL_SUPPORT_BACKPORTED_PATCHES || KERNEL >= 3.2.0 */

#ifdef WL11U
bcm_tlv_t *
wl_cfg80211_find_interworking_ie(u8 *parse, u32 len)
{
	bcm_tlv_t *ie;

	while ((ie = bcm_parse_tlvs(parse, (u32)len, DOT11_MNG_INTERWORKING_ID))) {
			return (bcm_tlv_t *)ie;
	}
	return NULL;
}


static s32
<<<<<<< HEAD
wl_cfg80211_add_iw_ie(struct wl_priv *wl, struct net_device *ndev, s32 bssidx, s32 pktflag,
=======
wl_cfg80211_add_iw_ie(struct bcm_cfg80211 *cfg, struct net_device *ndev, s32 bssidx, s32 pktflag,
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
            uint8 ie_id, uint8 *data, uint8 data_len)
{
	s32 err = BCME_OK;
	s32 buf_len;
	s32 iecount;
	ie_setbuf_t *ie_setbuf;

	if (ie_id != DOT11_MNG_INTERWORKING_ID)
		return BCME_UNSUPPORTED;

	/* Validate the pktflag parameter */
	if ((pktflag & ~(VNDR_IE_BEACON_FLAG | VNDR_IE_PRBRSP_FLAG |
	            VNDR_IE_ASSOCRSP_FLAG | VNDR_IE_AUTHRSP_FLAG |
	            VNDR_IE_PRBREQ_FLAG | VNDR_IE_ASSOCREQ_FLAG|
	            VNDR_IE_CUSTOM_FLAG))) {
		WL_ERR(("cfg80211 Add IE: Invalid packet flag 0x%x\n", pktflag));
		return -1;
	}

	/* use VNDR_IE_CUSTOM_FLAG flags for none vendor IE . currently fixed value */
	pktflag = htod32(pktflag);

	buf_len = sizeof(ie_setbuf_t) + data_len - 1;
	ie_setbuf = (ie_setbuf_t *) kzalloc(buf_len, GFP_KERNEL);

	if (!ie_setbuf) {
		WL_ERR(("Error allocating buffer for IE\n"));
		return -ENOMEM;
	}

<<<<<<< HEAD
	if (wl->iw_ie_len == data_len && !memcmp(wl->iw_ie, data, data_len)) {
		WL_ERR(("Previous IW IE is equals to current IE\n"));
		return err;
=======
	if (cfg->iw_ie_len == data_len && !memcmp(cfg->iw_ie, data, data_len)) {
		WL_ERR(("Previous IW IE is equals to current IE\n"));
		err = BCME_OK;
		goto exit;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

	strncpy(ie_setbuf->cmd, "add", VNDR_IE_CMD_LEN - 1);
	ie_setbuf->cmd[VNDR_IE_CMD_LEN - 1] = '\0';

	/* Buffer contains only 1 IE */
	iecount = htod32(1);
	memcpy((void *)&ie_setbuf->ie_buffer.iecount, &iecount, sizeof(int));
	memcpy((void *)&ie_setbuf->ie_buffer.ie_list[0].pktflag, &pktflag, sizeof(uint32));

	/* Now, add the IE to the buffer */
	ie_setbuf->ie_buffer.ie_list[0].ie_data.id = ie_id;

	/* if already set with previous values, delete it first */
<<<<<<< HEAD
	if (wl->iw_ie_len != 0) {
=======
	if (cfg->iw_ie_len != 0) {
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
		WL_DBG(("Different IW_IE was already set. clear first\n"));

		ie_setbuf->ie_buffer.ie_list[0].ie_data.len = 0;

		err = wldev_iovar_setbuf_bsscfg(ndev, "ie", ie_setbuf, buf_len,
<<<<<<< HEAD
			wl->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &wl->ioctl_buf_sync);

		if (err != BCME_OK)
			return err;
=======
			cfg->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);

		if (err != BCME_OK)
			goto exit;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	}

	ie_setbuf->ie_buffer.ie_list[0].ie_data.len = data_len;
	memcpy((uchar *)&ie_setbuf->ie_buffer.ie_list[0].ie_data.data[0], data, data_len);

	err = wldev_iovar_setbuf_bsscfg(ndev, "ie", ie_setbuf, buf_len,
<<<<<<< HEAD
		wl->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &wl->ioctl_buf_sync);

	if (err == BCME_OK) {
		memcpy(wl->iw_ie, data, data_len);
		wl->iw_ie_len = data_len;
		wl->wl11u = TRUE;
=======
		cfg->ioctl_buf, WLC_IOCTL_MAXLEN, bssidx, &cfg->ioctl_buf_sync);

	if (err == BCME_OK) {
		memcpy(cfg->iw_ie, data, data_len);
		cfg->iw_ie_len = data_len;
		cfg->wl11u = TRUE;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

		err = wldev_iovar_setint_bsscfg(ndev, "grat_arp", 1, bssidx);
	}

<<<<<<< HEAD
	kfree(ie_setbuf);
=======
exit:
	if (ie_setbuf)
		kfree(ie_setbuf);
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	return err;
}
#endif /* WL11U */

#ifdef WL_HOST_BAND_MGMT
s32
wl_cfg80211_set_band(struct net_device *ndev, int band)
{
<<<<<<< HEAD
	struct wl_priv *wl = wlcfg_drv_priv;
=======
	struct bcm_cfg80211 *cfg = g_bcm_cfg;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
	int ret = 0;
	char ioctl_buf[50];

	if ((band < WLC_BAND_AUTO) || (band > WLC_BAND_2G)) {
		WL_ERR(("Invalid band\n"));
		return -EINVAL;
	}

	if ((ret = wldev_iovar_setbuf(ndev, "roam_band", &band,
		sizeof(int), ioctl_buf, sizeof(ioctl_buf), NULL)) < 0) {
		WL_ERR(("seting roam_band failed code=%d\n", ret));
		return ret;
	}

	WL_DBG(("Setting band to %d\n", band));
<<<<<<< HEAD
	wl->curr_band = band;
=======
	cfg->curr_band = band;
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)

	return 0;
}
#endif /* WL_HOST_BAND_MGMT */

<<<<<<< HEAD
#if defined(DHCP_SCAN_SUPPRESS)
static void wl_cfg80211_scan_supp_timerfunc(ulong data)
{
	struct wl_priv *wl = (struct wl_priv *)data;

	WL_DBG(("Enter \n"));
	schedule_work(&wl->wlan_work);
}

static void wl_cfg80211_work_handler(struct work_struct *work)
{
	struct wl_priv *wl = wlcfg_drv_priv;

	wl = container_of(work, struct wl_priv, wlan_work);

	if (!wl) {
		WL_ERR(("wl_priv ptr NULL\n"));
		return;
	}

	if (wl->scan_suppressed) {
		/* There is pending scan_suppress. Clean it */
		WL_ERR(("Clean up from timer after %d msec\n", WL_SCAN_SUPPRESS_TIMEOUT));
		wl_cfg80211_scan_suppress(wl_to_prmry_ndev(wl), 0);
	}
}

int wl_cfg80211_scan_suppress(struct net_device *dev, int suppress)
{
	struct wl_priv *wl = wlcfg_drv_priv;
	int ret = 0;

	if (!dev || !wl || ((suppress != 0) && (suppress != 1)))
		return -EINVAL;

	if (suppress == wl->scan_suppressed) {
		WL_DBG(("No change in scan_suppress state. Ignoring cmd..\n"));
		return 0;
	}

	if (timer_pending(&wl->scan_supp_timer))
		del_timer_sync(&wl->scan_supp_timer);

	if ((ret = wldev_ioctl(dev, WLC_SET_SCANSUPPRESS,
		&suppress, sizeof(int), true)) < 0) {
		WL_ERR(("Scan suppress setting failed ret:%d \n", ret));
	} else {
		WL_DBG(("Scan suppress %s \n", suppress ? "Enabled" : "Disabled"));
		wl->scan_suppressed = suppress;
	}

	/* If scan_suppress is set, Start a timer to monitor it (just incase) */
	if (wl->scan_suppressed) {
		if (ret) {
			WL_ERR(("Retry scan_suppress reset at a later time \n"));
			mod_timer(&wl->scan_supp_timer,
				jiffies + msecs_to_jiffies(WL_SCAN_SUPPRESS_RETRY));
		} else {
			WL_DBG(("Start wlan_timer to clear of scan_suppress \n"));
			mod_timer(&wl->scan_supp_timer,
				jiffies + msecs_to_jiffies(WL_SCAN_SUPPRESS_TIMEOUT));
		}
	}

	return ret;
}
#endif /* DHCP_SCAN_SUPPRESS */
=======

int wl_cfg80211_scan_stop(bcm_struct_cfgdev *cfgdev)
{
	struct bcm_cfg80211 *cfg = NULL;
	struct net_device *ndev = NULL;
	unsigned long flags;
	int clear_flag = 0;
	int ret = 0;

	WL_TRACE(("Enter\n"));

	cfg = g_bcm_cfg;
	if (!cfg)
		return -EINVAL;

	ndev = cfgdev_to_wlc_ndev(cfgdev, cfg);

	spin_lock_irqsave(&cfg->cfgdrv_lock, flags);
#ifdef WL_CFG80211_P2P_DEV_IF
	if (cfg->scan_request && cfg->scan_request->wdev == cfgdev) {
#else
	if (cfg->scan_request && cfg->scan_request->dev == cfgdev) {
#endif
		cfg80211_scan_done(cfg->scan_request, true);
		cfg->scan_request = NULL;
		clear_flag = 1;
	}
	spin_unlock_irqrestore(&cfg->cfgdrv_lock, flags);

	if (clear_flag)
		wl_clr_drv_status(cfg, SCANNING, ndev);

	return ret;
}

bool wl_cfg80211_is_vsdb_mode(void)
{
	return (g_bcm_cfg && g_bcm_cfg->vsdb_mode);
}

void* wl_cfg80211_get_dhdp()
{
	struct bcm_cfg80211 *cfg = g_bcm_cfg;

	return cfg->pub;
}

bool wl_cfg80211_is_p2p_active(void)
{
	return (g_bcm_cfg && g_bcm_cfg->p2p);
}

static void wl_cfg80211_work_handler(struct work_struct * work)
{
	struct bcm_cfg80211 *cfg = NULL;
	struct net_info *iter, *next;
	s32 err = BCME_OK;
	s32 pm = PM_FAST;

	cfg = container_of(work, struct bcm_cfg80211, pm_enable_work.work);
	WL_DBG(("Enter \n"));
	if (cfg->pm_enable_work_on) {
		cfg->pm_enable_work_on = false;
		for_each_ndev(cfg, iter, next) {
			if (!wl_get_drv_status(cfg, CONNECTED, iter->ndev) ||
				(wl_get_mode_by_netdev(cfg, iter->ndev) != WL_MODE_BSS))
				continue;
			if (iter->ndev) {
				if ((err = wldev_ioctl(iter->ndev, WLC_SET_PM,
					&pm, sizeof(pm), true)) != 0) {
					if (err == -ENODEV)
						WL_DBG(("%s:netdev not ready\n", iter->ndev->name));
					else
						WL_ERR(("%s:error (%d)\n", iter->ndev->name, err));
				} else
					wl_cfg80211_update_power_mode(iter->ndev);
			}
		}
#ifdef CUSTOMER_HW4
		DHD_OS_WAKE_UNLOCK(cfg->pub);
#endif /* CUSTOMER_HW4 */
	}
}

u8
wl_get_action_category(void *frame, u32 frame_len)
{
	u8 category;
	u8 *ptr = (u8 *)frame;
	if (frame == NULL)
		return DOT11_ACTION_CAT_ERR_MASK;
	if (frame_len < DOT11_ACTION_HDR_LEN)
		return DOT11_ACTION_CAT_ERR_MASK;
	category = ptr[DOT11_ACTION_CAT_OFF];
	WL_INFO(("Action Category: %d\n", category));
	return category;
}

int
wl_get_public_action(void *frame, u32 frame_len, u8 *ret_action)
{
	u8 *ptr = (u8 *)frame;
	if (frame == NULL || ret_action == NULL)
		return BCME_ERROR;
	if (frame_len < DOT11_ACTION_HDR_LEN)
		return BCME_ERROR;
	if (DOT11_ACTION_CAT_PUBLIC != wl_get_action_category(frame, frame_len))
		return BCME_ERROR;
	*ret_action = ptr[DOT11_ACTION_ACT_OFF];
	WL_INFO(("Public Action : %d\n", *ret_action));
	return BCME_OK;
}

static int
wl_cfg80211_delayed_roam(struct bcm_cfg80211 *cfg, struct net_device *ndev,
	const struct ether_addr *bssid)
{
	s32 err;
	wl_event_msg_t e;

	bzero(&e, sizeof(e));
	e.event_type = cpu_to_be32(WLC_E_ROAM);
	memcpy(&e.addr, bssid, ETHER_ADDR_LEN);
	/* trigger the roam event handler */
	err = wl_notify_roaming_status(cfg, ndev_to_cfgdev(ndev), &e, NULL);

	return err;
}

#ifdef WLFBT
void
wl_cfg80211_get_fbt_key(uint8 *key)
{
	memcpy(key, g_bcm_cfg->fbt_key, FBT_KEYLEN);
}
#endif /* WLFBT */
>>>>>>> 90123ab... Update Wi-Fi drivers to 1.141.44 (coming from N5100 kernel drop)
