.. SPDX-License-Identifier: GPL-2.0

===================
PCI Test User Guide
===================

:Author: Kishon Vijay Abraham I <kishon@ti.com>

This document is a guide to help users use pci-epf-test function driver and pci_endpoint_test host driver for testing PCI. The list of steps to be followed in the host side and EP side is given below.
이 문서는 사용자가 PCI 테스트를 위해 pci-epf-test 기능 드라이버와 pci_endpoint_test 호스트 드라이버를 사용하는 데 도움이 되는 가이드입니다. 호스트 측과 EP 측에서 따라야 할 단계 목록은 아래에 나와 있습니다.

Endpoint Device
===============
엔드포인트 장치
================

Endpoint Controller Devices
---------------------------

엔드포인트 컨트롤러 장치
---------------------------

To find the list of endpoint controller devices in the system::
시스템에서 엔드포인트 컨트롤러 장치 목록을 찾으려면:

	# ls /sys/class/pci_epc/
	  51000000.pcie_ep

If PCI_ENDPOINT_CONFIGFS is enabled::
PCI_ENDPOINT_CONFIGFS가 활성화된 경우::

	# ls /sys/kernel/config/pci_ep/controllers
	  51000000.pcie_ep


Endpoint Function Drivers
-------------------------
끝점 기능 드라이버
----------------------

To find the list of endpoint function drivers in the system::
시스템에서 엔드포인트 기능 드라이버 목록을 찾으려면:
	# ls /sys/bus/pci-epf/drivers
	  pci_epf_test

If PCI_ENDPOINT_CONFIGFS is enabled::
PCI_ENDPOINT_CONFIGFS가 활성화된 경우::

	# ls /sys/kernel/config/pci_ep/functions
	  pci_epf_test


Creating pci-epf-test Device
----------------------------
pci-epf-test 장치 만들기
--------------------------

PCI endpoint function device can be created using the configfs. To create pci-epf-test device, the following commands can be used::
PCI 끝점 기능 장치는 configfs를 사용하여 생성할 수 있습니다. pci-epf-test 장치를 생성하려면 다음 명령을 사용할 수 있습니다.

	# mount -t configfs none /sys/kernel/config
	# cd /sys/kernel/config/pci_ep/
	# mkdir functions/pci_epf_test/func1

The "mkdir func1" above creates the pci-epf-test function device that will be probed by pci_epf_test driver.
위의 "mkdir func1"은 pci_epf_test 드라이버가 조사할 pci-epf-test 기능 장치를 생성합니다.

The PCI endpoint framework populates the directory with the following configurable fields::
PCI 끝점 프레임워크는 다음과 같은 구성 가능한 필드로 디렉터리를 채웁니다.

	# ls functions/pci_epf_test/func1
	  baseclass_code	interrupt_pin	progif_code	subsys_id
	  cache_line_size	msi_interrupts	revid		subsys_vendorid
	  deviceid          	msix_interrupts	subclass_code	vendorid

The PCI endpoint function driver populates these entries with default values when the device is bound to the driver. The pci-epf-test driver populates
vendorid with 0xffff and interrupt_pin with 0x0001::
PCI 끝점 기능 드라이버는 장치가 드라이버에 바인딩될 때 이러한 항목을 기본값으로 채웁니다. pci-epf-test 드라이버가 채워집니다.
0xffff가 있는 vendorid 및 0x0001이 있는 interrupt_pin::

	# cat functions/pci_epf_test/func1/vendorid
	  0xffff
	# cat functions/pci_epf_test/func1/interrupt_pin
	  0x0001


Configuring pci-epf-test Device
-------------------------------

The user can configure the pci-epf-test device using configfs entry. In order to change the vendorid and the number of MSI interrupts used by the function device, the following commands can be used::

pci-epf-test 장치 구성
-------------------------------------------

사용자는 configfs 항목을 사용하여 pci-epf-test 장치를 구성할 수 있습니다. 기능 장치에서 사용하는 공급업체 ID와 MSI 인터럽트 수를 변경하려면 다음 명령을 사용할 수 있습니다.

	# echo 0x104c > functions/pci_epf_test/func1/vendorid
	# echo 0xb500 > functions/pci_epf_test/func1/deviceid
	# echo 16 > functions/pci_epf_test/func1/msi_interrupts
	# echo 8 > functions/pci_epf_test/func1/msix_interrupts


Binding pci-epf-test Device to EP Controller
--------------------------------------------

In order for the endpoint function device to be useful, it has to be bound to a PCI endpoint controller driver. Use the configfs to bind the function device to one of the controller driver present in the system::

pci-epf-test 장치를 EP 컨트롤러에 바인딩
------------------------------------------

끝점 기능 장치가 유용하려면 PCI 끝점 컨트롤러 드라이버에 바인딩되어야 합니다. configfs를 사용하여 시스템에 있는 컨트롤러 드라이버 중 하나에 기능 장치를 바인딩합니다.

	# ln -s functions/pci_epf_test/func1 controllers/51000000.pcie_ep/

