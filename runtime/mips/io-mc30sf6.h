#ifndef _IO_MC30SF6_H
#define _IO_MC30SF6_H

#define MC_BASE_ADDR    0xB82F0000
#define MC_R(a)     *(volatile unsigned*)(MC_BASE_ADDR + (a))
#define MC_A(a)     (MC_BASE_ADDR + (a))

/*
 * Регистры DMA MEM(0-3)
 */
#define MC_CSR_MEM(n)   MC_R (0x0000+(n<<7))    /* Управление и состояние */
#define MC_CP_MEM(n)    MC_R (0x0004+(n<<7))    /* Указатель цепочки */
#define MC_IR0_MEM(n)   MC_R (0x0008+(n<<7))    /* Индекс "0" внешней памяти */
#define MC_IR1_MEM(n)   MC_R (0x000C+(n<<7))    /* Индекс "1" внешней памяти */
#define MC_OR_MEM(n)    MC_R (0x0010+(n<<7))    /* Смещение внешней памяти */
#define MC_Y_MEM(n)     MC_R (0x0014+(n<<7))    /* Параметры направления Y при
                                                 * двухмерной адресации */
#define MC_RUN_MEM(n)   MC_R (0x0018+(n<<7))    /* Управление состоянием бита RUN */

/*
 * Регистры контроллера SPI
 */
#define MC_TX_SPI       MC_R (0x6000)           /* Буфер передачи данных */
#define MC_RX_SPI       MC_R (0x6004)           /* Буфер приёма данных */
#define MC_CSR_SPI      MC_R (0x6008)           /* Регистр управления и состояния */
#define MC_DIR_SPI      MC_R (0x600C)           /* Регистр управления направлением выводов */
#define MC_TCTR         MC_R (0x6010)           /* Регистр управления передатчиком */
#define MC_RCTR         MC_R (0x6014)           /* Регистр управления приёмником */
#define MC_TSR          MC_R (0x6018)           /* Регистр состояния передатчика */
#define MC_RSR          MC_R (0x601C)           /* Регистр состояния приёмника */
#define MC_TCTR_RATE    MC_R (0x6020)           /* Регистр управления темпом передачи данных */
#define MC_RCTR_RATE    MC_R (0x6024)           /* Регистр управления темпом приёма данных */
#define MC_TSTART       MC_R (0x6028)           /* Запуск/останов передатчика */
#define MC_RSTART       MC_R (0x602C)           /* Запуск/останов приемника */
#define MC_EMERG        MC_R (0x6030)           /* Регистр аварийного управления портом */
#define MC_IMASK        MC_R (0x6034)           /* Регистр маски прерываний от порта */

/*
 * Регистры DMA EMAC(0-1): 0 - RX, 1 - TX
 */
#define MC_CSR_EMAC(n)  MC_R (0xE800+(n<<6))    /* Управление и состояние */
#define MC_CP_EMAC(n)   MC_R (0xE804+(n<<6))    /* Указатель цепочки */
#define MC_IR_EMAC(n)   MC_R (0xE808+(n<<6))    /* Индекс */
#define MC_RUN_EMAC(n)  MC_R (0xE80C+(n<<6))    /* Управление состоянием бита RUN */

/*
* Регистры DMA MFBSP(0-3)
*/
#define MC_CSR_MFBSP_RX(n)	MC_R (0x7800+(n<<6))	/* Управление и состояние */
#define MC_CP_MFBSP_RX(n)	MC_R (0x7804+(n<<6))	/* Указатель цепочки */
#define MC_IR_MFBSP_RX(n)	MC_R (0x7808+(n<<6))	/* Индекс */
#define MC_RUN_MFBSP_RX(n)	MC_R (0x780C+(n<<6))	/* Управления состоянием бита RUN */

#define MC_CSR_MFBSP_TX(n)	MC_R (0x7C00+(n<<6))	/* Управление и состояние */
#define MC_CP_MFBSP_TX(n)	MC_R (0x7C04+(n<<6))	/* Указатель цепочки */
#define MC_IR_MFBSP_TX(n)	MC_R (0x7C08+(n<<6))	/* Индекс */
#define MC_RUN_MFBSP_TX(n)	MC_R (0x7C0C+(n<<6))	/* Управления состоянием бита RUN */

/*
 * Регистры USBIC
 */
#define MC_USB_CSR          MC_R (0x2000)    /* Регистр управления и статуса контроллера */
#define MC_USB_INT_CSR      MC_R (0x2004)    /* Регистр управления и статуса прерываний */
#define MC_USB_VENDOR_DATA  MC_R (0x200C)    /* Данные для передачи по Vendor-каналу */
#define MC_USB_VENDOR_INDEX MC_R (0x2010)    /* Указатель на данные по Vendor-каналу */
#define MC_USB_VENDOR_VALUE MC_R (0x2014)    /* Принятые данные по Vendor-каналу */
#define MC_USB_CFG_ADDR     MC_R (0x2018)    /* Регистр адреса массива конфигурации */
#define MC_USB_CFG_DATA     MC_R (0x201C)    /* Регистр данных массива конфигурации */
#define MC_USB_REVISION     MC_R (0x2020)    /* Номер ревизии */
#define MC_USB_CSR_EP1      MC_R (0x2044)    /* Регистр управления и статуса ЕР1 */
#define MC_USB_CSR_EP2      MC_R (0x204C)    /* Регистр управления и статуса ЕР2 */
#define MC_USB_CSR_EP3      MC_R (0x2054)    /* Регистр управления и статуса ЕР3 */
#define MC_USB_CSR_EP4      MC_R (0x205C)    /* Регистр управления и статуса ЕР4 */

/*
 * Регистры DMA USBIC
 */
