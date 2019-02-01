Linux Magic System Request Key Hacks
====================================

Documentation for sysrq.c

마법의 sysrq 키는 무엇인가?
What is the magic SysRq key?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It is a 'magical' key combo you can hit which the kernel will respond to regardless of whatever else it is doing, unless it is completely locked up.
이 문서는 커널이 완전하게 락업상태가 아닌 한 커널이 수행하는 다른 작업에 상관없이 동작하는 커널의 '마법의'키 조합에 대한 것이다.

마법의 sysrq 키는 어떻게 활성화할까?
How do I enable the magic SysRq key?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You need to say "yes" to 'Magic SysRq key (CONFIG_MAGIC_SYSRQ)' when configuring the kernel. When running a kernel with SysRq compiled in, /proc/sys/kernel/sysrq controls the functions allowed to be invoked via the SysRq key. The default value in this file is set by the CONFIG_MAGIC_SYSRQ_DEFAULT_ENABLE config symbol, which itself defaults to 1. Here is the list of possible values in /proc/sys/kernel/sysrq:
커널 설정시에 마법의 sysrq 키(CONFIG_MAGIC_SYSRQ)를 yes로 설정해야 한다. sysrq가 컴파일된 커널이 동작중일때, /proc/sys/kernel/sysrq 파일은 sysrq 키를 통해 호출될 수 있는 기능에 대해 통제한다. 이 파일의 기본 값은 CONFIG_MAGIC_SYSRQ_DEFAULT_ENABLE 커널설정 값으로 설정되어 있다. 기본값은 1이다. 사용가능한 값의 리스트는 다음과 같다.

   -  0 - sysrq를 완전하게 비활성화함(disable sysrq completely)
   -  1 - sysrq의 모든 기능을 활성화함(enable all functions of sysrq)
   - >1 - 허용된 sysrq 기능의 비트조합(자세한 기능 설명은 아래를 보자)(bitmask of allowed sysrq functions (see below for detailed function description))::

          2 =   0x2 - console에 출력하는 로그레벨을 제어하는 기능 활성화(enable control of console logging level)
          4 =   0x4 - 키보드를 제어하는 기능 활성화(enable control of keyboard (SAK, unraw))
          8 =   0x8 - 프로세스들의 디버깅 덤프를 활성화(enable debugging dumps of processes etc.)
         16 =  0x10 - sync 명령어를 활성화(enable sync command)
         32 =  0x20 - read-only로 파일시스템을 remount하는 기능을 활성화(enable remount read-only)
         64 =  0x40 - 프로세스에게 시그널을 보내는 기능 활성화(enable signalling of processes (term, kill, oom-kill))
        128 =  0x80 - reboot/poweroff 기능 활성화(allow reboot/poweroff)
        256 = 0x100 - 모든 RT 태스크의 nice  기능 활성화(allow nicing of all RT tasks)

아래 명령어를 사용해서 값을 설정할 수 있다::
You can set the value in the file by the following command::

    echo "number" >/proc/sys/kernel/sysrq

숫자 값은 10진수나 0x로 시작하는 16진수로 설정할 수 있다. CONFIG_MAGIC_SYSRQ_DEFAULT_ENABLE 커널 설정은 16진수로 쓰여져야 한다.
The number may be written here either as decimal or as hexadecimal with the 0x prefix. CONFIG_MAGIC_SYSRQ_DEFAULT_ENABLE must always be written in hexadecimal.

/proc/sys/kernel/sysrq의 값은 오직 키보드를 통한 invocation으로만 influence된다. /proc/sysrq-trigger를 통한 어떠한 동작의 invocation도 항상 허용된다(관리자 권한을 가진 유저에 의해)
Note that the value of ``/proc/sys/kernel/sysrq`` influences only the invocation via a keyboard. Invocation of any operation via ``/proc/sysrq-trigger`` is always allowed (by a user with admin privileges).

마법의 sysrq 키를 어떻게 쓸 수 있을까?
How do I use the magic SysRq key?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

x86에서는 ALT + sysrq key + 명령어 키를 같이 누른다.
On x86   - You press the key combo :kbd:`ALT-SysRq-<command key>`.

