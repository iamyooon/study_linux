.. SPDX-License-Identifier: GPL-2.0

:Author: Kishon Vijay Abraham I <kishon@ti.com>

This document is a guide to use the PCI Endpoint Framework in order to create endpoint controller driver, endpoint function driver, and using configfs interface to bind the function driver to the controller driver.
이 문서는 엔드포인트 컨트롤러 드라이버, 엔드포인트 기능 드라이버를 생성하기 위해 PCI 엔드포인트 프레임워크를 사용하고, 컨트롤러 드라이버에 기능 드라이버를 바인딩하기 위해 configfs 인터페이스를 사용하는 가이드입니다.

Introduction
============

Linux has a comprehensive PCI subsystem to support PCI controllers that operates in Root Complex mode. The subsystem has capability to scan PCI bus, assign memory resources and IRQ resources, load PCI driver (based on vendor ID, device ID), support other services like hot-plug, power management, advanced error reporting and virtual channels.

However the PCI controller IP integrated in some SoCs is capable of operating either in Root Complex mode or Endpoint mode. PCI Endpoint Framework will add endpoint mode support in Linux. This will help to run Linux in an EP system which can have a wide variety of use cases from testing or validation, co-processor accelerator, etc.
소개
=============

Linux에는 Root Complex 모드에서 작동하는 PCI 컨트롤러를 지원하는 포괄적인 PCI 하위 시스템이 있습니다. 하위 시스템은 PCI 버스를 스캔하고, 메모리 리소스와 IRQ 리소스를 할당하고, PCI 드라이버를 로드하고(공급업체 ID, 장치 ID 기반), 핫플러그, 전원 관리, 고급 오류 보고 및 가상 채널과 같은 기타 서비스를 지원하는 기능이 있습니다.

그러나 일부 SoC에 통합된 PCI 컨트롤러 IP는 Root Complex 모드 또는 Endpoint 모드에서 작동할 수 있습니다. PCI 끝점 프레임워크는 Linux에서 끝점 모드 지원을 추가합니다. 이것은 테스트 또는 검증, 보조 프로세서 가속기 등 다양한 사용 사례를 가질 수 있는 EP 시스템에서 Linux를 실행하는 데 도움이 됩니다.

PCI Endpoint Core
=================

The PCI Endpoint Core layer comprises 3 components: the Endpoint Controller library, the Endpoint Function library, and the configfs layer to bind the endpoint function with the endpoint controller.
PCI 엔드포인트 코어
==================

PCI 엔드포인트 코어 계층은 엔드포인트 컨트롤러 라이브러리, 엔드포인트 기능 라이브러리 및 엔드포인트 기능을 엔드포인트 컨트롤러와 바인딩하기 위한 configfs 레이어의 3가지 구성요소로 구성됩니다.

PCI Endpoint Controller(EPC) Library
------------------------------------

The EPC library provides APIs to be used by the controller that can operate in endpoint mode. It also provides APIs to be used by function driver/library in order to implement a particular endpoint function.
PCI 엔드포인트 컨트롤러(EPC) 라이브러리
------------------------------------

EPC 라이브러리는 엔드포인트 모드에서 작동할 수 있는 컨트롤러에서 사용할 API를 제공합니다. 또한 특정 끝점 기능을 구현하기 위해 기능 드라이버/라이브러리에서 사용할 API를 제공합니다.

APIs for the PCI controller Driver
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section lists the APIs that the PCI Endpoint core provides to be used by the PCI controller driver.
PCI 컨트롤러 드라이버용 API
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

이 섹션은 PCI 컨트롤러 드라이버에서 사용하기 위해 PCI 엔드포인트 코어가 제공하는 API를 나열합니다.

* devm_pci_epc_create()/pci_epc_create()

   The PCI controller driver should implement the following ops:

	 * write_header: ops to populate configuration space header
	 * set_bar: ops to configure the BAR
	 * clear_bar: ops to reset the BAR
	 * alloc_addr_space: ops to allocate in PCI controller address space
	 * free_addr_space: ops to free the allocated address space
	 * raise_irq: ops to raise a legacy, MSI or MSI-X interrupt
	 * start: ops to start the PCI link
	 * stop: ops to stop the PCI link


   The PCI controller driver can then create a new EPC device by invoking devm_pci_epc_create()/pci_epc_create().
