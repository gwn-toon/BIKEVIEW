#!/usr/bin/env python3
"""Serve the Fietsview site locally with MP4-friendly byte ranges."""

from __future__ import annotations

import argparse
import os
import re
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parent.parent
RANGE_PATTERN = re.compile(r"bytes=(\d*)-(\d*)$")


class RangeRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self) -> None:
        self.send_header("Accept-Ranges", "bytes")
        super().end_headers()

    def send_head(self):  # noqa: ANN001 - signature follows SimpleHTTPRequestHandler
        path = self.translate_path(self.path)
        if os.path.isdir(path):
            for index in ("index.html", "index.htm"):
                index_path = os.path.join(path, index)
                if os.path.exists(index_path):
                    path = index_path
                    break
            else:
                return self.list_directory(path)

        ctype = self.guess_type(path)
        try:
            file_handle = open(path, "rb")
        except OSError:
            self.send_error(404, "File not found")
            return None

        file_size = os.fstat(file_handle.fileno()).st_size
        range_header = self.headers.get("Range", "")
        match = RANGE_PATTERN.match(range_header)
        if not match:
            self._range = None
            self.send_response(200)
            self.send_header("Content-type", ctype)
            self.send_header("Content-Length", str(file_size))
            self.send_header("Last-Modified", self.date_time_string(os.fstat(file_handle.fileno()).st_mtime))
            self.end_headers()
            return file_handle

        start_text, end_text = match.groups()
        if start_text:
            start = int(start_text)
            end = int(end_text) if end_text else file_size - 1
        else:
            suffix_length = int(end_text)
            start = max(file_size - suffix_length, 0)
            end = file_size - 1

        if start >= file_size or end < start:
            file_handle.close()
            self.send_error(416, "Requested Range Not Satisfiable")
            return None

        end = min(end, file_size - 1)
        self._range = (start, end)
        file_handle.seek(start)
        self.send_response(206)
        self.send_header("Content-type", ctype)
        self.send_header("Content-Range", f"bytes {start}-{end}/{file_size}")
        self.send_header("Content-Length", str(end - start + 1))
        self.send_header("Last-Modified", self.date_time_string(os.fstat(file_handle.fileno()).st_mtime))
        self.end_headers()
        return file_handle

    def copyfile(self, source, outputfile) -> None:  # noqa: ANN001 - signature follows SimpleHTTPRequestHandler
        byte_range = getattr(self, "_range", None)
        if not byte_range:
            return super().copyfile(source, outputfile)

        start, end = byte_range
        remaining = end - start + 1
        while remaining > 0:
            chunk = source.read(min(64 * 1024, remaining))
            if not chunk:
                break
            outputfile.write(chunk)
            remaining -= len(chunk)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8080)
    parser.add_argument("--root", type=Path, default=REPO_ROOT)
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    root = args.root.resolve()
    handler = partial(RangeRequestHandler, directory=str(root))
    server = ThreadingHTTPServer((args.host, args.port), handler)
    print(f"Serving {root} at http://{args.host}:{args.port}/")
    print(f"Open http://{args.host}:{args.port}/fietsview-finish.html")
    print(f"Controller monitor: http://{args.host}:{args.port}/arcade/joystick.html")
    server.serve_forever()


if __name__ == "__main__":
    main()