.. note::
sysrq 레이블이 붙은 키가 존재하지 않는 키보드도 있다. sysrq 키는 print screen 키로도 알려져 있다. 동시에 여러개의 키가 눌리는것이 처리되지 않은 키보드도 존재하며, 이럴때는 alt 키와 sysrq 키를 누르다가 sysrq 키를 떼고 명령어키를 누른 다음 모든 키를 때는 방식으로 동작되길 기대해야 한다.
Some keyboards may not have a key labeled 'SysRq'. The 'SysRq' key is also known as the 'Print Screen' key. Also some keyboards cannot handle so many keys being pressed at the same time, so you might have better luck with press :kbd:`Alt`, press :kbd:`SysRq`, release :kbd:`SysRq`, press :kbd:`<command key>`, release everything.

SPARC에서는 alt 키 + stop 키 + 명령어 키를 누르면 될것이라 본다.
On SPARC - You press :kbd:`ALT-STOP-<command key>`, I believe.

시리얼 콘솔에서는(pc style의 표준 시리얼 포트)
break를 보내고, 5초안에 명령어 키를 누르면 된다. break를 두번 보내는 것은 일반적인 break로 해석된다.
On the serial console (PC style standard serial ports only)
You send a ``BREAK``, then within 5 seconds a command key. Sending ``BREAK`` twice is interpreted as a normal BREAK.

On PowerPC
Press :kbd:`ALT - Print Screen` (or :kbd:`F13`) - :kbd:`<command key>`, :kbd:`Print Screen` (or :kbd:`F13`) - :kbd:`<command key>` may suffice.

On other
If you know of the key combos for other architectures, please let me know so I can add them to this section.

모든 아키텍쳐에서 허용되는 방법으로 문자를 /proc/sysrq-trigger에 아래와 같이 쓰는 방법이 있다.
On all write a character to /proc/sysrq-trigger.  e.g.::

		echo t > /proc/sysrq-trigger

command 키란 무엇인가?
What are the 'command' keys?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

=========== ===================================================================
Command	    Function
=========== ===================================================================
``b``	    fs sync, umount를 하지 않고 시스템리붓을 즉시 수행함.
            Will immediately reboot the system without syncing or unmounting your disks.

``c``	    NULL 포인터 접근을 통해 시스템크래시를 수행함.
            Will perform a system crash by a NULL pointer dereference.  A crashdump will be taken if configured.

``d``	    현재 잡은 모든 락 정보를 보여준다.
            Shows all locks that are held.

``e``	    init을 제외한 모든 프로세스들에게 SIGTERM 시그널을 전달한다.
            Send a SIGTERM to all processes, except for init.

``f``	    oom-killer를 호출해서 메모리를 많이 사용하는 프로세스를 죽인다. 죽일 수 없는 프로세스가 없어도 패닉을 발생시키지는 않는다.
            Will call the oom killer to kill a memory hog process, but do not panic if nothing can be killed.

``g``	    kgdb에 의해 쓰인다
            Used by kgdb (kernel debugger)

``h``	    help message를 출력함(실제로 여기에 나열된 것 이외의 다른 키를 사용하면 h와 같이  도움을 표시합니다.)
            Will display help (actually any other key than those listed here will display help. but ``h`` is easy to remember :-)

``i``	    init을 제외한 모든 프로세스들에게 SIGKILL 시그널을 전달한다.
            Send a SIGKILL to all processes, except for init.

``j``	    강제로 FIFREEZE ioctl에 의해 frozen 상태가 된 파일시스템들을 해동한다.
            Forcibly "Just thaw it" - filesystems frozen by the FIFREEZE ioctl.

``k``	    Secure Access Key (SAK) Kills all programs on the current virtual console. NOTE: See important comments below in SAK section.

``l``	    모든 active cpu의 stack backtrace를 출력한다.
            Shows a stack backtrace for all active CPUs.

``m``	    콘솔에 현재 메모리 상태를 출력한다.
            Will dump current memory info to your console.

``n``	    RT 태스크들에 nice값 설정이 가능케 한다.
            Used to make RT tasks nice-able

``o``	    system off를 수행한다.
            Will shut your system off (if configured and supported).

``p``	    콘솔에 현재 레지스터와 플래그를 출력한다.
            Will dump the current registers and flags to your console.

``q``	    armed hrtimer의 CPU별 리스트를 출력한다.
            Will dump per CPU lists of all armed hrtimers (but NOT regular timer_list timers) and detailed information about all clockevent devices.

``r``	    Turns off keyboard raw mode and sets it to XLATE.

``s``	    마운트된 모든 fs를 sync한다.
            Will attempt to sync all mounted filesystems.

``t``	    모든 current 태스크의 정보를 콘솔에 출력한다.
            Will dump a list of current tasks and their information to your console.

``u``	    마운트된 모든 fs를 read-only로 remount한다.
            Will attempt to remount all mounted filesystems read-only.

