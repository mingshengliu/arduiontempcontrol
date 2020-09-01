/* SYS_WKUP */
#ifdef PWR_WAKEUP_PIN1
  SYS_WKUP1 = PA_0,
#endif
#ifdef PWR_WAKEUP_PIN2
  SYS_WKUP2 = PA_2,
#endif
#ifdef PWR_WAKEUP_PIN3
  SYS_WKUP3 = PC_1,
#endif
#ifdef PWR_WAKEUP_PIN4
  SYS_WKUP4 = PC_13,
#endif
#ifdef PWR_WAKEUP_PIN5
  SYS_WKUP5 = NC,
#endif
#ifdef PWR_WAKEUP_PIN6
  SYS_WKUP6 = PI_11,
#endif
#ifdef PWR_WAKEUP_PIN7
  SYS_WKUP7 = NC,
#endif
#ifdef PWR_WAKEUP_PIN8
  SYS_WKUP8 = NC,
#endif
/* USB */
#ifdef USBCON
  USB_OTG_FS_SOF = PA_8,
  USB_OTG_FS_VBUS = PA_9,
  USB_OTG_FS_ID = PA_10,
  USB_OTG_FS_DM = PA_11,
  USB_OTG_FS_DP = PA_12,
  USB_OTG_HS_ULPI_D0 = PA_3,
  USB_OTG_HS_SOF = PA_4,
  USB_OTG_HS_ULPI_CK = PA_5,
  USB_OTG_HS_ULPI_D1 = PB_0,
  USB_OTG_HS_ULPI_D2 = PB_1,
  USB_OTG_HS_ULPI_D7 = PB_5,
  USB_OTG_HS_ULPI_D3 = PB_10,
  USB_OTG_HS_ULPI_D4 = PB_11,
  USB_OTG_HS_ID = PB_12,
  USB_OTG_HS_ULPI_D5 = PB_12,
  USB_OTG_HS_ULPI_D6 = PB_13,
  USB_OTG_HS_VBUS = PB_13,
  USB_OTG_HS_DM = PB_14,
  USB_OTG_HS_DP = PB_15,
  USB_OTG_HS_ULPI_STP = PC_0,
  USB_OTG_HS_ULPI_NXT = PH_4,
  USB_OTG_HS_ULPI_DIR = PI_11,
#endif
