.. SPDX-License-Identifier: GPL-2.0

=======================================
Configuring PCI Endpoint Using CONFIGFS
=======================================

:Author: Kishon Vijay Abraham I <kishon@ti.com>

The PCI Endpoint Core exposes configfs entry (pci_ep) to configure the PCI endpoint function and to bind the endpoint function with the endpoint controller. (For introducing other mechanisms to configure the PCI Endpoint Function refer to [1]).
PCI 끝점 코어는 configfs 항목(pci_ep)을 노출하여 PCI 끝점 기능을 구성하고 끝점 기능을 끝점 컨트롤러와 바인딩합니다. (PCI 엔드포인트 기능을 구성하기 위한 다른 메커니즘을 소개하려면 [1]을 참조하십시오.)

Mounting configfs
=================

The PCI Endpoint Core layer creates pci_ep directory in the mounted configfs directory. configfs can be mounted using the following command::
구성 파일 마운트
==================

PCI 엔드포인트 코어 계층은 마운트된 configfs 디렉토리에 pci_ep 디렉토리를 생성합니다. configfs는 다음 명령을 사용하여 마운트할 수 있습니다.

	mount -t configfs none /sys/kernel/config

Directory Structure
===================

The pci_ep configfs has two directories at its root: controllers and functions. Every EPC device present in the system will have an entry in the *controllers* directory and and every EPF driver present in the system will have an entry in the *functions* directory.
::
구성 파일 마운트
==================

PCI 엔드포인트 코어 계층은 마운트된 configfs 디렉토리에 pci_ep 디렉토리를 생성합니다. configfs는 다음 명령을 사용하여 마운트할 수 있습니다.

	/sys/kernel/config/pci_ep/
		.. controllers/
		.. functions/

Creating EPF Device
===================

Every registered EPF driver will be listed in controllers directory. The entries corresponding to EPF driver will be created by the EPF core.
::
EPF 장치 생성
====================

등록된 모든 EPF 드라이버는 컨트롤러 디렉토리에 나열됩니다. EPF 드라이버에 해당하는 항목은 EPF 코어에 의해 생성됩니다.
::

	/sys/kernel/config/pci_ep/functions/
		.. <EPF Driver1>/
			... <EPF Device 11>/
			... <EPF Device 21>/
		.. <EPF Driver2>/
			... <EPF Device 12>/
			... <EPF Device 22>/

In order to create a <EPF device> of the type probed by <EPF Driver>, the user has to create a directory inside <EPF DriverN>.

Every <EPF device> directory consists of the following entries that can be used to configure the standard configuration header of the endpoint function.  (These entries are created by the framework when any new <EPF Device> is created)
::
<EPF Driver>가 탐색하는 유형의 <EPF device>를 생성하기 위해서는 사용자가 <EPF DriverN> 내부에 디렉토리를 생성해야 합니다.

모든 <EPF 장치> 디렉터리는 끝점 기능의 표준 구성 헤더를 구성하는 데 사용할 수 있는 다음 항목으로 구성됩니다. (이 항목은 새 <EPF 장치>가 생성될 때 프레임워크에 의해 생성됩니다)
::

		.. <EPF Driver1>/
			... <EPF Device 11>/
				... vendorid
				... deviceid
				... revid
				... progif_code
				... subclass_code
				... baseclass_code
				... cache_line_size
				... subsys_vendor_id
				... subsys_id
				... interrupt_pin

EPC Device
==========

Every registered EPC device will be listed in controllers directory. The entries corresponding to EPC device will be created by the EPC core.
::
EPC 장치
===========

등록된 모든 EPC 장치는 컨트롤러 디렉토리에 나열됩니다. EPC 장치에 해당하는 항목은 EPC 코어에 의해 생성됩니다.
::

	/sys/kernel/config/pci_ep/controllers/
		.. <EPC Device1>/
			... <Symlink EPF Device11>/
			... <Symlink EPF Device12>/
			... start
		.. <EPC Device2>/
			... <Symlink EPF Device21>/
			... <Symlink EPF Device22>/
			... start

The <EPC Device> directory will have a list of symbolic links to <EPF Device>. These symbolic links should be created by the user to represent the functions present in the endpoint device.

The <EPC Device> directory will also have a *start* field. Once "1" is written to this field, the endpoint device will be ready to establish the link with the host. This is usually done after all the EPF devices are created and linked with the EPC device.
::
<EPC Device> 디렉토리에는 <EPF Device>에 대한 심볼릭 링크 목록이 있습니다. 이러한 심볼릭 링크는 엔드포인트 장치에 있는 기능을 나타내기 위해 사용자가 생성해야 합니다.

<EPC Device> 디렉토리에도 *start* 필드가 있습니다. 이 필드에 "1"이 기록되면 엔드포인트 장치는 호스트와의 링크를 설정할 준비가 됩니다. 이것은 일반적으로 모든 EPF 장치가 생성되고 EPC 장치와 연결된 후에 수행됩니다.
::

			 | controllers/
				| <Directory: EPC name>/
					| <Symbolic Link: Function>
					| start
			 | functions/
				| <Directory: EPF driver>/
					| <Directory: EPF device>/
						| vendorid
						| deviceid
						| revid
						| progif_code
						| subclass_code
						| baseclass_code
						| cache_line_size
						| subsys_vendor_id
						| subsys_id
						| interrupt_pin
						| function

[1] :doc:`pci-endpoint`
