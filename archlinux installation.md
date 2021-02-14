# Archlinux install guide
Although most steps are explained with fidelity in the online guide, I did get stuck a couple times during the installation so I will reflect the code. I used an existing Ubuntu OS as a starting point, the only thing that would change using windows would be th [Download image section](/Download image)
## Download image
---
Download the archlinux image .iso and the signature file .sig
In linux these can be verified by running gpg which in term needs to be downloaded. In addition to following the explanation from the guide, three libraries need to be [downloaded](https://www.gnupg.org/download/index.html) and installed in the order that they are laid out when getting and error from the failed installation:

- Libgpg-error
- Libgcrypt
- Libksba
- Libassuan
- nPth

After that, `cd`ing into the gpg... directory and executing the following commands will install gpg
```
./configure
make
make install
```
The verification method suggested by the wiki did not work for me (missing public key ID) so I ran gpg on the .iso and compared to the signature from the downloads page
```
gpg archlinux.iso
```
NOTE: that it is important to realize that packages installed in the live image are not installed in the OS that I am configuring. For packages to persist they need to be installed after arch-chroot to the /mnt partition. 

## Network configuration
---
In the [Installation section](https://wiki.archlinux.org/index.php/Installation_guide) of the installation guide, after installing the base package, install the following: 
```
vim
dhcp
dhcpcd
networkmanager
wireless_tools
iw
iwd

```
After installation (which I did with an ethernet cable since wifi made the OS crash), neither the ethernet connection nor the WiFi work. Since I forgot to install dhchp and iw during setup process, I had to go back to the live image to get these packages. 
### Ethernet
Although an ethernet connection will work seamlessly within the live-USB environment, it will not work after the installation is completed since the route needs to be setup again. In the live environment, this is the route:
```
default via 192.168.0.1 dev enp2s0 proto dhcp src 192.168.0.100 metric 512

192.168.0.0/24 dev enp2s0 proto kernel scope link src 192.168.0.100

192.168.0.1 dev enp2s0 proto dhcp scope link src 192.168.0.100 metric 512
```
A route can then be added as described in the [network configuration page](https://wiki.archlinux.org/index.php/Network_configuration)

### WiFi
#### Setting up the WiFi: check network
``` 
ip link
ip link set wlan0 up
dhcpcd 
systemctl start iwd
```
#### Configure WiFi with iwctl
``` 
iwctl
[iwd]# station device scan
[iwd]# station device get-networks
[iwd]# station device connect SSID
[iwd]# exit
```
Even after doing this, the OS was still crashing with the `Kernel panic` message, so `microcode` needs to be installed (which is actually refleceted in the installation guide).

## Other errors
---
```
error: failed to update core (unable to lock database)
```
When pacman starts to alter the package database during sync or installation it creates a lock file at /var/lib/pacman/db.lck. This lock file prevents any other pacman instance or applications from accessing the package database by creating an exclusive lock.

Trouble arises when you interrupt pacman midway, via a reboot, shutdown
```
rm /var/lib/pacman/db.lck
```

## Kernel panic due to WiFi 
[reference](https://wiki.archlinux.org/index.php/User:Mcnster/Kernel_Panics)

### Kernel

[broadcom drivers](https://wiki.archlinux.org/index.php/Broadcom_wireless)

- Download the broadcom-wl with pacman. Then

```
rmmod b43
rmmod ssb
depmod -a
modprobe wl
```

```
sudo mkdir -p /etc/modprobe.d/
sudo tee /etc/modprobe.d/broadcom.conf > /dev/null <<'EOF'
blacklist b43
blacklist b43legacy
blacklist ssb
blacklist bcm43xx
blacklist brcm80211
blacklist brcmfmac
blacklist brcmsmac
blacklist bcma
EOF
```

```
lspci -k % shows hardware
lspci -vnn -d 14e4: %shows broadcom drivers
```

Good references
- [[1](https://docs.01.org/clearlinux/latest/tutorials/broadcom.html)]
- [[2](https://wiki.archlinux.org/index.php/Broadcom_wireless#Loading_the_b43.2Fb43legacy_kernel_module)]
- [[3](https://bbs.archlinux.org/viewtopic.php?id=258428)]

## Pacman
pacman -Q | more
searach for packages 
pacman -Ss chromium



## i3 window manager and other graphics

First off, a display server (framework for guis)


https://wiki.archlinux.org/index.php/Xorg

also xinit and xterm

then 
$ startx /usr/bin/i3