#define MC_CSR_USB_EP1_RX   MC_R (0x2800)    /* Регистр управления и состояния */
#define MC_CP_USB_EP1_RX    MC_R (0x2804)    /* Регистр указателя цепочки */
#define MC_IR_USB_EP1_RX    MC_R (0x2808)    /* Регистр индекса */
#define MC_RUN_USB_EP1_RX   MC_R (0x280C)    /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_USB_EP2_TX   MC_R (0x2840)    /* Регистр управления и состояния */
#define MC_CP_USB_EP2_TX    MC_R (0x2844)    /* Регистр указателя цепочки */
#define MC_IR_USB_EP2_TX    MC_R (0x2848)    /* Регистр индекса */
#define MC_RUN_USB_EP2_TX   MC_R (0x284C)    /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_USB_EP3_RX   MC_R (0x2880)    /* Регистр управления и состояния */
#define MC_CP_USB_EP3_RX    MC_R (0x2884)    /* Регистр указателя цепочки */
#define MC_IR_USB_EP3_RX    MC_R (0x2888)    /* Регистр индекса */
#define MC_RUN_USB_EP3_RX   MC_R (0x288C)    /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_USB_EP4_TX   MC_R (0x28C0)    /* Регистр управления и состояния */
#define MC_CP_USB_EP4_TX    MC_R (0x28C4)    /* Регистр указателя цепочки */
#define MC_IR_USB_EP4_TX    MC_R (0x28C8)    /* Регистр индекса */
#define MC_RUN_USB_EP4_TX   MC_R (0x28CC)    /* Псевдорегистр управления состоянием бита RUN */

/*
 * Регистры SPFMIC(0-1)
 */
#define MC_SPFMIC_ID_VER(n)         MC_R (0xC000 + ((n) << 12)) /* Регистр аппаратной версии контроллера */
#define MC_SPFMIC_MODE_R(n)         MC_R (0xC004 + ((n) << 12)) /* Регистр управления режимом работы */
#define MC_SPFMIC_STATE_R(n)        MC_R (0xC008 + ((n) << 12)) /* Регистр состояния блока */
#define MC_SPFMIC_RISC_IRQ_MASK(n)  MC_R (0xC00C + ((n) << 12)) /* Регистр глобальных масок прерываний */
#define MC_SPFMIC_CCODE_OUT(n)      MC_R (0xC010 + ((n) << 12)) /* Регистр управляющего кода для отправки */
#define MC_SPFMIC_CUR_TIME(n)       MC_R (0xC014 + ((n) << 12)) /* Регистр маркера времени из сети */
#define MC_SPFMIC_ISR_L(n)          MC_R (0xC018 + ((n) << 12)) /* Регистр кодов распределенных прерываний (младшая часть) */
#define MC_SPFMIC_ISR_H(n)          MC_R (0xC01C + ((n) << 12)) /* Регистр кодов распределенных прерываний (старшая часть) */
#define MC_SPFMIC_INTR_IRQ_MASK_L(n)  MC_R (0xC020 + ((n) << 12)) /* Регистр маски прерывания при приеме кода распределенного прерывания (младшая половина) */
#define MC_SPFMIC_INTR_IRQ_MASK_H(n)  MC_R (0xC024 + ((n) << 12)) /* Регистр маски прерывания при приеме кода распределенного прерывания (старшая половина) */
#define MC_SPFMIC_INTA_IRQ_MASK_L(n)  MC_R (0xC028 + ((n) << 12)) /* Регистр маски прерывания при приеме кода подтверждения (младшая половина) */
#define MC_SPFMIC_INTA_IRQ_MASK_H(n)  MC_R (0xC02C + ((n) << 12)) /* Регистр маски прерывания при приеме кода подтверждения (старшая половина) */
#define MC_SPFMIC_CCODES_MASK(n)    MC_R (0xC030 + ((n) << 12)) /* Регистр маски входных и выходных портов для управляющих кодов */
#define MC_SPFMIC_DIST_INTS_TOUTS1(n) MC_R (0xC034 + ((n) << 12)) /* Регистр таймаутов распределенных прерываний 1 */
#define MC_SPFMIC_DIST_INTS_TOUTS2(n) MC_R (0xC038 + ((n) << 12)) /* Регистр таймаутов распределенных прерываний 2 */
#define MC_SPFMIC_ACK_NON_ACK_REGIME(n) MC_R (0xC03C + ((n) << 12)) /* Регистр флагов режима обработки распределенных прерываний */
#define MC_SPFMIC_CCODES_SPEC_REGIME(n) MC_R (0xC040 + ((n) << 12)) /* Регистр специального режима работы с управляющими кодами */
#define MC_SPFMIC_SPEC_ISR_REGIME(n)    MC_R (0xC044 + ((n) << 12)) /* Регистр флагов автоматической отправки подтверждения */
#define MC_SPFMIC_INTER_HANDLER_TERM_FUNCT(n)  MC_R (0xC048 + ((n) << 12)) /* Флаги-признаки обработчика для распределенных прерываний */
#define MC_SPFMIC_ISR_SOURCE_TERM_FUNCT(n)     MC_R (0xC04C + ((n) << 12)) /* Флаги-признаки источника для распределенных прерываний */
#define MC_SPFMIC_ISR_TOUTS_FLS_L(n)  MC_R (0xC050 + ((n) << 12)) /* Младшая половина регистра флагов таймаутов */
#define MC_SPFMIC_ISR_TOUTS_FLS_H(n)  MC_R (0xC054 + ((n) << 12)) /* Старшая половина регистра флагов таймаутов */
#define MC_SPFMIC_ISR_1101(n)         MC_R (0xC058 + ((n) << 12)) /* Значение регистра ISR для управляющих кодов, назначение которых не определено стандартом */
#define MC_SPFMIC_EXTERNAL_RESET_PARAMETERS(n) MC_R (0xC05C + ((n) << 12)) /* Регистр параметров удаленного сброса */
#define MC_SPFMIC_PORT_CCODES_MODE(n) MC_R (0xC060 + ((n) << 12)) /* Регистр режима обработки кодов распределенных прерываний порта - использутся и в режиме SpF и в режиме gigaSpW */
#define MC_SPFMIC_BROADCAST_MODE(n)   MC_R (0xC064 + ((n) << 12)) /* режим передачи BROADCAST */
#define MC_SPFMIC_BROADCAST_NUMS(n)   MC_R (0xC068 + ((n) << 12)) /* Номера виртуальных каналов BROADCAST, обрабатываемые данным блоком SpFmIC4 */
#define MC_SPFMIC_BROADCAST_DATA_OUT_L(n)   MC_R (0xC06C + ((n) << 12)) /* регистр младшей части поля данных отправляемого BROADCAST */
#define MC_SPFMIC_BROADCAST_DATA_OUT_H(n)   MC_R (0xC070 + ((n) << 12)) /* регистр младшей части поля данных отправляемого BROADCAST */
#define MC_SPFMIC_BROADCAST_DATA_OUT_PARAMETERS(n)  MC_R (0xC074 + ((n) << 12)) /* регистр параметров отправляемого BROADCAST */
#define MC_SPFMIC_BROADCAST_DATA_IN_L(n)    MC_R (0xC078 + ((n) << 12)) /* регистр младшей части поля данных принятого BROADCAST */
#define MC_SPFMIC_BROADCAST_DATA_IN_H(n)    MC_R (0xC07C + ((n) << 12)) /* регистр старшей части части поля данных принятого BROADCAST */
#define MC_SPFMIC_BROADCAST_DATA_IN_PARAMETERS(n)   MC_R (0xC080 + ((n) << 12)) /* регистр параметров принятых BROADCAST */
#define MC_SPFMIC_BROADCAST_IN_STATUS1(n)   MC_R (0xC084 + ((n) << 12)) /* регистр флагов принятых корректных BROADCAST */
#define MC_SPFMIC_BROADCAST_IN_STATUS_ALL1(n)    MC_R (0xC088 + ((n) << 12)) /* флагов принятых BROADCAST (корректных и не корректных) */
#define MC_SPFMIC_TIME_SLOTS_PARAMETERS(n)       MC_R (0xC08C + ((n) << 12)) /* регистр параметров таймслотов для класса сервиса Scheduling */
#define MC_SPFMIC_TIME_SLOTS_SETTINGS(n)         MC_R (0xC090 + ((n) << 12)) /* настройки таймслота */
#define MC_SPFMIC_PMA_STATUS(n)             MC_R (0xC094 + ((n) << 12)) /* Регистр состояния PMA - используется и в режиме порта SpF и в режиме gigaSpW */
#define MC_SPFMIC_PMA_MODE(n)               MC_R (0xC098 + ((n) << 12)) /* Регистр режима работы PMA - используется и в режиме порта SpF и в режиме gigaSpW */
#define MC_SPFMIC_GIGA_SPW_STATUS(n)        MC_R (0xC09C + ((n) << 12)) /* Регистр состояния порта gigaSpW */
#define MC_SPFMIC_GIGA_SPW_MODE(n)          MC_R (0xC0A0 + ((n) << 12)) /* Регистр режима работы порта gigaSpW */
#define MC_SPFMIC_GIGA_SPW_TRANSMISSION_PARAMETERS(n)  MC_R (0xC0A4 + ((n) << 12)) /* Регистр параметров передачи порта gigaSpW */
#define MC_SPFMIC_GIGA_SPW_RX_SPEED(n)      MC_R (0xC0A8 + ((n) << 12)) /* Регистр скорости приема порта gigaSpW */
#define MC_SPFMIC_SpF_PORT_MODE_VC_PARAMS(n)(x)    MC_R (0xC0AC + (x)*4 + ((n) << 12)) /* Регистр режима порта SpF VC_PARAMS */
#define MC_SPFMIC_SpF_PORT_MODE_VC_TSLOTS_L(n)(x)  MC_R (0xC0BC + (x)*4 + ((n) << 12)) /* Регистр флагов таймслотов (младшая часть) */
#define MC_SPFMIC_SpF_PORT_MODE_VC_TSLOTS_H(n)(x)  MC_R (0xC0CC + (x)*4 + ((n) << 12)) /* Регистр флагов таймслотов (старшая часть) */
#define MC_SPFMIC_SpF_PORT_MODE_VC_FR_LL(n)        MC_R (0xC0DC + ((n) << 12)) /* Регистр режима порта SpF VC framing lane */
#define MC_SPFMIC_SpF_PORT_MODE_LANE_CONSTANTS(n)  MC_R (0xC0E0 + ((n) << 12)) /*  */
#define MC_SPFMIC_SpF_PORT_STATUS_VC1(n)    MC_R (0xC0E4 + ((n) << 12)) /* Регистр состояния порта SpF Virtual channels 1 */
#define MC_SPFMIC_SpF_PORT_STATUS_VC2(n)    MC_R (0xC0E8 + ((n) << 12)) /* Регистр состояния порта SpF Virtual channels 2 and retry */
#define MC_SPFMIC_SpF_PORT_CUR_CREDIT_VALUE(n)     MC_R (0xC0EC + ((n) << 12)) /* Регистр состояния - значение cur_credit_counter порта SpF */
#define MC_SPFMIC_SpF_PORT_STATUS_LL_EL(n)         MC_R (0xC0F0 + ((n) << 12)) /* Регистр состояния порта SpF Lane Encoding */
#define MC_SPFMIC_CONF_PORT_MODE_VC_PARAMS(n)(x)   MC_R (0xC0F4 + (x)*4 + ((n) << 12)) /* Регистр режима виртуальных каналов конфигурационного порта VC_PARAMS */
#define MC_SPFMIC_CONF_PORT_CUR_CREDIT_VALUE(n)    MC_R (0xC104 + ((n) << 12)) /* Регистр состояния - значение cur_credit_counter конфигурационного порта */

