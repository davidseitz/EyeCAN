#!/bin/bash
#usage: source setup.sh
if [ ! -d "code/venv/bin/activate" ]; then
    python3 -m venv code/venv
fi
source code/venv/bin/activate