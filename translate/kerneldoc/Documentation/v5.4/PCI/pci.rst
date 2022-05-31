.. SPDX-License-Identifier: GPL-2.0

==============================
How To Write Linux PCI Drivers
==============================

:Authors: - Martin Mares <mj@ucw.cz>
          - Grant Grundler <grundler@parisc-linux.org>

The world of PCI is vast and full of (mostly unpleasant) surprises.  Since each CPU architecture implements different chip-sets and PCI devices have different requirements (erm, "features"), the result is the PCI support in the Linux kernel is not as trivial as one would wish. This short paper tries to introduce all potential driver authors to Linux APIs for PCI device drivers.
PCI의 세계는 광대하고 (대부분 불쾌한) 놀라움으로 가득 차 있습니다. 각 CPU 아키텍처는 서로 다른 칩셋을 구현하고 PCI 장치는 요구 사항(erm, "기능")이 다르기 때문에 결과적으로 Linux 커널에서 PCI 지원이 원하는 만큼 간단하지 않습니다. 이 짧은 문서에서는 모든 잠재적인 드라이버 작성자를 PCI 장치 드라이버용 Linux API에 대해 소개하려고 합니다.

A more complete resource is the third edition of "Linux Device Drivers" by Jonathan Corbet, Alessandro Rubini, and Greg Kroah-Hartman.  LDD3 is available for free (under Creative Commons License) from: http://lwn.net/Kernel/LDD3/.
보다 완전한 리소스는 Jonathan Corbet, Alessandro Rubini 및 Greg Kroah-Hartman의 "Linux Device Drivers" 제3판입니다. LDD3는 http://lwn.net/Kernel/LDD3/에서 무료로(Creative Commons License에 따라) 사용할 수 있습니다.

However, keep in mind that all documents are subject to "bit rot".  Refer to the source code if things are not working as described here.
그러나 모든 문서는 "비트 썩음"이 발생할 수 있습니다. 여기에 설명된 대로 작동하지 않는 경우 소스 코드를 참조하십시오.

Please send questions/comments/patches about Linux PCI API to the "Linux PCI" <linux-pci@atrey.karlin.mff.cuni.cz> mailing list.
Linux PCI API에 대한 질문/의견/패치를 "Linux PCI" <linux-pci@atrey.karlin.mff.cuni.cz> 메일링 리스트로 보내주십시오.

Structure of PCI drivers
========================
PCI drivers "discover" PCI devices in a system via pci_register_driver().  Actually, it's the other way around. When the PCI generic code discovers a new device, the driver with a matching "description" will be notified.  Details on this below.
PCI 드라이버는 pci_register_driver()를 통해 시스템에서 PCI 장치를 "검색"합니다. 사실, 그 반대입니다. PCI 일반 코드가 새 장치를 발견하면 일치하는 "설명"이 있는 드라이버에 알림이 전송됩니다. 이에 대한 자세한 내용은 아래에 있습니다.

pci_register_driver() leaves most of the probing for devices to the PCI layer and supports online insertion/removal of devices [thus supporting hot-pluggable PCI, CardBus, and Express-Card in a single driver].  pci_register_driver() call requires passing in a table of function
pointers and thus dictates the high level structure of a driver.
pci_register_driver()는 대부분의 장치 검색을 PCI 계층에 맡기고 장치의 온라인 삽입/제거를 지원합니다[따라서 단일 드라이버에서 핫플러그 가능한 PCI, CardBus 및 Express-Card 지원]. pci_register_driver() 호출은 함수 테이블을 전달해야 합니다.
포인터는 드라이버의 상위 레벨 구조를 지시합니다.

Once the driver knows about a PCI device and takes ownership, the driver generally needs to perform the following initialization:
드라이버가 PCI 장치에 대해 알고 소유권을 가져오면 일반적으로 드라이버는 다음 초기화를 수행해야 합니다.

  - Enable the device
  - 장치 활성화
  - Request MMIO/IOP resources
  - MMIO/IOP 리소스 요청
  - Set the DMA mask size (for both coherent and streaming DMA)
 - DMA 마스크 크기 설정(코히어런트 및 스트리밍 DMA 모두에 대해)
  - Allocate and initialize shared control data (pci_allocate_coherent())
  - 공유 제어 데이터 할당 및 초기화(pci_allocate_coherent())
  - Access device configuration space (if needed)
  - 장치 구성 공간에 액세스(필요한 경우)
  - Register IRQ handler (request_irq())
  - IRQ 핸들러 등록(request_irq())
  - Initialize non-PCI (i.e. LAN/SCSI/etc parts of the chip)
  - non-PCI 초기화(예: LAN/SCSI/칩의 일부)
  - Enable DMA/processing engines
  - DMA/처리 엔진 활성화

When done using the device, and perhaps the module needs to be unloaded, the driver needs to take the follow steps:
장치 사용이 완료되고 모듈을 언로드해야 할 경우 드라이버는 다음 단계를 수행해야 합니다.

  - Disable the device from generating IRQs
  - 장치가 IRQ를 생성하지 못하도록 비활성화
  - Release the IRQ (free_irq())
  - IRQ 해제(free_irq())
  - Stop all DMA activity
  - 모든 DMA 활동 중지
  - Release DMA buffers (both streaming and coherent)
  - DMA 버퍼 해제(스트리밍 및 일관성 모두)
  - Unregister from other subsystems (e.g. scsi or netdev)
  - 다른 하위 시스템(예: scsi 또는 netdev)에서 등록 취소
  - Release MMIO/IOP resources
  - MMIO/IOP 리소스 해제
  - Disable the device
  - 장치 비활성화

Most of these topics are covered in the following sections.  For the rest look at LDD3 or <linux/pci.h> .
이러한 주제의 대부분은 다음 섹션에서 다룹니다. 나머지는 LDD3 또는 <linux/pci.h>를 보십시오.

