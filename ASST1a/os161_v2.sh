#!/bin/bash
cd os161-1.99-2
./configure --ostree=$HOME/cs350-os161/root --toolprefix=cs350-
cd kern/conf
./config ASST1
cd ../compile/ASST1
bmake depend
bmake
bmake install
cd $HOME/cs350-os161/os161-1.99-2
bmake
bmake install
cd $HOME/cs350-os161/root
sys161 kernel-ASST1 
