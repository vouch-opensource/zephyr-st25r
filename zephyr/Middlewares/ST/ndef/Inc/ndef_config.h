
/**
  ******************************************************************************
  * @file           : ndef_config.h
  * @brief          : NDEF config header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#ifndef NDEF_CONFIG_H
#define NDEF_CONFIG_H


/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#include <st_errno.h>


/*
 ******************************************************************************
 * GLOBAL DEFINES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * NDEF FEATURES CONFIGURATION
 ******************************************************************************
 */

#ifdef NDEF_CONFIG_CUSTOM

#include "ndef_config_custom.h"

/* Define default configuration when not set in ndef_config_custom.h */
#ifndef ndefDevice
#include "rfal_nfc.h"
#include "rfal_t4t.h"
#define ndefDevice                 rfalNfcDevice          /*!< NDEF Device         */
#endif
#ifndef NDEF_FEATURE_T1T
#define NDEF_FEATURE_T1T           RFAL_FEATURE_T1T       /*!< T1T Support control */
#endif
#ifndef NDEF_FEATURE_T2T
#define NDEF_FEATURE_T2T           RFAL_FEATURE_T2T       /*!< T2T Support control */
#endif
#ifndef NDEF_FEATURE_T3T
#define NDEF_FEATURE_T3T           RFAL_FEATURE_NFCF      /*!< T3T Support control */
#endif
#ifndef NDEF_FEATURE_T4T
#define NDEF_FEATURE_T4T           RFAL_FEATURE_T4T       /*!< T4T Support control */
#endif
#ifndef NDEF_FEATURE_T5T
#define NDEF_FEATURE_T5T           RFAL_FEATURE_NFCV      /*!< T5T Support control */
#endif


#ifndef NDEF_FEATURE_FULL_API
#define NDEF_FEATURE_FULL_API                  false      /*!< Support Write, Format, Check Presence, set Read-only in addition to the Read feature */
#endif

#ifndef NDEF_TYPE_EMPTY_SUPPORT
#define NDEF_TYPE_EMPTY_SUPPORT                false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_FLAT_SUPPORT
#define NDEF_TYPE_FLAT_SUPPORT                 false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_AAR_SUPPORT
#define NDEF_TYPE_RTD_AAR_SUPPORT              false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_DEVICE_INFO_SUPPORT
#define NDEF_TYPE_RTD_DEVICE_INFO_SUPPORT      false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_URI_SUPPORT
#define NDEF_TYPE_RTD_URI_SUPPORT              false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_TEXT_SUPPORT
#define NDEF_TYPE_RTD_TEXT_SUPPORT             false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_WLC_SUPPORT
#define NDEF_TYPE_RTD_WLC_SUPPORT              false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_TNEP_SUPPORT
#define NDEF_TYPE_RTD_TNEP_SUPPORT             false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_MEDIA_SUPPORT
#define NDEF_TYPE_MEDIA_SUPPORT                false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_BLUETOOTH_SUPPORT
#define NDEF_TYPE_BLUETOOTH_SUPPORT            false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_VCARD_SUPPORT
#define NDEF_TYPE_VCARD_SUPPORT                false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_WIFI_SUPPORT
#define NDEF_TYPE_WIFI_SUPPORT                 false      /* NDEF library configuration missing. Disabled by default */
#endif

#else

#include "rfal_nfc.h"
#include "rfal_t4t.h"

#define ndefDevice                rfalNfcDevice           /*!< NDEF Device         */
#define NDEF_FEATURE_T1T          RFAL_FEATURE_T1T        /*!< T1T Support control */
#define NDEF_FEATURE_T2T          RFAL_FEATURE_T2T        /*!< T2T Support control */
#define NDEF_FEATURE_T3T          RFAL_FEATURE_NFCF       /*!< T3T Support control */
#define NDEF_FEATURE_T4T          RFAL_FEATURE_T4T        /*!< T4T Support control */
#define NDEF_FEATURE_T5T          RFAL_FEATURE_NFCV       /*!< T5T Support control */


#define NDEF_FEATURE_FULL_API                  true       /*!< Support Write, Format, Check Presence, set Read-only in addition to the Read feature */

#define NDEF_TYPE_EMPTY_SUPPORT                true       /*!< Support Empty type                          */
#define NDEF_TYPE_FLAT_SUPPORT                 true       /*!< Support Flat type                           */
#define NDEF_TYPE_RTD_DEVICE_INFO_SUPPORT      true       /*!< Support RTD Device Information type         */
#define NDEF_TYPE_RTD_TEXT_SUPPORT             true       /*!< Support RTD Text type                       */
#define NDEF_TYPE_RTD_URI_SUPPORT              true       /*!< Support RTD URI type                        */
#define NDEF_TYPE_RTD_AAR_SUPPORT              true       /*!< Support RTD Android Application Record type */
#define NDEF_TYPE_RTD_WLC_SUPPORT              true       /*!< Support RTD WLC Types                       */
#define NDEF_TYPE_RTD_WPCWLC_SUPPORT           true       /*!< Support RTD WPC WLC type                    */
#define NDEF_TYPE_RTD_TNEP_SUPPORT             true       /*!< Support RTD TNEP Types                      */
#define NDEF_TYPE_MEDIA_SUPPORT                true       /*!< Support Media type                          */
#define NDEF_TYPE_BLUETOOTH_SUPPORT            true       /*!< Support Bluetooth types                     */
#define NDEF_TYPE_VCARD_SUPPORT                true       /*!< Support vCard type                          */
#define NDEF_TYPE_WIFI_SUPPORT                 true       /*!< Support Wifi type                           */
#endif /* NDEF_CONFIG_CUSTOM */

#endif /* NDEF_CONFIG_H */

/**
  * @}
  *
  */