``v``	    Forcefully restores framebuffer console
``v``	    Causes ETM buffer dump [ARM-specific]

``w``	    uninterruptable 상태의 태스크 정보를 출력한다.
            Dumps tasks that are in uninterruptable (blocked) state.

``x``	    Used by xmon interface on ppc/powerpc platforms.  Show global PMU Registers on sparc64.  Dump all TLB entries on MIPS.

``y``	    Show global CPU Registers [SPARC-64 specific]

``z``	    ftrace buffer에 저장된 이벤트를 커널로그로 출력한다.
            Dump the ftrace buffer

``0``-``9`` 0-9는 콘솔 로그 레벨을 설정해서 콘솔에 출력되는 로그를 제어한다.(0의 경우는 emergency message이를테면 패닉이나 웁스만 출력함)
            Sets the console log level, controlling which kernel messages will be printed to your console. (``0``, for example would make it so that only emergency messages like PANICs or OOPSes would make it to your console.)
=========== ===================================================================

좋아, 이 명령어를 어떤 용도로 쓰면 될까?
Okay, so what can I use them for?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Well, unraw(r) is very handy when your X server or a svgalib program crashes.

sak(k) (Secure Access Key) is useful when you want to be sure there is no trojan program running at console which could grab your password when you would try to login. It will kill all programs on given console, thus letting you make sure that the login prompt you see is actually the one from init, not some trojan program.

.. important::

In its true form it is not a true SAK like the one in a c2 compliant system, and it should not be mistaken as such.

It seems others find it useful as (System Attention Key) which is useful when you want to exit a program that will not let you switch consoles.
(For example, X or a svgalib program.)

shutdown이 불가능할 때 ``reboot(b)`` 를 사용하면 좋다. 하지만 ``sync(s)`` 와 ``umount(u)`` 를 먼저 실행해야 한다.
``reboot(b)`` is good when you're unable to shut down. But you should also ``sync(s)`` and ``umount(u)`` first.

``crash(c)`` 는 시스템이 hung되었을때 crashdump를 수동으로 트리거하는데 사용할 수 있다.  dump 메카니즘이 불가능하다면 crash만 트리거한다.
``crash(c)`` can be used to manually trigger a crashdump when the system is hung.  Note that this just triggers a crash if there is no dump mechanism available.

``sync(s)`` 는 시스템이 락업에 빠졌을때 사용하면 좋다, 시스템의 모든 디스크를 sync하도록 하고 특히 데이터손실이나 fscking이 필요한 상황이 줄어든다. 화면에 Done이나 OK를 볼때까지는 sync가 제대로 된게 아니라는걸 유의하자.(커널이 만약 정말 in strife, OK나 Done 메세지를 못볼 수도 있다.)
``sync(s)`` is great when your system is locked up, it allows you to sync your disks and will certainly lessen the chance of data loss and fscking. Note that the sync hasn't taken place until you see the "OK" and "Done" appear on the screen. (If the kernel is really in strife, you may not ever get the OK or Done message...)

``umount(u)`` 도 기본적으로 ``sync(s)`` 처럼 유용하다. 나는 일반적으로 시스템이 락업에 빠지면 ``sync(s)`` , ``umount(u)`` 그리고 ``reboot(b)`` 를 사용한다. 이로 인해 많은 fsck에서 나를 구했다. 다시한번 말하지만, umount시에 OK나 Done 메시지가 출력될때까지 umount가 제대로 된게 아니라는걸 유의하자.
``umount(u)`` is basically useful in the same ways as ``sync(s)``. I generally ``sync(s)``, ``umount(u)``, then ``reboot(b)`` when my system locks. It's saved me many a fsck. Again, the unmount (remount read-only) hasn't taken place until you see the "OK" and "Done" message appear on the screen.

로그레벨 0-9은 보고싶지 않은 커널로그가 콘솔에 떠다닐때 유용하다. 0을 선택하는 것은 긴급한 커널 메세지를 제외한 모든 로그가 콘솔에 출력되는 것을 방지한다.(하지만 여전히 syslogd/klogd가 존재한다면 여전히 거기에는 로깅된다)
The loglevels ``0``-``9`` are useful when your console is being flooded with kernel messages you do not want to see. Selecting ``0`` will prevent all but the most urgent kernel messages from reaching your console. (They will still be logged if syslogd/klogd are alive, though.)

