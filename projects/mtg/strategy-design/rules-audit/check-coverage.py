#!/usr/bin/env python3
"""Coverage checker for the rules-conformance audit.
Usage: check-coverage.py <section-file.txt> <report.md>
Done means: exit 0 and 'MISSING: 0'. Every rule id present in the section file
must appear in the report (as a plain token like 601.2c). Prints missing ids."""
import re, sys
sec = open(sys.argv[1], encoding='utf-8').read()
rep = open(sys.argv[2], encoding='utf-8').read()
want = set(m.group(1) for m in re.finditer(r'^(\d{3}\.\d+[a-z]?)', sec, re.M))
have = set(re.findall(r'\b(\d{3}\.\d+[a-z]?)\b', rep))
missing = sorted(want - have)
print(f"section ids: {len(want)}  covered: {len(want)-len(missing)}  MISSING: {len(missing)}")
for m_ in missing[:50]:
    print("  missing:", m_)
sys.exit(0 if not missing else 1)
