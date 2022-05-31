.. SPDX-License-Identifier: GPL-2.0
.. include:: <isonum.txt>

===========================================
The PCI Express Port Bus Driver Guide HOWTO
===========================================

:Author: Tom L Nguyen tom.l.nguyen@intel.com 11/03/2004
:Copyright: |copy| 2004 Intel Corporation

About this guide
================

This guide describes the basics of the PCI Express Port Bus driver
and provides information on how to enable the service drivers to
register/unregister with the PCI Express Port Bus Driver.
이 안내서에서는 PCI Express 포트 버스 드라이버의 기본 사항에 대해 설명하고 서비스 드라이버가 PCI Express 포트 버스 드라이버에 등록/등록을 취소할 수 있도록 하는 방법에 대한 정보를 제공합니다.

What is the PCI Express Port Bus Driver
=======================================

A PCI Express Port is a logical PCI-PCI Bridge structure. There
are two types of PCI Express Port: the Root Port and the Switch
Port. The Root Port originates a PCI Express link from a PCI Express
Root Complex and the Switch Port connects PCI Express links to
internal logical PCI buses. The Switch Port, which has its secondary
bus representing the switch's internal routing logic, is called the
switch's Upstream Port. The switch's Downstream Port is bridging from
switch's internal routing bus to a bus representing the downstream
PCI Express link from the PCI Express Switch.
PCI Express 포트는 논리적 PCI-PCI 브리지 구조입니다. PCI Express 포트에는 루트 포트와 스위치 포트라는 두 가지 유형이 있습니다. 루트 포트는 PCI Express 루트 복합체에서 PCI Express 링크를 시작하고 스위치 포트는 PCI Express 링크를 내부 논리적 PCI 버스에 연결합니다. 스위치의 내부 라우팅 로직을 나타내는 보조 버스가 있는 스위치 포트를 스위치의 업스트림 포트라고 합니다. 스위치의 다운스트림 포트가 스위치의 내부 라우팅 버스에서 PCI Express 스위치의 다운스트림 PCI Express 링크를 나타내는 버스로 브리징되고 있습니다.

A PCI Express Port can provide up to four distinct functions,
referred to in this document as services, depending on its port type.
PCI Express Port's services include native hotplug support (HP),
power management event support (PME), advanced error reporting
support (AER), and virtual channel support (VC). These services may
be handled by a single complex driver or be individually distributed
and handled by corresponding service drivers.
PCI Express 포트는 포트 유형에 따라 이 문서에서 서비스라고 하는 최대 4가지 기능을 제공할 수 있습니다. PCI 익스프레스 포트의 서비스에는 HP(Native Hotplug Support), PME(Power Management Event Support), AER(Advanced Error Reporting Support), VC(Virtual Channel Support) 등이 있습니다. 이러한 서비스는 단일 복합 드라이버에 의해 처리되거나 해당 서비스 드라이버에 의해 개별적으로 배포 및 처리될 수 있습니다.

Why use the PCI Express Port Bus Driver?
========================================

In existing Linux kernels, the Linux Device Driver Model allows a
physical device to be handled by only a single driver. The PCI
Express Port is a PCI-PCI Bridge device with multiple distinct
services. To maintain a clean and simple solution each service
may have its own software service driver. In this case several
service drivers will compete for a single PCI-PCI Bridge device.
For example, if the PCI Express Root Port native hotplug service
driver is loaded first, it claims a PCI-PCI Bridge Root Port. The
kernel therefore does not load other service drivers for that Root
Port. In other words, it is impossible to have multiple service
drivers load and run on a PCI-PCI Bridge device simultaneously
using the current driver model.
기존 리눅스 커널에서 리눅스 장치 드라이버 모델은 물리적 장치를 단일 드라이버로만 처리할 수 있게 한다. PCI 익스프레스 포트는 여러 개의 개별 서비스가 있는 PCI-PCI 브리지 장치입니다. 깨끗하고 간단한 솔루션을 유지하기 위해 각 서비스에는 자체 소프트웨어 서비스 드라이버가 있을 수 있습니다. 이 경우 여러 서비스 드라이버가 단일 PCI-PCI Bridge 장치에 대해 경쟁합니다. 예를 들어 PCI Express 루트 포트 기본 핫 플러그 서비스 드라이버가 먼저 로드된 경우 PCI-PCI 브리지 루트 포트를 할당합니다. 따라서 커널은 해당 루트 포트에 대한 다른 서비스 드라이버를 로드하지 않습니다. 즉, 여러 서비스 드라이버가 현재 드라이버 모델을 사용하여 PCI-PCI 브리지 장치에서 동시에 로드되고 실행되도록 하는 것은 불가능하다.

