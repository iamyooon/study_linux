.. SPDX-License-Identifier: GPL-2.0

==================
PCI Error Recovery
==================


:Authors: - Linas Vepstas <linasvepstas@gmail.com>
          - Richard Lary <rlary@us.ibm.com>
          - Mike Mason <mmlnx@us.ibm.com>


Many PCI bus controllers are able to detect a variety of hardware
PCI errors on the bus, such as parity errors on the data and address
buses, as well as SERR and PERR errors.  Some of the more advanced
chipsets are able to deal with these errors; these include PCI-E chipsets,
and the PCI-host bridges found on IBM Power4, Power5 and Power6-based
pSeries boxes. A typical action taken is to disconnect the affected device,
halting all I/O to it.  The goal of a disconnection is to avoid system
corruption; for example, to halt system memory corruption due to DMA's
to "wild" addresses. Typically, a reconnection mechanism is also
offered, so that the affected PCI device(s) are reset and put back
into working condition. The reset phase requires coordination
between the affected device drivers and the PCI controller chip.
This document describes a generic API for notifying device drivers
of a bus disconnection, and then performing error recovery.
This API is currently implemented in the 2.6.16 and later kernels.

많은 PCI 버스 컨트롤러는 데이터 및 주소 버스의 패리티 오류뿐만 아니라 SERR 및 PERR 오류와 같은 다양한 하드웨어 PCI 오류를 감지할 수 있습니다. PCI-E 칩셋과 IBM Power4, Power5, Power6 기반 pSeries 박스에서 볼 수 있는 PCI-호스트 브릿지를 포함하여 이러한 오류를 처리할 수 있는 고급 칩셋도 있습니다. 일반적으로 수행되는 작업은 영향을 받는 장치의 연결을 끊고 장치에 대한 모든 I/O를 중지하는 것입니다. 예를 들어 DMA로 인한 시스템 메모리 손상을 막는 것이 절단의 목적이다. 일반적으로 영향을 받는 PCI 디바이스를 재설정하고 다시 작동 상태로 되돌릴 수 있는 재연결 메커니즘도 제공됩니다. 재설정 단계를 수행하려면 영향을 받는 디바이스 드라이버와 PCI 컨트롤러 칩 간의 조정이 필요합니다. 이 문서에서는 장치 드라이버에 버스 연결 끊김을 알리고 오류 복구를 수행하기 위한 일반 API를 설명합니다. 이 API는 현재 2.6.16 이상의 커널에서 구현되어 있습니다.

Reporting and recovery is performed in several steps. First, when
a PCI hardware error has resulted in a bus disconnect, that event
is reported as soon as possible to all affected device drivers,
including multiple instances of a device driver on multi-function
cards. This allows device drivers to avoid deadlocking in spinloops,
waiting for some i/o-space register to change, when it never will.
It also gives the drivers a chance to defer incoming I/O as
needed.

보고 및 복구는 여러 단계로 수행됩니다. 첫째, PCI 하드웨어 오류로 인해 버스 연결이 끊기면 해당 이벤트는 다기능 카드의 여러 장치 드라이버 인스턴스를 포함하여 영향을 받는 모든 장치 드라이버에 가능한 한 빨리 보고됩니다. 이를 통해 장치 드라이버는 스핀 루프에서 데드락킹(deadlocking)을 방지하고, 일부 I/O 공간 레지스터가 변경되기를 기다리며, 변경되지 않을 수 있습니다. 또한 드라이버는 필요에 따라 들어오는 I/O를 연기할 수 있습니다.

Next, recovery is performed in several stages. Most of the complexity
is forced by the need to handle multi-function devices, that is,
devices that have multiple device drivers associated with them.
In the first stage, each driver is allowed to indicate what type
of reset it desires, the choices being a simple re-enabling of I/O
or requesting a slot reset.

다음으로, 복구는 여러 단계로 수행됩니다. 대부분의 복잡성은 다기능 장치, 즉 여러 장치 드라이버가 연결된 장치를 처리해야 하기 때문에 발생합니다. 첫 번째 단계에서 각 드라이버는 I/O를 다시 활성화하거나 슬롯 리셋을 요청하는 등 원하는 리셋 유형을 지정할 수 있습니다.

If any driver requests a slot reset, that is what will be done.

운전자가 슬롯 재설정을 요청하면 그렇게 됩니다.

After a reset and/or a re-enabling of I/O, all drivers are
again notified, so that they may then perform any device setup/config
that may be required.  After these have all completed, a final
"resume normal operations" event is sent out.