/*
 * Регистры DMA SPFMIC(0-1)
 */
#define MC_CSR_SPFMIC_RX_DES(n)   MC_R (0xC800 + ((n) << 12)) /* Регистр управления и состояния канала */
#define MC_CP_SPFMIC_RX_DES(n)    MC_R (0xC804 + ((n) << 12)) /* Регистр указателя цепочки канала */
#define MC_IR_SPFMIC_RX_DES(n)    MC_R (0xC808 + ((n) << 12)) /* Индексный регистр внешней памяти канала */
#define MC_RUN_SPFMIC_RX_DES(n)   MC_R (0xC80C + ((n) << 12)) /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_SPFMIC_RX_DAT(n)   MC_R (0xC840 + ((n) << 12)) /* Регистр управления и состояния канала */
#define MC_CP_SPFMIC_RX_DAT(n)    MC_R (0xC844 + ((n) << 12)) /* Регистр указателя цепочки канала */
#define MC_IR_SPFMIC_RX_DAT(n)    MC_R (0xC848 + ((n) << 12)) /* Индексный регистр внешней памяти канала */
#define MC_RUN_SPFMIC_RX_DAT(n)   MC_R (0xC84C + ((n) << 12)) /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_SPFMIC_TX_DES(n)   MC_R (0xC880 + ((n) << 12)) /* Регистр управления и состояния канала */
#define MC_CP_SPFMIC_TX_DES(n)    MC_R (0xC884 + ((n) << 12)) /* Регистр указателя цепочки канала */
#define MC_IR_SPFMIC_TX_DES(n)    MC_R (0xC888 + ((n) << 12)) /* Индексный регистр внешней памяти канала */
#define MC_RUN_SPFMIC_TX_DES(n)   MC_R (0xC88C + ((n) << 12)) /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_SPFMIC_TX_DAT(n)   MC_R (0xC8C0 + ((n) << 12)) /* Регистр управления и состояния канала */
#define MC_CP_SPFMIC_TX_DAT(n)    MC_R (0xC8C4 + ((n) << 12)) /* Регистр указателя цепочки канала */
#define MC_IR_SPFMIC_TX_DAT(n)    MC_R (0xC8C8 + ((n) << 12)) /* Индексный регистр внешней памяти канала */
#define MC_RUN_SPFMIC_TX_DAT(n)   MC_R (0xC8CC + ((n) << 12)) /* Псевдорегистр управления состоянием бита RUN */

