.. SPDX-License-Identifier: GPL-2.0
.. include:: <isonum.txt>

===========================================================
The PCI Express Advanced Error Reporting Driver Guide HOWTO
===========================================================

:Authors: - T. Long Nguyen <tom.l.nguyen@intel.com>
          - Yanmin Zhang <yanmin.zhang@intel.com>

:Copyright: |copy| 2006 Intel Corporation

Overview
===========

About this guide
----------------

This guide describes the basics of the PCI Express Advanced Error Reporting (AER) driver and provides information on how to use it, as well as how to enable the drivers of endpoint devices to conform with PCI Express AER driver.
이 가이드는 PCI Express AER(Advanced Error Reporting) 드라이버의 기본 사항을 설명하고 사용 방법과 종단 장치의 드라이버가 PCI Express AER 드라이버를 준수하도록 하는 방법에 대한 정보를 제공합니다.

What is the PCI Express AER Driver?
-----------------------------------

PCI Express error signaling can occur on the PCI Express link itself or on behalf of transactions initiated on the link. PCI Express defines two error reporting paradigms: the baseline capability and the Advanced Error Reporting capability. The baseline capability is required of all PCI Express components providing a minimum defined set of error reporting requirements. Advanced Error Reporting capability is implemented with a PCI Express advanced error reporting extended capability structure providing more robust error reporting.
PCI Express 오류 신호는 PCI Express 링크 자체에서 또는 링크에서 시작된 트랜잭션을 대신하여 발생할 수 있습니다. PCI Express는 기준 기능과 고급 오류 보고 기능이라는 두 가지 오류 보고 패러다임을 정의합니다. 기본 기능은 최소 정의된 오류 보고 요구 사항 집합을 제공하는 모든 PCI Express 구성 요소에 필요합니다. 고급 오류 보고 기능은 PCI Express 고급 오류 보고 확장 기능 구조로 구현되어 보다 강력한 오류 보고를 제공합니다.

The PCI Express AER driver provides the infrastructure to support PCI Express Advanced Error Reporting capability. The PCI Express AER driver provides three basic functions:
PCI Express AER 드라이버는 PCI Express 고급 오류 보고 기능을 지원하는 인프라를 제공합니다. PCI Express AER 드라이버는 세 가지 기본 기능을 제공합니다.

  - Gathers the comprehensive error information if errors occurred.
  - Reports error to the users.
  - Performs error recovery actions.
  - 오류가 발생한 경우 종합적인 오류 정보를 수집합니다.
  - 사용자에게 오류를 보고합니다.
  - 오류 복구 작업을 수행합니다.

AER driver only attaches root ports which support PCI-Express AER capability.
AER 드라이버는 PCI-Express AER 기능을 지원하는 루트 포트만 연결합니다.

User Guide
==========

Include the PCI Express AER Root Driver into the Linux Kernel
-------------------------------------------------------------

The PCI Express AER Root driver is a Root Port service driver attached to the PCI Express Port Bus driver. If a user wants to use it, the driver has to be compiled. Option CONFIG_PCIEAER supports this capability. It depends on CONFIG_PCIEPORTBUS, so pls. set CONFIG_PCIEPORTBUS=y and CONFIG_PCIEAER = y.
PCI Express AER 루트 드라이버는 PCI Express 포트 버스 드라이버에 연결된 루트 포트 서비스 드라이버입니다. 사용자가 사용하려면 드라이버를 컴파일해야 합니다. CONFIG_PCIEAER 옵션은 이 기능을 지원합니다. CONFIG_PCIEPORTBUS에 따라 다르므로 pls. CONFIG_PCIEPORTBUS=y 및 CONFIG_PCIEAER = y로 설정합니다.

Load PCI Express AER Root Driver
--------------------------------

Some systems have AER support in firmware. Enabling Linux AER support at the same time the firmware handles AER may result in unpredictable behavior. Therefore, Linux does not handle AER events unless the firmware grants AER control to the OS via the ACPI _OSC method. See the PCI FW 3.0 Specification for details regarding _OSC usage.
일부 시스템은 펌웨어에서 AER을 지원합니다. 펌웨어가 AER을 처리함과 동시에 Linux AER 지원을 활성화하면 예측할 수 없는 동작이 발생할 수 있습니다. 따라서 펌웨어가 ACPI _OSC 메서드를 통해 OS에 AER 제어 권한을 부여하지 않는 한 Linux는 AER 이벤트를 처리하지 않습니다. _OSC 사용에 대한 자세한 내용은 PCI FW 3.0 사양을 참조하십시오.

AER error output
----------------