* devm_pci_epc_create()/pci_epc_create()

    PCI 컨트롤러 드라이버는 다음 작업을 구현해야 합니다.

* write_header: 구성 공간 헤더를 채우는 작업
* set_bar: BAR을 구성하는 작업
* clear_bar: BAR을 재설정하는 작업
* alloc_addr_space: PCI 컨트롤러 주소 공간에 할당하는 연산
* free_addr_space: 할당된 주소 공간을 해제하는 작업
* raise_irq: 레거시, MSI 또는 MSI-X 인터럽트를 발생시키는 연산
* 시작: PCI 링크를 시작하는 ops
* stop: PCI 링크를 중지하는 연산


    그런 다음 PCI 컨트롤러 드라이버는 devm_pci_epc_create()/pci_epc_create()를 호출하여 새 EPC 장치를 생성할 수 있습니다.
	
* devm_pci_epc_destroy()/pci_epc_destroy()

   The PCI controller driver can destroy the EPC device created by either devm_pci_epc_create() or pci_epc_create() using devm_pci_epc_destroy() or pci_epc_destroy().
* devm_pci_epc_destroy()/pci_epc_destroy()

    PCI 컨트롤러 드라이버는 devm_pci_epc_destroy() 또는 pci_epc_destroy()를 사용하여 devm_pci_epc_create() 또는 pci_epc_create()에 의해 생성된 EPC 장치를 파괴할 수 있습니다.

* pci_epc_linkup()

   In order to notify all the function devices that the EPC device to which they are linked has established a link with the host, the PCI controller driver should invoke pci_epc_linkup().
* pci_epc_linkup()

    연결된 EPC 장치가 호스트와 연결되었음을 모든 기능 장치에 알리기 위해 PCI 컨트롤러 드라이버는 pci_epc_linkup()을 호출해야 합니다.
	
* pci_epc_mem_init()

   Initialize the pci_epc_mem structure used for allocating EPC addr space.
* pci_epc_mem_init()

    EPC addr 공간 할당에 사용되는 pci_epc_mem 구조를 초기화합니다.
	
* pci_epc_mem_exit()

   Cleanup the pci_epc_mem structure allocated during pci_epc_mem_init().
   * pci_epc_mem_exit()

    pci_epc_mem_init() 동안 할당된 pci_epc_mem 구조를 정리합니다.


APIs for the PCI Endpoint Function Driver
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section lists the APIs that the PCI Endpoint core provides to be used by the PCI endpoint function driver.
PCI 끝점 기능 드라이버용 API
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

이 섹션은 PCI 끝점 기능 드라이버에서 사용하기 위해 PCI 끝점 코어가 제공하는 API를 나열합니다.

* pci_epc_write_header()

   The PCI endpoint function driver should use pci_epc_write_header() to write the standard configuration header to the endpoint controller.
* pci_epc_write_header()

   PCI 끝점 기능 드라이버는 pci_epc_write_header()를 사용하여 끝점 컨트롤러에 표준 구성 헤더를 기록해야 합니다.
   
* pci_epc_set_bar()

   The PCI endpoint function driver should use pci_epc_set_bar() to configure the Base Address Register in order for the host to assign PCI addr space.  Register space of the function driver is usually configured using this API.
* pci_epc_set_bar()

   PCI 끝점 기능 드라이버는 호스트가 PCI addr 공간을 할당하도록 기본 주소 레지스터를 구성하기 위해 pci_epc_set_bar()를 사용해야 합니다. 함수 드라이버의 레지스터 공간은 일반적으로 이 API를 사용하여 구성됩니다.
   
* pci_epc_clear_bar()

   The PCI endpoint function driver should use pci_epc_clear_bar() to reset the BAR.
* pci_epc_clear_bar()

   PCI 끝점 기능 드라이버는 pci_epc_clear_bar()를 사용하여 BAR을 재설정해야 합니다.
   
* pci_epc_raise_irq()

   The PCI endpoint function driver should use pci_epc_raise_irq() to raise Legacy Interrupt, MSI or MSI-X Interrupt.
