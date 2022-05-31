.. SPDX-License-Identifier: GPL-2.0

========================================
ACPI considerations for PCI host bridges
========================================

The general rule is that the ACPI namespace should describe everything the OS might use unless there's another way for the OS to find it [1, 2].
일반적인 규칙은 OS에서 찾을 수 있는 다른 방법이 없는 한 ACPI 네임스페이스는 OS가 사용할 수 있는 모든 것을 설명해야 한다는 것입니다[1, 2].

For example, there's no standard hardware mechanism for enumerating PCI host bridges, so the ACPI namespace must describe each host bridge, the
method for accessing PCI config space below it, the address space windows the host bridge forwards to PCI (using _CRS), and the routing of legacy
INTx interrupts (using _PRT).
예를 들어 PCI 호스트 브리지를 열거하기 위한 표준 하드웨어 메커니즘이 없으므로 ACPI 네임스페이스는 각 호스트 브리지를 설명해야 합니다.
그 아래의 PCI 구성 공간에 액세스하는 방법, 호스트 브리지가 PCI로 전달하는 주소 공간 창(_CRS 사용), 레거시 라우팅
INTx 인터럽트(_PRT 사용).

PCI devices, which are below the host bridge, generally do not need to be described via ACPI.  The OS can discover them via the standard PCI enumeration mechanism, using config accesses to discover and identify devices and read and size their BARs.  However, ACPI may describe PCI devices if it provides power management or hotplug functionality for them or if the device has INTx interrupts connected by platform interrupt controllers and a _PRT is needed to describe those connections.
호스트 브리지 아래에 있는 PCI 장치는 일반적으로 ACPI를 통해 설명할 필요가 없습니다. OS는 표준 PCI 열거 메커니즘을 통해 장치를 검색하고 구성 액세스를 사용하여 장치를 검색 및 식별하고 BAR을 읽고 크기를 조정할 수 있습니다. 그러나 ACPI는 전원 관리 또는 핫플러그 기능을 제공하거나 장치에 플랫폼 인터럽트 컨트롤러에 의해 연결된 INTx 인터럽트가 있고 이러한 연결을 설명하기 위해 _PRT가 필요한 경우 PCI 장치를 설명할 수 있습니다.

ACPI resource description is done via _CRS objects of devices in the ACPI namespace [2].   The _CRS is like a generalized PCI BAR: the OS can read _CRS and figure out what resource is being consumed even if it doesn't have a driver for the device [3].  That's important because it means an old OS can work correctly even on a system with new devices unknown to the OS.  The new devices might not do anything, but the OS can at least make sure no resources conflict with them.
ACPI 리소스 설명은 ACPI 네임스페이스에 있는 장치의 _CRS 개체를 통해 수행됩니다[2]. _CRS는 일반화된 PCI BAR과 같습니다. OS는 _CRS를 읽고 장치에 대한 드라이버가 없더라도 어떤 리소스가 소비되고 있는지 파악할 수 있습니다[3]. 이는 OS에 알려지지 않은 새 장치가 있는 시스템에서도 이전 OS가 올바르게 작동할 수 있음을 의미하기 때문에 중요합니다. 새 장치는 아무 것도 하지 않을 수 있지만 OS는 최소한 리소스가 장치와 충돌하지 않도록 할 수 있습니다.

Static tables like MCFG, HPET, ECDT, etc., are *not* mechanisms for reserving address space.  The static tables are for things the OS needs to know early in boot, before it can parse the ACPI namespace.  If a new table is defined, an old OS needs to operate correctly even though it ignores the table.  _CRS allows that because it is generic and understood by the old OS; a static table does not.
MCFG, HPET, ECDT 등과 같은 정적 테이블은 주소 공간을 예약하는 메커니즘이 *아닙니다*. 정적 테이블은 OS가 ACPI 네임스페이스를 구문 분석하기 전에 부팅 초기에 알아야 하는 항목을 위한 것입니다. 새 테이블이 정의되면 기존 OS는 테이블을 무시하더라도 올바르게 작동해야 합니다. _CRS는 이전 OS에서 일반적이고 이해하기 때문에 허용합니다. 정적 테이블은 그렇지 않습니다.