I/O를 재설정 및/또는 다시 활성화한 후에는 모든 드라이버에 다시 알림이 표시되므로 필요한 장치 설정/구성 작업을 수행할 수 있습니다. 모든 작업이 완료되면 최종 "정상 작업 재개" 이벤트가 전송됩니다.

The biggest reason for choosing a kernel-based implementation rather
than a user-space implementation was the need to deal with bus
disconnects of PCI devices attached to storage media, and, in particular,
disconnects from devices holding the root file system.  If the root
file system is disconnected, a user-space mechanism would have to go
through a large number of contortions to complete recovery. Almost all
of the current Linux file systems are not tolerant of disconnection
from/reconnection to their underlying block device. By contrast,
bus errors are easy to manage in the device driver. Indeed, most
device drivers already handle very similar recovery procedures;
for example, the SCSI-generic layer already provides significant
mechanisms for dealing with SCSI bus errors and SCSI bus resets.

사용자 공간 구현이 아닌 커널 기반 구현을 선택한 가장 큰 이유는 저장 매체에 연결된 PCI 장치의 버스 연결 끊기, 특히 루트 파일 시스템을 보유한 장치와의 연결을 끊는 문제를 해결해야 했기 때문이다. 루트 파일 시스템의 연결이 끊긴 경우 복구를 완료하기 위해 사용자 공간 메커니즘이 많은 왜곡을 거쳐야 합니다. 현재 대부분의 리눅스 파일 시스템은 기본 블록 장치와의 연결을 끊는 것을 허용하지 않는다. 대조적으로, 버스 오류는 장치 드라이버에서 관리하기 쉽다. 실제로 대부분의 장치 드라이버는 이미 매우 유사한 복구 절차를 처리합니다. 예를 들어 SCSI 일반 계층은 SCSI 버스 오류 및 SCSI 버스 재설정을 처리하기 위한 중요한 메커니즘을 이미 제공합니다.

Detailed Design
===============

Design and implementation details below, based on a chain of
public email discussions with Ben Herrenschmidt, circa 5 April 2005.

아래 설계 및 구현 세부 정보는 다음과 같습니다.
2005년 4월 5일경 벤 헤렌슈미트와의 공개 이메일 토론.

The error recovery API support is exposed to the driver in the form of
a structure of function pointers pointed to by a new field in struct
pci_driver. A driver that fails to provide the structure is "non-aware",
and the actual recovery steps taken are platform dependent.  The
arch/powerpc implementation will simulate a PCI hotplug remove/add.

오류 복구 API 지원은 새로운 필드 명령어 pci_driver가 가리키는 함수 포인터 구조의 형태로 드라이버에 노출된다. 구조를 제공하지 못하는 드라이버는 "인식되지 않음"이며 실제로 수행되는 복구 단계는 플랫폼에 따라 다릅니다. arch/powerpc 구현은 PCI 핫 플러그 제거/추가 시뮬레이션을 수행합니다.

This structure has the form::
이 구조는 다음과 같은 형태를 가진다

	struct pci_error_handlers
	{
		int (*error_detected)(struct pci_dev *dev, enum pci_channel_state);
		int (*mmio_enabled)(struct pci_dev *dev);
		int (*slot_reset)(struct pci_dev *dev);
		void (*resume)(struct pci_dev *dev);
	};

The possible channel states are::
가능한 채널 상태는 다음과 같습니다.

	enum pci_channel_state {
		pci_channel_io_normal,  /* I/O channel is in normal state */
		pci_channel_io_frozen,  /* I/O to channel is blocked */
		pci_channel_io_perm_failure, /* PCI card is dead */
	};

Possible return values are::
가능한 반환 값은 다음과 같습니다.

	enum pci_ers_result {
		PCI_ERS_RESULT_NONE,        /* no result/none/not supported in device driver */
		PCI_ERS_RESULT_CAN_RECOVER, /* Device driver can recover without slot reset */
		PCI_ERS_RESULT_NEED_RESET,  /* Device driver wants slot to be reset. */
		PCI_ERS_RESULT_DISCONNECT,  /* Device has completely failed, is unrecoverable */
		PCI_ERS_RESULT_RECOVERED,   /* Device driver is fully recovered and operational */
	};

A driver does not have to implement all of these callbacks; however,
if it implements any, it must implement error_detected(). If a callback
is not implemented, the corresponding feature is considered unsupported.
For example, if mmio_enabled() and resume() aren't there, then it
is assumed that the driver is not doing any direct recovery and requires
a slot reset.  Typically a driver will want to know about
a slot_reset().

