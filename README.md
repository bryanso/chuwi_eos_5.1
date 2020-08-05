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

Step 1. Double check your wifi adapter is Realtek 8821CE:

    lspci | grep Network

It should display RTL8821CE somewhere in the output.  My output:

    Network controller: Realtek Semiconductor Co., Ltd. RTL8821CE 802.11ac PCIe Wireless Network Adapter

Step 2. Check your EOS version.  This is my version but it may work with others, perhaps even Ubuntu:

    uname -a

    Linux Hero 5.3.0-53-generic #47~18.04.1-Ubuntu SMP Thu May 7 13:10:50 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux

# Installation Instructions

Step 1. Take care of dependent packages:

    cd ~/chuwi_eos_5.1/     # could be chuwi_eos_5.1-master/ if downloaded as Zip
    sudo cp archives/* /var/cache/apt/archives/
    cd /var/cache/apt/archives/
    sudo dpkg -i *.deb

Step 2. Compile tomaspinho's rtl8821ce driver:
 
    cd ~/chuwi_eos_5.1/rtl8821ce
    sudo ./dkms-install.sh

Step 3. Reboot.  Hopefully the wifi is working now.  It does for me.
    
# Github

https://github.com/bryanso/chuwi_eos_5.1.git
