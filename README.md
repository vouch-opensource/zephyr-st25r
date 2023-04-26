# zephyr-st25r

This repo provides a Zephyr module with a driver for the ST25R NFC reader IC. 
In particular, the ST25R3916 and ST25R3916B ICs are supported.

This driver supports at most one instance of the ST25R on the SPI or I2C bus.

ST's RFAL library is included for driving the IC, with Kconfig variables
defined to allow configuration and feature selection.

To use, after incorporating the module into your build, add an 
`st,st25r` compatible SPI or I2C configuration to your device tree

```
&spi3 {
    status = "okay";

    cs-gpios = <&gpio1 12 GPIO_ACTIVE_LOW>;

    nfc0: st25r@0 {
        compatible = "st,st25r";
        reg = <0x0>;
        spi-max-frequency = <4000000>;
        irq-gpios = <&gpio0 3 GPIO_ACTIVE_HIGH>;
    };
};
```

and configure the device in your `prj.conf` file as desired

```
CONFIG_ST25R=y
CONFIG_ST25R3916B=y
CONFIG_ST25R_TRIGGER_GLOBAL_THREAD=y

CONFIG_RFAL_FEATURE_NFCA=y
CONFIG_RFAL_FEATURE_ISO_DEP=y
CONFIG_RFAL_FEATURE_ISO_DEP_POLL=y
```

After that you can use RFAL to drive the IC

```c
#include "rfal_nfc.h"

void init() {
   ReturnCode err = rfalNfcInitialize();
   /* Use RFAL to poll NFC. */
}
```

See the `zephyr-nfc08a1` repo for a complete example of using this driver
with the X-NUCLEO-NFC08A1 dev board.