/*
 * Регистры коммутатора GigaSpWR
 */
#define MC_GSPW_ID_VER          MC_R (0xA000) /* Регистр версии */
#define MC_GSPW_ID_SWITCH       MC_R (0xA004) /* Регистр идентификатора gigaSpWR */
#define MC_GSPW_ID_NET          MC_R (0xA008) /* Регистр идентификации сетевых линков gigaSpWR */
#define MC_GSPW_MODE_R          MC_R (0xA00C) /* Регистр режима блока коммутатора gigaSpWR 1 */
#define MC_GSPW_MODE_R1         MC_R (0xA010) /* Регистр режима блока коммутатора gigaSpWR 2 */
#define MC_GSPW_STATE_R         MC_R (0xA014) /* Регистр состояния блока коммутатора gigaSpWR */
#define MC_GSPW_RISC_IRQ_MASK   MC_R (0xA018) /* Регистр глобальных масок прерываний для встроенного процессорного ядра */
#define MC_GSPW_AUTO_COU        MC_R (0xA01C) /* Регистр управления автоматической установкой соединения */
#define MC_GSPW_CONTROL_CONNECTION  MC_R (0xA020) /* Регистр контроля соединения */
#define MC_GSPW_STATE_CONNECTION    MC_R (0xA024) /* Регистр флагов состояния соединения */
#define MC_GSPW_SW_DAT_TOUTS    MC_R (0xA028) /* Регистр глобальных таймаутов данных */
#define MC_GSPW_SW_DAT_TOUTS2   MC_R (0xA02C) /* Регистр глобальных таймаутов данных 2 */
#define MC_GSPW_SW_DAT_TOUTS3   MC_R (0xA030) /* Регистр глобальных таймаутов данных 3 */
#define MC_GSPW_SPEC_ARB        MC_R (0xA034) /* Регистр специальных условий арбитража */
#define MC_GSPW_CCODE_OUT       MC_R (0xA038) /* Регистр управляющего кода для отправки в сеть */
#define MC_GSPW_CUR_TIME        MC_R (0xA03C) /* Регистр маркера времени из сети */
#define MC_GSPW_ISR_L           MC_R (0xA040) /* Младшая половина регистра ISR */
#define MC_GSPW_ISR_H           MC_R (0xA044) /* Старшая половина регистра ISR */
#define MC_GSPW_INTR_IRQ_MASK_L     MC_R (0xA048) /* Регистр маски прерывания для встроенного процессора
                                                    при приеме кода распределенного прерывания (младшая половина) */
#define MC_GSPW_INTR_IRQ_MASK_H     MC_R (0xA04C) /* Регистр маски прерывания для встроенного процессора
                                                    при приеме кода распределенного прерывания (старшая половина) */
#define MC_GSPW_INTA_IRQ_MASK_L     MC_R (0xA050) /* Регистр маски прерывания для встроенного процессора при
                                                    приеме кода подтверждения (младшая половина) */
#define MC_GSPW_INTA_IRQ_MASK_H     MC_R (0xA054) /* Регистр маски прерывания для встроенного процессора при
                                                    приеме кода подтверждения (старшая половина) */
#define MC_GSPW_CCODES_MASK1        MC_R (0xA058) /* Регистр маски входных и выходных портов для управляющих кодов 1 */
#define MC_GSPW_CCODES_MASK2        MC_R (0xA05C) /* Регистр маски входных и выходных портов для управляющих кодов 2 */
#define MC_GSPW_DIST_INTS_TOUTS1    MC_R (0xA060) /* Регистр таймаутов распределенных прерыаний 1 */
#define MC_GSPW_DIST_INTS_TOUTS2    MC_R (0xA064) /* Регистр таймаутов распределенных прерыаний 2 */
#define MC_GSPW_ACK_NON_ACK_REGIME  MC_R (0xA068) /* Регистр флагов режима обработки распределенных прерываний
                                                    (с подтверждением / без подтверждения) */
#define MC_GSPW_CCODES_SPEC_REGIME  MC_R (0xA06C) /* Регистр специального режима работы с управляющими кодами */
#define MC_GSPW_SPEC_ISR_REGIME     MC_R (0xA070) /* Регистр флагов автоматической отправки подтверждения */
#define MC_GSPW_INTER_HANDLER_TERM_FUNCT    MC_R (0xA074) /* Флаги-признаки обработчика для распределенных прерываний */
#define MC_GSPW_SR_SOURCE_TERM_FUNCT        MC_R (0xA078) /* Флаги-признаки источника для распределенных прерываний */
#define MC_GSPW_ISR_TOUTS_FLS_L     MC_R (0xA07C) /* Младшая половина регистра флагов таймаутов */
#define MC_GSPW_ISR_TOUTS_FLS_H     MC_R (0xA080) /* Старшая половина регистра флагов таймаутов */
#define MC_GSPW_ISR_1101            MC_R (0xA084) /* Значение регистра ISR для управляющих кодов,
                                                    назначение которых не определено стандартом */