* pci_epc_raise_irq()

   PCI 끝점 기능 드라이버는 레거시 인터럽트, MSI 또는 MSI-X 인터럽트를 발생시키기 위해 pci_epc_raise_irq()를 사용해야 합니다.
   
* pci_epc_mem_alloc_addr()

   The PCI endpoint function driver should use pci_epc_mem_alloc_addr(), to allocate memory address from EPC addr space which is required to access RC's buffer
* pci_epc_mem_alloc_addr()

   PCI 끝점 기능 드라이버는 pci_epc_mem_alloc_addr()을 사용하여 RC 버퍼에 액세스하는 데 필요한 EPC addr 공간에서 메모리 주소를 할당해야 합니다.
   
* pci_epc_mem_free_addr()

   The PCI endpoint function driver should use pci_epc_mem_free_addr() to free the memory space allocated using pci_epc_mem_alloc_addr().
* pci_epc_mem_free_addr()

   PCI 끝점 기능 드라이버는 pci_epc_mem_alloc_addr()을 사용하여 할당된 메모리 공간을 해제하기 위해 pci_epc_mem_free_addr()을 사용해야 합니다.
   
Other APIs
~~~~~~~~~~

There are other APIs provided by the EPC library. These are used for binding the EPF device with EPC device. pci-ep-cfs.c can be used as reference for using these APIs.
기타 API
~~~~~~~~~~

EPC 라이브러리에서 제공하는 다른 API가 있습니다. EPF 장치와 EPC 장치를 바인딩하는 데 사용됩니다. pci-ep-cfs.c는 이러한 API를 사용하기 위한 참조로 사용할 수 있습니다.

* pci_epc_get()

   Get a reference to the PCI endpoint controller based on the device name of the controller.
* pci_epc_get()

    컨트롤러의 장치 이름을 기반으로 PCI 끝점 컨트롤러에 대한 참조를 가져옵니다.
	
* pci_epc_put()

   Release the reference to the PCI endpoint controller obtained using pci_epc_get()
* pci_epc_put()

    pci_epc_get()을 사용하여 얻은 PCI 끝점 컨트롤러에 대한 참조를 해제합니다.
	
* pci_epc_add_epf()

   Add a PCI endpoint function to a PCI endpoint controller. A PCIe device can have up to 8 functions according to the specification.
* pci_epc_add_epf()

    PCI 끝점 컨트롤러에 PCI 끝점 기능을 추가합니다. PCIe 장치는 사양에 따라 최대 8개의 기능을 가질 수 있습니다.
	
* pci_epc_remove_epf()

   Remove the PCI endpoint function from PCI endpoint controller.
* pci_epc_remove_epf()

    PCI 끝점 컨트롤러에서 PCI 끝점 기능을 제거합니다.
	
* pci_epc_start()

   The PCI endpoint function driver should invoke pci_epc_start() once it has configured the endpoint function and wants to start the PCI link.
* pci_epc_start()

    PCI 끝점 기능 드라이버는 끝점 기능을 구성하고 PCI 링크를 시작하려는 경우 pci_epc_start()를 호출해야 합니다.
	
* pci_epc_stop()

   The PCI endpoint function driver should invoke pci_epc_stop() to stop the PCI LINK.
* pci_epc_stop()

    PCI 끝점 기능 드라이버는 PCI LINK를 중지하기 위해 pci_epc_stop()을 호출해야 합니다.

PCI Endpoint Function(EPF) Library
----------------------------------

The EPF library provides APIs to be used by the function driver and the EPC library to provide endpoint mode functionality.

PCI 엔드포인트 기능(EPF) 라이브러리
----------------------------------

EPF 라이브러리는 기능 드라이버에서 사용할 API와 엔드포인트 모드 기능을 제공하기 위해 EPC 라이브러리를 제공합니다.

APIs for the PCI Endpoint Function Driver
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section lists the APIs that the PCI Endpoint core provides to be used by the PCI endpoint function driver.
PCI 끝점 기능 드라이버용 API
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

이 섹션은 PCI 끝점 기능 드라이버에서 사용하기 위해 PCI 끝점 코어가 제공하는 API를 나열합니다.