When a PCIe AER error is captured, an error message will be output to console. If it's a correctable error, it is output as a warning.  Otherwise, it is printed as an error. So users could choose different log level to filter out correctable error messages.
PCIe AER 오류가 캡처되면 오류 메시지가 콘솔에 출력됩니다. 수정 가능한 오류인 경우 경고로 출력됩니다. 그렇지 않으면 오류로 인쇄됩니다. 따라서 사용자는 수정 가능한 오류 메시지를 필터링하기 위해 다른 로그 수준을 선택할 수 있습니다.

Below shows an example::
다음은 예를 보여줍니다.

  0000:50:00.0: PCIe Bus Error: severity=Uncorrected (Fatal), type=Transaction Layer, id=0500(Requester ID)
  0000:50:00.0:   device [8086:0329] error status/mask=00100000/00000000
  0000:50:00.0:    [20] Unsupported Request    (First)
  0000:50:00.0:   TLP Header: 04000001 00200a03 05010000 00050100

In the example, 'Requester ID' means the ID of the device who sends the error message to root port. Pls. refer to pci express specs for other fields.
예시에서 'Requester ID'는 루트 포트로 에러 메시지를 보내는 장치의 ID를 의미합니다. 제발. 다른 분야에 대해서는 pci express 사양을 참조하십시오.

AER Statistics / Counters
-------------------------

When PCIe AER errors are captured, the counters / statistics are also exposed in the form of sysfs attributes which are documented at Documentation/ABI/testing/sysfs-bus-pci-devices-aer_stats
PCIe AER 오류가 캡처되면 카운터/통계도 Documentation/ABI/testing/sysfs-bus-pci-devices-aer_stats에 문서화된 sysfs 속성의 형태로 노출됩니다. 

Developer Guide
===============

To enable AER aware support requires a software driver to configure the AER capability structure within its device and to provide callbacks.
AER 인식 지원을 활성화하려면 장치 내에서 AER 기능 구조를 구성하고 콜백을 제공하는 소프트웨어 드라이버가 필요합니다.

To support AER better, developers need understand how AER does work firstly.
AER을 더 잘 지원하려면 개발자는 먼저 AER이 어떻게 작동하는지 이해해야 합니다.

PCI Express errors are classified into two types: correctable errors and uncorrectable errors. This classification is based on the impacts of those errors, which may result in degraded performance or function failure.
PCI Express 오류는 수정 가능한 오류와 수정 불가능한 오류의 두 가지 유형으로 분류됩니다. 이 분류는 이러한 오류의 영향을 기반으로 하며 이로 인해 성능이 저하되거나 기능 장애가 발생할 수 있습니다.

Correctable errors pose no impacts on the functionality of the interface. The PCI Express protocol can recover without any software intervention or any loss of data. These errors are detected and corrected by hardware. Unlike correctable errors, uncorrectable errors impact functionality of the interface. Uncorrectable errors can cause a particular transaction or a particular PCI Express link to be unreliable. Depending on those error conditions, uncorrectable errors are further classified into non-fatal errors and fatal errors.  Non-fatal errors cause the particular transaction to be unreliable, but the PCI Express link itself is fully functional. Fatal errors, on the other hand, cause the link to be unreliable.
수정 가능한 오류는 인터페이스의 기능에 영향을 주지 않습니다. PCI Express 프로토콜은 소프트웨어 개입이나 데이터 손실 없이 복구할 수 있습니다. 이러한 오류는 하드웨어에 의해 감지되고 수정됩니다. 수정 가능한 오류와 달리 수정 불가능한 오류는 인터페이스의 기능에 영향을 줍니다. 수정할 수 없는 오류는 특정 트랜잭션이나 특정 PCI Express 링크를 신뢰할 수 없게 만들 수 있습니다. 이러한 오류 조건에 따라 수정 불가능한 오류는 치명적이지 않은 오류와 치명적 오류로 더 분류됩니다. 치명적이지 않은 오류는 특정 트랜잭션을 신뢰할 수 없게 만들지만 PCI Express 링크 자체는 완벽하게 작동합니다. 반면 치명적인 오류는 링크를 신뢰할 수 없게 만듭니다.

