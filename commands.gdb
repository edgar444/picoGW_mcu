define o_usbstatenext
o__usb_enum_devstate
p ((USB_OTG_EPTypeDef[2]) *hpcd_USB_OTG_FS.OUT_ep)[1]
next
end

define o_usbstatestep
o__usb_enum_devstate
p ((USB_OTG_EPTypeDef[2]) *hpcd_USB_OTG_FS.OUT_ep)[1]
step
end

define o__usb_enum_devstate
printf "hUsbDeviceFS.dev_state: "
set $x = hUsbDeviceFS.dev_state
if ($x == 1)
printf "1 <USBD_STATE_DEFAULT>\n"
end
if ($x == 2)
printf "2 <USBD_STATE_ADDRESSED>\n"
end
if ($x == 3)
printf "3 <USBD_STATE_CONFIGURED>\n"
end
if ($x == 4)
printf "4 <USBD_STATE_SUSPENDED>\n"
end
end


#break stm32f4xx_hal_pcd.c:654
#commands
#silent
define o_usbmem
p/x (uint32_t[64]) *0x50001000
p/x (uint32_t[64]) *0x50020000
printf "   pos+siz:  21+4   17+4  15+2  4+11  0+4\n"
printf "   GRXSTSR: FRMNUM PKTSTS DPID  BCNT EPNUM\n"
#      "0x00020001:    0x0    0x0  0x0 0x000   0x0"
printf "0x%08x:    0x%01x    0x%01x  0x%01x 0x%03x   0x%01x\n", (*(USB_OTG_GlobalTypeDef*) 0x50000000U).GRXSTSR, ((*(USB_OTG_GlobalTypeDef*) 0x50000000U).GRXSTSR & 0x01E00000U) >> 21, ((*(USB_OTG_GlobalTypeDef*) 0x50000000U).GRXSTSR & 0x001E0000U) >> 17, ((*(USB_OTG_GlobalTypeDef*) 0x50000000U).GRXSTSR & 0x00018000U) >> 15, ((*(USB_OTG_GlobalTypeDef*) 0x50000000U).GRXSTSR & 0x00007FF0U) >> 4, ((*(USB_OTG_GlobalTypeDef*) 0x50000000U).GRXSTSR & 0x0000000FU) >> 0
end

define o_usball
p CmdManager
p hUsbDeviceFS
p (USB_OTG_EPTypeDef[2]) *hpcd_USB_OTG_FS.IN_ep
p (USB_OTG_EPTypeDef[2]) *hpcd_USB_OTG_FS.OUT_ep

p "### OTG FS Registers (See page 694, Ch 22.16)"
p "### # Core global CSRs"
p /x (*(USB_OTG_GlobalTypeDef*) 0x50000000U)
p /x (*(USB_OTG_GlobalTypeDef*) 0x50000000U).
p "### # Device mode CSRs"
p /x (*(USB_OTG_DeviceTypeDef*) 0x50000800U)
p/x (USB_OTG_OUTEndpointTypeDef[2]) *0x50000900U
p/x (USB_OTG_OUTEndpointTypeDef[2]) *0x50000B00U
p "### # OUT EP0"
p/x ((USB_OTG_OUTEndpointTypeDef) *(0x50000B00U | 0 * 0x20))
p "### # IN EP1"
p/x ((USB_OTG_INEndpointTypeDef) *(0x50000900U | 1 * 0x20))

p "### # DFIFO (Device EP0 FIFO; Push/Pop; 4KB)"
x/100xb 0x50001000
p /x (uint32_t[16384]) *0x50001000U

p "### # DFIFO (Direct access for debugging; 128KB)"
x/100xb 0x50020000
end

# Generic Commands
define o_clear
	set $i = 0
	while $i < $arg1
		set *(uint8_t*)($arg0+$i) = 0x00
		set $i = $i + 1
	end
end
document o_clear
Opinionated.
USAGE: o_clear [Address, inclusive] [length, bytes]
end


# TODO
## USB Related
#define o_usbep
#end
#document o_usbep
#Opinionated.
#USAGE: o_usbep 0x80
#Examples: 0x80 | 0x01 is OUT EP1
#          0x00 | 0x0A is IN EP10
#end
