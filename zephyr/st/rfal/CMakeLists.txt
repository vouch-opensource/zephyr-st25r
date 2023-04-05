# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources(
        Src/rfal_analogConfig.c
        Src/rfal_crc.c
        Src/rfal_dpo.c
        Src/rfal_iso15693_2.c
        Src/rfal_isoDep.c
        Src/rfal_nfc.c
        Src/rfal_nfcDep.c
        Src/rfal_nfca.c
        Src/rfal_nfcb.c
        Src/rfal_nfcf.c
        Src/rfal_nfcv.c
        Src/rfal_st25tb.c
        Src/rfal_st25xv.c
        Src/rfal_t1t.c
        Src/rfal_t2t.c
        Src/rfal_t4t.c
)
zephyr_include_directories(Inc)
zephyr_include_directories(Src)