If the PCI subsystem is not configured (CONFIG_PCI is not set), most of the PCI functions described below are defined as inline functions either completely empty or just returning an appropriate error codes to avoid lots of ifdefs in the drivers.
PCI 하위 시스템이 구성되지 않은 경우(CONFIG_PCI가 설정되지 않음) 아래에 설명된 대부분의 PCI 기능은 드라이버에서 많은 ifdef를 피하기 위해 완전히 비어 있거나 적절한 오류 코드를 반환하는 인라인 기능으로 정의됩니다.

pci_register_driver() call
==========================

PCI device drivers call ``pci_register_driver()`` during their initialization with a pointer to a structure describing the driver
(``struct pci_driver``):
PCI 장치 드라이버는 초기화 중에 드라이버를 설명하는 구조에 대한 포인터로 ``pci_register_driver()``를 호출합니다.
(``구조체 pci_driver``):

.. kernel-doc:: include/linux/pci.h
   :functions: pci_driver

The ID table is an array of ``struct pci_device_id`` entries ending with an all-zero entry.  Definitions with static const are generally preferred.
ID 테이블은 모두 0인 항목으로 끝나는 ``struct pci_device_id`` 항목의 배열입니다. 정적 const가 있는 정의가 일반적으로 선호됩니다.

.. kernel-doc:: include/linux/mod_devicetable.h
   :functions: pci_device_id

Most drivers only need ``PCI_DEVICE()`` or ``PCI_DEVICE_CLASS()`` to set up a pci_device_id table.
대부분의 드라이버는 pci_device_id 테이블을 설정하기 위해 ``PCI_DEVICE()`` 또는 ``PCI_DEVICE_CLASS()``만 있으면 됩니다.

New PCI IDs may be added to a device driver pci_ids table at runtime as shown below::
새 PCI ID는 아래와 같이 런타임 시 장치 드라이버 pci_ids 테이블에 추가될 수 있습니다.

  echo "vendor device subvendor subdevice class class_mask driver_data" > \
  /sys/bus/pci/drivers/{driver}/new_id

All fields are passed in as hexadecimal values (no leading 0x).  The vendor and device fields are mandatory, the others are optional. Users need pass only as many optional fields as necessary:
모든 필드는 16진수 값으로 전달됩니다(선행 0x 없음). 공급업체 및 장치 필드는 필수이고 나머지는 선택 사항입니다. 사용자는 필요한 만큼만 선택적 필드를 전달해야 합니다.

  - subvendor and subdevice fields default to PCI_ANY_ID (FFFFFFFF)
  - 하위 공급업체 및 하위 장치 필드는 기본적으로 PCI_ANY_ID(FFFFFFFF)로 설정됩니다.
  - class and classmask fields default to 0
  - class 및 classmask 필드의 기본값은 0입니다.
  - driver_data defaults to 0UL.
  - driver_data의 기본값은 0UL입니다.

Note that driver_data must match the value used by any of the pci_device_id entries defined in the driver. This makes the driver_data field mandatory if all the pci_device_id entries have a non-zero driver_data value.
driver_data는 드라이버에 정의된 pci_device_id 항목이 사용하는 값과 일치해야 합니다. 모든 pci_device_id 항목에 0이 아닌 driver_data 값이 있는 경우 driver_data 필드가 필수가 됩니다.

Once added, the driver probe routine will be invoked for any unclaimed PCI devices listed in its (newly updated) pci_ids list.
일단 추가되면 (새로 업데이트된) pci_ids 목록에 나열된 청구되지 않은 PCI 장치에 대해 드라이버 프로브 루틴이 호출됩니다.

When the driver exits, it just calls pci_unregister_driver() and the PCI layer automatically calls the remove hook for all devices handled by the driver.
드라이버가 종료되면 pci_unregister_driver()를 호출하고 PCI 계층은 드라이버가 처리하는 모든 장치에 대해 제거 후크를 자동으로 호출합니다.

"Attributes" for driver functions/data
--------------------------------------

Please mark the initialization and cleanup functions where appropriate (the corresponding macros are defined in <linux/init.h>):
적절한 곳에 초기화 및 정리 기능을 표시하십시오(해당 매크로는 <linux/init.h>에 정의되어 있습니다):

	======		=================================================
	__init		Initialization code. Thrown away after the driver
			initializes.
	__exit		Exit code. Ignored for non-modular drivers.
	======		=================================================

Tips on when/where to use the above attributes:
위의 속성을 언제/어디서 사용해야 하는지에 대한 팁:
	- The module_init()/module_exit() functions (and all initialization functions called _only_ from these) should be marked __init/__exit.
        - module_init()/module_exit() 함수(및 이들 중 _only_라고 하는 모든 초기화 함수)는 __init/__exit로 표시해야 합니다.
	- Do not mark the struct pci_driver.
        - 구조체 pci_driver를 표시하지 마십시오.
	- Do NOT mark a function if you are not sure which mark to use.  Better to not mark the function than mark the function wrong.
        - 어떤 표시를 사용할지 확실하지 않은 경우 기능을 표시하지 마십시오. 기능을 잘못 표시하는 것보다 기능을 표시하지 않는 것이 좋습니다.

How to find PCI devices manually
================================

PCI drivers should have a really good reason for not using the pci_register_driver() interface to search for PCI devices.  The main reason PCI devices are controlled by multiple drivers is because one PCI device implements several different HW services.  E.g. combined serial/parallel port/floppy controller.
PCI 드라이버는 PCI 장치를 검색하기 위해 pci_register_driver() 인터페이스를 사용하지 않는 데는 정말 좋은 이유가 있어야 합니다. PCI 장치가 여러 드라이버에 의해 제어되는 주된 이유는 하나의 PCI 장치가 여러 다른 HW 서비스를 구현하기 때문입니다. 예를 들어 직렬/병렬 포트/플로피 컨트롤러 결합.

A manual search may be performed using the following constructs:
수동 검색은 다음 구성을 사용하여 수행할 수 있습니다.

Searching by vendor and device ID::
공급업체 및 장치 ID로 검색::

	struct pci_dev *dev = NULL;
	while (dev = pci_get_device(VENDOR_ID, DEVICE_ID, dev))
		configure_device(dev);