Once the above step is completed, the PCI endpoint is ready to establish a link with the host.
위의 단계가 완료되면 PCI 엔드포인트는 호스트와 링크를 설정할 준비가 된 것입니다.

Start the Link
--------------
링크 시작
--------------

In order for the endpoint device to establish a link with the host, the _start_ field should be populated with '1'::
엔드포인트 장치가 호스트와의 링크를 설정하려면 _start_ 필드가 '1'로 채워져야 합니다.

	# echo 1 > controllers/51000000.pcie_ep/start


RootComplex Device
==================
RootComplex 장치
===================

lspci Output
------------
lspci 출력
----------

Note that the devices listed here correspond to the value populated in 1.4 above::
여기에 나열된 장치는 위의 1.4에서 채워진 값에 해당합니다.

	00:00.0 PCI bridge: Texas Instruments Device 8888 (rev 01)
	01:00.0 Unassigned class [ff00]: Texas Instruments Device b500


Using Endpoint Test function Device
-----------------------------------

pcitest.sh added in tools/pci/ can be used to run all the default PCI endpoint tests. To compile this tool the following commands should be used::

엔드포인트 테스트 기능 디바이스 사용
-----------------------------------

tools/pci/에 추가된 pcitest.sh는 모든 기본 PCI 끝점 테스트를 실행하는 데 사용할 수 있습니다. 이 도구를 컴파일하려면 다음 명령을 사용해야 합니다.

	# cd <kernel-dir>
	# make -C tools/pci

or if you desire to compile and install in your system::
또는 시스템에 컴파일하고 설치하려는 경우:

	# cd <kernel-dir>
	# make -C tools/pci install

The tool and script will be located in <rootfs>/usr/bin/
도구와 스크립트는 <rootfs>/usr/bin/에 있습니다.


pcitest.sh Output
~~~~~~~~~~~~~~~~~
::

	# pcitest.sh
	BAR tests

	BAR0:           OKAY
	BAR1:           OKAY
	BAR2:           OKAY
	BAR3:           OKAY
	BAR4:           NOT OKAY
	BAR5:           NOT OKAY

	Interrupt tests

	SET IRQ TYPE TO LEGACY:         OKAY
	LEGACY IRQ:     NOT OKAY
	SET IRQ TYPE TO MSI:            OKAY
	MSI1:           OKAY
	MSI2:           OKAY
	MSI3:           OKAY
	MSI4:           OKAY
	MSI5:           OKAY
	MSI6:           OKAY
	MSI7:           OKAY
	MSI8:           OKAY
	MSI9:           OKAY
	MSI10:          OKAY
	MSI11:          OKAY
	MSI12:          OKAY
	MSI13:          OKAY
	MSI14:          OKAY
	MSI15:          OKAY
	MSI16:          OKAY
	MSI17:          NOT OKAY
	MSI18:          NOT OKAY
	MSI19:          NOT OKAY
	MSI20:          NOT OKAY
	MSI21:          NOT OKAY
	MSI22:          NOT OKAY
	MSI23:          NOT OKAY
	MSI24:          NOT OKAY
	MSI25:          NOT OKAY
	MSI26:          NOT OKAY
	MSI27:          NOT OKAY
	MSI28:          NOT OKAY
	MSI29:          NOT OKAY
	MSI30:          NOT OKAY
	MSI31:          NOT OKAY
	MSI32:          NOT OKAY
	SET IRQ TYPE TO MSI-X:          OKAY
	MSI-X1:         OKAY
	MSI-X2:         OKAY
	MSI-X3:         OKAY
	MSI-X4:         OKAY
	MSI-X5:         OKAY
	MSI-X6:         OKAY
	MSI-X7:         OKAY
	MSI-X8:         OKAY
	MSI-X9:         NOT OKAY
	MSI-X10:        NOT OKAY
	MSI-X11:        NOT OKAY
	MSI-X12:        NOT OKAY
	MSI-X13:        NOT OKAY
	MSI-X14:        NOT OKAY
	MSI-X15:        NOT OKAY
	MSI-X16:        NOT OKAY
	[...]
	MSI-X2047:      NOT OKAY
	MSI-X2048:      NOT OKAY

	Read Tests

	SET IRQ TYPE TO MSI:            OKAY
	READ (      1 bytes):           OKAY
	READ (   1024 bytes):           OKAY
	READ (   1025 bytes):           OKAY
	READ (1024000 bytes):           OKAY
	READ (1024001 bytes):           OKAY

	Write Tests

	WRITE (      1 bytes):          OKAY
	WRITE (   1024 bytes):          OKAY
	WRITE (   1025 bytes):          OKAY
	WRITE (1024000 bytes):          OKAY
	WRITE (1024001 bytes):          OKAY

	Copy Tests

	COPY (      1 bytes):           OKAY
	COPY (   1024 bytes):           OKAY
	COPY (   1025 bytes):           OKAY
	COPY (1024000 bytes):           OKAY
	COPY (1024001 bytes):           OKAY
