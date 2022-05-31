.. SPDX-License-Identifier: GPL-2.0
.. include:: <isonum.txt>

====================================
PCI Express I/O Virtualization Howto
====================================

:Copyright: |copy| 2009 Intel Corporation
:Authors: - Yu Zhao <yu.zhao@intel.com>
          - Donald Dutile <ddutile@redhat.com>

Overview
========

What is SR-IOV
--------------

Single Root I/O Virtualization (SR-IOV) is a PCI Express Extended
capability which makes one physical device appear as multiple virtual
devices. The physical device is referred to as Physical Function (PF)
while the virtual devices are referred to as Virtual Functions (VF).
Allocation of the VF can be dynamically controlled by the PF via
registers encapsulated in the capability. By default, this feature is
not enabled and the PF behaves as traditional PCIe device. Once it's
turned on, each VF's PCI configuration space can be accessed by its own
Bus, Device and Function Number (Routing ID). And each VF also has PCI
Memory Space, which is used to map its register set. VF device driver
operates on the register set so it can be functional and appear as a
real existing PCI device.
SR-IOV(Single Root I/O Virtualization)는 하나의 물리적 디바이스를 여러 가상 디바이스로 표시하는 PCI Express 확장 기능입니다. 물리적 디바이스를 PF(Physical Function)라고 하며 가상 디바이스를 VF(Virtual Function)라고 합니다. VF의 할당은 기능에 캡슐화된 레지스터를 통해 PF에 의해 동적으로 제어될 수 있다. 기본적으로 이 기능은 활성화되지 않으며 PF는 기존 PCIe 디바이스로 작동합니다. VF가 켜지면 자체 버스, 장치 및 기능 번호(라우팅 ID)를 통해 각 VF의 PCI 구성 공간에 액세스할 수 있습니다. 또한 각 VF에는 레지스터 세트를 매핑하는 데 사용되는 PCI 메모리 공간도 있습니다. VF 디바이스 드라이버는 레지스터 세트에서 작동하므로 실제 기존 PCI 디바이스로 표시될 수 있습니다.

User Guide
==========

How can I enable SR-IOV capability
----------------------------------

Multiple methods are available for SR-IOV enablement.
In the first method, the device driver (PF driver) will control the
enabling and disabling of the capability via API provided by SR-IOV core.
If the hardware has SR-IOV capability, loading its PF driver would
enable it and all VFs associated with the PF.  Some PF drivers require
a module parameter to be set to determine the number of VFs to enable.
In the second method, a write to the sysfs file sriov_numvfs will
enable and disable the VFs associated with a PCIe PF.  This method
enables per-PF, VF enable/disable values versus the first method,
which applies to all PFs of the same device.  Additionally, the
PCI SRIOV core support ensures that enable/disable operations are
valid to reduce duplication in multiple drivers for the same
checks, e.g., check numvfs == 0 if enabling VFs, ensure
numvfs <= totalvfs.
The second method is the recommended method for new/future VF devices.
SR-IOV 활성화에는 여러 가지 방법을 사용할 수 있습니다. 첫 번째 방법에서 장치 드라이버(PF 드라이버)는 SR-IOV 코어가 제공하는 API를 통해 기능의 활성화와 비활성화를 제어한다. 하드웨어에 SR-IOV 기능이 있는 경우 PF 드라이버를 로드하면 하드웨어와 PF와 관련된 모든 VF가 활성화된다. 일부 PF 드라이버는 활성화할 VF 수를 결정하기 위해 모듈 매개 변수를 설정해야 합니다. 두 번째 방법에서는 sysfs 파일 sriov_numvfs에 쓰기를 수행하면 PCIe PF와 관련된 VF를 사용하거나 사용하지 않도록 설정할 수 있습니다. 이 방법은 동일한 장치의 모든 PF에 적용되는 첫 번째 방법 대비 PF 단위, VF 활성화/비활성화 값을 활성화한다. 또한 PCI SRIOV 코어 지원을 통해 사용/사용 안 함 작업이 유효하여 동일한 검사에서 여러 드라이버의 중복을 줄일 수 있습니다. 예를 들어, VF를 사용하도록 설정한 경우 numvfs == 0을 확인하고 numvfs < = totalvfs를 확인하십시오. 두 번째 방법은 새로운/미래 VF 장치에 권장되는 방법입니다.