Searching by class ID (iterate in a similar way)::
클래스 ID로 검색(비슷한 방식으로 반복)::

	pci_get_class(CLASS_ID, dev)

Searching by both vendor/device and subsystem vendor/device ID::
공급업체/장치 및 하위 시스템 공급업체/장치 ID로 검색::
	pci_get_subsys(VENDOR_ID,DEVICE_ID, SUBSYS_VENDOR_ID, SUBSYS_DEVICE_ID, dev).

You can use the constant PCI_ANY_ID as a wildcard replacement for VENDOR_ID or DEVICE_ID.  This allows searching for any device from a specific vendor, for example.
pci_get_subsys(VENDOR_ID,DEVICE_ID, SUBSYS_VENDOR_ID, SUBSYS_DEVICE_ID, 개발).
VENDOR_ID 또는 DEVICE_ID에 대한 와일드카드 대체로 상수 PCI_ANY_ID를 사용할 수 있습니다. 이를 통해 예를 들어 특정 공급업체의 모든 장치를 검색할 수 있습니다.
pci_get_subsys(VENDOR_ID,DEVICE_ID, SUBSYS_VENDOR_ID, SUBSYS_DEVICE_ID, 개발).


Device Initialization Steps
===========================

As noted in the introduction, most PCI drivers need the following steps for device initialization:
소개에서 언급했듯이 대부분의 PCI 드라이버는 장치 초기화를 위해 다음 단계가 필요합니다.

  - Enable the device
  - Request MMIO/IOP resources
  - Set the DMA mask size (for both coherent and streaming DMA)
  - Allocate and initialize shared control data (pci_allocate_coherent())
  - Access device configuration space (if needed)
  - Register IRQ handler (request_irq())
  - Initialize non-PCI (i.e. LAN/SCSI/etc parts of the chip)
  - Enable DMA/processing engines.
  - 장치 활성화
  - MMIO/IOP 리소스 요청
  - DMA 마스크 크기 설정(코히어런트 및 스트리밍 DMA 모두에 대해)
  - 공유 제어 데이터 할당 및 초기화(pci_allocate_coherent())
  - 장치 구성 공간에 액세스(필요한 경우)
  - IRQ 핸들러 등록(request_irq())
  - non-PCI 초기화(예: LAN/SCSI/칩의 일부)
  - DMA/처리 엔진을 활성화합니다.

The driver can access PCI config space registers at any time.  (Well, almost. When running BIST, config space can go away...but that will just result in a PCI Bus Master Abort and config reads will return garbage).
드라이버는 언제든지 PCI 구성 공간 레지스터에 액세스할 수 있습니다. (음, 거의. BIST를 실행할 때 구성 공간이 없어질 수 있습니다. 그러나 PCI 버스 마스터 중단이 발생하고 구성 읽기가 쓰레기를 반환합니다).

Enable the PCI device
---------------------
Before touching any device registers, the driver needs to enable the PCI device by calling pci_enable_device(). This will:
장치 레지스터를 건드리기 전에 드라이버는 pci_enable_device()를 호출하여 PCI 장치를 활성화해야 합니다. 이렇게 하면:

  - wake up the device if it was in suspended state,
  - allocate I/O and memory regions of the device (if BIOS did not),
  - allocate an IRQ (if BIOS did not).
  - 일시 중단 상태인 경우 장치를 깨우고,
  - 장치의 I/O 및 메모리 영역 할당(BIOS가 할당하지 않은 경우)
  - IRQ를 할당합니다(BIOS가 할당하지 않은 경우).

.. note::
   pci_enable_device() can fail! Check the return value.
.. 노트::
   pci_enable_device()가 실패할 수 있습니다! 반환 값을 확인하십시오.

.. warning::
   OS BUG: we don't check resource allocations before enabling those resources. The sequence would make more sense if we called pci_request_resources() before calling pci_enable_device().  Currently, the device drivers can't detect the bug when when two devices have been allocated the same range. This is not a common problem and unlikely to get fixed soon.
.. 경고::
   OS 버그: 해당 리소스를 활성화하기 전에 리소스 할당을 확인하지 않습니다. pci_enable_device()를 호출하기 전에 pci_request_resources()를 호출하면 시퀀스가 더 의미가 있습니다. 현재 장치 드라이버는 두 장치에 동일한 범위가 할당된 경우 버그를 감지할 수 없습니다. 이것은 일반적인 문제가 아니며 곧 해결될 가능성이 낮습니다.

   This has been discussed before but not changed as of 2.6.19:
   이것은 이전에 논의되었지만 2.6.19에서 변경되지 않았습니다.
   http://lkml.org/lkml/2006/3/2/194


pci_set_master() will enable DMA by setting the bus master bit in the PCI_COMMAND register. It also fixes the latency timer value if it's set to something bogus by the BIOS.  pci_clear_master() will disable DMA by clearing the bus master bit.
pci_set_master()는 PCI_COMMAND 레지스터에서 버스 마스터 비트를 설정하여 DMA를 활성화합니다. 또한 BIOS에서 가짜로 설정한 경우 대기 시간 타이머 값을 수정합니다. pci_clear_master()는 버스 마스터 비트를 지워서 DMA를 비활성화합니다.

If the PCI device can use the PCI Memory-Write-Invalidate transaction, call pci_set_mwi().  This enables the PCI_COMMAND bit for Mem-Wr-Inval
and also ensures that the cache line size register is set correctly.  Check the return value of pci_set_mwi() as not all architectures or chip-sets may support Memory-Write-Invalidate.  Alternatively, if Mem-Wr-Inval would be nice to have but is not required, call pci_try_set_mwi() to have the system do its best effort at enabling Mem-Wr-Inval.
PCI 장치가 PCI 메모리 쓰기 무효화 트랜잭션을 사용할 수 있으면 pci_set_mwi()를 호출하십시오. 이것은 Mem-Wr-Inval에 대한 PCI_COMMAND 비트를 활성화합니다.
또한 캐시 라인 크기 레지스터가 올바르게 설정되었는지 확인합니다. 모든 아키텍처 또는 칩셋이 Memory-Write-Invalidate를 지원하지 않을 수 있으므로 pci_set_mwi()의 반환 값을 확인하십시오. 또는 Mem-Wr-Inval이 있으면 좋지만 필요하지 않은 경우 시스템이 Mem-Wr-Inval을 활성화하기 위해 최선을 다하도록 pci_try_set_mwi()를 호출합니다.