To enable multiple service drivers running simultaneously requires
having a PCI Express Port Bus driver, which manages all populated
PCI Express Ports and distributes all provided service requests
to the corresponding service drivers as required. Some key
advantages of using the PCI Express Port Bus driver are listed below:
여러 서비스 드라이버를 동시에 실행하려면 PCI Express 포트 버스 드라이버가 있어야 합니다. 이 드라이버는 모든 채워진 PCI Express 포트를 관리하고 필요한 경우 제공된 모든 서비스 요청을 해당 서비스 드라이버에 배포합니다. PCI Express Port Bus 드라이버를 사용할 때의 몇 가지 주요 이점은 다음과 같습니다.

  - Allow multiple service drivers to run simultaneously on
    a PCI-PCI Bridge Port device.
- PCI-PCI 브리지 포트 장치에서 여러 서비스 드라이버를 동시에 실행할 수 있습니다.

  - Allow service drivers implemented in an independent
    staged approach.
- 서비스 드라이버를 독립적으로 단계적 접근 방식으로 구현할 수 있습니다.

  - Allow one service driver to run on multiple PCI-PCI Bridge
    Port devices.
- 하나의 서비스 드라이버가 여러 PCI-PCI 브리지 포트 디바이스에서 실행되도록 허용합니다.

  - Manage and distribute resources of a PCI-PCI Bridge Port
    device to requested service drivers.
- PCI-PCI Bridge Port 디바이스의 리소스를 관리하고 요청된 서비스 드라이버에 배포합니다.

Configuring the PCI Express Port Bus Driver vs. Service Drivers
===============================================================

Including the PCI Express Port Bus Driver Support into the Kernel
-----------------------------------------------------------------

Including the PCI Express Port Bus driver depends on whether the PCI
Express support is included in the kernel config. The kernel will
automatically include the PCI Express Port Bus driver as a kernel
driver when the PCI Express support is enabled in the kernel.
PCI Express 포트 버스 드라이버를 포함시키는 것은 PCI Express 지원이 커널 구성에 포함되는지 여부에 따라 달라집니다. 커널에서 PCI 익스프레스 지원이 활성화되면 커널은 자동으로 PCI 익스프레스 포트 버스 드라이버를 커널 드라이버로 포함한다.

Enabling Service Driver Support
-------------------------------

PCI device drivers are implemented based on Linux Device Driver Model.
All service drivers are PCI device drivers. As discussed above, it is
impossible to load any service driver once the kernel has loaded the
PCI Express Port Bus Driver. To meet the PCI Express Port Bus Driver
Model requires some minimal changes on existing service drivers that
imposes no impact on the functionality of existing service drivers.
PCI 디바이스 드라이버는 Linux 디바이스 드라이버 모델을 기반으로 구현됩니다. 모든 서비스 드라이버는 PCI 장치 드라이버입니다. 위에서 설명한 바와 같이 커널이 PCI 익스프레스 포트 버스 드라이버를 로드한 후에는 서비스 드라이버를 로드할 수 없습니다. PCI 익스프레스 포트 버스 드라이버 모델을 충족시키려면 기존 서비스 드라이버의 기능에 영향을 미치지 않는 기존 서비스 드라이버에 대한 최소한의 변경 사항이 필요합니다.

