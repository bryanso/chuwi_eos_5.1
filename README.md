# chuwi_eos_5.1

# Overview

To fix no wifi adapter issue in CHUWI HeroBook Pro after Elementary OS 5.1 
(I'll call it EOS from now) is installed, we have to compile the rtl8821ce driver.
This git contains the rtl8821ce driver (created by tomaspinho, downloaded from 
https://github.com/tomaspinho/rtl8821ce) and all the necessary dependent packages.

Since EOS doesn't have wifi or ethernet port to begin with, you could
install EOS alongside Windows 10, then create a shared NTFS partition in Windows 10
for both OS to access. You may download this repository into this shared partition,
then boot to EOS and mount the partition to begin installation.

Another option is to download this git from another computer that has internet,
write the files to a USB drive, then mount the USB in CHUWI EOS.

# Prerequisite

Double check your wifi adapter is Realtek 8821CE:

    lspci | grep Network

It should display RTL8821CE somewhere in the output.

# Installation Instructions

1. Take care of dependent packages:

    cd chuwi_eos_5.1/     # could be chuwi_eos_5.1-master/
    sudo cp *.bin /var/cache/apt/
    sudo cp archives/* /var/cache/apt/archives/
    sudo apt install dkms build-essential         

Since all dependencies have been copied to the cache, EOS does not need internet.

2. Compile tomaspinho's rtl8821ce driver:
 
    cd rtl8821ce
    sudo ./dkms-install.sh

3. Reboot.  Hopefully the wifi is working now.  It does for me.
    
# Github

https://github.com/bryanso/chuwi_eos_5.1.git
