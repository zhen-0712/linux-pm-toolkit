import sys
import os

# allow `from collector import ...` without installing as a package
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "..", "src", "dashboard"))
