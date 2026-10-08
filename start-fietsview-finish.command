#!/bin/zsh
cd "$(dirname "$0")"

URL="http://127.0.0.1:8080/fietsview-finish.html"

if ! command -v python3 >/dev/null 2>&1; then
  echo "python3 was not found. Install Python 3, then run this file again."
  read -r "?Press return to close."
  exit 1
fi

echo "Starting Fietsview Finish at $URL"
(sleep 1 && open "$URL") &
python3 scripts/serve_fietsview.py --host 127.0.0.1 --port 8080
