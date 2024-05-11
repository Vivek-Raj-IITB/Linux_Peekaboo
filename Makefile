CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_ALL=n

KDIR = /lib/modules/`uname -r`/build

kbuild:
		make -C $(KDIR) M=`pwd` modules

clean:
		make -C $(KDIR) M=`pwd` clean
