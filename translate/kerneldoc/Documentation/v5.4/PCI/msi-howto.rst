.. SPDX-License-Identifier: GPL-2.0
.. include:: <isonum.txt>

==========================
The MSI Driver Guide HOWTO
==========================

:Authors: Tom L Nguyen; Martine Silbermann; Matthew Wilcox

:Copyright: 2003, 2008 Intel Corporation

About this guide
================

This guide describes the basics of Message Signaled Interrupts (MSIs), the advantages of using MSI over traditional interrupt mechanisms, how to change your driver to use MSI or MSI-X and some basic diagnostics to try if a device doesn't support MSIs.
이 가이드 정보
=================

이 가이드는 메시지 신호 인터럽트(MSI)의 기본, 기존 인터럽트 메커니즘에 비해 MSI를 사용할 때의 이점, MSI 또는 MSI-X를 사용하도록 드라이버를 변경하는 방법 및 장치가 MSI를 지원하지 않는 경우 시도할 몇 가지 기본 진단에 대해 설명합니다.



What are MSIs?
==============

A Message Signaled Interrupt is a write from the device to a special address which causes an interrupt to be received by the CPU.

The MSI capability was first specified in PCI 2.2 and was later enhanced in PCI 3.0 to allow each interrupt to be masked individually.  The MSI-X capability was also introduced with PCI 3.0.  It supports more interrupts per device than MSI and allows interrupts to be independently configured.

Devices may support both MSI and MSI-X, but only one can be enabled at a time.
MSI란 무엇입니까?
==============

메시지 신호 인터럽트는 CPU에서 인터럽트를 수신하도록 하는 장치에서 특수 주소로의 쓰기입니다.

MSI 기능은 PCI 2.2에서 처음 지정되었으며 나중에 PCI 3.0에서 향상되어 각 인터럽트를 개별적으로 마스킹할 수 있습니다. MSI-X 기능은 PCI 3.0에서도 도입되었습니다. MSI보다 장치당 더 많은 인터럽트를 지원하며 인터럽트를 독립적으로 구성할 수 있습니다.

장치는 MSI와 MSI-X를 모두 지원할 수 있지만 한 번에 하나만 활성화할 수 있습니다.



Why use MSIs?
=============

There are three reasons why using MSIs can give an advantage over traditional pin-based interrupts.

Pin-based PCI interrupts are often shared amongst several devices.  To support this, the kernel must call each interrupt handler associated with an interrupt, which leads to reduced performance for the system as a whole.  MSIs are never shared, so this problem cannot arise.

When a device writes data to memory, then raises a pin-based interrupt, it is possible that the interrupt may arrive before all the data has arrived in memory (this becomes more likely with devices behind PCI-PCI bridges).  In order to ensure that all the data has arrived in memory, the interrupt handler must read a register on the device which raised the interrupt.  PCI transaction ordering rules require that all the data arrive in memory before the value may be returned from the register.  Using MSIs avoids this problem as the interrupt-generating write cannot pass the data writes, so by the time the interrupt is raised, the driver knows that all the data has arrived in memory.

PCI devices can only support a single pin-based interrupt per function.  Often drivers have to query the device to find out what event has occurred, slowing down interrupt handling for the common case.  With MSIs, a device can support more interrupts, allowing each interrupt to be specialised to a different purpose.  One possible design gives infrequent conditions (such as errors) their own interrupt which allows the driver to handle the normal interrupt handling path more efficiently.  Other possible designs include giving one interrupt to each packet queue in a network card or each port in a storage controller.

왜 MSI를 사용합니까?
==============

MSI를 사용하면 기존의 핀 기반 인터럽트보다 이점을 얻을 수 있는 세 가지 이유가 있습니다.

핀 기반 PCI 인터럽트는 종종 여러 장치에서 공유됩니다. 이를 지원하기 위해 커널은 인터럽트와 관련된 각 인터럽트 핸들러를 호출해야 하므로 시스템 전체의 성능이 저하됩니다. MSI는 절대 공유되지 않으므로 이 문제가 발생할 수 없습니다.

