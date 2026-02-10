#!/usr/bin/env python3
"""Extract embedded RFC 6234 source files into reference/."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


FILE_HEADER_RE = re.compile(r"^/\*+\s*([A-Za-z0-9_.-]+\.(?:c|h))\s*\*+/$")
SECTION_TITLE_RE = re.compile(r"^\d+(?:\.\d+)+\.\s{2,}\S")
FOOTER_RE = re.compile(r"^Eastlake & Hansen\s+Informational\s+\[Page\s+\d+\]$")
HEADER_RE = re.compile(r"^RFC 6234\s+SHAs, HMAC-SHAs, and HKDF\s+May 2011$")


def remove_print_artifacts(lines: list[str]) -> tuple[list[str], int]:
    """Remove the 9-line printed page artifact block from code."""
    cleaned: list[str] = []
    removed_blocks = 0
    i = 0
    while i < len(lines):
        if FOOTER_RE.match(lines[i]) is not None:
            consume = 0
            max_header_idx = min(i + 3, len(lines) - 3)
            for header_idx in range(i + 1, max_header_idx + 1):
                between = lines[i + 1 : header_idx]
                if (
                    HEADER_RE.match(lines[header_idx]) is not None
                    and all(x in {"", "\f"} for x in between)
                    and lines[header_idx + 1] == ""
                    and lines[header_idx + 2] == ""
                ):
                    consume = (header_idx + 3) - i
                    break
            if consume:
                # The print block includes the four blank lines before the footer.
                for _ in range(4):
                    if cleaned and cleaned[-1] == "":
                        cleaned.pop()
                removed_blocks += 1
                i += consume
                continue
        cleaned.append(lines[i])
        i += 1
    return cleaned, removed_blocks


def find_last_blank_before(lines: list[str], heading_index: int, start_index: int) -> int:
    """Return the index of the last blank line before heading_index."""
    idx = heading_index - 1
    while idx >= start_index:
        if lines[idx] == "":
            return idx
        idx -= 1
    return heading_index - 1


def extract_files(rfc_text: str) -> dict[str, str]:
    # Keep form-feed lines visible so the print artifact detector can remove them.
    lines = [line.rstrip("\r") for line in rfc_text.split("\n")]

    section_indices = [i for i, line in enumerate(lines) if SECTION_TITLE_RE.match(line)]
    file_starts: list[tuple[str, int]] = []
    for i, line in enumerate(lines):
        match = FILE_HEADER_RE.match(line)
        if match:
            file_starts.append((match.group(1), i))

    if not file_starts:
        raise RuntimeError("No embedded source file headers found in RFC.")

    extracted: dict[str, str] = {}
    total_removed = 0
    for idx, (filename, start) in enumerate(file_starts):
        next_start = file_starts[idx + 1][1] if idx + 1 < len(file_starts) else len(lines)
        next_heading = next(
            (s for s in section_indices if s > start and s <= next_start),
            next_start,
        )
        end = find_last_blank_before(lines, next_heading, start)
        chunk = lines[start : end + 1]
        cleaned, removed = remove_print_artifacts(chunk)
        total_removed += removed
        content = "\n".join(cleaned) + "\n"
        if not content.startswith(lines[start] + "\n"):
            raise RuntimeError(f"Extracted file does not start at header line: {filename}")
        extracted[filename] = content

    # Validation to ensure page artifacts are gone.
    for name, content in extracted.items():
        if "\f" in content:
            raise RuntimeError(f"Form feed remains in extracted file: {name}")
        for line in content.splitlines():
            if FOOTER_RE.match(line) or HEADER_RE.match(line):
                raise RuntimeError(f"Page header/footer remains in extracted file: {name}")

    if total_removed == 0:
        raise RuntimeError("No print artifact blocks were removed; parser likely incorrect.")

    return extracted


def write_files(files: dict[str, str], output_dir: Path) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    for filename, content in files.items():
        (output_dir / filename).write_text(content, encoding="utf-8")


def compare_shared_files(reference_dir: Path, repo_root: Path) -> list[str]:
    """Check extracted files against existing include/src copies when present."""
    mismatches: list[str] = []
    for name in sorted(p.name for p in reference_dir.iterdir() if p.is_file()):
        target = None
        if (repo_root / "include" / name).exists():
            target = repo_root / "include" / name
        elif (repo_root / "src" / name).exists():
            target = repo_root / "src" / name
        if target is None:
            continue
        extracted = (reference_dir / name).read_text(encoding="utf-8")
        existing = target.read_text(encoding="utf-8")
        if extracted != existing:
            mismatches.append(name)
    return mismatches


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--input",
        type=Path,
        default=Path("../reference/rfc6234.txt"),
        help="Path to RFC text input",
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=Path("../reference"),
        help="Directory for extracted files",
    )
    parser.add_argument("--check", action="store_true", help="Run extraction requirement checks")
    parser.add_argument(
        "--compare-existing",
        action="store_true",
        help="Additionally compare shared files against include/src copies",
    )
    args = parser.parse_args()

    text = args.input.read_text(encoding="utf-8")
    files = extract_files(text)
    write_files(files, args.output_dir)

    print(f"Extracted {len(files)} files to {args.output_dir}")
    for name in sorted(files):
        print(f" - {name}")

    if args.check:
        for name, content in files.items():
            lines = content.splitlines()
            if not lines:
                raise SystemExit(f"Validation failed: extracted file is empty: {name}")
            if not content.endswith("\n\n"):
                raise SystemExit(
                    f"Validation failed: extracted file does not end on blank line: {name}"
                )
        print("Validation passed: extraction requirements satisfied.")

    if args.compare_existing:
        mismatches = compare_shared_files(args.output_dir, Path("../"))
        if mismatches:
            raise SystemExit(
                "Validation failed: extracted files differ from repository copies: "
                + ", ".join(mismatches)
            )
        print("Validation passed: shared files match include/src copies.")


if __name__ == "__main__":
    main()
