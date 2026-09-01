#!/usr/bin/env python3
# Copyright 2026 DevTools Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Bootstraps DevTools into .devtools/ (no git submodule required) and execs
into its devtools.py with this script's own argv. Downloads a pinned
snapshot of https://github.com/AndreiCostinescu/DevTools from GitHub's
archive endpoint the first time it's needed, or whenever DEVTOOLS_REV below
is bumped. .devtools/ is disposable and stays out of version control
('setup'/'sync-devtools' add it to .gitignore automatically).
"""

import io
import json
import os
import shutil
import subprocess
import sys
import tarfile
import urllib.error
import urllib.request

# Pin to a commit SHA or tag from https://github.com/AndreiCostinescu/DevTools/commits/main,
# or set to "latest" to always track the current tip of main (convenient for
# local use, but not reproducible - a CI pipeline should pin a real SHA/tag).
# Bump this to pick up DevTools changes - nothing here is pulled automatically.
DEVTOOLS_REV = "latest"
DEVTOOLS_REPO = "AndreiCostinescu/DevTools"
DEVTOOLS_DIR = ".devtools"
REV_MARKER = os.path.join(DEVTOOLS_DIR, ".rev")
# Only these are needed to run devtools.py; hooks/ etc. are fetched
# separately by pre-commit itself, straight from the pinned repo.
WANTED_PREFIXES = ("scripts/", "config/")


def fail(message):
    sys.stderr.write("ERROR: {}\n".format(message))
    sys.exit(1)


def devtools_current(rev):
    if not os.path.isfile(REV_MARKER):
        return False
    with io.open(REV_MARKER, "r", encoding="utf-8") as f:
        return f.read().strip() == rev


def resolve_rev():
    """Return the concrete commit/tag to fetch, validating it first.

    'latest' resolves to the current tip of main via GitHub's API, so it's
    checked - and may resolve to something new - on every run. Any other
    value is a pin the caller already has cached-or-not (see the fast path
    below); it's confirmed to actually exist via the same API before the
    (much larger) archive download is attempted, so a bad DEVTOOLS_REV
    (still the placeholder, a typo, a SHA from the wrong repo, ...) fails
    with a clear message instead of a confusing 404 partway through it.
    """
    ref = "main" if DEVTOOLS_REV == "latest" else DEVTOOLS_REV
    api_url = "https://api.github.com/repos/{}/commits/{}".format(DEVTOOLS_REPO, ref)
    try:
        with urllib.request.urlopen(api_url) as response:
            sha = json.loads(response.read().decode("utf-8"))["sha"]
    except urllib.error.HTTPError as exc:
        if exc.code in (404, 422):
            # 404: no such commit/tag. 422: GitHub rejects the ref outright
            # as malformed (e.g. the still-unfilled placeholder value).
            fail(
                "DEVTOOLS_REV '{}' is not 'latest' and no such commit/tag exists in "
                "{}. Pin it to a real commit SHA (or tag) - see "
                "https://github.com/{}/commits/main.".format(
                    DEVTOOLS_REV, DEVTOOLS_REPO, DEVTOOLS_REPO
                )
            )
        if exc.code == 403:
            fail(
                "GitHub API request failed (HTTP 403), likely the anonymous rate "
                "limit - wait a while and retry. Pinning DEVTOOLS_REV to a fixed "
                "SHA/tag (instead of 'latest') avoids this on every later run, "
                "since an already-fetched pin skips this check entirely."
            )
        raise
    return sha if DEVTOOLS_REV == "latest" else DEVTOOLS_REV


def extract_member(tar, member, dest):
    try:
        # 'filter' (Python 3.12+) enforces the same path/type checks we
        # already do by hand above; passing it avoids a DeprecationWarning
        # on 3.12-3.13 and matches the default 3.14 will switch to.
        tar.extract(member, dest, filter="data")
    except TypeError:
        tar.extract(member, dest)  # Python < 3.12: no 'filter' kwarg


def download_devtools(rev):
    url = "https://github.com/{}/archive/{}.tar.gz".format(DEVTOOLS_REPO, rev)
    print("Fetching DevTools @ {} ...".format(rev))
    with urllib.request.urlopen(url) as response:
        archive = response.read()

    # Rebuilt from scratch on every fetch so a file removed upstream can't
    # linger locally as a stale leftover from an older revision.
    shutil.rmtree(DEVTOOLS_DIR, ignore_errors=True)
    with tarfile.open(fileobj=io.BytesIO(archive), mode="r:gz") as tar:
        members = tar.getmembers()
        prefix = members[0].name.split("/", 1)[0] + "/"  # e.g. "DevTools-<sha>/"
        for member in members:
            if not member.name.startswith(prefix):
                continue
            rel = member.name[len(prefix) :]
            if not rel.startswith(WANTED_PREFIXES):
                continue
            if ".." in rel.split("/"):
                continue  # defensive: never extract outside DEVTOOLS_DIR
            if not (member.isreg() or member.isdir()):
                continue  # skip symlinks/devices/etc.
            member.name = rel
            extract_member(tar, member, DEVTOOLS_DIR)

    with io.open(REV_MARKER, "w", encoding="utf-8") as f:
        f.write(rev)


# Fast path: an already-fetched concrete pin needs no network at all.
# Only "latest" (always) or an as-yet-unfetched/changed DEVTOOLS_REV (once) goes through resolve_rev()'s validation.
if DEVTOOLS_REV != "latest" and devtools_current(DEVTOOLS_REV):
    resolved_rev = DEVTOOLS_REV
else:
    resolved_rev = resolve_rev()
    if not devtools_current(resolved_rev):
        download_devtools(resolved_rev)

sys.exit(
    subprocess.call(
        [sys.executable, os.path.join(DEVTOOLS_DIR, "scripts", "devtools.py")]
        + sys.argv[1:]
    )
)