Request MMIO/IOP resources
--------------------------
Memory (MMIO), and I/O port addresses should NOT be read directly from the PCI device config space. Use the values in the pci_dev structure as the PCI "bus address" might have been remapped to a "host physical" address by the arch/chip-set specific kernel support.
메모리(MMIO) 및 I/O 포트 주소는 PCI 장치 구성 공간에서 직접 읽어서는 안 됩니다. PCI "버스 주소"가 아치/칩셋 특정 커널 지원에 의해 "호스트 물리적" 주소로 다시 매핑되었을 수 있으므로 pci_dev 구조의 값을 사용하십시오.

See Documentation/io-mapping.txt for how to access device registers or device memory.
장치 레지스터 또는 장치 메모리에 액세스하는 방법은 Documentation/io-mapping.txt를 참조하십시오.

The device driver needs to call pci_request_region() to verify no other device is already using the same address resource.  Conversely, drivers should call pci_release_region() AFTER calling pci_disable_device().  The idea is to prevent two devices colliding on the same address range.
장치 드라이버는 pci_request_region()을 호출하여 다른 장치가 이미 동일한 주소 리소스를 사용하고 있지 않은지 확인해야 합니다. 반대로 드라이버는 pci_disable_device()를 호출한 후 pci_release_region()을 호출해야 합니다. 아이디어는 동일한 주소 범위에서 두 장치가 충돌하는 것을 방지하는 것입니다.

.. tip::
   See OS BUG comment above. Currently (2.6.19), The driver can only determine MMIO and IO Port resource availability _after_ calling pci_enable_device().
.. 팁::
   위의 OS BUG 주석을 참조하십시오. 현재(2.6.19), 드라이버는 pci_enable_device()를 호출한 후에 MMIO 및 IO 포트 리소스 가용성만 결정할 수 있습니다.

Generic flavors of pci_request_region() are request_mem_region() (for MMIO ranges) and request_region() (for IO Port ranges).  Use these for address resources that are not described by "normal" PCI BARs.
pci_request_region()의 일반적인 특징은 request_mem_region()(MMIO 범위의 경우) 및 request_region()(IO 포트 범위의 경우)입니다. "일반" PCI BAR에서 설명하지 않는 주소 리소스에 이를 사용합니다.

Also see pci_request_selected_regions() below.
아래의 pci_request_selected_regions()도 참조하십시오.

Set the DMA mask size
---------------------
.. note::
   If anything below doesn't make sense, please refer to Documentation/DMA-API.txt. This section is just a reminder that drivers need to indicate DMA capabilities of the device and is not an authoritative source for DMA interfaces.
.. 노트::
   아래 내용이 이해가 되지 않는 경우 Documentation/DMA-API.txt를 참조하세요. 이 섹션은 드라이버가 장치의 DMA 기능을 표시해야 하며 DMA 인터페이스에 대한 권한 있는 소스가 아님을 상기시켜줍니다.

While all drivers should explicitly indicate the DMA capability (e.g. 32 or 64 bit) of the PCI bus master, devices with more than 32-bit bus master capability for streaming data need the driver to "register" this capability by calling pci_set_dma_mask() with appropriate parameters.  In general this allows more efficient DMA on systems where System RAM exists above 4G _physical_ address.
모든 드라이버는 PCI 버스 마스터의 DMA 기능(예: 32 또는 64비트)을 명시적으로 나타내야 하지만 스트리밍 데이터를 위한 32비트 이상의 버스 마스터 기능을 가진 장치는 드라이버가 pci_set_dma_mask()를 호출하여 이 기능을 "등록"해야 합니다. 적절한 매개변수. 일반적으로 이것은 시스템 RAM이 4G _물리적_ 주소보다 높은 시스템에서 보다 효율적인 DMA를 허용합니다.

Drivers for all PCI-X and PCIe compliant devices must call pci_set_dma_mask() as they are 64-bit DMA devices.
모든 PCI-X 및 PCIe 호환 장치용 드라이버는 64비트 DMA 장치이므로 pci_set_dma_mask()를 호출해야 합니다.

Similarly, drivers must also "register" this capability if the device can directly address "consistent memory" in System RAM above 4G physical address by calling pci_set_consistent_dma_mask().  Again, this includes drivers for all PCI-X and PCIe compliant devices.  Many 64-bit "PCI" devices (before PCI-X) and some PCI-X devices are 64-bit DMA capable for payload ("streaming") data but not control ("consistent") data.
 마찬가지로, 드라이버는 pci_set_consistent_dma_mask()를 호출하여 장치가 4G 물리적 주소보다 높은 시스템 RAM의 "일관된 메모리"에 직접 주소를 지정할 수 있는 경우 이 기능을 "등록"해야 합니다. 다시 말하지만 여기에는 모든 PCI-X 및 PCIe 호환 장치용 드라이버가 포함됩니다. 많은 64비트 "PCI" 장치(PCI-X 이전)와 일부 PCI-X 장치는 페이로드("스트리밍") 데이터에 대해서는 64비트 DMA가 가능하지만("일관된") 데이터를 제어할 수는 없습니다.

Setup shared control data
-------------------------
Once the DMA masks are set, the driver can allocate "consistent" (a.k.a. shared) memory.  See Documentation/DMA-API.txt for a full description of the DMA APIs. This section is just a reminder that it needs to be done before enabling DMA on the device.
DMA 마스크가 설정되면 드라이버는 "일관된"(일명 공유) 메모리를 할당할 수 있습니다. DMA API에 대한 전체 설명은 Documentation/DMA-API.txt를 참조하세요. 이 섹션은 장치에서 DMA를 활성화하기 전에 완료해야 함을 상기시켜줍니다.


