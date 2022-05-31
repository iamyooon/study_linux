.. SPDX-License-Identifier: GPL-2.0

=================
PCI Test Function
=================

:Author: Kishon Vijay Abraham I <kishon@ti.com>

Traditionally PCI RC has always been validated by using standard PCI cards like ethernet PCI cards or USB PCI cards or SATA PCI cards.  However with the addition of EP-core in linux kernel, it is possible to configure a PCI controller that can operate in EP mode to work as a test device.

The PCI endpoint test device is a virtual device (defined in software) used to test the endpoint functionality and serve as a sample driver for other PCI endpoint devices (to use the EP framework).

The PCI endpoint test device has the following registers:
전통적으로 PCI RC는 이더넷 PCI 카드, USB PCI 카드 또는 SATA PCI 카드와 같은 표준 PCI 카드를 사용하여 항상 검증되었습니다. 그러나 리눅스 커널에 EP-core가 추가되면서 EP 모드로 동작할 수 있는 PCI 컨트롤러를 테스트 장치로 구성할 수 있게 됐다.

PCI 끝점 테스트 장치는 끝점 기능을 테스트하고 다른 PCI 끝점 장치(EP 프레임워크 사용)에 대한 샘플 드라이버 역할을 하는 데 사용되는 가상 장치(소프트웨어에 정의됨)입니다.

PCI 끝점 테스트 장치에는 다음 레지스터가 있습니다.

	1) PCI_ENDPOINT_TEST_MAGIC
	2) PCI_ENDPOINT_TEST_COMMAND
	3) PCI_ENDPOINT_TEST_STATUS
	4) PCI_ENDPOINT_TEST_SRC_ADDR
	5) PCI_ENDPOINT_TEST_DST_ADDR
	6) PCI_ENDPOINT_TEST_SIZE
	7) PCI_ENDPOINT_TEST_CHECKSUM
	8) PCI_ENDPOINT_TEST_IRQ_TYPE
	9) PCI_ENDPOINT_TEST_IRQ_NUMBER

* PCI_ENDPOINT_TEST_MAGIC

This register will be used to test BAR0. A known pattern will be written and read back from MAGIC register to verify BAR0.

* PCI_ENDPOINT_TEST_COMMAND

This register will be used by the host driver to indicate the function that the endpoint device must perform.

* PCI_ENDPOINT_TEST_MAGIC

이 레지스터는 BAR0을 테스트하는 데 사용됩니다. 알려진 패턴이 기록되고 BAR0을 확인하기 위해 MAGIC 레지스터에서 다시 읽혀집니다.

* PCI_ENDPOINT_TEST_COMMAND

이 레지스터는 끝점 장치가 수행해야 하는 기능을 나타내기 위해 호스트 드라이버에 의해 사용됩니다.

========	================================================================
Bitfield	Description
========	================================================================
Bit 0		raise legacy IRQ
Bit 1		raise MSI IRQ
Bit 2		raise MSI-X IRQ
Bit 3		read command (read data from RC buffer)
Bit 4		write command (write data to RC buffer)
Bit 5		copy command (copy data from one RC buffer to another RC buffer)
========	================================================================
비트 0은 레거시 IRQ를 올립니다.
비트 1은 MSI IRQ를 올립니다.
비트 2는 MSI-X IRQ를 올립니다.
비트 3 읽기 명령(RC 버퍼에서 데이터 읽기)
비트 4 쓰기 명령(RC 버퍼에 데이터 쓰기)
비트 5 복사 명령(한 RC 버퍼에서 다른 RC 버퍼로 데이터 복사)

* PCI_ENDPOINT_TEST_STATUS

This register reflects the status of the PCI endpoint device.

========	==============================
Bitfield	Description
========	==============================
Bit 0		read success
Bit 1		read fail
Bit 2		write success
Bit 3		write fail
Bit 4		copy success
Bit 5		copy fail
Bit 6		IRQ raised
Bit 7		source address is invalid
Bit 8		destination address is invalid
========	==============================

* PCI_ENDPOINT_TEST_STATUS

이 레지스터는 PCI 끝점 장치의 상태를 반영합니다.

========================================
비트 필드 설명
========================================
비트 0 읽기 성공
비트 1 읽기 실패
비트 2 쓰기 성공
비트 3 쓰기 실패
비트 4 복사 성공
비트 5 복사 실패
비트 6 IRQ 상승
비트 7 소스 주소가 잘못되었습니다.
비트 8 대상 주소가 잘못되었습니다.
========================================

* PCI_ENDPOINT_TEST_SRC_ADDR

This register contains the source address (RC buffer address) for the COPY/READ command.

* PCI_ENDPOINT_TEST_DST_ADDR

This register contains the destination address (RC buffer address) for the COPY/WRITE command.

* PCI_ENDPOINT_TEST_IRQ_TYPE

This register contains the interrupt type (Legacy/MSI) triggered for the READ/WRITE/COPY and raise IRQ (Legacy/MSI) commands.

Possible types:

======	==
Legacy	0
MSI	1
MSI-X	2
======	==

* PCI_ENDPOINT_TEST_SRC_ADDR

이 레지스터는 COPY/READ 명령에 대한 소스 주소(RC 버퍼 주소)를 포함합니다.

* PCI_ENDPOINT_TEST_DST_ADDR

이 레지스터는 COPY/WRITE 명령에 대한 목적지 주소(RC 버퍼 주소)를 포함합니다.

* PCI_ENDPOINT_TEST_IRQ_TYPE

이 레지스터는 READ/WRITE/COPY 및 Rise IRQ(Legacy/MSI) 명령에 대해 트리거된 인터럽트 유형(Legacy/MSI)을 포함합니다.

가능한 유형:

====== ==
레거시 0
MSI 1
MSI-X 2
====== ==

* PCI_ENDPOINT_TEST_IRQ_NUMBER

This register contains the triggered ID interrupt.

Admissible values:

======	===========
Legacy	0
MSI	[1 .. 32]
MSI-X	[1 .. 2048]
======	===========

* PCI_ENDPOINT_TEST_IRQ_NUMBER

이 레지스터는 트리거된 ID 인터럽트를 포함합니다.

허용되는 값:

====== ===========
레거시 0
MSI [1 .. 32]
MSI-X [1 .. 2048]
====== ===========