#define MC_GSPW_EXTERNAL_RESET_PARAMETERS   MC_R (0xA088) /* Регистр параметров удаленного сброса */
#define MC_GSPW_SPW_STATUS(n)       MC_R (0xA08C + (n) * 4) /* Регистр состояния порта SpW */
#define MC_GSPW_SPW_MODE(n)         MC_R (0xA0A4 + (n) * 4) /* Регистр режима работы порта SpW */
#define MC_GSPW_SPW_TX_SPEED(n)     MC_R (0xA0BC + (n) * 4) /* Регистр скорости передачи порта SpW */
#define MC_GSPW_GIGA_SPW_TX_SPEED(n)        MC_R (0xA0C4 + (n) * 4) /* Регистр параметров передачи порта gigaSpW */
#define MC_GSPW_SPW_RX_SPEED(n)     MC_R (0xA0D4 + (n) * 4) /* Регистр скорости приема порта SpW и порта gigaSpW */
#define MC_GSPW_ADG(n)              MC_R (0xA0EC + (n) * 4) /* Регистр адаптивной групповой маршрутизации порта SpW */
#define MC_GSPW_GIGA_PMA_STATUS(n)  MC_R (0xA104 + (n) * 4) /* Регистр состояния PMA порта gigaSpW */
#define MC_GSPW_GIGA_SPW_PMA_MODE(n)        MC_R (0xA114 + (n) * 4) /* Регистр режима работы PMA порта gigaSpW */


/* Таблица маршрутизации располагается по адресу 0x182FA400 и занимает 1024 байта */
#define MC_GSPW_TABLE       (volatile void *)0xB82FA400


/*
 * Регистры DMA коммутатора GigaSpWR
 */
#define MC_CSR_GSPW_RX_DES   MC_R (0xA800) /* Регистр управления и состояния канала */
#define MC_CP_GSPW_RX_DES    MC_R (0xA804) /* Регистр указателя цепочки канала */
#define MC_IR_GSPW_RX_DES    MC_R (0xA808) /* Индексный регистр внешней памяти канала */
#define MC_RUN_GSPW_RX_DES   MC_R (0xA80C) /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_GSPW_RX_DAT   MC_R (0xA840) /* Регистр управления и состояния канала */
#define MC_CP_GSPW_RX_DAT    MC_R (0xA844) /* Регистр указателя цепочки канала */
#define MC_IR_GSPW_RX_DAT    MC_R (0xA848) /* Индексный регистр внешней памяти канала */
#define MC_RUN_GSPW_RX_DAT   MC_R (0xA84C) /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_GSPW_TX_DES   MC_R (0xA880) /* Регистр управления и состояния канала */
#define MC_CP_GSPW_TX_DES    MC_R (0xA884) /* Регистр указателя цепочки канала */
#define MC_IR_GSPW_TX_DES    MC_R (0xA888) /* Индексный регистр внешней памяти канала */
#define MC_RUN_GSPW_TX_DES   MC_R (0xA88C) /* Псевдорегистр управления состоянием бита RUN */

#define MC_CSR_GSPW_TX_DAT   MC_R (0xA8C0) /* Регистр управления и состояния канала */
#define MC_CP_GSPW_TX_DAT    MC_R (0xA8C4) /* Регистр указателя цепочки канала */
#define MC_IR_GSPW_TX_DAT    MC_R (0xA8C8) /* Индексный регистр внешней памяти канала */
#define MC_RUN_GSPW_TX_DAT   MC_R (0xA8CC) /* Псевдорегистр управления состоянием бита RUN */

/*
 * Регистры DDR(0-1)
 */
#define MC_DDR_BAR(n)   MC_R (0x1210 + ((n) << 8)) /* Регистр базового адреса */
#define MC_DDR_CON(n)   MC_R (0x1214 + ((n) << 8)) /* Регистр конфигурации DDR */
#define MC_DDR_TMR(n)   MC_R (0x1218 + ((n) << 8)) /* Регистр параметров DDR */
#define MC_DDR_CSR(n)   MC_R (0x121C + ((n) << 8)) /* Регистр управления и состояния */
#define MC_DDR_MOD(n)   MC_R (0x1220 + ((n) << 8)) /* Регистр режимов */
#define MC_DDR_EXT(n)   MC_R (0x1224 + ((n) << 8)) /* Регистр управления режимами контроля памяти */
#define MC_DDR_ERR(n)   MC_R (0x1228 + ((n) << 8)) /* Регистр ошибок памяти */

/*
 * Регистры Ethernet MAC
 */
#define MC_MAC_CONTROL          MC_R (0xE000)   /* Управление MAC */
#define MC_MAC_ADDR_L           MC_R (0xE004)   /* Младшая часть исходного адреса MAC */
#define MC_MAC_ADDR_H           MC_R (0xE008)   /* Старшая часть исходного адреса MAC */
#define MC_MAC_DADDR_L          MC_R (0xE00C)   /* Младшая часть адреса назначения */
#define MC_MAC_DADDR_H          MC_R (0xE010)   /* Старшая часть адреса назначения */
#define MC_MAC_FCS_CLIENT       MC_R (0xE014)   /* Контрольная сумма кадра */
#define MC_MAC_TYPE             MC_R (0xE018)   /* Тип кадра */
#define MC_MAC_IFS_COLL_MODE    MC_R (0xE01C)   /* IFS и режим обработки коллизии */
#define MC_MAC_TX_FRAME_CONTROL MC_R (0xE020)   /* Управление передачей кадра */
#define MC_MAC_STATUS_TX        MC_R (0xE024)   /* Статус передачи кадра */
#define MC_MAC_UCADDR_L         MC_R (0xE028)   /* Младшая часть уникального адреса MAC */
#define MC_MAC_UCADDR_H         MC_R (0xE02C)   /* Старшая часть уникального адреса MAC */
#define MC_MAC_MCADDR_L         MC_R (0xE030)   /* Младшая часть группового адреса */
#define MC_MAC_MCADDR_H         MC_R (0xE034)   /* Старшая часть группового адреса */
#define MC_MAC_MCADDR_MASK_L    MC_R (0xE038)   /* Младшая часть маски группового адреса */
#define MC_MAC_MCADDR_MASK_H    MC_R (0xE03C)   /* Старшая часть маски группового адреса */
#define MC_MAC_HASHT_L          MC_R (0xE040)   /* Младшая часть хэш-таблицы */
#define MC_MAC_HASHT_H          MC_R (0xE044)   /* Старшая часть хэш-таблицы */
#define MC_MAC_RX_FRAME_CONTROL MC_R (0xE048)   /* Управление приемом кадра */
#define MC_MAC_RX_FR_MAXSIZE    MC_R (0xE04C)   /* Максимальный размер принимаемого кадра */
#define MC_MAC_STATUS_RX        MC_R (0xE050)   /* Статус приема кадра */
#define MC_MAC_RX_FRAME_STATUS_FIFO MC_R (0xE054) /* FIFO статусов принятых кадров */
#define MC_MAC_MD_CONTROL       MC_R (0xE058)   /* Управление порта MD */
#define MC_MAC_MD_STATUS        MC_R (0xE05C)   /* Статус порта MD */
#define MC_MAC_MD_MODE          MC_R (0xE060)   /* Режим работы порта MD */
#define MC_MAC_TX_TEST_CSR      MC_R (0xE064)   /* Управление и состояние режима тестирования TX_FIFO */
#define MC_MAC_TX_FIFO          MC_R (0xE068)   /* Передающее TX_FIFO */
#define MC_MAC_RX_TEST_CSR      MC_R (0xE06C)   /* Управление и состояние режима тестирования RX_FIFO */
#define MC_MAC_RX_FIFO          MC_R (0xE070)   /* Принимающее RX_FIFO */