장치가 메모리에 데이터를 기록한 다음 핀 기반 인터럽트를 발생시키면 모든 데이터가 메모리에 도착하기 전에 인터럽트가 도착할 수 있습니다(이는 PCI-PCI 브리지 뒤에 있는 장치에서 더 가능성이 높음). 모든 데이터가 메모리에 도착했는지 확인하기 위해 인터럽트 핸들러는 인터럽트를 발생시킨 장치의 레지스터를 읽어야 합니다. PCI 트랜잭션 순서 지정 규칙에서는 레지스터에서 값을 반환하기 전에 모든 데이터가 메모리에 도착해야 합니다. MSI를 사용하면 인터럽트 생성 쓰기가 데이터 쓰기를 전달할 수 없으므로 이 문제를 피할 수 있으므로 인터럽트가 발생할 때까지 드라이버는 모든 데이터가 메모리에 도착했음을 알게 됩니다.

PCI 장치는 기능당 단일 핀 기반 인터럽트만 지원할 수 있습니다. 종종 드라이버는 어떤 이벤트가 발생했는지 알아내기 위해 장치를 쿼리해야 하므로 일반적인 경우에 대한 인터럽트 처리 속도가 느려집니다. MSI를 사용하면 장치가 더 많은 인터럽트를 지원할 수 있으므로 각 인터럽트를 다른 목적으로 특수화할 수 있습니다. 한 가지 가능한 설계는 드라이버가 일반 인터럽트 처리 경로를 보다 효율적으로 처리할 수 있도록 하는 고유한 인터럽트(예: 오류)를 드물게 발생하는 조건에 제공합니다. 다른 가능한 디자인에는 네트워크 카드의 각 패킷 대기열 또는 저장소 컨트롤러의 각 포트에 하나의 인터럽트를 제공하는 것이 포함됩니다.

How to use MSIs
===============

PCI devices are initialised to use pin-based interrupts.  The device driver has to set up the device to use MSI or MSI-X.  Not all machines support MSIs correctly, and for those machines, the APIs described below will simply fail and the device will continue to use pin-based interrupts.
MSI 사용 방법
================

PCI 장치는 핀 기반 인터럽트를 사용하도록 초기화됩니다. 장치 드라이버는 MSI 또는 MSI-X를 사용하도록 장치를 설정해야 합니다. 모든 시스템이 MSI를 올바르게 지원하는 것은 아니며 이러한 시스템의 경우 아래에 설명된 API가 실패하고 장치는 계속해서 핀 기반 인터럽트를 사용합니다.

Include kernel support for MSIs
-------------------------------

To support MSI or MSI-X, the kernel must be built with the CONFIG_PCI_MSI option enabled.  This option is only available on some architectures, and it may depend on some other options also being set.  For example, on x86, you must also enable X86_UP_APIC or SMP in order to see the CONFIG_PCI_MSI option.

MSI에 대한 커널 지원 포함
-------------------------------------------

MSI 또는 MSI-X를 지원하려면 CONFIG_PCI_MSI 옵션이 활성화된 상태로 커널을 빌드해야 합니다. 이 옵션은 일부 아키텍처에서만 사용할 수 있으며 설정 중인 다른 옵션에 따라 달라질 수 있습니다. 예를 들어 x86에서 CONFIG_PCI_MSI 옵션을 보려면 X86_UP_APIC 또는 SMP도 활성화해야 합니다.

Using MSI
---------

Most of the hard work is done for the driver in the PCI layer.  The driver simply has to request that the PCI layer set up the MSI capability for this
device.  To automatically use MSI or MSI-X interrupt vectors, use the following
function::

  int pci_alloc_irq_vectors(struct pci_dev *dev, unsigned int min_vecs,
		unsigned int max_vecs, unsigned int flags);

which allocates up to max_vecs interrupt vectors for a PCI device.  It returns the number of vectors allocated or a negative error.  If the device has a requirements for a minimum number of vectors the driver can pass a min_vecs argument set to this limit, and the PCI core will return -ENOSPC if it can't meet the minimum number of vectors.

The flags argument is used to specify which type of interrupt can be used by the device and the driver (PCI_IRQ_LEGACY, PCI_IRQ_MSI, PCI_IRQ_MSIX).  A convenient short-hand (PCI_IRQ_ALL_TYPES) is also available to ask for any possible kind of interrupt.  If the PCI_IRQ_AFFINITY flag is set, pci_alloc_irq_vectors() will spread the interrupts around the available CPUs.

To get the Linux IRQ numbers passed to request_irq() and free_irq() and the vectors, use the following function::

  int pci_irq_vector(struct pci_dev *dev, unsigned int nr);

Any allocated resources should be freed before removing the device using the following function::

  void pci_free_irq_vectors(struct pci_dev *dev);