Initialize device registers
---------------------------
Some drivers will need specific "capability" fields programmed or other "vendor specific" register initialized or reset.  E.g. clearing pending interrupts.
일부 드라이버는 프로그래밍된 특정 "기능" 필드 또는 기타 "공급업체별" 레지스터 초기화 또는 재설정이 필요합니다. 예를 들어 보류 중인 인터럽트 지우기.


Register IRQ handler
--------------------
While calling request_irq() is the last step described here, this is often just another intermediate step to initialize a device.  This step can often be deferred until the device is opened for use.  All interrupt handlers for IRQ lines should be registered with IRQF_SHARED and use the devid to map IRQs to devices (remember that all PCI IRQ lines
can be shared).
여기에서 설명하는 마지막 단계인 request_irq()를 호출하는 것이지만 이는 종종 장치를 초기화하기 위한 또 다른 중간 단계일 뿐입니다. 이 단계는 종종 장치가 사용을 위해 열릴 때까지 연기될 수 있습니다. IRQ 라인에 대한 모든 인터럽트 핸들러는 IRQF_SHARED에 등록되어야 하고 devid를 사용하여 IRQ를 장치에 매핑해야 합니다(모든 PCI IRQ 라인이
공유 가능).

request_irq() will associate an interrupt handler and device handle with an interrupt number. Historically interrupt numbers represent IRQ lines which run from the PCI device to the Interrupt controller.  With MSI and MSI-X (more below) the interrupt number is a CPU "vector".
request_irq()는 인터럽트 핸들러 및 장치 핸들을 인터럽트 번호와 연관시킵니다. 역사적으로 인터럽트 번호는 PCI 장치에서 인터럽트 컨트롤러로 실행되는 IRQ 라인을 나타냅니다. MSI 및 MSI-X(아래 참조)에서 인터럽트 번호는 CPU "벡터"입니다.

request_irq() also enables the interrupt. Make sure the device is quiesced and does not have any interrupts pending before registering the interrupt handler.
request_irq()는 또한 인터럽트를 활성화합니다. 인터럽트 핸들러를 등록하기 전에 장치가 정지되어 있고 보류 중인 인터럽트가 없는지 확인하십시오.

MSI and MSI-X are PCI capabilities. Both are "Message Signaled Interrupts" which deliver interrupts to the CPU via a DMA write to a Local APIC.  The fundamental difference between MSI and MSI-X is how multiple "vectors" get allocated. MSI requires contiguous blocks of vectors while MSI-X can allocate several individual ones.
MSI 및 MSI-X는 PCI 기능입니다. 둘 다 로컬 APIC에 대한 DMA 쓰기를 통해 CPU에 인터럽트를 전달하는 "메시지 신호 인터럽트"입니다. MSI와 MSI-X의 근본적인 차이점은 여러 "벡터"가 할당되는 방식입니다. MSI는 연속적인 벡터 블록이 필요하지만 MSI-X는 여러 개의 개별 블록을 할당할 수 있습니다.

MSI capability can be enabled by calling pci_alloc_irq_vectors() with the PCI_IRQ_MSI and/or PCI_IRQ_MSIX flags before calling request_irq(). This causes the PCI support to program CPU vector data into the PCI device capability registers. Many architectures, chip-sets, or BIOSes do NOT support MSI or MSI-X and a call to pci_alloc_irq_vectors with just the PCI_IRQ_MSI and PCI_IRQ_MSIX flags will fail, so try to always specify PCI_IRQ_LEGACY as well.
MSI 기능은 request_irq()를 호출하기 전에 PCI_IRQ_MSI 및/또는 PCI_IRQ_MSIX 플래그와 함께 pci_alloc_irq_vectors()를 호출하여 활성화할 수 있습니다. 이로 인해 PCI 지원은 CPU 벡터 데이터를 PCI 장치 기능 레지스터에 프로그래밍합니다. 많은 아키텍처, 칩셋 또는 BIOS는 MSI 또는 MSI-X를 지원하지 않으며 PCI_IRQ_MSI 및 PCI_IRQ_MSIX 플래그만 있는 pci_alloc_irq_vectors에 대한 호출은 실패하므로 항상 PCI_IRQ_LEGACY도 지정하십시오.

Drivers that have different interrupt handlers for MSI/MSI-X and legacy INTx should chose the right one based on the msi_enabled and msix_enabled flags in the pci_dev structure after calling pci_alloc_irq_vectors.
MSI/MSI-X 및 레거시 INTx에 대해 서로 다른 인터럽트 핸들러를 사용하는 드라이버는 pci_alloc_irq_vectors를 호출한 후 pci_dev 구조의 msi_enabled 및 msix_enabled 플래그를 기반으로 올바른 핸들러를 선택해야 합니다.

There are (at least) two really good reasons for using MSI:
MSI를 사용하는 데에는 (적어도) 두 가지 정말 좋은 이유가 있습니다.

1) MSI is an exclusive interrupt vector by definition.  This means the interrupt handler doesn't have to verify its device caused the interrupt.
1) MSI는 정의에 따른 배타적 인터럽트 벡터입니다. 이것은 인터럽트 핸들러가 인터럽트를 일으킨 장치를 확인할 필요가 없다는 것을 의미합니다.

2) MSI avoids DMA/IRQ race conditions. DMA to host memory is guaranteed to be visible to the host CPU(s) when the MSI is delivered. This is important for both data coherency and avoiding stale control data.  This guarantee allows the driver to omit MMIO reads to flush the DMA stream.
2) MSI는 DMA/IRQ 경쟁 조건을 피합니다. 호스트 메모리에 대한 DMA는 MSI가 전달될 때 호스트 CPU에서 볼 수 있도록 보장됩니다. 이는 데이터 일관성과 오래된 제어 데이터 방지 모두에 중요합니다. 이 보장을 통해 드라이버는 DMA 스트림을 플러시하기 위해 MMIO 읽기를 생략할 수 있습니다.

See drivers/infiniband/hw/mthca/ or drivers/net/tg3.c for examples of MSI/MSI-X usage.
MSI/MSI-X 사용 예는 drivers/infiniband/hw/mthca/ 또는 drivers/net/tg3.c를 참조하십시오.