/*
 * Регистры MFBSP(0-3)
 */
#define MC_MFBSP_TX(n)		MC_R (0x7000+(n<<8))   /* Буфер передачи данных */
#define MC_MFBSP_RX(n)		MC_R (0x7000+(n<<8))   /* Буфер приема данных */
#define MC_MFBSP_CSR(n)		MC_R (0x7004+(n<<8))   /* Управление и состояние */
#define MC_MFBSP_DIR(n)		MC_R (0x7008+(n<<8))   /* Управление направлением
														* порта ввода-вывода */
#define MC_MFBSP_GPIO_DR(n)	MC_R (0x700C+(n<<8))   /* Данные порта ввода-вывода */
#define MC_MFBSP_TCTR(n)	MC_R (0x7010+(n<<8))   /* Управление передатчиком */
#define MC_MFBSP_RCTR(n)	MC_R (0x7014+(n<<8))   /* Управление приёмником */
#define MC_MFBSP_TSR(n)		MC_R (0x7018+(n<<8))   /* Состояние передатчика */
#define MC_MFBSP_RSR(n)		MC_R (0x701C+(n<<8))   /* Состояние приёмника */
#define MC_MFBSP_TCTR_RATE(n)	MC_R (0x7020+(n<<8))   /* Управление темпом передачи данных */
#define MC_MFBSP_RCTR_RATE(n)	MC_R (0x7024+(n<<8))   /* Управление темпом приёма данных */
#define MC_MFBSP_TSTART(n)	MC_R (0x7028+(n<<8))   /* Запуск/останов передатчика без изменения настроек передатчика */
#define MC_MFBSP_RSTART(n)	MC_R (0x702C+(n<<8))   /* Запуск/останов передатчика без изменения настроек приёмника */
#define MC_MFBSP_EMERG(n)	MC_R (0x7030+(n<<8))   /* Регистр аварийного управления портом */
#define MC_MFBSP_IMASK(n) 	MC_R (0x7034+(n<<8))   /* Регистр маски прерываний от порта */


/*
 * Регистры UART0, UART1
 */
#define MC_UART0_BASE   MC_A (0x3000)
#define MC_RBR          MC_R (0x3000)   /* Приемный буферный регистр */
#define MC_THR          MC_R (0x3000)   /* Передающий буферный регистр */
#define MC_IER          MC_R (0x3004)   /* Регистр разрешения прерываний */
#define MC_IIR          MC_R (0x3008)   /* Регистр идентификации прерывания */
#define MC_FCR          MC_R (0x3008)   /* Регистр управления FIFO */
#define MC_LCR          MC_R (0x300C)   /* Регистр управления линией */
#define MC_MCR          MC_R (0x3010)   /* Регистр управления модемом */
#define MC_LSR          MC_R (0x3014)   /* Регистр состояния линии */
#define MC_MSR          MC_R (0x3018)   /* Регистр состояния модемом */
#define MC_SPR          MC_R (0x301C)   /* Регистр Scratch Pad */
#define MC_DLL          MC_R (0x3000)   /* Регистр делителя младший */
#define MC_DLM          MC_R (0x3004)   /* Регистр делителя старший */
#define MC_SCLR         MC_R (0x3014)   /* Регистр предделителя (scaler) */

#define MC_UART1_BASE   MC_A (0x3400)
#define MC_RBR1         MC_R (0x3400)   /* Приемный буферный регистр */
#define MC_THR1         MC_R (0x3400)   /* Передающий буферный регистр */
#define MC_IER1         MC_R (0x3404)   /* Регистр разрешения прерываний */
#define MC_IIR1         MC_R (0x3408)   /* Регистр идентификации прерывания */
#define MC_FCR1         MC_R (0x3408)   /* Регистр управления FIFO */
#define MC_LCR1         MC_R (0x340C)   /* Регистр управления линией */
#define MC_MCR1         MC_R (0x3410)   /* Регистр управления модемом */
#define MC_LSR1         MC_R (0x3414)   /* Регистр состояния линии */
#define MC_MSR1         MC_R (0x3418)   /* Регистр состояния модемом */
#define MC_SPR1         MC_R (0x341C)   /* Регистр Scratch Pad */
#define MC_DLL1         MC_R (0x3400)   /* Регистр делителя младший */
#define MC_DLM1         MC_R (0x3404)   /* Регистр делителя старший */
#define MC_SCLR1        MC_R (0x3414)   /* Регистр предделителя (scaler) */

/*
 * Регистры интервальных таймеров IT0, IT1
 */
#define MC_ITCSR        MC_R (0x5000)   /* Управление */
#define MC_ITPERIOD     MC_R (0x5004)   /* Период работы таймера */
#define MC_ITCOUNT      MC_R (0x5008)   /* Счетчик */
#define MC_ITSCALE      MC_R (0x500C)   /* Предделитель */

#define MC_ITCSR1       MC_R (0x5020)   /* Управление */
#define MC_ITPERIOD1    MC_R (0x5024)   /* Период работы таймера */
#define MC_ITCOUNT1     MC_R (0x5028)   /* Счетчик */
#define MC_ITSCALE1     MC_R (0x502C)   /* Предделитель */

/*
 * Регистры WDT
 */