드라이버는 이러한 콜백을 모두 구현할 필요는 없지만, 만약 콜백을 구현한다면 error_detected()를 구현해야 한다. 콜백이 구현되지 않으면 해당 기능은 지원되지 않는 것으로 간주됩니다. 예를 들어 mmio_enabled()와 resume()가 없으면 드라이버가 직접 복구를 수행하지 않고 슬롯을 재설정해야 하는 것으로 가정합니다. 일반적으로 드라이버는 slot_reset()에 대해 알고 싶어 합니다.

The actual steps taken by a platform to recover from a PCI error
event will be platform-dependent, but will follow the general
sequence described below.

PCI 오류 이벤트로부터 복구하기 위해 플랫폼에 의해 수행되는 실제 단계는 플랫폼에 따라 다르지만 아래에 설명된 일반적인 순서를 따릅니다.

STEP 0: Error Event
-------------------
A PCI bus error is detected by the PCI hardware.  On powerpc, the slot
is isolated, in that all I/O is blocked: all reads return 0xffffffff,
all writes are ignored.

PCI 버스 오류가 PCI 하드웨어에서 감지되었습니다. powerpc에서는 모든 I/O가 차단된다는 점에서 슬롯이 분리됩니다. 모든 읽기는 0xffffff를 반환하고 모든 쓰기는 무시됩니다.

STEP 1: Notification
--------------------
Platform calls the error_detected() callback on every instance of
every driver affected by the error.

플랫폼은 오류의 영향을 받는 모든 드라이버의 모든 인스턴스에서 error_detected() 콜백을 호출합니다.

At this point, the device might not be accessible anymore, depending on
the platform (the slot will be isolated on powerpc). The driver may
already have "noticed" the error because of a failing I/O, but this
is the proper "synchronization point", that is, it gives the driver
a chance to cleanup, waiting for pending stuff (timers, whatever, etc...)
to complete; it can take semaphores, schedule, etc... everything but
touch the device. Within this function and after it returns, the driver
shouldn't do any new IOs. Called in task context. This is sort of a
"quiesce" point. See note about interrupts at the end of this doc.

이때 플랫폼에 따라 장치에 더 이상 액세스하지 못할 수 있습니다(전원 PC에서 슬롯이 분리됩니다). 드라이버는 I/O 실패 때문에 이미 오류를 알아차렸을 수도 있지만, 이것은 적절한 "동기화 지점"이다. 즉, 드라이버에게 정리할 기회를 주고 대기 중인 것(타이머, 기타 등)이 완료되기를 기다린다. 장치를 만지는 것 외에는 모두. 이 기능 내에서 그리고 그것이 돌아온 후에, 드라이버는 어떤 새로운 IO도 하지 말아야 한다. 작업 컨텍스트에서 호출됩니다. 이것은 일종의 "정기" 지점이다. 이 문서의 끝에 있는 인터럽트에 대한 참고 사항을 참조하십시오.

All drivers participating in this system must implement this call.
The driver must return one of the following result codes:

이 시스템에 참여하는 모든 드라이버는 이 호출을 구현해야 합니다. 운전자는 다음 결과 코드 중 하나를 반환해야 합니다.

  - PCI_ERS_RESULT_CAN_RECOVER
      Driver returns this if it thinks it might be able to recover
      the HW by just banging IOs or if it wants to be given
      a chance to extract some diagnostic information (see
      mmio_enable, below).
드라이버는 IO를 세게 쳐서 HW를 복구할 수 있다고 생각하거나 일부 진단 정보를 추출할 수 있는 기회를 제공받기를 원하는 경우 이 값을 반환합니다(아래의 mmio_enable 참조).

  - PCI_ERS_RESULT_NEED_RESET
      Driver returns this if it can't recover without a
      slot reset.
슬롯을 재설정하지 않으면 복구할 수 없는 경우 드라이버가 이 값을 반환합니다.

  - PCI_ERS_RESULT_DISCONNECT
      Driver returns this if it doesn't want to recover at all.
드라이버는 복구를 원하지 않으면 이 값을 반환합니다.

The next step taken will depend on the result codes returned by the
drivers.
다음 단계는 운전자가 반환하는 결과 코드에 따라 달라집니다.