If the OS is expected to manage a non-discoverable device described via ACPI, that device will have a specific _HID/_CID that tells the OS what driver to bind to it, and the _CRS tells the OS and the driver where the device's registers are.
OS가 ACPI를 통해 설명된 검색 불가능한 장치를 관리할 것으로 예상되는 경우 해당 장치에는 OS에 바인딩할 드라이버를 알려주는 특정 _HID/_CID가 있고 _CRS는 OS와 드라이버에 장치 레지스터가 있는 위치를 알려줍니다. .

PCI host bridges are PNP0A03 or PNP0A08 devices.  Their _CRS should describe all the address space they consume.  This includes all the windows they forward down to the PCI bus, as well as registers of the host bridge itself that are not forwarded to PCI.  The host bridge registers include things like secondary/subordinate bus registers that determine the bus range below the bridge, window registers that describe the apertures, etc.  These are all device-specific, non-architected things, so the only way a PNP0A03/PNP0A08 driver can manage them is via _PRS/_CRS/_SRS, which contain the device-specific details.  The host bridge registers also include ECAM space, since it is consumed by the host bridge.
PCI 호스트 브리지는 PNP0A03 또는 PNP0A08 장치입니다. 그들의 _CRS는 그들이 소비하는 모든 주소 공간을 설명해야 합니다. 여기에는 PCI 버스로 전달하는 모든 창과 PCI로 전달되지 않은 호스트 브리지 자체의 레지스터가 포함됩니다. 호스트 브리지 레지스터에는 브리지 아래의 버스 범위를 결정하는 2차/하위 버스 레지스터, 조리개를 설명하는 창 레지스터 등이 포함됩니다. 이들은 모두 장치 고유의 아키텍처가 아닌 항목이므로 PNP0A03/PNP0A08 드라이버는 장치별 세부 정보가 포함된 _PRS/_CRS/_SRS를 통해 이를 관리할 수 있습니다. 호스트 브리지 레지스터에는 호스트 브리지가 사용하기 때문에 ECAM 공간도 포함됩니다.

ACPI defines a Consumer/Producer bit to distinguish the bridge registers ("Consumer") from the bridge apertures ("Producer") [4, 5], but early BIOSes didn't use that bit correctly.  The result is that the current ACPI spec defines Consumer/Producer only for the Extended Address Space descriptors; the bit should be ignored in the older QWord/DWord/Word Address Space descriptors.  Consequently, OSes have to assume all QWord/DWord/Word descriptors are windows.
ACPI는 브리지 레지스터("소비자")를 브리지 조리개("생산자")[4, 5]와 구별하기 위해 소비자/생산자 비트를 정의하지만 초기 BIOS는 해당 비트를 올바르게 사용하지 않았습니다. 결과는 현재 ACPI 사양이 확장 주소 공간 설명자에 대해서만 소비자/생산자를 정의한다는 것입니다. 비트는 이전 QWord/DWord/Word 주소 공간 설명자에서 무시되어야 합니다. 결과적으로 OS는 모든 QWord/DWord/Word 설명자가 창이라고 가정해야 합니다.

Prior to the addition of Extended Address Space descriptors, the failure of Consumer/Producer meant there was no way to describe bridge registers in the PNP0A03/PNP0A08 device itself.  The workaround was to describe the bridge registers (including ECAM space) in PNP0C02 catch-all devices [6].  With the exception of ECAM, the bridge register space is device-specific anyway, so the generic PNP0A03/PNP0A08 driver (pci_root.c) has no need to know about it.  
확장 주소 공간 설명자를 추가하기 전에는 소비자/생산자 오류로 인해 PNP0A03/PNP0A08 장치 자체에서 브리지 레지스터를 설명할 방법이 없었습니다. 해결 방법은 PNP0C02 catch-all 장치[6]에서 브리지 레지스터(ECAM 공간 포함)를 설명하는 것이었습니다. ECAM을 제외하고 브리지 레지스터 공간은 어쨌든 장치에 따라 다르므로 일반 PNP0A03/PNP0A08 드라이버(pci_root.c)는 이에 대해 알 필요가 없습니다.

