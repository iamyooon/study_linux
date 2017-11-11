#!/bin/bash

echo "1)Build example"
make

echo "2)Run without argument"
./enable_disable_printf_when_startup_time
echo ""

echo "3)Run with argument to disable printf"
./enable_disable_printf_when_startup_time 1
echo ""