If all drivers on the segment/slot return PCI_ERS_RESULT_CAN_RECOVER,
then the platform should re-enable IOs on the slot (or do nothing in
particular, if the platform doesn't isolate slots), and recovery
proceeds to STEP 2 (MMIO Enable).
세그먼트/슬롯의 모든 드라이버가 PCI_ERS_RESULT_CAN_RECOVER를 반환하는 경우, 플랫폼은 슬롯에서 IO를 다시 활성화해야 하며(또는 플랫폼이 슬롯을 분리하지 않는 경우), 복구는 2단계(MMIO Enable)로 진행됩니다.

If any driver requested a slot reset (by returning PCI_ERS_RESULT_NEED_RESET),
then recovery proceeds to STEP 4 (Slot Reset).
PCI_ERS_RESULT_NEED_RESET을 반환하여 슬롯 재설정을 요청한 드라이버가 있으면 복구가 4단계(슬롯 재설정)로 진행됩니다.

If the platform is unable to recover the slot, the next step
is STEP 6 (Permanent Failure).
플랫폼이 슬롯을 복구할 수 없는 경우, 다음 단계는 6단계(영구적 오류)입니다.

.. note::

   The current powerpc implementation assumes that a device driver will
   *not* schedule or semaphore in this routine; the current powerpc
   implementation uses one kernel thread to notify all devices;
   thus, if one device sleeps/schedules, all devices are affected.
   Doing better requires complex multi-threaded logic in the error
   recovery implementation (e.g. waiting for all notification threads
   to "join" before proceeding with recovery.)  This seems excessively
   complex and not worth implementing.

현재의 powerpc 구현체는 장치 드라이버가 이 루틴에서 semaphore 또는 semaphore를 예약하지 않을 것으로 가정한다. 보다 효율적으로 하려면 오류 복구 구현에서 복잡한 멀티 스레드 논리가 필요합니다(예: 복구를 진행하기 전에 모든 알림 스레드가 "가입"되기를 기다리는 등). 이것은 지나치게 복잡하고 구현할 가치가 없어 보인다.

   The current powerpc implementation doesn't much care if the device
   attempts I/O at this point, or not.  I/O's will fail, returning
   a value of 0xff on read, and writes will be dropped. If more than
   EEH_MAX_FAILS I/O's are attempted to a frozen adapter, EEH
   assumes that the device driver has gone into an infinite loop
   and prints an error to syslog.  A reboot is then required to
   get the device working again.

현재 powerpc 구현체에서는 장치가 이 시점에서 I/O를 시도하든 하지 않든 크게 신경 쓰지 않는다. I/O가 실패하고 읽을 때 0xff의 값이 반환되고 쓰기가 삭제됩니다. 고정된 어댑터에 대해 EEH_MAX_FAILs I/O를 여러 번 시도하면 EEH는 디바이스 드라이버가 무한 루프 상태에 들어갔다고 가정하고 syslog에 오류를 인쇄합니다. 그런 다음 장치를 다시 작동시키려면 재부팅해야 합니다.

STEP 2: MMIO Enabled
--------------------
The platform re-enables MMIO to the device (but typically not the
DMA), and then calls the mmio_enabled() callback on all affected
device drivers.
이 플랫폼은 장치에 대한 MMIO를 다시 사용하도록 설정한 다음(일반적으로 DMA가 아님), 영향을 받는 모든 장치 드라이버에 대해 mmio_enabled() 콜백을 호출합니다.

This is the "early recovery" call. IOs are allowed again, but DMA is
not, with some restrictions. This is NOT a callback for the driver to
start operations again, only to peek/poke at the device, extract diagnostic
information, if any, and eventually do things like trigger a device local
reset or some such, but not restart operations. This callback is made if
all drivers on a segment agree that they can try to recover and if no automatic
link reset was performed by the HW. If the platform can't just re-enable IOs
without a slot reset or a link reset, it will not call this callback, and
instead will have gone directly to STEP 3 (Link Reset) or STEP 4 (Slot Reset)
"조기 복구"입니다. IO는 다시 지시하지 않습니다. 작업이 실행되기 시작하고, 언제 작동하는지 확인하고, 어떤 장치가 작동하고 있는지, 어떤 장치가 작동하는지 확인하고 있습니다. 캬. 이 구성의 모든 부분에서 강력한 성능을 발휘할 수 있을 것입니다. 이 시스템이 다시 활성화되지 않으면 IO가 없을 것입니다.

.. note::

   The following is proposed; no platform implements this yet:
   Proposal: All I/O's should be done _synchronously_ from within
   this callback, errors triggered by them will be returned via
   the normal pci_check_whatever() API, no new error_detected()
   callback will be issued due to an error happening here. However,
   such an error might cause IOs to be re-blocked for the whole
   segment, and thus invalidate the recovery that other devices
   on the same segment might have done, forcing the whole segment
   into one of the next states, that is, link reset or slot reset.

다음은 제안 사항입니다. 아직 이를 구현한 플랫폼은 없습니다. 제안: 모든 I/O는 이 콜백 내에서 _동기적으로_ 수행되어야 합니다. 이 콜백에 의해 트리거된 오류는 일반 pci_check_whathe() API를 통해 반환됩니다. 여기서 발생하는 오류로 인해 새 error_detected() 콜백은 발행되지 않습니다. 그러나 이러한 오류로 인해 전체 세그먼트에 대해 IO가 다시 차단될 수 있으므로 동일한 세그먼트에 있는 다른 디바이스에서 수행했을 수 있는 복구가 무효화되어 전체 세그먼트가 다음 상태, 즉 링크 재설정 또는 슬롯 재설정 중 하나가 될 수 있습니다.

The driver should return one of the following result codes:
운전자는 다음 결과 코드 중 하나를 반환해야 합니다.
  - PCI_ERS_RESULT_RECOVERED
      Driver returns this if it thinks the device is fully
      functional and thinks it is ready to start
      normal driver operations again. There is no
      guarantee that the driver will actually be
      allowed to proceed, as another driver on the
      same segment might have failed and thus triggered a
      slot reset on platforms that support it.
      드라이버가 장치가 완전히 작동하고 정상적인 드라이버 작업을 다시 시작할 준비가 되었다고 생각하는 경우 이 값을 반환합니다. 동일한 세그먼트에 있는 다른 드라이버가 실패하여 이를 지원하는 플랫폼에서 슬롯 리셋을 트리거했을 수 있기 때문에 드라이버가 실제로 계속 진행할 수 있다는 보장은 없습니다.

  - PCI_ERS_RESULT_NEED_RESET
      Driver returns this if it thinks the device is not
      recoverable in its current state and it needs a slot
      reset to proceed.
      현재 상태에서 장치를 복구할 수 없고 계속하려면 슬롯을 재설정해야 한다고 생각하는 경우 드라이버가 이 값을 반환합니다.

  - PCI_ERS_RESULT_DISCONNECT
      Same as above. Total failure, no recovery even after
      reset driver dead. (To be defined more precisely)
위와 같다. 전체 고장, 재설정 드라이버 데드 후에도 복구되지 않음. (더 정확하게 정의하기 위해)

The next step taken depends on the results returned by the drivers.
If all drivers returned PCI_ERS_RESULT_RECOVERED, then the platform
proceeds to either STEP3 (Link Reset) or to STEP 5 (Resume Operations).
다음 단계는 운전자가 반환한 결과에 따라 달라집니다. 모든 드라이버가 PCI_ERS_RESULT_RECOVERED를 반환한 경우 플랫폼은 STEP3(Link Reset) 또는 STEP 5(Resume Operations)로 진행합니다.

If any driver returned PCI_ERS_RESULT_NEED_RESET, then the platform
proceeds to STEP 4 (Slot Reset)
PCI_ERS_RESULT_NEED_RESET를 반환한 드라이버가 있으면 플랫폼은 4단계(슬롯 재설정)로 진행됩니다.

STEP 3: Link Reset
------------------
The platform resets the link.  This is a PCI-Express specific step
and is done whenever a fatal error has been detected that can be
"solved" by resetting the link.
플랫폼이 링크를 재설정합니다. 이 단계는 PCI-Express 관련 단계로, 링크를 재설정하여 "해결"할 수 있는 치명적인 오류가 감지될 때마다 수행됩니다.


STEP 4: Slot Reset
------------------

In response to a return value of PCI_ERS_RESULT_NEED_RESET, the
the platform will perform a slot reset on the requesting PCI device(s).
The actual steps taken by a platform to perform a slot reset
will be platform-dependent. Upon completion of slot reset, the
platform will call the device slot_reset() callback.
PCI_ERS_RESULT_NEED_RESET의 반환 값에 응답하여 플랫폼은 요청한 PCI 디바이스에서 슬롯 재설정을 수행합니다. 슬롯 재설정을 수행하기 위해 플랫폼에 의해 수행되는 실제 단계는 플랫폼에 따라 달라집니다. 슬롯 재설정이 완료되면 플랫폼이 장치 slot_reset() 콜백을 호출합니다.

Powerpc platforms implement two levels of slot reset:
soft reset(default) and fundamental(optional) reset.
PowerPC 플랫폼은 소프트 리셋(기본값)과 기본 리셋(선택사항)의 두 가지 수준의 슬롯 리셋을 구현합니다.

Powerpc soft reset consists of asserting the adapter #RST line and then
restoring the PCI BAR's and PCI configuration header to a state
that is equivalent to what it would be after a fresh system
power-on followed by power-on BIOS/system firmware initialization.
Soft reset is also known as hot-reset.
Powerpc 소프트웨어 재설정은 어댑터 #RST 라인을 확인한 다음 PCI BAR 및 PCI 구성 헤더를 시스템 전원을 새로 켠 후 BIOS/시스템 펌웨어 초기화와 같은 상태로 복원하는 것으로 구성됩니다. 소프트 리셋은 핫 리셋이라고도 합니다.

Powerpc fundamental reset is supported by PCI Express cards only
and results in device's state machines, hardware logic, port states and
configuration registers to initialize to their default conditions.
Powerpc 기본 재설정은 PCI Express 카드에서만 지원되며 장치의 상태 시스템, 하드웨어 논리, 포트 상태 및 구성 레지스터를 기본 상태로 초기화합니다.

For most PCI devices, a soft reset will be sufficient for recovery.
Optional fundamental reset is provided to support a limited number
of PCI Express devices for which a soft reset is not sufficient
for recovery.
대부분의 PCI 디바이스의 경우 소프트 리셋으로 복구하기에 충분합니다. 기본 재설정은 소프트 재설정이 복구에 충분하지 않은 제한된 수의 PCI Express 디바이스를 지원하기 위해 제공됩니다.

If the platform supports PCI hotplug, then the reset might be
performed by toggling the slot electrical power off/on.
플랫폼에서 PCI 핫 플러그를 지원하는 경우 슬롯의 전기 전원을 끄거나 켜서 재설정할 수 있습니다.

It is important for the platform to restore the PCI config space
to the "fresh poweron" state, rather than the "last state". After
a slot reset, the device driver will almost always use its standard
device initialization routines, and an unusual config space setup
may result in hung devices, kernel panics, or silent data corruption.
플랫폼이 PCI 구성 공간을 "마지막 상태"가 아닌 "새로 전원을 켠 상태"로 복원하는 것이 중요하다. 슬롯 재설정 후 장치 드라이버는 거의 항상 표준 장치 초기화 루틴을 사용하며 비정상적인 구성 공간 설정으로 인해 장치 중단, 커널 패닉 또는 조용한 데이터 손상이 발생할 수 있습니다.

This call gives drivers the chance to re-initialize the hardware
(re-download firmware, etc.).  At this point, the driver may assume
that the card is in a fresh state and is fully functional. The slot
is unfrozen and the driver has full access to PCI config space,
memory mapped I/O space and DMA. Interrupts (Legacy, MSI, or MSI-X)
will also be available.
이 호출을 통해 드라이버는 하드웨어를 다시 초기화할 수 있습니다(펌웨어 다시 다운로드 등). 이때 운전자는 카드가 새 상태이며 완전히 작동한다고 가정할 수 있습니다. 슬롯이 풀리고 드라이버가 PCI 구성 공간, 메모리 매핑 I/O 공간 및 DMA에 대한 전체 액세스 권한을 가집니다. 인터럽트(레거시, MSI 또는 MSI-X)도 사용할 수 있습니다.

Drivers should not restart normal I/O processing operations
at this point.  If all device drivers report success on this
callback, the platform will call resume() to complete the sequence,
and let the driver restart normal I/O processing.
드라이버는 이 시점에서 정상적인 I/O 처리 작업을 다시 시작하지 않아야 합니다. 모든 장치 드라이버가 이 콜백에 성공했다고 보고하면 플랫폼은 resume()를 호출하여 시퀀스를 완료하고 드라이버가 정상적인 I/O 처리를 다시 시작할 수 있도록 합니다.

A driver can still return a critical failure for this function if
it can't get the device operational after reset.  If the platform
previously tried a soft reset, it might now try a hard reset (power
cycle) and then call slot_reset() again.  It the device still can't
be recovered, there is nothing more that can be done;  the platform
will typically report a "permanent failure" in such a case.  The
device will be considered "dead" in this case.
재설정 후 장치를 작동할 수 없는 경우에도 드라이버는 이 기능에 대한 심각한 오류를 반환할 수 있습니다. 이전에 플랫폼이 소프트 리셋을 시도했다면 이제 하드 리셋(전원 주기)을 시도한 다음 slot_reset()을 다시 호출할 수 있습니다. 장치를 여전히 복구할 수 없으면 더 이상 할 수 있는 일이 없습니다. 이러한 경우 플랫폼은 일반적으로 "영구적 오류"를 보고합니다. 이 경우 장치는 "죽은 상태"로 간주됩니다.

Drivers for multi-function cards will need to coordinate among
themselves as to which driver instance will perform any "one-shot"
or global device initialization. For example, the Symbios sym53cxx2
driver performs device init only from PCI function 0::
다기능 카드용 드라이버는 어떤 드라이버 인스턴스가 "원샷" 또는 전역 장치 초기화를 수행할지 서로 조정해야 합니다. 예를 들어 Symbios sym53cxx2 드라이버는 PCI 함수 0::에서만 디바이스를 수행합니다.

	+       if (PCI_FUNC(pdev->devfn) == 0)
	+               sym_reset_scsi_bus(np, 0);

Result codes:
	- PCI_ERS_RESULT_DISCONNECT
	  Same as above.

Drivers for PCI Express cards that require a fundamental reset must
set the needs_freset bit in the pci_dev structure in their probe function.
For example, the QLogic qla2xxx driver sets the needs_freset bit for certain
PCI card types::
기본 재설정이 필요한 PCI Express 카드용 드라이버는 프로브 함수의 pci_dev 구조에서 needs_freset 비트를 설정해야 합니다. 예를 들어 QLogic qla2xx 드라이버는 특정 PCI 카드 유형에 대해 needs_freset 비트를 설정합니다.

	+	/* Set EEH reset type to fundamental if required by hba  */
	+	if (IS_QLA24XX(ha) || IS_QLA25XX(ha) || IS_QLA81XX(ha))
	+		pdev->needs_freset = 1;
	+

Platform proceeds either to STEP 5 (Resume Operations) or STEP 6 (Permanent
Failure).
플랫폼은 5단계(운영 재개) 또는 6단계(영구적 실패)로 진행됩니다.

.. note::

   The current powerpc implementation does not try a power-cycle
   reset if the driver returned PCI_ERS_RESULT_DISCONNECT.
   However, it probably should.
드라이버가 PCI_ERS_RESULT_DISCONNECT를 반환한 경우 현재 powerpc 구현에서는 전원 주기 재설정을 시도하지 않습니다. 하지만, 아마 그래야 할 것이다.


STEP 5: Resume Operations
-------------------------
The platform will call the resume() callback on all affected device
drivers if all drivers on the segment have returned
PCI_ERS_RESULT_RECOVERED from one of the 3 previous callbacks.
The goal of this callback is to tell the driver to restart activity,
that everything is back and running. This callback does not return
a result code.
세그먼트의 모든 드라이버가 이전 3개의 콜백 중 하나에서 PCI_ERSULT_RECOVERED를 반환한 경우 플랫폼은 영향을 받는 모든 장치 드라이버에 대해 resume() 콜백을 호출합니다. 이 콜백의 목적은 운전자에게 활동을 재시작하고 모든 것이 정상 작동 중임을 알리는 것입니다. 이 콜백은 결과 코드를 반환하지 않습니다.

At this point, if a new error happens, the platform will restart
a new error recovery sequence.
이때 새 오류가 발생하면 플랫폼이 새 오류 복구 시퀀스를 다시 시작합니다.


STEP 6: Permanent Failure
-------------------------
A "permanent failure" has occurred, and the platform cannot recover
the device.  The platform will call error_detected() with a
pci_channel_state value of pci_channel_io_perm_failure.
"영구적 오류"가 발생하여 플랫폼에서 장치를 복구할 수 없습니다. 플랫폼에서 pci_channel_state 값이 pci_channel_io_perm_failure인 error_detected()를 호출합니다.

The device driver should, at this point, assume the worst. It should
cancel all pending I/O, refuse all new I/O, returning -EIO to
higher layers. The device driver should then clean up all of its
memory and remove itself from kernel operations, much as it would
during system shutdown.
장치 드라이버는 이 시점에서 최악의 경우를 가정해야 한다. 보류 중인 모든 I/O를 취소하고, 모든 새 I/O를 거부하며, -EIO를 상위 계층으로 되돌려야 합니다. 그런 다음 장치 드라이버는 시스템 종료 시처럼 모든 메모리를 정리하고 커널 작업에서 자신을 제거해야 합니다.

The platform will typically notify the system operator of the
permanent failure in some way.  If the device is hotplug-capable,
the operator will probably want to remove and replace the device.
Note, however, not all failures are truly "permanent". Some are
caused by over-heating, some by a poorly seated card. Many
PCI error events are caused by software bugs, e.g. DMA's to
wild addresses or bogus split transactions due to programming
errors. See the discussion in powerpc/eeh-pci-error-recovery.txt
for additional detail on real-life experience of the causes of
software errors.
플랫폼은 일반적으로 어떤 방식으로든 시스템 운영자에게 영구적인 고장을 통지한다. 장치가 핫 플러그 지원인 경우 작업자는 장치를 제거하고 교체하기를 원할 수 있습니다. 그러나 모든 고장이 진정으로 "영구적"인 것은 아닙니다. 어떤 것은 과열로 인한 것이고, 어떤 것은 제대로 앉지 못한 카드로 인한 것이다. 많은 PCI 오류 이벤트는 소프트웨어 버그(예: 프로그래밍 오류로 인한 Wild Address로의 DMA 또는 가짜 분할 트랜잭션)로 인해 발생합니다. powerpc/eeh-pci-error-recovery의 설명을 참조하십시오.소프트웨어 오류의 원인에 대한 실제 경험에 대한 자세한 내용을 보려면 txt를 참조하십시오.

Conclusion; General Remarks
---------------------------
The way the callbacks are called is platform policy. A platform with
no slot reset capability may want to just "ignore" drivers that can't
recover (disconnect them) and try to let other cards on the same segment
recover. Keep in mind that in most real life cases, though, there will
be only one driver per segment.
콜백이 호출되는 방식은 플랫폼 정책입니다. 슬롯 재설정 기능이 없는 플랫폼은 복구할 수 없는 드라이버를 "무시"하고 동일한 세그먼트에 있는 다른 카드가 복구되도록 할 수 있습니다. 그러나 대부분의 실제 상황에서는 세그먼트당 하나의 드라이버만 있다는 점을 명심하십시오.

Now, a note about interrupts. If you get an interrupt and your
device is dead or has been isolated, there is a problem :)
The current policy is to turn this into a platform policy.
That is, the recovery API only requires that:
이제 인터럽트에 대한 메모입니다. 인터럽트가 발생하고 장치가 작동하지 않거나 분리된 경우 문제가 있습니다:) 현재 정책은 이를 플랫폼 정책으로 전환하는 것입니다. 즉, 복구 API에는 다음 사항만 필요합니다.

 - There is no guarantee that interrupt delivery can proceed from any
   device on the segment starting from the error detection and until the
   slot_reset callback is called, at which point interrupts are expected
   to be fully operational.