New architectures should be able to use "Consumer" Extended Address Space descriptors in the PNP0A03 device for bridge registers, including ECAM, although a strict interpretation of [6] might prohibit this.  Old x86 and ia64 kernels assume all address space descriptors, including "Consumer" Extended Address Space ones, are windows, so it would not be safe to describe bridge registers this way on those architectures.
새로운 아키텍처는 ECAM을 포함한 브리지 레지스터용 PNP0A03 장치에서 "소비자" 확장 주소 공간 설명자를 사용할 수 있어야 하지만 [6]의 엄격한 해석이 이를 금지할 수도 있습니다. 이전 x86 및 ia64 커널은 "소비자" 확장 주소 공간 디스크립터를 포함한 모든 주소 공간 디스크립터가 창이라고 가정하므로 이러한 아키텍처에서 브리지 레지스터를 이런 식으로 설명하는 것은 안전하지 않습니다.

PNP0C02 "motherboard" devices are basically a catch-all.  There's no programming model for them other than "don't use these resources for anything else."  So a PNP0C02 _CRS should claim any address space that is (1) not claimed by _CRS under any other device object in the ACPI namespace and (2) should not be assigned by the OS to something else.
PNP0C02 "마더보드" 장치는 기본적으로 포괄적입니다. "이 리소스를 다른 용도로 사용하지 마십시오" 외에는 프로그래밍 모델이 없습니다. 따라서 PNP0C02 _CRS는 (1) ACPI 네임스페이스의 다른 장치 개체에서 _CRS에 의해 요구되지 않고 (2) OS에 의해 다른 것에 할당되어서는 안 되는 주소 공간을 요구해야 합니다.

The PCIe spec requires the Enhanced Configuration Access Method (ECAM) unless there's a standard firmware interface for config access, e.g., the ia64 SAL interface [7].  A host bridge consumes ECAM memory address space and converts memory accesses into PCI configuration accesses.  The spec defines the ECAM address space layout and functionality; only the base of the address space is device-specific.  An ACPI OS learns the base address from either the static MCFG table or a _CBA method in the PNP0A03 device.
구성 액세스를 위한 표준 펌웨어 인터페이스(예: ia64 SAL 인터페이스[7])가 없는 한 PCIe 사양에는 ECAM(Enhanced Configuration Access Method)이 필요합니다. 호스트 브리지는 ECAM 메모리 주소 공간을 사용하고 메모리 액세스를 PCI 구성 액세스로 변환합니다. 사양은 ECAM 주소 공간 레이아웃 및 기능을 정의합니다. 주소 공간의 기초만 장치에 따라 다릅니다. ACPI OS는 PNP0A03 장치의 정적 MCFG 테이블 또는 _CBA 메서드에서 기본 주소를 학습합니다.

The MCFG table must describe the ECAM space of non-hot pluggable host bridges [8].  Since MCFG is a static table and can't be updated by hotplug, a _CBA method in the PNP0A03 device describes the ECAM space of a hot-pluggable host bridge [9].  Note that for both MCFG and _CBA, the base address always corresponds to bus 0, even if the bus range below the bridge (which is reported via _CRS) doesn't start at 0.
MCFG 테이블은 핫플러그가 불가능한 호스트 브리지의 ECAM 공간을 설명해야 합니다[8]. MCFG는 정적 테이블이고 핫플러그로 업데이트할 수 없기 때문에 PNP0A03 장치의 _CBA 메서드는 핫플러그 가능한 호스트 브리지의 ECAM 공간을 설명합니다[9]. MCFG 및 _CBA 모두에 대해 브리지 아래의 버스 범위(_CRS를 통해 보고됨)가 0에서 시작하지 않더라도 기본 주소는 항상 버스 0에 해당합니다.

[1] ACPI 6.2, sec 6.1:
    For any device that is on a non-enumerable type of bus (for example, an ISA bus), OSPM enumerates the devices' identifier(s) and the ACPI system firmware must supply an _HID object ... for each device to enable OSPM to do that.
    열거할 수 없는 유형의 버스(예: ISA 버스)에 있는 모든 장치의 경우 OSPM은 장치의 식별자를 열거하고 ACPI 시스템 펌웨어는 OSPM을 활성화하기 위해 각 장치에 대해 _HID 개체 ...를 제공해야 합니다. 하기 위해서.