#define MC_WTCSR    MC_R (0x5010)   /* Управление */
#define MC_WTPERIOD MC_R (0x5014)   /* Период работы таймера */
#define MC_WTCOUNT  MC_R (0x5018)   /* Счетчик */
#define MC_WTSCALE  MC_R (0x501C)   /* Предделитель */

/*
 * Регистры порта внешней памяти MPORT
 */
#define MC_CSCON0       MC_R (0x1000)   /* Регистр конфигурации 0 */
#define MC_CSCON1       MC_R (0x1004)   /* Регистр конфигурации 1 */
#define MC_CSCON2       MC_R (0x1008)   /* Регистр конфигурации 2 */
#define MC_CSCON3       MC_R (0x100C)   /* Регистр конфигурации 3 */
#define MC_CSCON4       MC_R (0x1010)   /* Регистр конфигурации 4 */
#define MC_SDRCON       MC_R (0x1014)   /* Регистр конфигурации памяти SDRAM */
#define MC_SDRTMR       MC_R (0x1018)   /* Временные параметры памяти SDRAM */
#define MC_SDRCSR       MC_R (0x101C)   /* Управление режимами памяти SDRAM */
#define MC_FLY_WS       MC_R (0x1020)   /* Доп. такты ожидания в режиме FLYBY */
#define MC_CSREXT       MC_R (0x1024)   /* Регистр управления режимами контроля внешней памяти */
#define MC_AERROR_EXT   MC_R (0x1028)   /* Регистр ошибок внешней памяти */

/*
 * Системные регистры
 */
#define MC_CRPLL    MC_R (0x4000)   /* Управление PLL */
#define MC_CRPLL1   MC_R (0x407C)   /* Управление PLL */
#define MC_CLKEN    MC_R (0x4004)   /* Управление отключением частоты от устройств */
#define MC_CSR      MC_R (0x4008)   /* Управление и состояние */
#define MC_MASKR0   MC_R (0x4010)   /* Регистр 0 маски прерываний */
#define MC_QSTR0    MC_R (0x4014)   /* Регистр 0 запросов прерываний */
#define MC_MASKR1   MC_R (0x4018)   /* Регистр 1 маски прерываний */
#define MC_QSTR1    MC_R (0x401C)   /* Регистр 1 запросов прерываний */
#define MC_MASKR2   MC_R (0x4020)   /* Регистр 2 маски прерываний */
#define MC_QSTR2    MC_R (0x4024)   /* Регистр 2 запросов прерываний */
#define MC_MASKR3   MC_R (0x4028)   /* Регистр 3 маски прерываний */
#define MC_QSTR3    MC_R (0x402c)   /* Регистр 3 запросов прерываний */
#define MC_IRQM     MC_R (0x4030)   /* Режимы внешних прерываний nIRQ[3:0] */

/*
 * Регистры контроля по коду Хэмминга внутренней памяти
 */
#define MC_CSR_CRAM0A       MC_R (0x4400)   /* Регистр управления и состояния CRAM0A */
#define MC_AERROR_CRAM0A    MC_R (0x4404)   /* FIFO ошибочных адресов CRAM0A */
#define MC_CSR_CRAM0B       MC_R (0x4408)   /* Регистр управления и состояния CRAM0B */
#define MC_AERROR_CRAM0B    MC_R (0x440C)   /* FIFO ошибочных адресов CRAM0B */
#define MC_CSR_CRAM1A       MC_R (0x4410)   /* Регистр управления и состояния CRAM1A */
#define MC_AERROR_CRAM1A    MC_R (0x4414)   /* FIFO ошибочных адресов CRAM1A */
#define MC_CSR_CRAM1B       MC_R (0x4418)   /* Регистр управления и состояния CRAM1B */
#define MC_AERROR_CRAM1B    MC_R (0x441C)   /* FIFO ошибочных адресов CRAM1B */
#define MC_CSR_CRAM2A       MC_R (0x4420)   /* Регистр управления и состояния CRAM2A */
#define MC_AERROR_CRAM2A    MC_R (0x4424)   /* FIFO ошибочных адресов CRAM2A */
#define MC_CSR_CRAM2B       MC_R (0x4428)   /* Регистр управления и состояния CRAM2B */
#define MC_AERROR_CRAM2B    MC_R (0x442C)   /* FIFO ошибочных адресов CRAM2B */
#define MC_CSR_CRAM3A       MC_R (0x4430)   /* Регистр управления и состояния CRAM3A */
#define MC_AERROR_CRAM3A    MC_R (0x4434)   /* FIFO ошибочных адресов CRAM3A */
#define MC_CSR_CRAM3B       MC_R (0x4438)   /* Регистр управления и состояния CRAM3B */
#define MC_AERROR_CRAM3B    MC_R (0x443C)   /* FIFO ошибочных адресов CRAM3B */
#define MC_CSR_ICACHE       MC_R (0x4800)   /* Регистр управления и состояния ICACHE */
#define MC_AERROR_ICACHE    MC_R (0x4804)   /* FIFO ошибочных адресов ICACHE */
#define MC_CSR_DCACHE       MC_R (0x4C00)   /* Регистр управления и состояния DCACHE */
#define MC_AERROR_DCACHE    MC_R (0x4C04)   /* FIFO ошибочных адресов DCACHE */

/*
 * Регистры блока аппаратных ускорителей ACC (CTR)
 */
#define ACC_R(a)   *(volatile unsigned*)(0xB8480000 + (a))
#define MC_ACC_CONF0        ACC_R (0x2000)   /* Регистр конфигурации */
#define MC_ACC_IRQM         ACC_R (0x2004)   /* Регистр маски прерываний */
#define MC_ACC_IRQ          ACC_R (0x2008)   /* Регистр прерываний */
#define MC_ACC_HEM_M(n)     ACC_R (0x200C + (n) * 8)   /* Регистр управления блока Хэмминга номер n */
#define MC_ACC_HEM_E(n)     ACC_R (0x2010 + (n) * 8)   /* Регистр FIFO адреса ошибки блока Хэмминга номер n */

/*
 * Регистры блока аппаратных ускорителей FFT
 */