A service driver is required to use the two APIs shown below to
register its service with the PCI Express Port Bus driver (see
section 5.2.1 & 5.2.2). It is important that a service driver
initializes the pcie_port_service_driver data structure, included in
header file /include/linux/pcieport_if.h, before calling these APIs.
Failure to do so will result an identity mismatch, which prevents
the PCI Express Port Bus driver from loading a service driver.
서비스 드라이버는 PCI Express Port Bus 드라이버에 서비스를 등록하기 위해 아래에 표시된 두 개의 API를 사용해야 합니다(섹션 5.2.1 및 5.2.2 참조). 서비스 드라이버는 이러한 API를 호출하기 전에 헤더 파일 /include/linux/pcieport_if.h에 포함된 pci_port_service_driver 데이터 구조를 초기화해야 합니다. 그렇지 않으면 ID 불일치가 발생하여 PCI Express Port Bus 드라이버가 서비스 드라이버를 로드할 수 없습니다.

pcie_port_service_register
~~~~~~~~~~~~~~~~~~~~~~~~~~
::

  int pcie_port_service_register(struct pcie_port_service_driver *new)

This API replaces the Linux Driver Model's pci_register_driver API. A
service driver should always calls pcie_port_service_register at
module init. Note that after service driver being loaded, calls
such as pci_enable_device(dev) and pci_set_master(dev) are no longer
necessary since these calls are executed by the PCI Port Bus driver.
이 API는 리눅스 드라이버 모델의 pci_register_driver API를 대체합니다. 서비스 드라이버는 항상 모듈 init에서 pci_port_service_register를 호출해야 합니다. 서비스 드라이버가 로드된 후 pci_enable_device(dev) 및 pci_set_master(dev)와 같은 호출은 PCI 포트 버스 드라이버에 의해 실행되므로 더 이상 필요하지 않습니다.

pcie_port_service_unregister
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
::

  void pcie_port_service_unregister(struct pcie_port_service_driver *new)

pcie_port_service_unregister replaces the Linux Driver Model's
pci_unregister_driver. It's always called by service driver when a
module exits.
pci_port_service_unregister는 Linux 드라이버 모델의 pci_unregister_driver를 대체합니다. 모듈이 종료되면 항상 서비스 드라이버가 호출합니다.

Sample Code
~~~~~~~~~~~

Below is sample service driver code to initialize the port service
driver data structure.
::
다음은 포트 서비스 드라이버 데이터 구조를 초기화하기 위한 샘플 서비스 드라이버 코드입니다.

  static struct pcie_port_service_id service_id[] = { {
    .vendor = PCI_ANY_ID,
    .device = PCI_ANY_ID,
    .port_type = PCIE_RC_PORT,
    .service_type = PCIE_PORT_SERVICE_AER,
    }, { /* end: all zeroes */ }
  };

  static struct pcie_port_service_driver root_aerdrv = {
    .name		= (char *)device_name,
    .id_table	= &service_id[0],

    .probe		= aerdrv_load,
    .remove		= aerdrv_unload,

    .suspend	= aerdrv_suspend,
    .resume		= aerdrv_resume,
  };

Below is a sample code for registering/unregistering a service
driver.
::

  static int __init aerdrv_service_init(void)
  {
    int retval = 0;

    retval = pcie_port_service_register(&root_aerdrv);
    if (!retval) {
      /*
      * FIX ME
      */
    }
    return retval;
  }

  static void __exit aerdrv_service_exit(void)
  {
    pcie_port_service_unregister(&root_aerdrv);
  }

  module_init(aerdrv_service_init);
  module_exit(aerdrv_service_exit);

Possible Resource Conflicts
===========================