If a device supports both MSI-X and MSI capabilities, this API will use the MSI-X facilities in preference to the MSI facilities.  MSI-X supports any number of interrupts between 1 and 2048.  In contrast, MSI is restricted to a maximum of 32 interrupts (and must be a power of two).  In addition, the MSI interrupt vectors must be allocated consecutively, so the system might not be able to allocate as many vectors for MSI as it could for MSI-X.  On
some platforms, MSI interrupts must all be targeted at the same set of CPUs whereas MSI-X interrupts can all be targeted at different CPUs.

If a device supports neither MSI-X or MSI it will fall back to a single
legacy IRQ vector.

The typical usage of MSI or MSI-X interrupts is to allocate as many vectors as possible, likely up to the limit supported by the device.  If nvec is larger than the number supported by the device it will automatically be capped to the supported limit, so there is no need to query the number of vectors supported beforehand::

	nvec = pci_alloc_irq_vectors(pdev, 1, nvec, PCI_IRQ_ALL_TYPES)
	if (nvec < 0)
		goto out_err;

If a driver is unable or unwilling to deal with a variable number of MSI interrupts it can request a particular number of interrupts by passing that number to pci_alloc_irq_vectors() function as both 'min_vecs' and 'max_vecs' parameters::

	ret = pci_alloc_irq_vectors(pdev, nvec, nvec, PCI_IRQ_ALL_TYPES);
	if (ret < 0)
		goto out_err;

The most notorious example of the request type described above is enabling the single MSI mode for a device.  It could be done by passing two 1s as 'min_vecs' and 'max_vecs'::

	ret = pci_alloc_irq_vectors(pdev, 1, 1, PCI_IRQ_ALL_TYPES);
	if (ret < 0)
		goto out_err;

Some devices might not support using legacy line interrupts, in which case the driver can specify that only MSI or MSI-X is acceptable::

	nvec = pci_alloc_irq_vectors(pdev, 1, nvec, PCI_IRQ_MSI | PCI_IRQ_MSIX);
	if (nvec < 0)
		goto out_err;

Legacy APIs
-----------

The following old APIs to enable and disable MSI or MSI-X interrupts should not be used in new code::
레거시 API
-----------

MSI 또는 MSI-X 인터럽트를 활성화 및 비활성화하는 다음 이전 API는 새 코드에서 사용해서는 안 됩니다.

  pci_enable_msi()		/* deprecated */
  pci_disable_msi()		/* deprecated */
  pci_enable_msix_range()	/* deprecated */
  pci_enable_msix_exact()	/* deprecated */
  pci_disable_msix()		/* deprecated */

Additionally there are APIs to provide the number of supported MSI or MSI-X vectors: pci_msi_vec_count() and pci_msix_vec_count().  In general these should be avoided in favor of letting pci_alloc_irq_vectors() cap the number of vectors.  If you have a legitimate special use case for the count of vectors we might have to revisit that decision and add a pci_nr_irq_vectors() helper that handles MSI and MSI-X transparently.
또한 지원되는 MSI 또는 MSI-X 벡터의 수를 제공하는 API(pci_msi_vec_count() 및 pci_msix_vec_count())가 있습니다. 일반적으로 pci_alloc_irq_vectors()가 벡터 수에 제한을 두도록 하려면 이를 피해야 합니다. 벡터 수에 대한 합법적인 특수 사용 사례가 있는 경우 해당 결정을 다시 검토하고 MSI 및 MSI-X를 투명하게 처리하는 pci_nr_irq_vectors() 도우미를 추가해야 할 수 있습니다.

Considerations when using MSIs
------------------------------

MSI 사용 시 고려 사항
------------------------------

Spinlocks
~~~~~~~~~

Most device drivers have a per-device spinlock which is taken in the interrupt handler.  With pin-based interrupts or a single MSI, it is not necessary to disable interrupts (Linux guarantees the same interrupt will not be re-entered).  If a device uses multiple interrupts, the driver must disable interrupts while the lock is held.  If the device sends a different interrupt, the driver will deadlock trying to recursively acquire the spinlock.  Such deadlocks can be avoided by using spin_lock_irqsave() or spin_lock_irq() which disable local interrupts and acquire the lock (see Documentation/kernel-hacking/locking.rst).
스핀록
~~~~~~~~~