PCI device shutdown
===================

When a PCI device driver is being unloaded, most of the following steps need to be performed:
PCI 장치 드라이버가 언로드될 때 대부분의 다음 단계를 수행해야 합니다.

  - Disable the device from generating IRQs
  - Release the IRQ (free_irq())
  - Stop all DMA activity
  - Release DMA buffers (both streaming and consistent)
  - Unregister from other subsystems (e.g. scsi or netdev)
  - Disable device from responding to MMIO/IO Port addresses
  - Release MMIO/IO Port resource(s)
  - 장치가 IRQ를 생성하지 못하도록 비활성화
  - IRQ 해제(free_irq())
  - 모든 DMA 활동 중지
  - DMA 버퍼 해제(스트리밍 및 일관성 모두)
  - 다른 하위 시스템(예: scsi 또는 netdev)에서 등록 취소
  - 장치가 MMIO/IO 포트 주소에 응답하지 않도록 비활성화
  - MMIO/IO 포트 리소스 해제


Stop IRQs on the device
-----------------------
How to do this is chip/device specific. If it's not done, it opens the possibility of a "screaming interrupt" if (and only if) the IRQ is shared with another device.
이 작업을 수행하는 방법은 칩/장치에 따라 다릅니다. 완료되지 않은 경우 IRQ가 다른 장치와 공유되는 경우에만 "비명을 지르는 인터럽트" 가능성이 열립니다.

When the shared IRQ handler is "unhooked", the remaining devices using the same IRQ line will still need the IRQ enabled. Thus if the "unhooked" device asserts IRQ line, the system will respond assuming it was one of the remaining devices asserted the IRQ line. Since none of the other devices will handle the IRQ, the system will "hang" until it decides the IRQ isn't going to get handled and masks the IRQ (100,000 iterations later). Once the shared IRQ is masked, the remaining devices will stop functioning properly. Not a nice situation.  
공유 IRQ 처리기가 "연결 해제"되면 동일한 IRQ 라인을 사용하는 나머지 장치는 여전히 IRQ를 활성화해야 합니다. 따라서 "후크 해제된" 장치가 IRQ 라인을 주장하면 시스템은 그것이 IRQ 라인을 주장한 나머지 장치 중 하나라고 가정하여 응답합니다. 다른 어떤 장치도 IRQ를 처리하지 않기 때문에 시스템은 IRQ가 처리되지 않고 IRQ를 마스킹할 때까지 "중단"됩니다(나중에 100,000번 반복). 공유 IRQ가 마스크되면 나머지 장치는 제대로 작동하지 않습니다. 좋은 상황이 아닙니다.

This is another reason to use MSI or MSI-X if it's available.  MSI and MSI-X are defined to be exclusive interrupts and thus are not susceptible to the "screaming interrupt" problem.
이것이 가능한 경우 MSI 또는 MSI-X를 사용하는 또 다른 이유입니다. MSI 및 MSI-X는 배타적 인터럽트로 정의되므로 "비명을 지르는 인터럽트" 문제에 취약하지 않습니다.

Release the IRQ
---------------
Once the device is quiesced (no more IRQs), one can call free_irq().  This function will return control once any pending IRQs are handled, "unhook" the drivers IRQ handler from that IRQ, and finally release the IRQ if no one else is using it.
장치가 정지되면(더 이상 IRQ가 없음) free_irq()를 호출할 수 있습니다. 이 함수는 보류 중인 IRQ가 처리되면 제어를 반환하고 해당 IRQ에서 드라이버 IRQ 처리기를 "해제"하고 다른 사람이 사용하지 않는 경우 마지막으로 IRQ를 해제합니다.

Stop all DMA activity
---------------------
It's extremely important to stop all DMA operations BEFORE attempting to deallocate DMA control data. Failure to do so can result in memory corruption, hangs, and on some chip-sets a hard crash.
DMA 제어 데이터를 할당 해제하기 전에 모든 DMA 작업을 중지하는 것이 매우 중요합니다. 그렇게 하지 않으면 메모리 손상, 중단 및 일부 칩셋에서 심각한 충돌이 발생할 수 있습니다.

Stopping DMA after stopping the IRQs can avoid races where the IRQ handler might restart DMA engines.
IRQ를 중지한 후 DMA를 중지하면 IRQ 처리기가 DMA 엔진을 다시 시작할 수 있는 경합을 피할 수 있습니다.

While this step sounds obvious and trivial, several "mature" drivers didn't get this step right in the past.
이 단계는 당연하고 사소한 것처럼 들리지만 일부 "성숙한" 운전자는 과거에 이 단계를 올바르게 수행하지 못했습니다.


Release DMA buffers
-------------------
Once DMA is stopped, clean up streaming DMA first.  I.e. unmap data buffers and return buffers to "upstream" owners if there is one.
DMA가 중지되면 스트리밍 DMA를 먼저 정리하십시오. 즉. 데이터 버퍼의 매핑을 해제하고 버퍼가 있는 경우 "업스트림" 소유자에게 버퍼를 반환합니다.

Then clean up "consistent" buffers which contain the control data.
그런 다음 제어 데이터가 포함된 "일관된" 버퍼를 정리합니다.

See Documentation/DMA-API.txt for details on unmapping interfaces.
인터페이스 매핑 해제에 대한 자세한 내용은 Documentation/DMA-API.txt를 참조하세요.


Unregister from other subsystems
-------------------------------- 
Most low level PCI device drivers support some other subsystem like USB, ALSA, SCSI, NetDev, Infiniband, etc. Make sure your driver isn't losing resources from that other subsystem.  If this happens, typically the symptom is an Oops (panic) when the subsystem attempts to call into a driver that has been unloaded.  
대부분의 저수준 PCI 장치 드라이버는 USB, ALSA, SCSI, NetDev, Infiniband 등과 같은 다른 하위 시스템을 지원합니다. 드라이버가 다른 하위 시스템에서 리소스를 잃지 않는지 확인하십시오. 이런 일이 발생하면 일반적으로 하위 시스템이 언로드된 드라이버를 호출하려고 시도할 때 증상은 죄송합니다(패닉).