Since all service drivers of a PCI-PCI Bridge Port device are
allowed to run simultaneously, below lists a few of possible resource
conflicts with proposed solutions.
PCI-PCI 브리지 포트 장치의 모든 서비스 드라이버가 동시에 실행되도록 허용되므로 아래에는 제안된 솔루션과 충돌할 수 있는 몇 가지 리소스 충돌이 나열되어 있습니다.

MSI and MSI-X Vector Resource
-----------------------------

Once MSI or MSI-X interrupts are enabled on a device, it stays in this
mode until they are disabled again.  Since service drivers of the same
PCI-PCI Bridge port share the same physical device, if an individual
service driver enables or disables MSI/MSI-X mode it may result
unpredictable behavior.
장치에서 MSI 또는 MSI-X 인터럽트를 사용하도록 설정한 후에는 다시 사용하지 않도록 설정될 때까지 이 모드로 유지됩니다. 동일한 PCI-PCI Bridge 포트의 서비스 드라이버는 동일한 물리적 디바이스를 공유하므로 개별 서비스 드라이버가 MSI/MSI-X 모드를 사용하거나 사용하지 않도록 설정하면 예기치 않은 동작이 발생할 수 있습니다.

To avoid this situation all service drivers are not permitted to
switch interrupt mode on its device. The PCI Express Port Bus driver
is responsible for determining the interrupt mode and this should be
transparent to service drivers. Service drivers need to know only
the vector IRQ assigned to the field irq of struct pcie_device, which
is passed in when the PCI Express Port Bus driver probes each service
driver. Service drivers should use (struct pcie_device*)dev->irq to
call request_irq/free_irq. In addition, the interrupt mode is stored
in the field interrupt_mode of struct pcie_device.
이러한 상황을 방지하기 위해 모든 서비스 드라이버는 장치의 인터럽트 모드를 전환할 수 없습니다. PCI 익스프레스 포트 버스 드라이버는 인터럽트 모드를 결정하는 역할을 하며 이는 서비스 드라이버에 대해 투명해야 합니다. 서비스 드라이버는 PCI 익스프레스 포트 버스 드라이버가 각 서비스 드라이버를 탐색할 때 전달되는 pci_device 필드의 irq에 할당된 벡터 IRQ만 알면 된다. 서비스 드라이버는 request_irq/free_irq를 호출하기 위해 (struct pci_device*)dev->irq를 사용해야 합니다. 또한 인터럽트 모드는 pci_device 구조체의 필드 interrupt_mode에 저장된다.

PCI Memory/IO Mapped Regions
----------------------------

Service drivers for PCI Express Power Management (PME), Advanced
Error Reporting (AER), Hot-Plug (HP) and Virtual Channel (VC) access
PCI configuration space on the PCI Express port. In all cases the
registers accessed are independent of each other. This patch assumes
that all service drivers will be well behaved and not overwrite
other service driver's configuration settings.
PCI Express 전원 관리(PME), AER(고급 오류 보고), HP(핫 플러그) 및 VC(가상 채널)용 서비스 드라이버가 PCI Express 포트의 PCI 구성 공간에 액세스합니다. 모든 경우에 접근된 레지스터들은 서로 독립적이다. 이 패치는 모든 서비스 드라이버가 정상적으로 작동하며 다른 서비스 드라이버의 구성 설정을 덮어쓰지 않는다고 가정합니다.

PCI Config Registers
--------------------

Each service driver runs its PCI config operations on its own
capability structure except the PCI Express capability structure, in
which Root Control register and Device Control register are shared
between PME and AER. This patch assumes that all service drivers
will be well behaved and not overwrite other service driver's
configuration settings.
각 서비스 드라이버는 루트 제어 레지스터와 장치 제어 레지스터를 PME와 AER 간에 공유하는 PCI 익스프레스 기능 구조를 제외하고 자체 기능 구조에서 PCI 구성 작업을 실행합니다. 이 패치는 모든 서비스 드라이버가 정상적으로 작동하며 다른 서비스 드라이버의 구성 설정을 덮어쓰지 않는다고 가정합니다.