대부분의 장치 드라이버에는 인터럽트 처리기에서 사용되는 장치별 스핀록이 있습니다. 핀 기반 인터럽트 또는 단일 MSI를 사용하면 인터럽트를 비활성화할 필요가 없습니다(Linux는 동일한 인터럽트가 다시 입력되지 않도록 보장합니다). 장치가 여러 인터럽트를 사용하는 경우 드라이버는 잠금이 유지되는 동안 인터럽트를 비활성화해야 합니다. 장치가 다른 인터럽트를 보내면 드라이버가 스핀록을 재귀적으로 획득하려고 시도하는 교착 상태가 됩니다. 이러한 교착 상태는 로컬 인터럽트를 비활성화하고 잠금을 획득하는 spin_lock_irqsave() 또는 spin_lock_irq()를 사용하여 피할 수 있습니다(Documentation/kernel-hacking/locking.rst 참조).

How to tell whether MSI/MSI-X is enabled on a device
----------------------------------------------------

Using 'lspci -v' (as root) may show some devices with "MSI", "Message Signalled Interrupts" or "MSI-X" capabilities.  Each of these capabilities has an 'Enable' flag which is followed with either "+" (enabled) or "-" (disabled).

장치에서 MSI/MSI-X가 활성화되었는지 확인하는 방법
-------------------------------------------------- --

'lspci -v'(루트)를 사용하면 "MSI", "Message Signaled Interrupts" 또는 "MSI-X" 기능이 있는 일부 장치가 표시될 수 있습니다. 이러한 각 기능에는 "+"(활성화됨) 또는 "-"(비활성화됨)가 뒤에 오는 '활성화' 플래그가 있습니다.


MSI quirks
==========

Several PCI chipsets or devices are known not to support MSIs.  The PCI stack provides three ways to disable MSIs:

1. globally
2. on all devices behind a specific bridge
3. on a single device

MSI의 단점
===========

몇몇 PCI 칩셋 또는 장치는 MSI를 지원하지 않는 것으로 알려져 있습니다. PCI 스택은 MSI를 비활성화하는 세 가지 방법을 제공합니다.

1. 전 세계적으로
2. 특정 브리지 뒤에 있는 모든 장치에서
3. 단일 장치에서
Disabling MSIs globally
-----------------------

Some host chipsets simply don't support MSIs properly.  If we're lucky, the manufacturer knows this and has indicated it in the ACPI FADT table.  In this case, Linux automatically disables MSIs.  Some boards don't include this information in the table and so we have to detect them ourselves.  The complete list of these is found near the quirk_disable_all_msi() function in drivers/pci/quirks.c.

If you have a board which has problems with MSIs, you can pass pci=nomsi on the kernel command line to disable MSIs on all devices.  It would be in your best interests to report the problem to linux-pci@vger.kernel.org including a full 'lspci -v' so we can add the quirks to the kernel.


전역적으로 MSI 비활성화
-----------------------

일부 호스트 칩셋은 단순히 MSI를 제대로 지원하지 않습니다. 운이 좋다면 제조업체가 이를 알고 ACPI FADT 테이블에 표시했습니다. 이 경우 Linux는 자동으로 MSI를 비활성화합니다. 일부 보드는 이 정보를 테이블에 포함하지 않으므로 우리가 직접 감지해야 합니다. 이들의 전체 목록은 drivers/pci/quirks.c의 quirk_disable_all_msi() 함수 근처에서 찾을 수 있습니다.

MSI에 문제가 있는 보드가 있는 경우 커널 명령줄에서 pci=nomsi를 전달하여 모든 장치에서 MSI를 비활성화할 수 있습니다. 커널에 단점을 추가할 수 있도록 전체 'lspci -v'를 포함하여 문제를 linux-pci@vger.kernel.org에 보고하는 것이 가장 좋습니다.

Disabling MSIs below a bridge
-----------------------------

Some PCI bridges are not able to route MSIs between busses properly.  In this case, MSIs must be disabled on all devices behind the bridge.

Some bridges allow you to enable MSIs by changing some bits in their PCI configuration space (especially the Hypertransport chipsets such as the nVidia nForce and Serverworks HT2000).  As with host chipsets, Linux mostly knows about them and automatically enables MSIs if it can.  If you have a bridge unknown to Linux, you can enable MSIs in configuration space using whatever method you know works, then enable MSIs on that bridge by doing::

       echo 1 > /sys/bus/pci/devices/$bridge/msi_bus

where $bridge is the PCI address of the bridge you've enabled (eg 0000:00:0e.0).

To disable MSIs, echo 0 instead of 1.  Changing this value should be done with caution as it could break interrupt handling for all devices below this bridge.