#define MC_FFT_CR           ACC_R (0x2100)  /* Регистр управления */
#define MC_FFT_SR           ACC_R (0x2104)  /* Регистр статуса */
#define MC_FFT_CONF0        ACC_R (0x2108)  /* Регистр конфигурации 0 */
#define MC_FFT_CONF1        ACC_R (0x210C)  /* Регистр конфигурации 1 */
#define MC_FFT_ADDRB        ACC_R (0x2110)  /* Регистр адреса входных данных массива 0 */
#define MC_FFT_ADDRH        ACC_R (0x2114)  /* Регистр адреса входных данных массива 1 */
#define MC_FFT_NORC         ACC_R (0x2118)  /* Регистр значения нормализации результата */
#define MC_FFT_IRQM         ACC_R (0x211C)  /* Регистр маски прерываний */
#define MC_FFT_IRQ          ACC_R (0x2120)  /* Регистр прерываний */
#define MC_FFT_ACC0         ACC_R (0x2124)  /* Регистр значения действительной части аккумулятора */
#define MC_FFT_ACC1         ACC_R (0x2128)  /* Регистр значения мнимой части аккумулятора */

/*
 * Регистры блока аппаратных ускорителей JPEG
 */
#define MC_JPEG_CR           ACC_R (0x2200)  /* Регистр управления */
#define MC_JPEG_SR           ACC_R (0x2204)  /* Регистр статуса */
#define MC_JPEG_CONF0        ACC_R (0x2208)  /* Регистр конфигурации 0 */
#define MC_JPEG_CONF1        ACC_R (0x220C)  /* Регистр конфигурации 1 */
#define MC_JPEG_ADDRy        ACC_R (0x2110)  /* Регистр адреса данных Y компоненты в памяти ускорителя */
#define MC_JPEG_ADDRcb       ACC_R (0x2114)  /* Регистр адреса Cb компоненты в памяти ускорителя */
#define MC_JPEG_ADDRcr       ACC_R (0x2118)  /* Регистр адреса Cr компоненты в памяти ускорителя */
#define MC_JPEG_ADDRo        ACC_R (0x211C)  /* Регистр адреса выходного массива в памяти ускорителя */
#define MC_JPEG_COEFa        ACC_R (0x2120)  /* Регистр адреса коэффициентов квантования */
#define MC_JPEG_COEFd        ACC_R (0x2124)  /* Регистр данных коэффициентов квантования */
#define MC_JPEG_LEN          ACC_R (0x2128)  /* Регистр длины выходного массива (в битах) */
#define MC_JPEG_IRQM         ACC_R (0x212C)  /* Регистр маски прерываний */
#define MC_JPEG_IRQ          ACC_R (0x2130)  /* Регистр прерываний */


/*
 * Номера прерываний uOS
 */
#define IRQ_IRQ0                0
#define IRQ_IRQ1                1
#define IRQ_IRQ2                2
#define IRQ_IRQ3                3
#define IRQ_UART0               4
#define IRQ_UART1               5
#define IRQ_SPI                 6
#define IRQ_USB                 7
#define IRQ_USB_EP1             8
#define IRQ_USB_EP2             9
#define IRQ_USB_EP3             10
#define IRQ_USB_EP4             11
#define IRQ_EMAC_RX_FRAME       12
#define IRQ_EMAC_TX_FRAME       13
#define IRQ_EMAC_DMA_RX         14
#define IRQ_EMAC_DMA_TX         15
#define IRQ_SPWR_RX_DES_CH      16
#define IRQ_SPWR_RX_DAT_CH      17
#define IRQ_SPWR_TX_DES_CH      18
#define IRQ_SPWR_TX_DAT_CH      19
#define IRQ_WDT                 20
#define IRQ_IT0                 21
#define IRQ_IT1                 22
#define IRQ_SPWR0               23
#define IRQ_SPWR1               24
#define IRQ_SPWR2               25
#define IRQ_SPWR3               26
#define IRQ_SPWR4               27
#define IRQ_SPWR5               28
#define IRQ_SPWR6               29
#define IRQ_SPWR7               30
#define IRQ_MEM_CH00            31
#define IRQ_MEM_CH01            32
#define IRQ_MEM_CH02            33
#define IRQ_MEM_CH03            34
#define IRQ_MEM_CH04            35
#define IRQ_MEM_CH05            36
#define IRQ_MEM_CH06            37
#define IRQ_MEM_CH07            38
#define IRQ_MEM_CH10            39
#define IRQ_MEM_CH11            40
#define IRQ_MEM_CH12			41
#define IRQ_MEM_CH13			42
#define IRQ_MEM_CH14			43
#define IRQ_MEM_CH15			44
#define IRQ_MEM_CH16			45
#define IRQ_MEM_CH17			46
#define IRQ_SRQ0				47
#define IRQ_MFBSP_RXBUF0		48
#define IRQ_MFBSP_TXBUF0		49
#define IRQ_DMA_MFBSP0			50
#define IRQ_SRQ1				51
#define IRQ_MFBSP_RXBUF1		52
#define IRQ_MFBSP_TXBUF1		53
#define IRQ_DMA_MFBSP1			54
#define IRQ_SRQ2				55
#define IRQ_MFBSP_RXBUF2		56
#define IRQ_MFBSP_TXBUF2		57
#define IRQ_DMA_MFBSP2			58
#define IRQ_SRQ3				59
#define IRQ_MFBSP_RXBUF3		60
#define IRQ_MFBSP_TXBUF3		61
#define IRQ_DMA_MFBSP3			62
#define IRQ_SPFMIC0				63
#define IRQ_SPFMIC_RX_DES_CH0	64
#define IRQ_SPFMIC_RX_DAT_CH0	65
#define IRQ_SPFMIC_TX_DES_CH0	66
#define IRQ_SPFMIC_TX_DAT_CH0	67
#define IRQ_SPFMIC1				68
#define IRQ_SPFMIC_RX_DES_CH1	69
#define IRQ_SPFMIC_RX_DAT_CH1	70
#define IRQ_SPFMIC_TX_DES_CH1	71
#define IRQ_SPFMIC_TX_DAT_CH1	72
#define IRQ_HM_CRAM0			73
#define IRQ_HM_CRAM1			74
#define IRQ_HM_CRAM2			75
#define IRQ_HM_CRAM3			76
#define IRQ_HM_ICACHE			77
#define IRQ_HM_DCACHE			78
#define IRQ_HM_MPORT			80
#define IRQ_HM_DSP0				81
#define IRQ_HM_DSP1				82
#define IRQ_HM_DDR0				85
#define IRQ_HM_DDR1				86
#define IRQ_COMPARE				90
#define IRQ_HM_ACC				104

#endif /* _IO_MC30SF6_H */