* pci_epf_register_driver()

   The PCI Endpoint Function driver should implement the following ops:
	 * bind: ops to perform when a EPC device has been bound to EPF device
	 * unbind: ops to perform when a binding has been lost between a EPC device and EPF device
	 * linkup: ops to perform when the EPC device has established a connection with a host system

  The PCI Function driver can then register the PCI EPF driver by using pci_epf_register_driver().

* pci_epf_unregister_driver()

  The PCI Function driver can unregister the PCI EPF driver by using pci_epf_unregister_driver().

* pci_epf_alloc_space()

  The PCI Function driver can allocate space for a particular BAR using pci_epf_alloc_space().

* pci_epf_free_space()

  The PCI Function driver can free the allocated space (using pci_epf_alloc_space) by invoking pci_epf_free_space().
* pci_epf_register_driver()

    PCI 끝점 기능 드라이버는 다음 작업을 구현해야 합니다.
* bind: EPC 장치가 EPF 장치에 바인딩되었을 때 수행하는 작업
* unbind: EPC 장치와 EPF 장치 간의 바인딩이 손실되었을 때 수행하는 작업
* linkup: EPC 장치가 호스트 시스템과 연결을 설정했을 때 수행하는 작업

   PCI 기능 드라이버는 pci_epf_register_driver()를 사용하여 PCI EPF 드라이버를 등록할 수 있습니다.

* pci_epf_unregister_driver()

   PCI 기능 드라이버는 pci_epf_unregister_driver()를 사용하여 PCI EPF 드라이버를 등록 취소할 수 있습니다.

* pci_epf_alloc_space()

   PCI 기능 드라이버는 pci_epf_alloc_space()를 사용하여 특정 BAR에 공간을 할당할 수 있습니다.

* pci_epf_free_space()

   PCI 기능 드라이버는 pci_epf_free_space()를 호출하여 할당된 공간을 해제할 수 있습니다(pci_epf_alloc_space 사용).
   
APIs for the PCI Endpoint Controller Library
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section lists the APIs that the PCI Endpoint core provides to be used by the PCI endpoint controller library.
PCI 엔드포인트 컨트롤러 라이브러리용 API
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

이 섹션에는 PCI 끝점 컨트롤러 라이브러리에서 사용하기 위해 PCI 끝점 코어가 제공하는 API가 나열되어 있습니다.

* pci_epf_linkup()

   The PCI endpoint controller library invokes pci_epf_linkup() when the EPC device has established the connection to the host.
* pci_epf_linkup()

    PCI 끝점 컨트롤러 라이브러리는 EPC 장치가 호스트에 대한 연결을 설정하면 pci_epf_linkup()을 호출합니다.
	
Other APIs
~~~~~~~~~~

There are other APIs provided by the EPF library. These are used to notify the function driver when the EPF device is bound to the EPC device.  pci-ep-cfs.c can be used as reference for using these APIs.
기타 API
~~~~~~~~~~

EPF 라이브러리에서 제공하는 다른 API가 있습니다. 이것은 EPF 장치가 EPC 장치에 바인딩될 때 기능 드라이버에 알리는 데 사용됩니다. pci-ep-cfs.c는 이러한 API를 사용하기 위한 참조로 사용할 수 있습니다.

* pci_epf_create()

   Create a new PCI EPF device by passing the name of the PCI EPF device.  This name will be used to bind the the EPF device to a EPF driver.

* pci_epf_destroy()

   Destroy the created PCI EPF device.

* pci_epf_bind()

   pci_epf_bind() should be invoked when the EPF device has been bound to a EPC device.

* pci_epf_unbind()

   pci_epf_unbind() should be invoked when the binding between EPC device and EPF device is lost.
* pci_epf_create()

    PCI EPF 장치의 이름을 전달하여 새 PCI EPF 장치를 만듭니다. 이 이름은 EPF 장치를 EPF 드라이버에 바인딩하는 데 사용됩니다.

* pci_epf_destroy()

    생성된 PCI EPF 장치를 파괴합니다.

* pci_epf_bind()

    pci_epf_bind()는 EPF 장치가 EPC 장치에 바인딩되었을 때 호출되어야 합니다.

* pci_epf_unbind()

    pci_epf_unbind()는 EPC 장치와 EPF 장치 간의 바인딩이 손실될 때 호출되어야 합니다.