[2] ACPI 6.2, sec 3.7:
    The OS enumerates motherboard devices simply by reading through the ACPI Namespace looking for devices with hardware IDs.
    OS는 ACPI 네임스페이스를 통해 하드웨어 ID가 있는 장치를 찾는 것으로 간단히 마더보드 장치를 열거합니다.

    Each device enumerated by ACPI includes ACPI-defined objects in the ACPI Namespace that report the hardware resources the device could occupy [_PRS], an object that reports the resources that are currently used by the device [_CRS], and objects for configuring those resources [_SRS].  The information is used by the Plug and Play OS (OSPM) to configure the devices.
    ACPI에 의해 열거된 각 장치에는 장치가 점유할 수 있는 하드웨어 리소스를 보고하는 ACPI 네임스페이스의 ACPI 정의 개체[_PRS], 현재 장치가 사용하는 리소스를 보고하는 개체[_CRS] 및 이러한 리소스를 구성하기 위한 개체가 포함됩니다. [_SRS]. 이 정보는 플러그 앤 플레이 OS(OSPM)에서 장치를 구성하는 데 사용됩니다.

[3] ACPI 6.2, sec 6.2:
    OSPM uses device configuration objects to configure hardware resources for devices enumerated via ACPI.  Device configuration objects provide information about current and possible resource requirements, the relationship between shared resources, and methods for configuring hardware resources.
    OSPM은 장치 구성 개체를 사용하여 ACPI를 통해 열거된 장치에 대한 하드웨어 리소스를 구성합니다. 장치 구성 개체는 현재 및 가능한 리소스 요구 사항, 공유 리소스 간의 관계 및 하드웨어 리소스를 구성하는 방법에 대한 정보를 제공합니다.

    When OSPM enumerates a device, it calls _PRS to determine the resource requirements of the device.  It may also call _CRS to find the current resource settings for the device.  Using this information, the Plug and Play system determines what resources the device should consume and sets those resources by calling the device’s _SRS control method.
    OSPM은 장치를 열거할 때 _PRS를 호출하여 장치의 리소스 요구 사항을 결정합니다. 또한 _CRS를 호출하여 장치의 현재 리소스 설정을 찾을 수도 있습니다. 이 정보를 사용하여 플러그 앤 플레이 시스템은 장치가 소비해야 하는 리소스를 결정하고 장치의 _SRS 제어 메서드를 호출하여 해당 리소스를 설정합니다.

    In ACPI, devices can consume resources (for example, legacy keyboards), provide resources (for example, a proprietary PCI bridge), or do both.  Unless otherwise specified, resources for a device are assumed to be taken from the nearest matching resource above the device in the device hierarchy.
ACPI에서 장치는 리소스(예: 레거시 키보드)를 사용하거나 리소스(예: 독점 PCI 브리지)를 제공하거나 둘 다를 수행할 수 있습니다. 달리 지정하지 않는 한, 장치에 대한 리소스는 장치 계층에서 장치 위에 있는 가장 근접하게 일치하는 리소스에서 가져온 것으로 가정합니다.

[4] ACPI 6.2, sec 6.4.3.5.1, 2, 3, 4:
    QWord/DWord/Word Address Space Descriptor (.1, .2, .3) General Flags: Bit [0] Ignored
    QWord/DWord/Word 주소 공간 설명자(.1, .2, .3) 일반 플래그: 비트 [0] 무시됨

    Extended Address Space Descriptor (.4)
    확장 주소 공간 설명자(.4)
      General Flags: Bit [0] Consumer/Producer:
      일반 플래그: 비트 [0] 소비자/생산자:

        * 1 – This device consumes this resource
        * 0 – This device produces and consumes this resource
        * 1 – 이 장치는 이 리소스를 소비합니다.
        * 0 – 이 장치는 이 리소스를 생성하고 소비합니다.

[5] ACPI 6.2, sec 19.6.43:
    ResourceUsage specifies whether the Memory range is consumed by this device (ResourceConsumer) or passed on to child devices (ResourceProducer).  If nothing is specified, then ResourceConsumer is assumed.
    ResourceUsage는 메모리 범위가 이 장치에서 소비되는지(ResourceConsumer) 아니면 자식 장치로 전달되는지(ResourceProducer) 지정합니다. 아무 것도 지정하지 않으면 ResourceConsumer가 가정됩니다.