When AER is enabled, a PCI Express device will automatically send an error message to the PCIe root port above it when the device captures an error. The Root Port, upon receiving an error reporting message, internally processes and logs the error message in its PCI Express capability structure. Error information being logged includes storing the error reporting agent's requestor ID into the Error Source Identification Registers and setting the error bits of the Root Error Status Register accordingly. If AER error reporting is enabled in Root Error Command Register, the Root Port generates an interrupt if an error is detected.
AER이 활성화되면 PCI Express 장치는 장치가 오류를 캡처할 때 위의 PCIe 루트 포트에 자동으로 오류 메시지를 보냅니다. 루트 포트는 오류 보고 메시지를 수신하면 내부적으로 PCI Express 기능 구조에 오류 메시지를 처리하고 기록합니다. 기록되는 오류 정보에는 오류 보고 에이전트의 요청자 ID를 오류 소스 식별 레지스터에 저장하고 그에 따라 루트 오류 상태 레지스터의 오류 비트를 설정하는 작업이 포함됩니다. 루트 오류 명령 레지스터에서 AER 오류 보고가 활성화된 경우 오류가 감지되면 루트 포트가 인터럽트를 생성합니다.

Note that the errors as described above are related to the PCI Express hierarchy and links. These errors do not include any device specific errors because device specific errors will still get sent directly to the device driver.
위에서 설명한 오류는 PCI Express 계층 구조 및 링크와 관련되어 있습니다. 장치 특정 오류는 여전히 장치 드라이버로 직접 전송되기 때문에 이러한 오류에는 장치 특정 오류가 포함되지 않습니다.

Configure the AER capability structure
--------------------------------------

AER aware drivers of PCI Express component need change the device control registers to enable AER. They also could change AER registers, including mask and severity registers. Helper function pci_enable_pcie_error_reporting could be used to enable AER. See
section 3.3.
PCI Express 구성 요소의 AER 인식 드라이버는 AER을 활성화하기 위해 장치 제어 레지스터를 변경해야 합니다. 또한 마스크 및 심각도 레지스터를 포함한 AER 레지스터를 변경할 수 있습니다. 도우미 기능 pci_enable_pcie_error_reporting을 사용하여 AER을 활성화할 수 있습니다. 보다
섹션 3.3.


Provide callbacks
-----------------

callback reset_link to reset pci express link
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This callback is used to reset the pci express physical link when a fatal error happens. The root port aer service driver provides a default reset_link function, but different upstream ports might have different specifications to reset pci express link, so all upstream ports should provide their own reset_link functions.
이 콜백은 치명적인 오류가 발생할 때 pci express 물리적 링크를 재설정하는 데 사용됩니다. 루트 포트 aer 서비스 드라이버는 기본 reset_link 기능을 제공하지만 다른 업스트림 포트는 pci 익스프레스 링크를 재설정하는 사양이 다를 수 있으므로 모든 업스트림 포트는 자체 reset_link 기능을 제공해야 합니다.

In struct pcie_port_service_driver, a new pointer, reset_link, is added.
::
pcie_port_service_driver 구조체에 새로운 포인터인 reset_link가 추가되었습니다.
::

	pci_ers_result_t (*reset_link) (struct pci_dev *dev);

Section 3.2.2.2 provides more detailed info on when to call reset_link.
3.2.2.2절은 언제 reset_link를 호출해야 하는지에 대한 자세한 정보를 제공합니다.

PCI error-recovery callbacks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The PCI Express AER Root driver uses error callbacks to coordinate with downstream device drivers associated with a hierarchy in question when performing error recovery actions.
PCI Express AER 루트 드라이버는 오류 복구 작업을 수행할 때 해당 계층과 관련된 다운스트림 장치 드라이버와 조정하기 위해 오류 콜백을 사용합니다.


Data struct pci_driver has a pointer, err_handler, to point to pci_error_handlers who consists of a couple of callback function pointers. AER driver follows the rules defined in pci-error-recovery.txt except pci express specific parts (e.g.  reset_link). Pls. refer to pci-error-recovery.txt for detailed definitions of the callbacks.
데이터 구조체 pci_driver에는 두 개의 콜백 함수 포인터로 구성된 pci_error_handlers를 가리키는 포인터 err_handler가 있습니다. AER 드라이버는 pci express 특정 부분(예: reset_link)을 제외하고 pci-error-recovery.txt에 정의된 규칙을 따릅니다. 제발. 콜백에 대한 자세한 정의는 pci-error-recovery.txt를 참조하십시오.

Below sections specify when to call the error callback functions.
아래 섹션에서는 오류 콜백 함수를 호출할 시기를 지정합니다.

Correctable errors
~~~~~~~~~~~~~~~~~~