Again, please notify linux-pci@vger.kernel.org of any bridges that need special handling.

브리지 아래에서 MSI 비활성화
--------------------------

일부 PCI 브리지는 버스 간에 MSI를 제대로 라우팅할 수 없습니다. 이 경우 브리지 뒤에 있는 모든 장치에서 MSI를 비활성화해야 합니다.

일부 브리지를 사용하면 PCI 구성 공간(특히 nVidia nForce 및 Serverworks HT2000과 같은 Hypertransport 칩셋)에서 일부 비트를 변경하여 MSI를 활성화할 수 있습니다. 호스트 칩셋과 마찬가지로 Linux는 대부분 이에 대해 알고 있으며 가능한 경우 MSI를 자동으로 활성화합니다. Linux에 알려지지 않은 브리지가 있는 경우 작동하는 방법을 사용하여 구성 공간에서 MSI를 활성화한 다음 다음을 수행하여 해당 브리지에서 MSI를 활성화할 수 있습니다.

       에코 1 > /sys/bus/pci/devices/$bridge/msi_bus

여기서 $bridge는 활성화한 브리지의 PCI 주소입니다(예: 0000:00:0e.0).

MSI를 비활성화하려면 1 대신 0을 에코합니다. 이 값을 변경하면 이 브리지 아래의 모든 장치에 대한 인터럽트 처리가 중단될 수 있으므로 주의해서 수행해야 합니다.

다시 한 번, 특별한 처리가 필요한 브리지가 있으면 linux-pci@vger.kernel.org에 알려주시기 바랍니다.


Disabling MSIs on a single device
---------------------------------

Some devices are known to have faulty MSI implementations.  Usually this is handled in the individual device driver, but occasionally it's necessary to handle this with a quirk.  Some drivers have an option to disable use of MSI.  While this is a convenient workaround for the driver author, it is not good practice, and should not be emulated.


단일 장치에서 MSI 비활성화
----------------------------------

일부 장치에는 잘못된 MSI 구현이 있는 것으로 알려져 있습니다. 일반적으로 이것은 개별 장치 드라이버에서 처리되지만 가끔은 이 문제를 기발하게 처리해야 합니다. 일부 드라이버에는 MSI 사용을 비활성화하는 옵션이 있습니다. 이것은 드라이버 작성자에게 편리한 해결 방법이지만 좋은 방법이 아니므로 에뮬레이트해서는 안 됩니다.

Finding why MSIs are disabled on a device
-----------------------------------------

From the above three sections, you can see that there are many reasons why MSIs may not be enabled for a given device.  Your first step should be to examine your dmesg carefully to determine whether MSIs are enabled for your machine.  You should also check your .config to be sure you have enabled CONFIG_PCI_MSI.

Then, 'lspci -t' gives the list of bridges above a device. Reading `/sys/bus/pci/devices/*/msi_bus` will tell you whether MSIs are enabled (1) or disabled (0).  If 0 is found in any of the msi_bus files belonging to bridges between the PCI root and the device, MSIs are disabled.

It is also worth checking the device driver to see whether it supports MSIs.  For example, it may contain calls to pci_irq_alloc_vectors() with the PCI_IRQ_MSI or PCI_IRQ_MSIX flags.



장치에서 MSI가 비활성화된 이유 찾기
------------------------------------------

위의 세 섹션에서 MSI가 특정 장치에 대해 활성화되지 않을 수 있는 많은 이유가 있음을 알 수 있습니다. 첫 번째 단계는 dmesg를 주의 깊게 검사하여 컴퓨터에 MSI가 활성화되어 있는지 확인하는 것입니다. 또한 .config를 확인하여 CONFIG_PCI_MSI를 활성화했는지 확인해야 합니다.

그런 다음 'lspci -t'는 장치 위의 브리지 목록을 제공합니다. `/sys/bus/pci/devices/*/msi_bus`를 읽으면 MSI가 활성화되었는지(1) 비활성화되었는지(0) 알 수 있습니다. PCI 루트와 장치 사이의 브리지에 속하는 msi_bus 파일에서 0이 발견되면 MSI가 비활성화됩니다.

MSI를 지원하는지 여부를 확인하기 위해 장치 드라이버를 확인하는 것도 가치가 있습니다. 예를 들어, PCI_IRQ_MSI 또는 PCI_IRQ_MSIX 플래그를 사용하여 pci_irq_alloc_vectors()에 대한 호출을 포함할 수 있습니다.