[6] PCI Firmware 3.2, sec 4.1.2:
    If the operating system does not natively comprehend reserving the MMCFG region, the MMCFG region must be reserved by firmware.  The address range reported in the MCFG table or by _CBA method (see Section 4.1.3) must be reserved by declaring a motherboard resource.  For most systems, the motherboard resource would appear at the root of the ACPI namespace (under \_SB) in a node with a _HID of EISAID (PNP0C02), and the resources in this case should not be claimed in the root PCI bus’s _CRS.  The resources can optionally be returned in Int15 E820 or EFIGetMemoryMap as reserved memory but must always be reported through ACPI as a motherboard resource.
    운영 체제가 기본적으로 MMCFG 영역 예약을 이해하지 못하는 경우 MMCFG 영역은 펌웨어에 의해 예약되어야 합니다. MCFG 테이블 또는 _CBA 메서드(섹션 4.1.3 참조)에 보고된 주소 범위는 마더보드 리소스를 선언하여 예약해야 합니다. 대부분의 시스템에서 마더보드 리소스는 _HID가 EISAID(PNP0C02)인 노드에서 ACPI 네임스페이스의 루트(\_SB 아래)에 나타나며 이 경우 리소스는 루트 PCI 버스의 _CRS에서 청구되어서는 안 됩니다. 리소스는 선택적으로 Int15 E820 또는 EFIGetMemoryMap에서 예약된 메모리로 반환될 수 있지만 항상 ACPI를 통해 마더보드 리소스로 보고되어야 합니다.

[7] PCI Express 4.0, sec 7.2.2:
    For systems that are PC-compatible, or that do not implement a processor-architecture-specific firmware interface standard that allows access to the Configuration Space, the ECAM is required as defined in this section.
    PC와 호환되거나 구성 공간에 대한 액세스를 허용하는 프로세서 아키텍처별 펌웨어 인터페이스 표준을 구현하지 않는 시스템의 경우 이 섹션에 정의된 대로 ECAM이 필요합니다.

[8] PCI Firmware 3.2, sec 4.1.2:
    The MCFG table is an ACPI table that is used to communicate the base addresses corresponding to the non-hot removable PCI Segment Groups range within a PCI Segment Group available to the operating system at boot. This is required for the PC-compatible systems.
    MCFG 테이블은 부팅 시 운영 체제에서 사용할 수 있는 PCI 세그먼트 그룹 내에서 핫 제거 불가능 PCI 세그먼트 그룹 범위에 해당하는 기본 주소를 통신하는 데 사용되는 ACPI 테이블입니다. 이것은 PC 호환 시스템에 필요합니다.

    The MCFG table is only used to communicate the base addresses corresponding to the PCI Segment Groups available to the system at boot.
    MCFG 테이블은 부팅 시 시스템에서 사용할 수 있는 PCI 세그먼트 그룹에 해당하는 기본 주소를 전달하는 데만 사용됩니다.

[9] PCI Firmware 3.2, sec 4.1.3:
    The _CBA (Memory mapped Configuration Base Address) control method is an optional ACPI object that returns the 64-bit memory mapped configuration base address for the hot plug capable host bridge. The base address returned by _CBA is processor-relative address. The _CBA control method evaluates to an Integer.
    _CBA(메모리 매핑된 구성 기본 주소) 제어 방법은 핫 플러그 가능 호스트 브리지에 대한 64비트 메모리 매핑된 구성 기본 주소를 반환하는 선택적 ACPI 개체입니다. _CBA에서 반환된 기본 주소는 프로세서 상대 주소입니다. _CBA 제어 메서드는 정수로 평가됩니다.

    This control method appears under a host bridge object. When the _CBA method appears under an active host bridge object, the operating system evaluates this structure to identify the memory mapped configuration base address corresponding to the PCI Segment Group for the bus number range specified in _CRS method. An ACPI name space object that contains the _CBA method must also contain a corresponding _SEG method.
이 제어 방법은 호스트 브리지 개체 아래에 나타납니다. _CBA 메서드가 활성 호스트 브리지 개체 아래에 나타나면 운영 체제는 이 구조를 평가하여 _CRS 메서드에 지정된 버스 번호 범위에 대한 PCI 세그먼트 그룹에 해당하는 메모리 매핑된 구성 기본 주소를 식별합니다. _CBA 메서드가 포함된 ACPI 이름 공간 개체에는 해당 _SEG 메서드도 포함되어야 합니다.