Disable Device from responding to MMIO/IO Port addresses
--------------------------------------------------------
io_unmap() MMIO or IO Port resources and then call pci_disable_device().  This is the symmetric opposite of pci_enable_device().  Do not access device registers after calling pci_disable_device().
io_unmap() MMIO 또는 IO 포트 리소스를 선택한 다음 pci_disable_device()를 호출합니다. 이것은 pci_enable_device()의 대칭 반대입니다. pci_disable_device()를 호출한 후 장치 레지스터에 액세스하지 마십시오.


Release MMIO/IO Port Resource(s)
--------------------------------
Call pci_release_region() to mark the MMIO or IO Port range as available.  Failure to do so usually results in the inability to reload the driver.
pci_release_region()을 호출하여 MMIO 또는 IO 포트 범위를 사용 가능한 것으로 표시합니다. 그렇게 하지 않으면 일반적으로 드라이버를 다시 로드할 수 없습니다.

How to access PCI config space
==============================

You can use `pci_(read|write)_config_(byte|word|dword)` to access the config space of a device represented by `struct pci_dev *`. All these functions return 0 when successful or an error code (`PCIBIOS_...`) which can be translated to a text string by pcibios_strerror. Most drivers expect that accesses to valid PCI devices don't fail.
`pci_(read|write)_config_(byte|word|dword)`를 사용하여 `struct pci_dev *`로 표시되는 장치의 구성 공간에 액세스할 수 있습니다. 이 모든 함수는 성공하면 0을 반환하거나 pcibios_strerror에 의해 텍스트 문자열로 번역될 수 있는 오류 코드(`PCIBIOS_...`)를 반환합니다. 대부분의 드라이버는 유효한 PCI 장치에 대한 액세스가 실패하지 않을 것으로 예상합니다.

If you don't have a struct pci_dev available, you can call `pci_bus_(read|write)_config_(byte|word|dword)` to access a given device and function on that bus.  
사용 가능한 pci_dev 구조체가 없으면 `pci_bus_(read|write)_config_(byte|word|dword)`를 호출하여 해당 버스의 지정된 장치 및 기능에 액세스할 수 있습니다.

If you access fields in the standard portion of the config header, please use symbolic names of locations and bits declared in <linux/pci.h>.
config 헤더의 표준 부분에 있는 필드에 액세스하는 경우 <linux/pci.h>에 선언된 위치 및 비트의 기호 이름을 사용하십시오.

If you need to access Extended PCI Capability registers, just call pci_find_capability() for the particular capability and it will find the
corresponding register block for you.  
확장 PCI 기능 레지스터에 액세스해야 하는 경우 특정 기능에 대해 pci_find_capability()를 호출하면 됩니다.
해당 레지스터 블록.

Other interesting functions
===========================

=============================	================================================
pci_get_domain_bus_and_slot()	Find pci_dev corresponding to given domain, bus and slot and number. If the device is found, its reference count is increased.
pci_set_power_state()		Set PCI Power Management state (0=D0 ... 3=D3)
pci_find_capability()		Find specified capability in device's capability list.
pci_resource_start()		Returns bus start address for a given PCI region
pci_resource_end()		Returns bus end address for a given PCI region
pci_resource_len()		Returns the byte length of a PCI region
pci_set_drvdata()		Set private driver data pointer for a pci_dev
pci_get_drvdata()		Return private driver data pointer for a pci_dev
pci_set_mwi()			Enable Memory-Write-Invalidate transactions.
pci_clear_mwi()			Disable Memory-Write-Invalidate transactions.
=============================	================================================
pci_get_domain_bus_and_slot() 주어진 도메인, 버스, 슬롯 및 번호에 해당하는 pci_dev를 찾습니다. 장치가 발견되면 참조 카운트가 증가합니다.
pci_set_power_state() PCI 전원 관리 상태 설정(0=D0 ... 3=D3)
pci_find_capability() 장치의 기능 목록에서 지정된 기능을 찾습니다.
pci_resource_start() 주어진 PCI 영역에 대한 버스 시작 주소를 반환합니다.
pci_resource_end() 주어진 PCI 영역에 대한 버스 끝 주소를 반환합니다.
pci_resource_len() PCI 영역의 바이트 길이를 반환합니다.
pci_set_drvdata() pci_dev에 대한 개인 드라이버 데이터 포인터 설정
pci_get_drvdata() pci_dev에 대한 개인 드라이버 데이터 포인터를 반환합니다.
pci_set_mwi() 메모리 쓰기 무효화 트랜잭션을 활성화합니다.
pci_clear_mwi() 메모리 쓰기 무효화 트랜잭션을 비활성화합니다.
 

Miscellaneous hints
===================

When displaying PCI device names to the user (for example when a driver wants to tell the user what card has it found), please use pci_name(pci_dev).
사용자에게 PCI 장치 이름을 표시할 때(예: 드라이버가 사용자에게 어떤 카드를 찾았는지 알려주고 싶을 때) pci_name(pci_dev)을 사용하십시오.

Always refer to the PCI devices by a pointer to the pci_dev structure.  All PCI layer functions use this identification and it's the only
reasonable one. Don't use bus/slot/function numbers except for very special purposes -- on systems with multiple primary buses their semantics
can be pretty complex.
항상 pci_dev 구조에 대한 포인터로 PCI 장치를 참조하십시오. 모든 PCI 계층 기능은 이 식별을 사용하며 유일한
합리적인 것. 매우 특별한 목적을 제외하고는 버스/슬롯/기능 번호를 사용하지 마십시오. 여러 기본 버스가 있는 시스템에서 의미 체계
꽤 복잡할 수 있습니다.

Don't try to turn on Fast Back to Back writes in your driver.  All devices on the bus need to be capable of doing it, so this is something which needs
to be handled by platform and generic code, not individual drivers.  
드라이버에서 빠른 백투백 쓰기를 켜려고 하지 마십시오. 버스의 모든 장치는 이를 수행할 수 있어야 하므로 이것이 필요한 것입니다.
개별 드라이버가 아닌 플랫폼 및 일반 코드에 의해 처리됩니다.

