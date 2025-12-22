# AC46 Firmware Tool
JieLi AC46/BT15 Firmware Unpacker<br>
![Screenshot](MainScreen.png)

## Disclaimer
This program checks for data corruption by CRC (Cyclic Redundancy Check) technique, but still there might be a risk of outputting corrupt data even CRC passes.<br>
If you reflash corrupt bootloader or SDRAM image back to your device, it may brick and go completely useless.<br>
Author is not responsible for any damages caused by using this program.<br>

## How to use
Drag and drop your flash image dumped from a device with AC46 (BT15) series chipset, or select it via menu.<br>
You can also use a BFU file if you remove first 512 bytes.<br>
Then click "Extract". The program will automatically detect an encryption state, and extract bootloader and all resources.<br>
Once all files are extracted, the program will create a batch file to repack extracted files. This file has .txt extension for safety.

## How to repack (or reflash)
WARNING:<br>
This procedure might brick your device completely if not done properly, or there are corrupt bootloader, program.<br>
Make sure that there are no JieLi devices are connected via USB, unless if you want to reflash.<br>

Repacking enables you to change voice prompt, EQ config, etc.<br>
To repack an extracted files, you will need an SDK for AC46.<br>
First, copy a "Resources" folder to directory with an isd_download.exe. It usually exist in (SDK)\apps\ac46\.<br>
Then move a "Repacker.bat.txt" from "Resources" to the directory mentioned earlier.<br>
Then if you want to just repack, make sure that there are no JieLi devices are connected via USB, unless if you want to reflash. If you want to reflash, you may need to change some setting on isd_tools.cfg.<br>
Finally, remove .txt from "Repacker.bat.txt" and run it. You'll get a jl_ac46.bin and jl_bt46.bfu files.<br>
For example of reflashing, you can watch this [video (MP4)](Reflashing.mp4).<br>


## Included Tools
- AC46 Cipher
- ENC Cipher
- SFC Cipher
- CRC16 Calculator
- Chipkey Decoder

## Credits
- [Andrey Grigoryev](https://github.com/kagaimiq) for cipher, data format
- [Rafa Couto](https://github.com/rafacouto) for CRC16 CCITT implementation

## See also
- [Information about AC46](https://github.com/0x0-nyan/JL_AC46/tree/main)
- [Information about JieLi](https://github.com/kagaimiq/jielie) (by Andrey Grigoryev)
- [JieLi UBOOT Tool](https://github.com/kagaimiq/jl-uboot-tool) (by Andrey Grigoryev)
