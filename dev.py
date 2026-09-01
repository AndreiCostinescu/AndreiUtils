#!/usr/bin/env python3
# dev.py

import subprocess
import sys

subprocess.check_call(["git", "submodule", "update", "--init", "--recursive"])
sys.exit(subprocess.call([sys.executable, ".devtools/scripts/devtools.py"] + sys.argv[1:]))
