# Phase 2 – Revert to embedded n2n 2.6.x baseline

What is included
- Reverted any attempted upgrade to n2n 3.x and ensured the repository uses the embedded n2n 2.6.x sources and headers.
- Verified OmniEdge.pro references n2n sources in a way compatible with 2.6.x API.
- Confirmed the n2n directory contents resemble a 2.6.x style (no n2n3.x artifacts present).

Rationale
- Ensures stability and compatibility with the current Qt6/C++17 baseline by sticking to the previously verified n2n version embedded in the project.

Validation plan
- Build using qmake OmniEdge.pro and MSVC 2022 on Windows.
- Run packaging/pack_win.ps1 to generate the ZIP; optionally run NSIS installer in CI.
- Verify VPN-related code paths compile and link correctly with the embedded n2n2 API.

Next steps
- If you’d like, I can add an explicit test harness to CI to validate VPN initialization against the embedded n2n2 codepath.