Vendor and device identifications
=================================

Do not add new device or vendor IDs to include/linux/pci_ids.h unless they are shared across multiple drivers.  You can add private definitions in your driver if they're helpful, or just use plain hex constants.
여러 드라이버에서 공유되지 않는 한 새 장치 또는 공급업체 ID를 include/linux/pci_ids.h에 추가하지 마십시오. 도움이 되는 경우 드라이버에 개인 정의를 추가하거나 일반 16진수 상수를 사용할 수 있습니다.

The device IDs are arbitrary hex numbers (vendor controlled) and normally used only in a single location, the pci_device_id table.
장치 ID는 임의의 16진수(공급업체 제어)이며 일반적으로 단일 위치인 pci_device_id 테이블에서만 사용됩니다.

Please DO submit new vendor/device IDs to http://pci-ids.ucw.cz/.
There are mirrors of the pci.ids file at http://pciids.sourceforge.net/ and https://github.com/pciutils/pciids.
http://pci-ids.ucw.cz/에 새 공급업체/장치 ID를 제출하십시오.
http://pciids.sourceforge.net/ 및 https://github.com/pciutils/pciids에 pci.ids 파일의 미러가 있습니다.


Obsolete functions
==================

There are several functions which you might come across when trying to port an old driver to the new PCI interface.  They are no longer present in the kernel as they aren't compatible with hotplug or PCI domains or having sane locking.
이전 드라이버를 새 PCI 인터페이스로 이식하려고 할 때 접할 수 있는 몇 가지 기능이 있습니다. 핫플러그 또는 PCI 도메인과 호환되지 않거나 정상적인 잠금이 있기 때문에 커널에 더 이상 존재하지 않습니다.

=================	===========================================
pci_find_device()	Superseded by pci_get_device()
pci_find_subsys()	Superseded by pci_get_subsys()
pci_find_slot()		Superseded by pci_get_domain_bus_and_slot()
pci_get_slot()		Superseded by pci_get_domain_bus_and_slot()
=================	===========================================
pci_find_device() pci_get_device()로 대체됨
pci_find_subsys() pci_get_subsys()로 대체됨
pci_find_slot() pci_get_domain_bus_and_slot()으로 대체됨
pci_get_slot() pci_get_domain_bus_and_slot()으로 대체됨

The alternative is the traditional PCI device driver that walks PCI device lists. This is still possible but discouraged.
대안은 PCI 장치 목록을 탐색하는 기존 PCI 장치 드라이버입니다. 이것은 여전히 ​​​​가능하지만 권장되지 않습니다.

MMIO Space and "Write Posting"
==============================

Converting a driver from using I/O Port space to using MMIO space often requires some additional changes. Specifically, "write posting" needs to be handled. Many drivers (e.g. tg3, acenic, sym53c8xx_2) already do this. I/O Port space guarantees write transactions reach the PCI device before the CPU can continue. Writes to MMIO space allow the CPU to continue before the transaction reaches the PCI device. HW weenies call this "Write Posting" because the write completion is "posted" to the CPU before the transaction has reached its destination.
드라이버를 I/O 포트 공간 사용에서 MMIO 공간 사용으로 변환하려면 종종 몇 가지 추가 변경 사항이 필요합니다. 특히 "글쓰기"를 처리해야 합니다. 많은 드라이버(예: tg3, acenic, sym53c8xx_2)가 이미 이 작업을 수행하고 있습니다. I/O 포트 공간은 쓰기 트랜잭션이 CPU를 계속하기 전에 PCI 장치에 도달하도록 보장합니다. MMIO 공간에 대한 쓰기는 트랜잭션이 PCI 장치에 도달하기 전에 CPU가 계속할 수 있도록 합니다. 트랜잭션이 목적지에 도달하기 전에 쓰기 완료가 CPU에 "게시"되기 때문에 하드웨어 위니는 이것을 "쓰기 게시"라고 부릅니다.

Thus, timing sensitive code should add readl() where the CPU is expected to wait before doing other work.  The classic "bit banging" sequence works fine for I/O Port space::
따라서 타이밍에 민감한 코드는 CPU가 다른 작업을 수행하기 전에 대기할 것으로 예상되는 readl()을 추가해야 합니다. 고전적인 "비트 뱅잉" 시퀀스는 I/O 포트 공간에서 잘 작동합니다.

       for (i = 8; --i; val >>= 1) {
               outb(val & 1, ioport_reg);      /* write bit */
               udelay(10);
       }

The same sequence for MMIO space should be::

       for (i = 8; --i; val >>= 1) {
               writeb(val & 1, mmio_reg);      /* write bit */
               readb(safe_mmio_reg);           /* flush posted write */
               udelay(10);
       }

It is important that "safe_mmio_reg" not have any side effects that interferes with the correct operation of the device.
"safe_mmio_reg"에 장치의 올바른 작동을 방해하는 부작용이 없는 것이 중요합니다.

Another case to watch out for is when resetting a PCI device. Use PCI Configuration space reads to flush the writel(). This will gracefully handle the PCI master abort on all platforms if the PCI device is expected to not respond to a readl().  Most x86 platforms will allow MMIO reads to master abort (a.k.a. "Soft Fail") and return garbage (e.g. ~0). But many RISC platforms will crash (a.k.a."Hard Fail").
주의해야 할 또 다른 경우는 PCI 장치를 재설정할 때입니다. PCI 구성 공간 읽기를 사용하여 writel()을 플러시하십시오. 이것은 PCI 장치가 readl()에 응답하지 않을 것으로 예상되는 경우 모든 플랫폼에서 PCI 마스터 중단을 정상적으로 처리합니다. 대부분의 x86 플랫폼은 MMIO 읽기가 마스터 중단(일명 "Soft Fail") 및 가비지 반환(예: ~0)을 허용합니다. 그러나 많은 RISC 플랫폼이 충돌합니다(일명 "하드 페일").
