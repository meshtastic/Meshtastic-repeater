# Meshtastic-repeater

[![CLA assistant](https://cla-assistant.io/readme/badge/meshtastic/Meshtastic-repeater)](https://cla-assistant.io/meshtastic/Meshtastic-repeater)
[![Fiscal Contributors](https://opencollective.com/meshtastic/tiers/badge.svg?label=Fiscal%20Contributors&color=deeppink)](https://opencollective.com/meshtastic/)
[![Vercel](https://img.shields.io/static/v1?label=Powered%20by&message=Vercel&style=flat&logo=vercel&color=000000)](https://vercel.com?utm_source=meshtastic&utm_campaign=oss)

> ⚠️ This is a Work In Progress and not functional at the moment. Feel free to Improve and Suggest.

## Overview

A minimalistic repeater node for Meshtastic. 

## Stats

![Alt](https://repobeats.axiom.co/api/embed/76d76b8187a1afb643963f9c8d570e2a8d6f4478.svg "Repobeats analytics image")

## Supported Targets:

- ASR6502 (CubeCell nodes by Heltec Automation)
- RP2040 (Raspberry Pico with Waveshare LoRa Hat)
- nRF52 (RAK4631 and similar devices)
- ESP32 (for development only, power consumption is too high on these)

The main point is that this firmware will not store any encryption keys or sensitive information. It will just decrement the hop count and rebroadcast the packet, without decrypting it.
Channel and Region Configuration is done through the known tools like the Meshtastic CLI or the Flasher.