Correctable errors pose no impacts on the functionality of the interface. The PCI Express protocol can recover without any software intervention or any loss of data. These errors do not require any recovery actions. The AER driver clears the device's correctable error status register accordingly and logs these errors.
수정 가능한 오류는 인터페이스의 기능에 영향을 주지 않습니다. PCI Express 프로토콜은 소프트웨어 개입이나 데이터 손실 없이 복구할 수 있습니다. 이러한 오류에는 복구 작업이 필요하지 않습니다. AER 드라이버는 그에 따라 장치의 수정 가능한 오류 상태 레지스터를 지우고 이러한 오류를 기록합니다.

Non-correctable (non-fatal and fatal) errors
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If an error message indicates a non-fatal error, performing link reset at upstream is not required. The AER driver calls error_detected(dev, pci_channel_io_normal) to all drivers associated within a hierarchy in question. for example::
오류 메시지가 치명적이지 않은 오류를 나타내는 경우 업스트림에서 링크 재설정을 수행할 필요가 없습니다. AER 드라이버는 해당 계층 내에 연결된 모든 드라이버에 대해 error_detected(dev, pci_channel_io_normal)를 호출합니다. 예를 들어::


  EndPoint
<==>DownstreamPort B<==>UpstreamPort A<==>RootPort

If Upstream port A captures an AER error, the hierarchy consists of Downstream port B and EndPoint.
업스트림 포트 A가 AER 오류를 캡처하는 경우 계층 구조는 다운스트림 포트 B와 EndPoint로 구성됩니다.

A driver may return PCI_ERS_RESULT_CAN_RECOVER, PCI_ERS_RESULT_DISCONNECT, or PCI_ERS_RESULT_NEED_RESET, depending on whether it can recover or the AER driver calls mmio_enabled as next.
드라이버는 복구할 수 있는지 또는 AER 드라이버가 다음으로 mmio_enabled를 호출하는지에 따라 PCI_ERS_RESULT_CAN_RECOVER, PCI_ERS_RESULT_DISCONNECT 또는 PCI_ERS_RESULT_NEED_RESET을 반환할 수 있습니다.

If an error message indicates a fatal error, kernel will broadcast error_detected(dev, pci_channel_io_frozen) to all drivers within a hierarchy in question. Then, performing link reset at upstream is necessary. As different kinds of devices might use different approaches to reset link, AER port service driver is required to provide the function to reset link. Firstly, kernel looks for if the upstream component has an aer driver. If it has, kernel uses the reset_link callback of the aer driver. If the upstream component has no aer driver and the port is downstream port, we will perform a hot reset as the default by setting the Secondary Bus Reset bit of the Bridge Control register associated with the downstream port. As for upstream ports, they should provide their own aer service drivers with reset_link function. If error_detected returns PCI_ERS_RESULT_CAN_RECOVER and reset_link returns PCI_ERS_RESULT_RECOVERED, the error handling goes to mmio_enabled.
오류 메시지가 치명적인 오류를 나타내는 경우 커널은 해당 계층 내의 모든 드라이버에 error_detected(dev, pci_channel_io_frozen)를 브로드캐스트합니다. 그런 다음 업스트림에서 링크 재설정을 수행해야 합니다. 장치의 종류에 따라 링크 재설정 방법이 다를 수 있으므로 링크 재설정 기능을 제공하려면 AER 포트 서비스 드라이버가 필요합니다. 먼저 커널은 업스트림 구성 요소에 aer 드라이버가 있는지 찾습니다. 있는 경우 커널은 aer 드라이버의 reset_link 콜백을 사용합니다. 업스트림 구성 요소에 aer 드라이버가 없고 포트가 다운스트림 포트인 경우 다운스트림 포트와 연결된 브리지 제어 레지스터의 2차 버스 재설정 비트를 설정하여 핫 재설정을 기본값으로 수행합니다. 업스트림 포트의 경우 reset_link 기능이 있는 자체 항공 서비스 드라이버를 제공해야 합니다. error_detected가 PCI_ERS_RESULT_CAN_RECOVER를 반환하고 reset_link가 PCI_ERS_RESULT_RECOVERED를 반환하면 오류 처리가 mmio_enabled로 이동합니다.

helper functions
----------------
::

  int pci_enable_pcie_error_reporting(struct pci_dev *dev);

pci_enable_pcie_error_reporting enables the device to send error messages to root port when an error is detected. Note that devices don't enable the error reporting by default, so device drivers need call this function to enable it.  
pci_enable_pcie_error_reporting은 오류가 감지될 때 장치가 루트 포트에 오류 메시지를 보낼 수 있도록 합니다. 장치는 기본적으로 오류 보고를 활성화하지 않으므로 장치 드라이버는 이 기능을 호출하여 활성화해야 합니다.

::

  int pci_disable_pcie_error_reporting(struct pci_dev *dev);