``term(e)`` 나 ``kill(i)`` 는 어떤방법으로 프로세스를 죽일 수 없을때 유용하다, 특히 해당 프로세스가 다른 프로세스를 spawn할때 유용하다.
``term(e)`` and ``kill(i)`` are useful if you have some sort of runaway process you are unable to kill any other way, especially if it's spawning other processes.

``just thaw it(j)는 FIFREEZE ioctl에 의해 파일시스템이 frozen 상태가 된 것때문에 응답이 없을때 유용하다.
"``just thaw it(j)``" is useful if your system becomes unresponsive due to a frozen (probably root) filesystem via the FIFREEZE ioctl.

때때로 sysrq가 명령어를 사용한 후에 stuck 상태에 빠진 것처럼 보일때 무엇을 해야할까?
Sometimes SysRq seems to get 'stuck' after using it, what can I do?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

That happens to me, also. I've found that tapping shift, alt, and control on both sides of the keyboard, and hitting an invalid sysrq sequence again will fix the problem. (i.e., something like :kbd:`alt-sysrq-z`). Switching to another virtual console (:kbd:`ALT+Fn`) and then back again should also help.

I hit SysRq, but nothing seems to happen, what's wrong?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are some keyboards that produce a different keycode for SysRq than the pre-defined value of 99 (see ``KEY_SYSRQ`` in ``include/uapi/linux/input-event-codes.h``), or which don't have a SysRq key at all. In these cases, run ``showkey -s`` to find an appropriate scancode sequence, and use ``setkeycodes <sequence> 99`` to map this sequence to the usual SysRq code (e.g., ``setkeycodes e05b 99``). It's probably best to put this command in a boot script. Oh, and by the way, you exit ``showkey`` by not typing anything for ten seconds.

I want to add SysRQ key events to a module, how does it work?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to register a basic function with the table, you must first include the header ``include/linux/sysrq.h``, this will define everything else you need.  Next, you must create a ``sysrq_key_op`` struct, and populate it with A) the key handler function you will use, B) a help_msg string, that will print when SysRQ prints help, and C) an action_msg string, that will print right before your handler is called. Your handler must conform to the prototype in 'sysrq.h'.

After the ``sysrq_key_op`` is created, you can call the kernel function ``register_sysrq_key(int key, struct sysrq_key_op *op_p);`` this will register the operation pointed to by ``op_p`` at table key 'key', if that slot in the table is blank. At module unload time, you must call the function ``unregister_sysrq_key(int key, struct sysrq_key_op *op_p)``, which will remove the key op pointed to by 'op_p' from the key 'key', if and only if it is currently registered in that slot. This is in case the slot has been overwritten since you registered it.

The Magic SysRQ system works by registering key operations against a key op lookup table, which is defined in 'drivers/tty/sysrq.c'. This key table has a number of operations registered into it at compile time, but is mutable, and 2 functions are exported for interface to it::

	register_sysrq_key and unregister_sysrq_key.

Of course, never ever leave an invalid pointer in the table. I.e., when your module that called register_sysrq_key() exits, it must call unregister_sysrq_key() to clean up the sysrq key table entry that it used.  Null pointers in the table are always safe. :)

If for some reason you feel the need to call the handle_sysrq function from within a function called by handle_sysrq, you must be aware that you are in a lock (you are also in an interrupt handler, which means don't sleep!), so you must call ``__handle_sysrq_nolock`` instead.

When I hit a SysRq key combination only the header appears on the console?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sysrq output is subject to the same console loglevel control as all other console output.  This means that if the kernel was booted 'quiet' as is common on distro kernels the output may not appear on the actual console, even though it will appear in the dmesg buffer, and be accessible via the dmesg command and to the consumers of ``/proc/kmsg``.  As a specific exception the header line from the sysrq command is passed to all console consumers as if the current loglevel was maximum.  If only the header is emitted it is almost certain that the kernel loglevel is too low.  Should you require the output on the console channel then you will need to temporarily up the console loglevel using :kbd:`alt-sysrq-8` or::

    echo 8 > /proc/sysrq-trigger

Remember to return the loglevel to normal after triggering the sysrq command you are interested in.

I have more questions, who can I ask?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Just ask them on the linux-kernel mailing list:
	linux-kernel@vger.kernel.org

Credits
~~~~~~~

Written by Mydraal <vulpyne@vulpyne.net>
Updated by Adam Sulmicki <adam@cfar.umd.edu>
Updated by Jeremy M. Dolan <jmd@turbogeek.org> 2001/01/28 10:15:59
Added to by Crutcher Dunnavant <crutcher+kernel@datastacks.com>
