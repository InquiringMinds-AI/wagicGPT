#!/usr/bin/env python3
"""Audit the item-level CardScript lexicon coverage ledger.

This is deliberately a structural audit. It verifies that a claimed behavioral
row is registered, has an executable [DO] section and an assertion section, and
corresponds to an item in the authoritative lexicon. It does not replace
running the gameplay suite or judging whether an assertion is strong enough.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Optional


BASIC_ROW = re.compile(r"^\|([0-9]+)\|`([^`]+)`")
MACRO_ROW = re.compile(r"^#AUTO_DEFINE\s+(\S+)")


def read_basic_items(doc: Path) -> list[str]:
    items = []
    for line in doc.read_text(encoding="utf-8").splitlines():
        match = BASIC_ROW.match(line)
        if match:
            items.append(match.group(2))
    return items


def read_macro_items(path: Path) -> list[str]:
    return [
        match.group(1)
        for line in path.read_text(encoding="utf-8").splitlines()
        if (match := MACRO_ROW.match(line))
    ]


def read_registry(path: Path) -> set[str]:
    return {
        line.strip()
        for line in path.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    }


def read_ledger(path: Path) -> list[tuple[str, str, str, str, str]]:
    rows = []
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        if not line.strip() or line.startswith("#") or line.startswith("section\t"):
            continue
        fields = line.split("\t")
        if len(fields) != 5:
            raise ValueError(f"{path}:{number}: expected five tab-separated fields")
        rows.append(tuple(fields))
    return rows


def section_body(text: str, name: str) -> Optional[str]:
    """Return a test section body, preserving the suite's line-oriented syntax."""
    match = re.search(rf"(?im)^\[{re.escape(name)}\]\s*$", text)
    if not match:
        return None
    next_section = re.search(r"(?im)^\[[^\]]+\]\s*$", text[match.end():])
    end = match.end() + next_section.start() if next_section else len(text)
    return text[match.end():end]


def has_executable_lines(body: str | None) -> bool:
    if body is None:
        return False
    return any(line.strip() and not line.lstrip().startswith("#") for line in body.splitlines())


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--strict", action="store_true", help="fail if any row is gap or blocked")
    args = parser.parse_args()

    repo = Path(__file__).resolve().parents[1]
    doc = repo / "docs/card-script-lexicon.md"
    macros_path = repo / "projects/mtg/bin/Res/sets/primitives/_macros.txt"
    ledger_path = repo / "docs/card-script-lexicon-coverage.tsv"
    test_root = repo / "projects/mtg/bin/Res/test"
    registry_path = test_root / "lexicon/_tests.txt"

    basics = read_basic_items(doc)
    rows = read_ledger(ledger_path)
    registry = read_registry(registry_path)
    errors = []

    keys = [(section, item) for section, item, *_ in rows]
    for key in sorted({key for key in keys if keys.count(key) > 1}):
        errors.append(f"duplicate ledger row: {key[0]}:{key[1]}")

    basic_rows = {item: (fixture, assertion, status) for section, item, fixture, assertion, status in rows if section == "basic"}
    missing_basics = [item for item in basics if item not in basic_rows]
    extra_basics = sorted(set(basic_rows) - set(basics))
    errors.extend(f"missing basic item: {item}" for item in missing_basics)
    errors.extend(f"unknown basic item: {item}" for item in extra_basics)

    macro_items = read_macro_items(macros_path)
    macro_rows = {item: (fixture, assertion, status) for section, item, fixture, assertion, status in rows if section == "macro"}
    missing_macros = [item for item in macro_items if item not in macro_rows]
    extra_macros = sorted(set(macro_rows) - set(macro_items))
    errors.extend(f"missing macro item: {item}" for item in missing_macros)
    errors.extend(f"unknown macro item: {item}" for item in extra_macros)

    for section, item, fixture, assertion, status in rows:
        if not fixture:
            if status in {"covered", "covered-unverified"}:
                errors.append(f"claimed coverage has no fixture: {section}:{item}")
            continue
        if fixture not in registry:
            errors.append(f"fixture is not registered: {section}:{item}: {fixture}")
            continue
        fixture_path = test_root / fixture
        if not fixture_path.exists():
            errors.append(f"registered fixture is missing: {fixture}")
            continue
        fixture_text = fixture_path.read_text(encoding="utf-8")
        if not has_executable_lines(section_body(fixture_text, "DO")):
            errors.append(f"fixture has no executable [DO] section: {fixture}")
        if section_body(fixture_text, "ASSERT") is None:
            errors.append(f"fixture has no assertion section: {fixture}")
        if not assertion.strip():
            errors.append(f"behavior row has no observable assertion: {section}:{item}")

    counts = {}
    for section, item, fixture, assertion, status in rows:
        counts[status] = counts.get(status, 0) + 1
    print(f"basic_items={len(basics)} basic_rows={len(basic_rows)} macro_items={len(macro_items)} macro_rows={len(macro_rows)} total_rows={len(rows)}")
    print("statuses=" + ", ".join(f"{key}:{counts[key]}" for key in sorted(counts)))
    if errors:
        print("errors:")
        print("\n".join(f"- {error}" for error in errors))
    if args.strict and any(status in {"gap", "blocked-harness"} for *_, status in rows):
        errors.append("strict coverage audit found gap or blocked-harness rows")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
