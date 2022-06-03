#
# Yocto recipe to build a kernel module out of the kernel tree
# hello-module.bb 
# www.makersweb.net
#

DESCRIPTION = "Hello kernel module out of the kernel tree"
SECTION = "examples"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"
PR = "r0"

inherit module

SRC_URI =  "\
            file://pointer-err.c \
            file://Makefile \
            file://COPYING \
            "

S = "${WORKDIR}"
