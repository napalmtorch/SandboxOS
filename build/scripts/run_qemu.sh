qemu-system-i386 -enable-kvm -cpu host -usb -m 2048M -vga std -cdrom 'sos.iso' -hda 'hdd.img' -serial stdio -boot d -device AC97 -rtc base=localtime -no-reboot -no-shutdown