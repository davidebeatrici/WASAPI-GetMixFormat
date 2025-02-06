# WASAPI-GetMixFormat

This program prints the current format for all input and output devices on the system.

Optionally, you can also specify the audio category by passing it as an argument.  

## Audio categories

Officially known values since Windows 10 21H1 go from 0 to 14:

| Value | Description |
| --- | --- |
| `0` | All other streams (default) |
| `1` | (deprecated for Win10) Music, Streaming audio |
| `2` | (deprecated for Win10) Video with audio |
| `3` | VOIP, chat, phone call |
| `4` | Alarm, Ring tones |
| `5` | Sound effects, clicks, dings |
| `6` | Game sound effects |
| `7` | Background audio for games |
| `8` | In game player chat |
| `9` | Speech recognition |
| `10` | Video with audio |
| `11` | Music, Streaming audio |
| `12` | Capture of far field speech |
| `13` | Uniform, device agnostic speech processing |
| `14` | Dictation, typing by voice |

---

Some audio drivers behave differently depending on the category.

For example, this program was originally created to investigate an issue that suddenly appeared after updating to Windows 11 24H2.

[VB-Cable](https://vb-audio.com) input devices advertise 1 channel instead of 2 and `0x0003` (`WAVE_FORMAT_IEEE_FLOAT`) as the format tag instead of `0xfffe` (`WAVE_FORMAT_EXTENSIBLE`) when the category is related to speech (`3`, `8`, `9`, `12`, `13`, `14`).
The issue is fixed with the driver package that was released in October 2024.

Please open an issue if you find any other audio driver to be affected. In that case the issue should be reported to Microsoft.