- 인터럽트 전달이 오류 감지에서 시작하여 slot_reset 콜백이 호출될 때까지 세그먼트의 어떤 장치로부터도 진행된다는 보장은 없다.

 - There is no guarantee that interrupt delivery is stopped, that is,
   a driver that gets an interrupt after detecting an error, or that detects
   an error within the interrupt handler such that it prevents proper
   ack'ing of the interrupt (and thus removal of the source) should just
   return IRQ_NOTHANDLED. It's up to the platform to deal with that
   condition, typically by masking the IRQ source during the duration of
   the error handling. It is expected that the platform "knows" which
   interrupts are routed to error-management capable slots and can deal
   with temporarily disabling that IRQ number during error processing (this
   isn't terribly complex). That means some IRQ latency for other devices
   sharing the interrupt, but there is simply no other way. High end
   platforms aren't supposed to share interrupts between many devices
   anyway :)
- 인터럽트 전달이 중단된다는 보장은 없다. 즉, 오류를 감지한 드라이버나 인터럽트 핸들러 내에서 오류를 감지하여 인터럽트의 적절한 확인(따라서 소스 제거)을 방지한 드라이버는 IRQ_NOTHANDLED를 반환해야 한다. 오류 처리 기간 동안 일반적으로 IRQ 소스를 마스킹하여 해당 상태를 처리하는 것은 플랫폼에 달려 있습니다. 플랫폼은 어떤 인터럽트가 오류 관리 가능한 슬롯으로 라우팅되는지 알고 오류 처리 중에 일시적으로 IRQ 번호를 비활성화할 수 있습니다(이것은 매우 복잡하지 않습니다). 그것은 인터럽트를 공유하는 다른 장치들의 IRQ 지연을 의미하지만, 단순히 다른 방법이 없다. 하이엔드 플랫폼은 여러 장치 간에 인터럽트를 공유해서는 안 됩니다.

.. note::

   Implementation details for the powerpc platform are discussed in
   the file Documentation/powerpc/eeh-pci-error-recovery.rst
powerpc 플랫폼에 대한 구현 세부 사항은 Documentation/powerpc/eeh-pci-error-recovery.rst 파일에 설명되어 있습니다.

   As of this writing, there is a growing list of device drivers with
   patches implementing error recovery. Not all of these patches are in
   mainline yet. These may be used as "examples":
이 문서 작성 시점부터 오류 복구를 구현하는 패치가 있는 장치 드라이버 목록이 늘어나고 있습니다. 이러한 패치가 모두 아직 메인라인은 아닙니다. 이는 "예"로 사용될 수 있다.

   - drivers/scsi/ipr
   - drivers/scsi/sym53c8xx_2
   - drivers/scsi/qla2xxx
   - drivers/scsi/lpfc
   - drivers/next/bnx2.c
   - drivers/next/e100.c
   - drivers/net/e1000
   - drivers/net/e1000e
   - drivers/net/ixgb
   - drivers/net/ixgbe
   - drivers/net/cxgb3
   - drivers/net/s2io.c

The End
-------
