[USB-C Port] --(Charging)--> [TP4056 Charger] --(3.7V)--> [LiPo Battery] --(Power)--> [LDO Regulator] --(3.3V)--> [ESP32-S3]
                                                                 |
                                                                 |--(Power to all)
                                                                 v
[Touch Screen (ILI9341/FT6236)] <--(SPI/I2C)--> [ESP32-S3] <--(Wi-Fi/Bluetooth)--> [Internet/Headphones]
                                 ^               |
                                 |               |--(I2S)--> [MAX98357A DAC] --(Analog)--> [3.5mm Audio Jack]
                                 |               |
                                 |               |--(SPI)--> [MicroSD Slot]
                                 |               |
                                 |               |--(GPIO)--> [Buttons]
                                 |
                                 --(Touch Inputs)--> Spotify UI Control