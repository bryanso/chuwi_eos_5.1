# chuwi_eos_5.1

To fix no wifi adapter issue in CHUWI HeroBook Pro after Elementary OS 5.1 is
installed, we have to compile the rtl8821ce driver.
This git contains all the necessary dependent packages and the rtl8821ce driver
(created by tomaspinho, downloaded from https://github.com/tomaspinho/rtl8821ce)

Since EOS doesn't have wifi or ethernet port to begin with, I recommend you could
install EOS alongside Windows 10, then create a shared NTFS partition in Windows 10
for both OS to access. You may download this repository into this shared partition,
then boot to EOS and mount the partition to begin installation.

Prerequisite, double check your wifi adapter is Realtek 8821CE:

    lspci | grep Network

    It should display rtl8821ce somewhere in the output.

First, let's take care of dependency:

    cd chuwi_eos_5.1/archives
    sudo cp * /var/cache/apt/archives/
    sudo apt install dkms build-essential         

    Since all dependencies have been copied to the cache, EOS does not need internet.

Next, compile tomaspinho's driver:
 
    cd ../rtl8821ce
    sudo ./dkms-install.sh

Finally, reboot.  Hopefully the wifi is working now.  It does for me.
    