How can I use the Virtual Functions
-----------------------------------

The VF is treated as hot-plugged PCI devices in the kernel, so they
should be able to work in the same way as real PCI devices. The VF
requires device driver that is same as a normal PCI device's.
VF는 커널에서 핫 플러그 PCI 디바이스로 처리되므로 실제 PCI 디바이스와 동일한 방식으로 작동할 수 있어야 합니다. VF에는 일반 PCI 디바이스와 동일한 디바이스 드라이버가 필요합니다.

Developer Guide
===============

SR-IOV API
----------

To enable SR-IOV capability:
SR-IOV 기능을 사용하려면:

(a) For the first method, in the driver::
(a) 첫 번째 방법은 드라이버에서::


	int pci_enable_sriov(struct pci_dev *dev, int nr_virtfn);

'nr_virtfn' is number of VFs to be enabled.
'nr_virtfn'은 사용하도록 설정할 VF 수입니다.

(b) For the second method, from sysfs::
(b) 두 번째 방법의 경우 sysfs에서::

	echo 'nr_virtfn' > \
        /sys/bus/pci/devices/<DOMAIN:BUS:DEVICE.FUNCTION>/sriov_numvfs

To disable SR-IOV capability:
SR-IOV 기능을 사용하지 않도록 설정하려면:

(a) For the first method, in the driver::
(a) 첫 번째 방법은 드라이버에서::

	void pci_disable_sriov(struct pci_dev *dev);

(b) For the second method, from sysfs::
(b) 두 번째 방법의 경우 sysfs에서::

	echo  0 > \
        /sys/bus/pci/devices/<DOMAIN:BUS:DEVICE.FUNCTION>/sriov_numvfs

To enable auto probing VFs by a compatible driver on the host, run
command below before enabling SR-IOV capabilities. This is the
default behavior.
::
호스트의 호환되는 드라이버에서 VF 자동 프로빙을 사용하도록 설정하려면 SR-IOV 기능을 사용하도록 설정하기 전에 아래 명령을 실행합니다. 이것이 기본 동작입니다. ::

	echo 1 > \
        /sys/bus/pci/devices/<DOMAIN:BUS:DEVICE.FUNCTION>/sriov_drivers_autoprobe

To disable auto probing VFs by a compatible driver on the host, run
command below before enabling SR-IOV capabilities. Updating this
entry will not affect VFs which are already probed.
::
호스트에서 호환되는 드라이버의 VF 자동 프로빙을 사용하지 않도록 설정하려면 SR-IOV 기능을 사용하도록 설정하기 전에 아래 명령을 실행하십시오. 이 항목을 업데이트해도 이미 프로빙된 VF에는 영향을 주지 않습니다. ::

	echo  0 > \
        /sys/bus/pci/devices/<DOMAIN:BUS:DEVICE.FUNCTION>/sriov_drivers_autoprobe

Usage example
-------------

Following piece of code illustrates the usage of the SR-IOV API.
::
다음 코드는 SR-IOV API의 사용을 보여줍니다. ::

	static int dev_probe(struct pci_dev *dev, const struct pci_device_id *id)
	{
		pci_enable_sriov(dev, NR_VIRTFN);

		...

		return 0;
	}

	static void dev_remove(struct pci_dev *dev)
	{
		pci_disable_sriov(dev);

		...
	}

	static int dev_suspend(struct pci_dev *dev, pm_message_t state)
	{
		...

		return 0;
	}

	static int dev_resume(struct pci_dev *dev)
	{
		...

		return 0;
	}

	static void dev_shutdown(struct pci_dev *dev)
	{
		...
	}

	static int dev_sriov_configure(struct pci_dev *dev, int numvfs)
	{
		if (numvfs > 0) {
			...
			pci_enable_sriov(dev, numvfs);
			...
			return numvfs;
		}
		if (numvfs == 0) {
			....
			pci_disable_sriov(dev);
			...
			return 0;
		}
	}

	static struct pci_driver dev_driver = {
		.name =		"SR-IOV Physical Function driver",
		.id_table =	dev_id_table,
		.probe =	dev_probe,
		.remove =	dev_remove,
		.suspend =	dev_suspend,
		.resume =	dev_resume,
		.shutdown =	dev_shutdown,
		.sriov_configure = dev_sriov_configure,
	};
