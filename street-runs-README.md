# Street Runs

Use one street folder per corrected-map street, then one folder per canonical playable run inside it.

Examples:

- `street-runs/001-albrecht-rodenbachstraat/001-12/`
- `street-runs/001-albrecht-rodenbachstraat/001-21/`
- `street-runs/031a-fonteinstraat-a/031A-12/`
- `street-runs/031a-fonteinstraat-a/031A-21/`

Recommended files inside each run folder:

- `mapping.json`
- `video.mp4`
- `grid-analysis/`
- `grid-analysis/frame-manifest.json`
- `grid-analysis/video-prediction-manifest.json`
- `grid.mp4`
- `overlay.mp4`
- `poster.jpg`
- `notes.md`

Keep temporary or experimental clips outside the canonical run folders:

- `street-runs/_drafts/`

If you ever need to rebuild the full folder scaffold, run:

- `python3 scripts/build_street_run_library.py --scaffold-dirs`
