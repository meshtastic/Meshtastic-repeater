[![CLA assistant](https://cla-assistant.io/readme/badge/meshtastic/Repeater)](https://cla-assistant.io/meshtastic/Repeater)

# Meshtastic-repeater

> ⚠️ This is a Work In Progress and not functional at the moment. Feel free to Improve and Suggest.

A minimalistic repeater node for Meshtastic. 

## Supported Targets:

- ASR6502 (CubeCell nodes by Heltec Automation)
- RP2040 (Raspberry Pico with Waveshare LoRa Hat)
- nRF52 (RAK4631 and similar devices)
- ESP32 (for development only, power consumption is too high on these)

The main point is that this firmware will not store any encryption keys or sensitive information. It will just decrement the hop count and rebroadcast the packet, without decrypting it.
Channel and Region Configuration is done through the known tools like the Meshtastic CLI or the Flasher.