pci_disable_pcie_error_reporting disables the device to send error messages to root port when an error is detected.
pci_disable_pcie_error_reporting은 오류가 감지될 때 루트 포트에 오류 메시지를 보내도록 장치를 비활성화합니다.


::

  int pci_cleanup_aer_uncorrect_error_status(struct pci_dev *dev);`

pci_cleanup_aer_uncorrect_error_status cleanups the uncorrectable error status register.
pci_cleanup_aer_uncorrect_error_status는 수정할 수 없는 오류 상태 레지스터를 정리합니다.


Frequent Asked Questions
------------------------

Q:
  What happens if a PCI Express device driver does not provide an error recovery handler (pci_driver->err_handler is equal to NULL)?

A:
  The devices attached with the driver won't be recovered. If the error is fatal, kernel will print out warning messages. Please refer to section 3 for more information.
큐:
  PCI Express 장치 드라이버가 오류 복구 핸들러를 제공하지 않으면 어떻게 됩니까(pci_driver->err_handler는 NULL임)?

ㅏ:
  드라이버에 연결된 장치는 복구되지 않습니다. 오류가 치명적이면 커널은 경고 메시지를 출력합니다. 자세한 내용은 섹션 3을 참조하십시오.

Q:
  What happens if an upstream port service driver does not provide callback reset_link?

A:
  Fatal error recovery will fail if the errors are reported by the upstream ports who are attached by the service driver.

큐:
  업스트림 포트 서비스 드라이버가 콜백 reset_link를 제공하지 않으면 어떻게 됩니까?

ㅏ:
  서비스 드라이버가 연결한 업스트림 포트에서 오류가 보고되면 치명적인 오류 복구가 실패합니다.

Q:
  How does this infrastructure deal with driver that is not PCI Express aware?

A:
  This infrastructure calls the error callback functions of the driver when an error happens. But if the driver is not aware of PCI Express, the device might not report its own errors to root port.

큐:
  이 인프라는 PCI Express를 인식하지 못하는 드라이버를 어떻게 처리합니까?

ㅏ:
  이 인프라는 오류가 발생했을 때 드라이버의 오류 콜백 기능을 호출합니다. 그러나 드라이버가 PCI Express를 인식하지 못하는 경우 장치는 자체 오류를 루트 포트에 보고하지 않을 수 있습니다.

Q:
  What modifications will that driver need to make it compatible with the PCI Express AER Root driver?

A:
  It could call the helper functions to enable AER in devices and cleanup uncorrectable status register. Pls. refer to section 3.3.

큐:
  해당 드라이버가 PCI Express AER 루트 드라이버와 호환되도록 하려면 어떤 수정이 필요합니까?

ㅏ:
  도우미 함수를 호출하여 장치에서 AER을 활성화하고 수정할 수 없는 상태 레지스터를 정리할 수 있습니다. 제발. 섹션 3.3을 참조하십시오.

Software error injection
========================

Debugging PCIe AER error recovery code is quite difficult because it is hard to trigger real hardware errors. Software based error injection can be used to fake various kinds of PCIe errors.

First you should enable PCIe AER software error injection in kernel configuration, that is, following item should be in your .config.

CONFIG_PCIEAER_INJECT=y or CONFIG_PCIEAER_INJECT=m

After reboot with new kernel or insert the module, a device file named /dev/aer_inject should be created.

Then, you need a user space tool named aer-inject, which can be gotten from:

    https://git.kernel.org/cgit/linux/kernel/git/gong.chen/aer-inject.git/

More information about aer-inject can be found in the document comes with its source code.

PCIe AER 오류 복구 코드를 디버깅하는 것은 실제 하드웨어 오류를 유발하기 어렵기 때문에 상당히 어렵습니다. 소프트웨어 기반 오류 주입을 사용하여 다양한 종류의 PCIe 오류를 가짜로 만들 수 있습니다.

먼저 커널 구성에서 PCIe AER 소프트웨어 오류 주입을 활성화해야 합니다. 즉, 다음 항목이 .config에 있어야 합니다.

CONFIG_PCIEAER_INJECT=y 또는 CONFIG_PCIEAER_INJECT=m

새 커널로 재부팅하거나 모듈을 삽입한 후 /dev/aer_inject라는 장치 파일이 생성되어야 합니다.

그런 다음 다음에서 얻을 수 있는 aer-inject라는 사용자 공간 도구가 필요합니다.

     https://git.kernel.org/cgit/linux/kernel/git/gong.chen/aer-inject.git/

aer-inject에 대한 자세한 내용은 소스 코드와 함께 제공되는 문서에서 찾을 수 있습니다.